#include "semantic_transforms.h"
#include "ast_builder.h"

static int NormalizeStaticArrayDecl(NType *decl_type, NInitDeclList *init_decls);
static int NormalizeArrayTypeInParams(NParamList *params);
static void NormalizeArrayType(NType *type);
static NType *CreateElementTypeFromArrayType(const NType *array_type);
static int ReplaceSliceWithFuncCall(NExpr *expr);

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
            TransformExpression(stmt->value.foreach_stmt.collection, ctx);
            TransformStatement(stmt->value.foreach_stmt.body, ctx);
            break;
        case STMT_SWITCH:
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
