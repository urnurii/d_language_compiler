#include "codegen_jvm.h"
#include "jvmc/jvmc.h"
#include "semantic/jvm_layout.h"
#include "semantic/jvm_codegen_helpers.h"
#include "semantic/name_resolution.h"
#include "semantic/type_inference.h"
#include "ast_builder.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int CodegenEmitExpr(jvmc_class *cls, jvmc_code *code, NExpr *expr, NParamList *params, NStmt *body,
                           SemanticContext *ctx);
static int CodegenReportExprFail(const char *where, const NExpr *expr);
static void CodegenReportFail(const char *where);
static NStmt *g_current_body = NULL;
static int g_temp_base_override = -1;
typedef struct {
    const char *name;
    int slot;
    NType *type;
} CodegenVar;
static CodegenVar *g_var_stack = NULL;
static int g_var_count = 0;
static int g_var_cap = 0;
static int *g_scope_stack = NULL;
static int g_scope_count = 0;
static int g_scope_cap = 0;

static char *BuildJvmArrayDescriptorForType(const NType *elem_type) {
    char *elem_desc;
    char *desc;
    size_t len;

    if (elem_type == NULL) {
        return NULL;
    }
    elem_desc = BuildJvmTypeDescriptor(elem_type);
    if (elem_desc == NULL) {
        return NULL;
    }
    len = strlen(elem_desc);
    desc = (char *)malloc(len + 2);
    if (desc == NULL) {
        free(elem_desc);
        return NULL;
    }
    desc[0] = '[';
    memcpy(desc + 1, elem_desc, len + 1);
    free(elem_desc);
    return desc;
}

typedef struct {
    jvmc_label **break_labels;
    int break_count;
    int break_capacity;
    jvmc_label **continue_labels;
    int continue_count;
    int continue_capacity;
} FlowContext;

static int CodegenEmitStmtList(jvmc_class *cls, jvmc_code *code, NStmt *stmts, NParamList *params,
                               int *return_emitted, SemanticContext *ctx, FlowContext *flow);
static int CodegenAddMainWithCleanup(jvmc_class *cls, NFuncDef *func, SemanticContext *ctx);
static int EmitImplicitThisFieldLoad(jvmc_class *cls, jvmc_code *code, SemanticContext *ctx,
                                     const char *name, NType **type_out);
static int EmitImplicitThisFieldStore(jvmc_class *cls, jvmc_code *code, SemanticContext *ctx,
                                      const char *name, const NType *value_type,
                                      NParamList *params, NStmt *body);

static int CodegenEmitCtor(jvmc_class *cls, NClassDef *class_def, NCtorDef *ctor,
                           AccessSpec access, SemanticContext *ctx);
static int CodegenEmitDefaultCtor(jvmc_class *cls, NClassDef *class_def, SemanticContext *ctx);
static int CodegenAddEnumFields(jvmc_class *cls, NEnumDef *enum_def, uint16_t access_flags);
static int CodegenEmitEnumValues(jvmc_class *cls, jvmc_code *code,
                                 const char *owner_internal, NEnumDef *enum_def);

static char *CodegenBuildTypeDescriptor(const NType *type) {
    return BuildJvmTypeDescriptor(type);
}

static char *CodegenBuildMethodDescriptor(const NType *return_type, const NParamList *params) {
    return BuildJvmMethodDescriptor(return_type, params);
}

static int CodegenAddFieldDecls(jvmc_class *cls, NInitDeclList *decls, uint16_t access_flag) {
    if (cls == NULL || decls == NULL) {
        return 1;
    }
    for (int i = 0; i < decls->count; i++) {
        NInitDecl *decl = decls->decls[i];
        jvmc_field *field = NULL;
        if (decl == NULL || decl->name == NULL || decl->jvm_descriptor == NULL) {
            continue;
        }
        field = jvmc_class_get_or_create_field(cls, decl->name, decl->jvm_descriptor);
        if (field == NULL) {
            return 0;
        }
        if (!jvmc_field_add_flag(field, access_flag)) {
            return 0;
        }
    }
    return 1;
}

static void UpdateMaxLocalsFromDecls(const NType *decl_type, NInitDeclList *decls, int *max_out) {
    int width;
    if (decl_type == NULL || decls == NULL || max_out == NULL) {
        return;
    }
    width = GetJvmSlotWidthForType(decl_type);
    for (int i = 0; i < decls->count; i++) {
        NInitDecl *decl = decls->decls[i];
        if (decl == NULL || decl->jvm_slot_index < 0) {
            continue;
        }
        if (decl->jvm_slot_index + width > *max_out) {
            *max_out = decl->jvm_slot_index + width;
        }
    }
}

static void UpdateMaxLocalsFromStmt(NStmt *stmt, int *max_out) {
    if (stmt == NULL || max_out == NULL) {
        return;
    }
    switch (stmt->type) {
        case STMT_DECL:
            UpdateMaxLocalsFromDecls(stmt->value.decl.decl_type,
                                     stmt->value.decl.init_decls,
                                     max_out);
            break;
        case STMT_COMPOUND: {
            NStmt *cur = stmt->value.stmt_list ? stmt->value.stmt_list->first : NULL;
            while (cur != NULL) {
                UpdateMaxLocalsFromStmt(cur, max_out);
                cur = cur->next;
            }
            break;
        }
        case STMT_IF:
            UpdateMaxLocalsFromStmt(stmt->value.if_stmt.then_stmt, max_out);
            UpdateMaxLocalsFromStmt(stmt->value.if_stmt.else_stmt, max_out);
            break;
        case STMT_WHILE:
            UpdateMaxLocalsFromStmt(stmt->value.while_stmt.body, max_out);
            break;
        case STMT_DO_WHILE:
            UpdateMaxLocalsFromStmt(stmt->value.do_while_stmt.body, max_out);
            break;
        case STMT_FOR:
            UpdateMaxLocalsFromDecls(stmt->value.for_stmt.init_decl_type,
                                     stmt->value.for_stmt.init_decls,
                                     max_out);
            UpdateMaxLocalsFromStmt(stmt->value.for_stmt.body, max_out);
            break;
        case STMT_FOREACH:
            UpdateMaxLocalsFromStmt(stmt->value.foreach_stmt.body, max_out);
            break;
        case STMT_SWITCH:
            for (int i = 0; i < stmt->value.switch_stmt.cases.count; i++) {
                NCaseItem *item = stmt->value.switch_stmt.cases.items[i];
                if (item && item->stmts) {
                    UpdateMaxLocalsFromStmt(item->stmts->first, max_out);
                }
            }
            break;
        case STMT_EXPR:
        case STMT_RETURN:
        case STMT_BREAK:
        case STMT_CONTINUE:
            break;
    }
}

static int MaxTempSlotsInExpr(NExpr *expr);
static int MaxTempSlotsInStmt(NStmt *stmt);
static int MaxTempSlotsInStmtList(NStmt *stmts);
static int FindMaxSlot(NParamList *params, NStmt *body);
static int GetTempBase(NParamList *params, NStmt *body);
static int EmitPopForType(jvmc_code *code, const NType *type);
static void ResetCodegenScope(void);
static int PushCodegenScope(void);
static void PopCodegenScope(void);
static int AddCodegenVar(const char *name, int slot, NType *type);
static int LookupCodegenVar(const char *name, int *slot_out, NType **type_out);
static void AddParamsToScope(NParamList *params);

static int ComputeMaxLocals(NParamList *params, NStmt *body, int include_this) {
    int max_locals = include_this ? 1 : 0;
    int max_temp = 0;
    int base_slot = 0;
    if (params != NULL) {
        for (int i = 0; i < params->count; i++) {
            NParam *param = params->params[i];
            int width = 1;
            if (param == NULL || param->jvm_slot_index < 0) {
                continue;
            }
            if (param->is_ref) {
                width = 1;
            } else {
                width = GetJvmSlotWidthForType(param->param_type);
            }
            if (param->jvm_slot_index + width > max_locals) {
                max_locals = param->jvm_slot_index + width;
            }
        }
    }
    if (body != NULL) {
        UpdateMaxLocalsFromStmt(body, &max_locals);
        max_temp = MaxTempSlotsInStmt(body);
    }
    base_slot = FindMaxSlot(params, body) + 1;
    if (include_this && base_slot < 1) {
        base_slot = 1;
    }
    if (base_slot + max_temp > max_locals) {
        max_locals = base_slot + max_temp;
    }
    return (max_locals > 0) ? max_locals : 1;
}

static int CodegenEmitEmptyReturn(jvmc_code *code, const char *descriptor) {
    const char *ret = NULL;
    if (code == NULL || descriptor == NULL) {
        return 0;
    }
    ret = strchr(descriptor, ')');
    if (ret == NULL || ret[1] == '\0') {
        return jvmc_code_return_void(code);
    }
    ret = ret + 1;
    switch (*ret) {
        case 'V':
            return jvmc_code_return_void(code);
        case 'J':
            return jvmc_code_return_long(code);
        case 'D':
            return jvmc_code_return_double(code);
        case 'F':
            return jvmc_code_return_float(code);
        case 'Z':
        case 'B':
        case 'C':
        case 'S':
        case 'I':
            return jvmc_code_return_int(code);
        default:
            return jvmc_code_return_ref(code);
    }
}

static uint16_t CodegenAccessFromSpec(AccessSpec access) {
    switch (access) {
        case ACCESS_PRIVATE:
            // D-like private is module/file-level; for single-file compile
            // map to package-private on JVM (no flag).
            return 0;
        case ACCESS_PROTECTED:
            return JVMC_METHOD_ACC_PROTECTED;
        case ACCESS_PUBLIC:
        default:
            return JVMC_METHOD_ACC_PUBLIC;
    }
}

static int CodegenAddMethodStub(jvmc_class *cls, const char *name, const char *descriptor,
                                uint16_t access_flags, int is_static) {
    jvmc_method *method = NULL;
    jvmc_code *code = NULL;
    if (cls == NULL || name == NULL || descriptor == NULL) {
        return 0;
    }
    method = jvmc_class_get_or_create_method(cls, name, descriptor);
    if (method == NULL) {
        return 0;
    }
    if (!jvmc_method_add_flag(method, access_flags)) {
        return 0;
    }
    if (is_static) {
        if (!jvmc_method_add_flag(method, JVMC_METHOD_ACC_STATIC)) {
            return 0;
        }
    }
    code = jvmc_method_get_code(method);
    if (code == NULL) {
        return 0;
    }
    if (!jvmc_code_set_max_stack(code, 32)) {
        return 0;
    }
    if (!jvmc_code_set_max_locals(code, (uint16_t)ComputeMaxLocals(NULL, NULL, is_static ? 0 : 1))) {
        return 0;
    }
    return CodegenEmitEmptyReturn(code, descriptor);
}

static int CodegenAddMethodAbstract(jvmc_class *cls, const char *name, const char *descriptor,
                                    uint16_t access_flags, int is_static) {
    jvmc_method *method = NULL;
    if (cls == NULL || name == NULL || descriptor == NULL) {
        return 0;
    }
    method = jvmc_class_get_or_create_method(cls, name, descriptor);
    if (method == NULL) {
        return 0;
    }
    if (!jvmc_method_add_flag(method, access_flags)) {
        return 0;
    }
    if (is_static) {
        if (!jvmc_method_add_flag(method, JVMC_METHOD_ACC_STATIC)) {
            return 0;
        }
    }
    return jvmc_method_add_flag(method, JVMC_METHOD_ACC_ABSTRACT);
}

static int CodegenAddMethodNative(jvmc_class *cls, const char *name, const char *descriptor,
                                  uint16_t access_flags, int is_static) {
    jvmc_method *method = NULL;
    if (cls == NULL || name == NULL || descriptor == NULL) {
        return 0;
    }
    method = jvmc_class_get_or_create_method(cls, name, descriptor);
    if (method == NULL) {
        return 0;
    }
    if (!jvmc_method_add_flag(method, access_flags)) {
        return 0;
    }
    if (is_static) {
        if (!jvmc_method_add_flag(method, JVMC_METHOD_ACC_STATIC)) {
            return 0;
        }
    }
    return jvmc_method_add_flag(method, JVMC_METHOD_ACC_NATIVE);
}

static int ResolveVariableSlotInStmt(NStmt *stmt, const char *name, int *slot_out, NType **type_out) {
    if (stmt == NULL || name == NULL) {
        return 0;
    }
    switch (stmt->type) {
        case STMT_DECL:
            if (stmt->value.decl.init_decls != NULL && stmt->value.decl.decl_type != NULL) {
                for (int i = 0; i < stmt->value.decl.init_decls->count; i++) {
                    NInitDecl *decl = stmt->value.decl.init_decls->decls[i];
                    if (decl && decl->name && strcmp(decl->name, name) == 0) {
                        if (slot_out) *slot_out = decl->jvm_slot_index;
                        if (type_out) *type_out = stmt->value.decl.decl_type;
                        return 1;
                    }
                }
            }
            break;
        case STMT_COMPOUND: {
            NStmt *cur = stmt->value.stmt_list ? stmt->value.stmt_list->first : NULL;
            while (cur != NULL) {
                if (ResolveVariableSlotInStmt(cur, name, slot_out, type_out)) {
                    return 1;
                }
                cur = cur->next;
            }
            break;
        }
        case STMT_IF:
            if (ResolveVariableSlotInStmt(stmt->value.if_stmt.then_stmt, name, slot_out, type_out)) {
                return 1;
            }
            if (ResolveVariableSlotInStmt(stmt->value.if_stmt.else_stmt, name, slot_out, type_out)) {
                return 1;
            }
            break;
        case STMT_WHILE:
            return ResolveVariableSlotInStmt(stmt->value.while_stmt.body, name, slot_out, type_out);
        case STMT_DO_WHILE:
            return ResolveVariableSlotInStmt(stmt->value.do_while_stmt.body, name, slot_out, type_out);
        case STMT_FOR:
            if (stmt->value.for_stmt.init_decls != NULL) {
                for (int i = 0; i < stmt->value.for_stmt.init_decls->count; i++) {
                    NInitDecl *decl = stmt->value.for_stmt.init_decls->decls[i];
                    if (decl && decl->name && strcmp(decl->name, name) == 0) {
                        if (slot_out) *slot_out = decl->jvm_slot_index;
                        if (type_out) {
                            if (stmt->value.for_stmt.init_decl_type != NULL) {
                                *type_out = stmt->value.for_stmt.init_decl_type;
                            } else if (decl->initializer && decl->initializer->expr) {
                                *type_out = decl->initializer->expr->inferred_type;
                            }
                        }
                        return 1;
                    }
                }
            }
            return ResolveVariableSlotInStmt(stmt->value.for_stmt.body, name, slot_out, type_out);
        case STMT_SWITCH:
            for (int i = 0; i < stmt->value.switch_stmt.cases.count; i++) {
                NCaseItem *item = stmt->value.switch_stmt.cases.items[i];
                if (item && item->stmts) {
                    NStmt *cur = item->stmts->first;
                    while (cur != NULL) {
                        if (ResolveVariableSlotInStmt(cur, name, slot_out, type_out)) {
                            return 1;
                        }
                        cur = cur->next;
                    }
                }
            }
            break;
        case STMT_FOREACH:
        case STMT_EXPR:
        case STMT_RETURN:
        case STMT_BREAK:
        case STMT_CONTINUE:
            break;
    }
    return 0;
}

static int ResolveVariableSlot(const char *name, NParamList *params, NStmt *body, int *slot_out, NType **type_out) {
    if (name == NULL) {
        return 0;
    }
    if (LookupCodegenVar(name, slot_out, type_out)) {
        return 1;
    }
    if (params != NULL) {
        for (int i = 0; i < params->count; i++) {
            NParam *param = params->params[i];
            if (param && param->param_name && strcmp(param->param_name, name) == 0) {
                if (slot_out) *slot_out = param->jvm_slot_index;
                if (type_out) *type_out = param->param_type;
                return 1;
            }
        }
    }
    if (body != NULL) {
        NStmt *cur = body;
        while (cur != NULL) {
            if (ResolveVariableSlotInStmt(cur, name, slot_out, type_out)) {
                return 1;
            }
            cur = cur->next;
        }
    }
    if (g_current_body != NULL && g_current_body != body) {
        NStmt *cur = g_current_body;
        while (cur != NULL) {
            if (ResolveVariableSlotInStmt(cur, name, slot_out, type_out)) {
                return 1;
            }
            cur = cur->next;
        }
    }
    return 0;
}

static int EmitLoadByType(jvmc_code *code, const NType *type, int slot) {
    return JvmEmitLoadByType(code, type, slot);
}

static int EmitStoreByType(jvmc_code *code, const NType *type, int slot) {
    return JvmEmitStoreByType(code, type, slot);
}

static int EmitReturnByType(jvmc_code *code, const NType *type) {
    return JvmEmitReturnByType(code, type);
}

static int EmitLiteralExpr(jvmc_code *code, NExpr *expr, const NType *type) {
    if (expr == NULL || code == NULL) {
        return 0;
    }
    switch (expr->type) {
        case EXPR_INT:
            return jvmc_code_push_int(code, expr->value.int_value);
        case EXPR_CHAR:
            return jvmc_code_push_int(code, (int)expr->value.char_value);
        case EXPR_BOOL:
            return jvmc_code_push_int(code, expr->value.int_value ? 1 : 0);
        case EXPR_FLOAT:
            if (type != NULL && (type->kind == TYPE_KIND_BASE) &&
                (type->base_type == TYPE_DOUBLE || type->base_type == TYPE_REAL)) {
                return jvmc_code_push_double(code, expr->value.float_value);
            }
            return jvmc_code_push_float(code, (float)expr->value.float_value);
        case EXPR_STRING:
            return jvmc_code_push_string(code, expr->value.string_value ? expr->value.string_value : "");
        case EXPR_NULL:
            return jvmc_code_push_null(code);
        case EXPR_NAN:
            if (type != NULL && (type->kind == TYPE_KIND_BASE) &&
                (type->base_type == TYPE_DOUBLE || type->base_type == TYPE_REAL)) {
                return jvmc_code_push_double(code, expr->value.float_value);
            }
            return jvmc_code_push_float(code, (float)expr->value.float_value);
        default:
            return 0;
    }
}

static int EmitInvokeFromRefKey(jvmc_class *cls, jvmc_code *code, const JvmRefKey *key, int is_static_call) {
    jvmc_methodref *mref = NULL;
    if (cls == NULL || code == NULL || key == NULL || !key->has_key || key->kind != JVM_REF_METHOD) {
        return 0;
    }
    if (key->owner_internal_name == NULL || key->member_name == NULL || key->member_descriptor == NULL) {
        return 0;
    }
    mref = jvmc_class_get_or_create_methodref(cls,
                                              key->owner_internal_name,
                                              key->member_name,
                                              key->member_descriptor);
    if (mref == NULL) {
        return 0;
    }
    if (is_static_call) {
        return jvmc_code_invokestatic(code, mref);
    }
    return jvmc_code_invokevirtual(code, mref);
}

static const char *PrintDescriptorForType(const NType *type) {
    return JvmPrintDescriptorForType(type);
}

static int EmitPrintCall(jvmc_class *cls, jvmc_code *code, NExpr *arg, int is_last, int is_writeln,
                         NParamList *params, NStmt *body, SemanticContext *ctx) {
    jvmc_fieldref *out_ref = NULL;
    jvmc_methodref *print_ref = NULL;
    const char *owner = "java/io/PrintStream";
    const char *method = "print";
    const char *desc_part = NULL;
    char desc_buf[32];

    if (cls == NULL || code == NULL) {
        return 0;
    }

    out_ref = jvmc_class_get_or_create_fieldref(cls,
                                                "java/lang/System",
                                                "out",
                                                "Ljava/io/PrintStream;");
    if (out_ref == NULL) {
        return 0;
    }
    if (!jvmc_code_getstatic(code, out_ref)) {
        return 0;
    }

    if (arg != NULL) {
        if (!CodegenEmitExpr(cls, code, arg, params, body, ctx)) {
            return 0;
        }
        if (is_writeln && is_last) {
            method = "println";
        }
        desc_part = PrintDescriptorForType(arg->inferred_type);
        snprintf(desc_buf, sizeof(desc_buf), "(%s)V", desc_part);
        print_ref = jvmc_class_get_or_create_methodref(cls, owner, method, desc_buf);
        if (print_ref == NULL) {
            return 0;
        }
        return jvmc_code_invokevirtual(code, print_ref);
    }

    if (is_writeln) {
        print_ref = jvmc_class_get_or_create_methodref(cls, owner, "println", "()V");
        if (print_ref == NULL) {
            return 0;
        }
        return jvmc_code_invokevirtual(code, print_ref);
    }
    return 1;
}

static int EmitReadlnCall(jvmc_class *cls, jvmc_code *code) {
    jvmc_methodref *mref = NULL;
    if (cls == NULL || code == NULL) {
        return 0;
    }
    mref = jvmc_class_get_or_create_methodref(cls, "dlang/Runtime", "readln", "()Ljava/lang/String;");
    if (mref == NULL) {
        return 0;
    }
    return jvmc_code_invokestatic(code, mref);
}

static int EmitNumericCast(jvmc_code *code, const NType *from, const NType *to) {
    return JvmEmitNumericCast(code, from, to);
}

static int IsFloatLikeType(const NType *type, int *is_double_out) {
    if (is_double_out) {
        *is_double_out = 0;
    }
    if (type == NULL || type->kind != TYPE_KIND_BASE) {
        return 0;
    }
    switch (type->base_type) {
        case TYPE_FLOAT:
            if (is_double_out) *is_double_out = 0;
            return 1;
        case TYPE_DOUBLE:
        case TYPE_REAL:
            if (is_double_out) *is_double_out = 1;
            return 1;
        default:
            return 0;
    }
}

static int EmitNumericBinaryOp(jvmc_code *code, OpType op, const NType *type) {
    int is_double = 0;
    if (code == NULL) {
        return 0;
    }
    if (IsFloatLikeType(type, &is_double)) {
        if (is_double) {
            switch (op) {
                case OP_PLUS:  return jvmc_code_add_double(code);
                case OP_MINUS: return jvmc_code_sub_double(code);
                case OP_MUL:   return jvmc_code_mul_double(code);
                case OP_DIV:   return jvmc_code_div_double(code);
                default:       return 0;
            }
        }
        switch (op) {
            case OP_PLUS:  return jvmc_code_add_float(code);
            case OP_MINUS: return jvmc_code_sub_float(code);
            case OP_MUL:   return jvmc_code_mul_float(code);
            case OP_DIV:   return jvmc_code_div_float(code);
            default:       return 0;
        }
    }
    switch (op) {
        case OP_PLUS:  return jvmc_code_add_int(code);
        case OP_MINUS: return jvmc_code_sub_int(code);
        case OP_MUL:   return jvmc_code_mul_int(code);
        case OP_DIV:   return jvmc_code_div_int(code);
        default:       return 0;
    }
}

static int EmitNumericNeg(jvmc_code *code, const NType *type) {
    int is_double = 0;
    if (code == NULL) {
        return 0;
    }
    if (IsFloatLikeType(type, &is_double)) {
        return is_double ? jvmc_code_neg_double(code) : jvmc_code_neg_float(code);
    }
    if (!jvmc_code_push_int(code, -1)) {
        return 0;
    }
    return jvmc_code_mul_int(code);
}

