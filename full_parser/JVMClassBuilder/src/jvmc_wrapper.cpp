#include "jvmc/jvmc.h"

#include <fstream>
#include <new>
#include <string>

#include "jvm/attribute-code.h"
#include "jvm/class.h"
#include "jvm/constant-class.h"
#include "jvm/constant-fieldref.h"
#include "jvm/constant-interface-methodref.h"
#include "jvm/constant-methodref.h"
#include "jvm/constant-name-and-type.h"
#include "jvm/constant-utf-8-info.h"
#include "jvm/field.h"
#include "jvm/label.h"
#include "jvm/method.h"

struct jvmc_class { jvm::Class *impl; };
struct jvmc_method { jvm::Method *impl; };
struct jvmc_field { jvm::Field *impl; };
struct jvmc_code { jvm::AttributeCode *impl; };
struct jvmc_label { jvm::Label *impl; };
struct jvmc_class_ref { jvm::ConstantClass *impl; };
struct jvmc_fieldref { jvm::ConstantFieldref *impl; };
struct jvmc_methodref { jvm::ConstantMethodref *impl; };
struct jvmc_iface_methodref { jvm::ConstantInterfaceMethodref *impl; };

static jvm::Instruction::Compare ToCompare(jvmc_compare cmp) {
    switch (cmp) {
        case JVMC_CMP_EQ: return jvm::Instruction::Equal;
        case JVMC_CMP_NE: return jvm::Instruction::NotEqual;
        case JVMC_CMP_LT: return jvm::Instruction::LessThan;
        case JVMC_CMP_LE: return jvm::Instruction::LessEqual;
        case JVMC_CMP_GT: return jvm::Instruction::GreaterThan;
        case JVMC_CMP_GE: return jvm::Instruction::GreaterEqual;
        default: return jvm::Instruction::Equal;
    }
}

static int AddInstruction(jvm::AttributeCode *code, jvm::Instruction *inst) {
    if (code == nullptr || inst == nullptr) {
        return 0;
    }
    code->addInstruction(inst);
    return 1;
}

