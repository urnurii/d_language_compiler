#include "codegen_jvm.h"
#include "jvmc/jvmc.h"
#include "semantic/jvm_layout.h"

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
        jvmc_method *main_method = jvmc_class_get_or_create_method(cls, "main", main_desc);
        jvmc_code *code = NULL;
        if (main_method == NULL) {
            jvmc_class_destroy(cls);
            return 1;
        }
        ok = jvmc_method_add_flag(main_method, JVMC_METHOD_ACC_PUBLIC);
        ok = ok && jvmc_method_add_flag(main_method, JVMC_METHOD_ACC_STATIC);
        if (!ok) {
            jvmc_class_destroy(cls);
            return 1;
        }
        code = jvmc_method_get_code(main_method);
        if (code == NULL) {
            jvmc_class_destroy(cls);
            return 1;
        }
        if (!jvmc_code_return_void(code)) {
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
                        }
                        member = member->next;
                    }
                    break;
                }
                case SOURCE_ITEM_FUNC:
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
