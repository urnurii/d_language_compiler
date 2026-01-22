#include "semantic_transforms.h"
#include "../ast_builder.h"
#include "type_inference.h"
#include <stdio.h>

static int NormalizeStaticArrayDecl(NType *decl_type, NInitDeclList *init_decls);
static int NormalizeArrayTypeInParams(NParamList *params);
static void NormalizeArrayType(NType *type);
static NType *CreateElementTypeFromArrayType(const NType *array_type);
static int ReplaceSliceWithFuncCall(NExpr *expr);
static int TransformForeachToFor(NStmt *stmt, SemanticContext *ctx);
static char *MakeTempName(const char *prefix, int id);
static int TransformSwitchToIfChain(NStmt *stmt, SemanticContext *ctx);
static void PurgeBreaksInStmtList(NStmtList *list, int loop_depth, int switch_depth);
static void PurgeBreaksInStmt(NStmt *stmt, int loop_depth, int switch_depth);

static int TransformSourceItems(NSourceItem *items, SemanticContext *ctx);
static int TransformStatement(NStmt *stmt, SemanticContext *ctx);
static int TransformStatementList(NStmt *stmts, SemanticContext *ctx);
static int TransformExpression(NExpr *expr, SemanticContext *ctx);

static int TransformAssignment(NExpr *expr, SemanticContext *ctx);
static int PrepareLValueRValue(NExpr *lhs, NExpr *rhs, SemanticContext *ctx);
static int TryInsertImplicitCast(NExpr **expr_ptr, const NType *target_type, SemanticContext *ctx);

int TransformProgram(NProgram *root, SemanticContext *ctx) {
    if (ctx == NULL) {
        return 1;
    }
    if (root == NULL) {
        return 0;
    }
    return TransformSourceItems(root->first_item, ctx);
}

static int TransformSourceItems(NSourceItem *items, SemanticContext *ctx) {
    NSourceItem *item = items;
    while (item != NULL) {
        switch (item->type) {
            case SOURCE_ITEM_FUNC:
                if (item->value.func) {
                    NormalizeArrayType(item->value.func->return_type);
                    NormalizeArrayTypeInParams(item->value.func->params);
                    if (item->value.func->body) {
                        TransformStatement(item->value.func->body, ctx);
                    }
                }
                break;
            case SOURCE_ITEM_CLASS: {
                NClassDef *class_def = item->value.class_def;
                NClassMember *member = class_def ? class_def->members.first : NULL;
                while (member != NULL) {
                    if (member->type == CLASS_MEMBER_FIELD) {
                        NormalizeStaticArrayDecl(member->value.field.field_type, member->value.field.init_decls);
                    } else if (member->type == CLASS_MEMBER_METHOD && member->value.method) {
                        NormalizeArrayType(member->value.method->return_type);
                        NormalizeArrayTypeInParams(member->value.method->params);
                        if (member->value.method->body) {
                            TransformStatement(member->value.method->body, ctx);
                        }
                    } else if (member->type == CLASS_MEMBER_CTOR && member->value.ctor) {
                        NormalizeArrayTypeInParams(member->value.ctor->params);
                        if (member->value.ctor->body) {
                            TransformStatement(member->value.ctor->body, ctx);
                        }
                    } else if (member->type == CLASS_MEMBER_DTOR && member->value.dtor && member->value.dtor->body) {
                        TransformStatement(member->value.dtor->body, ctx);
                    }
                    member = member->next;
                }
                break;
            }
            case SOURCE_ITEM_DECL:
                NormalizeStaticArrayDecl(item->value.decl.item_type, item->value.decl.init_decls);
                break;
            case SOURCE_ITEM_ENUM:
                break;
        }
        item = item->next;
    }
    return 0;
}

static int TransformStatementList(NStmt *stmts, SemanticContext *ctx) {
    NStmt *stmt = stmts;
    while (stmt != NULL) {
        TransformStatement(stmt, ctx);
        stmt = stmt->next;
    }
    return 0;
}