extern "C" {

jvmc_class *jvmc_class_create(const char *internal_name, const char *super_internal_name) {
    try {
        const char *super_name = (super_internal_name && super_internal_name[0])
            ? super_internal_name
            : "java/lang/Object";
        auto *cls = new jvm::Class(internal_name ? internal_name : "Unknown", super_name);
        auto *wrap = new jvmc_class{cls};
        return wrap;
    } catch (...) {
        return nullptr;
    }
}

void jvmc_class_destroy(jvmc_class *cls) {
    if (cls == nullptr) {
        return;
    }
    delete cls->impl;
    delete cls;
}

int jvmc_class_add_flag(jvmc_class *cls, uint16_t flag) {
    if (cls == nullptr || cls->impl == nullptr) {
        return 0;
    }
    try {
        cls->impl->addFlag(static_cast<jvm::Class::AccessFlag>(flag));
        return 1;
    } catch (...) {
        return 0;
    }
}

int jvmc_class_write_to_file(jvmc_class *cls, const char *path) {
    if (cls == nullptr || cls->impl == nullptr || path == nullptr) {
        return 0;
    }
    try {
        std::ofstream out(path, std::ios::binary);
        if (!out) {
            return 0;
        }
        cls->impl->writeTo(out);
        out.close();
        return 1;
    } catch (...) {
        return 0;
    }
}

jvmc_method *jvmc_class_get_or_create_method(jvmc_class *cls, const char *name, const char *descriptor) {
    if (cls == nullptr || cls->impl == nullptr || name == nullptr || descriptor == nullptr) {
        return nullptr;
    }
    try {
        auto *desc_const = cls->impl->getOrCreateUtf8Constant(descriptor);
        auto *method = cls->impl->getOrCreateMethod(name, desc_const);
        return new jvmc_method{method};
    } catch (...) {
        return nullptr;
    }
}

jvmc_field *jvmc_class_get_or_create_field(jvmc_class *cls, const char *name, const char *descriptor) {
    if (cls == nullptr || cls->impl == nullptr || name == nullptr || descriptor == nullptr) {
        return nullptr;
    }
    try {
        auto *desc_const = cls->impl->getOrCreateUtf8Constant(descriptor);
        auto *field = cls->impl->getOrCreateField(name, desc_const);
        return new jvmc_field{field};
    } catch (...) {
        return nullptr;
    }
}

int jvmc_method_add_flag(jvmc_method *method, uint16_t flag) {
    if (method == nullptr || method->impl == nullptr) {
        return 0;
    }
    try {
        method->impl->addFlag(static_cast<jvm::Method::AccessFlag>(flag));
        return 1;
    } catch (...) {
        return 0;
    }
}

int jvmc_field_add_flag(jvmc_field *field, uint16_t flag) {
    if (field == nullptr || field->impl == nullptr) {
        return 0;
    }
    try {
        field->impl->addFlag(static_cast<jvm::Field::AccessFlag>(flag));
        return 1;
    } catch (...) {
        return 0;
    }
}

jvmc_code *jvmc_method_get_code(jvmc_method *method) {
    if (method == nullptr || method->impl == nullptr) {
        return nullptr;
    }
    try {
        auto *code = method->impl->getCodeAttribute();
        return new jvmc_code{code};
    } catch (...) {
        return nullptr;
    }
}

jvmc_class_ref *jvmc_class_get_or_create_class_ref(jvmc_class *cls, const char *internal_name) {
    if (cls == nullptr || cls->impl == nullptr || internal_name == nullptr) {
        return nullptr;
    }
    try {
        auto *c = cls->impl->getOrCreateClassConstant(internal_name);
        return new jvmc_class_ref{c};
    } catch (...) {
        return nullptr;
    }
}

jvmc_fieldref *jvmc_class_get_or_create_fieldref(jvmc_class *cls, const char *owner_internal,
                                                 const char *name, const char *descriptor) {
    if (cls == nullptr || cls->impl == nullptr || owner_internal == nullptr ||
        name == nullptr || descriptor == nullptr) {
        return nullptr;
    }
    try {
        auto *owner = cls->impl->getOrCreateClassConstant(owner_internal);
        auto *name_const = cls->impl->getOrCreateUtf8Constant(name);
        auto *desc_const = cls->impl->getOrCreateUtf8Constant(descriptor);
        auto *name_type = cls->impl->getOrCreateNameAndTypeConstant(name_const, desc_const);
        auto *ref = cls->impl->getOrCreateFieldrefConstant(owner, name_type);
        return new jvmc_fieldref{ref};
    } catch (...) {
        return nullptr;
    }
}

jvmc_methodref *jvmc_class_get_or_create_methodref(jvmc_class *cls, const char *owner_internal,
                                                   const char *name, const char *descriptor) {
    if (cls == nullptr || cls->impl == nullptr || owner_internal == nullptr ||
        name == nullptr || descriptor == nullptr) {
        return nullptr;
    }
    try {
        auto *owner = cls->impl->getOrCreateClassConstant(owner_internal);
        auto *name_const = cls->impl->getOrCreateUtf8Constant(name);
        auto *desc_const = cls->impl->getOrCreateUtf8Constant(descriptor);
        auto *name_type = cls->impl->getOrCreateNameAndTypeConstant(name_const, desc_const);
        auto *ref = cls->impl->getOrCreateMethodrefConstant(owner, name_type);
        return new jvmc_methodref{ref};
    } catch (...) {
        return nullptr;
    }
}

jvmc_iface_methodref *jvmc_class_get_or_create_iface_methodref(jvmc_class *cls, const char *owner_internal,
                                                               const char *name, const char *descriptor) {
    if (cls == nullptr || cls->impl == nullptr || owner_internal == nullptr ||
        name == nullptr || descriptor == nullptr) {
        return nullptr;
    }
    try {
        auto *owner = cls->impl->getOrCreateClassConstant(owner_internal);
        auto *name_const = cls->impl->getOrCreateUtf8Constant(name);
        auto *desc_const = cls->impl->getOrCreateUtf8Constant(descriptor);
        auto *name_type = cls->impl->getOrCreateNameAndTypeConstant(name_const, desc_const);
        auto *ref = cls->impl->getOrCreateInterfaceMethodrefConstant(owner, name_type);
        return new jvmc_iface_methodref{ref};
    } catch (...) {
        return nullptr;
    }
}

int jvmc_code_push_null(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->PushNull());
}
int jvmc_code_push_int(jvmc_code *code, int32_t value) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->PushInt(value));
}
int jvmc_code_push_long(jvmc_code *code, int64_t value) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->PushLong(value));
}
int jvmc_code_push_float(jvmc_code *code, float value) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->PushFloat(value));
}
int jvmc_code_push_double(jvmc_code *code, double value) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->PushDouble(value));
}
int jvmc_code_push_string(jvmc_code *code, const char *value) {
    if (code == nullptr || code->impl == nullptr || value == nullptr) {
        return 0;
    }
    return AddInstruction(code->impl, code->impl->PushString(value));
}

