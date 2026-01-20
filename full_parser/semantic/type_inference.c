#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "type_inference.h"
#include "name_resolution.h"
#include "error_reporting.h"
#include "ast_builder.h"

static int GetNumericRank(const NType *type, int *rank_out);
static BaseType RankToBaseType(int rank);
static const char *OpToString(OpType op);
static int IsNullClassType(const NType *type);
static int IsArrayType(const NType *type);
static int IsDynamicArrayType(const NType *type);
static void BuildElementType(const NType *array_type, NType *out);
static void ReportAppendAssignError(NType *left_type, NType *right_type,
                                    SemanticContext *ctx, int line, int column);

/* ============================================================================
   ВЫВОД ТИПОВ ВЫРАЖЕНИЙ
   ============================================================================ */

static NType* InferExpressionTypeInternal(NExpr *expr, SemanticContext *ctx, int report_errors);

NType* InferExpressionType(NExpr *expr, SemanticContext *ctx) {
    return InferExpressionTypeInternal(expr, ctx, 1);
}

NType* InferExpressionTypeSilent(NExpr *expr, SemanticContext *ctx) {
    return InferExpressionTypeInternal(expr, ctx, 0);
}

static NType* InferExpressionTypeInternal(NExpr *expr, SemanticContext *ctx, int report_errors) {

    NType *left_type;
    NType *right_type;
    NType *operand_type;
    NType *result_type;

    if (expr == NULL) {
        return NULL;
    }

    switch (expr->type) {
        case EXPR_INT:
        case EXPR_FLOAT:
        case EXPR_CHAR:
        case EXPR_STRING:
        case EXPR_BOOL:
        case EXPR_NULL:
        case EXPR_NAN:
            return InferLiteralType(expr);
        case EXPR_IDENT: {
            Symbol *sym = LookupSymbol(ctx, expr->value.ident_name);
            if (sym == NULL || sym->kind != SYMBOL_VARIABLE || sym->info.var_info == NULL) {
                if (sym != NULL && sym->kind == SYMBOL_ENUM_ITEM) {
                    return CreateBaseType(TYPE_INT);
                }
                if (ctx != NULL && ctx->current_class != NULL) {
                    ClassInfo *owner = NULL;
                    FieldInfo *field = NULL;
                    ClassInfo *current = ctx->current_class;
                    while (current != NULL) {
                        field = LookupClassField(current, expr->value.ident_name);
                        if (field != NULL) {
                            owner = current;
                            break;
                        }
                        if (current->base_class == NULL) {
                            break;
                        }
                        current = LookupClass(ctx, current->base_class);
                    }
                    if (field != NULL && owner != NULL) {
                        return CopyType(field->type, ctx);
                    }
                }
                if (report_errors && ctx != NULL && ctx->errors != NULL) {
                    SemanticError err = CreateUndefinedVariableError(expr->value.ident_name,
                                                                    expr->line,
                                                                    expr->column);
                    AddError(ctx->errors, &err);
                }
                return NULL;
            }
            return CopyType(sym->info.var_info->type, ctx);
        }
        case EXPR_BINARY_OP:
            left_type = InferExpressionTypeInternal(expr->value.binary.left, ctx, report_errors);
            right_type = InferExpressionTypeInternal(expr->value.binary.right, ctx, report_errors);
            return InferBinaryOperationType(expr->value.binary.op,
                                            left_type,
                                            right_type,
                                            ctx,
                                            expr->line,
                                            expr->column);
        case EXPR_ASSIGN:
            left_type = InferExpressionTypeInternal(expr->value.binary.left, ctx, report_errors);
            right_type = InferExpressionTypeInternal(expr->value.binary.right, ctx, report_errors);
            if (left_type == NULL || right_type == NULL) {
                return NULL;
            }
            if (expr->value.binary.op == OP_BITWISE_NOT_ASSIGN) {
                if (!CheckAppendAssignment(left_type, right_type, ctx, expr->line, expr->column)) {
                    return NULL;
                }
            } else if (!CanAssign(left_type, right_type)) {
                return NULL;
            }
            return CopyType(left_type, ctx);
        case EXPR_UNARY_OP:
            operand_type = InferExpressionTypeInternal(expr->value.unary.operand, ctx, report_errors);
            return InferUnaryOperationType(expr->value.unary.op, operand_type);
        case EXPR_FUNC_CALL: {
            int ambiguous = 0;
            FunctionInfo *func = LookupFunctionOverload(ctx,
                                                        expr->value.func_call.func_name,
                                                        expr->value.func_call.args,
                                                        expr->value.func_call.arg_count,
                                                        &ambiguous);
            if (func == NULL) {
                if (ctx != NULL && ctx->current_class != NULL) {
                    ClassInfo *current = ctx->current_class;
                    MethodInfo *method = NULL;
                    while (current != NULL) {
                        method = LookupClassMethod(current, expr->value.func_call.func_name);
                        if (method != NULL) {
                            break;
                        }
                        if (current->base_class == NULL) {
                            break;
                        }
                        current = LookupClass(ctx, current->base_class);
                    }
                    if (method != NULL) {
                        if (method->return_type == NULL) {
                            return CreateBaseType(TYPE_VOID);
                        }
                        return CopyType(method->return_type, ctx);
                    }
                }
                if (report_errors && ctx != NULL && ctx->errors != NULL) {
                    SemanticError err;
                    if (ambiguous) {
                        err = CreateCustomError(SEMANTIC_ERROR_OTHER,
                                                "Ambiguous function call",
                                                expr->line,
                                                expr->column);
                    } else if (HasFunctionName(ctx, expr->value.func_call.func_name)) {
                        err = CreateCustomError(SEMANTIC_ERROR_OTHER,
                                                "No matching overload for function call",
                                                expr->line,
                                                expr->column);
                    } else {
                        err = CreateUndefinedFunctionError(expr->value.func_call.func_name,
                                                          expr->line,
                                                          expr->column);
                    }
                    AddError(ctx->errors, &err);
                }
                return NULL;
            }
            if (func->return_type == NULL) {
                return CreateBaseType(TYPE_VOID);
            }
            return CopyType(func->return_type, ctx);
        }
        case EXPR_METHOD_CALL: {
            NType *obj_type = InferExpressionTypeInternal(expr->value.member_access.object, ctx, report_errors);
            if (obj_type == NULL) {
                return NULL;
            }
            if (obj_type->kind != TYPE_KIND_CLASS && obj_type->kind != TYPE_KIND_CLASS_ARRAY) {
                return NULL;
            }
            if (obj_type->kind == TYPE_KIND_CLASS_ARRAY) {
                return NULL;
            }
            ClassInfo *class_info = LookupClass(ctx, obj_type->class_name);
            if (class_info == NULL) {
                if (report_errors && ctx != NULL && ctx->errors != NULL) {
                    SemanticError err = CreateUndefinedClassError(obj_type->class_name,
                                                                  expr->line,
                                                                  expr->column);
                    AddError(ctx->errors, &err);
                }
                return NULL;
            }
            MethodInfo *method = LookupClassMethodInHierarchy(ctx,
                                                              class_info,
                                                              expr->value.member_access.member_name);
            if (method == NULL) {
                if (report_errors && ctx != NULL && ctx->errors != NULL) {
                    SemanticError err = CreateMethodNotFoundError(expr->value.member_access.member_name,
                                                                  obj_type->class_name,
                                                                  expr->line,
                                                                  expr->column);
                    AddError(ctx->errors, &err);
                }
                return NULL;
            }
            if (method->return_type == NULL) {
                return CreateBaseType(TYPE_VOID);
            }
            return CopyType(method->return_type, ctx);
        }
        case EXPR_SUPER_METHOD: {
            ClassInfo *current = (ctx != NULL) ? ctx->current_class : NULL;
            ClassInfo *base_info;
            if (current == NULL || current->base_class == NULL) {
                return NULL;
            }
            base_info = LookupClass(ctx, current->base_class);
            if (base_info == NULL) {
                return NULL;
            }
            MethodInfo *method = LookupClassMethodInHierarchy(ctx,
                                                              base_info,
                                                              expr->value.member_access.member_name);
            if (method == NULL) {
                return NULL;
            }
            if (method->return_type == NULL) {
                return CreateBaseType(TYPE_VOID);
            }
            return CopyType(method->return_type, ctx);
        }
        case EXPR_MEMBER_ACCESS: {
            NType *obj_type = InferExpressionTypeInternal(expr->value.member_access.object, ctx, report_errors);
            if (obj_type == NULL) {
                return NULL;
            }
            if (obj_type->kind != TYPE_KIND_CLASS && obj_type->kind != TYPE_KIND_CLASS_ARRAY) {
                return NULL;
            }
            if (obj_type->kind == TYPE_KIND_CLASS_ARRAY) {
                return NULL;
            }
            ClassInfo *class_info = LookupClass(ctx, obj_type->class_name);
            if (class_info == NULL) {
                if (report_errors && ctx != NULL && ctx->errors != NULL) {
                    SemanticError err = CreateUndefinedClassError(obj_type->class_name,
                                                                  expr->line,
                                                                  expr->column);
                    AddError(ctx->errors, &err);
                }
                return NULL;
            }
            FieldInfo *field = LookupClassFieldInHierarchy(ctx,
                                                           class_info,
                                                           expr->value.member_access.member_name);
            if (field == NULL) {
                if (report_errors && ctx != NULL && ctx->errors != NULL) {
                    SemanticError err = CreateFieldNotFoundError(expr->value.member_access.member_name,
                                                                obj_type->class_name,
                                                                expr->line,
                                                                expr->column);
                    AddError(ctx->errors, &err);
                }
                return NULL;
            }
            return CopyType(field->type, ctx);
        }
        case EXPR_ARRAY_ACCESS: {
            NType *array_type = InferExpressionTypeInternal(expr->value.array_access.array, ctx, report_errors);
            if (array_type == NULL) {
                return NULL;
            }
            if (expr->value.array_access.index_end != NULL) {
                return CopyType(array_type, ctx);
            }
            if (array_type->kind == TYPE_KIND_BASE_ARRAY) {
                return CreateBaseType(array_type->base_type);
            }
            if (array_type->kind == TYPE_KIND_CLASS_ARRAY) {
                return CreateClassType(array_type->class_name);
            }
            return NULL;
        }
        case EXPR_NEW:
            return CopyType(expr->value.new_expr.type, ctx);
        case EXPR_PAREN:
            return InferExpressionTypeInternal(expr->value.inner_expr, ctx, report_errors);
        case EXPR_THIS:
            if (ctx != NULL && ctx->current_class != NULL) {
                return CreateClassType(ctx->current_class->name);
            }
            return NULL;
        case EXPR_SUPER:
            if (ctx != NULL && ctx->current_class != NULL &&
                ctx->current_class->base_class != NULL) {
                return CreateClassType(ctx->current_class->base_class);
            }
            return NULL;
        default:
            if (report_errors && ctx != NULL && ctx->errors != NULL) {
                SemanticError err = CreateCustomError(SEMANTIC_ERROR_OTHER,
                                                     "Unsupported expression in type inference",
                                                     expr->line,
                                                     expr->column);
                AddError(ctx->errors, &err);
            }
            return NULL;
    }
}