static int EmitCompareZeroResult(jvmc_code *code, jvmc_compare cmp) {
    jvmc_label *label_true;
    jvmc_label *label_end;
    if (code == NULL) {
        return 0;
    }
    label_true = jvmc_code_label_create(code);
    label_end = jvmc_code_label_create(code);
    if (label_true == NULL || label_end == NULL) {
        return 0;
    }
    if (!jvmc_code_if(code, cmp, label_true)) {
        return 0;
    }
    if (!jvmc_code_push_int(code, 0)) {
        return 0;
    }
    if (!jvmc_code_goto(code, label_end)) {
        return 0;
    }
    if (!jvmc_code_label_place(code, label_true)) {
        return 0;
    }
    if (!jvmc_code_push_int(code, 1)) {
        return 0;
    }
    return jvmc_code_label_place(code, label_end);
}

static int EmitBoxIfNeeded(jvmc_class *cls, jvmc_code *code, const NType *type) {
    jvmc_methodref *mref = NULL;
    if (code == NULL || cls == NULL || type == NULL) {
        return 1;
    }
    if (type->kind != TYPE_KIND_BASE) {
        return 1;
    }
    switch (type->base_type) {
        case TYPE_BOOL:
            mref = jvmc_class_get_or_create_methodref(cls,
                                                      "java/lang/Boolean",
                                                      "valueOf",
                                                      "(Z)Ljava/lang/Boolean;");
            break;
        case TYPE_CHAR:
        case TYPE_INT:
            mref = jvmc_class_get_or_create_methodref(cls,
                                                      "java/lang/Integer",
                                                      "valueOf",
                                                      "(I)Ljava/lang/Integer;");
            break;
        case TYPE_FLOAT:
            mref = jvmc_class_get_or_create_methodref(cls,
                                                      "java/lang/Float",
                                                      "valueOf",
                                                      "(F)Ljava/lang/Float;");
            break;
        case TYPE_DOUBLE:
        case TYPE_REAL:
            mref = jvmc_class_get_or_create_methodref(cls,
                                                      "java/lang/Double",
                                                      "valueOf",
                                                      "(D)Ljava/lang/Double;");
            break;
        case TYPE_STRING:
        case TYPE_VOID:
        case TYPE_CLASS:
        default:
            return 1;
    }
    if (mref == NULL) {
        return 0;
    }
    return jvmc_code_invokestatic(code, mref);
}

static int EmitBuildObjectArray(jvmc_class *cls, jvmc_code *code, NExpr **args, int count,
                                NParamList *params, NStmt *body, SemanticContext *ctx) {
    jvmc_class_ref *cref = NULL;
    if (cls == NULL || code == NULL) {
        return 0;
    }
    if (count < 0) {
        count = 0;
    }
    if (!jvmc_code_push_int(code, count)) {
        return 0;
    }
    cref = jvmc_class_get_or_create_class_ref(cls, "java/lang/Object");
    if (cref == NULL) {
        return 0;
    }
    if (!jvmc_code_newarray_ref(code, cref)) {
        return 0;
    }
    for (int i = 0; i < count; i++) {
        NExpr *arg = args ? args[i] : NULL;
        if (!jvmc_code_dup(code)) {
            return 0;
        }
        if (!jvmc_code_push_int(code, i)) {
            return 0;
        }
        if (!CodegenEmitExpr(cls, code, arg, params, body, ctx)) {
            return 0;
        }
        if (arg != NULL && !EmitBoxIfNeeded(cls, code, arg->inferred_type)) {
            return 0;
        }
        if (!jvmc_code_array_store_ref(code)) {
            return 0;
        }
    }
    return 1;
}

static int EmitGlobalLoad(jvmc_class *cls, jvmc_code *code, SemanticContext *ctx, const char *name, NType **type_out) {
    Symbol *sym;
    char *desc;
    jvmc_fieldref *fref;
    if (cls == NULL || code == NULL || ctx == NULL || name == NULL) {
        return 0;
    }
    sym = LookupGlobalSymbol(ctx, name);
    if (sym == NULL || sym->kind != SYMBOL_VARIABLE || sym->info.var_info == NULL) {
        return 0;
    }
    if (type_out != NULL) {
        *type_out = sym->info.var_info->type;
    }
    desc = BuildJvmTypeDescriptor(sym->info.var_info->type);
    if (desc == NULL) {
        return 0;
    }
    fref = jvmc_class_get_or_create_fieldref(cls, "Main", name, desc);
    free(desc);
    if (fref == NULL) {
        return 0;
    }
    return jvmc_code_getstatic(code, fref);
}

static int EmitGlobalStore(jvmc_class *cls, jvmc_code *code, SemanticContext *ctx, const char *name, NType **type_out) {
    Symbol *sym;
    char *desc;
    jvmc_fieldref *fref;
    if (cls == NULL || code == NULL || ctx == NULL || name == NULL) {
        return 0;
    }
    sym = LookupGlobalSymbol(ctx, name);
    if (sym == NULL || sym->kind != SYMBOL_VARIABLE || sym->info.var_info == NULL) {
        return 0;
    }
    if (type_out != NULL) {
        *type_out = sym->info.var_info->type;
    }
    desc = BuildJvmTypeDescriptor(sym->info.var_info->type);
    if (desc == NULL) {
        return 0;
    }
    fref = jvmc_class_get_or_create_fieldref(cls, "Main", name, desc);
    free(desc);
    if (fref == NULL) {
        return 0;
    }
    return jvmc_code_putstatic(code, fref);
}

static int FlowPushLabel(jvmc_label ***labels, int *count, int *capacity, jvmc_label *label) {
    jvmc_label **grown;
    if (labels == NULL || count == NULL || capacity == NULL) {
        return 0;
    }
    if (*count >= *capacity) {
        int new_cap = (*capacity == 0) ? 4 : (*capacity * 2);
        grown = (jvmc_label **)realloc(*labels, sizeof(jvmc_label *) * (size_t)new_cap);
        if (grown == NULL) {
            return 0;
        }
        *labels = grown;
        *capacity = new_cap;
    }
    (*labels)[*count] = label;
    *count += 1;
    return 1;
}

static void FlowPopLabel(int *count) {
    if (count == NULL || *count <= 0) {
        return;
    }
    *count -= 1;
}

static int FlowPushLoop(FlowContext *flow, jvmc_label *break_label, jvmc_label *continue_label) {
    if (flow == NULL) {
        return 0;
    }
    if (!FlowPushLabel(&flow->break_labels, &flow->break_count, &flow->break_capacity, break_label)) {
        return 0;
    }
    if (!FlowPushLabel(&flow->continue_labels, &flow->continue_count, &flow->continue_capacity, continue_label)) {
        FlowPopLabel(&flow->break_count);
        return 0;
    }
    return 1;
}

static int FlowPushSwitch(FlowContext *flow, jvmc_label *break_label) {
    if (flow == NULL) {
        return 0;
    }
    return FlowPushLabel(&flow->break_labels, &flow->break_count, &flow->break_capacity, break_label);
}

static void FlowPopLoop(FlowContext *flow) {
    if (flow == NULL) {
        return;
    }
    FlowPopLabel(&flow->break_count);
    FlowPopLabel(&flow->continue_count);
}

static void FlowPopSwitch(FlowContext *flow) {
    if (flow == NULL) {
        return;
    }
    FlowPopLabel(&flow->break_count);
}

static int HasClassDestructor(SemanticContext *ctx, const char *class_name) {
    ClassInfo *info;
    if (ctx == NULL || class_name == NULL) {
        return 0;
    }
    info = LookupClass(ctx, class_name);
    if (info == NULL) {
        return 0;
    }
    return info->destructor != NULL;
}

static jvmc_label *FlowTopBreak(const FlowContext *flow) {
    if (flow == NULL || flow->break_count <= 0) {
        return NULL;
    }
    return flow->break_labels[flow->break_count - 1];
}

static jvmc_label *FlowTopContinue(const FlowContext *flow) {
    if (flow == NULL || flow->continue_count <= 0) {
        return NULL;
    }
    return flow->continue_labels[flow->continue_count - 1];
}

static int IsRefType(const NType *type) {
    if (type == NULL) {
        return 0;
    }
    if (type->kind == TYPE_KIND_CLASS || type->kind == TYPE_KIND_CLASS_ARRAY) {
        return 1;
    }
    if (type->kind == TYPE_KIND_BASE_ARRAY) {
        return 1;
    }
    if (type->kind == TYPE_KIND_BASE && type->base_type == TYPE_STRING) {
        return 1;
    }
    return 0;
}

static int IsStringType(const NType *type) {
    if (type == NULL) {
        return 0;
    }
    return type->kind == TYPE_KIND_BASE && type->base_type == TYPE_STRING;
}

typedef struct {
    const NType *type;
    int slot;
} DtorVar;

static int CollectDtorVarsFromStmt(NStmt *stmt, DtorVar **vars, int *count, int *cap, SemanticContext *ctx) {
    if (stmt == NULL || vars == NULL || count == NULL || cap == NULL) {
        return 0;
    }
    switch (stmt->type) {
        case STMT_DECL:
            if (stmt->value.decl.decl_type &&
                stmt->value.decl.decl_type->kind == TYPE_KIND_CLASS &&
                stmt->value.decl.decl_type->class_name &&
                HasClassDestructor(ctx, stmt->value.decl.decl_type->class_name) &&
                stmt->value.decl.init_decls) {
                for (int i = 0; i < stmt->value.decl.init_decls->count; i++) {
                    NInitDecl *decl = stmt->value.decl.init_decls->decls[i];
                    if (decl == NULL || decl->jvm_slot_index < 0) {
                        continue;
                    }
                    if (*count >= *cap) {
                        int new_cap = (*cap == 0) ? 4 : (*cap * 2);
                        DtorVar *grown = (DtorVar *)realloc(*vars, sizeof(DtorVar) * (size_t)new_cap);
                        if (grown == NULL) {
                            return 0;
                        }
                        *vars = grown;
                        *cap = new_cap;
                    }
                    (*vars)[*count].type = stmt->value.decl.decl_type;
                    (*vars)[*count].slot = decl->jvm_slot_index;
                    *count += 1;
                }
            }
            break;
        case STMT_COMPOUND: {
            NStmt *cur = stmt->value.stmt_list ? stmt->value.stmt_list->first : NULL;
            while (cur != NULL) {
                if (!CollectDtorVarsFromStmt(cur, vars, count, cap, ctx)) {
                    return 0;
                }
                cur = cur->next;
            }
            break;
        }
        case STMT_IF:
            if (!CollectDtorVarsFromStmt(stmt->value.if_stmt.then_stmt, vars, count, cap, ctx)) {
                return 0;
            }
            if (!CollectDtorVarsFromStmt(stmt->value.if_stmt.else_stmt, vars, count, cap, ctx)) {
                return 0;
            }
            break;
        case STMT_WHILE:
            if (!CollectDtorVarsFromStmt(stmt->value.while_stmt.body, vars, count, cap, ctx)) {
                return 0;
            }
            break;
        case STMT_DO_WHILE:
            if (!CollectDtorVarsFromStmt(stmt->value.do_while_stmt.body, vars, count, cap, ctx)) {
                return 0;
            }
            break;
        case STMT_FOR:
            if (stmt->value.for_stmt.body &&
                !CollectDtorVarsFromStmt(stmt->value.for_stmt.body, vars, count, cap, ctx)) {
                return 0;
            }
            break;
        case STMT_FOREACH:
            if (!CollectDtorVarsFromStmt(stmt->value.foreach_stmt.body, vars, count, cap, ctx)) {
                return 0;
            }
            break;
        case STMT_SWITCH:
            for (int i = 0; i < stmt->value.switch_stmt.cases.count; i++) {
                NCaseItem *item = stmt->value.switch_stmt.cases.items[i];
                if (item && item->stmts) {
                    if (!CollectDtorVarsFromStmt(item->stmts->first, vars, count, cap, ctx)) {
                        return 0;
                    }
                }
            }
            break;
        case STMT_EXPR:
        case STMT_RETURN:
        case STMT_BREAK:
        case STMT_CONTINUE:
            break;
    }
    return 1;
}

static int EmitMainDtorCleanup(jvmc_class *cls, jvmc_code *code, NStmt *body,
                               SemanticContext *ctx) {
    DtorVar *vars = NULL;
    int count = 0;
    int cap = 0;
    if (cls == NULL || code == NULL) {
        return 0;
    }
    if (body == NULL) {
        return 1;
    }
    if (!CollectDtorVarsFromStmt(body, &vars, &count, &cap, ctx)) {
        free(vars);
        return 0;
    }
    for (int i = count - 1; i >= 0; i--) {
        const NType *type = vars[i].type;
        jvmc_methodref *mref;
        jvmc_label *skip;
        char *owner_internal;
        if (type == NULL || type->class_name == NULL) {
            continue;
        }
        owner_internal = BuildJvmInternalName(type->class_name);
        if (owner_internal == NULL) {
            free(vars);
            return 0;
        }
        mref = jvmc_class_get_or_create_methodref(cls, owner_internal, "__dtor", "()V");
        free(owner_internal);
        if (mref == NULL) {
            free(vars);
            return 0;
        }
        skip = jvmc_code_label_create(code);
        if (skip == NULL) {
            free(vars);
            return 0;
        }
        if (!jvmc_code_load_ref(code, (uint16_t)vars[i].slot)) {
            free(vars);
            return 0;
        }
        if (!jvmc_code_if_null(code, skip)) {
            free(vars);
            return 0;
        }
        if (!jvmc_code_load_ref(code, (uint16_t)vars[i].slot)) {
            free(vars);
            return 0;
        }
        if (!jvmc_code_invokevirtual(code, mref)) {
            free(vars);
            return 0;
        }
        if (!jvmc_code_label_place(code, skip)) {
            free(vars);
            return 0;
        }
    }
    free(vars);
    return 1;
}

static jvmc_compare InvertCompare(jvmc_compare cmp) {
    switch (cmp) {
        case JVMC_CMP_EQ: return JVMC_CMP_NE;
        case JVMC_CMP_NE: return JVMC_CMP_EQ;
        case JVMC_CMP_LT: return JVMC_CMP_GE;
        case JVMC_CMP_LE: return JVMC_CMP_GT;
        case JVMC_CMP_GT: return JVMC_CMP_LE;
        case JVMC_CMP_GE: return JVMC_CMP_LT;
        default: return JVMC_CMP_EQ;
    }
}

static int EmitStringEqualsResult(jvmc_class *cls, jvmc_code *code, NExpr *left, NExpr *right,
                                  NParamList *params, NStmt *body, SemanticContext *ctx) {
    int temp_base;
    int left_slot;
    int right_slot;
    jvmc_label *label_left_null;
    jvmc_label *label_right_null;
    jvmc_label *label_end;
    jvmc_methodref *mref;

    if (cls == NULL || code == NULL || left == NULL || right == NULL) {
        return 0;
    }

    temp_base = FindMaxSlot(params, body) + 1;
    left_slot = temp_base;
    right_slot = temp_base + 1;

    if (!CodegenEmitExpr(cls, code, left, params, body, ctx)) {
        return 0;
    }
    if (!jvmc_code_store_ref(code, (uint16_t)left_slot)) {
        return 0;
    }
    if (!CodegenEmitExpr(cls, code, right, params, body, ctx)) {
        return 0;
    }
    if (!jvmc_code_store_ref(code, (uint16_t)right_slot)) {
        return 0;
    }

    label_left_null = jvmc_code_label_create(code);
    label_right_null = jvmc_code_label_create(code);
    label_end = jvmc_code_label_create(code);
    if (label_left_null == NULL || label_right_null == NULL || label_end == NULL) {
        return 0;
    }

    if (!jvmc_code_load_ref(code, (uint16_t)left_slot)) {
        return 0;
    }
    if (!jvmc_code_if_null(code, label_left_null)) {
        return 0;
    }
    if (!jvmc_code_load_ref(code, (uint16_t)left_slot)) {
        return 0;
    }
    if (!jvmc_code_load_ref(code, (uint16_t)right_slot)) {
        return 0;
    }
    mref = jvmc_class_get_or_create_methodref(cls,
                                              "java/lang/String",
                                              "equals",
                                              "(Ljava/lang/Object;)Z");
    if (mref == NULL) {
        return 0;
    }
    if (!jvmc_code_invokevirtual(code, mref)) {
        return 0;
    }
    if (!jvmc_code_goto(code, label_end)) {
        return 0;
    }

    if (!jvmc_code_label_place(code, label_left_null)) {
        return 0;
    }
    if (!jvmc_code_load_ref(code, (uint16_t)right_slot)) {
        return 0;
    }
    if (!jvmc_code_if_null(code, label_right_null)) {
        return 0;
    }
    if (!jvmc_code_push_int(code, 0)) {
        return 0;
    }
    if (!jvmc_code_goto(code, label_end)) {
        return 0;
    }
    if (!jvmc_code_label_place(code, label_right_null)) {
        return 0;
    }
    if (!jvmc_code_push_int(code, 1)) {
        return 0;
    }
    return jvmc_code_label_place(code, label_end);
}

static int EmitRefCompareResult(jvmc_code *code, jvmc_compare cmp) {
    jvmc_label *label_true;
    jvmc_label *label_end;
    if (code == NULL) {
        return 0;
    }
    label_true = jvmc_code_label_create(code);
    label_end = jvmc_code_label_create(code);
    if (label_true == NULL || label_end == NULL) {
        return 0;
    }
    if (cmp == JVMC_CMP_EQ) {
        if (!jvmc_code_if_acmp_eq(code, label_true)) {
            return 0;
        }
    } else {
        if (!jvmc_code_if_acmp_ne(code, label_true)) {
            return 0;
        }
    }
    if (!jvmc_code_push_int(code, 0)) {
        return 0;
    }
    if (!jvmc_code_goto(code, label_end)) {
        return 0;
    }
    if (!jvmc_code_label_place(code, label_true)) {
        return 0;
    }
    if (!jvmc_code_push_int(code, 1)) {
        return 0;
    }
    return jvmc_code_label_place(code, label_end);
}

static int EmitInvertBoolean(jvmc_code *code) {
    jvmc_label *label_true;
    jvmc_label *label_end;
    if (code == NULL) {
        return 0;
    }
    label_true = jvmc_code_label_create(code);
    label_end = jvmc_code_label_create(code);
    if (label_true == NULL || label_end == NULL) {
        return 0;
    }
    if (!jvmc_code_if(code, JVMC_CMP_EQ, label_true)) {
        return 0;
    }
    if (!jvmc_code_push_int(code, 0)) {
        return 0;
    }
    if (!jvmc_code_goto(code, label_end)) {
        return 0;
    }
    if (!jvmc_code_label_place(code, label_true)) {
        return 0;
    }
    if (!jvmc_code_push_int(code, 1)) {
        return 0;
    }
    return jvmc_code_label_place(code, label_end);
}

static int EmitConditionJump(jvmc_class *cls, jvmc_code *code, NExpr *expr, NParamList *params, NStmt *body,
                             SemanticContext *ctx, int jump_if_true, jvmc_label *label) {
    jvmc_compare cmp;
    if (expr == NULL || code == NULL || label == NULL) {
        CodegenReportFail("EmitConditionJump: invalid args");
        return 0;
    }
    if (expr->type == EXPR_BINARY_OP) {
        switch (expr->value.binary.op) {
            case OP_EQ: cmp = JVMC_CMP_EQ; break;
            case OP_NEQ: cmp = JVMC_CMP_NE; break;
            case OP_LT: cmp = JVMC_CMP_LT; break;
            case OP_LE: cmp = JVMC_CMP_LE; break;
            case OP_GT: cmp = JVMC_CMP_GT; break;
            case OP_GE: cmp = JVMC_CMP_GE; break;
            default: cmp = JVMC_CMP_EQ; break;
        }
        if (cmp != JVMC_CMP_EQ || expr->value.binary.op == OP_EQ) {
            int is_double = 0;
            if (expr->value.binary.op == OP_EQ || expr->value.binary.op == OP_NEQ) {
                NType *left_type = expr->value.binary.left ? expr->value.binary.left->inferred_type : NULL;
                NType *right_type = expr->value.binary.right ? expr->value.binary.right->inferred_type : NULL;
                if (IsStringType(left_type) || IsStringType(right_type)) {
                    if (!EmitStringEqualsResult(cls, code,
                                                expr->value.binary.left,
                                                expr->value.binary.right,
                                                params, body, ctx)) {
                        CodegenReportFail("EmitConditionJump: string equals failed");
                        return 0;
                    }
                    if (expr->value.binary.op == OP_NEQ) {
                        jump_if_true = !jump_if_true;
                    }
                    return jvmc_code_if(code, jump_if_true ? JVMC_CMP_NE : JVMC_CMP_EQ, label);
                }
                if (IsRefType(left_type) || IsRefType(right_type)) {
                    if (!CodegenEmitExpr(cls, code, expr->value.binary.left, params, body, ctx)) {
                        CodegenReportExprFail("EmitConditionJump: ref left emit failed", expr->value.binary.left);
                        return 0;
                    }
                    if (!CodegenEmitExpr(cls, code, expr->value.binary.right, params, body, ctx)) {
                        CodegenReportExprFail("EmitConditionJump: ref right emit failed", expr->value.binary.right);
                        return 0;
                    }
                    if (expr->value.binary.op == OP_NEQ) {
                        jump_if_true = !jump_if_true;
                    }
                    if (jump_if_true) {
                        return jvmc_code_if_acmp_eq(code, label);
                    }
                    return jvmc_code_if_acmp_ne(code, label);
                }
            }
            if (!CodegenEmitExpr(cls, code, expr->value.binary.left, params, body, ctx)) {
                CodegenReportExprFail("EmitConditionJump: left emit failed", expr->value.binary.left);
                return 0;
            }
            if (!CodegenEmitExpr(cls, code, expr->value.binary.right, params, body, ctx)) {
                CodegenReportExprFail("EmitConditionJump: right emit failed", expr->value.binary.right);
                return 0;
            }
            if (!jump_if_true) {
                cmp = InvertCompare(cmp);
            }
            if (IsFloatLikeType(expr->value.binary.left ? expr->value.binary.left->inferred_type : NULL,
                                &is_double)) {
                if (is_double) {
                    if (!jvmc_code_cmp_double_g(code)) {
                        CodegenReportFail("EmitConditionJump: dcmpg failed");
                        return 0;
                    }
                } else {
                    if (!jvmc_code_cmp_float_g(code)) {
                        CodegenReportFail("EmitConditionJump: fcmpg failed");
                        return 0;
                    }
                }
                return jvmc_code_if(code, cmp, label);
            }
            if (!jvmc_code_if_icmp(code, cmp, label)) {
                CodegenReportFail("EmitConditionJump: if_icmp failed");
                return 0;
            }
            return 1;
        }
    }
    if (!CodegenEmitExpr(cls, code, expr, params, body, ctx)) {
        CodegenReportExprFail("EmitConditionJump: expr emit failed", expr);
        return 0;
    }
    if (IsRefType(expr->inferred_type)) {
        if (jump_if_true) {
            return jvmc_code_if_not_null(code, label);
        }
        return jvmc_code_if_null(code, label);
    }
    {
        int is_double = 0;
        if (IsFloatLikeType(expr->inferred_type, &is_double)) {
            if (is_double) {
                if (!jvmc_code_push_double(code, 0.0)) {
                    CodegenReportFail("EmitConditionJump: push double 0 failed");
                    return 0;
                }
                if (!jvmc_code_cmp_double_g(code)) {
                    CodegenReportFail("EmitConditionJump: dcmpg (zero) failed");
                    return 0;
                }
            } else {
                if (!jvmc_code_push_float(code, 0.0f)) {
                    CodegenReportFail("EmitConditionJump: push float 0 failed");
                    return 0;
                }
                if (!jvmc_code_cmp_float_g(code)) {
                    CodegenReportFail("EmitConditionJump: fcmpg (zero) failed");
                    return 0;
                }
            }
            return jvmc_code_if(code, jump_if_true ? JVMC_CMP_NE : JVMC_CMP_EQ, label);
        }
    }
    if (jump_if_true) {
        return jvmc_code_if(code, JVMC_CMP_NE, label);
    }
    return jvmc_code_if(code, JVMC_CMP_EQ, label);
}