int jvmc_code_load_int(jvmc_code *code, uint16_t index) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->LoadInt(index));
}
int jvmc_code_load_long(jvmc_code *code, uint16_t index) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->LoadLong(index));
}
int jvmc_code_load_float(jvmc_code *code, uint16_t index) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->LoadFloat(index));
}
int jvmc_code_load_double(jvmc_code *code, uint16_t index) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->LoadDouble(index));
}
int jvmc_code_load_ref(jvmc_code *code, uint16_t index) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->LoadReference(index));
}

int jvmc_code_store_int(jvmc_code *code, uint16_t index) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->StoreInt(index));
}
int jvmc_code_store_long(jvmc_code *code, uint16_t index) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->StoreLong(index));
}
int jvmc_code_store_float(jvmc_code *code, uint16_t index) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->StoreFloat(index));
}
int jvmc_code_store_double(jvmc_code *code, uint16_t index) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->StoreDouble(index));
}
int jvmc_code_store_ref(jvmc_code *code, uint16_t index) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->StoreReference(index));
}

int jvmc_code_add_int(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->AddInt());
}
int jvmc_code_sub_int(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->SubInt());
}
int jvmc_code_mul_int(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->MulInt());
}
int jvmc_code_div_int(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->DivInt());
}
int jvmc_code_add_float(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->AddFloat());
}
int jvmc_code_sub_float(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->SubFloat());
}
int jvmc_code_mul_float(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->MulFloat());
}
int jvmc_code_div_float(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->DivFloat());
}
int jvmc_code_add_double(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->AddDouble());
}
int jvmc_code_sub_double(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->SubDouble());
}
int jvmc_code_mul_double(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->MulDouble());
}
int jvmc_code_div_double(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->DivDouble());
}
int jvmc_code_neg_float(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->NegFloat());
}
int jvmc_code_neg_double(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->NegDouble());
}
int jvmc_code_cmp_float_g(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->CompareFloat(jvm::Instruction::StrictCompare::greater));
}
int jvmc_code_cmp_double_g(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->CompareDouble(jvm::Instruction::StrictCompare::greater));
}
int jvmc_code_i2f(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->IntToFloat());
}
int jvmc_code_i2d(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->IntToDouble());
}
int jvmc_code_f2i(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->FloatToInt());
}
int jvmc_code_f2d(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->FloatToDouble());
}
int jvmc_code_d2i(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->DoubleToInt());
}
int jvmc_code_d2f(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->DoubleToFloat());
}

int jvmc_code_dup(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->Duplicate());
}
int jvmc_code_pop(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->PopOne());
}
int jvmc_code_pop2(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->PopTwo());
}

int jvmc_code_return_void(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->ReturnVoid());
}
int jvmc_code_return_int(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->ReturnInt());
}
int jvmc_code_return_long(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->ReturnLong());
}
int jvmc_code_return_float(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->ReturnFloat());
}
int jvmc_code_return_double(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->ReturnDouble());
}
int jvmc_code_return_ref(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->ReturnReference());
}

int jvmc_code_getstatic(jvmc_code *code, jvmc_fieldref *field) {
    if (code == nullptr || code->impl == nullptr || field == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->GetStatic(field->impl));
}
int jvmc_code_putstatic(jvmc_code *code, jvmc_fieldref *field) {
    if (code == nullptr || code->impl == nullptr || field == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->PutStatic(field->impl));
}
int jvmc_code_getfield(jvmc_code *code, jvmc_fieldref *field) {
    if (code == nullptr || code->impl == nullptr || field == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->GetField(field->impl));
}
int jvmc_code_putfield(jvmc_code *code, jvmc_fieldref *field) {
    if (code == nullptr || code->impl == nullptr || field == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->PutField(field->impl));
}

int jvmc_code_invokevirtual(jvmc_code *code, jvmc_methodref *method) {
    if (code == nullptr || code->impl == nullptr || method == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->InvokeVirtual(method->impl));
}
int jvmc_code_invokespecial(jvmc_code *code, jvmc_methodref *method) {
    if (code == nullptr || code->impl == nullptr || method == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->InvokeSpecial(method->impl));
}
int jvmc_code_invokestatic(jvmc_code *code, jvmc_methodref *method) {
    if (code == nullptr || code->impl == nullptr || method == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->InvokeStatic(method->impl));
}
int jvmc_code_invokeinterface(jvmc_code *code, jvmc_iface_methodref *method) {
    if (code == nullptr || code->impl == nullptr || method == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->InvokeInterface(method->impl));
}

int jvmc_code_new(jvmc_code *code, jvmc_class_ref *class_ref) {
    if (code == nullptr || code->impl == nullptr || class_ref == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->New(class_ref->impl));
}

