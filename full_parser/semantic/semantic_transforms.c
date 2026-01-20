#include "semantic_transforms.h"

/* Future transform hooks (examples, no-op now):
   1) EXPR_ASSIGN with EXPR_MEMBER_ACCESS on LHS -> explicit field lvalue node.
   2) EXPR_ASSIGN with EXPR_ARRAY_ACCESS on LHS -> explicit array element lvalue node.
   3) Compound assignment (+=, -=, etc.) -> rewrite to simple assign with binary op.
   4) EXPR_PAREN -> optionally drop parentheses to simplify codegen.
   5) EXPR_METHOD_CALL on super -> explicit base dispatch node.
   6) EXPR_FUNC_CALL on method name inside class -> rewrite to this.method().
   7) Return without value in non-void function -> insert default value error node.
   8) Implicit numeric promotion -> insert explicit cast node.
   9) Boolean condition normalization -> insert compare-to-zero node.
  10) Foreach -> rewrite to for with index/temp.
  11) For with decl init -> split into decl + for.
  12) New array with size -> rewrite to allocation helper node.
*/

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
                if (item->value.func && item->value.func->body) {
                    TransformStatement(item->value.func->body, ctx);
                }
                break;
            case SOURCE_ITEM_CLASS: {
                NClassDef *class_def = item->value.class_def;
                NClassMember *member = class_def ? class_def->members.first : NULL;
                while (member != NULL) {
                    if (member->type == CLASS_MEMBER_METHOD && member->value.method && member->value.method->body) {
                        TransformStatement(member->value.method->body, ctx);
                    } else if (member->type == CLASS_MEMBER_CTOR && member->value.ctor && member->value.ctor->body) {
                        TransformStatement(member->value.ctor->body, ctx);
                    } else if (member->type == CLASS_MEMBER_DTOR && member->value.dtor && member->value.dtor->body) {
                        TransformStatement(member->value.dtor->body, ctx);
                    }
                    member = member->next;
                }
                break;
            }
            case SOURCE_ITEM_DECL:
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