static int EmitCompareResult(jvmc_class *cls, jvmc_code *code, NExpr *left, NExpr *right, NParamList *params,
                             NStmt *body, SemanticContext *ctx, jvmc_compare cmp) {
    int is_double = 0;
    if (code == NULL) {
        return 0;
    }
    if ((cmp == JVMC_CMP_EQ || cmp == JVMC_CMP_NE) && left != NULL && right != NULL) {
        NType *left_type = left->inferred_type;
        NType *right_type = right->inferred_type;
        if (IsStringType(left_type) || IsStringType(right_type)) {
            if (!EmitStringEqualsResult(cls, code, left, right, params, body, ctx)) {
                return 0;
            }
            if (cmp == JVMC_CMP_NE) {
                return EmitInvertBoolean(code);
            }
            return 1;
        }
        if (IsRefType(left_type) || IsRefType(right_type)) {
            if (!CodegenEmitExpr(cls, code, left, params, body, ctx)) {
                return 0;
            }
            if (!CodegenEmitExpr(cls, code, right, params, body, ctx)) {
                return 0;
            }
            return EmitRefCompareResult(code, cmp);
        }
    }
    if (!CodegenEmitExpr(cls, code, left, params, body, ctx)) {
        return 0;
    }
    if (!CodegenEmitExpr(cls, code, right, params, body, ctx)) {
        return 0;
    }
    if (IsFloatLikeType(left ? left->inferred_type : NULL, &is_double)) {
        if (is_double) {
            if (!jvmc_code_cmp_double_g(code)) {
                return 0;
            }
        } else {
            if (!jvmc_code_cmp_float_g(code)) {
                return 0;
            }
        }
        return EmitCompareZeroResult(code, cmp);
    }
    return JvmEmitCompareIntResult(code, cmp);
}

static int FindMaxSlotInStmtList(NStmt *stmts);

static int CountRefArgs(const unsigned char *flags, int count) {
    int refs = 0;
    if (flags == NULL || count <= 0) {
        return 0;
    }
    for (int i = 0; i < count; i++) {
        if (flags[i] != 0) {
            refs++;
        }
    }
    return refs;
}

static int TempReturnWidth(const NType *type) {
    if (type == NULL) {
        return 0;
    }
    if (type->kind == TYPE_KIND_BASE && type->base_type == TYPE_VOID) {
        return 0;
    }
    if (type->kind == TYPE_KIND_BASE &&
        (type->base_type == TYPE_DOUBLE || type->base_type == TYPE_REAL)) {
        return 2;
    }
    return 1;
}

static int MaxTempSlotsInExpr(NExpr *expr) {
    int max = 0;
    int local = 0;
    if (expr == NULL) {
        return 0;
    }
    switch (expr->type) {
        case EXPR_PAREN:
            return MaxTempSlotsInExpr(expr->value.inner_expr);
        case EXPR_CAST:
            return MaxTempSlotsInExpr(expr->value.cast.expr);
        case EXPR_UNARY_OP:
            return MaxTempSlotsInExpr(expr->value.unary.operand);
        case EXPR_BINARY_OP: {
            int left_max = MaxTempSlotsInExpr(expr->value.binary.left);
            int right_max = MaxTempSlotsInExpr(expr->value.binary.right);
            max = (left_max > right_max) ? left_max : right_max;
            if ((expr->value.binary.op == OP_EQ || expr->value.binary.op == OP_NEQ) &&
                (IsStringType(expr->value.binary.left ? expr->value.binary.left->inferred_type : NULL) ||
                 IsStringType(expr->value.binary.right ? expr->value.binary.right->inferred_type : NULL))) {
                local = 2;
            }
            break;
        }
        case EXPR_ASSIGN: {
            int left_max = MaxTempSlotsInExpr(expr->value.binary.left);
            int right_max = MaxTempSlotsInExpr(expr->value.binary.right);
            max = (left_max > right_max) ? left_max : right_max;
            break;
        }
        case EXPR_ARRAY_ACCESS: {
            int a = MaxTempSlotsInExpr(expr->value.array_access.array);
            int b = MaxTempSlotsInExpr(expr->value.array_access.index);
            int c = MaxTempSlotsInExpr(expr->value.array_access.index_end);
            max = a;
            if (b > max) max = b;
            if (c > max) max = c;
            break;
        }
        case EXPR_MEMBER_ACCESS:
            max = MaxTempSlotsInExpr(expr->value.member_access.object);
            break;
        case EXPR_METHOD_CALL:
        case EXPR_SUPER_METHOD: {
            int obj_max = MaxTempSlotsInExpr(expr->value.member_access.object);
            max = obj_max;
            for (int i = 0; i < expr->value.member_access.arg_count; i++) {
                int arg_max = MaxTempSlotsInExpr(expr->value.member_access.args[i]);
                if (arg_max > max) {
                    max = arg_max;
                }
            }
            {
                int refs = CountRefArgs(expr->resolved_arg_is_ref, expr->resolved_arg_count);
                int extra_store = (refs > 0) ? 2 : 0;
                local = refs + TempReturnWidth(expr->inferred_type) + extra_store;
            }
            break;
        }
        case EXPR_FUNC_CALL: {
            max = 0;
            for (int i = 0; i < expr->value.func_call.arg_count; i++) {
                int arg_max = MaxTempSlotsInExpr(expr->value.func_call.args[i]);
                if (arg_max > max) {
                    max = arg_max;
                }
            }
            if (expr->value.func_call.func_name != NULL &&
                strcmp(expr->value.func_call.func_name, "readf") == 0) {
                local = 4;
            } else {
                int refs = CountRefArgs(expr->resolved_arg_is_ref, expr->resolved_arg_count);
                int extra_store = (refs > 0) ? 2 : 0;
                local = refs + TempReturnWidth(expr->inferred_type) + extra_store;
            }
            break;
        }
        case EXPR_NEW: {
            max = 0;
            for (int i = 0; i < expr->value.new_expr.init_count; i++) {
                int arg_max = MaxTempSlotsInExpr(expr->value.new_expr.init_exprs[i]);
                if (arg_max > max) {
                    max = arg_max;
                }
            }
            local = 1 + CountRefArgs(expr->resolved_arg_is_ref, expr->resolved_arg_count);
            break;
        }
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
        default:
            break;
    }
    if (local > max) {
        max = local;
    }
    return max;
}

static int MaxTempSlotsInStmt(NStmt *stmt) {
    int max = 0;
    if (stmt == NULL) {
        return 0;
    }
    switch (stmt->type) {
        case STMT_EXPR:
        case STMT_RETURN:
            return MaxTempSlotsInExpr(stmt->value.expr);
        case STMT_DECL:
            if (stmt->value.decl.init_decls) {
                for (int i = 0; i < stmt->value.decl.init_decls->count; i++) {
                    NInitDecl *decl = stmt->value.decl.init_decls->decls[i];
                    if (decl && decl->initializer) {
                        if (decl->initializer->is_array) {
                            for (int j = 0; j < decl->initializer->array_init.count; j++) {
                                int elem_max = MaxTempSlotsInExpr(decl->initializer->array_init.elements[j]);
                                if (elem_max > max) {
                                    max = elem_max;
                                }
                            }
                        } else {
                            int expr_max = MaxTempSlotsInExpr(decl->initializer->expr);
                            if (expr_max > max) {
                                max = expr_max;
                            }
                        }
                    }
                }
            }
            return max;
        case STMT_COMPOUND:
            return MaxTempSlotsInStmtList(stmt->value.stmt_list ? stmt->value.stmt_list->first : NULL);
        case STMT_IF: {
            int a = MaxTempSlotsInExpr(stmt->value.if_stmt.condition);
            int b = MaxTempSlotsInStmt(stmt->value.if_stmt.then_stmt);
            int c = MaxTempSlotsInStmt(stmt->value.if_stmt.else_stmt);
            max = a;
            if (b > max) max = b;
            if (c > max) max = c;
            return max;
        }
        case STMT_WHILE: {
            int a = MaxTempSlotsInExpr(stmt->value.while_stmt.condition);
            int b = MaxTempSlotsInStmt(stmt->value.while_stmt.body);
            return (a > b) ? a : b;
        }
        case STMT_DO_WHILE: {
            int a = MaxTempSlotsInStmt(stmt->value.do_while_stmt.body);
            int b = MaxTempSlotsInExpr(stmt->value.do_while_stmt.condition);
            return (a > b) ? a : b;
        }
        case STMT_FOR: {
            int a = MaxTempSlotsInExpr(stmt->value.for_stmt.init_expr);
            int b = MaxTempSlotsInExpr(stmt->value.for_stmt.cond_expr);
            int c = MaxTempSlotsInExpr(stmt->value.for_stmt.iter_expr);
            int d = MaxTempSlotsInStmt(stmt->value.for_stmt.body);
            max = a;
            if (b > max) max = b;
            if (c > max) max = c;
            if (d > max) max = d;
            if (stmt->value.for_stmt.init_decls) {
                for (int i = 0; i < stmt->value.for_stmt.init_decls->count; i++) {
                    NInitDecl *decl = stmt->value.for_stmt.init_decls->decls[i];
                    if (decl && decl->initializer && decl->initializer->expr) {
                        int e = MaxTempSlotsInExpr(decl->initializer->expr);
                        if (e > max) {
                            max = e;
                        }
                    }
                }
            }
            return max;
        }
        case STMT_FOREACH: {
            int a = MaxTempSlotsInExpr(stmt->value.foreach_stmt.collection);
            int b = MaxTempSlotsInStmt(stmt->value.foreach_stmt.body);
            int max_local = (a > b) ? a : b;
            if (max_local < 3) {
                max_local = 3;
            }
            return max_local;
        }
        case STMT_SWITCH: {
            int a = MaxTempSlotsInExpr(stmt->value.switch_stmt.expr);
            max = a;
            for (int i = 0; i < stmt->value.switch_stmt.cases.count; i++) {
                NCaseItem *item = stmt->value.switch_stmt.cases.items[i];
                if (item && item->case_expr) {
                    int e = MaxTempSlotsInExpr(item->case_expr);
                    if (e > max) {
                        max = e;
                    }
                }
                if (item && item->stmts) {
                    int s = MaxTempSlotsInStmtList(item->stmts->first);
                    if (s > max) {
                        max = s;
                    }
                }
            }
            return max;
        }
        case STMT_BREAK:
        case STMT_CONTINUE:
            return 0;
    }
    return max;
}

static int MaxTempSlotsInStmtList(NStmt *stmts) {
    int max = 0;
    NStmt *cur = stmts;
    while (cur != NULL) {
        int local = MaxTempSlotsInStmt(cur);
        if (local > max) {
            max = local;
        }
        cur = cur->next;
    }
    return max;
}

static int FindMaxSlotInStmt(NStmt *stmt) {
    int max = -1;
    if (stmt == NULL) {
        return -1;
    }
    switch (stmt->type) {
        case STMT_DECL:
            if (stmt->value.decl.init_decls) {
                for (int i = 0; i < stmt->value.decl.init_decls->count; i++) {
                    NInitDecl *decl = stmt->value.decl.init_decls->decls[i];
                    if (decl && decl->jvm_slot_index > max) {
                        max = decl->jvm_slot_index;
                    }
                }
            }
            break;
        case STMT_COMPOUND: {
            int inner = FindMaxSlotInStmtList(stmt->value.stmt_list ? stmt->value.stmt_list->first : NULL);
            if (inner > max) {
                max = inner;
            }
            break;
        }
        case STMT_IF: {
            int a = FindMaxSlotInStmt(stmt->value.if_stmt.then_stmt);
            int b = FindMaxSlotInStmt(stmt->value.if_stmt.else_stmt);
            if (a > max) max = a;
            if (b > max) max = b;
            break;
        }
        case STMT_WHILE:
            max = FindMaxSlotInStmt(stmt->value.while_stmt.body);
            break;
        case STMT_DO_WHILE:
            max = FindMaxSlotInStmt(stmt->value.do_while_stmt.body);
            break;
        case STMT_FOR:
            if (stmt->value.for_stmt.init_decls) {
                for (int i = 0; i < stmt->value.for_stmt.init_decls->count; i++) {
                    NInitDecl *decl = stmt->value.for_stmt.init_decls->decls[i];
                    if (decl && decl->jvm_slot_index > max) {
                        max = decl->jvm_slot_index;
                    }
                }
            }
            {
                int inner = FindMaxSlotInStmt(stmt->value.for_stmt.body);
                if (inner > max) {
                    max = inner;
                }
            }
            break;
        case STMT_FOREACH:
            max = FindMaxSlotInStmt(stmt->value.foreach_stmt.body);
            break;
        case STMT_SWITCH:
            for (int i = 0; i < stmt->value.switch_stmt.cases.count; i++) {
                NCaseItem *item = stmt->value.switch_stmt.cases.items[i];
                int inner = FindMaxSlotInStmtList(item && item->stmts ? item->stmts->first : NULL);
                if (inner > max) {
                    max = inner;
                }
            }
            break;
        case STMT_EXPR:
        case STMT_RETURN:
        case STMT_BREAK:
        case STMT_CONTINUE:
            break;
    }
    return max;
}

static int FindMaxSlotInStmtList(NStmt *stmts) {
    int max = -1;
    NStmt *cur = stmts;
    while (cur != NULL) {
        int local = FindMaxSlotInStmt(cur);
        if (local > max) {
            max = local;
        }
        cur = cur->next;
    }
    return max;
}

static int FindMaxSlot(NParamList *params, NStmt *body) {
    int max = -1;
    if (params != NULL) {
        for (int i = 0; i < params->count; i++) {
            NParam *param = params->params[i];
            if (param && param->jvm_slot_index > max) {
                max = param->jvm_slot_index;
            }
        }
    }
    {
        int body_max = FindMaxSlotInStmt(body);
        if (body_max > max) {
            max = body_max;
        }
    }
    if (g_current_body != NULL && g_current_body != body) {
        int body_max = FindMaxSlotInStmt(g_current_body);
        if (body_max > max) {
            max = body_max;
        }
    }
    return max;
}

static int GetTempBase(NParamList *params, NStmt *body) {
    int base = FindMaxSlot(params, body) + 1;
    if (g_temp_base_override > base) {
        base = g_temp_base_override;
    }
    return base;
}

static int EmitPopForType(jvmc_code *code, const NType *type) {
    if (code == NULL || type == NULL) {
        return 1;
    }
    if (type->kind == TYPE_KIND_BASE && type->base_type == TYPE_VOID) {
        return 1;
    }
    if (type->kind == TYPE_KIND_BASE &&
        (type->base_type == TYPE_DOUBLE || type->base_type == TYPE_REAL)) {
        return jvmc_code_pop2(code);
    }
    return jvmc_code_pop(code);
}

static int EmitImplicitThisFieldLoad(jvmc_class *cls, jvmc_code *code, SemanticContext *ctx,
                                     const char *name, NType **type_out) {
    FieldInfo *field;
    char *desc;
    jvmc_fieldref *fref;
    if (cls == NULL || code == NULL || ctx == NULL || name == NULL) {
        return 0;
    }
    if (ctx->current_class == NULL) {
        return 0;
    }
    field = LookupClassFieldInHierarchy(ctx, ctx->current_class, name);
    if (field == NULL) {
        return 0;
    }
    desc = BuildJvmTypeDescriptor(field->type);
    if (desc == NULL) {
        return 0;
    }
    fref = jvmc_class_get_or_create_fieldref(cls, ctx->current_class->name, name, desc);
    free(desc);
    if (fref == NULL) {
        return 0;
    }
    if (!jvmc_code_load_ref(code, 0)) {
        return 0;
    }
    if (type_out) {
        *type_out = field->type;
    }
    return jvmc_code_getfield(code, fref);
}

static int EmitImplicitThisFieldStore(jvmc_class *cls, jvmc_code *code, SemanticContext *ctx,
                                      const char *name, const NType *value_type,
                                      NParamList *params, NStmt *body) {
    FieldInfo *field;
    char *desc;
    jvmc_fieldref *fref;
    int temp_slot;
    if (cls == NULL || code == NULL || ctx == NULL || name == NULL) {
        return 0;
    }
    if (ctx->current_class == NULL) {
        return 0;
    }
    field = LookupClassFieldInHierarchy(ctx, ctx->current_class, name);
    if (field == NULL) {
        return 0;
    }
    desc = BuildJvmTypeDescriptor(field->type);
    if (desc == NULL) {
        return 0;
    }
    fref = jvmc_class_get_or_create_fieldref(cls, ctx->current_class->name, name, desc);
    free(desc);
    if (fref == NULL) {
        return 0;
    }
    temp_slot = GetTempBase(params, body);
    if (!EmitStoreByType(code, value_type ? value_type : field->type, temp_slot)) {
        return 0;
    }
    if (!jvmc_code_load_ref(code, 0)) {
        return 0;
    }
    if (!EmitLoadByType(code, value_type ? value_type : field->type, temp_slot)) {
        return 0;
    }
    return jvmc_code_putfield(code, fref);
}

static void ResetCodegenScope(void) {
    free(g_var_stack);
    free(g_scope_stack);
    g_var_stack = NULL;
    g_scope_stack = NULL;
    g_var_count = 0;
    g_var_cap = 0;
    g_scope_count = 0;
    g_scope_cap = 0;
}

static int PushCodegenScope(void) {
    if (g_scope_count >= g_scope_cap) {
        int new_cap = (g_scope_cap == 0) ? 8 : (g_scope_cap * 2);
        int *grown = (int *)realloc(g_scope_stack, sizeof(int) * (size_t)new_cap);
        if (grown == NULL) {
            return 0;
        }
        g_scope_stack = grown;
        g_scope_cap = new_cap;
    }
    g_scope_stack[g_scope_count++] = g_var_count;
    return 1;
}

static void PopCodegenScope(void) {
    if (g_scope_count <= 0) {
        return;
    }
    g_scope_count--;
    g_var_count = g_scope_stack[g_scope_count];
}

static int AddCodegenVar(const char *name, int slot, NType *type) {
    if (name == NULL) {
        return 0;
    }
    if (g_var_count >= g_var_cap) {
        int new_cap = (g_var_cap == 0) ? 32 : (g_var_cap * 2);
        CodegenVar *grown = (CodegenVar *)realloc(g_var_stack, sizeof(CodegenVar) * (size_t)new_cap);
        if (grown == NULL) {
            return 0;
        }
        g_var_stack = grown;
        g_var_cap = new_cap;
    }
    g_var_stack[g_var_count].name = name;
    g_var_stack[g_var_count].slot = slot;
    g_var_stack[g_var_count].type = type;
    g_var_count++;
    return 1;
}

static int LookupCodegenVar(const char *name, int *slot_out, NType **type_out) {
    if (name == NULL) {
        return 0;
    }
    for (int i = g_var_count - 1; i >= 0; i--) {
        if (g_var_stack[i].name && strcmp(g_var_stack[i].name, name) == 0) {
            if (slot_out) *slot_out = g_var_stack[i].slot;
            if (type_out) *type_out = g_var_stack[i].type;
            return 1;
        }
    }
    return 0;
}

static void AddParamsToScope(NParamList *params) {
    if (params == NULL) {
        return;
    }
    for (int i = 0; i < params->count; i++) {
        NParam *param = params->params[i];
        if (param && param->param_name) {
            AddCodegenVar(param->param_name, param->jvm_slot_index, param->param_type);
        }
    }
}

static int GetElementTypeFromArray(const NType *array_type, NType *out_elem) {
    if (array_type == NULL || out_elem == NULL) {
        return 0;
    }
    if (array_type->kind == TYPE_KIND_BASE_ARRAY) {
        out_elem->kind = TYPE_KIND_BASE;
        out_elem->base_type = array_type->base_type;
        out_elem->class_name = NULL;
        out_elem->array_decl = NULL;
        return 1;
    }
    if (array_type->kind == TYPE_KIND_CLASS_ARRAY) {
        out_elem->kind = TYPE_KIND_CLASS;
        out_elem->base_type = TYPE_CLASS;
        out_elem->class_name = array_type->class_name;
        out_elem->array_decl = NULL;
        return 1;
    }
    return 0;
}

static int EmitNewArrayForType(jvmc_class *cls, jvmc_code *code, const NType *type) {
    if (code == NULL || type == NULL) {
        return 0;
    }
    if (type->kind == TYPE_KIND_CLASS) {
        char *internal = BuildJvmInternalName(type->class_name);
        jvmc_class_ref *cref;
        if (internal == NULL) {
            return 0;
        }
        cref = jvmc_class_get_or_create_class_ref(cls, internal);
        free(internal);
        if (cref == NULL) {
            return 0;
        }
        return jvmc_code_newarray_ref(code, cref);
    }
    if (type->kind == TYPE_KIND_BASE) {
        switch (type->base_type) {
            case TYPE_BOOL:
                return jvmc_code_newarray_primitive(code, JVMC_NEWARRAY_BOOLEAN);
            case TYPE_CHAR:
            case TYPE_INT:
                return jvmc_code_newarray_primitive(code, JVMC_NEWARRAY_INT);
            case TYPE_FLOAT:
                return jvmc_code_newarray_primitive(code, JVMC_NEWARRAY_FLOAT);
            case TYPE_DOUBLE:
            case TYPE_REAL:
                return jvmc_code_newarray_primitive(code, JVMC_NEWARRAY_DOUBLE);
            case TYPE_STRING: {
                jvmc_class_ref *cref = jvmc_class_get_or_create_class_ref(cls, "java/lang/String");
                if (cref == NULL) {
                    return 0;
                }
                return jvmc_code_newarray_ref(code, cref);
            }
            default:
                return jvmc_code_newarray_primitive(code, JVMC_NEWARRAY_INT);
        }
    }
    return 0;
}

static int EmitArrayStoreForType(jvmc_code *code, const NType *type) {
    if (code == NULL || type == NULL) {
        return 0;
    }
    if (type->kind == TYPE_KIND_CLASS || (type->kind == TYPE_KIND_BASE && type->base_type == TYPE_STRING)) {
        return jvmc_code_array_store_ref(code);
    }
    if (type->kind == TYPE_KIND_BASE) {
        switch (type->base_type) {
            case TYPE_BOOL:
                return jvmc_code_array_store_bool(code);
            case TYPE_FLOAT:
                return jvmc_code_array_store_float(code);
            case TYPE_DOUBLE:
            case TYPE_REAL:
                return jvmc_code_array_store_double(code);
            default:
                return jvmc_code_array_store_int(code);
        }
    }
    return jvmc_code_array_store_ref(code);
}

