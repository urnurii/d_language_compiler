#include "semantic_transforms.h"
#include "../ast_builder.h"
#include "type_inference.h"
#include "name_resolution.h"
#include <stdio.h>
#include <string.h>

static int NormalizeStaticArrayDecl(NType *decl_type, NInitDeclList *init_decls);
static int NormalizeArrayTypeInParams(NParamList *params);
static void NormalizeArrayType(NType *type);
static NType *CreateElementTypeFromArrayType(const NType *array_type);
static void FreeTypeLocal(NType *type);
static void TryCastArrayInitElements(NType *decl_type, NInitializer *init, SemanticContext *ctx);
static int ReplaceSliceWithFuncCall(NExpr *expr, SemanticContext *ctx);
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
static NExpr *CloneLValueExpr(const NExpr *expr);
static int IsArrayType(const NType *type);
static int IsStringType(const NType *type);
static NExpr *CloneExpr(const NExpr *expr);
static int ExpandDefaultArgsForFuncCall(NExpr *expr, SemanticContext *ctx);
static int ExpandDefaultArgsForMethodCall(NExpr *expr, SemanticContext *ctx);
static int ExpandDefaultArgsForSuperMethod(NExpr *expr, SemanticContext *ctx);
static int ExpandDefaultArgsForSuperCtorCall(NStmt *stmt, SemanticContext *ctx);
static int ExpandDefaultArgsForThisCtorCall(NStmt *stmt, SemanticContext *ctx);
static int GetNumericRankLocal(const NType *type, int *rank_out);
static BaseType RankToBaseTypeLocal(int rank);
static int BuildNumericPromotionType(const NType *left, const NType *right, NType *out);