NType* InferLiteralType(NExpr *expr) {

    if (expr == NULL) {
        return NULL;
    }
    switch (expr->type) {
        case EXPR_INT:
            return CreateBaseType(TYPE_INT);
        case EXPR_FLOAT:
            return CreateBaseType(TYPE_FLOAT);
        case EXPR_CHAR:
            return CreateBaseType(TYPE_CHAR);
        case EXPR_STRING:
            return CreateBaseType(TYPE_STRING);
        case EXPR_BOOL:
            return CreateBaseType(TYPE_BOOL);
        case EXPR_NULL:
            return CreateClassType(NULL);
        case EXPR_NAN:
            return CreateBaseType(TYPE_FLOAT);
        default:
            return NULL;
    }
}

NType* InferBinaryOperationType(OpType op, NType *left_type, NType *right_type,
                                SemanticContext *ctx, int line, int column) {

    if (left_type == NULL || right_type == NULL) {
        return NULL;
    }
    (void)ctx;
    (void)line;
    (void)column;

    switch (op) {
        case OP_PLUS:
        case OP_MINUS:
        case OP_MUL:
        case OP_DIV:
            if (IsNumericType(left_type) && IsNumericType(right_type)) {
                int left_rank = 0;
                int right_rank = 0;
                int max_rank;
                BaseType result_base;
                if (!GetNumericRank(left_type, &left_rank) ||
                    !GetNumericRank(right_type, &right_rank)) {
                    return NULL;
                }
                max_rank = (left_rank > right_rank) ? left_rank : right_rank;
                result_base = RankToBaseType(max_rank);
                return CreateBaseType(result_base);
            }
            return NULL;
        case OP_EQ:
        case OP_NEQ:
            if (!AreTypesCompatible(left_type, right_type, 0) &&
                !(IsNullClassType(left_type) &&
                  (right_type->kind == TYPE_KIND_CLASS || right_type->kind == TYPE_KIND_CLASS_ARRAY)) &&
                !(IsNullClassType(right_type) &&
                  (left_type->kind == TYPE_KIND_CLASS || left_type->kind == TYPE_KIND_CLASS_ARRAY))) {
                return NULL;
            }
            return CreateBaseType(TYPE_BOOL);
        case OP_LT:
        case OP_GT:
        case OP_LE:
        case OP_GE:
            if (!IsNumericType(left_type) || !IsNumericType(right_type)) {
                return NULL;
            }
            return CreateBaseType(TYPE_BOOL);
        case OP_AND:
        case OP_OR:
            if (!((IsBooleanType(left_type) && IsBooleanType(right_type)) ||
                  (IsNumericType(left_type) && IsNumericType(right_type)))) {
                return NULL;
            }
            return CreateBaseType(TYPE_BOOL);
        case OP_ASSIGN:
        case OP_PLUS_ASSIGN:
        case OP_MINUS_ASSIGN:
        case OP_MUL_ASSIGN:
        case OP_DIV_ASSIGN:
            if (CanAssign(left_type, right_type)) {
                return CopyType(left_type, ctx);
            }
            return NULL;
        default:
            return NULL;
    }
}