static int EmitArrayLoadForType(jvmc_code *code, const NType *type) {
    if (code == NULL || type == NULL) {
        return 0;
    }
    if (type->kind == TYPE_KIND_CLASS || (type->kind == TYPE_KIND_BASE && type->base_type == TYPE_STRING)) {
        return jvmc_code_array_load_ref(code);
    }
    if (type->kind == TYPE_KIND_BASE) {
        switch (type->base_type) {
            case TYPE_BOOL:
                return jvmc_code_array_load_bool(code);
            case TYPE_FLOAT:
                return jvmc_code_array_load_float(code);
            case TYPE_DOUBLE:
            case TYPE_REAL:
                return jvmc_code_array_load_double(code);
            default:
                return jvmc_code_array_load_int(code);
        }
    }
    return jvmc_code_array_load_ref(code);
}

static int TypeSlotWidth(const NType *type) {
    if (type == NULL) {
        return 1;
    }
    if (type->kind == TYPE_KIND_BASE) {
        if (type->base_type == TYPE_DOUBLE || type->base_type == TYPE_REAL) {
            return 2;
        }
    }
    return 1;
}

static int EmitLoadLValue(jvmc_class *cls, jvmc_code *code, NExpr *lhs, NParamList *params, NStmt *body,
                          SemanticContext *ctx, NType **type_out) {
    int slot = -1;
    NType *type = NULL;
    if (lhs == NULL || code == NULL) {
        return 0;
    }
    if (lhs->type == EXPR_IDENT) {
        if (ResolveVariableSlot(lhs->value.ident_name, params, body, &slot, &type)) {
            if (type_out) *type_out = type;
            return EmitLoadByType(code, type, slot);
        }
        if (EmitGlobalLoad(cls, code, ctx, lhs->value.ident_name, &type)) {
            if (type_out) *type_out = type;
            return 1;
        }
    }
    if (lhs->type == EXPR_MEMBER_ACCESS && lhs->jvm_ref_key.has_key &&
        lhs->jvm_ref_key.kind == JVM_REF_FIELD) {
        jvmc_fieldref *fref = jvmc_class_get_or_create_fieldref(cls,
                                                                lhs->jvm_ref_key.owner_internal_name,
                                                                lhs->jvm_ref_key.member_name,
                                                                lhs->jvm_ref_key.member_descriptor);
        if (fref == NULL) {
            return 0;
        }
        if (!CodegenEmitExpr(cls, code, lhs->value.member_access.object, params, body, ctx)) {
            return 0;
        }
        if (type_out != NULL) {
            *type_out = lhs->inferred_type;
        }
        return jvmc_code_getfield(code, fref);
    }
    if (lhs->type == EXPR_SUPER && lhs->jvm_ref_key.has_key &&
        lhs->jvm_ref_key.kind == JVM_REF_FIELD) {
        jvmc_fieldref *fref = jvmc_class_get_or_create_fieldref(cls,
                                                                lhs->jvm_ref_key.owner_internal_name,
                                                                lhs->jvm_ref_key.member_name,
                                                                lhs->jvm_ref_key.member_descriptor);
        if (fref == NULL) {
            return 0;
        }
        if (!jvmc_code_load_ref(code, 0)) {
            return 0;
        }
        if (type_out != NULL) {
            *type_out = lhs->inferred_type;
        }
        return jvmc_code_getfield(code, fref);
    }
    return 0;
}

static int EmitStoreLValue(jvmc_class *cls, jvmc_code *code, NExpr *lhs, const NType *type,
                           NParamList *params, NStmt *body, SemanticContext *ctx) {
    int slot = -1;
    NType *resolved = (NType *)type;
    if (lhs == NULL || code == NULL) {
        return 0;
    }
    if (resolved == NULL) {
        resolved = lhs->inferred_type;
    }
    if (resolved == NULL) {
        resolved = InferExpressionTypeSilent(lhs, ctx);
    }
    if (lhs->type == EXPR_IDENT) {
        if (ResolveVariableSlot(lhs->value.ident_name, params, body, &slot, &resolved)) {
            return EmitStoreByType(code, resolved, slot);
        }
        if (EmitGlobalStore(cls, code, ctx, lhs->value.ident_name, &resolved)) {
            return 1;
        }
        if (EmitImplicitThisFieldStore(cls, code, ctx, lhs->value.ident_name, resolved, params, body)) {
            return 1;
        }
        return 0;
    }
    if (lhs->type == EXPR_ARRAY_ACCESS) {
        NExpr *arr = lhs->value.array_access.array;
        NExpr *idx = lhs->value.array_access.index;
        NType elem_type;
        NType *elem_ptr = resolved;
        int temp_slot = GetTempBase(params, body);
        if (arr == NULL || idx == NULL) {
            return 0;
        }
        if (elem_ptr == NULL) {
            if (!GetElementTypeFromArray(arr->inferred_type, &elem_type)) {
                return 0;
            }
            elem_ptr = &elem_type;
        }
        if (!EmitStoreByType(code, elem_ptr, temp_slot)) {
            return 0;
        }
        if (!CodegenEmitExpr(cls, code, arr, params, body, ctx)) {
            return 0;
        }
        if (!CodegenEmitExpr(cls, code, idx, params, body, ctx)) {
            return 0;
        }
        if (!EmitLoadByType(code, elem_ptr, temp_slot)) {
            return 0;
        }
        return EmitArrayStoreForType(code, elem_ptr);
    }
    if (lhs->type == EXPR_MEMBER_ACCESS) {
        jvmc_fieldref *fref;
        NExpr *obj = lhs->value.member_access.object;
        int temp_slot = GetTempBase(params, body);
        if (!lhs->jvm_ref_key.has_key || lhs->jvm_ref_key.kind != JVM_REF_FIELD) {
            return 0;
        }
        fref = jvmc_class_get_or_create_fieldref(cls,
                                                 lhs->jvm_ref_key.owner_internal_name,
                                                 lhs->jvm_ref_key.member_name,
                                                 lhs->jvm_ref_key.member_descriptor);
        if (fref == NULL) {
            return 0;
        }
        if (!EmitStoreByType(code, resolved, temp_slot)) {
            return 0;
        }
        if (!CodegenEmitExpr(cls, code, obj, params, body, ctx)) {
            return 0;
        }
        if (!EmitLoadByType(code, resolved, temp_slot)) {
            return 0;
        }
        return jvmc_code_putfield(code, fref);
    }
    if (lhs->type == EXPR_SUPER) {
        jvmc_fieldref *fref;
        int temp_slot = GetTempBase(params, body);
        if (!lhs->jvm_ref_key.has_key || lhs->jvm_ref_key.kind != JVM_REF_FIELD) {
            return 0;
        }
        fref = jvmc_class_get_or_create_fieldref(cls,
                                                 lhs->jvm_ref_key.owner_internal_name,
                                                 lhs->jvm_ref_key.member_name,
                                                 lhs->jvm_ref_key.member_descriptor);
        if (fref == NULL) {
            return 0;
        }
        if (!EmitStoreByType(code, resolved, temp_slot)) {
            return 0;
        }
        if (!jvmc_code_load_ref(code, 0)) {
            return 0;
        }
        if (!EmitLoadByType(code, resolved, temp_slot)) {
            return 0;
        }
        return jvmc_code_putfield(code, fref);
    }
    fprintf(stderr, "[CODEGEN ERROR] EmitStoreLValue: unsupported lhs type %d\n",
            lhs ? (int)lhs->type : -1);
    return 0;
}

static int EmitArrayLiteral(jvmc_class *cls, jvmc_code *code, const NType *decl_type, const NInitializer *init,
                            NParamList *params, NStmt *body, SemanticContext *ctx, int store_slot,
                            const char *static_field_name, const char *static_desc) {
    NType elem_type;
    if (code == NULL || decl_type == NULL || init == NULL || !init->is_array) {
        return 0;
    }
    if (!GetElementTypeFromArray(decl_type, &elem_type)) {
        CodegenReportFail("EmitArrayLiteral: element type failed");
        return 0;
    }
    if (!jvmc_code_push_int(code, init->array_init.count)) {
        CodegenReportFail("EmitArrayLiteral: push count failed");
        return 0;
    }
    if (!EmitNewArrayForType(cls, code, &elem_type)) {
        CodegenReportFail("EmitArrayLiteral: newarray failed");
        return 0;
    }
    if (static_field_name != NULL && static_desc != NULL) {
        jvmc_fieldref *fref = jvmc_class_get_or_create_fieldref(cls, "Main", static_field_name, static_desc);
        if (fref == NULL) {
            CodegenReportFail("EmitArrayLiteral: fieldref failed");
            return 0;
        }
        if (!jvmc_code_putstatic(code, fref)) {
            CodegenReportFail("EmitArrayLiteral: putstatic failed");
            return 0;
        }
    } else {
        if (store_slot < 0) {
            CodegenReportFail("EmitArrayLiteral: invalid store slot");
            return 0;
        }
        if (!jvmc_code_store_ref(code, (uint16_t)store_slot)) {
            CodegenReportFail("EmitArrayLiteral: store array failed");
            return 0;
        }
    }
    for (int i = 0; i < init->array_init.count; i++) {
        if (static_field_name != NULL && static_desc != NULL) {
            jvmc_fieldref *fref = jvmc_class_get_or_create_fieldref(cls, "Main", static_field_name, static_desc);
            if (fref == NULL) {
                CodegenReportFail("EmitArrayLiteral: fieldref reload failed");
                return 0;
            }
            if (!jvmc_code_getstatic(code, fref)) {
                CodegenReportFail("EmitArrayLiteral: getstatic failed");
                return 0;
            }
        } else {
            if (!jvmc_code_load_ref(code, (uint16_t)store_slot)) {
                CodegenReportFail("EmitArrayLiteral: load array failed");
                return 0;
            }
        }
        if (!jvmc_code_push_int(code, i)) {
            CodegenReportFail("EmitArrayLiteral: push index failed");
            return 0;
        }
        if (!CodegenEmitExpr(cls, code, init->array_init.elements[i], params, body, ctx)) {
            fprintf(stderr, "[CODEGEN ERROR] EmitArrayLiteral: element emit failed at index %d\n", i);
            return 0;
        }
        if (!EmitArrayStoreForType(code, &elem_type)) {
            CodegenReportFail("EmitArrayLiteral: array store failed");
            return 0;
        }
    }
    if (static_field_name != NULL && static_desc != NULL) {
        jvmc_fieldref *fref = jvmc_class_get_or_create_fieldref(cls, "Main", static_field_name, static_desc);
        if (fref == NULL) {
            CodegenReportFail("EmitArrayLiteral: fieldref final failed");
            return 0;
        }
        return jvmc_code_getstatic(code, fref);
    }
    return jvmc_code_load_ref(code, (uint16_t)store_slot);
}

static int EmitAppendArrayElement(jvmc_class *cls, jvmc_code *code, NExpr *array_expr, NExpr *value_expr,
                                  NParamList *params, NStmt *body, SemanticContext *ctx) {
    NType elem_type;
    int base = FindMaxSlot(params, body) + 1;
    int slot_arr = base++;
    int slot_len = base++;
    int slot_new = base++;
    int slot_idx = base++;
    jvmc_label *label_loop = jvmc_code_label_create(code);
    jvmc_label *label_done = jvmc_code_label_create(code);

    if (label_loop == NULL || label_done == NULL) {
        return 0;
    }
    if (!GetElementTypeFromArray(array_expr ? array_expr->inferred_type : NULL, &elem_type)) {
        return 0;
    }
    if (!CodegenEmitExpr(cls, code, array_expr, params, body, ctx)) {
        return 0;
    }
    if (!jvmc_code_store_ref(code, (uint16_t)slot_arr)) {
        return 0;
    }
    if (!jvmc_code_load_ref(code, (uint16_t)slot_arr)) {
        return 0;
    }
    if (!jvmc_code_array_length(code)) {
        return 0;
    }
    if (!jvmc_code_store_int(code, (uint16_t)slot_len)) {
        return 0;
    }
    if (!jvmc_code_load_int(code, (uint16_t)slot_len)) {
        return 0;
    }
    if (!jvmc_code_push_int(code, 1)) {
        return 0;
    }
    if (!jvmc_code_add_int(code)) {
        return 0;
    }
    if (!EmitNewArrayForType(cls, code, &elem_type)) {
        return 0;
    }
    if (!jvmc_code_store_ref(code, (uint16_t)slot_new)) {
        return 0;
    }
    if (!jvmc_code_push_int(code, 0)) {
        return 0;
    }
    if (!jvmc_code_store_int(code, (uint16_t)slot_idx)) {
        return 0;
    }
    if (!jvmc_code_label_place(code, label_loop)) {
        return 0;
    }
    if (!jvmc_code_load_int(code, (uint16_t)slot_idx)) {
        return 0;
    }
    if (!jvmc_code_load_int(code, (uint16_t)slot_len)) {
        return 0;
    }
    if (!jvmc_code_if_icmp(code, JVMC_CMP_GE, label_done)) {
        return 0;
    }
    if (!jvmc_code_load_ref(code, (uint16_t)slot_new)) {
        return 0;
    }
    if (!jvmc_code_load_int(code, (uint16_t)slot_idx)) {
        return 0;
    }
    if (!jvmc_code_load_ref(code, (uint16_t)slot_arr)) {
        return 0;
    }
    if (!jvmc_code_load_int(code, (uint16_t)slot_idx)) {
        return 0;
    }
    if (!EmitArrayLoadForType(code, &elem_type)) {
        return 0;
    }
    if (!EmitArrayStoreForType(code, &elem_type)) {
        return 0;
    }
    if (!jvmc_code_load_int(code, (uint16_t)slot_idx)) {
        return 0;
    }
    if (!jvmc_code_push_int(code, 1)) {
        return 0;
    }
    if (!jvmc_code_add_int(code)) {
        return 0;
    }
    if (!jvmc_code_store_int(code, (uint16_t)slot_idx)) {
        return 0;
    }
    if (!jvmc_code_goto(code, label_loop)) {
        return 0;
    }
    if (!jvmc_code_label_place(code, label_done)) {
        return 0;
    }
    if (!jvmc_code_load_ref(code, (uint16_t)slot_new)) {
        return 0;
    }
    if (!jvmc_code_load_int(code, (uint16_t)slot_len)) {
        return 0;
    }
    if (!CodegenEmitExpr(cls, code, value_expr, params, body, ctx)) {
        return 0;
    }
    if (!EmitArrayStoreForType(code, &elem_type)) {
        return 0;
    }
    return jvmc_code_load_ref(code, (uint16_t)slot_new);
}

static int EmitAppendArrayConcat(jvmc_class *cls, jvmc_code *code, NExpr *array_expr, NExpr *rhs_expr,
                                 NParamList *params, NStmt *body, SemanticContext *ctx) {
    NType elem_type;
    int base = FindMaxSlot(params, body) + 1;
    int slot_arr = base++;
    int slot_rhs = base++;
    int slot_len1 = base++;
    int slot_len2 = base++;
    int slot_new = base++;
    int slot_idx = base++;
    jvmc_label *label_loop1 = jvmc_code_label_create(code);
    jvmc_label *label_done1 = jvmc_code_label_create(code);
    jvmc_label *label_loop2 = jvmc_code_label_create(code);
    jvmc_label *label_done2 = jvmc_code_label_create(code);

    if (label_loop1 == NULL || label_done1 == NULL || label_loop2 == NULL || label_done2 == NULL) {
        return 0;
    }
    if (!GetElementTypeFromArray(array_expr ? array_expr->inferred_type : NULL, &elem_type)) {
        return 0;
    }
    if (!CodegenEmitExpr(cls, code, array_expr, params, body, ctx)) {
        return 0;
    }
    if (!jvmc_code_store_ref(code, (uint16_t)slot_arr)) {
        return 0;
    }
    if (!CodegenEmitExpr(cls, code, rhs_expr, params, body, ctx)) {
        return 0;
    }
    if (!jvmc_code_store_ref(code, (uint16_t)slot_rhs)) {
        return 0;
    }
    if (!jvmc_code_load_ref(code, (uint16_t)slot_arr)) {
        return 0;
    }
    if (!jvmc_code_array_length(code)) {
        return 0;
    }
    if (!jvmc_code_store_int(code, (uint16_t)slot_len1)) {
        return 0;
    }
    if (!jvmc_code_load_ref(code, (uint16_t)slot_rhs)) {
        return 0;
    }
    if (!jvmc_code_array_length(code)) {
        return 0;
    }
    if (!jvmc_code_store_int(code, (uint16_t)slot_len2)) {
        return 0;
    }
    if (!jvmc_code_load_int(code, (uint16_t)slot_len1)) {
        return 0;
    }
    if (!jvmc_code_load_int(code, (uint16_t)slot_len2)) {
        return 0;
    }
    if (!jvmc_code_add_int(code)) {
        return 0;
    }
    if (!EmitNewArrayForType(cls, code, &elem_type)) {
        return 0;
    }
    if (!jvmc_code_store_ref(code, (uint16_t)slot_new)) {
        return 0;
    }
    if (!jvmc_code_push_int(code, 0)) {
        return 0;
    }
    if (!jvmc_code_store_int(code, (uint16_t)slot_idx)) {
        return 0;
    }
    if (!jvmc_code_label_place(code, label_loop1)) {
        return 0;
    }
    if (!jvmc_code_load_int(code, (uint16_t)slot_idx)) {
        return 0;
    }
    if (!jvmc_code_load_int(code, (uint16_t)slot_len1)) {
        return 0;
    }
    if (!jvmc_code_if_icmp(code, JVMC_CMP_GE, label_done1)) {
        return 0;
    }
    if (!jvmc_code_load_ref(code, (uint16_t)slot_new)) {
        return 0;
    }
    if (!jvmc_code_load_int(code, (uint16_t)slot_idx)) {
        return 0;
    }
    if (!jvmc_code_load_ref(code, (uint16_t)slot_arr)) {
        return 0;
    }
    if (!jvmc_code_load_int(code, (uint16_t)slot_idx)) {
        return 0;
    }
    if (!EmitArrayLoadForType(code, &elem_type)) {
        return 0;
    }
    if (!EmitArrayStoreForType(code, &elem_type)) {
        return 0;
    }
    if (!jvmc_code_load_int(code, (uint16_t)slot_idx)) {
        return 0;
    }
    if (!jvmc_code_push_int(code, 1)) {
        return 0;
    }
    if (!jvmc_code_add_int(code)) {
        return 0;
    }
    if (!jvmc_code_store_int(code, (uint16_t)slot_idx)) {
        return 0;
    }
    if (!jvmc_code_goto(code, label_loop1)) {
        return 0;
    }
    if (!jvmc_code_label_place(code, label_done1)) {
        return 0;
    }
    if (!jvmc_code_push_int(code, 0)) {
        return 0;
    }
    if (!jvmc_code_store_int(code, (uint16_t)slot_idx)) {
        return 0;
    }
    if (!jvmc_code_label_place(code, label_loop2)) {
        return 0;
    }
    if (!jvmc_code_load_int(code, (uint16_t)slot_idx)) {
        return 0;
    }
    if (!jvmc_code_load_int(code, (uint16_t)slot_len2)) {
        return 0;
    }
    if (!jvmc_code_if_icmp(code, JVMC_CMP_GE, label_done2)) {
        return 0;
    }
    if (!jvmc_code_load_ref(code, (uint16_t)slot_new)) {
        return 0;
    }
    if (!jvmc_code_load_int(code, (uint16_t)slot_len1)) {
        return 0;
    }
    if (!jvmc_code_load_int(code, (uint16_t)slot_idx)) {
        return 0;
    }
    if (!jvmc_code_add_int(code)) {
        return 0;
    }
    if (!jvmc_code_load_ref(code, (uint16_t)slot_rhs)) {
        return 0;
    }
    if (!jvmc_code_load_int(code, (uint16_t)slot_idx)) {
        return 0;
    }
    if (!EmitArrayLoadForType(code, &elem_type)) {
        return 0;
    }
    if (!EmitArrayStoreForType(code, &elem_type)) {
        return 0;
    }
    if (!jvmc_code_load_int(code, (uint16_t)slot_idx)) {
        return 0;
    }
    if (!jvmc_code_push_int(code, 1)) {
        return 0;
    }
    if (!jvmc_code_add_int(code)) {
        return 0;
    }
    if (!jvmc_code_store_int(code, (uint16_t)slot_idx)) {
        return 0;
    }
    if (!jvmc_code_goto(code, label_loop2)) {
        return 0;
    }
    if (!jvmc_code_label_place(code, label_done2)) {
        return 0;
    }
    return jvmc_code_load_ref(code, (uint16_t)slot_new);
}

static int EmitNewClassExpr(jvmc_class *cls, jvmc_code *code, NExpr *expr, NParamList *params,
                            NStmt *body, SemanticContext *ctx) {
    char *owner_internal = NULL;
    const char *ctor_desc = NULL;
    jvmc_class_ref *cref;
    jvmc_methodref *mref;
    int arg_count;
    int *ref_slots = NULL;
    int temp_base;
    int temp_obj;

    if (cls == NULL || code == NULL || expr == NULL || expr->inferred_type == NULL) {
        return 0;
    }
    owner_internal = expr->resolved_owner_internal
        ? DuplicateString(expr->resolved_owner_internal)
        : BuildJvmInternalName(expr->inferred_type->class_name);
    if (owner_internal == NULL) {
        return 0;
    }
    ctor_desc = (expr->resolved_member_descriptor != NULL) ? expr->resolved_member_descriptor : "()V";
    cref = jvmc_class_get_or_create_class_ref(cls, owner_internal);
    if (cref == NULL) {
        free(owner_internal);
        return 0;
    }
    if (!jvmc_code_new(code, cref)) {
        free(owner_internal);
        return 0;
    }
    if (!jvmc_code_dup(code)) {
        free(owner_internal);
        return 0;
    }
    temp_base = FindMaxSlot(params, body) + 1;
    temp_obj = temp_base++;
    if (!jvmc_code_store_ref(code, (uint16_t)temp_obj)) {
        free(owner_internal);
        return 0;
    }

    arg_count = expr->value.new_expr.init_count;
    if (arg_count > 0) {
        ref_slots = (int *)malloc(sizeof(int) * (size_t)arg_count);
        if (ref_slots == NULL) {
            free(owner_internal);
            return 0;
        }
        for (int i = 0; i < arg_count; i++) {
            ref_slots[i] = -1;
        }
    }
    for (int i = 0; i < arg_count; i++) {
        NExpr *arg = expr->value.new_expr.init_exprs[i];
        int is_ref = (expr->resolved_arg_is_ref != NULL &&
                      i < expr->resolved_arg_count &&
                      expr->resolved_arg_is_ref[i] != 0);
        if (is_ref) {
            int tmp_slot = temp_base++;
            if (!jvmc_code_push_int(code, 1)) {
                free(ref_slots);
                free(owner_internal);
                return 0;
            }
            if (!EmitNewArrayForType(cls, code, arg ? arg->inferred_type : NULL)) {
                free(ref_slots);
                free(owner_internal);
                return 0;
            }
            if (!jvmc_code_store_ref(code, (uint16_t)tmp_slot)) {
                free(ref_slots);
                free(owner_internal);
                return 0;
            }
            if (!jvmc_code_load_ref(code, (uint16_t)tmp_slot)) {
                free(ref_slots);
                free(owner_internal);
                return 0;
            }
            if (!jvmc_code_push_int(code, 0)) {
                free(ref_slots);
                free(owner_internal);
                return 0;
            }
            if (!EmitLoadLValue(cls, code, arg, params, body, ctx, NULL)) {
                free(ref_slots);
                free(owner_internal);
                return 0;
            }
            if (!EmitArrayStoreForType(code, arg ? arg->inferred_type : NULL)) {
                free(ref_slots);
                free(owner_internal);
                return 0;
            }
            if (!jvmc_code_load_ref(code, (uint16_t)tmp_slot)) {
                free(ref_slots);
                free(owner_internal);
                return 0;
            }
            ref_slots[i] = tmp_slot;
        } else {
            if (!CodegenEmitExpr(cls, code, arg, params, body, ctx)) {
                free(ref_slots);
                free(owner_internal);
                return 0;
            }
        }
    }

    mref = jvmc_class_get_or_create_methodref(cls, owner_internal, "<init>", ctor_desc);
    if (mref == NULL) {
        free(ref_slots);
        free(owner_internal);
        return 0;
    }
    if (!jvmc_code_invokespecial(code, mref)) {
        free(ref_slots);
        free(owner_internal);
        return 0;
    }

    for (int i = 0; i < arg_count; i++) {
        int is_ref = (expr->resolved_arg_is_ref != NULL &&
                      i < expr->resolved_arg_count &&
                      expr->resolved_arg_is_ref[i] != 0);
        if (is_ref && ref_slots && ref_slots[i] >= 0) {
            NExpr *arg = expr->value.new_expr.init_exprs[i];
            if (!jvmc_code_load_ref(code, (uint16_t)ref_slots[i])) {
                free(ref_slots);
                free(owner_internal);
                return 0;
            }
            if (!jvmc_code_push_int(code, 0)) {
                free(ref_slots);
                free(owner_internal);
                return 0;
            }
            if (!EmitArrayLoadForType(code, arg ? arg->inferred_type : NULL)) {
                free(ref_slots);
                free(owner_internal);
                return 0;
            }
            if (!EmitStoreLValue(cls, code, arg, arg ? arg->inferred_type : NULL, params, body, ctx)) {
                free(ref_slots);
                free(owner_internal);
                return 0;
            }
        }
    }

    if (!jvmc_code_load_ref(code, (uint16_t)temp_obj)) {
        free(ref_slots);
        free(owner_internal);
        return 0;
    }
    free(ref_slots);
    free(owner_internal);
    return 1;
}

