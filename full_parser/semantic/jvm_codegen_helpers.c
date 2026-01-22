#include "jvm_codegen_helpers.h"

int JvmEmitLoadByType(jvmc_code *code, const NType *type, int slot) {
    if (type == NULL) {
        return 0;
    }
    if (type->kind == TYPE_KIND_CLASS || type->kind == TYPE_KIND_CLASS_ARRAY ||
        type->kind == TYPE_KIND_BASE_ARRAY) {
        return jvmc_code_load_ref(code, (uint16_t)slot);
    }
    if (type->kind == TYPE_KIND_BASE) {
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

int JvmEmitStoreByType(jvmc_code *code, const NType *type, int slot) {
    if (type == NULL) {
        return 0;
    }
    if (type->kind == TYPE_KIND_CLASS || type->kind == TYPE_KIND_CLASS_ARRAY ||
        type->kind == TYPE_KIND_BASE_ARRAY) {
        return jvmc_code_store_ref(code, (uint16_t)slot);
    }
    if (type->kind == TYPE_KIND_BASE) {
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

int JvmEmitReturnByType(jvmc_code *code, const NType *type) {
    if (type == NULL) {
        return jvmc_code_return_void(code);
    }
    if (type->kind == TYPE_KIND_CLASS || type->kind == TYPE_KIND_CLASS_ARRAY ||
        type->kind == TYPE_KIND_BASE_ARRAY) {
        return jvmc_code_return_ref(code);
    }
    if (type->kind == TYPE_KIND_BASE) {
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

int JvmEmitNumericCast(jvmc_code *code, const NType *from, const NType *to) {
    if (code == NULL || from == NULL || to == NULL) {
        return 0;
    }
    if (from->kind != TYPE_KIND_BASE || to->kind != TYPE_KIND_BASE) {
        return 1;
    }
    if (from->base_type == to->base_type) {
        return 1;
    }
    switch (to->base_type) {
        case TYPE_FLOAT:
            switch (from->base_type) {
                case TYPE_INT:
                case TYPE_CHAR:
                    return jvmc_code_i2f(code);
                case TYPE_DOUBLE:
                case TYPE_REAL:
                    return jvmc_code_d2f(code);
                default:
                    return 1;
            }
        case TYPE_DOUBLE:
        case TYPE_REAL:
            switch (from->base_type) {
                case TYPE_INT:
                case TYPE_CHAR:
                    return jvmc_code_i2d(code);
                case TYPE_FLOAT:
                    return jvmc_code_f2d(code);
                default:
                    return 1;
            }
        case TYPE_INT:
        case TYPE_CHAR:
            switch (from->base_type) {
                case TYPE_FLOAT:
                    return jvmc_code_f2i(code);
                case TYPE_DOUBLE:
                case TYPE_REAL:
                    return jvmc_code_d2i(code);
                default:
                    return 1;
            }
        default:
            return 1;
    }
}

int JvmEmitCompareIntResult(jvmc_code *code, jvmc_compare cmp) {
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
    if (!jvmc_code_if_icmp(code, cmp, label_true)) {
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

const char *JvmPrintDescriptorForType(const NType *type) {
    if (type == NULL) {
        return "Ljava/lang/Object;";
    }
    if (type->kind == TYPE_KIND_CLASS || type->kind == TYPE_KIND_CLASS_ARRAY) {
        return "Ljava/lang/Object;";
    }
    if (type->kind == TYPE_KIND_BASE || type->kind == TYPE_KIND_BASE_ARRAY) {
        switch (type->base_type) {
            case TYPE_BOOL: return "Z";
            case TYPE_CHAR: return "C";
            case TYPE_INT: return "I";
            case TYPE_FLOAT: return "F";
            case TYPE_DOUBLE: return "D";
            case TYPE_REAL: return "D";
            case TYPE_STRING: return "Ljava/lang/String;";
            default: return "Ljava/lang/Object;";
        }
    }
    return "Ljava/lang/Object;";
}
