#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "semantic_passes.h"
#include "name_resolution.h"
#include "type_inference.h"
#include "error_reporting.h"
#include "jvm_layout.h"
#include "../ast_builder.h"


static int ReportOutOfMemory(SemanticContext *ctx) {
    if (ctx != NULL && ctx->errors != NULL) {
        SemanticError err = CreateCustomError(SEMANTIC_ERROR_OTHER, "Out of memory", 0, 0);
        AddError(ctx->errors, &err);
    }
    return 1;
}

static const char *AccessSpecToString(AccessSpec access) {
    switch (access) {
        case ACCESS_PUBLIC:
            return "public";
        case ACCESS_PRIVATE:
            return "private";
        case ACCESS_PROTECTED:
            return "protected";
        default:
            return "unknown";
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

static int ValidateArraySizeType(const NType *type, SemanticContext *ctx, int line, int column) {
    const int max_size = 1000000;
    if (type == NULL || type->array_decl == NULL || !type->array_decl->has_size) {
        return 1;
    }
    if (type->array_decl->size < 0) {
        if (ctx && ctx->errors) {
            SemanticError err = CreateInvalidArraySizeError("negative size", line, column);
            AddError(ctx->errors, &err);
        }
        return 0;
    }
    if (type->array_decl->size > max_size) {
        if (ctx && ctx->errors) {
            SemanticError err = CreateInvalidArraySizeError("size exceeds limit", line, column);
            AddError(ctx->errors, &err);
        }
        return 0;
    }
    return 1;
}

static int IsValidLValueExpr(NExpr *expr) {
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
            return IsValidLValueExpr(expr->value.inner_expr);
        default:
            return 0;
    }
}

static int IsSameOrBaseClass(SemanticContext *ctx, const char *current_class, const char *target_class) {
    ClassInfo *cls;

    if (ctx == NULL || current_class == NULL || target_class == NULL) {
        return 0;
    }
    if (strcmp(current_class, target_class) == 0) {
        return 1;
    }
    cls = LookupClass(ctx, current_class);
    while (cls != NULL && cls->base_class != NULL) {
        if (strcmp(cls->base_class, target_class) == 0) {
            return 1;
        }
        cls = LookupClass(ctx, cls->base_class);
    }
    return 0;
}

static int EnsureTypeDefined(const NType *type, SemanticContext *ctx, int line, int column) {
    if (type == NULL || ctx == NULL) {
        return 1;
    }
    if (type->kind == TYPE_KIND_CLASS || type->kind == TYPE_KIND_CLASS_ARRAY) {
        if (type->class_name == NULL || LookupClass(ctx, type->class_name) == NULL) {
            if (ctx->errors != NULL) {
                SemanticError err = CreateUndefinedTypeError(type->class_name ? type->class_name : "class",
                                                            line,
                                                            column);
                AddError(ctx->errors, &err);
            }
            return 0;
        }
    } else if (type->kind == TYPE_KIND_ENUM || type->kind == TYPE_KIND_ENUM_ARRAY) {
        if (type->enum_name == NULL || LookupEnum(ctx, type->enum_name) == NULL) {
            if (ctx->errors != NULL) {
                SemanticError err = CreateUndefinedTypeError(type->enum_name ? type->enum_name : "enum",
                                                            line,
                                                            column);
                AddError(ctx->errors, &err);
            }
            return 0;
        }
    }
    return 1;
}

static int CheckStaticArrayInitSize(const NType *decl_type, const NInitializer *init,
                                    SemanticContext *ctx, int line, int column) {
    if (decl_type == NULL || init == NULL || !init->is_array) {
        return 0;
    }
    if (decl_type->array_decl == NULL || !decl_type->array_decl->has_size) {
        return 0;
    }
    if (init->array_init.count != decl_type->array_decl->size) {
        if (ctx && ctx->errors) {
            SemanticError err = CreateCustomError(SEMANTIC_ERROR_INVALID_OPERANDS,
                                                  "Static array initializer length mismatch",
                                                  line,
                                                  column);
            AddError(ctx->errors, &err);
        }
        return 1;
    }
    return 0;
}

static void GetInitializerLocation(const NInitializer *init, int fallback_line, int fallback_column,
                                   int *out_line, int *out_column) {
    int line = fallback_line;
    int column = fallback_column;
    if (init != NULL) {
        if (init->is_array && init->array_init.elements != NULL && init->array_init.count > 0) {
            NExpr *first = init->array_init.elements[0];
            if (first != NULL) {
                line = first->line;
                column = first->column;
            }
        } else if (!init->is_array && init->expr != NULL) {
            line = init->expr->line;
            column = init->expr->column;
        }
    }
    if (out_line != NULL) {
        *out_line = line;
    }
    if (out_column != NULL) {
        *out_column = column;
    }
}
static ClassInfo *FindFieldOwnerInHierarchy(SemanticContext *ctx, ClassInfo *class_info,
                                            const char *field_name) {
    ClassInfo *current = class_info;
    while (current != NULL) {
        if (LookupClassField(current, field_name) != NULL) {
            return current;
        }
        if (current->base_class == NULL || ctx == NULL) {
            break;
        }
        current = LookupClass(ctx, current->base_class);
    }
    return NULL;
}

static ClassInfo *FindMethodOwnerInHierarchy(SemanticContext *ctx, ClassInfo *class_info,
                                             const char *method_name) {
    ClassInfo *current = class_info;
    while (current != NULL) {
        if (LookupClassMethod(current, method_name) != NULL) {
            return current;
        }
        if (current->base_class == NULL || ctx == NULL) {
            break;
        }
        current = LookupClass(ctx, current->base_class);
    }
    return NULL;
}

static int IsInsideHierarchyAccess(SemanticContext *ctx, const NExpr *obj, const char *target_class) {
    if (ctx == NULL || ctx->current_class == NULL || target_class == NULL) {
        return 0;
    }
    if (obj != NULL && (obj->type == EXPR_THIS || obj->type == EXPR_SUPER)) {
        return 1;
    }
    return IsSameOrBaseClass(ctx, ctx->current_class->name, target_class);
}

static int IsInsideExactClass(SemanticContext *ctx, const NExpr *obj, const char *target_class) {
    if (ctx == NULL || ctx->current_class == NULL || target_class == NULL) {
        return 0;
    }
    if (obj != NULL && (obj->type == EXPR_THIS || obj->type == EXPR_SUPER)) {
        return 1;
    }
    return strcmp(ctx->current_class->name, target_class) == 0;
}

static int AreParamListsEqual(const NParamList *a, const NParamList *b) {
    int count_a = 0;
    int count_b = 0;
    if (a != NULL) {
        count_a = a->count;
    }
    if (b != NULL) {
        count_b = b->count;
    }
    if (count_a != count_b) {
        return 0;
    }
    for (int i = 0; i < count_a; i++) {
        NParam *pa = a->params[i];
        NParam *pb = b->params[i];
        if (pa == NULL || pb == NULL) {
            return 0;
        }
        if (pa->is_ref != pb->is_ref) {
            return 0;
        }
        if (!TypesEqual(pa->param_type, pb->param_type)) {
            return 0;
        }
    }
    return 1;
}

static int MethodsHaveSameSignature(const MethodInfo *a, const MethodInfo *b) {
    if (a == NULL || b == NULL || a->name == NULL || b->name == NULL) {
        return 0;
    }
    if (strcmp(a->name, b->name) != 0) {
        return 0;
    }
    return AreParamListsEqual(a->params, b->params);
}

static MethodInfo *FindMethodInClassBySignature(ClassInfo *class_info, const char *name,
                                                const NParamList *params) {
    if (class_info == NULL || name == NULL) {
        return NULL;
    }
    for (int i = 0; i < class_info->method_count; i++) {
        MethodInfo *method = class_info->methods[i];
        if (method == NULL || method->name == NULL) {
            continue;
        }
        if (strcmp(method->name, name) != 0) {
            continue;
        }
        if (AreParamListsEqual(method->params, params)) {
            return method;
        }
    }
    return NULL;
}

static MethodInfo *FindMethodInHierarchyBySignature(SemanticContext *ctx, ClassInfo *class_info,
                                                    const char *name, const NParamList *params) {
    ClassInfo *current = class_info;
    while (current != NULL) {
        MethodInfo *method = FindMethodInClassBySignature(current, name, params);
        if (method != NULL) {
            return method;
        }
        if (current->base_class == NULL || ctx == NULL) {
            break;
        }
        current = LookupClass(ctx, current->base_class);
    }
    return NULL;
}

static int AreCtorSignaturesCompatible(const NCtorDef *a, const NCtorDef *b) {
    int count_a = 0;
    int count_b = 0;

    if (a == NULL || b == NULL) {
        return 0;
    }
    if (a->params != NULL) {
        count_a = a->params->count;
    }
    if (b->params != NULL) {
        count_b = b->params->count;
    }
    if (count_a != count_b) {
        return 0;
    }
    for (int i = 0; i < count_a; i++) {
        NParam *pa = a->params->params[i];
        NParam *pb = b->params->params[i];
        if (pa == NULL || pb == NULL) {
            return 0;
        }
        if (pa->is_ref != pb->is_ref) {
            return 0;
        }
        if (!TypesEqual(pa->param_type, pb->param_type)) {
            return 0;
        }
    }
    return 1;
}

static int AddConstructorToClass(ClassInfo *class_info, NCtorDef *ctor, SemanticContext *ctx) {
    NCtorDef **grown;

    if (class_info == NULL || ctor == NULL) {
        return 1;
    }
    for (int i = 0; i < class_info->constructor_count; i++) {
        NCtorDef *prev = class_info->constructors[i];
        if (prev == NULL) {
            continue;
        }
        if (AreCtorSignaturesCompatible(prev, ctor)) {
            if (ctx != NULL && ctx->errors != NULL) {
                SemanticError err = CreateCustomError(SEMANTIC_ERROR_OTHER,
                                                      "Duplicate constructor in class",
                                                      0,
                                                      0);
                AddError(ctx->errors, &err);
            }
            return 1;
        }
    }
    grown = (NCtorDef**)realloc(class_info->constructors,
                                sizeof(NCtorDef*) * (size_t)(class_info->constructor_count + 1));
    if (grown == NULL) {
        return ReportOutOfMemory(ctx);
    }
    class_info->constructors = grown;
    class_info->constructors[class_info->constructor_count] = ctor;
    class_info->constructor_count += 1;
    return 0;
}

static int CanMatchConstructor(const NCtorDef *ctor, NExpr **args, int arg_count,
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
        if (param->is_ref && !IsValidLValueExpr(arg)) {
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

static NCtorDef *LookupConstructorOverload(ClassInfo *class_info, NExpr **args, int arg_count,
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
        if (!CanMatchConstructor(ctor, args, arg_count, ctx, &score)) {
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

static NType *GetForeachElementType(NType *collection_type) {
    if (collection_type == NULL) {
        return NULL;
    }
    if (collection_type->kind == TYPE_KIND_BASE_ARRAY) {
        return CreateBaseType(collection_type->base_type);
    }
    if (collection_type->kind == TYPE_KIND_CLASS_ARRAY) {
        return CreateClassType(collection_type->class_name);
    }
    if (collection_type->kind == TYPE_KIND_ENUM_ARRAY) {
        return CreateEnumType(collection_type->enum_name);
    }
    return NULL;
}

static int CheckParamDefaultValues(NParamList *params, SemanticContext *ctx) {
    int had_error = 0;
    if (params == NULL || ctx == NULL) {
        return 0;
    }
    for (int i = 0; i < params->count; i++) {
        NParam *param = params->params[i];
        if (param == NULL || param->default_value == NULL || param->param_type == NULL) {
            continue;
        }
        if (CheckExpressions(param->default_value, ctx) != 0) {
            had_error = 1;
        }
        {
            NType *def_type = InferExpressionTypeSilent(param->default_value, ctx);
            if (def_type != NULL && !CanAssign(param->param_type, def_type)) {
                if (ctx->errors != NULL) {
                    SemanticError err = CreateTypeMismatchError(TypeToString(param->param_type),
                                                                TypeToString(def_type),
                                                                "as default value",
                                                                param->default_value->line,
                                                                param->default_value->column);
                    AddError(ctx->errors, &err);
                }
                had_error = 1;
            }
        }
    }
    return had_error;
}

static int IsStringType(const NType *type) {
    return type != NULL &&
        type->kind == TYPE_KIND_BASE &&
        type->base_type == TYPE_STRING;
}

static int IsCharType(const NType *type) {
    return type != NULL &&
        type->kind == TYPE_KIND_BASE &&
        type->base_type == TYPE_CHAR;
}

static int IsIntType(const NType *type) {
    return type != NULL &&
        ((type->kind == TYPE_KIND_BASE && type->base_type == TYPE_INT) ||
         type->kind == TYPE_KIND_ENUM);
}

static int ValidateFormatArguments(const char *func_name, NExpr **args, int arg_count,
                                   SemanticContext *ctx, int line, int column) {
    const char *fmt;
    char specs[128];
    int spec_count = 0;
    int i = 0;
    int had_error = 0;

    if (arg_count <= 0 || args == NULL || args[0] == NULL) {
        return 0;
    }
    if (args[0]->type != EXPR_STRING || args[0]->value.string_value == NULL) {
        return 0;
    }
    fmt = args[0]->value.string_value;

    while (fmt[i] != '\0') {
        if (fmt[i] == '%') {
            if (fmt[i + 1] == '%') {
                i += 2;
                continue;
            }
            if (fmt[i + 1] == '\0') {
                if (ctx && ctx->errors) {
                    SemanticError err = CreateCustomError(SEMANTIC_ERROR_OTHER,
                                                          "Incomplete format specifier",
                                                          line,
                                                          column);
                    AddError(ctx->errors, &err);
                }
                return 1;
            }
            i++;
            while (fmt[i] == '-' || fmt[i] == '+' || fmt[i] == ' ' ||
                   fmt[i] == '0' || fmt[i] == '#') {
                i++;
            }
            while (fmt[i] >= '0' && fmt[i] <= '9') {
                i++;
            }
            if (fmt[i] == '.') {
                i++;
                while (fmt[i] >= '0' && fmt[i] <= '9') {
                    i++;
                }
            }
            if (fmt[i] == '\0') {
                if (ctx && ctx->errors) {
                    SemanticError err = CreateCustomError(SEMANTIC_ERROR_OTHER,
                                                          "Incomplete format specifier",
                                                          line,
                                                          column);
                    AddError(ctx->errors, &err);
                }
                return 1;
            }
            if (spec_count < (int)(sizeof(specs) / sizeof(specs[0]))) {
                specs[spec_count++] = fmt[i];
            }
            i += 1;
            continue;
        }
        i += 1;
    }

    if (spec_count != arg_count - 1) {
        if (ctx && ctx->errors) {
            SemanticError err = CreateCustomError(SEMANTIC_ERROR_OTHER,
                                                  "Format specifier count does not match arguments",
                                                  line,
                                                  column);
            AddError(ctx->errors, &err);
        }
        had_error = 1;
    }

    for (int idx = 0; idx < spec_count && (idx + 1) < arg_count; idx++) {
        char spec = specs[idx];
        NExpr *arg = args[idx + 1];
        NType *arg_type = InferExpressionTypeSilent(arg, ctx);
        const char *expected = NULL;
        int ok = 1;

        switch (spec) {
            case 'd':
                expected = "int";
                ok = IsIntType(arg_type);
                break;
            case 'f':
                expected = "float";
                ok = IsFloatingPointType(arg_type);
                break;
            case 's':
                expected = "string";
                ok = IsStringType(arg_type);
                break;
            case 'c':
                expected = "char";
                ok = IsCharType(arg_type);
                break;
            default:
                if (ctx && ctx->errors) {
                    SemanticError err = CreateCustomError(SEMANTIC_ERROR_OTHER,
                                                          "Unsupported format specifier",
                                                          line,
                                                          column);
                    AddError(ctx->errors, &err);
                }
                had_error = 1;
                continue;
        }

        if (!ok && ctx && ctx->errors && arg_type != NULL) {
            SemanticError err = CreateTypeMismatchError(expected,
                                                        TypeToString(arg_type),
                                                        "in format argument",
                                                        arg->line,
                                                        arg->column);
            AddError(ctx->errors, &err);
            had_error = 1;
        }
    }

    (void)func_name;
    return had_error;
}

static FunctionInfo *FindFunctionInfoBySignature(SemanticContext *ctx, const NFuncDef *def) {
    if (ctx == NULL || def == NULL || def->func_name == NULL) {
        return NULL;
    }
    if (ctx->functions == NULL) {
        return NULL;
    }
    for (int i = 0; i < ctx->function_count; i++) {
        FunctionInfo *func = ctx->functions[i];
        if (func == NULL || func->name == NULL) {
            continue;
        }
        if (strcmp(func->name, def->func_name) != 0) {
            continue;
        }
        if (!TypesEqual(func->return_type, def->return_type)) {
            continue;
        }
        {
            int params_a = (func->params != NULL) ? func->params->count : 0;
            int params_b = (def->params != NULL) ? def->params->count : 0;
            if (params_a != params_b) {
                continue;
            }
            for (int j = 0; j < params_a; j++) {
                NParam *pa = func->params->params[j];
                NParam *pb = def->params->params[j];
                if (pa == NULL || pb == NULL) {
                    continue;
                }
                if (pa->is_ref != pb->is_ref) {
                    params_a = -1;
                    break;
                }
                if (!TypesEqual(pa->param_type, pb->param_type)) {
                    params_a = -1;
                    break;
                }
            }
            if (params_a < 0) {
                continue;
            }
        }
        return func;
    }
    return NULL;
}

static int IsDuplicateCaseExpr(const NExpr *left, const NExpr *right) {
    if (left == NULL || right == NULL) {
        return 0;
    }
    if (left->type != right->type) {
        return 0;
    }
    switch (left->type) {
        case EXPR_INT:
            return left->value.int_value == right->value.int_value;
        case EXPR_CHAR:
            return left->value.char_value == right->value.char_value;
        case EXPR_BOOL:
            return left->value.int_value == right->value.int_value;
        case EXPR_STRING:
            if (left->value.string_value == NULL || right->value.string_value == NULL) {
                return 0;
            }
            return strcmp(left->value.string_value, right->value.string_value) == 0;
        case EXPR_IDENT:
        case EXPR_MEMBER_ACCESS:
            if (left->enum_value_is_set && right->enum_value_is_set) {
                return left->enum_value == right->enum_value;
            }
            return 0;
        default:
            return 0;
    }
}

static int SetResolvedCallInfo(NExpr *expr, const char *owner_internal, const char *descriptor,
                               const NParamList *params) {
    if (expr == NULL) {
        return 0;
    }
    if (owner_internal != NULL) {
        expr->resolved_owner_internal = DuplicateString(owner_internal);
    }
    if (descriptor != NULL) {
        expr->resolved_member_descriptor = DuplicateString(descriptor);
    }
    if (params != NULL && params->count > 0) {
        expr->resolved_arg_is_ref = (unsigned char *)malloc((size_t)params->count);
        if (expr->resolved_arg_is_ref == NULL) {
            return 0;
        }
        expr->resolved_arg_count = params->count;
        for (int i = 0; i < params->count; i++) {
            NParam *param = params->params[i];
            expr->resolved_arg_is_ref[i] = (param != NULL && param->is_ref) ? 1 : 0;
        }
    } else {
        expr->resolved_arg_count = 0;
    }
    return 1;
}

static int CanMatchMethod(const MethodInfo *method, NExpr **args, int arg_count,
                          SemanticContext *ctx, int *score_out) {
    int expected;
    int count;
    int score = 0;

    if (method == NULL) {
        return 0;
    }
    expected = (method->params != NULL) ? method->params->count : 0;
    if (arg_count > expected) {
        return 0;
    }
    if (arg_count < expected && method->params != NULL) {
        for (int i = arg_count; i < expected; i++) {
            NParam *param = method->params->params[i];
            if (param == NULL || param->default_value == NULL) {
                return 0;
            }
        }
    }
    if (method->params == NULL) {
        if (score_out != NULL) {
            *score_out = score;
        }
        return 1;
    }
    count = expected < arg_count ? expected : arg_count;
    for (int i = 0; i < count; i++) {
        NParam *param = method->params->params[i];
        NExpr *arg = (args != NULL) ? args[i] : NULL;
        NType *arg_type;
        if (param == NULL || arg == NULL || param->param_type == NULL) {
            return 0;
        }
        if (param->is_ref && !IsValidLValueExpr(arg)) {
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

static MethodInfo *LookupMethodOverloadInHierarchy(SemanticContext *ctx, ClassInfo *class_info,
                                                    const char *method_name, NExpr **args, int arg_count,
                                                    const NExpr *obj_expr, ClassInfo **owner_out,
                                                    int *is_ambiguous) {
    MethodInfo *best = NULL;
    ClassInfo *best_owner = NULL;
    int best_score = -1;
    int ambiguous = 0;
    ClassInfo *current = class_info;
    MethodInfo **seen = NULL;
    int seen_count = 0;
    int seen_cap = 0;

    if (is_ambiguous != NULL) {
        *is_ambiguous = 0;
    }
    if (owner_out != NULL) {
        *owner_out = NULL;
    }
    if (ctx == NULL || class_info == NULL || method_name == NULL) {
        return NULL;
    }
    while (current != NULL) {
        for (int i = 0; i < current->method_count; i++) {
            MethodInfo *method = current->methods[i];
            int score = 0;
            if (method == NULL || method->name == NULL) {
                continue;
            }
            if (strcmp(method->name, method_name) != 0) {
                continue;
            }
            {
                int already_seen = 0;
                for (int s = 0; s < seen_count; s++) {
                    if (MethodsHaveSameSignature(method, seen[s])) {
                        already_seen = 1;
                        break;
                    }
                }
                if (already_seen) {
                    continue;
                }
                if (seen_count == seen_cap) {
                    int new_cap = (seen_cap == 0) ? 4 : seen_cap * 2;
                    MethodInfo **grown = (MethodInfo**)realloc(seen,
                                                               sizeof(MethodInfo*) * (size_t)new_cap);
                    if (grown == NULL) {
                        free(seen);
                        return NULL;
                    }
                    seen = grown;
                    seen_cap = new_cap;
                }
                seen[seen_count++] = method;
            }
            {
                int inside_class = IsInsideExactClass(ctx, obj_expr, current->name);
                int inside_hierarchy = IsInsideHierarchyAccess(ctx, obj_expr, current->name);
                if (!IsMethodAccessible(method, inside_class, inside_hierarchy)) {
                    continue;
                }
            }
            if (!CanMatchMethod(method, args, arg_count, ctx, &score)) {
                continue;
            }
            if (score > best_score) {
                best = method;
                best_owner = current;
                best_score = score;
                ambiguous = 0;
            } else if (score == best_score) {
                ambiguous = 1;
            }
        }
        if (current->base_class == NULL) {
            break;
        }
        current = LookupClass(ctx, current->base_class);
    }
    free(seen);
    if (is_ambiguous != NULL) {
        *is_ambiguous = ambiguous;
    }
    if (owner_out != NULL) {
        *owner_out = best_owner;
    }
    if (ambiguous) {
        return NULL;
    }
    return best;
}

static int AttributeJvmRefKeysInStmtList(NStmt *stmts, SemanticContext *ctx);
static int AttributeJvmRefKeysInStmt(NStmt *stmt, SemanticContext *ctx);
static int AttributeJvmRefKeysInExpr(NExpr *expr, SemanticContext *ctx);
static int FillRefKeyForMemberAccess(NExpr *expr, SemanticContext *ctx);
static int FillRefKeyForMethodCall(NExpr *expr, SemanticContext *ctx);
static int FillRefKeyForSuperMethod(NExpr *expr, SemanticContext *ctx);
static int FillRefKeyForFuncCall(NExpr *expr, SemanticContext *ctx);
static int FillRefKeyForSuperField(NExpr *expr, SemanticContext *ctx);
static int SetResolvedCallInfo(NExpr *expr, const char *owner_internal, const char *descriptor,
                               const NParamList *params);
static MethodInfo *LookupMethodOverloadInHierarchy(SemanticContext *ctx, ClassInfo *class_info,
                                                    const char *method_name, NExpr **args, int arg_count,
                                                    const NExpr *obj_expr, ClassInfo **owner_out, int *is_ambiguous);
static int CanMatchMethod(const MethodInfo *method, NExpr **args, int arg_count,
                          SemanticContext *ctx, int *score_out);

/* ============================================================================
   ПЕРВЫЙ ПРОХОД: СБОР ВСЕХ ДЕКЛАРАЦИЙ
   ============================================================================ */

int ProcessSourceItems(NProgram *root, SemanticContext *ctx) {
    int had_error = 0;
    NSourceItem *item;

    if (root == NULL) {
        return 0;
    }
    if (ctx == NULL) {
        return 1;
    }

    item = root->first_item;
    while (item != NULL) {
        if (item->type == SOURCE_ITEM_CLASS) {
            NClassDef *class_def = item->value.class_def;
            if (ProcessClassDefinition(class_def, ctx) != 0) {
                had_error = 1;
            } else if (class_def && class_def->class_name) {
                ClassInfo *class_info = LookupClass(ctx, class_def->class_name);
                if (class_info != NULL && !class_info->members_processed) {
                    if (ProcessClassMembers(class_def->members.first, class_info, ctx) != 0) {
                        had_error = 1;
                    }
                    class_info->members_processed = 1;
                }
            }
        }
        item = item->next;
    }

    item = root->first_item;
    while (item != NULL) {
        switch (item->type) {
            case SOURCE_ITEM_FUNC:
                if (ProcessFunctionDefinition(item->value.func, ctx) != 0) {
                    had_error = 1;
                }
                break;
            case SOURCE_ITEM_CLASS:
                break;
            case SOURCE_ITEM_DECL:
                if (ProcessGlobalVariables(item->value.decl.item_type,
                                           item->value.decl.init_decls,
                                           ctx) != 0) {
                    had_error = 1;
                }
                break;
            case SOURCE_ITEM_ENUM:
                if (ProcessEnumDefinition(item->value.enum_def, ctx) != 0) {
                    had_error = 1;
                }
                break;
        }
        item = item->next;
    }

    return had_error;
}


int ProcessFunctionDefinition(NFuncDef *func_def, SemanticContext *ctx) {
    FunctionInfo *info;
    int had_error = 0;

    if (func_def == NULL) {
        return 0;
    }
    if (ctx == NULL) {
        return 1;
    }

    if (!ValidateArraySizeType(func_def->return_type, ctx, 0, 0)) {
        return 1;
    }
    if (func_def->params != NULL) {
        for (int i = 0; i < func_def->params->count; i++) {
            NParam *param = func_def->params->params[i];
            if (param != NULL && !ValidateArraySizeType(param->param_type, ctx, 0, 0)) {
                return 1;
            }
        }
    }
    if (func_def->params != NULL) {
        for (int i = 0; i < func_def->params->count; i++) {
            NParam *param = func_def->params->params[i];
            if (param != NULL && !EnsureTypeDefined(param->param_type, ctx, 0, 0)) {
                had_error = 1;
            }
        }
    }
    if (CheckParamDefaultValues(func_def->params, ctx) != 0) {
        had_error = 1;
    }

    if (!EnsureTypeDefined(func_def->return_type, ctx, 0, 0)) {
        return 1;
    }

    info = (FunctionInfo*)malloc(sizeof(FunctionInfo));
    if (info == NULL) {
        return ReportOutOfMemory(ctx);
    }
    memset(info, 0, sizeof(FunctionInfo));
    info->name = func_def->func_name;
    info->return_type = func_def->return_type;
    info->params = func_def->params;
    info->is_prototype = (func_def->body == NULL) ? 1 : 0;
    info->line = 0;
    info->column = 0;

    if (func_def->body == NULL) {
        if (ctx->errors != NULL) {
            SemanticError err = CreateCustomError(SEMANTIC_ERROR_OTHER,
                                                  "Function prototype without body is not supported for JVM target",
                                                  0,
                                                  0);
            AddError(ctx->errors, &err);
        }
        had_error = 1;
    }

    if (AddFunctionToContext(ctx, info) != 0) {
        return 1;
    }
    return had_error;
}


int ProcessClassDefinition(NClassDef *class_def, SemanticContext *ctx) {
    ClassInfo *info;

    if (class_def == NULL) {
        return 0;
    }
    if (ctx == NULL) {
        return 1;
    }

    info = (ClassInfo*)malloc(sizeof(ClassInfo));
    if (info == NULL) {
        return ReportOutOfMemory(ctx);
    }
    memset(info, 0, sizeof(ClassInfo));
    info->name = class_def->class_name;
    info->base_class = class_def->base_class_name;
    if (info->base_class == NULL && info->name != NULL &&
        strcmp(info->name, "Object") != 0) {
        info->base_class = "Object";
    }
    info->fields = NULL;
    info->field_count = 0;
    info->methods = NULL;
    info->method_count = 0;
    info->constructors = NULL;
    info->constructor_count = 0;
    info->destructor = NULL;
    info->members_processed = 0;
    info->line = 0;
    info->column = 0;

    return AddClassToContext(ctx, info);
}


int ProcessClassMembers(NClassMember *members, ClassInfo *class_info, SemanticContext *ctx) {
    int had_error = 0;
    NClassMember *member;

    if (members == NULL) {
        return 0;
    }
    if (class_info == NULL || ctx == NULL) {
        return 1;
    }

    member = members;
    while (member != NULL) {
        switch (member->type) {
            case CLASS_MEMBER_FIELD:
                if (ProcessFieldDeclaration(member->value.field.init_decls,
                                            member->value.field.field_type,
                                            member->access,
                                            class_info,
                                            ctx) != 0) {
                    had_error = 1;
                }
                break;
            case CLASS_MEMBER_METHOD:
                if (ProcessMethodDefinition(member->value.method,
                                            member->access,
                                            class_info,
                                            ctx) != 0) {
                    had_error = 1;
                }
                break;
            case CLASS_MEMBER_CTOR:
                if (AddConstructorToClass(class_info, member->value.ctor, ctx) != 0) {
                    had_error = 1;
                }
                break;
            case CLASS_MEMBER_DTOR:
                if (class_info->destructor != NULL) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateCustomError(SEMANTIC_ERROR_OTHER,
                                                              "Duplicate destructor in class",
                                                              0,
                                                              0);
                        AddError(ctx->errors, &err);
                    }
                    had_error = 1;
                } else {
                    class_info->destructor = member->value.dtor;
                }
                break;
            case CLASS_MEMBER_ENUM:
                break;
        }
        member = member->next;
    }

    return had_error;
}


int ProcessFieldDeclaration(NInitDeclList *init_decls, NType *field_type, 
                           AccessSpec access, ClassInfo *class_info, SemanticContext *ctx) {
    int had_error = 0;

    if (field_type == NULL) {
        return 1;
    }
    if (class_info == NULL || ctx == NULL) {
        return 1;
    }

    if (!ValidateArraySizeType(field_type, ctx, 0, 0)) {
        return 1;
    }
    if (!EnsureTypeDefined(field_type, ctx, 0, 0)) {
        return 1;
    }

    if (init_decls == NULL) {
        return 0;
    }

    for (int i = 0; i < init_decls->count; i++) {
        NInitDecl *decl = init_decls->decls[i];
        FieldInfo *field;
        FieldInfo **grown;
        if (decl == NULL) {
            continue;
        }
        field = (FieldInfo*)malloc(sizeof(FieldInfo));
        if (field == NULL) {
            return ReportOutOfMemory(ctx);
        }
        memset(field, 0, sizeof(FieldInfo));
        field->name = decl->name;
        field->type = field_type;
        field->access = access;
        field->line = 0;
        field->column = 0;

        if (LookupClassField(class_info, field->name) != NULL) {
            if (ctx->errors != NULL) {
                SemanticError err = CreateDuplicateSymbolError(field->name,
                                                              field->line,
                                                              field->column);
                AddError(ctx->errors, &err);
            }
            free(field);
            had_error = 1;
            continue;
        }

        grown = (FieldInfo**)realloc(class_info->fields,
                                     sizeof(FieldInfo*) * (size_t)(class_info->field_count + 1));
        if (grown == NULL) {
            free(field);
            return ReportOutOfMemory(ctx);
        }
        class_info->fields = grown;
        class_info->fields[class_info->field_count] = field;
        class_info->field_count += 1;
    }

    return had_error;
}


int ProcessMethodDefinition(NMethodDef *method_def, AccessSpec access, 
                           ClassInfo *class_info, SemanticContext *ctx) {
    MethodInfo *info;
    MethodInfo **grown;
    ClassInfo *base_info = NULL;
    MethodInfo *base_method = NULL;
    int had_error = 0;

    if (method_def == NULL) {
        return 0;
    }
    if (class_info == NULL || ctx == NULL) {
        return 1;
    }

    if (!ValidateArraySizeType(method_def->return_type, ctx, 0, 0)) {
        return 1;
    }
    if (method_def->params != NULL) {
        for (int i = 0; i < method_def->params->count; i++) {
            NParam *param = method_def->params->params[i];
            if (param != NULL && !ValidateArraySizeType(param->param_type, ctx, 0, 0)) {
                return 1;
            }
        }
    }
    if (method_def->params != NULL) {
        for (int i = 0; i < method_def->params->count; i++) {
            NParam *param = method_def->params->params[i];
            if (param != NULL && !EnsureTypeDefined(param->param_type, ctx, 0, 0)) {
                had_error = 1;
            }
        }
    }
    if (CheckParamDefaultValues(method_def->params, ctx) != 0) {
        had_error = 1;
    }

    if (!EnsureTypeDefined(method_def->return_type, ctx, 0, 0)) {
        return 1;
    }

    if (method_def->is_override) {
        if (class_info->base_class == NULL) {
            if (ctx->errors != NULL) {
                SemanticError err = CreateInvalidOverrideError(method_def->method_name,
                                                              "no base class",
                                                              0,
                                                              0);
                AddError(ctx->errors, &err);
            }
            return 1;
        }
        base_info = LookupClass(ctx, class_info->base_class);
        if (base_info == NULL) {
            if (ctx->errors != NULL) {
                SemanticError err = CreateInvalidOverrideError(method_def->method_name,
                                                              "base class not found",
                                                              0,
                                                              0);
                AddError(ctx->errors, &err);
            }
            return 1;
        }
        base_method = FindMethodInHierarchyBySignature(ctx,
                                                       base_info,
                                                       method_def->method_name,
                                                       method_def->params);
        if (base_method == NULL) {
            if (ctx->errors != NULL) {
                SemanticError err = CreateInvalidOverrideError(method_def->method_name,
                                                              "method not found in base class",
                                                              0,
                                                              0);
                AddError(ctx->errors, &err);
            }
            return 1;
        }
        if (!TypesEqual(base_method->return_type, method_def->return_type)) {
            if (ctx->errors != NULL) {
                SemanticError err = CreateInvalidOverrideError(method_def->method_name,
                                                              "return type mismatch",
                                                              0,
                                                              0);
                AddError(ctx->errors, &err);
            }
            return 1;
        }
    }

    if (FindMethodInClassBySignature(class_info, method_def->method_name, method_def->params) != NULL) {
        if (ctx->errors != NULL) {
            SemanticError err = CreateDuplicateSymbolError(method_def->method_name,
                                                          0,
                                                          0);
            AddError(ctx->errors, &err);
        }
        return 1;
    }

    info = (MethodInfo*)malloc(sizeof(MethodInfo));
    if (info == NULL) {
        return ReportOutOfMemory(ctx);
    }
    memset(info, 0, sizeof(MethodInfo));
    info->name = method_def->method_name;
    info->return_type = method_def->return_type;
    info->params = method_def->params;
    info->access = access;
    info->is_override = method_def->is_override;
    info->line = 0;
    info->column = 0;

    grown = (MethodInfo**)realloc(class_info->methods,
                                  sizeof(MethodInfo*) * (size_t)(class_info->method_count + 1));
    if (grown == NULL) {
        free(info);
        return ReportOutOfMemory(ctx);
    }
    class_info->methods = grown;
    class_info->methods[class_info->method_count] = info;
    class_info->method_count += 1;
    return had_error;
}


int ProcessEnumDefinition(NEnumDef *enum_def, SemanticContext *ctx) {
    EnumInfo *info;
    int next_value = 0;

    if (enum_def == NULL) {
        return 0;
    }
    if (ctx == NULL) {
        return 1;
    }

    info = (EnumInfo*)malloc(sizeof(EnumInfo));
    if (info == NULL) {
        return ReportOutOfMemory(ctx);
    }
    memset(info, 0, sizeof(EnumInfo));
    info->name = enum_def->enum_name;
    info->items = NULL;
    info->item_count = 0;
    info->line = 0;
    info->column = 0;

    if (enum_def->items.items != NULL) {
        for (int i = 0; i < enum_def->items.count; i++) {
            NEnumItem *src = enum_def->items.items[i];
            EnumItemInfo *item;
            EnumItemInfo **grown;
            if (src == NULL) {
                continue;
            }
            for (int j = 0; j < info->item_count; j++) {
                EnumItemInfo *prev = info->items[j];
                if (prev != NULL && prev->name != NULL && src->name != NULL &&
                    strcmp(prev->name, src->name) == 0) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateEnumDuplicateItemError(src->name, 0, 0);
                        AddError(ctx->errors, &err);
                    }
                    return 1;
                }
            }
            item = (EnumItemInfo*)malloc(sizeof(EnumItemInfo));
            if (item == NULL) {
                return ReportOutOfMemory(ctx);
            }
            memset(item, 0, sizeof(EnumItemInfo));
            item->name = src->name;
            item->has_explicit_value = src->has_value;
            item->owner = info;
            if (src->has_value) {
                item->value = src->value;
                next_value = src->value + 1;
            } else {
                item->value = next_value;
                next_value += 1;
            }

            grown = (EnumItemInfo**)realloc(info->items,
                                            sizeof(EnumItemInfo*) * (size_t)(info->item_count + 1));
            if (grown == NULL) {
                free(item);
                return ReportOutOfMemory(ctx);
            }
            info->items = grown;
            info->items[info->item_count] = item;
            info->item_count += 1;
        }
    }

    return AddEnumToContext(ctx, info);
}


int ProcessGlobalVariables(NType *type, NInitDeclList *init_decls, SemanticContext *ctx) {
    int had_error = 0;

    if (type == NULL) {
        return 1;
    }
    if (ctx == NULL) {
        return 1;
    }

    if (!ValidateArraySizeType(type, ctx, 0, 0)) {
        return 1;
    }
    if (!EnsureTypeDefined(type, ctx, 0, 0)) {
        return 1;
    }

    if (init_decls == NULL) {
        return 0;
    }

    for (int i = 0; i < init_decls->count; i++) {
        NInitDecl *decl = init_decls->decls[i];
        VariableInfo *var;
        if (decl == NULL) {
            continue;
        }
        var = (VariableInfo*)malloc(sizeof(VariableInfo));
        if (var == NULL) {
            return ReportOutOfMemory(ctx);
        }
        memset(var, 0, sizeof(VariableInfo));
        var->name = decl->name;
        var->type = type;
        var->is_initialized = (decl->initializer != NULL);
        var->is_param = 0;
        var->is_ref = 0;
        var->line = 0;
        var->column = 0;

        if (AddLocalVariable(ctx, var) != 0) {
            had_error = 1;
        }
    }

    return had_error;
}


/* ============================================================================
   ВТОРОЙ ПРОХОД: ПРОВЕРКА СЕМАНТИКИ
   ============================================================================ */

int CheckAllSemantics(NProgram *root, SemanticContext *ctx) {

    int had_error;

    if (root == NULL) {
        return 0;
    }
    if (ctx == NULL) {
        return 1;
    }

    had_error = CheckSourceItems(root->first_item, ctx);
    if (had_error != 0) {
        return 1;
    }

    return HasErrors(ctx->errors) ? 1 : 0;
}

int CheckSourceItems(NSourceItem *items, SemanticContext *ctx) {

    int had_error = 0;
    NSourceItem *item = items;

    if (items == NULL) {
        return 0;
    }
    if (ctx == NULL) {
        return 1;
    }

    while (item != NULL) {
        switch (item->type) {
            case SOURCE_ITEM_FUNC: {
                NFuncDef *func = item->value.func;
                FunctionInfo *func_info = NULL;
                if (func != NULL && func->func_name != NULL) {
                    func_info = FindFunctionInfoBySignature(ctx, func);
                }
                if (CheckFunctionBody(func ? func->body : NULL, func_info, ctx) != 0) {
                    had_error = 1;
                }
                break;
            }
            case SOURCE_ITEM_CLASS: {
                NClassDef *class_def = item->value.class_def;
                NClassMember *member = class_def ? class_def->members.first : NULL;
                ClassInfo *saved_class = ctx->current_class;
                if (class_def != NULL && class_def->class_name != NULL) {
                    ctx->current_class = LookupClass(ctx, class_def->class_name);
                }
                if (ctx->current_class != NULL && ctx->current_class->base_class != NULL) {
                    if (LookupClass(ctx, ctx->current_class->base_class) == NULL) {
                        if (ctx->errors != NULL) {
                            SemanticError err = CreateInvalidBaseClassError(ctx->current_class->base_class,
                                                                           0,
                                                                           0);
                            AddError(ctx->errors, &err);
                        }
                        had_error = 1;
                    }
                }
                if (ctx->current_class != NULL) {
                    if (!ctx->current_class->members_processed) {
                        if (ProcessClassMembers(member, ctx->current_class, ctx) != 0) {
                            had_error = 1;
                        }
                        ctx->current_class->members_processed = 1;
                    }
                }
                while (member != NULL) {
                    if (member->type == CLASS_MEMBER_FIELD) {
                        NInitDeclList *init_decls = member->value.field.init_decls;
                        if (init_decls != NULL) {
                            for (int i = 0; i < init_decls->count; i++) {
                                NInitDecl *decl = init_decls->decls[i];
                                if (decl && decl->initializer) {
                                    if (decl->initializer->is_array) {
                                        int line = 0;
                                        int column = 0;
                                        GetInitializerLocation(decl->initializer, 0, 0, &line, &column);
                                        if (CheckStaticArrayInitSize(member->value.field.field_type,
                                                                     decl->initializer,
                                                                     ctx,
                                                                     line,
                                                                     column) != 0) {
                                            had_error = 1;
                                        }
                                        for (int j = 0; j < decl->initializer->array_init.count; j++) {
                                            if (CheckExpressions(decl->initializer->array_init.elements[j], ctx) != 0) {
                                                had_error = 1;
                                            }
                                        }
                                    } else {
                                        if (CheckExpressions(decl->initializer->expr, ctx) != 0) {
                                            had_error = 1;
                                        }
                                    }
                                }
                            }
                        }
                    } else if (member->type == CLASS_MEMBER_METHOD) {
                        NMethodDef *method = member->value.method;
                        FunctionInfo tmp_info;
                        if (method != NULL) {
                            memset(&tmp_info, 0, sizeof(FunctionInfo));
                            tmp_info.name = method->method_name;
                            tmp_info.return_type = method->return_type;
                            tmp_info.params = method->params;
                            if (CheckFunctionBody(method->body, &tmp_info, ctx) != 0) {
                                had_error = 1;
                            }
                        }
                    } else if (member->type == CLASS_MEMBER_CTOR) {
                        NCtorDef *ctor = member->value.ctor;
                        FunctionInfo tmp_info;
                        if (ctor != NULL) {
                            memset(&tmp_info, 0, sizeof(FunctionInfo));
                            tmp_info.name = "ctor";
                            tmp_info.return_type = NULL;
                            tmp_info.params = ctor->params;
                            if (CheckFunctionBody(ctor->body, &tmp_info, ctx) != 0) {
                                had_error = 1;
                            }
                        }
                    } else if (member->type == CLASS_MEMBER_DTOR) {
                        NDtorDef *dtor = member->value.dtor;
                        FunctionInfo tmp_info;
                        if (dtor != NULL) {
                            memset(&tmp_info, 0, sizeof(FunctionInfo));
                            tmp_info.name = "dtor";
                            tmp_info.return_type = NULL;
                            tmp_info.params = NULL;
                            if (CheckFunctionBody(dtor->body, &tmp_info, ctx) != 0) {
                                had_error = 1;
                            }
                        }
                    }
                    member = member->next;
                }
                ctx->current_class = saved_class;
                break;
            }
            case SOURCE_ITEM_DECL: {
                NInitDeclList *init_decls = item->value.decl.init_decls;
                if (init_decls != NULL) {
                    for (int i = 0; i < init_decls->count; i++) {
                        NInitDecl *decl = init_decls->decls[i];
                        if (decl && decl->initializer) {
                            if (decl->initializer->is_array) {
                                int line = 0;
                                int column = 0;
                                GetInitializerLocation(decl->initializer, 0, 0, &line, &column);
                                if (CheckStaticArrayInitSize(item->value.decl.item_type,
                                                             decl->initializer,
                                                             ctx,
                                                             line,
                                                             column) != 0) {
                                    had_error = 1;
                                }
                                for (int j = 0; j < decl->initializer->array_init.count; j++) {
                                    if (CheckExpressions(decl->initializer->array_init.elements[j], ctx) != 0) {
                                        had_error = 1;
                                    }
                                }
                            } else {
                                if (CheckExpressions(decl->initializer->expr, ctx) != 0) {
                                    had_error = 1;
                                }
                            }
                        }
                    }
                }
                break;
            }
            case SOURCE_ITEM_ENUM:
                break;
        }
        item = item->next;
    }

    return had_error;
}

int CheckFunctionBody(NStmt *func_body, FunctionInfo *func_info, SemanticContext *ctx) {

    int had_error = 0;
    int return_seen = 0;
    const char *scope_name = (func_info && func_info->name) ? func_info->name : "function";

    if (func_body == NULL) {
        return 0;
    }
    if (ctx == NULL) {
        return 1;
    }

    if (PushScope(ctx, scope_name) != 0) {
        return 1;
    }

    if (func_info != NULL && func_info->params != NULL) {
        for (int i = 0; i < func_info->params->count; i++) {
            NParam *param = func_info->params->params[i];
            VariableInfo *var;
            if (param == NULL || param->param_name == NULL) {
                continue;
            }
            var = (VariableInfo*)malloc(sizeof(VariableInfo));
            if (var == NULL) {
                had_error = ReportOutOfMemory(ctx);
                continue;
            }
            memset(var, 0, sizeof(VariableInfo));
            var->name = param->param_name;
            var->type = param->param_type;
            var->is_initialized = 1;
            var->is_param = 1;
            var->is_ref = param->is_ref;
            var->line = 0;
            var->column = 0;

            if (AddLocalVariable(ctx, var) != 0) {
                had_error = 1;
            }
        }
    }

    if (CheckStatements(func_body, ctx, func_info ? func_info->return_type : NULL, &return_seen) != 0) {
        had_error = 1;
    }

    if (func_info != NULL && func_info->return_type != NULL && !return_seen) {
        if (ctx->errors != NULL) {
            SemanticError err = CreateCustomError(SEMANTIC_ERROR_OTHER,
                                                 "Missing return in non-void function",
                                                 func_body->line,
                                                 func_body->column);
            AddError(ctx->errors, &err);
        }
        had_error = 1;
    }

    if (PopScope(ctx) != 0) {
        return 1;
    }

    return had_error;
}

int CheckStatements(NStmt *stmts, SemanticContext *ctx, NType *expected_return_type, int *return_seen) {

    int had_error = 0;
    NStmt *stmt = stmts;

    if (stmts == NULL) {
        return 0;
    }
    if (ctx == NULL) {
        return 1;
    }

    while (stmt != NULL) {
        if (CheckStatement(stmt, ctx, expected_return_type, return_seen) != 0) {
            had_error = 1;
        }
        stmt = stmt->next;
    }

    return had_error;
}

int CheckStatement(NStmt *stmt, SemanticContext *ctx, NType *expected_return_type, int *return_seen) {

    int had_error = 0;

    if (stmt == NULL) {
        return 0;
    }
    if (ctx == NULL) {
        return 1;
    }

    switch (stmt->type) {
        case STMT_EXPR:
            if (CheckExpressions(stmt->value.expr, ctx) != 0) {
                had_error = 1;
            }
            break;
        case STMT_DECL: {
            NType *decl_type = stmt->value.decl.decl_type;
            NInitDeclList *init_decls = stmt->value.decl.init_decls;
            if (!ValidateArraySizeType(decl_type, ctx, stmt->line, stmt->column)) {
                had_error = 1;
            }
            if (!EnsureTypeDefined(decl_type, ctx, stmt->line, stmt->column)) {
                had_error = 1;
            }
            if (init_decls != NULL) {
                for (int i = 0; i < init_decls->count; i++) {
                    NInitDecl *decl = init_decls->decls[i];
                    VariableInfo *var;
                    if (decl == NULL || decl->name == NULL) {
                        continue;
                    }
                    var = (VariableInfo*)malloc(sizeof(VariableInfo));
                    if (var == NULL) {
                        had_error = ReportOutOfMemory(ctx);
                        continue;
                    }
                    memset(var, 0, sizeof(VariableInfo));
                    var->name = decl->name;
                    var->type = decl_type;
                    var->is_initialized = (decl->initializer != NULL);
                    var->is_param = 0;
                    var->is_ref = 0;
                    var->line = stmt->line;
                    var->column = stmt->column;

                    if (AddLocalVariable(ctx, var) != 0) {
                        had_error = 1;
                    }

                    if (decl->initializer != NULL) {
                        if (decl->initializer->is_array) {
                            int line = stmt->line;
                            int column = stmt->column;
                            GetInitializerLocation(decl->initializer, stmt->line, stmt->column, &line, &column);
                            if (CheckStaticArrayInitSize(decl_type,
                                                         decl->initializer,
                                                         ctx,
                                                         line,
                                                         column) != 0) {
                                had_error = 1;
                            }
                            for (int j = 0; j < decl->initializer->array_init.count; j++) {
                                NExpr *elem = decl->initializer->array_init.elements[j];
                                if (CheckExpressions(elem, ctx) != 0) {
                                    had_error = 1;
                                }
                                if (decl_type != NULL && elem != NULL) {
                                    NType *expr_type = InferExpressionTypeSilent(elem, ctx);
                                    if (expr_type != NULL) {
                                        NType *elem_type = GetForeachElementType(decl_type);
                                        if (elem_type == NULL) {
                                            if (ctx->errors != NULL) {
                                                SemanticError err = CreateTypeMismatchError("array",
                                                                                            TypeToString(decl_type),
                                                                                            "in array initializer",
                                                                                            elem->line,
                                                                                            elem->column);
                                                AddError(ctx->errors, &err);
                                            }
                                            had_error = 1;
                                        } else if (!CanAssign(elem_type, expr_type)) {
                                            if (ctx->errors != NULL) {
                                                SemanticError err = CreateTypeMismatchError(TypeToString(elem_type),
                                                                                            TypeToString(expr_type),
                                                                                            "in array initializer",
                                                                                            elem->line,
                                                                                            elem->column);
                                                AddError(ctx->errors, &err);
                                            }
                                            had_error = 1;
                                        }
                                    }
                                }
                            }
                        } else {
                            if (CheckExpressions(decl->initializer->expr, ctx) != 0) {
                                had_error = 1;
                            }
                            if (decl_type != NULL) {
                                NType *expr_type = InferExpressionTypeSilent(decl->initializer->expr, ctx);
                                if (expr_type != NULL && !CanAssign(decl_type, expr_type)) {
                                    if (ctx->errors != NULL) {
                                        SemanticError err = CreateTypeMismatchError(TypeToString(decl_type),
                                                                                    TypeToString(expr_type),
                                                                                    "in declaration",
                                                                                    stmt->line,
                                                                                    stmt->column);
                                        AddError(ctx->errors, &err);
                                    }
                                    had_error = 1;
                                }
                            }
                        }
                    }
                }
            }
            break;
        }
        case STMT_IF: {
            NExpr *cond = stmt->value.if_stmt.condition;
            if (CheckExpressions(cond, ctx) != 0) {
                had_error = 1;
            }
            if (cond != NULL) {
                NType *cond_type = InferExpressionTypeSilent(cond, ctx);
                if (cond_type != NULL && !CanBeCondition(cond_type)) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateTypeMismatchError("bool or numeric",
                                                                    TypeToString(cond_type),
                                                                    "as condition",
                                                                    cond->line,
                                                                    cond->column);
                        AddError(ctx->errors, &err);
                    }
                    had_error = 1;
                }
            }
            if (CheckStatement(stmt->value.if_stmt.then_stmt, ctx, expected_return_type, return_seen) != 0) {
                had_error = 1;
            }
            if (CheckStatement(stmt->value.if_stmt.else_stmt, ctx, expected_return_type, return_seen) != 0) {
                had_error = 1;
            }
            break;
        }
        case STMT_WHILE: {
            NExpr *cond = stmt->value.while_stmt.condition;
            if (CheckExpressions(cond, ctx) != 0) {
                had_error = 1;
            }
            if (cond != NULL) {
                NType *cond_type = InferExpressionTypeSilent(cond, ctx);
                if (cond_type != NULL && !CanBeCondition(cond_type)) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateTypeMismatchError("bool or numeric",
                                                                    TypeToString(cond_type),
                                                                    "as condition",
                                                                    cond->line,
                                                                    cond->column);
                        AddError(ctx->errors, &err);
                    }
                    had_error = 1;
                }
            }
            ctx->loop_depth += 1;
            if (CheckStatement(stmt->value.while_stmt.body, ctx, expected_return_type, return_seen) != 0) {
                had_error = 1;
            }
            ctx->loop_depth -= 1;
            break;
        }
        case STMT_DO_WHILE: {
            NExpr *cond = stmt->value.do_while_stmt.condition;
            ctx->loop_depth += 1;
            if (CheckStatement(stmt->value.do_while_stmt.body, ctx, expected_return_type, return_seen) != 0) {
                had_error = 1;
            }
            ctx->loop_depth -= 1;
            if (CheckExpressions(cond, ctx) != 0) {
                had_error = 1;
            }
            if (cond != NULL) {
                NType *cond_type = InferExpressionTypeSilent(cond, ctx);
                if (cond_type != NULL && !CanBeCondition(cond_type)) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateTypeMismatchError("bool or numeric",
                                                                    TypeToString(cond_type),
                                                                    "as condition",
                                                                    cond->line,
                                                                    cond->column);
                        AddError(ctx->errors, &err);
                    }
                    had_error = 1;
                }
            }
            break;
        }
        case STMT_FOR: {
            if (PushScope(ctx, "for") != 0) {
                had_error = 1;
                break;
            }
            NExpr *cond = stmt->value.for_stmt.cond_expr;
            if (CheckExpressions(stmt->value.for_stmt.init_expr, ctx) != 0) {
                had_error = 1;
            }
            if (stmt->value.for_stmt.init_decl_type != NULL) {
                NType *decl_type = stmt->value.for_stmt.init_decl_type;
                NInitDeclList *init_decls = stmt->value.for_stmt.init_decls;
                if (!ValidateArraySizeType(decl_type, ctx, stmt->line, stmt->column)) {
                    had_error = 1;
                }
                if (!EnsureTypeDefined(decl_type, ctx, stmt->line, stmt->column)) {
                    had_error = 1;
                }
                if (init_decls != NULL) {
                    for (int i = 0; i < init_decls->count; i++) {
                        NInitDecl *decl = init_decls->decls[i];
                        VariableInfo *var;
                        if (decl == NULL || decl->name == NULL) {
                            continue;
                        }
                        var = (VariableInfo*)malloc(sizeof(VariableInfo));
                        if (var == NULL) {
                            had_error = ReportOutOfMemory(ctx);
                            continue;
                        }
                        memset(var, 0, sizeof(VariableInfo));
                        var->name = decl->name;
                        var->type = decl_type;
                        var->is_initialized = (decl->initializer != NULL);
                        var->is_param = 0;
                        var->is_ref = 0;
                        var->line = stmt->line;
                        var->column = stmt->column;

                        if (AddLocalVariable(ctx, var) != 0) {
                            had_error = 1;
                        }

                        if (decl->initializer != NULL) {
                            if (decl->initializer->is_array) {
                                int line = stmt->line;
                                int column = stmt->column;
                                GetInitializerLocation(decl->initializer, stmt->line, stmt->column, &line, &column);
                                if (CheckStaticArrayInitSize(decl_type,
                                                             decl->initializer,
                                                             ctx,
                                                             line,
                                                             column) != 0) {
                                    had_error = 1;
                                }
                                for (int j = 0; j < decl->initializer->array_init.count; j++) {
                                    if (CheckExpressions(decl->initializer->array_init.elements[j], ctx) != 0) {
                                        had_error = 1;
                                    }
                                }
                            } else {
                                if (CheckExpressions(decl->initializer->expr, ctx) != 0) {
                                    had_error = 1;
                                }
                                if (decl_type != NULL) {
                                    NType *expr_type = InferExpressionTypeSilent(decl->initializer->expr, ctx);
                                    if (expr_type != NULL && !CanAssign(decl_type, expr_type)) {
                                        if (ctx->errors != NULL) {
                                            SemanticError err = CreateTypeMismatchError(TypeToString(decl_type),
                                                                                        TypeToString(expr_type),
                                                                                        "in for init",
                                                                                        stmt->line,
                                                                                        stmt->column);
                                            AddError(ctx->errors, &err);
                                        }
                                        had_error = 1;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if (CheckExpressions(cond, ctx) != 0) {
                had_error = 1;
            }
            if (cond != NULL) {
                NType *cond_type = InferExpressionTypeSilent(cond, ctx);
                if (cond_type != NULL && !CanBeCondition(cond_type)) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateTypeMismatchError("bool or numeric",
                                                                    TypeToString(cond_type),
                                                                    "as condition",
                                                                    cond->line,
                                                                    cond->column);
                        AddError(ctx->errors, &err);
                    }
                    had_error = 1;
                }
            }
            if (CheckExpressions(stmt->value.for_stmt.iter_expr, ctx) != 0) {
                had_error = 1;
            }
            ctx->loop_depth += 1;
            if (CheckStatement(stmt->value.for_stmt.body, ctx, expected_return_type, return_seen) != 0) {
                had_error = 1;
            }
            ctx->loop_depth -= 1;
            if (PopScope(ctx) != 0) {
                had_error = 1;
            }
            break;
        }
        case STMT_FOREACH: {
            NType *collection_type = NULL;
            NType *element_type = NULL;
            if (PushScope(ctx, "foreach") != 0) {
                had_error = 1;
                break;
            }
            if (CheckExpressions(stmt->value.foreach_stmt.collection, ctx) != 0) {
                had_error = 1;
            }
            if (stmt->value.foreach_stmt.collection != NULL) {
                collection_type = InferExpressionTypeSilent(stmt->value.foreach_stmt.collection, ctx);
                element_type = GetForeachElementType(collection_type);
                if (collection_type != NULL && element_type == NULL) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateCustomError(SEMANTIC_ERROR_OTHER,
                                                              "foreach collection must be an array",
                                                              stmt->value.foreach_stmt.collection->line,
                                                              stmt->value.foreach_stmt.collection->column);
                        AddError(ctx->errors, &err);
                    }
                    had_error = 1;
                }
            }
            if (stmt->value.foreach_stmt.is_typed && stmt->value.foreach_stmt.var_type != NULL &&
                stmt->value.foreach_stmt.var_name != NULL) {
                if (!EnsureTypeDefined(stmt->value.foreach_stmt.var_type, ctx, stmt->line, stmt->column)) {
                    had_error = 1;
                }
                VariableInfo *var = (VariableInfo*)malloc(sizeof(VariableInfo));
                if (var == NULL) {
                    had_error = ReportOutOfMemory(ctx);
                } else {
                    memset(var, 0, sizeof(VariableInfo));
                    var->name = stmt->value.foreach_stmt.var_name;
                    var->type = stmt->value.foreach_stmt.var_type;
                    var->is_initialized = 1;
                    var->is_param = 0;
                    var->is_ref = 0;
                    var->line = stmt->line;
                    var->column = stmt->column;
                    if (AddLocalVariable(ctx, var) != 0) {
                        had_error = 1;
                    }
                }
                if (element_type != NULL &&
                    !CanAssign(stmt->value.foreach_stmt.var_type, element_type)) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateTypeMismatchError(TypeToString(stmt->value.foreach_stmt.var_type),
                                                                    TypeToString(element_type),
                                                                    "in foreach",
                                                                    stmt->line,
                                                                    stmt->column);
                        AddError(ctx->errors, &err);
                    }
                    had_error = 1;
                }
            } else if (!stmt->value.foreach_stmt.is_typed &&
                       stmt->value.foreach_stmt.var_name != NULL) {
                if (element_type == NULL) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateCustomError(SEMANTIC_ERROR_OTHER,
                                                              "Cannot infer foreach variable type",
                                                              stmt->line,
                                                              stmt->column);
                        AddError(ctx->errors, &err);
                    }
                    had_error = 1;
                } else {
                    VariableInfo *var = (VariableInfo*)malloc(sizeof(VariableInfo));
                    if (var == NULL) {
                        had_error = ReportOutOfMemory(ctx);
                    } else {
                        memset(var, 0, sizeof(VariableInfo));
                        var->name = stmt->value.foreach_stmt.var_name;
                        var->type = element_type;
                        var->is_initialized = 1;
                        var->is_param = 0;
                        var->is_ref = 0;
                        var->line = stmt->line;
                        var->column = stmt->column;
                        if (AddLocalVariable(ctx, var) != 0) {
                            had_error = 1;
                        }
                    }
                }
            }
            ctx->loop_depth += 1;
            if (CheckStatement(stmt->value.foreach_stmt.body, ctx, expected_return_type, return_seen) != 0) {
                had_error = 1;
            }
            ctx->loop_depth -= 1;
            if (PopScope(ctx) != 0) {
                had_error = 1;
            }
            break;
        }
        case STMT_SWITCH: {
            NExpr *switch_expr = stmt->value.switch_stmt.expr;
            NType *switch_type = NULL;
            int seen_default = 0;
            if (CheckExpressions(switch_expr, ctx) != 0) {
                had_error = 1;
            }
            if (switch_expr != NULL) {
                switch_type = InferExpressionTypeSilent(switch_expr, ctx);
                if (switch_type != NULL && !IsIntegralType(switch_type)) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateTypeMismatchError("integral type",
                                                                    TypeToString(switch_type),
                                                                    "in switch expression",
                                                                    switch_expr->line,
                                                                    switch_expr->column);
                        AddError(ctx->errors, &err);
                    }
                    had_error = 1;
                }
            }
            ctx->switch_depth += 1;
            for (int i = 0; i < stmt->value.switch_stmt.cases.count; i++) {
                NCaseItem *item = stmt->value.switch_stmt.cases.items[i];
                if (item == NULL) {
                    continue;
                }
                if (item->type == CASE_ITEM_DEFAULT) {
                    if (seen_default) {
                        if (ctx->errors != NULL) {
                            SemanticError err = CreateCustomError(SEMANTIC_ERROR_OTHER,
                                                                  "Duplicate default label in switch",
                                                                  stmt->line,
                                                                  stmt->column);
                            AddError(ctx->errors, &err);
                        }
                        had_error = 1;
                    }
                    seen_default = 1;
                } else if (item->case_expr != NULL) {
                    if (CheckExpressions(item->case_expr, ctx) != 0) {
                        had_error = 1;
                    }
                    if (switch_type != NULL) {
                        NType *case_type = InferExpressionTypeSilent(item->case_expr, ctx);
                        if (case_type != NULL && !TypesEqual(switch_type, case_type)) {
                            if (ctx->errors != NULL) {
                                SemanticError err = CreateTypeMismatchError(TypeToString(switch_type),
                                                                            TypeToString(case_type),
                                                                            "in switch case",
                                                                            item->case_expr->line,
                                                                            item->case_expr->column);
                                AddError(ctx->errors, &err);
                            }
                            had_error = 1;
                        }
                    }
                    for (int j = 0; j < i; j++) {
                        NCaseItem *prev = stmt->value.switch_stmt.cases.items[j];
                        if (prev == NULL || prev->type != CASE_ITEM_CASE || prev->case_expr == NULL) {
                            continue;
                        }
                        if (IsDuplicateCaseExpr(prev->case_expr, item->case_expr)) {
                            if (ctx->errors != NULL) {
                                SemanticError err = CreateCustomError(SEMANTIC_ERROR_OTHER,
                                                                      "Duplicate case label in switch",
                                                                      item->case_expr->line,
                                                                      item->case_expr->column);
                                AddError(ctx->errors, &err);
                            }
                            had_error = 1;
                            break;
                        }
                    }
                }
                if (item->stmts != NULL) {
                    if (CheckStatements(item->stmts->first, ctx, expected_return_type, return_seen) != 0) {
                        had_error = 1;
                    }
                }
            }
            ctx->switch_depth -= 1;
            break;
        }
        case STMT_RETURN:
            if (CheckExpressions(stmt->value.expr, ctx) != 0) {
                had_error = 1;
            }
            if (return_seen != NULL) {
                *return_seen = 1;
            }
            if (expected_return_type == NULL) {
                if (stmt->value.expr != NULL) {
                    NType *ret_type = InferExpressionTypeSilent(stmt->value.expr, ctx);
                    if (ret_type != NULL && ctx->errors != NULL) {
                        SemanticError err = CreateTypeMismatchError("void",
                                                                    TypeToString(ret_type),
                                                                    "in return",
                                                                    stmt->line,
                                                                    stmt->column);
                        AddError(ctx->errors, &err);
                    } else if (ret_type == NULL && ctx->errors != NULL) {
                        SemanticError err = CreateTypeMismatchError("void",
                                                                    "unknown",
                                                                    "in return",
                                                                    stmt->line,
                                                                    stmt->column);
                        AddError(ctx->errors, &err);
                    }
                    had_error = 1;
                }
            } else {
                if (stmt->value.expr == NULL) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateTypeMismatchError(TypeToString(expected_return_type),
                                                                    "void",
                                                                    "in return",
                                                                    stmt->line,
                                                                    stmt->column);
                        AddError(ctx->errors, &err);
                    }
                    had_error = 1;
                } else {
                    NType *ret_type = InferExpressionTypeSilent(stmt->value.expr, ctx);
                    if (ret_type != NULL && !CanAssign(expected_return_type, ret_type)) {
                        if (ctx->errors != NULL) {
                            SemanticError err = CreateTypeMismatchError(TypeToString(expected_return_type),
                                                                        TypeToString(ret_type),
                                                                        "in return",
                                                                        stmt->line,
                                                                        stmt->column);
                            AddError(ctx->errors, &err);
                        }
                        had_error = 1;
                    }
                }
            }
            break;
        case STMT_BREAK:
            if (ctx->loop_depth <= 0 && ctx->switch_depth <= 0) {
                if (ctx->errors != NULL) {
                    SemanticError err = CreateCustomError(SEMANTIC_ERROR_OTHER,
                                                          "break used outside of loop or switch",
                                                          stmt->line,
                                                          stmt->column);
                    AddError(ctx->errors, &err);
                }
                had_error = 1;
            }
            /* fall through */
        case STMT_CONTINUE:
            if (stmt->type == STMT_CONTINUE && ctx->loop_depth <= 0) {
                if (ctx->errors != NULL) {
                    SemanticError err = CreateCustomError(SEMANTIC_ERROR_OTHER,
                                                          "continue used outside of loop",
                                                          stmt->line,
                                                          stmt->column);
                    AddError(ctx->errors, &err);
                }
                had_error = 1;
            }
            break;
        case STMT_COMPOUND:
            if (PushScope(ctx, "block") != 0) {
                had_error = 1;
                break;
            }
            if (CheckStatements(stmt->value.stmt_list ? stmt->value.stmt_list->first : NULL,
                                ctx,
                                expected_return_type,
                                return_seen) != 0) {
                had_error = 1;
            }
            if (PopScope(ctx) != 0) {
                had_error = 1;
            }
            break;
    }

    return had_error;
}