static int TransformStatement(NStmt *stmt, SemanticContext *ctx) {
    if (stmt == NULL) {
        return 0;
    }
    switch (stmt->type) {
        case STMT_EXPR:
        case STMT_RETURN:
            TransformExpression(stmt->value.expr, ctx);
            break;
        case STMT_DECL:
            NormalizeStaticArrayDecl(stmt->value.decl.decl_type, stmt->value.decl.init_decls);
            if (stmt->value.decl.init_decls) {
                for (int i = 0; i < stmt->value.decl.init_decls->count; i++) {
                    NInitDecl *decl = stmt->value.decl.init_decls->decls[i];
                    if (decl && decl->initializer && decl->initializer->expr) {
                        TransformExpression(decl->initializer->expr, ctx);
                    }
                }
            }
            break;
        case STMT_COMPOUND:
            TransformStatementList(stmt->value.stmt_list ? stmt->value.stmt_list->first : NULL, ctx);
            break;
        case STMT_IF:
            TransformExpression(stmt->value.if_stmt.condition, ctx);
            TransformStatement(stmt->value.if_stmt.then_stmt, ctx);
            TransformStatement(stmt->value.if_stmt.else_stmt, ctx);
            break;
        case STMT_WHILE:
            TransformExpression(stmt->value.while_stmt.condition, ctx);
            TransformStatement(stmt->value.while_stmt.body, ctx);
            break;
        case STMT_DO_WHILE:
            TransformStatement(stmt->value.do_while_stmt.body, ctx);
            TransformExpression(stmt->value.do_while_stmt.condition, ctx);
            break;
        case STMT_FOR:
            TransformExpression(stmt->value.for_stmt.init_expr, ctx);
            TransformExpression(stmt->value.for_stmt.cond_expr, ctx);
            TransformExpression(stmt->value.for_stmt.iter_expr, ctx);
            TransformStatement(stmt->value.for_stmt.body, ctx);
            if (stmt->value.for_stmt.init_decls) {
                NormalizeStaticArrayDecl(stmt->value.for_stmt.init_decl_type, stmt->value.for_stmt.init_decls);
                for (int i = 0; i < stmt->value.for_stmt.init_decls->count; i++) {
                    NInitDecl *decl = stmt->value.for_stmt.init_decls->decls[i];
                    if (decl && decl->initializer && decl->initializer->expr) {
                        TransformExpression(decl->initializer->expr, ctx);
                    }
                }
            }
            break;
        case STMT_FOREACH:
            if (TransformForeachToFor(stmt, ctx)) {
                TransformStatement(stmt, ctx);
                return 0;
            }
            TransformExpression(stmt->value.foreach_stmt.collection, ctx);
            TransformStatement(stmt->value.foreach_stmt.body, ctx);
            break;
        case STMT_SWITCH:
            if (TransformSwitchToIfChain(stmt, ctx)) {
                TransformStatement(stmt, ctx);
                return 0;
            }
            TransformExpression(stmt->value.switch_stmt.expr, ctx);
            for (int i = 0; i < stmt->value.switch_stmt.cases.count; i++) {
                NCaseItem *item = stmt->value.switch_stmt.cases.items[i];
                if (item && item->case_expr) {
                    TransformExpression(item->case_expr, ctx);
                }
                if (item && item->stmts) {
                    TransformStatementList(item->stmts->first, ctx);
                }
            }
            break;
        case STMT_BREAK:
        case STMT_CONTINUE:
            break;
    }
    return 0;
}