static NType *current_return_type = NULL;

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
                        current_return_type = item->value.func->return_type;
                        TransformStatement(item->value.func->body, ctx);
                        current_return_type = NULL;
                    }
                }
                break;
            case SOURCE_ITEM_CLASS: {
                NClassDef *class_def = item->value.class_def;
                NClassMember *member = class_def ? class_def->members.first : NULL;
                while (member != NULL) {
                    if (member->type == CLASS_MEMBER_FIELD) {
                        NormalizeStaticArrayDecl(member->value.field.field_type, member->value.field.init_decls);
                        if (member->value.field.init_decls) {
                            for (int i = 0; i < member->value.field.init_decls->count; i++) {
                                NInitDecl *decl = member->value.field.init_decls->decls[i];
                                if (decl && decl->initializer && decl->initializer->expr) {
                                    TransformExpression(decl->initializer->expr, ctx);
                                    TryInsertImplicitCast(&decl->initializer->expr,
                                                          member->value.field.field_type,
                                                          ctx);
                                }
                                if (decl && decl->initializer && decl->initializer->is_array) {
                                    for (int j = 0; j < decl->initializer->array_init.count; j++) {
                                        TransformExpression(decl->initializer->array_init.elements[j], ctx);
                                    }
                                    TryCastArrayInitElements(member->value.field.field_type,
                                                             decl->initializer,
                                                             ctx);
                                }
                            }
                        }
                    } else if (member->type == CLASS_MEMBER_METHOD && member->value.method) {
                        NormalizeArrayType(member->value.method->return_type);
                        NormalizeArrayTypeInParams(member->value.method->params);
                        if (member->value.method->body) {
                            current_return_type = member->value.method->return_type;
                            TransformStatement(member->value.method->body, ctx);
                            current_return_type = NULL;
                        }
                    } else if (member->type == CLASS_MEMBER_CTOR && member->value.ctor) {
                        NormalizeArrayTypeInParams(member->value.ctor->params);
                        if (member->value.ctor->body) {
                            current_return_type = NULL;
                            TransformStatement(member->value.ctor->body, ctx);
                        }
                    } else if (member->type == CLASS_MEMBER_DTOR && member->value.dtor && member->value.dtor->body) {
                        current_return_type = NULL;
                        TransformStatement(member->value.dtor->body, ctx);
                    }
                    member = member->next;
                }
                break;
            }
            case SOURCE_ITEM_DECL:
                NormalizeStaticArrayDecl(item->value.decl.item_type, item->value.decl.init_decls);
                if (item->value.decl.init_decls) {
                    for (int i = 0; i < item->value.decl.init_decls->count; i++) {
                        NInitDecl *decl = item->value.decl.init_decls->decls[i];
                        if (decl && decl->initializer && decl->initializer->expr) {
                            TransformExpression(decl->initializer->expr, ctx);
                            TryInsertImplicitCast(&decl->initializer->expr,
                                                  item->value.decl.item_type,
                                                  ctx);
                        }
                        if (decl && decl->initializer && decl->initializer->is_array) {
                            for (int j = 0; j < decl->initializer->array_init.count; j++) {
                                TransformExpression(decl->initializer->array_init.elements[j], ctx);
                            }
                            TryCastArrayInitElements(item->value.decl.item_type,
                                                     decl->initializer,
                                                     ctx);
                        }
                    }
                }
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
            TransformExpression(stmt->value.expr, ctx);
            break;
        case STMT_RETURN:
            TransformExpression(stmt->value.expr, ctx);
            if (stmt->value.expr != NULL && current_return_type != NULL) {
                TryInsertImplicitCast(&stmt->value.expr, current_return_type, ctx);
            }
            break;
        case STMT_DECL:
            NormalizeStaticArrayDecl(stmt->value.decl.decl_type, stmt->value.decl.init_decls);
            if (stmt->value.decl.init_decls) {
                for (int i = 0; i < stmt->value.decl.init_decls->count; i++) {
                    NInitDecl *decl = stmt->value.decl.init_decls->decls[i];
                    if (decl && decl->initializer && decl->initializer->expr) {
                        TransformExpression(decl->initializer->expr, ctx);
                        TryInsertImplicitCast(&decl->initializer->expr,
                                              stmt->value.decl.decl_type,
                                              ctx);
                    }
                    if (decl && decl->initializer && decl->initializer->is_array) {
                        for (int j = 0; j < decl->initializer->array_init.count; j++) {
                            TransformExpression(decl->initializer->array_init.elements[j], ctx);
                        }
                        TryCastArrayInitElements(stmt->value.decl.decl_type, decl->initializer, ctx);
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
                    if (decl && decl->initializer && decl->initializer->is_array) {
                        for (int j = 0; j < decl->initializer->array_init.count; j++) {
                            TransformExpression(decl->initializer->array_init.elements[j], ctx);
                        }
                        TryCastArrayInitElements(stmt->value.for_stmt.init_decl_type, decl->initializer, ctx);
                    }
                }
            }
            break;
        case STMT_SUPER_CTOR_CALL:
            ExpandDefaultArgsForSuperCtorCall(stmt, ctx);
            if (stmt->value.super_ctor.args != NULL) {
                for (int i = 0; i < stmt->value.super_ctor.args->count; i++) {
                    TransformExpression(stmt->value.super_ctor.args->elements[i], ctx);
                }
            }
            break;
        case STMT_THIS_CTOR_CALL:
            ExpandDefaultArgsForThisCtorCall(stmt, ctx);
            if (stmt->value.this_ctor.args != NULL) {
                for (int i = 0; i < stmt->value.this_ctor.args->count; i++) {
                    TransformExpression(stmt->value.this_ctor.args->elements[i], ctx);
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
        case EXPR_CAST:
            TransformExpression(expr->value.cast.expr, ctx);
            break;
        case EXPR_NEW:
            for (int i = 0; i < expr->value.new_expr.init_count; i++) {
                TransformExpression(expr->value.new_expr.init_exprs[i], ctx);
            }
            break;
        case EXPR_ARRAY_ACCESS:
            if (expr->value.array_access.index_end != NULL) {
                ReplaceSliceWithFuncCall(expr, ctx);
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
            ExpandDefaultArgsForMethodCall(expr, ctx);
            TransformExpression(expr->value.member_access.object, ctx);
            for (int i = 0; i < expr->value.member_access.arg_count; i++) {
                TransformExpression(expr->value.member_access.args[i], ctx);
            }
            break;
        case EXPR_FUNC_CALL:
            ExpandDefaultArgsForFuncCall(expr, ctx);
            for (int i = 0; i < expr->value.func_call.arg_count; i++) {
                TransformExpression(expr->value.func_call.args[i], ctx);
            }
            break;
        case EXPR_SUPER:
            break;
        case EXPR_SUPER_METHOD:
            ExpandDefaultArgsForSuperMethod(expr, ctx);
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
            if (expr->value.binary.op == OP_PLUS ||
                expr->value.binary.op == OP_MINUS ||
                expr->value.binary.op == OP_MUL ||
                expr->value.binary.op == OP_DIV) {
                if (expr->inferred_type != NULL && IsNumericType(expr->inferred_type)) {
                    TryInsertImplicitCast(&expr->value.binary.left, expr->inferred_type, ctx);
                    TryInsertImplicitCast(&expr->value.binary.right, expr->inferred_type, ctx);
                }
            } else if (expr->value.binary.op == OP_EQ ||
                       expr->value.binary.op == OP_NEQ ||
                       expr->value.binary.op == OP_LT ||
                       expr->value.binary.op == OP_LE ||
                       expr->value.binary.op == OP_GT ||
                       expr->value.binary.op == OP_GE) {
                NType promoted;
                if (BuildNumericPromotionType(expr->value.binary.left ?
                                                  expr->value.binary.left->inferred_type : NULL,
                                              expr->value.binary.right ?
                                                  expr->value.binary.right->inferred_type : NULL,
                                              &promoted)) {
                    TryInsertImplicitCast(&expr->value.binary.left, &promoted, ctx);
                    TryInsertImplicitCast(&expr->value.binary.right, &promoted, ctx);
                }
            }
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
    if ((type->kind == TYPE_KIND_BASE_ARRAY ||
         type->kind == TYPE_KIND_CLASS_ARRAY ||
         type->kind == TYPE_KIND_ENUM_ARRAY) &&
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

static void FreeTypeLocal(NType *type) {
    if (type == NULL) {
        return;
    }
    if (type->class_name != NULL) {
        free(type->class_name);
    }
    if (type->enum_name != NULL) {
        free(type->enum_name);
    }
    if (type->array_decl != NULL) {
        free(type->array_decl);
    }
    free(type);
}

static void TryCastArrayInitElements(NType *decl_type, NInitializer *init, SemanticContext *ctx) {
    NType *elem_type;
    if (decl_type == NULL || init == NULL || ctx == NULL) {
        return;
    }
    if (!init->is_array) {
        return;
    }
    elem_type = CreateElementTypeFromArrayType(decl_type);
    if (elem_type == NULL) {
        return;
    }
    for (int i = 0; i < init->array_init.count; i++) {
        TryInsertImplicitCast(&init->array_init.elements[i], elem_type, ctx);
    }
    FreeTypeLocal(elem_type);
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
    if (array_type->kind == TYPE_KIND_ENUM_ARRAY) {
        return CreateEnumType(array_type->enum_name);
    }
    return NULL;
}

static int NormalizeStaticArrayDecl(NType *decl_type, NInitDeclList *init_decls) {
    if (decl_type == NULL) {
        return 0;
    }
    if ((decl_type->kind != TYPE_KIND_BASE_ARRAY &&
         decl_type->kind != TYPE_KIND_CLASS_ARRAY &&
         decl_type->kind != TYPE_KIND_ENUM_ARRAY) ||
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

static int ReplaceSliceWithFuncCall(NExpr *expr, SemanticContext *ctx) {
    NExprList *list;
    NExpr *array_expr;
    NExpr *index_expr;
    NExpr *index_end_expr;
    NType *array_type;
    int type_tag = 0;
    const char *class_name = NULL;

    if (expr == NULL || expr->type != EXPR_ARRAY_ACCESS) {
        return 0;
    }
    if (expr->value.array_access.index_end == NULL) {
        return 0;
    }

    array_expr = expr->value.array_access.array;
    index_expr = expr->value.array_access.index;
    index_end_expr = expr->value.array_access.index_end;

    array_type = InferExpressionTypeSilent(array_expr, ctx);
    if (array_type != NULL) {
        expr->inferred_type = CopyType(array_type, ctx);
    }
    if (array_type != NULL) {
        switch (array_type->kind) {
            case TYPE_KIND_BASE_ARRAY:
                switch (array_type->base_type) {
                    case TYPE_BOOL: type_tag = 1; break;
                    case TYPE_INT:
                    case TYPE_CHAR: type_tag = 2; break;
                    case TYPE_FLOAT: type_tag = 3; break;
                    case TYPE_DOUBLE:
                    case TYPE_REAL: type_tag = 4; break;
                    case TYPE_STRING:
                        type_tag = 5;
                        class_name = "java/lang/String";
                        break;
                    default:
                        type_tag = 2;
                        break;
                }
                break;
            case TYPE_KIND_CLASS_ARRAY:
                type_tag = 6;
                class_name = array_type->class_name;
                break;
            case TYPE_KIND_ENUM_ARRAY:
                type_tag = 7;
                break;
            default:
                break;
        }
    }

    list = CreateExprList();
    AddExprToList(list, array_expr);
    AddExprToList(list, index_expr);
    AddExprToList(list, index_end_expr);
    AddExprToList(list, CreateIntExpr(type_tag));
    if (class_name != NULL) {
        AddExprToList(list, CreateStringExpr(class_name));
    } else {
        AddExprToList(list, CreateNullExpr());
    }

    expr->type = EXPR_FUNC_CALL;
    expr->value.func_call.func_name = DuplicateString("__slice_typed");
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
    NType *collection_elem_type;
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
        collection_type->kind != TYPE_KIND_CLASS_ARRAY &&
        collection_type->kind != TYPE_KIND_ENUM_ARRAY) {
        return 0;
    }

    collection_elem_type = CreateElementTypeFromArrayType(collection_type);

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
        NExpr *var_init_expr = element_access;
        if (stmt->value.foreach_stmt.is_typed &&
            stmt->value.foreach_stmt.var_type != NULL &&
            collection_elem_type != NULL &&
            !TypesEqual(stmt->value.foreach_stmt.var_type, collection_elem_type) &&
            IsNumericType(stmt->value.foreach_stmt.var_type) &&
            IsNumericType(collection_elem_type)) {
            int src_rank = 0;
            int tgt_rank = 0;
            if (GetNumericRankLocal(collection_elem_type, &src_rank) &&
                GetNumericRankLocal(stmt->value.foreach_stmt.var_type, &tgt_rank) &&
                tgt_rank >= src_rank) {
                NType *cast_type = CopyTypeNoContext(stmt->value.foreach_stmt.var_type);
                if (cast_type != NULL) {
                    var_init_expr = CreateCastExpr(cast_type, element_access);
                }
            }
        }
        NInitDeclList *var_init = CreateInitDeclList();
        AddInitDeclToList(var_init, CreateInitDecl(stmt->value.foreach_stmt.var_name,
                                                  CreateExprInitializer(var_init_expr)));
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
        case STMT_SUPER_CTOR_CALL:
        case STMT_THIS_CTOR_CALL:
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
    NExpr *lhs_copy;
    NExpr *new_right;
    NExprList *args;
    NType *lhs_type;
    int is_array;
    if (expr == NULL || expr->type != EXPR_ASSIGN) {
        return 0;
    }
    if (expr->value.binary.op == OP_ASSIGN &&
        expr->value.binary.left != NULL &&
        expr->value.binary.left->type == EXPR_ARRAY_ACCESS &&
        expr->value.binary.left->value.array_access.index_end != NULL) {
        NExpr *lhs = expr->value.binary.left;
        NExpr *arr = lhs->value.array_access.array;
        NExpr *idx = lhs->value.array_access.index;
        NExpr *idx_end = lhs->value.array_access.index_end;
        if (arr != NULL) {
            TransformExpression(arr, ctx);
        }
        if (idx != NULL) {
            TransformExpression(idx, ctx);
        }
        if (idx_end != NULL) {
            TransformExpression(idx_end, ctx);
        }
        TransformExpression(expr->value.binary.right, ctx);
        args = CreateExprList();
        AddExprToList(args, arr);
        AddExprToList(args, idx);
        AddExprToList(args, idx_end);
        AddExprToList(args, expr->value.binary.right);
        expr->type = EXPR_FUNC_CALL;
        expr->value.func_call.func_name = DuplicateString("__slice_assign");
        expr->value.func_call.args = args->elements;
        expr->value.func_call.arg_count = args->count;
        return 0;
    }
    TransformExpression(expr->value.binary.left, ctx);
    TransformExpression(expr->value.binary.right, ctx);
    if (expr->value.binary.op != OP_ASSIGN) {
        if (expr->value.binary.op == OP_BITWISE_NOT_ASSIGN) {
            lhs_type = expr->value.binary.left ? expr->value.binary.left->inferred_type : NULL;
            if (lhs_type == NULL) {
                lhs_type = InferExpressionTypeSilent(expr->value.binary.left, ctx);
            }
            args = CreateExprList();
            AddExprToList(args, CloneLValueExpr(expr->value.binary.left));
            AddExprToList(args, expr->value.binary.right);
            if (IsStringType(lhs_type)) {
                new_right = CreateFuncCallExpr("__str_append", args->elements, args->count);
            } else {
                is_array = IsArrayType(lhs_type);
                new_right = CreateFuncCallExpr("__append", args->elements, args->count);
                (void)is_array;
            }
            expr->value.binary.op = OP_ASSIGN;
            expr->value.binary.right = new_right;
        } else {
            OpType base_op = OP_PLUS;
            switch (expr->value.binary.op) {
                case OP_PLUS_ASSIGN: base_op = OP_PLUS; break;
                case OP_MINUS_ASSIGN: base_op = OP_MINUS; break;
                case OP_MUL_ASSIGN: base_op = OP_MUL; break;
                case OP_DIV_ASSIGN: base_op = OP_DIV; break;
                default: base_op = OP_PLUS; break;
            }
            lhs_copy = CloneLValueExpr(expr->value.binary.left);
            new_right = CreateBinaryOpExpr(base_op, lhs_copy, expr->value.binary.right);
            expr->value.binary.op = OP_ASSIGN;
            expr->value.binary.right = new_right;
        }
    }
    PrepareLValueRValue(expr->value.binary.left, expr->value.binary.right, ctx);
    lhs_type = expr->value.binary.left ? expr->value.binary.left->inferred_type : NULL;
    if (lhs_type == NULL) {
        lhs_type = InferExpressionTypeSilent(expr->value.binary.left, ctx);
    }
    if (lhs_type != NULL) {
        TryInsertImplicitCast(&expr->value.binary.right, lhs_type, ctx);
    }
    return 0;
}

static int PrepareLValueRValue(NExpr *lhs, NExpr *rhs, SemanticContext *ctx) {
    (void)lhs;
    (void)rhs;
    (void)ctx;
    return 0;
}

static int TryInsertImplicitCast(NExpr **expr_ptr, const NType *target_type, SemanticContext *ctx) {
    NExpr *expr;
    NType *source_type;
    int src_rank = 0;
    int tgt_rank = 0;
    if (expr_ptr == NULL || *expr_ptr == NULL || target_type == NULL) {
        return 0;
    }
    expr = *expr_ptr;
    if (expr->type == EXPR_CAST &&
        expr->value.cast.target_type != NULL &&
        TypesEqual(expr->value.cast.target_type, (NType *)target_type)) {
        return 0;
    }
    source_type = expr->inferred_type;
    if (source_type == NULL) {
        source_type = InferExpressionTypeSilent(expr, ctx);
        expr->inferred_type = source_type;
    }
    if (source_type == NULL) {
        return 0;
    }
    if (TypesEqual((NType *)target_type, source_type)) {
        return 0;
    }
    if (!IsNumericType((NType *)target_type) || !IsNumericType(source_type)) {
        return 0;
    }
    if (!GetNumericRankLocal(target_type, &tgt_rank) ||
        !GetNumericRankLocal(source_type, &src_rank)) {
        return 0;
    }
    if (tgt_rank < src_rank) {
        return 0;
    }
    {
        NType *target_copy = CopyType((NType *)target_type, ctx);
        NExpr *cast_expr;
        if (target_copy == NULL) {
            return 1;
        }
        cast_expr = CreateCastExpr(target_copy, expr);
        if (cast_expr == NULL) {
            return 1;
        }
        cast_expr->line = expr->line;
        cast_expr->column = expr->column;
        cast_expr->inferred_type = target_copy;
        *expr_ptr = cast_expr;
    }
    return 0;
}

static int GetNumericRankLocal(const NType *type, int *rank_out) {
    if (type == NULL || rank_out == NULL) {
        return 0;
    }
    if (type->kind != TYPE_KIND_BASE) {
        return 0;
    }
    switch (type->base_type) {
        case TYPE_INT:
        case TYPE_CHAR:
            *rank_out = 1;
            return 1;
        case TYPE_FLOAT:
            *rank_out = 2;
            return 1;
        case TYPE_DOUBLE:
            *rank_out = 3;
            return 1;
        case TYPE_REAL:
            *rank_out = 4;
            return 1;
        default:
            return 0;
    }
}

static BaseType RankToBaseTypeLocal(int rank) {
    switch (rank) {
        case 4:
            return TYPE_REAL;
        case 3:
            return TYPE_DOUBLE;
        case 2:
            return TYPE_FLOAT;
        case 1:
        default:
            return TYPE_INT;
    }
}

static int BuildNumericPromotionType(const NType *left, const NType *right, NType *out) {
    int left_rank;
    int right_rank;
    int max_rank;
    if (left == NULL || right == NULL || out == NULL) {
        return 0;
    }
    if (!IsNumericType((NType *)left) || !IsNumericType((NType *)right)) {
        return 0;
    }
    if (!GetNumericRankLocal(left, &left_rank) || !GetNumericRankLocal(right, &right_rank)) {
        return 0;
    }
    max_rank = (left_rank > right_rank) ? left_rank : right_rank;
    memset(out, 0, sizeof(NType));
    out->kind = TYPE_KIND_BASE;
    out->base_type = RankToBaseTypeLocal(max_rank);
    out->class_name = NULL;
    out->array_decl = NULL;
    return 1;
}

static int IsArrayType(const NType *type) {
    if (type == NULL) {
        return 0;
    }
    return type->kind == TYPE_KIND_BASE_ARRAY ||
           type->kind == TYPE_KIND_CLASS_ARRAY ||
           type->kind == TYPE_KIND_ENUM_ARRAY;
}

static int IsStringType(const NType *type) {
    if (type == NULL) {
        return 0;
    }
    return type->kind == TYPE_KIND_BASE && type->base_type == TYPE_STRING;
}

static NExpr *CloneLValueExpr(const NExpr *expr) {
    if (expr == NULL) {
        return NULL;
    }
    switch (expr->type) {
        case EXPR_IDENT:
            return CreateIdentExpr(expr->value.ident_name);
        case EXPR_MEMBER_ACCESS:
            return CreateMemberAccessExpr(CloneLValueExpr(expr->value.member_access.object),
                                          expr->value.member_access.member_name);
        case EXPR_ARRAY_ACCESS:
            return CreateArrayAccessExpr(CloneLValueExpr(expr->value.array_access.array),
                                         CloneLValueExpr(expr->value.array_access.index),
                                         CloneLValueExpr(expr->value.array_access.index_end));
        case EXPR_PAREN:
            return CreateParenExpr(CloneLValueExpr(expr->value.inner_expr));
        case EXPR_THIS:
            return CreateThisExpr();
        default:
            return CreateIdentExpr("__invalid_lvalue");
    }
}

static NExpr *CloneExpr(const NExpr *expr) {
    if (expr == NULL) {
        return NULL;
    }
    switch (expr->type) {
        case EXPR_IDENT:
            return CreateIdentExpr(expr->value.ident_name);
        case EXPR_INT:
            return CreateIntExpr(expr->value.int_value);
        case EXPR_FLOAT:
            return CreateFloatExpr(expr->value.float_value);
        case EXPR_CHAR:
            return CreateCharExpr(expr->value.char_value);
        case EXPR_STRING:
            return CreateStringExpr(expr->value.string_value ? expr->value.string_value : "");
        case EXPR_BOOL:
            return CreateBoolExpr(expr->value.int_value);
        case EXPR_NULL:
            return CreateNullExpr();
        case EXPR_NAN:
            return CreateNanExpr();
        case EXPR_THIS:
            return CreateThisExpr();
        case EXPR_SUPER:
            return CreateSuperExpr(expr->value.ident_name);
        case EXPR_PAREN:
            return CreateParenExpr(CloneExpr(expr->value.inner_expr));
        case EXPR_CAST: {
            NType *target_copy = CopyTypeNoContext(expr->value.cast.target_type);
            NExpr *inner = CloneExpr(expr->value.cast.expr);
            NExpr *cast_expr = CreateCastExpr(target_copy, inner);
            if (cast_expr != NULL && target_copy != NULL) {
                cast_expr->inferred_type = target_copy;
            }
            return cast_expr;
        }
        case EXPR_UNARY_OP:
            return CreateUnaryOpExpr(expr->value.unary.op, CloneExpr(expr->value.unary.operand));
        case EXPR_BINARY_OP:
        case EXPR_ASSIGN:
            return CreateBinaryOpExpr(expr->value.binary.op,
                                      CloneExpr(expr->value.binary.left),
                                      CloneExpr(expr->value.binary.right));
        case EXPR_FUNC_CALL: {
            NExprList *list = CreateExprList();
            for (int i = 0; i < expr->value.func_call.arg_count; i++) {
                AddExprToList(list, CloneExpr(expr->value.func_call.args[i]));
            }
            return CreateFuncCallExpr(expr->value.func_call.func_name, list->elements, list->count);
        }
        case EXPR_MEMBER_ACCESS:
            return CreateMemberAccessExpr(CloneExpr(expr->value.member_access.object),
                                          expr->value.member_access.member_name);
        case EXPR_METHOD_CALL: {
            NExprList *list = CreateExprList();
            for (int i = 0; i < expr->value.member_access.arg_count; i++) {
                AddExprToList(list, CloneExpr(expr->value.member_access.args[i]));
            }
            return CreateMethodCallExpr(CloneExpr(expr->value.member_access.object),
                                        expr->value.member_access.member_name,
                                        list->elements,
                                        list->count);
        }
        case EXPR_SUPER_METHOD: {
            NExprList *list = CreateExprList();
            for (int i = 0; i < expr->value.member_access.arg_count; i++) {
                AddExprToList(list, CloneExpr(expr->value.member_access.args[i]));
            }
            return CreateSuperMethodCallExpr(expr->value.member_access.member_name, list->elements, list->count);
        }
        case EXPR_ARRAY_ACCESS:
            return CreateArrayAccessExpr(CloneExpr(expr->value.array_access.array),
                                         CloneExpr(expr->value.array_access.index),
                                         CloneExpr(expr->value.array_access.index_end));
        case EXPR_NEW: {
            NExprList *list = CreateExprList();
            NType *type_copy = CopyTypeNoContext(expr->value.new_expr.type);
            for (int i = 0; i < expr->value.new_expr.init_count; i++) {
                AddExprToList(list, CloneExpr(expr->value.new_expr.init_exprs[i]));
            }
            return CreateNewExpr(type_copy, list->elements, list->count);
        }
        default:
            return CreateIdentExpr("__invalid_default");
    }
}

static int ExpandDefaultArgsForFuncCall(NExpr *expr, SemanticContext *ctx) {
    FunctionInfo *func;
    int expected;
    int count;
    if (expr == NULL || expr->type != EXPR_FUNC_CALL || ctx == NULL) {
        return 0;
    }
    func = LookupFunctionOverload(ctx,
                                  expr->value.func_call.func_name,
                                  expr->value.func_call.args,
                                  expr->value.func_call.arg_count,
                                  NULL);
    if (func == NULL || func->params == NULL) {
        return 0;
    }
    expected = func->params->count;
    count = expr->value.func_call.arg_count;
    if (count >= expected) {
        return 0;
    }
    {
        NExpr **new_args = (NExpr **)malloc(sizeof(NExpr *) * (size_t)expected);
        if (new_args == NULL) {
            return 1;
        }
        for (int i = 0; i < count; i++) {
            new_args[i] = expr->value.func_call.args[i];
        }
        for (int i = count; i < expected; i++) {
            NParam *param = func->params->params[i];
            if (param == NULL || param->default_value == NULL) {
                free(new_args);
                return 0;
            }
            new_args[i] = CloneExpr(param->default_value);
        }
        expr->value.func_call.args = new_args;
        expr->value.func_call.arg_count = expected;
    }
    return 1;
}

static int ExpandDefaultArgsForMethodCall(NExpr *expr, SemanticContext *ctx) {
    ClassInfo *cls;
    MethodInfo *method;
    NType *obj_type;
    int expected;
    int count;
    if (expr == NULL || expr->type != EXPR_METHOD_CALL || ctx == NULL) {
        return 0;
    }
    obj_type = InferExpressionTypeSilent(expr->value.member_access.object, ctx);
    if (obj_type == NULL || obj_type->kind != TYPE_KIND_CLASS) {
        return 0;
    }
    cls = LookupClass(ctx, obj_type->class_name);
    if (cls == NULL) {
        return 0;
    }
    method = LookupClassMethod(cls, expr->value.member_access.member_name);
    if (method == NULL || method->params == NULL) {
        return 0;
    }
    expected = method->params->count;
    count = expr->value.member_access.arg_count;
    if (count >= expected) {
        return 0;
    }
    {
        NExpr **new_args = (NExpr **)malloc(sizeof(NExpr *) * (size_t)expected);
        if (new_args == NULL) {
            return 1;
        }
        for (int i = 0; i < count; i++) {
            new_args[i] = expr->value.member_access.args[i];
        }
        for (int i = count; i < expected; i++) {
            NParam *param = method->params->params[i];
            if (param == NULL || param->default_value == NULL) {
                free(new_args);
                return 0;
            }
            new_args[i] = CloneExpr(param->default_value);
        }
        expr->value.member_access.args = new_args;
        expr->value.member_access.arg_count = expected;
    }
    return 1;
}

static int ExpandDefaultArgsForSuperMethod(NExpr *expr, SemanticContext *ctx) {
    ClassInfo *base;
    MethodInfo *method;
    int expected;
    int count;
    if (expr == NULL || expr->type != EXPR_SUPER_METHOD || ctx == NULL || ctx->current_class == NULL) {
        return 0;
    }
    if (ctx->current_class->base_class == NULL) {
        return 0;
    }
    base = LookupClass(ctx, ctx->current_class->base_class);
    if (base == NULL) {
        return 0;
    }
    method = LookupClassMethod(base, expr->value.member_access.member_name);
    if (method == NULL || method->params == NULL) {
        return 0;
    }
    expected = method->params->count;
    count = expr->value.member_access.arg_count;
    if (count >= expected) {
        return 0;
    }
    {
        NExpr **new_args = (NExpr **)malloc(sizeof(NExpr *) * (size_t)expected);
        if (new_args == NULL) {
            return 1;
        }
        for (int i = 0; i < count; i++) {
            new_args[i] = expr->value.member_access.args[i];
        }
        for (int i = count; i < expected; i++) {
            NParam *param = method->params->params[i];
            if (param == NULL || param->default_value == NULL) {
                free(new_args);
                return 0;
            }
            new_args[i] = CloneExpr(param->default_value);
        }
        expr->value.member_access.args = new_args;
        expr->value.member_access.arg_count = expected;
    }
    return 1;
}

static int IsValidLValueForCtorRef(const NExpr *expr) {
    if (expr == NULL) {
        return 0;
    }
    switch (expr->type) {
        case EXPR_IDENT:
            return expr->enum_value_is_set ? 0 : 1;
        case EXPR_MEMBER_ACCESS:
            return expr->enum_value_is_set ? 0 : 1;
        case EXPR_ARRAY_ACCESS:
            return 1;
        case EXPR_PAREN:
            return IsValidLValueForCtorRef(expr->value.inner_expr);
        default:
            return 0;
    }
}

static int CanMatchCtorInTransform(const NCtorDef *ctor, NExpr **args, int arg_count,
                                   SemanticContext *ctx, int *score_out) {
    int expected;
    int count;
    int score = 0;

    if (ctor == NULL) {
        return 0;
    }

    expected = (ctor->params != NULL) ? ctor->params->count : 0;

    if (arg_count > expected) {
        return 0;
    }
    if (arg_count < expected && ctor->params != NULL) {
        for (int i = arg_count; i < expected; i++) {
            NParam *param = ctor->params->params[i];
            if (param == NULL || param->default_value == NULL) {
                return 0;
            }
        }
    }

    if (ctor->params == NULL) {
        if (score_out != NULL) {
            *score_out = score;
        }
        return 1;
    }

    count = expected < arg_count ? expected : arg_count;
    for (int i = 0; i < count; i++) {
        NParam *param = ctor->params->params[i];
        NExpr *arg = (args != NULL) ? args[i] : NULL;
        NType *arg_type;
        if (param == NULL || arg == NULL || param->param_type == NULL) {
            return 0;
        }
        if (param->is_ref && !IsValidLValueForCtorRef(arg)) {
            return 0;
        }
        arg_type = InferExpressionTypeSilent(arg, ctx);
        if (arg_type == NULL) {
            return 0;
        }
        if (!IsArgumentCompatibleWithParameter(param->param_type, arg_type, param->is_ref)) {
            return 0;
        }
        if (TypesEqual(param->param_type, arg_type)) {
            score += 2;
        } else {
            score += 1;
        }
    }

    if (score_out != NULL) {
        *score_out = score;
    }
    return 1;
}

static NCtorDef *LookupCtorOverloadInTransform(ClassInfo *class_info, NExpr **args, int arg_count,
                                               SemanticContext *ctx, int *is_ambiguous) {
    NCtorDef *best = NULL;
    int best_score = -1;
    int ambiguous = 0;

    if (is_ambiguous != NULL) {
        *is_ambiguous = 0;
    }
    if (class_info == NULL || class_info->constructors == NULL) {
        return NULL;
    }

    for (int i = 0; i < class_info->constructor_count; i++) {
        NCtorDef *ctor = class_info->constructors[i];
        int score = 0;
        if (ctor == NULL) {
            continue;
        }
        if (!CanMatchCtorInTransform(ctor, args, arg_count, ctx, &score)) {
            continue;
        }
        if (score > best_score) {
            best = ctor;
            best_score = score;
            ambiguous = 0;
        } else if (score == best_score) {
            ambiguous = 1;
        }
    }

    if (is_ambiguous != NULL) {
        *is_ambiguous = ambiguous;
    }
    if (ambiguous) {
        return NULL;
    }
    return best;
}

static int ExpandDefaultArgsForSuperCtorCall(NStmt *stmt, SemanticContext *ctx) {
    ClassInfo *base;
    NCtorDef *ctor;
    NExprList *args_list;
    NExpr **args;
    int expected;
    int count;
    int ambiguous = 0;

    if (stmt == NULL || stmt->type != STMT_SUPER_CTOR_CALL || ctx == NULL || ctx->current_class == NULL) {
        return 0;
    }
    if (ctx->current_class->base_class == NULL) {
        return 0;
    }
    base = LookupClass(ctx, ctx->current_class->base_class);
    if (base == NULL || base->constructor_count <= 0) {
        return 0;
    }
    args_list = stmt->value.super_ctor.args;
    args = args_list ? args_list->elements : NULL;
    count = args_list ? args_list->count : 0;
    ctor = LookupCtorOverloadInTransform(base, args, count, ctx, &ambiguous);
    if (ctor == NULL || ctor->params == NULL) {
        return 0;
    }
    expected = ctor->params->count;
    if (count >= expected) {
        return 0;
    }
    {
        NExpr **new_args = (NExpr **)malloc(sizeof(NExpr *) * (size_t)expected);
        if (new_args == NULL) {
            return 1;
        }
        for (int i = 0; i < count; i++) {
            new_args[i] = args[i];
        }
        for (int i = count; i < expected; i++) {
            NParam *param = ctor->params->params[i];
            if (param == NULL || param->default_value == NULL) {
                free(new_args);
                return 0;
            }
            new_args[i] = CloneExpr(param->default_value);
        }
        if (args_list == NULL) {
            args_list = CreateExprList();
            if (args_list == NULL) {
                free(new_args);
                return 1;
            }
            stmt->value.super_ctor.args = args_list;
        }
        args_list->elements = new_args;
        args_list->count = expected;
        args_list->capacity = expected;
    }
    return 1;
}

static int ExpandDefaultArgsForThisCtorCall(NStmt *stmt, SemanticContext *ctx) {
    ClassInfo *cls;
    NCtorDef *ctor;
    NExprList *args_list;
    NExpr **args;
    int expected;
    int count;
    int ambiguous = 0;

    if (stmt == NULL || stmt->type != STMT_THIS_CTOR_CALL || ctx == NULL || ctx->current_class == NULL) {
        return 0;
    }
    cls = ctx->current_class;
    if (cls == NULL || cls->constructor_count <= 0) {
        return 0;
    }
    args_list = stmt->value.this_ctor.args;
    args = args_list ? args_list->elements : NULL;
    count = args_list ? args_list->count : 0;
    ctor = LookupCtorOverloadInTransform(cls, args, count, ctx, &ambiguous);
    if (ctor == NULL || ctor->params == NULL) {
        return 0;
    }
    expected = ctor->params->count;
    if (count >= expected) {
        return 0;
    }
    {
        NExpr **new_args = (NExpr **)malloc(sizeof(NExpr *) * (size_t)expected);
        if (new_args == NULL) {
            return 1;
        }
        for (int i = 0; i < count; i++) {
            new_args[i] = args[i];
        }
        for (int i = count; i < expected; i++) {
            NParam *param = ctor->params->params[i];
            if (param == NULL || param->default_value == NULL) {
                free(new_args);
                return 0;
            }
            new_args[i] = CloneExpr(param->default_value);
        }
        if (args_list == NULL) {
            args_list = CreateExprList();
            if (args_list == NULL) {
                free(new_args);
                return 1;
            }
            stmt->value.this_ctor.args = args_list;
        }
        args_list->elements = new_args;
        args_list->count = expected;
        args_list->capacity = expected;
    }
    return 1;
}