NType* InferUnaryOperationType(OpType op, NType *operand_type) {

    if (operand_type == NULL) {
        return NULL;
    }

    switch (op) {
        case OP_MINUS:
        case OP_PLUS:
            if (IsIntegralType(operand_type)) {
                return CreateBaseType(TYPE_INT);
            }
            if (IsFloatingPointType(operand_type) &&
                operand_type->kind == TYPE_KIND_BASE) {
                return CreateBaseType(operand_type->base_type);
            }
            return NULL;
        case OP_NOT:
            if (IsBooleanType(operand_type) || IsNumericType(operand_type)) {
                return CreateBaseType(TYPE_BOOL);
            }
            return NULL;
        default:
            return NULL;
    }
}

/* ============================================================================
   ПРОВЕРКА СОВМЕСТИМОСТИ ТИПОВ
   ============================================================================ */

int AreTypesCompatible(NType *type1, NType *type2, int strict) {

    if (type1 == NULL || type2 == NULL) {
        return 0;
    }

    if (strict) {
        return TypesEqual(type1, type2);
    }

    if ((type1->kind == TYPE_KIND_BASE_ARRAY || type1->kind == TYPE_KIND_CLASS_ARRAY) ||
        (type2->kind == TYPE_KIND_BASE_ARRAY || type2->kind == TYPE_KIND_CLASS_ARRAY)) {
        if (type1->kind != type2->kind) {
            return 0;
        }
        if (type1->array_decl != NULL || type2->array_decl != NULL) {
            if (type1->array_decl == NULL || type2->array_decl == NULL) {
                return 0;
            }
            if (type1->array_decl->has_size != type2->array_decl->has_size) {
                return 0;
            }
            if (type1->array_decl->has_size &&
                type1->array_decl->size != type2->array_decl->size) {
                return 0;
            }
        }
        if (type1->kind == TYPE_KIND_BASE_ARRAY) {
            NType elem1;
            NType elem2;
            memset(&elem1, 0, sizeof(NType));
            memset(&elem2, 0, sizeof(NType));
            elem1.kind = TYPE_KIND_BASE;
            elem1.base_type = type1->base_type;
            elem2.kind = TYPE_KIND_BASE;
            elem2.base_type = type2->base_type;
            return AreTypesCompatible(&elem1, &elem2, 0);
        }
        if (type1->kind == TYPE_KIND_CLASS_ARRAY) {
            NType elem1;
            NType elem2;
            memset(&elem1, 0, sizeof(NType));
            memset(&elem2, 0, sizeof(NType));
            elem1.kind = TYPE_KIND_CLASS;
            elem1.class_name = type1->class_name;
            elem2.kind = TYPE_KIND_CLASS;
            elem2.class_name = type2->class_name;
            return AreTypesCompatible(&elem1, &elem2, 0);
        }
    }

    if (TypesEqual(type1, type2)) {
        return 1;
    }

    if (IsNumericType(type1) && IsNumericType(type2)) {
        return 1;
    }

    if ((type1->kind == TYPE_KIND_CLASS || type1->kind == TYPE_KIND_CLASS_ARRAY) &&
        (type2->kind == TYPE_KIND_CLASS || type2->kind == TYPE_KIND_CLASS_ARRAY)) {
        if (type1->class_name != NULL && type2->class_name != NULL &&
            strcmp(type1->class_name, type2->class_name) == 0) {
            return 1;
        }
        return 0;
    }

    return 0;
}

