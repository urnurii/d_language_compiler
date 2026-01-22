#include "codegen_jvm.h"
#include "jvmc/jvmc.h"
#include "semantic/jvm_layout.h"
#include <string.h>
#include <stdlib.h>

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
            return JVMC_METHOD_ACC_PRIVATE;
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
    return CodegenEmitEmptyReturn(code, descriptor);
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
            if (stmt->value.for_stmt.init_decls != NULL && stmt->value.for_stmt.init_decl_type != NULL) {
                for (int i = 0; i < stmt->value.for_stmt.init_decls->count; i++) {
                    NInitDecl *decl = stmt->value.for_stmt.init_decls->decls[i];
                    if (decl && decl->name && strcmp(decl->name, name) == 0) {
                        if (slot_out) *slot_out = decl->jvm_slot_index;
                        if (type_out) *type_out = stmt->value.for_stmt.init_decl_type;
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
        return ResolveVariableSlotInStmt(body, name, slot_out, type_out);
    }
    return 0;
}

static int EmitLoadByType(jvmc_code *code, const NType *type, int slot) {
    if (type == NULL) {
        return 0;
    }
    if (type->kind == TYPE_KIND_CLASS || type->kind == TYPE_KIND_CLASS_ARRAY) {
        return jvmc_code_load_ref(code, (uint16_t)slot);
    }
    if (type->kind == TYPE_KIND_BASE || type->kind == TYPE_KIND_BASE_ARRAY) {
        switch (type->base_type) {
            case TYPE_DOUBLE:
            case TYPE_REAL:
                return jvmc_code_load_double(code, (uint16_t)slot);
            case TYPE_FLOAT:
                return jvmc_code_load_float(code, (uint16_t)slot);
            case TYPE_BOOL:
            case TYPE_CHAR:
            case TYPE_INT:
                return jvmc_code_load_int(code, (uint16_t)slot);
            case TYPE_STRING:
                return jvmc_code_load_ref(code, (uint16_t)slot);
            default:
                return jvmc_code_load_ref(code, (uint16_t)slot);
        }
    }
    return 0;
}

static int EmitStoreByType(jvmc_code *code, const NType *type, int slot) {
    if (type == NULL) {
        return 0;
    }
    if (type->kind == TYPE_KIND_CLASS || type->kind == TYPE_KIND_CLASS_ARRAY) {
        return jvmc_code_store_ref(code, (uint16_t)slot);
    }
    if (type->kind == TYPE_KIND_BASE || type->kind == TYPE_KIND_BASE_ARRAY) {
        switch (type->base_type) {
            case TYPE_DOUBLE:
            case TYPE_REAL:
                return jvmc_code_store_double(code, (uint16_t)slot);
            case TYPE_FLOAT:
                return jvmc_code_store_float(code, (uint16_t)slot);
            case TYPE_BOOL:
            case TYPE_CHAR:
            case TYPE_INT:
                return jvmc_code_store_int(code, (uint16_t)slot);
            case TYPE_STRING:
                return jvmc_code_store_ref(code, (uint16_t)slot);
            default:
                return jvmc_code_store_ref(code, (uint16_t)slot);
        }
    }
    return 0;
}

static int EmitReturnByType(jvmc_code *code, const NType *type) {
    if (type == NULL) {
        return jvmc_code_return_void(code);
    }
    if (type->kind == TYPE_KIND_CLASS || type->kind == TYPE_KIND_CLASS_ARRAY) {
        return jvmc_code_return_ref(code);
    }
    if (type->kind == TYPE_KIND_BASE || type->kind == TYPE_KIND_BASE_ARRAY) {
        switch (type->base_type) {
            case TYPE_DOUBLE:
            case TYPE_REAL:
                return jvmc_code_return_double(code);
            case TYPE_FLOAT:
                return jvmc_code_return_float(code);
            case TYPE_BOOL:
            case TYPE_CHAR:
            case TYPE_INT:
                return jvmc_code_return_int(code);
            case TYPE_STRING:
                return jvmc_code_return_ref(code);
            case TYPE_VOID:
                return jvmc_code_return_void(code);
            default:
                return jvmc_code_return_ref(code);
        }
    }
    return jvmc_code_return_ref(code);
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

static int CodegenEmitExpr(jvmc_class *cls, jvmc_code *code, NExpr *expr, NParamList *params, NStmt *body) {
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
                return EmitLoadByType(code, type, slot);
            }
            return 0;
        case EXPR_FUNC_CALL: {
            for (int i = 0; i < expr->value.func_call.arg_count; i++) {
                if (!CodegenEmitExpr(cls, code, expr->value.func_call.args[i], params, body)) {
                    return 0;
                }
            }
            return EmitInvokeFromRefKey(cls, code, &expr->jvm_ref_key, 1);
        }
        case EXPR_METHOD_CALL: {
            if (!CodegenEmitExpr(cls, code, expr->value.member_access.object, params, body)) {
                return 0;
            }
            for (int i = 0; i < expr->value.member_access.arg_count; i++) {
                if (!CodegenEmitExpr(cls, code, expr->value.member_access.args[i], params, body)) {
                    return 0;
                }
            }
            return EmitInvokeFromRefKey(cls, code, &expr->jvm_ref_key, 0);
        }
        case EXPR_ASSIGN:
            if (expr->value.binary.left != NULL &&
                expr->value.binary.left->type == EXPR_IDENT) {
                const char *name = expr->value.binary.left->value.ident_name;
                NExpr *right = expr->value.binary.right;
                if (right == NULL) {
                    return 0;
                }
                if (!CodegenEmitExpr(cls, code, right, params, body)) {
                    return 0;
                }
                if (ResolveVariableSlot(name, params, body, &slot, &type)) {
                    return EmitStoreByType(code, type, slot);
                }
            }
            return 0;
        default:
            return 0;
    }
}