static int TransformExpression(NExpr *expr, SemanticContext *ctx) {
    if (expr == NULL) {
        return 0;
    }
    switch (expr->type) {
        case EXPR_PAREN:
            TransformExpression(expr->value.inner_expr, ctx);
            break;
        case EXPR_NEW:
            for (int i = 0; i < expr->value.new_expr.init_count; i++) {
                TransformExpression(expr->value.new_expr.init_exprs[i], ctx);
            }
            break;
        case EXPR_ARRAY_ACCESS:
            if (expr->value.array_access.index_end != NULL) {
                ReplaceSliceWithFuncCall(expr);
                TransformExpression(expr, ctx);
                return 0;
            }
            TransformExpression(expr->value.array_access.array, ctx);
            TransformExpression(expr->value.array_access.index, ctx);
            TransformExpression(expr->value.array_access.index_end, ctx);
            break;
        case EXPR_MEMBER_ACCESS:
            TransformExpression(expr->value.member_access.object, ctx);
            break;
        case EXPR_METHOD_CALL:
            TransformExpression(expr->value.member_access.object, ctx);
            for (int i = 0; i < expr->value.member_access.arg_count; i++) {
                TransformExpression(expr->value.member_access.args[i], ctx);
            }
            break;
        case EXPR_FUNC_CALL:
            for (int i = 0; i < expr->value.func_call.arg_count; i++) {
                TransformExpression(expr->value.func_call.args[i], ctx);
            }
            break;
        case EXPR_SUPER:
            break;
        case EXPR_SUPER_METHOD:
            for (int i = 0; i < expr->value.member_access.arg_count; i++) {
                TransformExpression(expr->value.member_access.args[i], ctx);
            }
            break;
        case EXPR_UNARY_OP:
            TransformExpression(expr->value.unary.operand, ctx);
            break;
        case EXPR_BINARY_OP:
            TransformExpression(expr->value.binary.left, ctx);
            TransformExpression(expr->value.binary.right, ctx);
            break;
        case EXPR_ASSIGN:
            TransformAssignment(expr, ctx);
            break;
        case EXPR_IDENT:
        case EXPR_INT:
        case EXPR_FLOAT:
        case EXPR_CHAR:
        case EXPR_STRING:
        case EXPR_BOOL:
        case EXPR_NULL:
        case EXPR_NAN:
        case EXPR_THIS:
            break;
    }
    return 0;
}

static void NormalizeArrayType(NType *type) {
    if (type == NULL) {
        return;
    }
    if ((type->kind == TYPE_KIND_BASE_ARRAY || type->kind == TYPE_KIND_CLASS_ARRAY) &&
        type->array_decl != NULL && type->array_decl->has_size) {
        type->array_decl->has_size = 0;
    }
}

static int NormalizeArrayTypeInParams(NParamList *params) {
    if (params == NULL) {
        return 0;
    }
    for (int i = 0; i < params->count; i++) {
        NParam *param = params->params[i];
        if (param && param->param_type) {
            NormalizeArrayType(param->param_type);
        }
    }
    return 0;
}

static NType *CreateElementTypeFromArrayType(const NType *array_type) {
    if (array_type == NULL) {
        return NULL;
    }
    if (array_type->kind == TYPE_KIND_BASE_ARRAY) {
        return CreateBaseType(array_type->base_type);
    }
    if (array_type->kind == TYPE_KIND_CLASS_ARRAY) {
        return CreateClassType(array_type->class_name);
    }
    return NULL;
}

static int NormalizeStaticArrayDecl(NType *decl_type, NInitDeclList *init_decls) {
    if (decl_type == NULL) {
        return 0;
    }
    if ((decl_type->kind != TYPE_KIND_BASE_ARRAY && decl_type->kind != TYPE_KIND_CLASS_ARRAY) ||
        decl_type->array_decl == NULL || !decl_type->array_decl->has_size) {
        NormalizeArrayType(decl_type);
        return 0;
    }

    if (init_decls != NULL) {
        for (int i = 0; i < init_decls->count; i++) {
            NInitDecl *decl = init_decls->decls[i];
            if (decl == NULL || decl->initializer != NULL) {
                continue;
            }
            {
                NExprList *list = CreateExprList();
                NExpr *size_expr = CreateIntExpr(decl_type->array_decl->size);
                NType *elem_type = CreateElementTypeFromArrayType(decl_type);
                AddExprToList(list, size_expr);
                {
                    NExpr *new_expr = CreateNewExpr(elem_type, list->elements, list->count);
                    decl->initializer = CreateExprInitializer(new_expr);
                }
            }
        }
    }

    NormalizeArrayType(decl_type);
    return 0;
}