static int CodegenAddEnumFields(jvmc_class *cls, NEnumDef *enum_def, uint16_t access_flags) {
    if (cls == NULL || enum_def == NULL || enum_def->items.items == NULL) {
        return 1;
    }
    for (int i = 0; i < enum_def->items.count; i++) {
        NEnumItem *item = enum_def->items.items[i];
        jvmc_field *field;
        if (item == NULL || item->name == NULL) {
            continue;
        }
        field = jvmc_class_get_or_create_field(cls, item->name, "I");
        if (field == NULL) {
            return 0;
        }
        if (!jvmc_field_add_flag(field, access_flags) ||
            !jvmc_field_add_flag(field, JVMC_FIELD_ACC_STATIC) ||
            !jvmc_field_add_flag(field, JVMC_FIELD_ACC_FINAL)) {
            return 0;
        }
    }
    return 1;
}

static int CodegenEmitEnumValues(jvmc_class *cls, jvmc_code *code,
                                 const char *owner_internal, NEnumDef *enum_def) {
    int next_value = 0;
    if (cls == NULL || code == NULL || owner_internal == NULL || enum_def == NULL) {
        return 0;
    }
    if (enum_def->items.items == NULL) {
        return 1;
    }
    for (int i = 0; i < enum_def->items.count; i++) {
        NEnumItem *item = enum_def->items.items[i];
        int value;
        jvmc_fieldref *fref;
        if (item == NULL || item->name == NULL) {
            continue;
        }
        if (item->has_value) {
            value = item->value;
            next_value = value + 1;
        } else {
            value = next_value;
            next_value += 1;
        }
        fref = jvmc_class_get_or_create_fieldref(cls, owner_internal, item->name, "I");
        if (fref == NULL) {
            return 0;
        }
        if (!jvmc_code_push_int(code, value)) {
            return 0;
        }
        if (!jvmc_code_putstatic(code, fref)) {
            return 0;
        }
    }
    return 1;
}

static int EmitInstanceFieldInitializers(jvmc_class *cls, NClassDef *class_def, SemanticContext *ctx,
                                         jvmc_code *code, NParamList *params, NStmt *body) {
    NClassMember *member;
    int temp_base;
    char *owner_internal;
    if (cls == NULL || class_def == NULL) {
        return 0;
    }
    if (code == NULL) {
        return 0;
    }
    owner_internal = BuildJvmInternalName(class_def->class_name);
    if (owner_internal == NULL) {
        return 0;
    }
    temp_base = FindMaxSlot(params, body) + 1;
    member = class_def->members.first;
    while (member != NULL) {
        if (member->type == CLASS_MEMBER_FIELD && member->value.field.init_decls) {
            NInitDeclList *decls = member->value.field.init_decls;
            for (int i = 0; i < decls->count; i++) {
                NInitDecl *decl = decls->decls[i];
                jvmc_fieldref *fref;
                if (decl == NULL || decl->name == NULL || decl->jvm_descriptor == NULL ||
                    decl->initializer == NULL) {
                    if (decl != NULL && decl->initializer == NULL &&
                        member->value.field.field_type != NULL &&
                        member->value.field.field_type->kind == TYPE_KIND_BASE) {
                        if (member->value.field.field_type->base_type == TYPE_FLOAT ||
                            member->value.field.field_type->base_type == TYPE_DOUBLE ||
                            member->value.field.field_type->base_type == TYPE_REAL) {
                            double nan_val = 0.0 / 0.0;
                            if (!jvmc_code_load_ref(code, 0)) {
                                free(owner_internal);
                                return 0;
                            }
                            if (member->value.field.field_type->base_type == TYPE_FLOAT) {
                                if (!jvmc_code_push_float(code, (float)nan_val)) {
                                    free(owner_internal);
                                    return 0;
                                }
                            } else {
                                if (!jvmc_code_push_double(code, nan_val)) {
                                    free(owner_internal);
                                    return 0;
                                }
                            }
                            if (!jvmc_code_putfield(code, fref)) {
                                free(owner_internal);
                                return 0;
                            }
                        }
                    }
                    continue;
                }
                fref = jvmc_class_get_or_create_fieldref(cls,
                                                        owner_internal,
                                                        decl->name,
                                                        decl->jvm_descriptor);
                if (fref == NULL) {
                    free(owner_internal);
                    return 0;
                }
                if (decl->initializer->is_array) {
                    int tmp_slot = temp_base++;
                    if (!EmitArrayLiteral(cls, code, member->value.field.field_type,
                                          decl->initializer, params, body, ctx, tmp_slot, NULL, NULL)) {
                        free(owner_internal);
                        return 0;
                    }
                    if (!jvmc_code_load_ref(code, 0)) {
                        free(owner_internal);
                        return 0;
                    }
                    if (!jvmc_code_load_ref(code, (uint16_t)tmp_slot)) {
                        free(owner_internal);
                        return 0;
                    }
                    if (!jvmc_code_putfield(code, fref)) {
                        free(owner_internal);
                        return 0;
                    }
                } else if (decl->initializer->expr) {
                    if (!jvmc_code_load_ref(code, 0)) {
                        free(owner_internal);
                        return 0;
                    }
                    if (!CodegenEmitExpr(cls, code,
                                         decl->initializer->expr, params, body, ctx)) {
                        free(owner_internal);
                        return 0;
                    }
                    if (!jvmc_code_putfield(code, fref)) {
                        free(owner_internal);
                        return 0;
                    }
                }
            }
        }
        member = member->next;
    }
    free(owner_internal);
    return 1;
}

static int CountArrayFieldInitTemps(const NClassDef *class_def) {
    int count = 0;
    NClassMember *member;
    if (class_def == NULL) {
        return 0;
    }
    member = class_def->members.first;
    while (member != NULL) {
        if (member->type == CLASS_MEMBER_FIELD && member->value.field.init_decls) {
            NInitDeclList *decls = member->value.field.init_decls;
            for (int i = 0; i < decls->count; i++) {
                NInitDecl *decl = decls->decls[i];
                if (decl != NULL && decl->initializer && decl->initializer->is_array) {
                    count++;
                }
            }
        }
        member = member->next;
    }
    return count;
}

static int CodegenEmitCtorBody(jvmc_class *cls, NClassDef *class_def, jvmc_code *code,
                               NStmt *body, NParamList *params, SemanticContext *ctx) {
    jvmc_methodref *mref;
    char *base_internal;
    int returned = 0;
    FlowContext flow;
    NStmt *prev_body = g_current_body;
    memset(&flow, 0, sizeof(flow));

    if (cls == NULL || class_def == NULL || code == NULL) {
        return 0;
    }
    if (!jvmc_code_load_ref(code, 0)) {
        return 0;
    }
    base_internal = BuildJvmInternalName(class_def->base_class_name ? class_def->base_class_name
                                                                    : "java/lang/Object");
    if (base_internal == NULL) {
        return 0;
    }
    mref = jvmc_class_get_or_create_methodref(cls, base_internal, "<init>", "()V");
    free(base_internal);
    if (mref == NULL) {
        return 0;
    }
    if (!jvmc_code_invokespecial(code, mref)) {
        return 0;
    }
    if (!EmitInstanceFieldInitializers(cls, class_def, ctx, code, params, body)) {
        free(flow.break_labels);
        free(flow.continue_labels);
        return 0;
    }
    if (body != NULL) {
        g_current_body = body;
        ResetCodegenScope();
        PushCodegenScope();
        AddParamsToScope(params);
        if (!CodegenEmitStmtList(cls, code, body, params, &returned, ctx, &flow)) {
            PopCodegenScope();
            ResetCodegenScope();
            g_current_body = prev_body;
            free(flow.break_labels);
            free(flow.continue_labels);
            return 0;
        }
        PopCodegenScope();
        ResetCodegenScope();
        g_current_body = prev_body;
    }
    free(flow.break_labels);
    free(flow.continue_labels);
    if (!returned) {
        return jvmc_code_return_void(code);
    }
    return 1;
}

static int CodegenEmitCtor(jvmc_class *cls, NClassDef *class_def, NCtorDef *ctor,
                           AccessSpec access, SemanticContext *ctx) {
    const char *ctor_desc;
    jvmc_method *method;
    jvmc_code *code;
    int max_locals;
    int base_slot;
    int extra_slots;
    if (cls == NULL || class_def == NULL || ctor == NULL) {
        return 0;
    }
    ctor_desc = CodegenBuildMethodDescriptor(NULL, ctor->params);
    if (ctor_desc == NULL) {
        return 0;
    }
    method = jvmc_class_get_or_create_method(cls, "<init>", ctor_desc);
    free((void *)ctor_desc);
    if (method == NULL) {
        return 0;
    }
    if (!jvmc_method_add_flag(method, CodegenAccessFromSpec(access))) {
        return 0;
    }
    code = jvmc_method_get_code(method);
    if (code == NULL) {
        return 0;
    }
    if (!jvmc_code_set_max_stack(code, 32)) {
        return 0;
    }
    max_locals = ComputeMaxLocals(ctor->params, ctor->body, 1);
    base_slot = FindMaxSlot(ctor->params, ctor->body) + 1;
    if (base_slot < 1) {
        base_slot = 1;
    }
    extra_slots = CountArrayFieldInitTemps(class_def);
    if (base_slot + extra_slots > max_locals) {
        max_locals = base_slot + extra_slots;
    }
    if (!jvmc_code_set_max_locals(code, (uint16_t)max_locals)) {
        return 0;
    }
    return CodegenEmitCtorBody(cls, class_def, code, ctor->body, ctor->params, ctx);
}

static int CodegenEmitDefaultCtor(jvmc_class *cls, NClassDef *class_def, SemanticContext *ctx) {
    jvmc_method *method;
    jvmc_code *code;
    int max_locals;
    int base_slot;
    int extra_slots;
    if (cls == NULL || class_def == NULL) {
        return 0;
    }
    method = jvmc_class_get_or_create_method(cls, "<init>", "()V");
    if (method == NULL) {
        return 0;
    }
    if (!jvmc_method_add_flag(method, JVMC_METHOD_ACC_PUBLIC)) {
        return 0;
    }
    code = jvmc_method_get_code(method);
    if (code == NULL) {
        return 0;
    }
    if (!jvmc_code_set_max_stack(code, 32)) {
        return 0;
    }
    max_locals = ComputeMaxLocals(NULL, NULL, 1);
    base_slot = FindMaxSlot(NULL, NULL) + 1;
    if (base_slot < 1) {
        base_slot = 1;
    }
    extra_slots = CountArrayFieldInitTemps(class_def);
    if (base_slot + extra_slots > max_locals) {
        max_locals = base_slot + extra_slots;
    }
    if (!jvmc_code_set_max_locals(code, (uint16_t)max_locals)) {
        return 0;
    }
    return CodegenEmitCtorBody(cls, class_def, code, NULL, NULL, ctx);
}

