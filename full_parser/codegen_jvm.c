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
                            if (!CodegenAddMethodStub(cls,
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
                            if (!CodegenAddMethodStub(cls, "<init>", ctor_desc, CodegenAccessFromSpec(member->access), 0)) {
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
                        if (!CodegenAddMethodStub(cls,
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