static int ReplaceSliceWithFuncCall(NExpr *expr) {
    NExprList *list;
    NExpr *array_expr;
    NExpr *index_expr;
    NExpr *index_end_expr;

    if (expr == NULL || expr->type != EXPR_ARRAY_ACCESS) {
        return 0;
    }
    if (expr->value.array_access.index_end == NULL) {
        return 0;
    }

    array_expr = expr->value.array_access.array;
    index_expr = expr->value.array_access.index;
    index_end_expr = expr->value.array_access.index_end;

    list = CreateExprList();
    AddExprToList(list, array_expr);
    AddExprToList(list, index_expr);
    AddExprToList(list, index_end_expr);

    expr->type = EXPR_FUNC_CALL;
    expr->value.func_call.func_name = DuplicateString("__slice");
    expr->value.func_call.args = list->elements;
    expr->value.func_call.arg_count = list->count;
    return 1;
}

static char *MakeTempName(const char *prefix, int id) {
    char buf[64];
    snprintf(buf, sizeof(buf), "%s%d", prefix, id);
    return DuplicateString(buf);
}

static int TransformForeachToFor(NStmt *stmt, SemanticContext *ctx) {
    static int foreach_counter = 0;
    int local_id;
    NExpr *collection;
    NType *collection_type;
    NType *element_type;
    NType *array_decl_type;
    char *arr_name;
    char *idx_name;
    NExprList *len_args;
    NExpr *len_call;
    NExpr *idx_ident_cond;
    NExpr *arr_ident_index;
    NExpr *idx_ident_index;
    NExpr *element_access;
    NExpr *iter_expr;
    NExpr *cond_expr;
    NInitDeclList *arr_init_list;
    NInitDeclList *idx_init_list;
    NStmtList *outer_list;
    NStmtList *body_list;
    NStmt *arr_decl_stmt;
    NStmt *for_stmt;
    NStmt *var_decl_stmt;
    NStmt *body_stmt;
    NType *var_decl_type;
    NType *idx_decl_type;

    if (stmt == NULL || ctx == NULL || stmt->type != STMT_FOREACH) {
        return 0;
    }

    collection = stmt->value.foreach_stmt.collection;
    if (collection == NULL) {
        return 0;
    }

    collection_type = InferExpressionTypeSilent(collection, ctx);
    if (collection_type == NULL) {
        return 0;
    }
    if (collection_type->kind != TYPE_KIND_BASE_ARRAY &&
        collection_type->kind != TYPE_KIND_CLASS_ARRAY) {
        return 0;
    }

    element_type = stmt->value.foreach_stmt.is_typed
        ? stmt->value.foreach_stmt.var_type
        : CreateElementTypeFromArrayType(collection_type);
    if (element_type == NULL) {
        return 0;
    }

    local_id = foreach_counter++;
    arr_name = MakeTempName("__foreach_arr", local_id);
    idx_name = MakeTempName("__foreach_idx", local_id);

    array_decl_type = CopyTypeNoContext(collection_type);
    if (array_decl_type == NULL) {
        return 0;
    }
    NormalizeArrayType(array_decl_type);

    arr_init_list = CreateInitDeclList();
    AddInitDeclToList(arr_init_list, CreateInitDecl(arr_name, CreateExprInitializer(collection)));
    arr_decl_stmt = CreateDeclStmt(array_decl_type, arr_init_list);

    idx_decl_type = CreateBaseType(TYPE_INT);
    idx_init_list = CreateInitDeclList();
    AddInitDeclToList(idx_init_list, CreateInitDecl(idx_name, CreateExprInitializer(CreateIntExpr(0))));

    len_args = CreateExprList();
    AddExprToList(len_args, CreateIdentExpr(arr_name));
    len_call = CreateFuncCallExpr("__len", len_args->elements, len_args->count);

    idx_ident_cond = CreateIdentExpr(idx_name);
    cond_expr = CreateBinaryOpExpr(OP_LT, idx_ident_cond, len_call);

    iter_expr = CreateAssignExpr(
        OP_ASSIGN,
        CreateIdentExpr(idx_name),
        CreateBinaryOpExpr(OP_PLUS, CreateIdentExpr(idx_name), CreateIntExpr(1))
    );

    arr_ident_index = CreateIdentExpr(arr_name);
    idx_ident_index = CreateIdentExpr(idx_name);
    element_access = CreateArrayAccessExpr(arr_ident_index, idx_ident_index, NULL);

    var_decl_type = CopyTypeNoContext(element_type);
    if (var_decl_type == NULL) {
        return 0;
    }
    {
        NInitDeclList *var_init = CreateInitDeclList();
        AddInitDeclToList(var_init, CreateInitDecl(stmt->value.foreach_stmt.var_name,
                                                  CreateExprInitializer(element_access)));
        var_decl_stmt = CreateDeclStmt(var_decl_type, var_init);
    }

    body_list = CreateStmtList();
    AddStmtToList(body_list, var_decl_stmt);
    body_stmt = stmt->value.foreach_stmt.body;
    if (body_stmt != NULL) {
        AddStmtToList(body_list, body_stmt);
    }
    body_stmt = CreateCompoundStmt(body_list);

    for_stmt = CreateForStmt(NULL, idx_decl_type, idx_init_list, cond_expr, iter_expr, body_stmt);

    outer_list = CreateStmtList();
    AddStmtToList(outer_list, arr_decl_stmt);
    AddStmtToList(outer_list, for_stmt);

    stmt->type = STMT_COMPOUND;
    stmt->value.stmt_list = outer_list;
    return 1;
}