int CheckExpressions(NExpr *root, SemanticContext *ctx) {

    if (root == NULL) {
        return 0;
    }
    if (ctx == NULL) {
        return 1;
    }
    return CheckExpression(root, ctx);
}

int CheckExpression(NExpr *expr, SemanticContext *ctx) {

    int had_error = 0;

    if (expr == NULL) {
        return 0;
    }
    if (ctx == NULL) {
        return 1;
    }

    switch (expr->type) {
        case EXPR_IDENT: {
            Symbol *sym = LookupSymbol(ctx, expr->value.ident_name);
            if (sym == NULL) {
                int handled = 0;
                if (ctx->current_class != NULL) {
                    ClassInfo *owner = FindFieldOwnerInHierarchy(ctx,
                                                                 ctx->current_class,
                                                                 expr->value.ident_name);
                    if (owner != NULL) {
                        FieldInfo *field = LookupClassField(owner, expr->value.ident_name);
                        int inside_class = strcmp(ctx->current_class->name, owner->name) == 0;
                        int inside_hierarchy = IsSameOrBaseClass(ctx,
                                                                 ctx->current_class->name,
                                                                 owner->name);
                        if (!IsFieldAccessible(field, inside_class, inside_hierarchy)) {
                            if (ctx->errors != NULL) {
                                SemanticError err = CreateAccessViolationError(field->name,
                                                                              AccessSpecToString(field->access),
                                                                              expr->line,
                                                                              expr->column);
                                AddError(ctx->errors, &err);
                            }
                            had_error = 1;
                        }
                        handled = 1;
                    }
                }
                if (!handled) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateUndefinedVariableError(expr->value.ident_name,
                                                                        expr->line,
                                                                        expr->column);
                        AddError(ctx->errors, &err);
                    }
                    had_error = 1;
                }
            } else if (sym->kind != SYMBOL_VARIABLE && sym->kind != SYMBOL_ENUM_ITEM) {
                if (ctx->errors != NULL) {
                    SemanticError err = CreateCustomError(SEMANTIC_ERROR_OTHER,
                                                          "Identifier is not a variable",
                                                          expr->line,
                                                          expr->column);
                    AddError(ctx->errors, &err);
                }
                had_error = 1;
            } else if (sym->kind == SYMBOL_ENUM_ITEM && sym->info.enum_item_info != NULL) {
                expr->enum_value = sym->info.enum_item_info->value;
                expr->enum_value_is_set = 1;
            } else if (!IsSymbolAccessible(sym, ctx)) {
                if (ctx->errors != NULL) {
                    SemanticError err = CreateOutOfScopeError(expr->value.ident_name,
                                                              expr->line,
                                                              expr->column);
                    AddError(ctx->errors, &err);
                }
                had_error = 1;
            }
            break;
        }
        case EXPR_CAST:
            if (expr->value.cast.expr != NULL) {
                if (CheckExpression(expr->value.cast.expr, ctx) != 0) {
                    had_error = 1;
                }
            }
            if (!EnsureTypeDefined(expr->value.cast.target_type, ctx, expr->line, expr->column)) {
                had_error = 1;
            }
            break;
        case EXPR_FUNC_CALL: {
            int ambiguous = 0;
            if (expr->value.func_call.func_name != NULL &&
                strcmp(expr->value.func_call.func_name, "destroy") == 0) {
                int ok = 1;
                if (expr->value.func_call.arg_count != 1 || expr->value.func_call.args == NULL) {
                    ok = 0;
                } else {
                    NExpr *arg = expr->value.func_call.args[0];
                    NType *arg_type = InferExpressionTypeSilent(arg, ctx);
                    if (arg_type == NULL ||
                        arg_type->kind != TYPE_KIND_CLASS ||
                        arg_type->class_name == NULL) {
                        ok = 0;
                    }
                }
                if (!ok && ctx->errors != NULL) {
                    SemanticError err = CreateCustomError(SEMANTIC_ERROR_OTHER,
                                                          "destroy expects a class instance",
                                                          expr->line,
                                                          expr->column);
                    AddError(ctx->errors, &err);
                    had_error = 1;
                }
                return had_error;
            }
            FunctionInfo *func = LookupFunctionOverload(ctx,
                                                        expr->value.func_call.func_name,
                                                        expr->value.func_call.args,
                                                        expr->value.func_call.arg_count,
                                                        &ambiguous);
            if (func != NULL) {
                char *owner_internal = BuildJvmInternalName("Main");
                char *desc = BuildJvmMethodDescriptor(func->return_type, func->params);
                if (owner_internal != NULL && desc != NULL) {
                    SetResolvedCallInfo(expr, owner_internal, desc, func->params);
                }
                if (owner_internal != NULL) {
                    free(owner_internal);
                }
                if (desc != NULL) {
                    free(desc);
                }
            }
            if (func == NULL && ctx->current_class != NULL) {
                int ambiguous_method = 0;
                ClassInfo *owner = NULL;
                MethodInfo *method = LookupMethodOverloadInHierarchy(ctx,
                                                                    ctx->current_class,
                                                                    expr->value.func_call.func_name,
                                                                    expr->value.func_call.args,
                                                                    expr->value.func_call.arg_count,
                                                                    NULL,
                                                                    &owner,
                                                                    &ambiguous_method);
                if (ambiguous_method && ctx->errors != NULL) {
                    SemanticError err = CreateCustomError(SEMANTIC_ERROR_OTHER,
                                                          "Ambiguous method overload",
                                                          expr->line,
                                                          expr->column);
                    AddError(ctx->errors, &err);
                    had_error = 1;
                }
                if (method != NULL && owner != NULL) {
                    char *owner_internal = BuildJvmInternalName(owner->name);
                    char *desc = BuildJvmMethodDescriptor(method->return_type, method->params);
                    if (owner_internal != NULL && desc != NULL) {
                        SetResolvedCallInfo(expr, owner_internal, desc, method->params);
                    }
                    if (owner_internal != NULL) {
                        free(owner_internal);
                    }
                    if (desc != NULL) {
                        free(desc);
                    }
                }
                if (method != NULL) {
                    int expected = method->params ? method->params->count : 0;
                    int actual = expr->value.func_call.arg_count;
                    if (actual > expected) {
                        if (ctx->errors != NULL) {
                            SemanticError err = CreateWrongArgCountError(method->name,
                                                                        expected,
                                                                        actual,
                                                                        expr->line,
                                                                        expr->column);
                            AddError(ctx->errors, &err);
                        }
                        had_error = 1;
                    } else if (actual < expected && method->params != NULL) {
                        for (int i = actual; i < expected; i++) {
                            NParam *param = method->params->params[i];
                            if (param == NULL || param->default_value == NULL) {
                                if (ctx->errors != NULL) {
                                    SemanticError err = CreateWrongArgCountError(method->name,
                                                                                expected,
                                                                                actual,
                                                                                expr->line,
                                                                                expr->column);
                                    AddError(ctx->errors, &err);
                                }
                                had_error = 1;
                                break;
                            }
                        }
                    }
                    for (int i = 0; i < expr->value.func_call.arg_count; i++) {
                        if (CheckExpression(expr->value.func_call.args[i], ctx) != 0) {
                            had_error = 1;
                        }
                    }
                    if (method->params != NULL) {
                        int count = expected < actual ? expected : actual;
                        for (int i = 0; i < count; i++) {
                            NParam *param = method->params->params[i];
                            NExpr *arg = expr->value.func_call.args[i];
                            if (param == NULL || arg == NULL) {
                                continue;
                            }
                            if (param->is_ref && !IsValidLValueExpr(arg)) {
                                if (ctx->errors != NULL) {
                                    SemanticError err = CreateCustomError(SEMANTIC_ERROR_INVALID_OPERANDS,
                                                                        "ref argument must be assignable",
                                                                        arg->line,
                                                                        arg->column);
                                    AddError(ctx->errors, &err);
                                }
                                had_error = 1;
                            }
                            if (param->param_type != NULL) {
                                NType *arg_type = InferExpressionTypeSilent(arg, ctx);
                                if (arg_type != NULL &&
                                    !IsArgumentCompatibleWithParameter(param->param_type, arg_type, param->is_ref)) {
                                    if (ctx->errors != NULL) {
                                        SemanticError err = CreateTypeMismatchError(TypeToString(param->param_type),
                                                                                    TypeToString(arg_type),
                                                                                    "as argument",
                                                                                    arg->line,
                                                                                    arg->column);
                                        AddError(ctx->errors, &err);
                                    }
                                    had_error = 1;
                                }
                            }
                        }
                    }
                    break;
                }
            }
            if (func == NULL) {
                if (ctx->errors != NULL) {
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
                had_error = 1;
            } else {
                int expected = func->params ? func->params->count : 0;
                int actual = expr->value.func_call.arg_count;
                if (func->allow_extra_args) {
                    if (actual < expected) {
                        if (ctx->errors != NULL) {
                            SemanticError err = CreateWrongArgCountError(func->name,
                                                                        expected,
                                                                        actual,
                                                                        expr->line,
                                                                        expr->column);
                            AddError(ctx->errors, &err);
                        }
                        had_error = 1;
                    }
                } else if (actual > expected) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateWrongArgCountError(func->name,
                                                                    expected,
                                                                    actual,
                                                                    expr->line,
                                                                    expr->column);
                        AddError(ctx->errors, &err);
                    }
                    had_error = 1;
                } else if (actual < expected && func->params != NULL) {
                    for (int i = actual; i < expected; i++) {
                        NParam *param = func->params->params[i];
                        if (param == NULL || param->default_value == NULL) {
                            if (ctx->errors != NULL) {
                                SemanticError err = CreateWrongArgCountError(func->name,
                                                                            expected,
                                                                            actual,
                                                                            expr->line,
                                                                            expr->column);
                                AddError(ctx->errors, &err);
                            }
                            had_error = 1;
                            break;
                        }
                    }
                }
                for (int i = 0; i < expr->value.func_call.arg_count; i++) {
                    if (CheckExpression(expr->value.func_call.args[i], ctx) != 0) {
                        had_error = 1;
                    }
                }
                if (func->is_builtin &&
                    (strcmp(func->name, "writef") == 0 || strcmp(func->name, "readf") == 0)) {
                    if (ValidateFormatArguments(func->name,
                                                expr->value.func_call.args,
                                                expr->value.func_call.arg_count,
                                                ctx,
                                                expr->line,
                                                expr->column) != 0) {
                        had_error = 1;
                    }
                    if (strcmp(func->name, "readf") == 0) {
                        for (int i = 1; i < expr->value.func_call.arg_count; i++) {
                            NExpr *arg = expr->value.func_call.args[i];
                            if (arg != NULL && !IsValidLValueExpr(arg)) {
                                if (ctx->errors != NULL) {
                                    SemanticError err = CreateCustomError(SEMANTIC_ERROR_INVALID_OPERANDS,
                                                                          "readf requires lvalue arguments",
                                                                          arg->line,
                                                                          arg->column);
                                    AddError(ctx->errors, &err);
                                }
                                had_error = 1;
                            }
                        }
                    }
                }
                if (func->params != NULL) {
                    int count = expected < actual ? expected : actual;
                    for (int i = 0; i < count; i++) {
                        NParam *param = func->params->params[i];
                        NExpr *arg = expr->value.func_call.args[i];
                        if (param == NULL || arg == NULL) {
                            continue;
                        }
                        if (param->is_ref && !IsValidLValueExpr(arg)) {
                            if (ctx->errors != NULL) {
                                SemanticError err = CreateCustomError(SEMANTIC_ERROR_INVALID_OPERANDS,
                                                                      "ref argument must be assignable",
                                                                      arg->line,
                                                                      arg->column);
                                AddError(ctx->errors, &err);
                            }
                            had_error = 1;
                        }
                        if (param->param_type != NULL) {
                            NType *arg_type = InferExpressionTypeSilent(arg, ctx);
                            if (arg_type != NULL &&
                                !IsArgumentCompatibleWithParameter(param->param_type, arg_type, param->is_ref)) {
                                if (ctx->errors != NULL) {
                                    SemanticError err = CreateTypeMismatchError(TypeToString(param->param_type),
                                                                                TypeToString(arg_type),
                                                                                "as argument",
                                                                                arg->line,
                                                                                arg->column);
                                    AddError(ctx->errors, &err);
                                }
                                had_error = 1;
                            }
                        }
                    }
                }
            }
            break;
        }
        case EXPR_METHOD_CALL: {
            NExpr *obj = expr->value.member_access.object;
            if (CheckExpression(obj, ctx) != 0) {
                had_error = 1;
            }
            for (int i = 0; i < expr->value.member_access.arg_count; i++) {
                if (CheckExpression(expr->value.member_access.args[i], ctx) != 0) {
                    had_error = 1;
                }
            }
            if (obj != NULL) {
                NType *obj_type = InferExpressionTypeSilent(obj, ctx);
                if (obj_type != NULL &&
                    (obj_type->kind == TYPE_KIND_CLASS || obj_type->kind == TYPE_KIND_CLASS_ARRAY)) {
                    if (obj_type->kind == TYPE_KIND_CLASS_ARRAY) {
                        if (ctx->errors != NULL) {
                            SemanticError err = CreateMethodNotFoundError(expr->value.member_access.member_name,
                                                                          TypeToString(obj_type),
                                                                          expr->line,
                                                                          expr->column);
                            AddError(ctx->errors, &err);
                        }
                        had_error = 1;
                        break;
                    }
                    const char *class_name = obj_type->class_name;
                    ClassInfo *class_info = LookupClass(ctx, class_name);
                    if (class_info == NULL) {
                        if (ctx->errors != NULL) {
                            SemanticError err = CreateUndefinedClassError(class_name,
                                                                          expr->line,
                                                                          expr->column);
                            AddError(ctx->errors, &err);
                        }
                        had_error = 1;
                    } else {
                        int ambiguous_method = 0;
                        ClassInfo *owner = NULL;
                        MethodInfo *method = LookupMethodOverloadInHierarchy(ctx,
                                                                            class_info,
                                                                            expr->value.member_access.member_name,
                                                                            expr->value.member_access.args,
                                                                            expr->value.member_access.arg_count,
                                                                            obj,
                                                                            &owner,
                                                                            &ambiguous_method);
                        if (ambiguous_method && ctx->errors != NULL) {
                            SemanticError err = CreateCustomError(SEMANTIC_ERROR_OTHER,
                                                                  "Ambiguous method overload",
                                                                  expr->line,
                                                                  expr->column);
                            AddError(ctx->errors, &err);
                            had_error = 1;
                        }
                        if (method == NULL) {
                            if (ctx->errors != NULL) {
                                SemanticError err = CreateMethodNotFoundError(expr->value.member_access.member_name,
                                                                              class_name,
                                                                              expr->line,
                                                                              expr->column);
                                AddError(ctx->errors, &err);
                            }
                            had_error = 1;
                        } else {
                            if (owner != NULL) {
                                char *owner_internal = BuildJvmInternalName(owner->name);
                                char *desc = BuildJvmMethodDescriptor(method->return_type, method->params);
                                if (owner_internal != NULL && desc != NULL) {
                                    SetResolvedCallInfo(expr, owner_internal, desc, method->params);
                                }
                                if (owner_internal != NULL) {
                                    free(owner_internal);
                                }
                                if (desc != NULL) {
                                    free(desc);
                                }
                            }
                            {
                                    int expected = method->params ? method->params->count : 0;
                                    int actual = expr->value.member_access.arg_count;
                                    if (actual > expected) {
                                        if (ctx->errors != NULL) {
                                            SemanticError err = CreateWrongArgCountError(method->name,
                                                                                        expected,
                                                                                        actual,
                                                                                        expr->line,
                                                                                        expr->column);
                                            AddError(ctx->errors, &err);
                                        }
                                        had_error = 1;
                                    } else if (actual < expected && method->params != NULL) {
                                        for (int i = actual; i < expected; i++) {
                                            NParam *param = method->params->params[i];
                                            if (param == NULL || param->default_value == NULL) {
                                                if (ctx->errors != NULL) {
                                                    SemanticError err = CreateWrongArgCountError(method->name,
                                                                                                expected,
                                                                                                actual,
                                                                                                expr->line,
                                                                                                expr->column);
                                                    AddError(ctx->errors, &err);
                                                }
                                                had_error = 1;
                                                break;
                                            }
                                        }
                                    }
                                    if (method->params != NULL) {
                                        int count = expected < actual ? expected : actual;
                                        for (int i = 0; i < count; i++) {
                                            NParam *param = method->params->params[i];
                                            NExpr *arg = expr->value.member_access.args[i];
                                            if (param == NULL || arg == NULL) {
                                                continue;
                                            }
                                            if (param->is_ref && !IsValidLValueExpr(arg)) {
                                                if (ctx->errors != NULL) {
                                                    SemanticError err = CreateCustomError(SEMANTIC_ERROR_INVALID_OPERANDS,
                                                                                          "ref argument must be assignable",
                                                                                          arg->line,
                                                                                          arg->column);
                                                    AddError(ctx->errors, &err);
                                                }
                                                had_error = 1;
                                            }
                                            if (param->param_type != NULL) {
                                                NType *arg_type = InferExpressionTypeSilent(arg, ctx);
                                                if (arg_type != NULL &&
                                                    !IsArgumentCompatibleWithParameter(param->param_type, arg_type, param->is_ref)) {
                                                    if (ctx->errors != NULL) {
                                                        SemanticError err = CreateTypeMismatchError(TypeToString(param->param_type),
                                                                                                    TypeToString(arg_type),
                                                                                                    "as argument",
                                                                                                    arg->line,
                                                                                                    arg->column);
                                                        AddError(ctx->errors, &err);
                                                    }
                                                    had_error = 1;
                                                }
                                            }
                                        }
                                    }
                            }
                        }
                    }
                } else if (obj_type != NULL) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateMethodNotFoundError(expr->value.member_access.member_name,
                                                                      TypeToString(obj_type),
                                                                      expr->line,
                                                                      expr->column);
                        AddError(ctx->errors, &err);
                    }
                    had_error = 1;
                }
            }
            break;
        }
        case EXPR_MEMBER_ACCESS: {
            NExpr *obj = expr->value.member_access.object;
            if (obj != NULL) {
                if (obj->type == EXPR_IDENT) {
                    Symbol *sym = LookupSymbol(ctx, obj->value.ident_name);
                    if (sym != NULL && sym->kind == SYMBOL_ENUM_TYPE) {
                        EnumInfo *en = sym->info.enum_info;
                        EnumItemInfo *item = LookupEnumItem(en, expr->value.member_access.member_name);
                        if (item == NULL) {
                            if (ctx->errors != NULL) {
                                SemanticError err = CreateCustomError(SEMANTIC_ERROR_OTHER,
                                                                    "Enum item not found",
                                                                    expr->line,
                                                                    expr->column);
                                AddError(ctx->errors, &err);
                            }
                            had_error = 1;
                        } else {
                            expr->enum_value = item->value;
                            expr->enum_value_is_set = 1;
                        }
                        break;
                    }
                }
                if (CheckExpression(obj, ctx) != 0) {
                    had_error = 1;
                }
                NType *obj_type = InferExpressionTypeSilent(obj, ctx);
                if (obj_type != NULL &&
                    (obj_type->kind == TYPE_KIND_BASE_ARRAY ||
                     obj_type->kind == TYPE_KIND_CLASS_ARRAY ||
                     obj_type->kind == TYPE_KIND_ENUM_ARRAY) &&
                    expr->value.member_access.member_name != NULL &&
                    strcmp(expr->value.member_access.member_name, "length") == 0) {
                    break;
                }
                if (obj_type != NULL &&
                    (obj_type->kind == TYPE_KIND_CLASS || obj_type->kind == TYPE_KIND_CLASS_ARRAY)) {
                    if (obj_type->kind == TYPE_KIND_CLASS_ARRAY) {
                        if (ctx->errors != NULL) {
                            SemanticError err = CreateFieldNotFoundError(expr->value.member_access.member_name,
                                                                         TypeToString(obj_type),
                                                                         expr->line,
                                                                         expr->column);
                            AddError(ctx->errors, &err);
                        }
                        had_error = 1;
                        break;
                    }
                    const char *class_name = obj_type->class_name;
                    ClassInfo *class_info = LookupClass(ctx, class_name);
                    if (class_info == NULL) {
                        if (ctx->errors != NULL) {
                            SemanticError err = CreateUndefinedClassError(class_name,
                                                                          expr->line,
                                                                          expr->column);
                            AddError(ctx->errors, &err);
                        }
                        had_error = 1;
                    } else {
                    FieldInfo *field = LookupClassFieldInHierarchy(ctx,
                                                                   class_info,
                                                                   expr->value.member_access.member_name);
                    if (field == NULL) {
                            if (ctx->errors != NULL) {
                                SemanticError err = CreateFieldNotFoundError(expr->value.member_access.member_name,
                                                                            class_name,
                                                                            expr->line,
                                                                            expr->column);
                                AddError(ctx->errors, &err);
                            }
                            had_error = 1;
                    } else {
                        int inside_class = IsInsideExactClass(ctx, obj, class_info->name);
                        int inside_hierarchy = IsInsideHierarchyAccess(ctx, obj, class_info->name);
                        if (!IsFieldAccessible(field, inside_class, inside_hierarchy)) {
                                if (ctx->errors != NULL) {
                                    SemanticError err = CreateAccessViolationError(field->name,
                                                                                  AccessSpecToString(field->access),
                                                                                  expr->line,
                                                                                  expr->column);
                                    AddError(ctx->errors, &err);
                                }
                                had_error = 1;
                            }
                        }
                    }
                } else if (obj_type != NULL) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateFieldNotFoundError(expr->value.member_access.member_name,
                                                                    TypeToString(obj_type),
                                                                    expr->line,
                                                                    expr->column);
                        AddError(ctx->errors, &err);
                    }
                    had_error = 1;
                }
            }
            break;
        }
        case EXPR_ARRAY_ACCESS: {
            NExpr *array_expr = expr->value.array_access.array;
            NExpr *index_expr = expr->value.array_access.index;
            NExpr *index_end = expr->value.array_access.index_end;
            if (CheckExpression(array_expr, ctx) != 0) {
                had_error = 1;
            }
            if (CheckExpression(index_expr, ctx) != 0) {
                had_error = 1;
            }
            if (CheckExpression(index_end, ctx) != 0) {
                had_error = 1;
            }
            if (array_expr != NULL) {
                NType *array_type = InferExpressionTypeSilent(array_expr, ctx);
                if (array_type != NULL &&
                    array_type->kind != TYPE_KIND_BASE_ARRAY &&
                    array_type->kind != TYPE_KIND_CLASS_ARRAY &&
                    array_type->kind != TYPE_KIND_ENUM_ARRAY) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateInvalidOperandsError("[]",
                                                                      TypeToString(array_type),
                                                                      NULL,
                                                                      expr->line,
                                                                      expr->column);
                        AddError(ctx->errors, &err);
                    }
                    had_error = 1;
                }
            }
            if (index_expr != NULL) {
                NType *index_type = InferExpressionTypeSilent(index_expr, ctx);
                if (index_type != NULL && !CanBeArrayIndex(index_type)) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateInvalidArrayIndexError(TypeToString(index_type),
                                                                        index_expr->line,
                                                                        index_expr->column);
                        AddError(ctx->errors, &err);
                    }
                    had_error = 1;
                }
            }
            if (index_end != NULL) {
                NType *index_type = InferExpressionTypeSilent(index_end, ctx);
                if (index_type != NULL && !CanBeArrayIndex(index_type)) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateInvalidArrayIndexError(TypeToString(index_type),
                                                                        index_end->line,
                                                                        index_end->column);
                        AddError(ctx->errors, &err);
                    }
                    had_error = 1;
                }
            }
            break;
        }
        case EXPR_BINARY_OP: {
            NExpr *left = expr->value.binary.left;
            NExpr *right = expr->value.binary.right;
            if (CheckExpression(left, ctx) != 0) {
                had_error = 1;
            }
            if (CheckExpression(right, ctx) != 0) {
                had_error = 1;
            }
            if (left != NULL && right != NULL) {
                NType *left_type = InferExpressionTypeSilent(left, ctx);
                NType *right_type = InferExpressionTypeSilent(right, ctx);
                NType *result_type = InferBinaryOperationType(expr->value.binary.op,
                                                              left_type,
                                                              right_type,
                                                              ctx,
                                                              expr->line,
                                                              expr->column);
                if (result_type == NULL && left_type != NULL && right_type != NULL) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateInvalidOperandsError(OpToString(expr->value.binary.op),
                                                                      TypeToString(left_type),
                                                                      TypeToString(right_type),
                                                                      expr->line,
                                                                      expr->column);
                        AddError(ctx->errors, &err);
                    }
                    had_error = 1;
                }
            }
            break;
        }
        case EXPR_UNARY_OP: {
            NExpr *operand = expr->value.unary.operand;
            if (CheckExpression(operand, ctx) != 0) {
                had_error = 1;
            }
            if (operand != NULL) {
                NType *operand_type = InferExpressionTypeSilent(operand, ctx);
                NType *result_type = InferUnaryOperationType(expr->value.unary.op, operand_type);
                if (result_type == NULL && operand_type != NULL) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateInvalidOperandsError(OpToString(expr->value.unary.op),
                                                                      TypeToString(operand_type),
                                                                      NULL,
                                                                      expr->line,
                                                                      expr->column);
                        AddError(ctx->errors, &err);
                    }
                    had_error = 1;
                }
            }
            break;
        }
        case EXPR_ASSIGN: {
            NExpr *left = expr->value.binary.left;
            NExpr *right = expr->value.binary.right;
            if (CheckExpression(left, ctx) != 0) {
                had_error = 1;
            }
            if (CheckExpression(right, ctx) != 0) {
                had_error = 1;
            }
            if (!IsValidLValueExpr(left)) {
                if (ctx->errors != NULL) {
                    SemanticError err = CreateCustomError(SEMANTIC_ERROR_INVALID_OPERANDS,
                                                          "Left side of assignment is not assignable",
                                                          expr->line,
                                                          expr->column);
                    AddError(ctx->errors, &err);
                }
                had_error = 1;
                break;
            }
            if (left != NULL && right != NULL) {
                NType *left_type = InferExpressionTypeSilent(left, ctx);
                NType *right_type = InferExpressionTypeSilent(right, ctx);
                if (left_type != NULL && right_type != NULL) {
                    if (expr->value.binary.op == OP_BITWISE_NOT_ASSIGN) {
                        if (!CheckAppendAssignment(left_type, right_type, ctx, expr->line, expr->column)) {
                            had_error = 1;
                        }
                    } else if (!CanAssign(left_type, right_type)) {
                        if (ctx->errors != NULL) {
                            SemanticError err = CreateTypeMismatchError(TypeToString(left_type),
                                                                        TypeToString(right_type),
                                                                        "in assignment",
                                                                        expr->line,
                                                                        expr->column);
                            AddError(ctx->errors, &err);
                        }
                        had_error = 1;
                    }
                }
            }
            break;
        }
        case EXPR_NEW: {
            if (!EnsureTypeDefined(expr->value.new_expr.type, ctx, expr->line, expr->column)) {
                had_error = 1;
            }
            if (expr->value.new_expr.type != NULL &&
                (expr->value.new_expr.type->kind == TYPE_KIND_CLASS ||
                 expr->value.new_expr.type->kind == TYPE_KIND_CLASS_ARRAY)) {
                if (LookupClass(ctx, expr->value.new_expr.type->class_name) == NULL) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateUndefinedTypeError(expr->value.new_expr.type->class_name,
                                                                    expr->line,
                                                                    expr->column);
                        AddError(ctx->errors, &err);
                    }
                    had_error = 1;
                } else if (expr->value.new_expr.type->kind == TYPE_KIND_CLASS) {
                    ClassInfo *class_info = LookupClass(ctx, expr->value.new_expr.type->class_name);
                    NCtorDef *ctor = NULL;
                    int ambiguous = 0;
                    int actual = expr->value.new_expr.init_count;

                    if (class_info != NULL && class_info->constructor_count > 0) {
                        ctor = LookupConstructorOverload(class_info,
                                                         expr->value.new_expr.init_exprs,
                                                         actual,
                                                         ctx,
                                                         &ambiguous);
                        if (ctor == NULL) {
                            if (ctx->errors != NULL) {
                                SemanticError err = CreateCustomError(SEMANTIC_ERROR_OTHER,
                                                                      ambiguous
                                                                          ? "Ambiguous constructor call"
                                                                          : "No matching constructor for class",
                                                                      expr->line,
                                                                      expr->column);
                                AddError(ctx->errors, &err);
                            }
                            had_error = 1;
                        }
                    } else if (actual > 0) {
                        if (ctx->errors != NULL) {
                            SemanticError err = CreateWrongArgCountError("constructor",
                                                                        0,
                                                                        actual,
                                                                        expr->line,
                                                                        expr->column);
                            AddError(ctx->errors, &err);
                        }
                        had_error = 1;
                    }

                      if (ctor != NULL) {
                          {
                              char *owner_internal = BuildJvmInternalName(expr->value.new_expr.type->class_name);
                              char *desc = BuildJvmMethodDescriptor(NULL, ctor->params);
                              if (owner_internal != NULL && desc != NULL) {
                                  SetResolvedCallInfo(expr, owner_internal, desc, ctor->params);
                              }
                              if (owner_internal != NULL) {
                                  free(owner_internal);
                              }
                              if (desc != NULL) {
                                  free(desc);
                              }
                          }
                          int expected = (ctor->params != NULL) ? ctor->params->count : 0;
                        if (actual < expected && ctor->params != NULL) {
                            for (int i = actual; i < expected; i++) {
                                NParam *param = ctor->params->params[i];
                                if (param == NULL || param->default_value == NULL) {
                                    if (ctx->errors != NULL) {
                                        SemanticError err = CreateWrongArgCountError("constructor",
                                                                                    expected,
                                                                                    actual,
                                                                                    expr->line,
                                                                                    expr->column);
                                        AddError(ctx->errors, &err);
                      }
                      if (ctor == NULL && actual == 0) {
                          char *owner_internal = BuildJvmInternalName(expr->value.new_expr.type->class_name);
                          if (owner_internal != NULL) {
                              SetResolvedCallInfo(expr, owner_internal, "()V", NULL);
                              free(owner_internal);
                          }
                      }
                                    had_error = 1;
                                    break;
                                }
                            }
                        }
                        if (ctor->params != NULL) {
                            int count = expected < actual ? expected : actual;
                            for (int i = 0; i < count; i++) {
                                NParam *param = ctor->params->params[i];
                                NExpr *arg = expr->value.new_expr.init_exprs[i];
                                if (param == NULL || arg == NULL) {
                                    continue;
                                }
                                if (param->is_ref && !IsValidLValueExpr(arg)) {
                                    if (ctx->errors != NULL) {
                                        SemanticError err = CreateCustomError(SEMANTIC_ERROR_INVALID_OPERANDS,
                                                                              "ref argument must be assignable",
                                                                              arg->line,
                                                                              arg->column);
                                        AddError(ctx->errors, &err);
                                    }
                                    had_error = 1;
                                }
                                if (param->param_type != NULL) {
                                    NType *arg_type = InferExpressionTypeSilent(arg, ctx);
                                    if (arg_type != NULL &&
                                        !IsArgumentCompatibleWithParameter(param->param_type, arg_type, param->is_ref)) {
                                        if (ctx->errors != NULL) {
                                            SemanticError err = CreateTypeMismatchError(TypeToString(param->param_type),
                                                                                        TypeToString(arg_type),
                                                                                        "as argument",
                                                                                        arg->line,
                                                                                        arg->column);
                                            AddError(ctx->errors, &err);
                                        }
                                        had_error = 1;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            for (int i = 0; i < expr->value.new_expr.init_count; i++) {
                if (CheckExpression(expr->value.new_expr.init_exprs[i], ctx) != 0) {
                    had_error = 1;
                }
            }
            break;
        }
        case EXPR_PAREN:
            if (CheckExpression(expr->value.inner_expr, ctx) != 0) {
                had_error = 1;
            }
            break;
        case EXPR_SUPER_METHOD:
            for (int i = 0; i < expr->value.member_access.arg_count; i++) {
                if (CheckExpression(expr->value.member_access.args[i], ctx) != 0) {
                    had_error = 1;
                }
            }
            if (ctx == NULL || ctx->current_class == NULL) {
                if (ctx != NULL && ctx->errors != NULL) {
                    SemanticError err = CreateCustomError(SEMANTIC_ERROR_OTHER,
                                                          "super used outside of class",
                                                          expr->line,
                                                          expr->column);
                    AddError(ctx->errors, &err);
                }
                had_error = 1;
                break;
            }
            if (ctx->current_class->base_class == NULL) {
                if (ctx->errors != NULL) {
                    SemanticError err = CreateUndefinedClassError("super",
                                                                  expr->line,
                                                                  expr->column);
                    AddError(ctx->errors, &err);
                }
                had_error = 1;
                break;
            }
            {
                ClassInfo *base_info = LookupClass(ctx, ctx->current_class->base_class);
                if (base_info == NULL) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateUndefinedClassError(ctx->current_class->base_class,
                                                                      expr->line,
                                                                      expr->column);
                        AddError(ctx->errors, &err);
                    }
                    had_error = 1;
                    break;
                }
                {
                    int ambiguous_method = 0;
                    ClassInfo *owner = NULL;
                    MethodInfo *method = LookupMethodOverloadInHierarchy(ctx,
                                                                         base_info,
                                                                         expr->value.member_access.member_name,
                                                                         expr->value.member_access.args,
                                                                         expr->value.member_access.arg_count,
                                                                         NULL,
                                                                         &owner,
                                                                         &ambiguous_method);
                    if (ambiguous_method && ctx->errors != NULL) {
                        SemanticError err = CreateCustomError(SEMANTIC_ERROR_OTHER,
                                                              "Ambiguous method overload",
                                                              expr->line,
                                                              expr->column);
                        AddError(ctx->errors, &err);
                        had_error = 1;
                    }
                    if (method == NULL) {
                        if (ctx->errors != NULL) {
                            SemanticError err = CreateMethodNotFoundError(expr->value.member_access.member_name,
                                                                          base_info->name,
                                                                          expr->line,
                                                                          expr->column);
                            AddError(ctx->errors, &err);
                        }
                        had_error = 1;
                        break;
                    }
                    if (owner != NULL) {
                        char *owner_internal = BuildJvmInternalName(owner->name);
                        char *desc = BuildJvmMethodDescriptor(method->return_type, method->params);
                        if (owner_internal != NULL && desc != NULL) {
                            SetResolvedCallInfo(expr, owner_internal, desc, method->params);
                        }
                        if (owner_internal != NULL) {
                            free(owner_internal);
                        }
                        if (desc != NULL) {
                            free(desc);
                        }
                    }
                    {
                        int expected = method->params ? method->params->count : 0;
                        int actual = expr->value.member_access.arg_count;
                        if (actual > expected) {
                            if (ctx->errors != NULL) {
                                SemanticError err = CreateWrongArgCountError(method->name,
                                                                            expected,
                                                                            actual,
                                                                            expr->line,
                                                                            expr->column);
                                AddError(ctx->errors, &err);
                            }
                            had_error = 1;
                        } else if (actual < expected && method->params != NULL) {
                            for (int i = actual; i < expected; i++) {
                                NParam *param = method->params->params[i];
                                if (param == NULL || param->default_value == NULL) {
                                    if (ctx->errors != NULL) {
                                        SemanticError err = CreateWrongArgCountError(method->name,
                                                                                    expected,
                                                                                    actual,
                                                                                    expr->line,
                                                                                    expr->column);
                                        AddError(ctx->errors, &err);
                                    }
                                    had_error = 1;
                                    break;
                                }
                            }
                        }
                        if (method->params != NULL) {
                            int count = expected < actual ? expected : actual;
                            for (int i = 0; i < count; i++) {
                                NParam *param = method->params->params[i];
                                NExpr *arg = expr->value.member_access.args[i];
                                if (param == NULL || arg == NULL) {
                                    continue;
                                }
                                if (param->is_ref && !IsValidLValueExpr(arg)) {
                                    if (ctx->errors != NULL) {
                                        SemanticError err = CreateCustomError(SEMANTIC_ERROR_INVALID_OPERANDS,
                                                                              "ref argument must be assignable",
                                                                              arg->line,
                                                                              arg->column);
                                        AddError(ctx->errors, &err);
                                    }
                                    had_error = 1;
                                }
                                if (param->param_type != NULL) {
                                    NType *arg_type = InferExpressionTypeSilent(arg, ctx);
                                    if (arg_type != NULL &&
                                        !IsArgumentCompatibleWithParameter(param->param_type, arg_type, param->is_ref)) {
                                        if (ctx->errors != NULL) {
                                            SemanticError err = CreateTypeMismatchError(TypeToString(param->param_type),
                                                                                        TypeToString(arg_type),
                                                                                        "as argument",
                                                                                        arg->line,
                                                                                        arg->column);
                                            AddError(ctx->errors, &err);
                                        }
                                        had_error = 1;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            break;
        case EXPR_SUPER:
            if (ctx == NULL || ctx->current_class == NULL) {
                if (ctx != NULL && ctx->errors != NULL) {
                    SemanticError err = CreateCustomError(SEMANTIC_ERROR_OTHER,
                                                          "super used outside of class",
                                                          expr->line,
                                                          expr->column);
                    AddError(ctx->errors, &err);
                }
                had_error = 1;
            } else if (ctx->current_class->base_class == NULL) {
                if (ctx->errors != NULL) {
                    SemanticError err = CreateUndefinedClassError("super",
                                                                  expr->line,
                                                                  expr->column);
                    AddError(ctx->errors, &err);
                }
                had_error = 1;
            }
            break;
        case EXPR_THIS:
            if (ctx == NULL || ctx->current_class == NULL) {
                if (ctx != NULL && ctx->errors != NULL) {
                    SemanticError err = CreateCustomError(SEMANTIC_ERROR_OTHER,
                                                          "this used outside of class",
                                                          expr->line,
                                                          expr->column);
                    AddError(ctx->errors, &err);
                }
                had_error = 1;
            }
            break;
        case EXPR_INT:
        case EXPR_FLOAT:
        case EXPR_CHAR:
        case EXPR_STRING:
        case EXPR_BOOL:
        case EXPR_NULL:
        case EXPR_NAN:
            break;
    }

    return had_error;
}

int IsSymbolAccessible(Symbol *symbol, SemanticContext *ctx) {

    Scope *scope;

    if (symbol == NULL || ctx == NULL) {
        return 0;
    }

    if (symbol->scope_depth == 0) {
        return 1;
    }

    scope = GetCurrentScope(ctx);
    while (scope != NULL) {
        if (scope->depth == symbol->scope_depth) {
            return 1;
        }
        scope = scope->parent;
    }

    return 0;
}

/* ============================================================================
   ТРЕТИЙ ПРОХОД: АТРИБУТИРОВАНИЕ
   ============================================================================ */

int AttributeExpressions(NExpr *root, SemanticContext *ctx) {

    if (root == NULL) {
        return 0;
    }
    if (ctx == NULL) {
        return 1;
    }

    switch (root->type) {
        case EXPR_PAREN:
            AttributeExpressions(root->value.inner_expr, ctx);
            break;
        case EXPR_CAST:
            AttributeExpressions(root->value.cast.expr, ctx);
            break;
        case EXPR_NEW:
            for (int i = 0; i < root->value.new_expr.init_count; i++) {
                AttributeExpressions(root->value.new_expr.init_exprs[i], ctx);
            }
            break;
        case EXPR_ARRAY_ACCESS:
            AttributeExpressions(root->value.array_access.array, ctx);
            AttributeExpressions(root->value.array_access.index, ctx);
            AttributeExpressions(root->value.array_access.index_end, ctx);
            break;
        case EXPR_MEMBER_ACCESS:
            AttributeExpressions(root->value.member_access.object, ctx);
            break;
        case EXPR_METHOD_CALL:
            AttributeExpressions(root->value.member_access.object, ctx);
            for (int i = 0; i < root->value.member_access.arg_count; i++) {
                AttributeExpressions(root->value.member_access.args[i], ctx);
            }
            break;
        case EXPR_FUNC_CALL:
            for (int i = 0; i < root->value.func_call.arg_count; i++) {
                AttributeExpressions(root->value.func_call.args[i], ctx);
            }
            break;
        case EXPR_SUPER_METHOD:
            for (int i = 0; i < root->value.member_access.arg_count; i++) {
                AttributeExpressions(root->value.member_access.args[i], ctx);
            }
            break;
        case EXPR_UNARY_OP:
            AttributeExpressions(root->value.unary.operand, ctx);
            break;
        case EXPR_BINARY_OP:
        case EXPR_ASSIGN:
            AttributeExpressions(root->value.binary.left, ctx);
            AttributeExpressions(root->value.binary.right, ctx);
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
        case EXPR_SUPER:
            break;
    }

    root->inferred_type = InferExpressionTypeSilent(root, ctx);
    return 0;
}

static int AttributeStatement(NStmt *stmt, SemanticContext *ctx) {
    Scope *scope;

    if (stmt == NULL) {
        return 0;
    }
    if (ctx == NULL) {
        return 1;
    }

    scope = GetCurrentScope(ctx);
    stmt->scope_id = scope ? scope->depth : -1;

    switch (stmt->type) {
        case STMT_EXPR:
        case STMT_RETURN:
            AttributeExpressions(stmt->value.expr, ctx);
            break;
        case STMT_DECL:
            if (stmt->value.decl.init_decls) {
                for (int i = 0; i < stmt->value.decl.init_decls->count; i++) {
                    NInitDecl *decl = stmt->value.decl.init_decls->decls[i];
                    if (decl && decl->name && stmt->value.decl.decl_type) {
                        VariableInfo *var = (VariableInfo*)malloc(sizeof(VariableInfo));
                        if (var != NULL) {
                            memset(var, 0, sizeof(VariableInfo));
                            var->name = decl->name;
                            var->type = stmt->value.decl.decl_type;
                            var->is_initialized = (decl->initializer != NULL);
                            var->is_param = 0;
                            var->is_ref = 0;
                            var->line = stmt->line;
                            var->column = stmt->column;
                            if (AddLocalVariable(ctx, var) != 0) {
                                free(var);
                            }
                        }
                    }
                    if (decl && decl->initializer && decl->initializer->expr) {
                        AttributeExpressions(decl->initializer->expr, ctx);
                    }
                    if (decl && decl->initializer && decl->initializer->is_array) {
                        for (int j = 0; j < decl->initializer->array_init.count; j++) {
                            NExpr *elem = decl->initializer->array_init.elements[j];
                            if (elem != NULL) {
                                AttributeExpressions(elem, ctx);
                            }
                        }
                    }
                }
            }
            break;
        case STMT_COMPOUND:
            if (PushScope(ctx, "block") != 0) {
                return 1;
            }
            AttributeStatements(stmt->value.stmt_list ? stmt->value.stmt_list->first : NULL, ctx);
            if (PopScope(ctx) != 0) {
                return 1;
            }
            break;
        case STMT_IF:
            AttributeExpressions(stmt->value.if_stmt.condition, ctx);
            AttributeStatement(stmt->value.if_stmt.then_stmt, ctx);
            AttributeStatement(stmt->value.if_stmt.else_stmt, ctx);
            break;
        case STMT_WHILE:
            AttributeExpressions(stmt->value.while_stmt.condition, ctx);
            AttributeStatement(stmt->value.while_stmt.body, ctx);
            break;
        case STMT_DO_WHILE:
            AttributeStatement(stmt->value.do_while_stmt.body, ctx);
            AttributeExpressions(stmt->value.do_while_stmt.condition, ctx);
            break;
        case STMT_FOR:
            if (PushScope(ctx, "for") != 0) {
                return 1;
            }
            AttributeExpressions(stmt->value.for_stmt.init_expr, ctx);
            if (stmt->value.for_stmt.init_decls) {
                for (int i = 0; i < stmt->value.for_stmt.init_decls->count; i++) {
                    NInitDecl *decl = stmt->value.for_stmt.init_decls->decls[i];
                    if (decl && decl->name && stmt->value.for_stmt.init_decl_type) {
                        VariableInfo *var = (VariableInfo*)malloc(sizeof(VariableInfo));
                        if (var != NULL) {
                            memset(var, 0, sizeof(VariableInfo));
                            var->name = decl->name;
                            var->type = stmt->value.for_stmt.init_decl_type;
                            var->is_initialized = (decl->initializer != NULL);
                            var->is_param = 0;
                            var->is_ref = 0;
                            var->line = stmt->line;
                            var->column = stmt->column;
                            if (AddLocalVariable(ctx, var) != 0) {
                                free(var);
                            }
                        }
                    }
                    if (decl && decl->initializer && decl->initializer->expr) {
                        AttributeExpressions(decl->initializer->expr, ctx);
                    }
                }
            }
            AttributeExpressions(stmt->value.for_stmt.cond_expr, ctx);
            AttributeExpressions(stmt->value.for_stmt.iter_expr, ctx);
            AttributeStatement(stmt->value.for_stmt.body, ctx);
            if (PopScope(ctx) != 0) {
                return 1;
            }
            break;
        case STMT_FOREACH:
            if (PushScope(ctx, "foreach") != 0) {
                return 1;
            }
            {
                NType *collection_type = NULL;
                NType *element_type = NULL;
                NType *var_type = NULL;
                if (stmt->value.foreach_stmt.is_typed) {
                    var_type = stmt->value.foreach_stmt.var_type;
                } else if (stmt->value.foreach_stmt.collection != NULL) {
                    collection_type = InferExpressionTypeSilent(stmt->value.foreach_stmt.collection, ctx);
                    element_type = GetForeachElementType(collection_type);
                    var_type = element_type;
                }
                if (var_type != NULL && stmt->value.foreach_stmt.var_name != NULL) {
                    VariableInfo *var = (VariableInfo*)malloc(sizeof(VariableInfo));
                    if (var != NULL) {
                        memset(var, 0, sizeof(VariableInfo));
                        var->name = stmt->value.foreach_stmt.var_name;
                        var->type = var_type;
                        var->is_initialized = 1;
                        var->is_param = 0;
                        var->is_ref = 0;
                        var->line = stmt->line;
                        var->column = stmt->column;
                        if (AddLocalVariable(ctx, var) != 0) {
                            free(var);
                        }
                    }
                }
            }
            AttributeExpressions(stmt->value.foreach_stmt.collection, ctx);
            AttributeStatement(stmt->value.foreach_stmt.body, ctx);
            if (PopScope(ctx) != 0) {
                return 1;
            }
            break;
        case STMT_SWITCH:
            AttributeExpressions(stmt->value.switch_stmt.expr, ctx);
            for (int i = 0; i < stmt->value.switch_stmt.cases.count; i++) {
                NCaseItem *item = stmt->value.switch_stmt.cases.items[i];
                if (item && item->case_expr) {
                    AttributeExpressions(item->case_expr, ctx);
                }
                if (item && item->stmts) {
                    AttributeStatements(item->stmts->first, ctx);
                }
            }
            break;
        case STMT_BREAK:
        case STMT_CONTINUE:
            break;
    }

    return 0;
}

int AttributeStatements(NStmt *stmts, SemanticContext *ctx) {
    NStmt *stmt;

    if (stmts == NULL) {
        return 0;
    }
    if (ctx == NULL) {
        return 1;
    }

    stmt = stmts;
    while (stmt != NULL) {
        AttributeStatement(stmt, ctx);
        stmt = stmt->next;
    }
    return 0;
}

int AttributeJvmRefKeys(NProgram *root, SemanticContext *ctx) {
    NSourceItem *item;
    if (root == NULL || ctx == NULL) {
        return 0;
    }
    item = root->first_item;
    while (item != NULL) {
        switch (item->type) {
            case SOURCE_ITEM_FUNC:
                if (item->value.func && item->value.func->body) {
                    AttributeJvmRefKeysInStmtList(item->value.func->body, ctx);
                }
                break;
            case SOURCE_ITEM_CLASS: {
                NClassDef *class_def = item->value.class_def;
                NClassMember *member = class_def ? class_def->members.first : NULL;
                ClassInfo *saved = ctx->current_class;
                if (class_def && class_def->class_name) {
                    ctx->current_class = LookupClass(ctx, class_def->class_name);
                }
                while (member != NULL) {
                    if (member->type == CLASS_MEMBER_METHOD && member->value.method &&
                        member->value.method->body) {
                        AttributeJvmRefKeysInStmtList(member->value.method->body, ctx);
                    } else if (member->type == CLASS_MEMBER_CTOR && member->value.ctor &&
                        member->value.ctor->body) {
                        AttributeJvmRefKeysInStmtList(member->value.ctor->body, ctx);
                    } else if (member->type == CLASS_MEMBER_DTOR && member->value.dtor &&
                        member->value.dtor->body) {
                        AttributeJvmRefKeysInStmtList(member->value.dtor->body, ctx);
                    }
                    member = member->next;
                }
                ctx->current_class = saved;
                break;
            }
            case SOURCE_ITEM_DECL:
            case SOURCE_ITEM_ENUM:
                break;
        }
        item = item->next;
    }
    return HasErrors(ctx->errors) ? 1 : 0;
}

static int AttributeJvmRefKeysInStmtList(NStmt *stmts, SemanticContext *ctx) {
    NStmt *stmt = stmts;
    while (stmt != NULL) {
        AttributeJvmRefKeysInStmt(stmt, ctx);
        stmt = stmt->next;
    }
    return 0;
}

static int AttributeJvmRefKeysInStmt(NStmt *stmt, SemanticContext *ctx) {
    if (stmt == NULL) {
        return 0;
    }
    switch (stmt->type) {
        case STMT_EXPR:
        case STMT_RETURN:
            AttributeJvmRefKeysInExpr(stmt->value.expr, ctx);
            break;
        case STMT_DECL:
            if (stmt->value.decl.init_decls) {
                for (int i = 0; i < stmt->value.decl.init_decls->count; i++) {
                    NInitDecl *decl = stmt->value.decl.init_decls->decls[i];
                    if (decl && decl->initializer && decl->initializer->expr) {
                        AttributeJvmRefKeysInExpr(decl->initializer->expr, ctx);
                    }
                }
            }
            break;
        case STMT_COMPOUND:
            AttributeJvmRefKeysInStmtList(stmt->value.stmt_list ? stmt->value.stmt_list->first : NULL, ctx);
            break;
        case STMT_IF:
            AttributeJvmRefKeysInExpr(stmt->value.if_stmt.condition, ctx);
            AttributeJvmRefKeysInStmt(stmt->value.if_stmt.then_stmt, ctx);
            AttributeJvmRefKeysInStmt(stmt->value.if_stmt.else_stmt, ctx);
            break;
        case STMT_WHILE:
            AttributeJvmRefKeysInExpr(stmt->value.while_stmt.condition, ctx);
            AttributeJvmRefKeysInStmt(stmt->value.while_stmt.body, ctx);
            break;
        case STMT_DO_WHILE:
            AttributeJvmRefKeysInStmt(stmt->value.do_while_stmt.body, ctx);
            AttributeJvmRefKeysInExpr(stmt->value.do_while_stmt.condition, ctx);
            break;
        case STMT_FOR:
            AttributeJvmRefKeysInExpr(stmt->value.for_stmt.init_expr, ctx);
            if (stmt->value.for_stmt.init_decls) {
                for (int i = 0; i < stmt->value.for_stmt.init_decls->count; i++) {
                    NInitDecl *decl = stmt->value.for_stmt.init_decls->decls[i];
                    if (decl && decl->initializer && decl->initializer->expr) {
                        AttributeJvmRefKeysInExpr(decl->initializer->expr, ctx);
                    }
                }
            }
            AttributeJvmRefKeysInExpr(stmt->value.for_stmt.cond_expr, ctx);
            AttributeJvmRefKeysInExpr(stmt->value.for_stmt.iter_expr, ctx);
            AttributeJvmRefKeysInStmt(stmt->value.for_stmt.body, ctx);
            break;
        case STMT_FOREACH:
            AttributeJvmRefKeysInExpr(stmt->value.foreach_stmt.collection, ctx);
            AttributeJvmRefKeysInStmt(stmt->value.foreach_stmt.body, ctx);
            break;
        case STMT_SWITCH:
            AttributeJvmRefKeysInExpr(stmt->value.switch_stmt.expr, ctx);
            for (int i = 0; i < stmt->value.switch_stmt.cases.count; i++) {
                NCaseItem *item = stmt->value.switch_stmt.cases.items[i];
                if (item && item->case_expr) {
                    AttributeJvmRefKeysInExpr(item->case_expr, ctx);
                }
                if (item && item->stmts) {
                    AttributeJvmRefKeysInStmtList(item->stmts->first, ctx);
                }
            }
            break;
        case STMT_BREAK:
        case STMT_CONTINUE:
            break;
    }
    return 0;
}

static int AttributeJvmRefKeysInExpr(NExpr *expr, SemanticContext *ctx) {
    if (expr == NULL) {
        return 0;
    }
    switch (expr->type) {
        case EXPR_MEMBER_ACCESS:
            FillRefKeyForMemberAccess(expr, ctx);
            AttributeJvmRefKeysInExpr(expr->value.member_access.object, ctx);
            break;
        case EXPR_METHOD_CALL:
            FillRefKeyForMethodCall(expr, ctx);
            AttributeJvmRefKeysInExpr(expr->value.member_access.object, ctx);
            for (int i = 0; i < expr->value.member_access.arg_count; i++) {
                AttributeJvmRefKeysInExpr(expr->value.member_access.args[i], ctx);
            }
            break;
        case EXPR_SUPER_METHOD:
            FillRefKeyForSuperMethod(expr, ctx);
            for (int i = 0; i < expr->value.member_access.arg_count; i++) {
                AttributeJvmRefKeysInExpr(expr->value.member_access.args[i], ctx);
            }
            break;
        case EXPR_SUPER:
            FillRefKeyForSuperField(expr, ctx);
            break;
        case EXPR_FUNC_CALL:
            FillRefKeyForFuncCall(expr, ctx);
            for (int i = 0; i < expr->value.func_call.arg_count; i++) {
                AttributeJvmRefKeysInExpr(expr->value.func_call.args[i], ctx);
            }
            break;
        case EXPR_ARRAY_ACCESS:
            AttributeJvmRefKeysInExpr(expr->value.array_access.array, ctx);
            AttributeJvmRefKeysInExpr(expr->value.array_access.index, ctx);
            AttributeJvmRefKeysInExpr(expr->value.array_access.index_end, ctx);
            break;
        case EXPR_NEW:
            for (int i = 0; i < expr->value.new_expr.init_count; i++) {
                AttributeJvmRefKeysInExpr(expr->value.new_expr.init_exprs[i], ctx);
            }
            break;
        case EXPR_UNARY_OP:
            AttributeJvmRefKeysInExpr(expr->value.unary.operand, ctx);
            break;
        case EXPR_BINARY_OP:
        case EXPR_ASSIGN:
            AttributeJvmRefKeysInExpr(expr->value.binary.left, ctx);
            AttributeJvmRefKeysInExpr(expr->value.binary.right, ctx);
            break;
        case EXPR_PAREN:
            AttributeJvmRefKeysInExpr(expr->value.inner_expr, ctx);
            break;
        case EXPR_CAST:
            AttributeJvmRefKeysInExpr(expr->value.cast.expr, ctx);
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

static int FillRefKeyForMemberAccess(NExpr *expr, SemanticContext *ctx) {
    NType *obj_type;
    ClassInfo *class_info;
    FieldInfo *field;
    ClassInfo *owner;
    char *owner_internal;
    char *desc;

    if (expr == NULL || ctx == NULL) {
        return 0;
    }
    if (expr->jvm_ref_key.has_key) {
        return 1;
    }
    if (expr->value.member_access.object != NULL &&
        expr->value.member_access.object->type == EXPR_IDENT) {
        Symbol *sym = LookupSymbol(ctx, expr->value.member_access.object->value.ident_name);
        if (sym != NULL && sym->kind == SYMBOL_ENUM_TYPE) {
            return 0;
        }
    }
    obj_type = InferExpressionTypeSilent(expr->value.member_access.object, ctx);
    if (obj_type == NULL || obj_type->kind != TYPE_KIND_CLASS) {
        return 0;
    }
    class_info = LookupClass(ctx, obj_type->class_name);
    if (class_info == NULL) {
        return 0;
    }
    owner = FindFieldOwnerInHierarchy(ctx, class_info, expr->value.member_access.member_name);
    if (owner == NULL) {
        return 0;
    }
    field = LookupClassField(owner, expr->value.member_access.member_name);
    if (field == NULL) {
        return 0;
    }
    desc = BuildJvmTypeDescriptor(field->type);
    if (desc == NULL) {
        return 0;
    }
    owner_internal = BuildJvmInternalName(owner->name);
    if (owner_internal == NULL) {
        free(desc);
        return 0;
    }
    if (!SetJvmRefKey(&expr->jvm_ref_key,
                      owner_internal,
                      field->name,
                      desc,
                      JVM_REF_FIELD)) {
        free(owner_internal);
        free(desc);
        return 0;
    }
    free(owner_internal);
    free(desc);
    return 1;
}

static int FillRefKeyForMethodCall(NExpr *expr, SemanticContext *ctx) {
    NType *obj_type;
    ClassInfo *class_info;
    MethodInfo *method;
    ClassInfo *owner;
    char *owner_internal;
    char *desc;

    if (expr == NULL || ctx == NULL) {
        return 0;
    }
    if (expr->jvm_ref_key.has_key) {
        return 1;
    }
    if (expr->resolved_member_descriptor != NULL && expr->resolved_owner_internal != NULL) {
        if (!SetJvmRefKey(&expr->jvm_ref_key,
                          expr->resolved_owner_internal,
                          expr->value.member_access.member_name,
                          expr->resolved_member_descriptor,
                          JVM_REF_METHOD)) {
            return 0;
        }
        return 1;
    }
    obj_type = InferExpressionTypeSilent(expr->value.member_access.object, ctx);
    if (obj_type == NULL || obj_type->kind != TYPE_KIND_CLASS) {
        return 0;
    }
    class_info = LookupClass(ctx, obj_type->class_name);
    if (class_info == NULL) {
        return 0;
    }
    owner = FindMethodOwnerInHierarchy(ctx, class_info, expr->value.member_access.member_name);
    if (owner == NULL) {
        return 0;
    }
    method = LookupClassMethod(owner, expr->value.member_access.member_name);
    if (method == NULL) {
        return 0;
    }
    desc = BuildJvmMethodDescriptor(method->return_type, method->params);
    if (desc == NULL) {
        return 0;
    }
    owner_internal = BuildJvmInternalName(owner->name);
    if (owner_internal == NULL) {
        free(desc);
        return 0;
    }
    if (!SetJvmRefKey(&expr->jvm_ref_key,
                      owner_internal,
                      method->name,
                      desc,
                      JVM_REF_METHOD)) {
        free(owner_internal);
        free(desc);
        return 0;
    }
    free(owner_internal);
    free(desc);
    return 1;
}

static int FillRefKeyForSuperMethod(NExpr *expr, SemanticContext *ctx) {
    ClassInfo *base_info;
    MethodInfo *method;
    char *owner_internal;
    char *desc;

    if (expr == NULL || ctx == NULL || ctx->current_class == NULL) {
        return 0;
    }
    if (expr->jvm_ref_key.has_key) {
        return 1;
    }
    if (expr->resolved_member_descriptor != NULL && expr->resolved_owner_internal != NULL) {
        if (!SetJvmRefKey(&expr->jvm_ref_key,
                          expr->resolved_owner_internal,
                          expr->value.member_access.member_name,
                          expr->resolved_member_descriptor,
                          JVM_REF_METHOD)) {
            return 0;
        }
        return 1;
    }
    if (ctx->current_class->base_class == NULL) {
        return 0;
    }
    base_info = LookupClass(ctx, ctx->current_class->base_class);
    if (base_info == NULL) {
        return 0;
    }
    method = LookupMethodOverloadInHierarchy(ctx,
                                             base_info,
                                             expr->value.member_access.member_name,
                                             expr->value.member_access.args,
                                             expr->value.member_access.arg_count,
                                             NULL,
                                             NULL,
                                             NULL);
    if (method == NULL) {
        return 0;
    }
    desc = BuildJvmMethodDescriptor(method->return_type, method->params);
    if (desc == NULL) {
        return 0;
    }
    owner_internal = BuildJvmInternalName(base_info->name);
    if (owner_internal == NULL) {
        free(desc);
        return 0;
    }
    if (!SetJvmRefKey(&expr->jvm_ref_key,
                      owner_internal,
                      method->name,
                      desc,
                      JVM_REF_METHOD)) {
        free(owner_internal);
        free(desc);
        return 0;
    }
    free(owner_internal);
    free(desc);
    return 1;
}

static int FillRefKeyForSuperField(NExpr *expr, SemanticContext *ctx) {
    ClassInfo *base_info;
    FieldInfo *field;
    char *owner_internal;
    char *desc;

    if (expr == NULL || ctx == NULL || ctx->current_class == NULL) {
        return 0;
    }
    if (expr->jvm_ref_key.has_key) {
        return 1;
    }
    if (ctx->current_class->base_class == NULL) {
        return 0;
    }
    base_info = LookupClass(ctx, ctx->current_class->base_class);
    if (base_info == NULL) {
        return 0;
    }
    field = LookupClassFieldInHierarchy(ctx, base_info, expr->value.ident_name);
    if (field == NULL) {
        return 0;
    }
    desc = BuildJvmTypeDescriptor(field->type);
    if (desc == NULL) {
        return 0;
    }
    owner_internal = BuildJvmInternalName(base_info->name);
    if (owner_internal == NULL) {
        free(desc);
        return 0;
    }
    if (!SetJvmRefKey(&expr->jvm_ref_key,
                      owner_internal,
                      field->name,
                      desc,
                      JVM_REF_FIELD)) {
        free(owner_internal);
        free(desc);
        return 0;
    }
    free(owner_internal);
    free(desc);
    return 1;
}

static int FillRefKeyForFuncCall(NExpr *expr, SemanticContext *ctx) {
    FunctionInfo *func;
    char *owner_internal;
    char *desc;

    if (expr == NULL || ctx == NULL) {
        return 0;
    }
    if (expr->jvm_ref_key.has_key) {
        return 1;
    }
    if (expr->resolved_member_descriptor != NULL) {
        const char *owner_name = expr->resolved_owner_internal != NULL
            ? expr->resolved_owner_internal
            : "Main";
        if (!SetJvmRefKey(&expr->jvm_ref_key,
                          owner_name,
                          expr->value.func_call.func_name,
                          expr->resolved_member_descriptor,
                          JVM_REF_METHOD)) {
            return 0;
        }
        return 1;
    }
    func = LookupFunctionOverload(ctx,
                                  expr->value.func_call.func_name,
                                  expr->value.func_call.args,
                                  expr->value.func_call.arg_count,
                                  NULL);
    if (func == NULL) {
        return 0;
    }
    desc = BuildJvmMethodDescriptor(func->return_type, func->params);
    if (desc == NULL) {
        return 0;
    }
    owner_internal = BuildJvmInternalName("Main");
    if (owner_internal == NULL) {
        free(desc);
        return 0;
    }
    if (!SetJvmRefKey(&expr->jvm_ref_key,
                      owner_internal,
                      func->name,
                      desc,
                      JVM_REF_METHOD)) {
        free(owner_internal);
        free(desc);
        return 0;
    }
    free(owner_internal);
    free(desc);
    return 1;
}