int CanAssign(NType *target_type, NType *source_type) {

    if (target_type == NULL || source_type == NULL) {
        return 0;
    }

    if (TypesEqual(target_type, source_type)) {
        return 1;
    }

    if ((target_type->kind == TYPE_KIND_BASE_ARRAY || target_type->kind == TYPE_KIND_CLASS_ARRAY) ||
        (source_type->kind == TYPE_KIND_BASE_ARRAY || source_type->kind == TYPE_KIND_CLASS_ARRAY)) {
        if (target_type->kind != source_type->kind) {
            return 0;
        }
        if (target_type->array_decl != NULL || source_type->array_decl != NULL) {
            if (target_type->array_decl == NULL || source_type->array_decl == NULL) {
                return 0;
            }
            if (target_type->array_decl->has_size != source_type->array_decl->has_size) {
                return 0;
            }
            if (target_type->array_decl->has_size &&
                target_type->array_decl->size != source_type->array_decl->size) {
                return 0;
            }
        }
        if (target_type->kind == TYPE_KIND_BASE_ARRAY) {
            NType elem_t;
            NType elem_s;
            memset(&elem_t, 0, sizeof(NType));
            memset(&elem_s, 0, sizeof(NType));
            elem_t.kind = TYPE_KIND_BASE;
            elem_t.base_type = target_type->base_type;
            elem_s.kind = TYPE_KIND_BASE;
            elem_s.base_type = source_type->base_type;
            return CanAssign(&elem_t, &elem_s);
        }
        if (target_type->kind == TYPE_KIND_CLASS_ARRAY) {
            NType elem_t;
            NType elem_s;
            memset(&elem_t, 0, sizeof(NType));
            memset(&elem_s, 0, sizeof(NType));
            elem_t.kind = TYPE_KIND_CLASS;
            elem_t.class_name = target_type->class_name;
            elem_s.kind = TYPE_KIND_CLASS;
            elem_s.class_name = source_type->class_name;
            return CanAssign(&elem_t, &elem_s);
        }
    }

    if (IsFloatingPointType(target_type) && IsIntegralType(source_type)) {
        return 1;
    }

    if (IsIntegralType(target_type) && IsFloatingPointType(source_type)) {
        return 0;
    }

    if ((target_type->kind == TYPE_KIND_CLASS || target_type->kind == TYPE_KIND_CLASS_ARRAY) &&
        (source_type->kind == TYPE_KIND_CLASS || source_type->kind == TYPE_KIND_CLASS_ARRAY)) {
        if (target_type->class_name != NULL && source_type->class_name != NULL &&
            strcmp(target_type->class_name, source_type->class_name) == 0) {
            return 1;
        }
        return 0;
    }

    if ((target_type->kind == TYPE_KIND_CLASS || target_type->kind == TYPE_KIND_CLASS_ARRAY) &&
        source_type->kind == TYPE_KIND_CLASS && source_type->class_name == NULL) {
        return 1;
    }

    return 0;
}

