#include "codegen_jvm.h"
#include "jvmc/jvmc.h"

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

    if (!jvmc_class_write_to_file(cls, out_file)) {
        jvmc_class_destroy(cls);
        return 1;
    }

    jvmc_class_destroy(cls);
    return 0;
}
