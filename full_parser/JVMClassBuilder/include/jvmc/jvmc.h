#ifndef JVMC_H
#define JVMC_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct jvmc_class jvmc_class;
typedef struct jvmc_method jvmc_method;
typedef struct jvmc_field jvmc_field;
typedef struct jvmc_code jvmc_code;
typedef struct jvmc_label jvmc_label;
typedef struct jvmc_class_ref jvmc_class_ref;
typedef struct jvmc_fieldref jvmc_fieldref;
typedef struct jvmc_methodref jvmc_methodref;
typedef struct jvmc_iface_methodref jvmc_iface_methodref;

typedef enum {
    JVMC_CLASS_ACC_PUBLIC = 0x0001,
    JVMC_CLASS_ACC_FINAL = 0x0010,
    JVMC_CLASS_ACC_SUPER = 0x0020,
    JVMC_CLASS_ACC_INTERFACE = 0x0200,
    JVMC_CLASS_ACC_ABSTRACT = 0x0400,
    JVMC_CLASS_ACC_SYNTHETIC = 0x1000,
    JVMC_CLASS_ACC_ANNOTATION = 0x2000,
    JVMC_CLASS_ACC_ENUM = 0x4000,
    JVMC_CLASS_ACC_MODULE = 0x8000
} jvmc_class_access;

typedef enum {
    JVMC_METHOD_ACC_PUBLIC = 0x0001,
    JVMC_METHOD_ACC_PRIVATE = 0x0002,
    JVMC_METHOD_ACC_PROTECTED = 0x0004,
    JVMC_METHOD_ACC_STATIC = 0x0008,
    JVMC_METHOD_ACC_FINAL = 0x0010,
    JVMC_METHOD_ACC_SYNCHRONIZED = 0x0020,
    JVMC_METHOD_ACC_BRIDGE = 0x0040,
    JVMC_METHOD_ACC_VARARGS = 0x0080,
    JVMC_METHOD_ACC_NATIVE = 0x0100,
    JVMC_METHOD_ACC_ABSTRACT = 0x0400,
    JVMC_METHOD_ACC_STRICT = 0x0800,
    JVMC_METHOD_ACC_SYNTHETIC = 0x1000
} jvmc_method_access;

typedef enum {
    JVMC_FIELD_ACC_PUBLIC = 0x0001,
    JVMC_FIELD_ACC_PRIVATE = 0x0002,
    JVMC_FIELD_ACC_PROTECTED = 0x0004,
    JVMC_FIELD_ACC_STATIC = 0x0008,
    JVMC_FIELD_ACC_FINAL = 0x0010,
    JVMC_FIELD_ACC_VOLATILE = 0x0040,
    JVMC_FIELD_ACC_TRANSIENT = 0x0080,
    JVMC_FIELD_ACC_SYNTHETIC = 0x1000,
    JVMC_FIELD_ACC_ENUM = 0x4000
} jvmc_field_access;

typedef enum {
    JVMC_NEWARRAY_BOOLEAN = 4,
    JVMC_NEWARRAY_CHAR = 5,
    JVMC_NEWARRAY_FLOAT = 6,
    JVMC_NEWARRAY_DOUBLE = 7,
    JVMC_NEWARRAY_BYTE = 8,
    JVMC_NEWARRAY_SHORT = 9,
    JVMC_NEWARRAY_INT = 10,
    JVMC_NEWARRAY_LONG = 11
} jvmc_newarray_type;

typedef enum {
    JVMC_CMP_EQ,
    JVMC_CMP_NE,
    JVMC_CMP_LT,
    JVMC_CMP_LE,
    JVMC_CMP_GT,
    JVMC_CMP_GE
} jvmc_compare;

jvmc_class *jvmc_class_create(const char *internal_name, const char *super_internal_name);
void jvmc_class_destroy(jvmc_class *cls);
int jvmc_class_add_flag(jvmc_class *cls, uint16_t flag);
int jvmc_class_write_to_file(jvmc_class *cls, const char *path);

jvmc_method *jvmc_class_get_or_create_method(jvmc_class *cls, const char *name, const char *descriptor);
jvmc_field *jvmc_class_get_or_create_field(jvmc_class *cls, const char *name, const char *descriptor);

int jvmc_method_add_flag(jvmc_method *method, uint16_t flag);
int jvmc_field_add_flag(jvmc_field *field, uint16_t flag);
jvmc_code *jvmc_method_get_code(jvmc_method *method);

jvmc_class_ref *jvmc_class_get_or_create_class_ref(jvmc_class *cls, const char *internal_name);
jvmc_fieldref *jvmc_class_get_or_create_fieldref(jvmc_class *cls, const char *owner_internal,
                                                 const char *name, const char *descriptor);
jvmc_methodref *jvmc_class_get_or_create_methodref(jvmc_class *cls, const char *owner_internal,
                                                   const char *name, const char *descriptor);