int IsArgumentCompatibleWithParameter(NType *param_type, NType *arg_type, int is_ref) {

    if (param_type == NULL || arg_type == NULL) {
        return 0;
    }

    if (is_ref) {
        return TypesEqual(param_type, arg_type);
    }

    return AreTypesCompatible(param_type, arg_type, 0);
}

/* ============================================================================
   СПЕЦИАЛЬНЫЕ ПРОВЕРКИ ТИПОВ
   ============================================================================ */

static int GetNumericRank(const NType *type, int *rank_out) {
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

static BaseType RankToBaseType(int rank) {
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

static const char *OpToString(OpType op) {
    switch (op) {
        case OP_PLUS: return "+";
        case OP_MINUS: return "-";
        case OP_MUL: return "*";
        case OP_DIV: return "/";
        case OP_LT: return "<";
        case OP_GT: return ">";
        case OP_LE: return "<=";
        case OP_GE: return ">=";
        case OP_EQ: return "==";
        case OP_NEQ: return "!=";
        case OP_AND: return "&&";
        case OP_OR: return "||";
        case OP_NOT: return "!";
        case OP_ASSIGN: return "=";
        case OP_PLUS_ASSIGN: return "+=";
        case OP_MINUS_ASSIGN: return "-=";
        case OP_MUL_ASSIGN: return "*=";
        case OP_DIV_ASSIGN: return "/=";
        case OP_BITWISE_NOT_ASSIGN: return "~=";
        default: return "?";
    }
}

static int IsNullClassType(const NType *type) {
    if (type == NULL) {
        return 0;
    }
    return type->kind == TYPE_KIND_CLASS && type->class_name == NULL;
}

static int IsArrayType(const NType *type) {
    if (type == NULL) {
        return 0;
    }
    return type->kind == TYPE_KIND_BASE_ARRAY || type->kind == TYPE_KIND_CLASS_ARRAY;
}

static int IsDynamicArrayType(const NType *type) {
    if (!IsArrayType(type)) {
        return 0;
    }
    if (type->array_decl == NULL) {
        return 1;
    }
    return type->array_decl->has_size ? 0 : 1;
}

static void BuildElementType(const NType *array_type, NType *out) {
    if (out == NULL) {
        return;
    }
    memset(out, 0, sizeof(NType));
    if (array_type == NULL) {
        return;
    }
    if (array_type->kind == TYPE_KIND_BASE_ARRAY) {
        out->kind = TYPE_KIND_BASE;
        out->base_type = array_type->base_type;
    } else if (array_type->kind == TYPE_KIND_CLASS_ARRAY) {
        out->kind = TYPE_KIND_CLASS;
        out->class_name = array_type->class_name;
    }
}

static void ReportAppendAssignError(NType *left_type, NType *right_type,
                                    SemanticContext *ctx, int line, int column) {
    char left_copy[128];
    char right_copy[128];
    const char *left_str;
    const char *right_str;

    if (ctx == NULL || ctx->errors == NULL) {
        return;
    }

    left_str = TypeToString(left_type);
    right_str = TypeToString(right_type);

    snprintf(left_copy, sizeof(left_copy), "%s", left_str ? left_str : "unknown");
    snprintf(right_copy, sizeof(right_copy), "%s", right_str ? right_str : "unknown");

    {
        SemanticError err = CreateInvalidOperandsError("~=",
                                                      left_copy,
                                                      right_copy,
                                                      line,
                                                      column);
        AddError(ctx->errors, &err);
    }
}

int CheckAppendAssignment(NType *left_type, NType *right_type,
                          SemanticContext *ctx, int line, int column) {
    NType left_elem;
    NType right_elem;

    if (left_type == NULL || right_type == NULL) {
        return 0;
    }
    if (!IsDynamicArrayType(left_type)) {
        ReportAppendAssignError(left_type, right_type, ctx, line, column);
        return 0;
    }

    BuildElementType(left_type, &left_elem);
    if (!IsArrayType(right_type)) {
        if (!CanAssign(&left_elem, right_type)) {
            ReportAppendAssignError(left_type, right_type, ctx, line, column);
            return 0;
        }
        return 1;
    }

    BuildElementType(right_type, &right_elem);
    if (!AreTypesCompatible(&left_elem, &right_elem, 0)) {
        ReportAppendAssignError(left_type, right_type, ctx, line, column);
        return 0;
    }
    return 1;
}

int IsNumericType(NType *type) {

    if (type == NULL) {
        return 0;
    }
    if (type->kind == TYPE_KIND_BASE || type->kind == TYPE_KIND_BASE_ARRAY) {
        switch (type->base_type) {
            case TYPE_INT:
            case TYPE_FLOAT:
            case TYPE_DOUBLE:
            case TYPE_REAL:
            case TYPE_CHAR:
                return 1;
            default:
                return 0;
        }
    }
    return 0;
}

int IsIntegralType(NType *type) {

    if (type == NULL) {
        return 0;
    }
    if (type->kind == TYPE_KIND_BASE || type->kind == TYPE_KIND_BASE_ARRAY) {
        switch (type->base_type) {
            case TYPE_INT:
            case TYPE_CHAR:
                return 1;
            default:
                return 0;
        }
    }
    return 0;
}

int IsFloatingPointType(NType *type) {

    if (type == NULL) {
        return 0;
    }
    if (type->kind == TYPE_KIND_BASE || type->kind == TYPE_KIND_BASE_ARRAY) {
        switch (type->base_type) {
            case TYPE_FLOAT:
            case TYPE_DOUBLE:
            case TYPE_REAL:
                return 1;
            default:
                return 0;
        }
    }
    return 0;
}

int IsBooleanType(NType *type) {

    if (type == NULL) {
        return 0;
    }
    if (type->kind == TYPE_KIND_BASE || type->kind == TYPE_KIND_BASE_ARRAY) {
        return type->base_type == TYPE_BOOL ? 1 : 0;
    }
    return 0;
}

int CanBeArrayIndex(NType *type) {

    return IsIntegralType(type);
}

int CanBeCondition(NType *type) {

    if (type == NULL) {
        return 0;
    }
    if (IsBooleanType(type)) {
        return 1;
    }
    if (IsNumericType(type)) {
        return 1;
    }
    return 0;
}

NType* CopyType(NType *type, SemanticContext *ctx) {

    NType *copy;
    NArrayDecl *array_copy = NULL;

    if (type == NULL) {
        return NULL;
    }

    copy = (NType*)malloc(sizeof(NType));
    if (copy == NULL) {
        if (ctx != NULL && ctx->errors != NULL) {
            SemanticError err = CreateCustomError(SEMANTIC_ERROR_OTHER, "Out of memory", 0, 0);
            AddError(ctx->errors, &err);
        }
        return NULL;
    }
    memset(copy, 0, sizeof(NType));
    copy->kind = type->kind;
    copy->base_type = type->base_type;

    if (type->class_name != NULL) {
        copy->class_name = (char*)malloc(strlen(type->class_name) + 1);
        if (copy->class_name == NULL) {
            if (ctx != NULL && ctx->errors != NULL) {
                SemanticError err = CreateCustomError(SEMANTIC_ERROR_OTHER, "Out of memory", 0, 0);
                AddError(ctx->errors, &err);
            }
            free(copy);
            return NULL;
        }
        strcpy(copy->class_name, type->class_name);
    }

    if (type->array_decl != NULL) {
        array_copy = (NArrayDecl*)malloc(sizeof(NArrayDecl));
        if (array_copy == NULL) {
            if (ctx != NULL && ctx->errors != NULL) {
                SemanticError err = CreateCustomError(SEMANTIC_ERROR_OTHER, "Out of memory", 0, 0);
                AddError(ctx->errors, &err);
            }
            free(copy->class_name);
            free(copy);
            return NULL;
        }
        array_copy->has_size = type->array_decl->has_size;
        array_copy->size = type->array_decl->size;
        copy->array_decl = array_copy;
    }

    return copy;
}

NType* CopyTypeNoContext(NType *type) {
    return CopyType(type, NULL);
}

int TypesEqual(NType *type1, NType *type2) {

    if (type1 == NULL && type2 == NULL) {
        return 1;
    }
    if (type1 == NULL || type2 == NULL) {
        return 0;
    }

    if (type1->kind != type2->kind) {
        return 0;
    }
    if (type1->base_type != type2->base_type) {
        return 0;
    }

    if (type1->kind == TYPE_KIND_CLASS || type1->kind == TYPE_KIND_CLASS_ARRAY) {
        if (type1->class_name == NULL && type2->class_name == NULL) {
            return 1;
        }
        if (type1->class_name == NULL || type2->class_name == NULL) {
            return 0;
        }
        if (strcmp(type1->class_name, type2->class_name) != 0) {
            return 0;
        }
    }

    if ((type1->kind == TYPE_KIND_BASE_ARRAY || type1->kind == TYPE_KIND_CLASS_ARRAY) ||
        (type2->kind == TYPE_KIND_BASE_ARRAY || type2->kind == TYPE_KIND_CLASS_ARRAY)) {
        if (type1->array_decl == NULL && type2->array_decl == NULL) {
            return 1;
        }
        if (type1->array_decl == NULL || type2->array_decl == NULL) {
            return 0;
        }
        if (type1->array_decl->has_size != type2->array_decl->has_size) {
            return 0;
        }
        if (type1->array_decl->has_size &&
            type1->array_decl->size != type2->array_decl->size) {
            return 0;
        }
    }

    return 1;
}

const char* TypeToString(NType *type) {

    static char buffer[128];
    const char *base = "unknown";

    if (type == NULL) {
        return "unknown";
    }

    if (type->kind == TYPE_KIND_CLASS || type->kind == TYPE_KIND_CLASS_ARRAY) {
        base = (type->class_name != NULL) ? type->class_name : "class";
    } else if (type->kind == TYPE_KIND_BASE || type->kind == TYPE_KIND_BASE_ARRAY) {
        switch (type->base_type) {
            case TYPE_INT: base = "int"; break;
            case TYPE_FLOAT: base = "float"; break;
            case TYPE_STRING: base = "string"; break;
            case TYPE_CHAR: base = "char"; break;
            case TYPE_BOOL: base = "bool"; break;
            case TYPE_DOUBLE: base = "double"; break;
            case TYPE_REAL: base = "real"; break;
            case TYPE_VOID: base = "void"; break;
            default: base = "unknown"; break;
        }
    }

    if (type->kind == TYPE_KIND_BASE_ARRAY || type->kind == TYPE_KIND_CLASS_ARRAY) {
        snprintf(buffer, sizeof(buffer), "%s[]", base);
        return buffer;
    }

    return base;
}