static int CodegenEmitExpr(jvmc_class *cls, jvmc_code *code, NExpr *expr, NParamList *params, NStmt *body,
                           SemanticContext *ctx) {
    int slot = -1;
    NType *type = NULL;
    if (expr == NULL || code == NULL) {
        return 0;
    }
    type = expr->inferred_type;
    switch (expr->type) {
        case EXPR_INT:
        case EXPR_FLOAT:
        case EXPR_CHAR:
        case EXPR_STRING:
        case EXPR_BOOL:
        case EXPR_NULL:
        case EXPR_NAN:
            return EmitLiteralExpr(code, expr, type);
        case EXPR_IDENT:
            if (ResolveVariableSlot(expr->value.ident_name, params, body, &slot, &type)) {
                if (type == NULL) {
                    type = expr->inferred_type;
                }
                if (!EmitLoadByType(code, type, slot)) {
                    CodegenReportExprFail("EXPR_IDENT: local load failed", expr);
                    return 0;
                }
                return 1;
            }
            if (EmitGlobalLoad(cls, code, ctx, expr->value.ident_name, &type)) {
                return 1;
            }
            if (EmitImplicitThisFieldLoad(cls, code, ctx, expr->value.ident_name, &type)) {
                return 1;
            }
            fprintf(stderr,
                    "[CODEGEN ERROR] EXPR_IDENT unresolved: %s (line %d)\n",
                    expr->value.ident_name ? expr->value.ident_name : "(null)",
                    expr->line);
            CodegenReportExprFail("EXPR_IDENT: unresolved symbol", expr);
            return 0;
        case EXPR_PAREN:
            return CodegenEmitExpr(cls, code, expr->value.inner_expr, params, body, ctx);
        case EXPR_CAST:
            if (expr->value.cast.expr == NULL) {
                return 0;
            }
            if (!CodegenEmitExpr(cls, code, expr->value.cast.expr, params, body, ctx)) {
                return 0;
            }
            return EmitNumericCast(code,
                                   expr->value.cast.expr->inferred_type,
                                   expr->value.cast.target_type != NULL ?
                                       expr->value.cast.target_type : expr->inferred_type);
        case EXPR_ARRAY_ACCESS: {
            NExpr *arr = expr->value.array_access.array;
            NExpr *idx = expr->value.array_access.index;
            NType elem_type;
            if (arr == NULL || idx == NULL) {
                return CodegenReportExprFail("EXPR_ARRAY_ACCESS: missing operands", expr);
            }
            if (!GetElementTypeFromArray(arr->inferred_type, &elem_type)) {
                return CodegenReportExprFail("EXPR_ARRAY_ACCESS: element type failed", expr);
            }
            if (!CodegenEmitExpr(cls, code, arr, params, body, ctx)) {
                return CodegenReportExprFail("EXPR_ARRAY_ACCESS: array emit failed", expr);
            }
            if (!CodegenEmitExpr(cls, code, idx, params, body, ctx)) {
                return CodegenReportExprFail("EXPR_ARRAY_ACCESS: index emit failed", expr);
            }
            return EmitArrayLoadForType(code, &elem_type);
        }
        case EXPR_FUNC_CALL: {
            const char *fname = expr->value.func_call.func_name;
            if (fname != NULL) {
                if (strcmp(fname, "__len") == 0 && expr->value.func_call.arg_count == 1) {
                    if (!CodegenEmitExpr(cls, code, expr->value.func_call.args[0], params, body, ctx)) {
                        return CodegenReportExprFail("EXPR_FUNC_CALL: __len arg emit failed", expr);
                    }
                    return jvmc_code_array_length(code);
                }
                if (strcmp(fname, "__slice") == 0 && expr->value.func_call.arg_count == 3) {
                    NExpr *arr = expr->value.func_call.args[0];
                    NExpr *start = expr->value.func_call.args[1];
                    NExpr *end = expr->value.func_call.args[2];
                    char *arr_desc = NULL;
                    jvmc_methodref *mref = NULL;
                    jvmc_class_ref *cref = NULL;
                    if (arr == NULL || start == NULL || end == NULL) {
                        return CodegenReportExprFail("EXPR_FUNC_CALL: __slice args missing", expr);
                    }
                    if (!CodegenEmitExpr(cls, code, arr, params, body, ctx)) {
                        return CodegenReportExprFail("EXPR_FUNC_CALL: __slice array emit failed", expr);
                    }
                    if (!CodegenEmitExpr(cls, code, start, params, body, ctx)) {
                        return CodegenReportExprFail("EXPR_FUNC_CALL: __slice start emit failed", expr);
                    }
                    if (!CodegenEmitExpr(cls, code, end, params, body, ctx)) {
                        return CodegenReportExprFail("EXPR_FUNC_CALL: __slice end emit failed", expr);
                    }
                    mref = jvmc_class_get_or_create_methodref(cls,
                                                              "dlang/Runtime",
                                                              "__slice",
                                                              "(Ljava/lang/Object;II)Ljava/lang/Object;");
                    if (mref == NULL) {
                        return CodegenReportExprFail("EXPR_FUNC_CALL: __slice methodref failed", expr);
                    }
                    if (!jvmc_code_invokestatic(code, mref)) {
                        return CodegenReportExprFail("EXPR_FUNC_CALL: __slice invoke failed", expr);
                    }
                    arr_desc = BuildJvmTypeDescriptor(arr->inferred_type);
                    if (arr_desc == NULL) {
                        return CodegenReportExprFail("EXPR_FUNC_CALL: __slice desc failed", expr);
                    }
                    cref = jvmc_class_get_or_create_class_ref(cls, arr_desc);
                    free(arr_desc);
                    if (cref == NULL) {
                        return CodegenReportExprFail("EXPR_FUNC_CALL: __slice cast failed", expr);
                    }
                    return jvmc_code_checkcast(code, cref);
                }
                if (strcmp(fname, "__append") == 0 && expr->value.func_call.arg_count == 2) {
                    NExpr *arr = expr->value.func_call.args[0];
                    NExpr *rhs = expr->value.func_call.args[1];
                    if (arr == NULL || rhs == NULL) {
                        return CodegenReportExprFail("EXPR_FUNC_CALL: __append args missing", expr);
                    }
                    if (rhs->inferred_type != NULL &&
                        (rhs->inferred_type->kind == TYPE_KIND_BASE_ARRAY ||
                         rhs->inferred_type->kind == TYPE_KIND_CLASS_ARRAY)) {
                        return EmitAppendArrayConcat(cls, code, arr, rhs, params, body, ctx);
                    }
                    return EmitAppendArrayElement(cls, code, arr, rhs, params, body, ctx);
                }
                if (strcmp(fname, "readf") == 0) {
                    int arg_count = expr->value.func_call.arg_count;
                    int extra_count = arg_count - 1;
                    int temp_base = GetTempBase(params, body);
                    int temp_obj = temp_base;
                    int temp_ret = temp_base + 1;
                    int prev_temp_base = g_temp_base_override;
                    jvmc_class_ref *cref = NULL;
                    jvmc_methodref *mref = NULL;
                    if (arg_count < 1) {
                        return CodegenReportExprFail("EXPR_FUNC_CALL: readf arg_count < 1", expr);
                    }
                    g_temp_base_override = temp_base + 2;
                    if (!jvmc_code_push_int(code, extra_count)) {
                        g_temp_base_override = prev_temp_base;
                        return 0;
                    }
                    cref = jvmc_class_get_or_create_class_ref(cls, "java/lang/Object");
                    if (cref == NULL) {
                        g_temp_base_override = prev_temp_base;
                        return 0;
                    }
                    if (!jvmc_code_newarray_ref(code, cref)) {
                        g_temp_base_override = prev_temp_base;
                        return CodegenReportExprFail("EXPR_FUNC_CALL: readf newarray failed", expr);
                    }
                    if (!jvmc_code_store_ref(code, (uint16_t)temp_obj)) {
                        g_temp_base_override = prev_temp_base;
                        return CodegenReportExprFail("EXPR_FUNC_CALL: readf store temp_obj failed", expr);
                    }
                    for (int i = 0; i < extra_count; i++) {
                        NExpr *arg = expr->value.func_call.args[i + 1];
                        if (!jvmc_code_load_ref(code, (uint16_t)temp_obj)) {
                            g_temp_base_override = prev_temp_base;
                            return CodegenReportExprFail("EXPR_FUNC_CALL: readf load temp_obj failed", expr);
                        }
                        if (!jvmc_code_push_int(code, i)) {
                            g_temp_base_override = prev_temp_base;
                            return CodegenReportExprFail("EXPR_FUNC_CALL: readf push index failed", expr);
                        }
                        if (!jvmc_code_push_int(code, 1)) {
                            g_temp_base_override = prev_temp_base;
                            return CodegenReportExprFail("EXPR_FUNC_CALL: readf push size failed", expr);
                        }
                        if (!EmitNewArrayForType(cls, code, arg ? arg->inferred_type : NULL)) {
                            g_temp_base_override = prev_temp_base;
                            return CodegenReportExprFail("EXPR_FUNC_CALL: readf new array elem failed", expr);
                        }
                        if (!jvmc_code_array_store_ref(code)) {
                            g_temp_base_override = prev_temp_base;
                            return CodegenReportExprFail("EXPR_FUNC_CALL: readf store ref failed", expr);
                        }
                    }
                    if (!CodegenEmitExpr(cls, code, expr->value.func_call.args[0], params, body, ctx)) {
                        g_temp_base_override = prev_temp_base;
                        return CodegenReportExprFail("EXPR_FUNC_CALL: readf fmt emit failed", expr);
                    }
                    if (!jvmc_code_load_ref(code, (uint16_t)temp_obj)) {
                        g_temp_base_override = prev_temp_base;
                        return CodegenReportExprFail("EXPR_FUNC_CALL: readf load temp_obj(2) failed", expr);
                    }
                    mref = jvmc_class_get_or_create_methodref(cls,
                                                              "dlang/Runtime",
                                                              "readf",
                                                              "(Ljava/lang/String;[Ljava/lang/Object;)I");
                    if (mref == NULL) {
                        g_temp_base_override = prev_temp_base;
                        return CodegenReportExprFail("EXPR_FUNC_CALL: readf methodref failed", expr);
                    }
                    if (!jvmc_code_invokestatic(code, mref)) {
                        g_temp_base_override = prev_temp_base;
                        return CodegenReportExprFail("EXPR_FUNC_CALL: readf invoke failed", expr);
                    }
                    if (!jvmc_code_store_int(code, (uint16_t)temp_ret)) {
                        g_temp_base_override = prev_temp_base;
                        return CodegenReportExprFail("EXPR_FUNC_CALL: readf store ret failed", expr);
                    }
                    for (int i = 0; i < extra_count; i++) {
                        NExpr *arg = expr->value.func_call.args[i + 1];
                        if (!jvmc_code_load_ref(code, (uint16_t)temp_obj)) {
                            g_temp_base_override = prev_temp_base;
                            return CodegenReportExprFail("EXPR_FUNC_CALL: readf load temp_obj(3) failed", expr);
                        }
                        if (!jvmc_code_push_int(code, i)) {
                            g_temp_base_override = prev_temp_base;
                            return CodegenReportExprFail("EXPR_FUNC_CALL: readf push index(2) failed", expr);
                        }
                        if (!jvmc_code_array_load_ref(code)) {
                            g_temp_base_override = prev_temp_base;
                            return CodegenReportExprFail("EXPR_FUNC_CALL: readf array load ref failed", expr);
                        }
                        {
                            char *arr_desc = BuildJvmArrayDescriptorForType(arg ? arg->inferred_type : NULL);
                            jvmc_class_ref *aref;
                            if (arr_desc == NULL) {
                                g_temp_base_override = prev_temp_base;
                                return CodegenReportExprFail("EXPR_FUNC_CALL: readf array desc failed", expr);
                            }
                            aref = jvmc_class_get_or_create_class_ref(cls, arr_desc);
                            free(arr_desc);
                            if (aref == NULL) {
                                g_temp_base_override = prev_temp_base;
                                return CodegenReportExprFail("EXPR_FUNC_CALL: readf array class ref failed", expr);
                            }
                            if (!jvmc_code_checkcast(code, aref)) {
                                g_temp_base_override = prev_temp_base;
                                return CodegenReportExprFail("EXPR_FUNC_CALL: readf checkcast failed", expr);
                            }
                        }
                        if (!jvmc_code_push_int(code, 0)) {
                            g_temp_base_override = prev_temp_base;
                            return CodegenReportExprFail("EXPR_FUNC_CALL: readf push 0 failed", expr);
                        }
                        if (!EmitArrayLoadForType(code, arg ? arg->inferred_type : NULL)) {
                            g_temp_base_override = prev_temp_base;
                            return CodegenReportExprFail("EXPR_FUNC_CALL: readf load elem failed", expr);
                        }
                        if (!EmitStoreLValue(cls, code, arg, arg ? arg->inferred_type : NULL,
                                             params, body, ctx)) {
                            g_temp_base_override = prev_temp_base;
                            return CodegenReportExprFail("EXPR_FUNC_CALL: readf store lvalue failed", expr);
                        }
                    }
                    g_temp_base_override = prev_temp_base;
                    return jvmc_code_load_int(code, (uint16_t)temp_ret);
                }
                if (strcmp(fname, "writef") == 0) {
                    jvmc_methodref *mref = NULL;
                    int arg_count = expr->value.func_call.arg_count;
                    if (arg_count < 1) {
                        return 0;
                    }
                    if (!CodegenEmitExpr(cls, code, expr->value.func_call.args[0], params, body, ctx)) {
                        return 0;
                    }
                    if (!EmitBuildObjectArray(cls, code, &expr->value.func_call.args[1], arg_count - 1,
                                              params, body, ctx)) {
                        return 0;
                    }
                    mref = jvmc_class_get_or_create_methodref(cls,
                                                              "dlang/Runtime",
                                                              "writef",
                                                              "(Ljava/lang/String;[Ljava/lang/Object;)V");
                    if (mref == NULL) {
                        return 0;
                    }
                    return jvmc_code_invokestatic(code, mref);
                }
                if (strcmp(fname, "format") == 0) {
                    jvmc_methodref *mref = NULL;
                    int arg_count = expr->value.func_call.arg_count;
                    if (arg_count < 1) {
                        return 0;
                    }
                    if (!CodegenEmitExpr(cls, code, expr->value.func_call.args[0], params, body, ctx)) {
                        return 0;
                    }
                    if (!EmitBuildObjectArray(cls, code, &expr->value.func_call.args[1], arg_count - 1,
                                              params, body, ctx)) {
                        return 0;
                    }
                    mref = jvmc_class_get_or_create_methodref(cls,
                                                              "dlang/Runtime",
                                                              "format",
                                                              "(Ljava/lang/String;[Ljava/lang/Object;)Ljava/lang/String;");
                    if (mref == NULL) {
                        return 0;
                    }
                    return jvmc_code_invokestatic(code, mref);
                }
                if (strcmp(fname, "write") == 0 || strcmp(fname, "writeln") == 0) {
                    int is_writeln = (strcmp(fname, "writeln") == 0);
                    if (expr->value.func_call.arg_count == 0) {
                        return EmitPrintCall(cls, code, NULL, 1, is_writeln, params, body, ctx);
                    }
                    for (int i = 0; i < expr->value.func_call.arg_count; i++) {
                        int is_last = (i == expr->value.func_call.arg_count - 1);
                        if (!EmitPrintCall(cls, code, expr->value.func_call.args[i], is_last, is_writeln,
                                           params, body, ctx)) {
                            return 0;
                        }
                    }
                    return 1;
                }
                if (strcmp(fname, "readln") == 0) {
                    return EmitReadlnCall(cls, code);
                }
            }
            {
                int arg_count = expr->value.func_call.arg_count;
                int *ref_slots = NULL;
                int temp_base = FindMaxSlot(params, body) + 1;
                int temp_ret = -1;
                int ret_width = TypeSlotWidth(expr->inferred_type);
                if (arg_count > 0) {
                    ref_slots = (int *)malloc(sizeof(int) * (size_t)arg_count);
                    if (ref_slots == NULL) {
                        return 0;
                    }
                    for (int i = 0; i < arg_count; i++) {
                        ref_slots[i] = -1;
                    }
                }
                for (int i = 0; i < arg_count; i++) {
                    NExpr *arg = expr->value.func_call.args[i];
                    int is_ref = (expr->resolved_arg_is_ref != NULL &&
                                  i < expr->resolved_arg_count &&
                                  expr->resolved_arg_is_ref[i] != 0);
                    if (is_ref) {
                        int temp_slot = temp_base++;
                        if (!jvmc_code_push_int(code, 1)) {
                            free(ref_slots);
                            return 0;
                        }
                        if (!EmitNewArrayForType(cls, code, arg ? arg->inferred_type : NULL)) {
                            free(ref_slots);
                            return 0;
                        }
                        if (!jvmc_code_store_ref(code, (uint16_t)temp_slot)) {
                            free(ref_slots);
                            return 0;
                        }
                        if (!jvmc_code_load_ref(code, (uint16_t)temp_slot)) {
                            free(ref_slots);
                            return 0;
                        }
                        if (!jvmc_code_push_int(code, 0)) {
                            free(ref_slots);
                            return 0;
                        }
                        if (!EmitLoadLValue(cls, code, arg, params, body, ctx, NULL)) {
                            free(ref_slots);
                            return 0;
                        }
                        if (!EmitArrayStoreForType(code, arg ? arg->inferred_type : NULL)) {
                            free(ref_slots);
                            return 0;
                        }
                        if (!jvmc_code_load_ref(code, (uint16_t)temp_slot)) {
                            free(ref_slots);
                            return 0;
                        }
                        ref_slots[i] = temp_slot;
                    } else {
                        if (!CodegenEmitExpr(cls, code, arg, params, body, ctx)) {
                            free(ref_slots);
                            return 0;
                        }
                    }
                }
                if (!EmitInvokeFromRefKey(cls, code, &expr->jvm_ref_key, 1)) {
                    free(ref_slots);
                    return 0;
                }
                if (expr->inferred_type != NULL &&
                    !(expr->inferred_type->kind == TYPE_KIND_BASE && expr->inferred_type->base_type == TYPE_VOID)) {
                    temp_ret = temp_base;
                    temp_base += ret_width;
                    if (!EmitStoreByType(code, expr->inferred_type, temp_ret)) {
                        free(ref_slots);
                        return 0;
                    }
                }
                for (int i = 0; i < arg_count; i++) {
                    NExpr *arg = expr->value.func_call.args[i];
                    int is_ref = (expr->resolved_arg_is_ref != NULL &&
                                  i < expr->resolved_arg_count &&
                                  expr->resolved_arg_is_ref[i] != 0);
                    if (is_ref && ref_slots && ref_slots[i] >= 0) {
                        if (!jvmc_code_load_ref(code, (uint16_t)ref_slots[i])) {
                            free(ref_slots);
                            return 0;
                        }
                        if (!jvmc_code_push_int(code, 0)) {
                            free(ref_slots);
                            return 0;
                        }
                        if (!EmitArrayLoadForType(code, arg ? arg->inferred_type : NULL)) {
                            free(ref_slots);
                            return 0;
                        }
                        if (!EmitStoreLValue(cls, code, expr->value.func_call.args[i],
                                             arg ? arg->inferred_type : NULL,
                                             params, body, ctx)) {
                            free(ref_slots);
                            return 0;
                        }
                    }
                }
                if (temp_ret >= 0) {
                    if (!EmitLoadByType(code, expr->inferred_type, temp_ret)) {
                        free(ref_slots);
                        return 0;
                    }
                }
                free(ref_slots);
            }
            return 1;
        }
        case EXPR_METHOD_CALL: {
            if (!CodegenEmitExpr(cls, code, expr->value.member_access.object, params, body, ctx)) {
                return 0;
            }
            {
                int arg_count = expr->value.member_access.arg_count;
                int *ref_slots = NULL;
                int temp_base = FindMaxSlot(params, body) + 1;
                int temp_ret = -1;
                int ret_width = TypeSlotWidth(expr->inferred_type);
                if (arg_count > 0) {
                    ref_slots = (int *)malloc(sizeof(int) * (size_t)arg_count);
                    if (ref_slots == NULL) {
                        return 0;
                    }
                    for (int i = 0; i < arg_count; i++) {
                        ref_slots[i] = -1;
                    }
                }
                for (int i = 0; i < arg_count; i++) {
                    NExpr *arg = expr->value.member_access.args[i];
                    int is_ref = (expr->resolved_arg_is_ref != NULL &&
                                  i < expr->resolved_arg_count &&
                                  expr->resolved_arg_is_ref[i] != 0);
                    if (is_ref) {
                        int temp_slot = temp_base++;
                        if (!jvmc_code_push_int(code, 1)) {
                            free(ref_slots);
                            return 0;
                        }
                        if (!EmitNewArrayForType(cls, code, arg ? arg->inferred_type : NULL)) {
                            free(ref_slots);
                            return 0;
                        }
                        if (!jvmc_code_store_ref(code, (uint16_t)temp_slot)) {
                            free(ref_slots);
                            return 0;
                        }
                        if (!jvmc_code_load_ref(code, (uint16_t)temp_slot)) {
                            free(ref_slots);
                            return 0;
                        }
                        if (!jvmc_code_push_int(code, 0)) {
                            free(ref_slots);
                            return 0;
                        }
                        if (!EmitLoadLValue(cls, code, arg, params, body, ctx, NULL)) {
                            free(ref_slots);
                            return 0;
                        }
                        if (!EmitArrayStoreForType(code, arg ? arg->inferred_type : NULL)) {
                            free(ref_slots);
                            return 0;
                        }
                        if (!jvmc_code_load_ref(code, (uint16_t)temp_slot)) {
                            free(ref_slots);
                            return 0;
                        }
                        ref_slots[i] = temp_slot;
                    } else {
                        if (!CodegenEmitExpr(cls, code, arg, params, body, ctx)) {
                            free(ref_slots);
                            return 0;
                        }
                    }
                }
                if (!EmitInvokeFromRefKey(cls, code, &expr->jvm_ref_key, 0)) {
                    free(ref_slots);
                    return 0;
                }
                if (expr->inferred_type != NULL &&
                    !(expr->inferred_type->kind == TYPE_KIND_BASE && expr->inferred_type->base_type == TYPE_VOID)) {
                    temp_ret = temp_base;
                    temp_base += ret_width;
                    if (!EmitStoreByType(code, expr->inferred_type, temp_ret)) {
                        free(ref_slots);
                        return 0;
                    }
                }
                for (int i = 0; i < arg_count; i++) {
                    NExpr *arg = expr->value.member_access.args[i];
                    int is_ref = (expr->resolved_arg_is_ref != NULL &&
                                  i < expr->resolved_arg_count &&
                                  expr->resolved_arg_is_ref[i] != 0);
                    if (is_ref && ref_slots && ref_slots[i] >= 0) {
                        if (!jvmc_code_load_ref(code, (uint16_t)ref_slots[i])) {
                            free(ref_slots);
                            return 0;
                        }
                        if (!jvmc_code_push_int(code, 0)) {
                            free(ref_slots);
                            return 0;
                        }
                        if (!EmitArrayLoadForType(code, arg ? arg->inferred_type : NULL)) {
                            free(ref_slots);
                            return 0;
                        }
                        if (!EmitStoreLValue(cls, code, expr->value.member_access.args[i],
                                             arg ? arg->inferred_type : NULL,
                                             params, body, ctx)) {
                            free(ref_slots);
                            return 0;
                        }
                    }
                }
                if (temp_ret >= 0) {
                    if (!EmitLoadByType(code, expr->inferred_type, temp_ret)) {
                        free(ref_slots);
                        return 0;
                    }
                }
                free(ref_slots);
            }
            return 1;
        }
        case EXPR_MEMBER_ACCESS: {
            NExpr *obj = expr->value.member_access.object;
            if (expr->enum_value_is_set) {
                return jvmc_code_push_int(code, expr->enum_value);
            }
            if (obj != NULL && obj->inferred_type != NULL &&
                (obj->inferred_type->kind == TYPE_KIND_BASE_ARRAY ||
                 obj->inferred_type->kind == TYPE_KIND_CLASS_ARRAY) &&
                expr->value.member_access.member_name != NULL &&
                strcmp(expr->value.member_access.member_name, "length") == 0) {
                if (!CodegenEmitExpr(cls, code, obj, params, body, ctx)) {
                    return 0;
                }
                return jvmc_code_array_length(code);
            }
            if (expr->jvm_ref_key.has_key && expr->jvm_ref_key.kind == JVM_REF_FIELD) {
                jvmc_fieldref *fref = jvmc_class_get_or_create_fieldref(cls,
                                                                        expr->jvm_ref_key.owner_internal_name,
                                                                        expr->jvm_ref_key.member_name,
                                                                        expr->jvm_ref_key.member_descriptor);
                if (fref == NULL) {
                    return 0;
                }
                if (!CodegenEmitExpr(cls, code, obj, params, body, ctx)) {
                    return 0;
                }
                return jvmc_code_getfield(code, fref);
            }
            return 0;
        }
        case EXPR_NEW: {
            if (expr->inferred_type != NULL &&
                (expr->inferred_type->kind == TYPE_KIND_BASE_ARRAY ||
                 expr->inferred_type->kind == TYPE_KIND_CLASS_ARRAY)) {
                NType elem_type;
                if (expr->value.new_expr.init_count < 1) {
                    return 0;
                }
                if (!CodegenEmitExpr(cls, code, expr->value.new_expr.init_exprs[0], params, body, ctx)) {
                    return 0;
                }
                if (!GetElementTypeFromArray(expr->inferred_type, &elem_type)) {
                    return 0;
                }
                return EmitNewArrayForType(cls, code, &elem_type);
            }
            if (expr->inferred_type != NULL && expr->inferred_type->kind == TYPE_KIND_CLASS) {
                return EmitNewClassExpr(cls, code, expr, params, body, ctx);
            }
            return 0;
        }
        case EXPR_UNARY_OP: {
            if (expr->value.unary.operand == NULL) {
                return 0;
            }
            if (expr->value.unary.op == OP_NOT) {
                jvmc_label *label_true = jvmc_code_label_create(code);
                jvmc_label *label_end = jvmc_code_label_create(code);
                if (label_true == NULL || label_end == NULL) {
                    return 0;
                }
                if (!EmitConditionJump(cls, code, expr->value.unary.operand, params, body, ctx, 1, label_true)) {
                    return 0;
                }
                if (!jvmc_code_push_int(code, 1)) {
                    return 0;
                }
                if (!jvmc_code_goto(code, label_end)) {
                    return 0;
                }
                if (!jvmc_code_label_place(code, label_true)) {
                    return 0;
                }
                if (!jvmc_code_push_int(code, 0)) {
                    return 0;
                }
                return jvmc_code_label_place(code, label_end);
            }
            if (!CodegenEmitExpr(cls, code, expr->value.unary.operand, params, body, ctx)) {
                return 0;
            }
            if (expr->value.unary.op == OP_MINUS) {
                return EmitNumericNeg(code, expr->value.unary.operand->inferred_type);
            }
            return 1;
        }
        case EXPR_BINARY_OP: {
            if (expr->value.binary.left == NULL || expr->value.binary.right == NULL) {
                return 0;
            }
            switch (expr->value.binary.op) {
                case OP_AND: {
                    jvmc_label *label_false = jvmc_code_label_create(code);
                    jvmc_label *label_end = jvmc_code_label_create(code);
                    if (label_false == NULL || label_end == NULL) {
                        return 0;
                    }
                    if (!EmitConditionJump(cls, code, expr->value.binary.left, params, body, ctx, 0, label_false)) {
                        return 0;
                    }
                    if (!EmitConditionJump(cls, code, expr->value.binary.right, params, body, ctx, 0, label_false)) {
                        return 0;
                    }
                    if (!jvmc_code_push_int(code, 1)) {
                        return 0;
                    }
                    if (!jvmc_code_goto(code, label_end)) {
                        return 0;
                    }
                    if (!jvmc_code_label_place(code, label_false)) {
                        return 0;
                    }
                    if (!jvmc_code_push_int(code, 0)) {
                        return 0;
                    }
                    return jvmc_code_label_place(code, label_end);
                }
                case OP_OR: {
                    jvmc_label *label_true = jvmc_code_label_create(code);
                    jvmc_label *label_end = jvmc_code_label_create(code);
                    if (label_true == NULL || label_end == NULL) {
                        return 0;
                    }
                    if (!EmitConditionJump(cls, code, expr->value.binary.left, params, body, ctx, 1, label_true)) {
                        return 0;
                    }
                    if (!EmitConditionJump(cls, code, expr->value.binary.right, params, body, ctx, 1, label_true)) {
                        return 0;
                    }
                    if (!jvmc_code_push_int(code, 0)) {
                        return 0;
                    }
                    if (!jvmc_code_goto(code, label_end)) {
                        return 0;
                    }
                    if (!jvmc_code_label_place(code, label_true)) {
                        return 0;
                    }
                    if (!jvmc_code_push_int(code, 1)) {
                        return 0;
                    }
                    return jvmc_code_label_place(code, label_end);
                }
                case OP_EQ:
                    return EmitCompareResult(cls, code, expr->value.binary.left, expr->value.binary.right,
                                             params, body, ctx, JVMC_CMP_EQ);
                case OP_NEQ:
                    return EmitCompareResult(cls, code, expr->value.binary.left, expr->value.binary.right,
                                             params, body, ctx, JVMC_CMP_NE);
                case OP_LT:
                    return EmitCompareResult(cls, code, expr->value.binary.left, expr->value.binary.right,
                                             params, body, ctx, JVMC_CMP_LT);
                case OP_LE:
                    return EmitCompareResult(cls, code, expr->value.binary.left, expr->value.binary.right,
                                             params, body, ctx, JVMC_CMP_LE);
                case OP_GT:
                    return EmitCompareResult(cls, code, expr->value.binary.left, expr->value.binary.right,
                                             params, body, ctx, JVMC_CMP_GT);
                case OP_GE:
                    return EmitCompareResult(cls, code, expr->value.binary.left, expr->value.binary.right,
                                             params, body, ctx, JVMC_CMP_GE);
                case OP_PLUS:
                case OP_MINUS:
                case OP_MUL:
                case OP_DIV:
                    if (!CodegenEmitExpr(cls, code, expr->value.binary.left, params, body, ctx)) {
                        return 0;
                    }
                    if (!CodegenEmitExpr(cls, code, expr->value.binary.right, params, body, ctx)) {
                        return 0;
                    }
                    return EmitNumericBinaryOp(code, expr->value.binary.op, expr->inferred_type);
                default:
                    return 0;
            }
        }
        case EXPR_ASSIGN:
            if (expr->value.binary.left != NULL &&
                expr->value.binary.left->type == EXPR_IDENT) {
                const char *name = expr->value.binary.left->value.ident_name;
                NExpr *right = expr->value.binary.right;
                if (right == NULL) {
                    return 0;
                }
                if (!CodegenEmitExpr(cls, code, right, params, body, ctx)) {
                    return 0;
                }
                if (ResolveVariableSlot(name, params, body, &slot, &type)) {
                    return EmitStoreByType(code, type, slot);
                }
                return EmitGlobalStore(cls, code, ctx, name, &type);
            }
            if (expr->value.binary.left != NULL &&
                expr->value.binary.left->type == EXPR_MEMBER_ACCESS) {
                NExpr *lhs = expr->value.binary.left;
                NExpr *rhs = expr->value.binary.right;
                jvmc_fieldref *fref;
                if (rhs == NULL || !lhs->jvm_ref_key.has_key ||
                    lhs->jvm_ref_key.kind != JVM_REF_FIELD) {
                    return 0;
                }
                fref = jvmc_class_get_or_create_fieldref(cls,
                                                         lhs->jvm_ref_key.owner_internal_name,
                                                         lhs->jvm_ref_key.member_name,
                                                         lhs->jvm_ref_key.member_descriptor);
                if (fref == NULL) {
                    return 0;
                }
                if (!CodegenEmitExpr(cls, code, lhs->value.member_access.object, params, body, ctx)) {
                    return 0;
                }
                if (!CodegenEmitExpr(cls, code, rhs, params, body, ctx)) {
                    return 0;
                }
                return jvmc_code_putfield(code, fref);
            }
            if (expr->value.binary.left != NULL &&
                expr->value.binary.left->type == EXPR_SUPER) {
                NExpr *lhs = expr->value.binary.left;
                NExpr *rhs = expr->value.binary.right;
                jvmc_fieldref *fref;
                if (rhs == NULL || !lhs->jvm_ref_key.has_key ||
                    lhs->jvm_ref_key.kind != JVM_REF_FIELD) {
                    return 0;
                }
                fref = jvmc_class_get_or_create_fieldref(cls,
                                                         lhs->jvm_ref_key.owner_internal_name,
                                                         lhs->jvm_ref_key.member_name,
                                                         lhs->jvm_ref_key.member_descriptor);
                if (fref == NULL) {
                    return 0;
                }
                if (!jvmc_code_load_ref(code, 0)) {
                    return 0;
                }
                if (!CodegenEmitExpr(cls, code, rhs, params, body, ctx)) {
                    return 0;
                }
                return jvmc_code_putfield(code, fref);
            }
            if (expr->value.binary.left != NULL &&
                expr->value.binary.left->type == EXPR_ARRAY_ACCESS) {
                NExpr *lhs = expr->value.binary.left;
                NExpr *rhs = expr->value.binary.right;
                NExpr *arr = lhs->value.array_access.array;
                NExpr *idx = lhs->value.array_access.index;
                NType elem_type;
                if (rhs == NULL || arr == NULL || idx == NULL) {
                    return 0;
                }
                if (!GetElementTypeFromArray(arr->inferred_type, &elem_type)) {
                    return 0;
                }
                if (!CodegenEmitExpr(cls, code, arr, params, body, ctx)) {
                    return 0;
                }
                if (!CodegenEmitExpr(cls, code, idx, params, body, ctx)) {
                    return 0;
                }
                if (!CodegenEmitExpr(cls, code, rhs, params, body, ctx)) {
                    return 0;
                }
                return EmitArrayStoreForType(code, &elem_type);
            }
            return 0;
        case EXPR_SUPER: {
            if (expr->jvm_ref_key.has_key && expr->jvm_ref_key.kind == JVM_REF_FIELD) {
                jvmc_fieldref *fref = jvmc_class_get_or_create_fieldref(cls,
                                                                        expr->jvm_ref_key.owner_internal_name,
                                                                        expr->jvm_ref_key.member_name,
                                                                        expr->jvm_ref_key.member_descriptor);
                if (fref == NULL) {
                    return 0;
                }
                if (!jvmc_code_load_ref(code, 0)) {
                    return 0;
                }
                return jvmc_code_getfield(code, fref);
            }
            return jvmc_code_load_ref(code, 0);
        }
        case EXPR_THIS:
            return jvmc_code_load_ref(code, 0);
        case EXPR_SUPER_METHOD: {
            if (!jvmc_code_load_ref(code, 0)) {
                return 0;
            }
            for (int i = 0; i < expr->value.member_access.arg_count; i++) {
                if (!CodegenEmitExpr(cls, code, expr->value.member_access.args[i], params, body, ctx)) {
                    return 0;
                }
            }
            {
                jvmc_methodref *mref = jvmc_class_get_or_create_methodref(cls,
                                                                          expr->jvm_ref_key.owner_internal_name,
                                                                          expr->jvm_ref_key.member_name,
                                                                          expr->jvm_ref_key.member_descriptor);
                if (mref == NULL) {
                    return 0;
                }
                return jvmc_code_invokespecial(code, mref);
            }
        }
        default:
            return 0;
    }
}