static void PurgeBreaksInStmtList(NStmtList *list, int loop_depth, int switch_depth) {
    NStmt *prev = NULL;
    NStmt *curr;
    if (list == NULL) {
        return;
    }
    curr = list->first;
    while (curr != NULL) {
        NStmt *next = curr->next;
        if (curr->type == STMT_BREAK && loop_depth == 0 && switch_depth == 1) {
            if (prev == NULL) {
                list->first = next;
            } else {
                prev->next = next;
            }
            if (list->last == curr) {
                list->last = prev;
            }
            curr = next;
            continue;
        }
        PurgeBreaksInStmt(curr, loop_depth, switch_depth);
        prev = curr;
        curr = next;
    }
}

static void PurgeBreaksInStmt(NStmt *stmt, int loop_depth, int switch_depth) {
    if (stmt == NULL) {
        return;
    }
    switch (stmt->type) {
        case STMT_COMPOUND:
            PurgeBreaksInStmtList(stmt->value.stmt_list, loop_depth, switch_depth);
            break;
        case STMT_IF:
            PurgeBreaksInStmt(stmt->value.if_stmt.then_stmt, loop_depth, switch_depth);
            PurgeBreaksInStmt(stmt->value.if_stmt.else_stmt, loop_depth, switch_depth);
            break;
        case STMT_WHILE:
        case STMT_DO_WHILE:
            PurgeBreaksInStmt(stmt->value.while_stmt.body, loop_depth + 1, switch_depth);
            break;
        case STMT_FOR:
            PurgeBreaksInStmt(stmt->value.for_stmt.body, loop_depth + 1, switch_depth);
            break;
        case STMT_FOREACH:
            PurgeBreaksInStmt(stmt->value.foreach_stmt.body, loop_depth + 1, switch_depth);
            break;
        case STMT_SWITCH:
            for (int i = 0; i < stmt->value.switch_stmt.cases.count; i++) {
                NCaseItem *item = stmt->value.switch_stmt.cases.items[i];
                if (item && item->stmts) {
                    PurgeBreaksInStmtList(item->stmts, loop_depth, switch_depth + 1);
                }
            }
            break;
        case STMT_DECL:
        case STMT_EXPR:
        case STMT_RETURN:
        case STMT_BREAK:
        case STMT_CONTINUE:
            break;
    }
}