static int CodegenEmitStmtList(jvmc_class *cls, jvmc_code *code, NStmt *stmts, NParamList *params, int *return_emitted) {
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
            if (!CodegenEmitExpr(cls, code, ret_expr, params, stmts)) {
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
                    if (decl && decl->initializer && decl->initializer->expr) {
                        if (!CodegenEmitExpr(cls, code, decl->initializer->expr, params, stmts)) {
                            return 0;
                        }
                        if (!EmitStoreByType(code, stmt->value.decl.decl_type, decl->jvm_slot_index)) {
                            return 0;
                        }
                    }
                }
            }
        } else if (stmt->type == STMT_EXPR) {
            if (stmt->value.expr != NULL) {
                if (!CodegenEmitExpr(cls, code, stmt->value.expr, params, stmts)) {
                    return 0;
                }
            }
        } else if (stmt->type == STMT_COMPOUND) {
            if (!CodegenEmitStmtList(cls, code,
                                     stmt->value.stmt_list ? stmt->value.stmt_list->first : NULL,
                                     params,
                                     return_emitted)) {
                return 0;
            }
            if (return_emitted && *return_emitted) {
                return 1;
            }
        }
        stmt = stmt->next;
    }
    return 1;
}

static int CodegenAddMethodWithBody(jvmc_class *cls, const char *name, const char *descriptor,
                                    uint16_t access_flags, int is_static,
                                    NStmt *body, NParamList *params) {
    jvmc_method *method = NULL;
    jvmc_code *code = NULL;
    int returned = 0;

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
    if (!CodegenEmitStmtList(cls, code, body, params, &returned)) {
        return 0;
    }
    if (!returned) {
        return CodegenEmitEmptyReturn(code, descriptor);
    }
    return 1;
}