static int CodegenEmitStmtList(jvmc_class *cls, jvmc_code *code, NStmt *stmts, NParamList *params,
                               int *return_emitted, SemanticContext *ctx, FlowContext *flow) {
    NStmt *stmt = stmts;
    while (stmt != NULL) {
        if (stmt->type == STMT_RETURN) {
            NExpr *ret_expr = stmt->value.expr;
            if (ret_expr == NULL) {
                if (!jvmc_code_return_void(code)) {
                    return 0;
                }
                if (return_emitted) *return_emitted = 1;
                return 1;
            }
            if (!CodegenEmitExpr(cls, code, ret_expr, params, stmts, ctx)) {
                return 0;
            }
            if (!EmitReturnByType(code, ret_expr->inferred_type)) {
                return 0;
            }
            if (return_emitted) *return_emitted = 1;
            return 1;
        } else if (stmt->type == STMT_DECL) {
            if (stmt->value.decl.init_decls && stmt->value.decl.decl_type) {
                for (int i = 0; i < stmt->value.decl.init_decls->count; i++) {
                    NInitDecl *decl = stmt->value.decl.init_decls->decls[i];
                if (decl && decl->initializer) {
                    if (decl->initializer->is_array) {
                        if (!EmitArrayLiteral(cls, code, stmt->value.decl.decl_type, decl->initializer,
                                              params, stmts, ctx, decl->jvm_slot_index, NULL, NULL)) {
                            fprintf(stderr, "[CODEGEN ERROR] STMT_DECL: array initializer failed at line %d\n",
                                    stmt->line);
                            return 0;
                        }
                        if (!jvmc_code_pop(code)) {
                            fprintf(stderr, "[CODEGEN ERROR] STMT_DECL: array pop failed at line %d\n",
                                    stmt->line);
                            return 0;
                        }
                    } else if (decl->initializer->expr) {
                        if (!CodegenEmitExpr(cls, code, decl->initializer->expr, params, stmts, ctx)) {
                            fprintf(stderr, "[CODEGEN ERROR] STMT_DECL: expr initializer failed at line %d\n",
                                    stmt->line);
                            return 0;
                        }
                        if (!EmitStoreByType(code, stmt->value.decl.decl_type, decl->jvm_slot_index)) {
                            fprintf(stderr, "[CODEGEN ERROR] STMT_DECL: store failed at line %d\n",
                                    stmt->line);
                            return 0;
                        }
                    }
                }
                    if (decl && decl->name) {
                        AddCodegenVar(decl->name, decl->jvm_slot_index, stmt->value.decl.decl_type);
                    }
                }
            }
        } else if (stmt->type == STMT_EXPR) {
            if (stmt->value.expr != NULL) {
                if (!CodegenEmitExpr(cls, code, stmt->value.expr, params, stmts, ctx)) {
                    fprintf(stderr, "[CODEGEN ERROR] STMT_EXPR: emit expr failed at line %d (expr type %d)\n",
                            stmt->line,
                            stmt->value.expr ? (int)stmt->value.expr->type : -1);
                    return 0;
                }
                if (stmt->value.expr->type != EXPR_ASSIGN) {
                    NType *expr_type = stmt->value.expr->inferred_type;
                    if (expr_type == NULL) {
                        expr_type = InferExpressionTypeSilent(stmt->value.expr, ctx);
                    }
                    if (!EmitPopForType(code, expr_type)) {
                        fprintf(stderr, "[CODEGEN ERROR] STMT_EXPR: pop result failed at line %d\n", stmt->line);
                        return 0;
                    }
                }
            }
        } else if (stmt->type == STMT_COMPOUND) {
            if (!PushCodegenScope()) {
                CodegenReportFail("STMT_COMPOUND: scope push failed");
                return 0;
            }
            if (!CodegenEmitStmtList(cls, code,
                                     stmt->value.stmt_list ? stmt->value.stmt_list->first : NULL,
                                     params,
                                     return_emitted,
                                     ctx,
                                     flow)) {
                PopCodegenScope();
                return 0;
            }
            PopCodegenScope();
            if (return_emitted && *return_emitted) {
                return 1;
            }
        } else if (stmt->type == STMT_IF) {
            jvmc_label *label_else = jvmc_code_label_create(code);
            jvmc_label *label_end = jvmc_code_label_create(code);
            int local_return = 0;
            if (label_else == NULL || label_end == NULL) {
                CodegenReportFail("STMT_IF: label create failed");
                return 0;
            }
            if (!EmitConditionJump(cls, code, stmt->value.if_stmt.condition, params, stmts, ctx, 0, label_else)) {
                CodegenReportFail("STMT_IF: condition emit failed");
                return 0;
            }
            if (!CodegenEmitStmtList(cls, code, stmt->value.if_stmt.then_stmt, params, &local_return, ctx, flow)) {
                CodegenReportFail("STMT_IF: then block failed");
                return 0;
            }
            if (stmt->value.if_stmt.else_stmt != NULL) {
                if (!jvmc_code_goto(code, label_end)) {
                    CodegenReportFail("STMT_IF: goto end failed");
                    return 0;
                }
            }
            if (!jvmc_code_label_place(code, label_else)) {
                CodegenReportFail("STMT_IF: place else label failed");
                return 0;
            }
            if (stmt->value.if_stmt.else_stmt != NULL) {
                local_return = 0;
                if (!CodegenEmitStmtList(cls, code, stmt->value.if_stmt.else_stmt, params, &local_return, ctx, flow)) {
                    CodegenReportFail("STMT_IF: else block failed");
                    return 0;
                }
            }
            if (!jvmc_code_label_place(code, label_end)) {
                CodegenReportFail("STMT_IF: place end label failed");
                return 0;
            }
        } else if (stmt->type == STMT_WHILE) {
            jvmc_label *label_cond = jvmc_code_label_create(code);
            jvmc_label *label_end = jvmc_code_label_create(code);
            int local_return = 0;
            if (label_cond == NULL || label_end == NULL) {
                CodegenReportFail("STMT_WHILE: label create failed");
                return 0;
            }
            if (!jvmc_code_label_place(code, label_cond)) {
                CodegenReportFail("STMT_WHILE: place cond label failed");
                return 0;
            }
            if (!EmitConditionJump(cls, code, stmt->value.while_stmt.condition, params, stmts, ctx, 0, label_end)) {
                CodegenReportFail("STMT_WHILE: condition emit failed");
                return 0;
            }
            if (!FlowPushLoop(flow, label_end, label_cond)) {
                CodegenReportFail("STMT_WHILE: flow push failed");
                return 0;
            }
            if (!CodegenEmitStmtList(cls, code, stmt->value.while_stmt.body, params, &local_return, ctx, flow)) {
                CodegenReportFail("STMT_WHILE: body failed");
                return 0;
            }
            FlowPopLoop(flow);
            if (!jvmc_code_goto(code, label_cond)) {
                CodegenReportFail("STMT_WHILE: goto cond failed");
                return 0;
            }
            if (!jvmc_code_label_place(code, label_end)) {
                CodegenReportFail("STMT_WHILE: place end label failed");
                return 0;
            }
        } else if (stmt->type == STMT_DO_WHILE) {
            jvmc_label *label_body = jvmc_code_label_create(code);
            jvmc_label *label_cond = jvmc_code_label_create(code);
            jvmc_label *label_end = jvmc_code_label_create(code);
            int local_return = 0;
            if (label_body == NULL || label_cond == NULL || label_end == NULL) {
                CodegenReportFail("STMT_DO_WHILE: label create failed");
                return 0;
            }
            if (!jvmc_code_label_place(code, label_body)) {
                CodegenReportFail("STMT_DO_WHILE: place body label failed");
                return 0;
            }
            if (!FlowPushLoop(flow, label_end, label_cond)) {
                CodegenReportFail("STMT_DO_WHILE: flow push failed");
                return 0;
            }
            if (!CodegenEmitStmtList(cls, code, stmt->value.do_while_stmt.body, params, &local_return, ctx, flow)) {
                CodegenReportFail("STMT_DO_WHILE: body failed");
                return 0;
            }
            FlowPopLoop(flow);
            if (!jvmc_code_label_place(code, label_cond)) {
                CodegenReportFail("STMT_DO_WHILE: place cond label failed");
                return 0;
            }
            if (!EmitConditionJump(cls, code, stmt->value.do_while_stmt.condition, params, stmts, ctx, 1, label_body)) {
                CodegenReportFail("STMT_DO_WHILE: condition emit failed");
                return 0;
            }
            if (!jvmc_code_label_place(code, label_end)) {
                CodegenReportFail("STMT_DO_WHILE: place end label failed");
                return 0;
            }
        } else if (stmt->type == STMT_FOR) {
            jvmc_label *label_cond = jvmc_code_label_create(code);
            jvmc_label *label_iter = jvmc_code_label_create(code);
            jvmc_label *label_end = jvmc_code_label_create(code);
            int local_return = 0;
            if (label_cond == NULL || label_iter == NULL || label_end == NULL) {
                CodegenReportFail("STMT_FOR: label create failed");
                return 0;
            }
            if (!PushCodegenScope()) {
                CodegenReportFail("STMT_FOR: scope push failed");
                return 0;
            }
            if (stmt->value.for_stmt.init_expr != NULL) {
                if (!CodegenEmitExpr(cls, code, stmt->value.for_stmt.init_expr, params, stmts, ctx)) {
                    PopCodegenScope();
                    CodegenReportFail("STMT_FOR: init expr failed");
                    return 0;
                }
            }
            if (stmt->value.for_stmt.init_decls && stmt->value.for_stmt.init_decl_type) {
                for (int i = 0; i < stmt->value.for_stmt.init_decls->count; i++) {
                    NInitDecl *decl = stmt->value.for_stmt.init_decls->decls[i];
                    if (decl && decl->initializer && decl->initializer->expr) {
                        if (!CodegenEmitExpr(cls, code, decl->initializer->expr, params, stmts, ctx)) {
                            PopCodegenScope();
                            CodegenReportFail("STMT_FOR: init decl expr failed");
                            return 0;
                        }
                        if (!EmitStoreByType(code, stmt->value.for_stmt.init_decl_type, decl->jvm_slot_index)) {
                            PopCodegenScope();
                            CodegenReportFail("STMT_FOR: init decl store failed");
                            return 0;
                        }
                    }
                    if (decl && decl->name) {
                        AddCodegenVar(decl->name, decl->jvm_slot_index, stmt->value.for_stmt.init_decl_type);
                    }
                }
            }
            if (!jvmc_code_label_place(code, label_cond)) {
                PopCodegenScope();
                CodegenReportFail("STMT_FOR: place cond label failed");
                return 0;
            }
            if (stmt->value.for_stmt.cond_expr != NULL) {
                if (!EmitConditionJump(cls, code, stmt->value.for_stmt.cond_expr, params, stmts, ctx, 0, label_end)) {
                    PopCodegenScope();
                    CodegenReportFail("STMT_FOR: condition emit failed");
                    return 0;
                }
            }
            if (!FlowPushLoop(flow, label_end,
                              (stmt->value.for_stmt.iter_expr != NULL) ? label_iter : label_cond)) {
                PopCodegenScope();
                CodegenReportFail("STMT_FOR: flow push failed");
                return 0;
            }
            if (!CodegenEmitStmtList(cls, code, stmt->value.for_stmt.body, params, &local_return, ctx, flow)) {
                PopCodegenScope();
                CodegenReportFail("STMT_FOR: body failed");
                return 0;
            }
            FlowPopLoop(flow);
            if (!jvmc_code_label_place(code, label_iter)) {
                PopCodegenScope();
                CodegenReportFail("STMT_FOR: place iter label failed");
                return 0;
            }
            if (stmt->value.for_stmt.iter_expr != NULL) {
                if (!CodegenEmitExpr(cls, code, stmt->value.for_stmt.iter_expr, params, stmts, ctx)) {
                    PopCodegenScope();
                    CodegenReportFail("STMT_FOR: iter expr failed");
                    return 0;
                }
            }
            if (!jvmc_code_goto(code, label_cond)) {
                PopCodegenScope();
                CodegenReportFail("STMT_FOR: goto cond failed");
                return 0;
            }
            if (!jvmc_code_label_place(code, label_end)) {
                PopCodegenScope();
                CodegenReportFail("STMT_FOR: place end label failed");
                return 0;
            }
            PopCodegenScope();
        } else if (stmt->type == STMT_FOREACH) {
            NExpr *collection = stmt->value.foreach_stmt.collection;
            NType *coll_type = NULL;
            NType elem_type;
            int temp_base;
            int arr_slot;
            int idx_slot;
            int var_slot;
            int prev_temp_base = g_temp_base_override;
            jvmc_label *label_cond = jvmc_code_label_create(code);
            jvmc_label *label_end = jvmc_code_label_create(code);
            int local_return = 0;

            if (label_cond == NULL || label_end == NULL) {
                CodegenReportFail("STMT_FOREACH: label create failed");
                return 0;
            }
            if (collection == NULL) {
                CodegenReportFail("STMT_FOREACH: collection is NULL");
                return 0;
            }
            coll_type = InferExpressionTypeSilent(collection, ctx);
            if (coll_type == NULL || !GetElementTypeFromArray(coll_type, &elem_type)) {
                if (collection->type == EXPR_IDENT) {
                    NType *resolved = NULL;
                    if (ResolveVariableSlot(collection->value.ident_name, params, stmts, NULL, &resolved) &&
                        resolved != NULL && GetElementTypeFromArray(resolved, &elem_type)) {
                        coll_type = resolved;
                    }
                }
            }
            if (coll_type == NULL || !GetElementTypeFromArray(coll_type, &elem_type)) {
                CodegenReportFail("STMT_FOREACH: collection type is not array");
                return 0;
            }

            temp_base = GetTempBase(params, stmts);
            arr_slot = temp_base;
            idx_slot = temp_base + 1;
            var_slot = temp_base + 2;
            g_temp_base_override = temp_base + 3;

            if (!PushCodegenScope()) {
                g_temp_base_override = prev_temp_base;
                CodegenReportFail("STMT_FOREACH: scope push failed");
                return 0;
            }
            if (stmt->value.foreach_stmt.var_name) {
                AddCodegenVar(stmt->value.foreach_stmt.var_name, var_slot,
                              stmt->value.foreach_stmt.is_typed ? stmt->value.foreach_stmt.var_type : &elem_type);
            }
            if (!CodegenEmitExpr(cls, code, collection, params, stmts, ctx)) {
                g_temp_base_override = prev_temp_base;
                PopCodegenScope();
                CodegenReportFail("STMT_FOREACH: collection emit failed");
                return 0;
            }
            if (!jvmc_code_store_ref(code, (uint16_t)arr_slot)) {
                g_temp_base_override = prev_temp_base;
                PopCodegenScope();
                CodegenReportFail("STMT_FOREACH: store array failed");
                return 0;
            }
            if (!jvmc_code_push_int(code, 0)) {
                g_temp_base_override = prev_temp_base;
                PopCodegenScope();
                CodegenReportFail("STMT_FOREACH: init idx failed");
                return 0;
            }
            if (!jvmc_code_store_int(code, (uint16_t)idx_slot)) {
                g_temp_base_override = prev_temp_base;
                PopCodegenScope();
                CodegenReportFail("STMT_FOREACH: store idx failed");
                return 0;
            }
            if (!jvmc_code_label_place(code, label_cond)) {
                g_temp_base_override = prev_temp_base;
                PopCodegenScope();
                CodegenReportFail("STMT_FOREACH: place cond label failed");
                return 0;
            }
            if (!jvmc_code_load_int(code, (uint16_t)idx_slot)) {
                g_temp_base_override = prev_temp_base;
                PopCodegenScope();
                CodegenReportFail("STMT_FOREACH: load idx failed");
                return 0;
            }
            if (!jvmc_code_load_ref(code, (uint16_t)arr_slot)) {
                g_temp_base_override = prev_temp_base;
                PopCodegenScope();
                CodegenReportFail("STMT_FOREACH: load array failed");
                return 0;
            }
            if (!jvmc_code_array_length(code)) {
                g_temp_base_override = prev_temp_base;
                PopCodegenScope();
                CodegenReportFail("STMT_FOREACH: array length failed");
                return 0;
            }
            if (!jvmc_code_if_icmp(code, JVMC_CMP_GE, label_end)) {
                g_temp_base_override = prev_temp_base;
                PopCodegenScope();
                CodegenReportFail("STMT_FOREACH: compare failed");
                return 0;
            }
            if (!jvmc_code_load_ref(code, (uint16_t)arr_slot)) {
                g_temp_base_override = prev_temp_base;
                PopCodegenScope();
                CodegenReportFail("STMT_FOREACH: load array(2) failed");
                return 0;
            }
            if (!jvmc_code_load_int(code, (uint16_t)idx_slot)) {
                g_temp_base_override = prev_temp_base;
                PopCodegenScope();
                CodegenReportFail("STMT_FOREACH: load idx(2) failed");
                return 0;
            }
            if (!EmitArrayLoadForType(code, &elem_type)) {
                g_temp_base_override = prev_temp_base;
                PopCodegenScope();
                CodegenReportFail("STMT_FOREACH: load element failed");
                return 0;
            }
            if (!EmitStoreByType(code, &elem_type, var_slot)) {
                g_temp_base_override = prev_temp_base;
                PopCodegenScope();
                CodegenReportFail("STMT_FOREACH: store var failed");
                return 0;
            }
            if (stmt->value.foreach_stmt.body) {
                if (!CodegenEmitStmtList(cls, code, stmt->value.foreach_stmt.body, params, &local_return, ctx, flow)) {
                    g_temp_base_override = prev_temp_base;
                    PopCodegenScope();
                    CodegenReportFail("STMT_FOREACH: body failed");
                    return 0;
                }
            }
            if (!jvmc_code_load_int(code, (uint16_t)idx_slot)) {
                g_temp_base_override = prev_temp_base;
                PopCodegenScope();
                CodegenReportFail("STMT_FOREACH: load idx(3) failed");
                return 0;
            }
            if (!jvmc_code_push_int(code, 1)) {
                g_temp_base_override = prev_temp_base;
                PopCodegenScope();
                CodegenReportFail("STMT_FOREACH: push 1 failed");
                return 0;
            }
            if (!jvmc_code_add_int(code)) {
                g_temp_base_override = prev_temp_base;
                PopCodegenScope();
                CodegenReportFail("STMT_FOREACH: add idx failed");
                return 0;
            }
            if (!jvmc_code_store_int(code, (uint16_t)idx_slot)) {
                g_temp_base_override = prev_temp_base;
                PopCodegenScope();
                CodegenReportFail("STMT_FOREACH: store idx(2) failed");
                return 0;
            }
            if (!jvmc_code_goto(code, label_cond)) {
                g_temp_base_override = prev_temp_base;
                PopCodegenScope();
                CodegenReportFail("STMT_FOREACH: goto cond failed");
                return 0;
            }
            if (!jvmc_code_label_place(code, label_end)) {
                g_temp_base_override = prev_temp_base;
                PopCodegenScope();
                CodegenReportFail("STMT_FOREACH: place end label failed");
                return 0;
            }
            PopCodegenScope();
            g_temp_base_override = prev_temp_base;
            if (return_emitted && *return_emitted) {
                return 1;
            }
        } else if (stmt->type == STMT_SWITCH) {
            jvmc_label *label_end = jvmc_code_label_create(code);
            jvmc_label **case_labels = NULL;
            int case_count = stmt->value.switch_stmt.cases.count;
            int default_index = -1;
            int local_return = 0;
            NType *switch_type = NULL;
            if (label_end == NULL) {
                return 0;
            }
            if (stmt->value.switch_stmt.expr != NULL) {
                switch_type = stmt->value.switch_stmt.expr->inferred_type;
                if (switch_type == NULL) {
                    switch_type = InferExpressionTypeSilent(stmt->value.switch_stmt.expr, ctx);
                }
                if (switch_type != NULL && !IsIntegralType(switch_type)) {
                    return 0;
                }
            }
            if (case_count > 0) {
                case_labels = (jvmc_label **)calloc((size_t)case_count, sizeof(jvmc_label *));
                if (case_labels == NULL) {
                    return 0;
                }
                for (int i = 0; i < case_count; i++) {
                    case_labels[i] = jvmc_code_label_create(code);
                    if (case_labels[i] == NULL) {
                        free(case_labels);
                        return 0;
                    }
                    if (stmt->value.switch_stmt.cases.items[i] &&
                        stmt->value.switch_stmt.cases.items[i]->type == CASE_ITEM_DEFAULT) {
                        default_index = i;
                    }
                }
            }
            for (int i = 0; i < case_count; i++) {
                NCaseItem *item = stmt->value.switch_stmt.cases.items[i];
                if (item == NULL || item->type == CASE_ITEM_DEFAULT) {
                    continue;
                }
                if (!CodegenEmitExpr(cls, code, stmt->value.switch_stmt.expr, params, stmts, ctx)) {
                    free(case_labels);
                    return 0;
                }
                if (!CodegenEmitExpr(cls, code, item->case_expr, params, stmts, ctx)) {
                    free(case_labels);
                    return 0;
                }
                if (!jvmc_code_if_icmp(code, JVMC_CMP_EQ, case_labels[i])) {
                    free(case_labels);
                    return 0;
                }
            }
            if (default_index >= 0) {
                if (!jvmc_code_goto(code, case_labels[default_index])) {
                    free(case_labels);
                    return 0;
                }
            } else {
                if (!jvmc_code_goto(code, label_end)) {
                    free(case_labels);
                    return 0;
                }
            }
            if (!FlowPushSwitch(flow, label_end)) {
                free(case_labels);
                return 0;
            }
            for (int i = 0; i < case_count; i++) {
                NCaseItem *item = stmt->value.switch_stmt.cases.items[i];
                if (!jvmc_code_label_place(code, case_labels[i])) {
                    FlowPopSwitch(flow);
                    free(case_labels);
                    return 0;
                }
                if (item && item->stmts) {
                    if (!CodegenEmitStmtList(cls, code, item->stmts->first, params, &local_return, ctx, flow)) {
                        FlowPopSwitch(flow);
                        free(case_labels);
                        return 0;
                    }
                }
            }
            FlowPopSwitch(flow);
            free(case_labels);
            if (!jvmc_code_label_place(code, label_end)) {
                return 0;
            }
        } else if (stmt->type == STMT_BREAK) {
            jvmc_label *target = FlowTopBreak(flow);
            if (target == NULL) {
                return 0;
            }
            if (!jvmc_code_goto(code, target)) {
                return 0;
            }
        } else if (stmt->type == STMT_CONTINUE) {
            jvmc_label *target = FlowTopContinue(flow);
            if (target == NULL) {
                return 0;
            }
            if (!jvmc_code_goto(code, target)) {
                return 0;
            }
        }
        stmt = stmt->next;
    }
    return 1;
}