static int TransformSwitchToIfChain(NStmt *stmt, SemanticContext *ctx) {
    static int switch_counter = 0;
    int local_id;
    char *tmp_name;
    NExpr *switch_expr;
    NType *switch_type;
    NType *tmp_type;
    NInitDeclList *tmp_init_list;
    NStmt *tmp_decl_stmt;
    NStmt *if_chain = NULL;
    NStmt *current_if = NULL;
    NStmt *default_stmt = NULL;
    NStmtList *outer_list;

    if (stmt == NULL || ctx == NULL || stmt->type != STMT_SWITCH) {
        return 0;
    }

    switch_expr = stmt->value.switch_stmt.expr;
    if (switch_expr == NULL) {
        return 0;
    }

    switch_type = InferExpressionTypeSilent(switch_expr, ctx);
    tmp_type = CopyTypeNoContext(switch_type);
    if (tmp_type == NULL) {
        tmp_type = CreateBaseType(TYPE_INT);
    }

    local_id = switch_counter++;
    tmp_name = MakeTempName("__switch_val", local_id);

    tmp_init_list = CreateInitDeclList();
    AddInitDeclToList(tmp_init_list, CreateInitDecl(tmp_name, CreateExprInitializer(switch_expr)));
    tmp_decl_stmt = CreateDeclStmt(tmp_type, tmp_init_list);

    for (int i = 0; i < stmt->value.switch_stmt.cases.count; i++) {
        NCaseItem *item = stmt->value.switch_stmt.cases.items[i];
        NExpr *cond;
        NStmt *then_stmt;
        if (item == NULL) {
            continue;
        }
        if (item->type == CASE_ITEM_DEFAULT) {
            if (default_stmt == NULL) {
                PurgeBreaksInStmtList(item->stmts, 0, 1);
                default_stmt = CreateCompoundStmt(item->stmts);
            }
            continue;
        }
        if (item->case_expr == NULL) {
            continue;
        }
        PurgeBreaksInStmtList(item->stmts, 0, 1);
        cond = CreateBinaryOpExpr(OP_EQ, CreateIdentExpr(tmp_name), item->case_expr);
        then_stmt = CreateCompoundStmt(item->stmts);
        if (if_chain == NULL) {
            if_chain = CreateIfStmt(cond, then_stmt, NULL);
            current_if = if_chain;
        } else {
            current_if->value.if_stmt.else_stmt = CreateIfStmt(cond, then_stmt, NULL);
            current_if = current_if->value.if_stmt.else_stmt;
        }
    }

    if (if_chain != NULL && default_stmt != NULL) {
        current_if->value.if_stmt.else_stmt = default_stmt;
    } else if (if_chain == NULL && default_stmt != NULL) {
        if_chain = default_stmt;
    }

    outer_list = CreateStmtList();
    AddStmtToList(outer_list, tmp_decl_stmt);
    if (if_chain != NULL) {
        AddStmtToList(outer_list, if_chain);
    }

    stmt->type = STMT_COMPOUND;
    stmt->value.stmt_list = outer_list;
    return 1;
}

static int TransformAssignment(NExpr *expr, SemanticContext *ctx) {
    if (expr == NULL || expr->type != EXPR_ASSIGN) {
        return 0;
    }
    TransformExpression(expr->value.binary.left, ctx);
    TransformExpression(expr->value.binary.right, ctx);
    PrepareLValueRValue(expr->value.binary.left, expr->value.binary.right, ctx);
    TryInsertImplicitCast(&expr->value.binary.right, NULL, ctx);
    return 0;
}

static int PrepareLValueRValue(NExpr *lhs, NExpr *rhs, SemanticContext *ctx) {
    (void)lhs;
    (void)rhs;
    (void)ctx;
    return 0;
}

static int TryInsertImplicitCast(NExpr **expr_ptr, const NType *target_type, SemanticContext *ctx) {
    (void)expr_ptr;
    (void)target_type;
    (void)ctx;
    return 0;
}