int jvmc_code_newarray_primitive(jvmc_code *code, uint8_t type) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->NewArray(static_cast<jvm::Instruction::Type>(type)));
}

int jvmc_code_newarray_ref(jvmc_code *code, jvmc_class_ref *class_ref) {
    if (code == nullptr || code->impl == nullptr || class_ref == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->NewArray(class_ref->impl));
}

int jvmc_code_array_length(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->ArrayLength());
}

int jvmc_code_array_load_bool(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->LoadBooleanFromArray());
}
int jvmc_code_array_load_int(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->LoadIntFromArray());
}
int jvmc_code_array_load_float(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->LoadFloatFromArray());
}
int jvmc_code_array_load_double(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->LoadDoubleFromArray());
}
int jvmc_code_array_load_ref(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->LoadReferenceFromArray());
}

int jvmc_code_array_store_bool(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->StoreBooleanToArray());
}
int jvmc_code_array_store_int(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->StoreIntToArray());
}
int jvmc_code_array_store_float(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->StoreFloatToArray());
}
int jvmc_code_array_store_double(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->StoreDoubleToArray());
}
int jvmc_code_array_store_ref(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) return 0;
    return AddInstruction(code->impl, code->impl->StoreReferenceToArray());
}

jvmc_label *jvmc_code_label_create(jvmc_code *code) {
    if (code == nullptr || code->impl == nullptr) {
        return nullptr;
    }
    try {
        auto *label = code->impl->CodeLabel();
        return new jvmc_label{label};
    } catch (...) {
        return nullptr;
    }
}

int jvmc_code_label_place(jvmc_code *code, jvmc_label *label) {
    if (code == nullptr || code->impl == nullptr || label == nullptr) {
        return 0;
    }
    try {
        code->impl->addLabel(label->impl);
        return 1;
    } catch (...) {
        return 0;
    }
}

int jvmc_code_goto(jvmc_code *code, jvmc_label *label) {
    if (code == nullptr || code->impl == nullptr || label == nullptr) {
        return 0;
    }
    return AddInstruction(code->impl, code->impl->GoTo(label->impl));
}

int jvmc_code_if(jvmc_code *code, jvmc_compare cmp, jvmc_label *label) {
    if (code == nullptr || code->impl == nullptr || label == nullptr) {
        return 0;
    }
    return AddInstruction(code->impl, code->impl->If(ToCompare(cmp), label->impl));
}

int jvmc_code_if_icmp(jvmc_code *code, jvmc_compare cmp, jvmc_label *label) {
    if (code == nullptr || code->impl == nullptr || label == nullptr) {
        return 0;
    }
    return AddInstruction(code->impl, code->impl->IfWithCompare(ToCompare(cmp), label->impl));
}

int jvmc_code_if_acmp_eq(jvmc_code *code, jvmc_label *label) {
    if (code == nullptr || code->impl == nullptr || label == nullptr) {
        return 0;
    }
    return AddInstruction(code->impl, code->impl->IfReferenceEqual(label->impl));
}

int jvmc_code_if_acmp_ne(jvmc_code *code, jvmc_label *label) {
    if (code == nullptr || code->impl == nullptr || label == nullptr) {
        return 0;
    }
    return AddInstruction(code->impl, code->impl->IfReferenceNotEqual(label->impl));
}

int jvmc_code_if_null(jvmc_code *code, jvmc_label *label) {
    if (code == nullptr || code->impl == nullptr || label == nullptr) {
        return 0;
    }
    return AddInstruction(code->impl, code->impl->IfNull(label->impl));
}

int jvmc_code_if_not_null(jvmc_code *code, jvmc_label *label) {
    if (code == nullptr || code->impl == nullptr || label == nullptr) {
        return 0;
    }
    return AddInstruction(code->impl, code->impl->IfNotNull(label->impl));
}

int jvmc_code_checkcast(jvmc_code *code, jvmc_class_ref *class_ref) {
    if (code == nullptr || code->impl == nullptr || class_ref == nullptr) {
        return 0;
    }
    return AddInstruction(code->impl, code->impl->CheckCast(class_ref->impl));
}

int jvmc_code_set_max_stack(jvmc_code *code, uint16_t value) {
    if (code == nullptr || code->impl == nullptr) {
        return 0;
    }
    try {
        code->impl->setMaxStack(value);
        return 1;
    } catch (...) {
        return 0;
    }
}

int jvmc_code_set_max_locals(jvmc_code *code, uint16_t value) {
    if (code == nullptr || code->impl == nullptr) {
        return 0;
    }
    try {
        code->impl->setMaxLocals(value);
        return 1;
    } catch (...) {
        return 0;
    }
}

} // extern "C"