static int CodegenAddMethodWithBody(jvmc_class *cls, const char *name, const char *descriptor,
                                    uint16_t access_flags, int is_static,
                                    NStmt *body, NParamList *params, SemanticContext *ctx) {
    jvmc_method *method = NULL;
    jvmc_code *code = NULL;
    int returned = 0;
    NStmt *prev_body = g_current_body;

    if (cls == NULL || name == NULL || descriptor == NULL) {
        return 0;
    }
    method = jvmc_class_get_or_create_method(cls, name, descriptor);
    if (method == NULL) {
        return 0;
    }
    if (!jvmc_method_add_flag(method, access_flags)) {
        return 0;
    }
    if (is_static) {
        if (!jvmc_method_add_flag(method, JVMC_METHOD_ACC_STATIC)) {
            return 0;
        }
    }
    code = jvmc_method_get_code(method);
    if (code == NULL) {
        return 0;
    }
    if (!jvmc_code_set_max_stack(code, 32)) {
        return 0;
    }
    if (!jvmc_code_set_max_locals(code, (uint16_t)ComputeMaxLocals(params, body, is_static ? 0 : 1))) {
        return 0;
    }
    FlowContext flow;
    memset(&flow, 0, sizeof(flow));
    ResetCodegenScope();
    PushCodegenScope();
    AddParamsToScope(params);
    g_current_body = body;
    if (!CodegenEmitStmtList(cls, code, body, params, &returned, ctx, &flow)) {
        PopCodegenScope();
        ResetCodegenScope();
        g_current_body = prev_body;
        free(flow.break_labels);
        free(flow.continue_labels);
        return 0;
    }
    PopCodegenScope();
    ResetCodegenScope();
    g_current_body = prev_body;
    free(flow.break_labels);
    free(flow.continue_labels);
    if (!returned) {
        return CodegenEmitEmptyReturn(code, descriptor);
    }
    return 1;
}

static void CodegenReportFail(const char *where) {
    if (where == NULL) {
        where = "unknown";
    }
    fprintf(stderr, "[CODEGEN ERROR] %s\n", where);
}

static int CodegenReportExprFail(const char *where, const NExpr *expr) {
    fprintf(stderr,
            "[CODEGEN ERROR] %s (line %d, expr type %d)\n",
            where ? where : "expr",
            expr ? expr->line : 0,
            expr ? (int)expr->type : -1);
    return 0;
}

static int CodegenAddMainWithCleanup(jvmc_class *cls, NFuncDef *func, SemanticContext *ctx) {
    jvmc_method *method = NULL;
    jvmc_code *code = NULL;
    int returned = 0;
    FlowContext flow;
    NStmt *prev_body = g_current_body;
    memset(&flow, 0, sizeof(flow));

    if (cls == NULL || func == NULL || func->jvm_descriptor == NULL) {
        CodegenReportFail("CodegenAddMainWithCleanup: invalid args or missing descriptor");
        return 0;
    }
    method = jvmc_class_get_or_create_method(cls, func->func_name, func->jvm_descriptor);
    if (method == NULL) {
        CodegenReportFail("CodegenAddMainWithCleanup: get_or_create_method failed");
        return 0;
    }
    if (!jvmc_method_add_flag(method, JVMC_METHOD_ACC_PUBLIC) ||
        !jvmc_method_add_flag(method, JVMC_METHOD_ACC_STATIC)) {
        CodegenReportFail("CodegenAddMainWithCleanup: add flags failed");
        return 0;
    }
    code = jvmc_method_get_code(method);
    if (code == NULL) {
        CodegenReportFail("CodegenAddMainWithCleanup: get_code failed");
        return 0;
    }
    if (!jvmc_code_set_max_stack(code, 32)) {
        CodegenReportFail("CodegenAddMainWithCleanup: set_max_stack failed");
        return 0;
    }
    if (!jvmc_code_set_max_locals(code, (uint16_t)ComputeMaxLocals(func->params, func->body, 0))) {
        CodegenReportFail("CodegenAddMainWithCleanup: set_max_locals failed");
        return 0;
    }
    g_current_body = func->body;
    ResetCodegenScope();
    PushCodegenScope();
    AddParamsToScope(func->params);
    if (!CodegenEmitStmtList(cls, code, func->body, func->params, &returned, ctx, &flow)) {
        PopCodegenScope();
        ResetCodegenScope();
        g_current_body = prev_body;
        CodegenReportFail("CodegenAddMainWithCleanup: emit stmt list failed");
        free(flow.break_labels);
        free(flow.continue_labels);
        return 0;
    }
    PopCodegenScope();
    ResetCodegenScope();
    g_current_body = prev_body;
    free(flow.break_labels);
    free(flow.continue_labels);
    if (!returned) {
        if (!EmitMainDtorCleanup(cls, code, func->body, ctx)) {
            CodegenReportFail("CodegenAddMainWithCleanup: EmitMainDtorCleanup failed (skipping)");
        }
        if (!CodegenEmitEmptyReturn(code, func->jvm_descriptor)) {
            CodegenReportFail("CodegenAddMainWithCleanup: empty return failed");
            return 0;
        }
        return 1;
    }
    return 1;
}

static int CodegenEmitClinit(jvmc_class *cls, NProgram *root, SemanticContext *ctx) {
    NSourceItem *item;
    jvmc_method *method;
    jvmc_code *code;
    int has_init = 0;
    int has_enum = 0;

    if (cls == NULL || root == NULL) {
        return 0;
    }

    item = root->first_item;
    while (item != NULL) {
        if (item->type == SOURCE_ITEM_DECL && item->value.decl.init_decls) {
            for (int i = 0; i < item->value.decl.init_decls->count; i++) {
                NInitDecl *decl = item->value.decl.init_decls->decls[i];
                if (decl && decl->initializer && decl->initializer->expr) {
                    has_init = 1;
                    break;
                }
            }
        }
        if (has_init) {
            break;
        }
        if (item->type == SOURCE_ITEM_ENUM && item->value.enum_def &&
            item->value.enum_def->items.items != NULL) {
            has_enum = 1;
            break;
        }
        item = item->next;
    }
    if (!has_init && !has_enum) {
        return 1;
    }

    method = jvmc_class_get_or_create_method(cls, "<clinit>", "()V");
    if (method == NULL) {
        return 0;
    }
    if (!jvmc_method_add_flag(method, JVMC_METHOD_ACC_STATIC)) {
        return 0;
    }
    code = jvmc_method_get_code(method);
    if (code == NULL) {
        return 0;
    }

    item = root->first_item;
    while (item != NULL) {
        if (item->type == SOURCE_ITEM_DECL && item->value.decl.init_decls) {
            for (int i = 0; i < item->value.decl.init_decls->count; i++) {
                NInitDecl *decl = item->value.decl.init_decls->decls[i];
                if (decl == NULL || decl->name == NULL ||
                    decl->jvm_descriptor == NULL ||
                    decl->initializer == NULL) {
                    continue;
                }
                if (decl->initializer->is_array) {
                    if (!EmitArrayLiteral(cls, code, item->value.decl.item_type, decl->initializer,
                                          NULL, NULL, ctx, -1, decl->name, decl->jvm_descriptor)) {
                        return 0;
                    }
                } else if (decl->initializer->expr) {
                    if (!CodegenEmitExpr(cls, code, decl->initializer->expr, NULL, NULL, ctx)) {
                        return 0;
                    }
                    {
                        jvmc_fieldref *fref = jvmc_class_get_or_create_fieldref(cls,
                                                                                "Main",
                                                                                decl->name,
                                                                                decl->jvm_descriptor);
                        if (fref == NULL) {
                            return 0;
                        }
                        if (!jvmc_code_putstatic(code, fref)) {
                            return 0;
                        }
                    }
                }
            }
        } else if (item->type == SOURCE_ITEM_ENUM && item->value.enum_def) {
            if (!CodegenEmitEnumValues(cls, code, "Main", item->value.enum_def)) {
                return 0;
            }
        }
        item = item->next;
    }
    return jvmc_code_return_void(code);
}

static NFuncDef *FindGlobalFunction(NProgram *root, const char *name) {
    NSourceItem *item;
    if (root == NULL || name == NULL) {
        return NULL;
    }
    item = root->first_item;
    while (item != NULL) {
        if (item->type == SOURCE_ITEM_FUNC) {
            if (item->value.func && item->value.func->func_name &&
                strcmp(item->value.func->func_name, name) == 0) {
                return item->value.func;
            }
        }
        item = item->next;
    }
    return NULL;
}

static int EmitJavaEntryMain(jvmc_class *cls, int call_noarg_main) {
    jvmc_method *method = NULL;
    jvmc_code *code = NULL;
    jvmc_methodref *mref = NULL;
    const char *desc = "([Ljava/lang/String;)V";
    if (cls == NULL) {
        return 0;
    }
    method = jvmc_class_get_or_create_method(cls, "main", desc);
    if (method == NULL) {
        return 0;
    }
    if (!jvmc_method_add_flag(method, JVMC_METHOD_ACC_PUBLIC) ||
        !jvmc_method_add_flag(method, JVMC_METHOD_ACC_STATIC)) {
        return 0;
    }
    code = jvmc_method_get_code(method);
    if (code == NULL) {
        return 0;
    }
    if (!jvmc_code_set_max_stack(code, 4)) {
        return 0;
    }
    if (!jvmc_code_set_max_locals(code, 1)) {
        return 0;
    }
    if (call_noarg_main) {
        mref = jvmc_class_get_or_create_methodref(cls, "Main", "main", "()V");
        if (mref == NULL) {
            return 0;
        }
        if (!jvmc_code_invokestatic(code, mref)) {
            return 0;
        }
    }
    return jvmc_code_return_void(code);
}

static int CodegenEmitClassFromDef(NClassDef *class_def, SemanticContext *ctx) {
    jvmc_class *cls = NULL;
    NClassMember *member;
    char *class_internal = NULL;
    char *base_internal = NULL;
    char *out_file = NULL;
    size_t out_len = 0;
    int ok = 1;
    int has_ctor = 0;
    int has_enum = 0;
    int has_abstract = 0;

    ClassInfo *prev_class = NULL;

    if (class_def == NULL || class_def->class_name == NULL) {
        CodegenReportFail("CodegenEmitClassFromDef: class_def is NULL");
        return 1;
    }
    if (ctx != NULL) {
        prev_class = ctx->current_class;
        ctx->current_class = LookupClass(ctx, class_def->class_name);
    }

    class_internal = BuildJvmInternalName(class_def->class_name);
    if (class_internal == NULL) {
        CodegenReportFail("CodegenEmitClassFromDef: BuildJvmInternalName(class) failed");
        if (ctx != NULL) ctx->current_class = prev_class;
        return 1;
    }
    if (class_def->base_class_name != NULL) {
        base_internal = BuildJvmInternalName(class_def->base_class_name);
    } else {
        base_internal = BuildJvmInternalName("java/lang/Object");
    }
    if (base_internal == NULL) {
        CodegenReportFail("CodegenEmitClassFromDef: BuildJvmInternalName(base) failed");
        free(class_internal);
        if (ctx != NULL) ctx->current_class = prev_class;
        return 1;
    }

    cls = jvmc_class_create(class_internal, base_internal);
    free(class_internal);
    free(base_internal);
    if (cls == NULL) {
        CodegenReportFail("CodegenEmitClassFromDef: jvmc_class_create failed");
        if (ctx != NULL) ctx->current_class = prev_class;
        return 1;
    }

    ok = jvmc_class_add_flag(cls, JVMC_CLASS_ACC_PUBLIC);
    ok = ok && jvmc_class_add_flag(cls, JVMC_CLASS_ACC_SUPER);
    if (!ok) {
        CodegenReportFail("CodegenEmitClassFromDef: add class flags failed");
        jvmc_class_destroy(cls);
        if (ctx != NULL) ctx->current_class = prev_class;
        return 1;
    }

    member = class_def->members.first;
    while (member != NULL) {
            if (member->type == CLASS_MEMBER_FIELD) {
                uint16_t access = JVMC_FIELD_ACC_PUBLIC;
                if (member->access == ACCESS_PRIVATE) {
                    access = 0;
                } else if (member->access == ACCESS_PROTECTED) {
                    access = JVMC_FIELD_ACC_PROTECTED;
                }
                if (!CodegenAddFieldDecls(cls, member->value.field.init_decls, access)) {
                    CodegenReportFail("CodegenEmitClassFromDef: add field decls failed");
                    jvmc_class_destroy(cls);
                    return 1;
                }
            } else if (member->type == CLASS_MEMBER_METHOD && member->value.method) {
                uint16_t access = CodegenAccessFromSpec(member->access);
                const char *desc = member->value.method->jvm_descriptor;
                if (desc == NULL) {
                    CodegenReportFail("CodegenEmitClassFromDef: method descriptor is NULL");
                    jvmc_class_destroy(cls);
                    return 1;
                }
                if (member->value.method->body != NULL) {
                    if (!CodegenAddMethodWithBody(cls,
                                                  member->value.method->method_name,
                                                  desc,
                                                  access,
                                                  0,
                                                  member->value.method->body,
                                                  member->value.method->params,
                                                  ctx)) {
                        CodegenReportFail("CodegenEmitClassFromDef: add method with body failed");
                        jvmc_class_destroy(cls);
                        return 1;
                    }
                } else {
                    has_abstract = 1;
                    if (!CodegenAddMethodAbstract(cls,
                                                  member->value.method->method_name,
                                                  desc,
                                                  access,
                                                  0)) {
                        CodegenReportFail("CodegenEmitClassFromDef: add abstract method failed");
                        jvmc_class_destroy(cls);
                        return 1;
                    }
                }
            } else if (member->type == CLASS_MEMBER_CTOR && member->value.ctor) {
                has_ctor = 1;
                if (!CodegenEmitCtor(cls, class_def, member->value.ctor, member->access, ctx)) {
                    CodegenReportFail("CodegenEmitClassFromDef: emit ctor failed");
                    jvmc_class_destroy(cls);
                    return 1;
                }
            } else if (member->type == CLASS_MEMBER_DTOR && member->value.dtor) {
                uint16_t access = CodegenAccessFromSpec(member->access);
                if (member->value.dtor->body != NULL) {
                    if (!CodegenAddMethodWithBody(cls,
                                                  "__dtor",
                                                  "()V",
                                                  access,
                                                  0,
                                                  member->value.dtor->body,
                                                  NULL,
                                                  ctx)) {
                        CodegenReportFail("CodegenEmitClassFromDef: add __dtor failed");
                        jvmc_class_destroy(cls);
                        return 1;
                    }
                }
            } else if (member->type == CLASS_MEMBER_ENUM && member->value.enum_def) {
                has_enum = 1;
                if (!CodegenAddEnumFields(cls, member->value.enum_def, JVMC_FIELD_ACC_PUBLIC)) {
                    CodegenReportFail("CodegenEmitClassFromDef: add enum fields failed");
                    jvmc_class_destroy(cls);
                    return 1;
                }
            }
        member = member->next;
    }

    if (!has_ctor) {
        if (!CodegenEmitDefaultCtor(cls, class_def, ctx)) {
            CodegenReportFail("CodegenEmitClassFromDef: emit default ctor failed");
            jvmc_class_destroy(cls);
            return 1;
        }
    }
    if (has_abstract) {
        if (!jvmc_class_add_flag(cls, JVMC_CLASS_ACC_ABSTRACT)) {
            CodegenReportFail("CodegenEmitClassFromDef: add abstract flag failed");
            jvmc_class_destroy(cls);
            return 1;
        }
    }

    if (has_enum) {
        jvmc_method *method = jvmc_class_get_or_create_method(cls, "<clinit>", "()V");
        jvmc_code *code = NULL;
        char *owner_internal = NULL;
        if (method == NULL) {
            CodegenReportFail("CodegenEmitClassFromDef: create <clinit> failed");
            jvmc_class_destroy(cls);
            return 1;
        }
        if (!jvmc_method_add_flag(method, JVMC_METHOD_ACC_STATIC)) {
            CodegenReportFail("CodegenEmitClassFromDef: add <clinit> static flag failed");
            jvmc_class_destroy(cls);
            return 1;
        }
        code = jvmc_method_get_code(method);
        if (code == NULL) {
            CodegenReportFail("CodegenEmitClassFromDef: get <clinit> code failed");
            jvmc_class_destroy(cls);
            return 1;
        }
        owner_internal = BuildJvmInternalName(class_def->class_name);
        if (owner_internal == NULL) {
            CodegenReportFail("CodegenEmitClassFromDef: BuildJvmInternalName(owner) failed");
            jvmc_class_destroy(cls);
            return 1;
        }
        member = class_def->members.first;
        while (member != NULL) {
            if (member->type == CLASS_MEMBER_ENUM && member->value.enum_def) {
                if (!CodegenEmitEnumValues(cls, code, owner_internal, member->value.enum_def)) {
                    CodegenReportFail("CodegenEmitClassFromDef: emit enum values failed");
                    free(owner_internal);
                    jvmc_class_destroy(cls);
                    return 1;
                }
            }
            member = member->next;
        }
        free(owner_internal);
        if (!jvmc_code_return_void(code)) {
            CodegenReportFail("CodegenEmitClassFromDef: <clinit> return failed");
            jvmc_class_destroy(cls);
            return 1;
        }
    }

    out_len = strlen(class_def->class_name) + 7;
    out_file = (char *)malloc(out_len);
    if (out_file == NULL) {
        CodegenReportFail("CodegenEmitClassFromDef: out_file alloc failed");
        jvmc_class_destroy(cls);
        if (ctx != NULL) ctx->current_class = prev_class;
        return 1;
    }
    snprintf(out_file, out_len, "%s.class", class_def->class_name);
    if (!jvmc_class_write_to_file(cls, out_file)) {
        CodegenReportFail("CodegenEmitClassFromDef: write class file failed");
        free(out_file);
        jvmc_class_destroy(cls);
        if (ctx != NULL) ctx->current_class = prev_class;
        return 1;
    }
    free(out_file);
    jvmc_class_destroy(cls);
    return 0;
}

int GenerateClassFiles(NProgram *root, SemanticContext *ctx) {
    const char *class_name = "Main";
    const char *out_file = "Main.class";
    jvmc_class *cls = NULL;
    int ok = 1;
    NFuncDef *main_func = NULL;
    const char *main_desc = NULL;
    int need_java_entry = 1;

    (void)root;
    (void)ctx;

    jvmc_set_class_version(49, 0);

    cls = jvmc_class_create(class_name, "java/lang/Object");
    if (cls == NULL) {
        CodegenReportFail("GenerateClassFiles: jvmc_class_create(Main) failed");
        return 1;
    }
    ok = jvmc_class_add_flag(cls, JVMC_CLASS_ACC_PUBLIC);
    ok = ok && jvmc_class_add_flag(cls, JVMC_CLASS_ACC_SUPER);
    if (!ok) {
        CodegenReportFail("GenerateClassFiles: add Main class flags failed");
        jvmc_class_destroy(cls);
        return 1;
    }

    main_func = FindGlobalFunction(root, "main");
    if (main_func != NULL) {
        main_desc = main_func->jvm_descriptor;
    }
    if (main_desc != NULL && strcmp(main_desc, "([Ljava/lang/String;)V") == 0) {
        need_java_entry = 0;
    }

    if (root != NULL) {
        NSourceItem *item = root->first_item;
        while (item != NULL) {
            switch (item->type) {
                case SOURCE_ITEM_DECL:
                    if (!CodegenAddFieldDecls(cls, item->value.decl.init_decls,
                                              (uint16_t)(JVMC_FIELD_ACC_PUBLIC | JVMC_FIELD_ACC_STATIC))) {
                        CodegenReportFail("GenerateClassFiles: add global fields failed");
                        jvmc_class_destroy(cls);
                        return 1;
                    }
                    break;
                case SOURCE_ITEM_CLASS: {
                    if (item->value.class_def != NULL) {
                        if (CodegenEmitClassFromDef(item->value.class_def, ctx) != 0) {
                            CodegenReportFail("GenerateClassFiles: emit class failed");
                            jvmc_class_destroy(cls);
                            return 1;
                        }
                    }
                    break;
                }
                case SOURCE_ITEM_FUNC:
                    if (item->value.func && item->value.func->jvm_descriptor) {
                        if (item->value.func->body != NULL) {
                            if (item->value.func->func_name != NULL &&
                                strcmp(item->value.func->func_name, "main") == 0) {
                                if (!CodegenAddMainWithCleanup(cls, item->value.func, ctx)) {
                                    CodegenReportFail("GenerateClassFiles: add main failed");
                                    jvmc_class_destroy(cls);
                                    return 1;
                                }
                            } else {
                                if (!CodegenAddMethodWithBody(cls,
                                                              item->value.func->func_name,
                                                              item->value.func->jvm_descriptor,
                                                              JVMC_METHOD_ACC_PUBLIC,
                                                              1,
                                                              item->value.func->body,
                                                              item->value.func->params,
                                                              ctx)) {
                                    CodegenReportFail("GenerateClassFiles: add function failed");
                                    jvmc_class_destroy(cls);
                                    return 1;
                                }
                            }
                        } else if (!CodegenAddMethodNative(cls,
                                                           item->value.func->func_name,
                                                           item->value.func->jvm_descriptor,
                                                           JVMC_METHOD_ACC_PUBLIC,
                                                           1)) {
                            CodegenReportFail("GenerateClassFiles: add native prototype failed");
                            jvmc_class_destroy(cls);
                            return 1;
                        }
                    }
                    break;
                case SOURCE_ITEM_ENUM:
                    if (!CodegenAddEnumFields(cls, item->value.enum_def, JVMC_FIELD_ACC_PUBLIC)) {
                        CodegenReportFail("GenerateClassFiles: add enum fields failed");
                        jvmc_class_destroy(cls);
                        return 1;
                    }
                    break;
            }
            item = item->next;
        }
    }

    if (need_java_entry) {
        int call_noarg_main = 0;
        if (main_desc != NULL && strcmp(main_desc, "()V") == 0) {
            call_noarg_main = 1;
        }
        if (!EmitJavaEntryMain(cls, call_noarg_main)) {
            CodegenReportFail("GenerateClassFiles: emit Java entry main failed");
            jvmc_class_destroy(cls);
            return 1;
        }
    }
    if (!CodegenEmitClinit(cls, root, ctx)) {
        CodegenReportFail("GenerateClassFiles: emit <clinit> failed");
        jvmc_class_destroy(cls);
        return 1;
    }

    if (!jvmc_class_write_to_file(cls, out_file)) {
        CodegenReportFail("GenerateClassFiles: write Main.class failed");
        jvmc_class_destroy(cls);
        return 1;
    }

    jvmc_class_destroy(cls);
    return 0;
}