int GenerateClassFiles(NProgram *root, SemanticContext *ctx) {
    const char *class_name = "Main";
    const char *out_file = "Main.class";
    jvmc_class *cls = NULL;
    int ok = 1;

    (void)root;
    (void)ctx;

    cls = jvmc_class_create(class_name, "java/lang/Object");
    if (cls == NULL) {
        return 1;
    }
    ok = jvmc_class_add_flag(cls, JVMC_CLASS_ACC_PUBLIC);
    ok = ok && jvmc_class_add_flag(cls, JVMC_CLASS_ACC_SUPER);
    if (!ok) {
        jvmc_class_destroy(cls);
        return 1;
    }

    {
        const char *main_desc = "([Ljava/lang/String;)V";
        if (!CodegenAddMethodStub(cls, "main", main_desc, JVMC_METHOD_ACC_PUBLIC, 1)) {
            jvmc_class_destroy(cls);
            return 1;
        }
    }

    if (root != NULL) {
        NSourceItem *item = root->first_item;
        while (item != NULL) {
            switch (item->type) {
                case SOURCE_ITEM_DECL:
                    if (!CodegenAddFieldDecls(cls, item->value.decl.init_decls, JVMC_FIELD_ACC_PUBLIC)) {
                        jvmc_class_destroy(cls);
                        return 1;
                    }
                    break;
                case SOURCE_ITEM_CLASS: {
                    NClassDef *class_def = item->value.class_def;
                    NClassMember *member = class_def ? class_def->members.first : NULL;
                    while (member != NULL) {
                        if (member->type == CLASS_MEMBER_FIELD) {
                            uint16_t access = JVMC_FIELD_ACC_PUBLIC;
                            if (member->access == ACCESS_PRIVATE) {
                                access = JVMC_FIELD_ACC_PRIVATE;
                            } else if (member->access == ACCESS_PROTECTED) {
                                access = JVMC_FIELD_ACC_PROTECTED;
                            }
                            if (!CodegenAddFieldDecls(cls, member->value.field.init_decls, access)) {
                                jvmc_class_destroy(cls);
                                return 1;
                            }
                        } else if (member->type == CLASS_MEMBER_METHOD && member->value.method) {
                            uint16_t access = CodegenAccessFromSpec(member->access);
                            const char *desc = member->value.method->jvm_descriptor;
                            if (desc == NULL) {
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
                                                              member->value.method->params)) {
                                    jvmc_class_destroy(cls);
                                    return 1;
                                }
                            } else if (!CodegenAddMethodStub(cls,
                                                             member->value.method->method_name,
                                                             desc,
                                                             access,
                                                             0)) {
                                jvmc_class_destroy(cls);
                                return 1;
                            }
                        } else if (member->type == CLASS_MEMBER_CTOR && member->value.ctor) {
                            NClassDef *owner_class = class_def;
                            const char *ctor_desc = NULL;
                            if (owner_class == NULL || owner_class->class_name == NULL) {
                                jvmc_class_destroy(cls);
                                return 1;
                            }
                            ctor_desc = CodegenBuildMethodDescriptor(NULL, member->value.ctor->params);
                            if (ctor_desc == NULL) {
                                jvmc_class_destroy(cls);
                                return 1;
                            }
                            if (member->value.ctor->body != NULL) {
                                if (!CodegenAddMethodWithBody(cls, "<init>", ctor_desc,
                                                              CodegenAccessFromSpec(member->access), 0,
                                                              member->value.ctor->body,
                                                              member->value.ctor->params)) {
                                    free((void *)ctor_desc);
                                    jvmc_class_destroy(cls);
                                    return 1;
                                }
                            } else if (!CodegenAddMethodStub(cls, "<init>", ctor_desc,
                                                             CodegenAccessFromSpec(member->access), 0)) {
                                free((void *)ctor_desc);
                                jvmc_class_destroy(cls);
                                return 1;
                            }
                            free((void *)ctor_desc);
                        }
                        member = member->next;
                    }
                    break;
                }
                case SOURCE_ITEM_FUNC:
                    if (item->value.func && item->value.func->jvm_descriptor) {
                        if (item->value.func->body != NULL) {
                            if (!CodegenAddMethodWithBody(cls,
                                                          item->value.func->func_name,
                                                          item->value.func->jvm_descriptor,
                                                          JVMC_METHOD_ACC_PUBLIC,
                                                          1,
                                                          item->value.func->body,
                                                          item->value.func->params)) {
                                jvmc_class_destroy(cls);
                                return 1;
                            }
                        } else if (!CodegenAddMethodStub(cls,
                                                         item->value.func->func_name,
                                                         item->value.func->jvm_descriptor,
                                                         JVMC_METHOD_ACC_PUBLIC,
                                                         1)) {
                            jvmc_class_destroy(cls);
                            return 1;
                        }
                    }
                    break;
                case SOURCE_ITEM_ENUM:
                    break;
            }
            item = item->next;
        }
    }

    if (!jvmc_class_write_to_file(cls, out_file)) {
        jvmc_class_destroy(cls);
        return 1;
    }

    jvmc_class_destroy(cls);
    return 0;
}