jvmc_iface_methodref *jvmc_class_get_or_create_iface_methodref(jvmc_class *cls, const char *owner_internal,
                                                               const char *name, const char *descriptor);

int jvmc_code_push_null(jvmc_code *code);
int jvmc_code_push_int(jvmc_code *code, int32_t value);
int jvmc_code_push_long(jvmc_code *code, int64_t value);
int jvmc_code_push_float(jvmc_code *code, float value);
int jvmc_code_push_double(jvmc_code *code, double value);
int jvmc_code_push_string(jvmc_code *code, const char *value);

int jvmc_code_load_int(jvmc_code *code, uint16_t index);
int jvmc_code_load_long(jvmc_code *code, uint16_t index);
int jvmc_code_load_float(jvmc_code *code, uint16_t index);
int jvmc_code_load_double(jvmc_code *code, uint16_t index);
int jvmc_code_load_ref(jvmc_code *code, uint16_t index);

int jvmc_code_store_int(jvmc_code *code, uint16_t index);
int jvmc_code_store_long(jvmc_code *code, uint16_t index);
int jvmc_code_store_float(jvmc_code *code, uint16_t index);
int jvmc_code_store_double(jvmc_code *code, uint16_t index);
int jvmc_code_store_ref(jvmc_code *code, uint16_t index);

int jvmc_code_add_int(jvmc_code *code);
int jvmc_code_sub_int(jvmc_code *code);
int jvmc_code_mul_int(jvmc_code *code);
int jvmc_code_div_int(jvmc_code *code);
int jvmc_code_i2f(jvmc_code *code);
int jvmc_code_i2d(jvmc_code *code);
int jvmc_code_f2i(jvmc_code *code);
int jvmc_code_f2d(jvmc_code *code);
int jvmc_code_d2i(jvmc_code *code);
int jvmc_code_d2f(jvmc_code *code);

int jvmc_code_dup(jvmc_code *code);
int jvmc_code_pop(jvmc_code *code);
int jvmc_code_pop2(jvmc_code *code);

int jvmc_code_return_void(jvmc_code *code);
int jvmc_code_return_int(jvmc_code *code);
int jvmc_code_return_long(jvmc_code *code);
int jvmc_code_return_float(jvmc_code *code);
int jvmc_code_return_double(jvmc_code *code);
int jvmc_code_return_ref(jvmc_code *code);

int jvmc_code_getstatic(jvmc_code *code, jvmc_fieldref *field);
int jvmc_code_putstatic(jvmc_code *code, jvmc_fieldref *field);
int jvmc_code_getfield(jvmc_code *code, jvmc_fieldref *field);
int jvmc_code_putfield(jvmc_code *code, jvmc_fieldref *field);

int jvmc_code_invokevirtual(jvmc_code *code, jvmc_methodref *method);
int jvmc_code_invokespecial(jvmc_code *code, jvmc_methodref *method);
int jvmc_code_invokestatic(jvmc_code *code, jvmc_methodref *method);
int jvmc_code_invokeinterface(jvmc_code *code, jvmc_iface_methodref *method);

int jvmc_code_new(jvmc_code *code, jvmc_class_ref *class_ref);
int jvmc_code_newarray_primitive(jvmc_code *code, uint8_t type);
int jvmc_code_newarray_ref(jvmc_code *code, jvmc_class_ref *class_ref);
int jvmc_code_array_length(jvmc_code *code);
int jvmc_code_array_load_bool(jvmc_code *code);
int jvmc_code_array_load_int(jvmc_code *code);
int jvmc_code_array_load_float(jvmc_code *code);
int jvmc_code_array_load_double(jvmc_code *code);
int jvmc_code_array_load_ref(jvmc_code *code);
int jvmc_code_array_store_bool(jvmc_code *code);
int jvmc_code_array_store_int(jvmc_code *code);
int jvmc_code_array_store_float(jvmc_code *code);
int jvmc_code_array_store_double(jvmc_code *code);
int jvmc_code_array_store_ref(jvmc_code *code);

jvmc_label *jvmc_code_label_create(jvmc_code *code);
int jvmc_code_label_place(jvmc_code *code, jvmc_label *label);
int jvmc_code_goto(jvmc_code *code, jvmc_label *label);
int jvmc_code_if(jvmc_code *code, jvmc_compare cmp, jvmc_label *label);
int jvmc_code_if_icmp(jvmc_code *code, jvmc_compare cmp, jvmc_label *label);
int jvmc_code_if_null(jvmc_code *code, jvmc_label *label);
int jvmc_code_if_not_null(jvmc_code *code, jvmc_label *label);

int jvmc_code_checkcast(jvmc_code *code, jvmc_class_ref *class_ref);

int jvmc_code_set_max_stack(jvmc_code *code, uint16_t value);
int jvmc_code_set_max_locals(jvmc_code *code, uint16_t value);

#ifdef __cplusplus
}
#endif

#endif
