#include "jvm/attribute-code.h"

#include <cassert>
#include <stdexcept>

#include "jvm/constant-double.h"
#include "jvm/constant-fieldref.h"
#include "jvm/constant-methodref.h"
#include "jvm/constant-interface-methodref.h"
#include "jvm/constant-float.h"
#include "jvm/constant-integer.h"
#include "jvm/constant-long.h"
#include "jvm/constant-string.h"
#include "jvm/instruction-ldc.h"
#include "jvm/instruction-value.h"
#include "jvm/instruction-with-constant.h"
#include "jvm/method.h"


using namespace jvm;

#define REQUIRE_FINALIZED() \
if (!isFinalized()) { \
    throw std::logic_error("CodeAttribute is not finalized"); \
}


AttributeCode::~AttributeCode()
{
    for (auto* instruction : code_)
    {
        delete instruction;
    }

    for (auto* handler : exceptionHandlers_)
    {
        delete handler;
    }

    for (auto* label : allRegisteredLabels_)
    {
        delete label;
    }

    for (auto* attribute : attributes_)
    {
        delete attribute;
    }
}

Instruction* AttributeCode::Nop()
{
    return new Instruction(this, Instruction::INSTRUCTION_nop);
}

Instruction* AttributeCode::PushNull()
{
    return new Instruction(this, Instruction::INSTRUCTION_aconst_null);
}

Instruction* AttributeCode::PushInt(int32_t value)
{
    switch (value)
    {
    case -1:
        return new Instruction(this, Instruction::INSTRUCTION_iconst_m1);
    case 0:
        return new Instruction(this, Instruction::INSTRUCTION_iconst_0);
    case 1:
        return new Instruction(this, Instruction::INSTRUCTION_iconst_1);
    case 2:
        return new Instruction(this, Instruction::INSTRUCTION_iconst_2);
    case 3:
        return new Instruction(this, Instruction::INSTRUCTION_iconst_3);
    case 4:
        return new Instruction(this, Instruction::INSTRUCTION_iconst_4);
    case 5:
        return new Instruction(this, Instruction::INSTRUCTION_iconst_5);
    default:
        {
            if (INT8_MIN <= value && value <= INT8_MAX)
            {
                return new InstructionValue(this, Instruction::INSTRUCTION_bipush, static_cast<int8_t>(value));
            }
            if (INT16_MIN <= value && value <= INT16_MAX)
            {
                return new InstructionValue(this, Instruction::INSTRUCTION_sipush, static_cast<int16_t>(value));
            }
            if (INT32_MIN <= value && value <= INT32_MAX)
            {
                auto* integerConstant = getOwner()->getOwner()->getOrCreateIntegerConstant(value);
                return new InstructionLdc(this, integerConstant);
            }
            throw std::runtime_error("Unsupported integer value.");
        }
    }
}

Instruction* AttributeCode::PushLong(int64_t value)
{
    switch (value)
    {
    case 0:
        return new Instruction(this, Instruction::INSTRUCTION_lconst_0);
    case 1:
        return new Instruction(this, Instruction::INSTRUCTION_lconst_1);
    default:
        auto* longConstant = getOwner()->getOwner()->getOrCreateLongConstant(value);
        return new InstructionLdc(this, longConstant);
    }
}

Instruction* AttributeCode::PushFloat(float value)
{
    if (value == 0)
    {
        return new Instruction(this, Instruction::INSTRUCTION_fconst_0);
    }
    if (value == 1)
    {
        return new Instruction(this, Instruction::INSTRUCTION_fconst_1);
    }
    if (value == 2)
    {
        return new Instruction(this, Instruction::INSTRUCTION_fconst_2);
    }
    auto* floatConstant = getOwner()->getOwner()->getOrCreateFloatConstant(value);
    return new InstructionLdc(this, floatConstant);
}

Instruction* AttributeCode::PushDouble(double value)
{
    if (value == 0)
    {
        return new Instruction(this, Instruction::INSTRUCTION_dconst_0);
    }
    if (value == 1)
    {
        return new Instruction(this, Instruction::INSTRUCTION_dconst_1);
    }
    auto* doubleConstant = getOwner()->getOwner()->getOrCreateDoubleConstant(value);
    return new InstructionLdc(this, doubleConstant);
}

Instruction* AttributeCode::PushString(ConstantString* stringConstant)
{
    return new InstructionLdc(this, stringConstant);
}

Instruction* AttributeCode::PushString(const std::string& value)
{
    ConstantString* constantString = getOwner()->getOwner()->getOrCreateStringConstant(value);
    return PushString(constantString);
}

Instruction* AttributeCode::LoadInt(uint16_t index)
{
    switch (index)
    {
    case 0:
        return new Instruction(this, Instruction::INSTRUCTION_iload_0);
    case 1:
        return new Instruction(this, Instruction::INSTRUCTION_iload_1);
    case 2:
        return new Instruction(this, Instruction::INSTRUCTION_iload_2);
    case 3:
        return new Instruction(this, Instruction::INSTRUCTION_iload_3);
    default:
        {
            if (index <= UINT8_MAX)
            {
                return new InstructionValue(this, Instruction::INSTRUCTION_iload, static_cast<uint8_t>(index));
            }
            // ToDo: Implement support for the "INSTRUCTION_wide" instruction
            throw std::logic_error("\"wide\" instruction not implemented yet.");
            return new Instruction(this, Instruction::INSTRUCTION_wide);
        }
    }
}

Instruction* AttributeCode::LoadLong(uint16_t index)
{
    switch (index)
    {
    case 0:
        return new Instruction(this, Instruction::INSTRUCTION_lload_0);
    case 1:
        return new Instruction(this, Instruction::INSTRUCTION_lload_1);
    case 2:
        return new Instruction(this, Instruction::INSTRUCTION_lload_2);
    case 3:
        return new Instruction(this, Instruction::INSTRUCTION_lload_3);
    default:
        {
            if (index <= UINT8_MAX)
            {
                return new InstructionValue(this, Instruction::INSTRUCTION_lload, static_cast<uint8_t>(index));
            }
            // ToDo: Implement support for the "INSTRUCTION_wide" instruction
            throw std::logic_error("\"wide\" instruction not implemented yet.");
            return new Instruction(this, Instruction::INSTRUCTION_wide);
        }
    }
}

Instruction* AttributeCode::LoadFloat(uint16_t index)
{
    switch (index)
    {
    case 0:
        return new Instruction(this, Instruction::INSTRUCTION_fload_0);
    case 1:
        return new Instruction(this, Instruction::INSTRUCTION_fload_1);
    case 2:
        return new Instruction(this, Instruction::INSTRUCTION_fload_2);
    case 3:
        return new Instruction(this, Instruction::INSTRUCTION_fload_3);
    default:
        {
            if (index <= UINT8_MAX)
            {
                return new InstructionValue(this, Instruction::INSTRUCTION_fload, static_cast<uint8_t>(index));
            }
            // ToDo: Implement support for the "INSTRUCTION_wide" instruction
            throw std::logic_error("\"wide\" instruction not implemented yet.");
            return new Instruction(this, Instruction::INSTRUCTION_wide);
        }
    }
}

Instruction* AttributeCode::LoadDouble(uint16_t index)
{
    switch (index)
    {
    case 0:
        return new Instruction(this, Instruction::INSTRUCTION_dload_0);
    case 1:
        return new Instruction(this, Instruction::INSTRUCTION_dload_1);
    case 2:
        return new Instruction(this, Instruction::INSTRUCTION_dload_2);
    case 3:
        return new Instruction(this, Instruction::INSTRUCTION_dload_3);
    default:
        {
            if (index <= UINT8_MAX)
            {
                return new InstructionValue(this, Instruction::INSTRUCTION_dload, static_cast<uint8_t>(index));
            }
            // ToDo: Implement support for the "INSTRUCTION_wide" instruction
            throw std::logic_error("\"wide\" instruction not implemented yet.");
            return new Instruction(this, Instruction::INSTRUCTION_wide);
        }
    }
}

Instruction* AttributeCode::LoadReference(uint16_t index)
{
    switch (index)
    {
    case 0:
        return new Instruction(this, Instruction::INSTRUCTION_aload_0);
    case 1:
        return new Instruction(this, Instruction::INSTRUCTION_aload_1);
    case 2:
        return new Instruction(this, Instruction::INSTRUCTION_aload_2);
    case 3:
        return new Instruction(this, Instruction::INSTRUCTION_aload_3);
    default:
        {
            if (index <= UINT8_MAX)
            {
                return new InstructionValue(this, Instruction::INSTRUCTION_aload, static_cast<uint8_t>(index));
            }
            // ToDo: Implement support for the "INSTRUCTION_wide" instruction
            throw std::logic_error("\"wide\" instruction not implemented yet.");
            return new Instruction(this, Instruction::INSTRUCTION_wide);
        }
    }
}

Instruction* AttributeCode::LoadBooleanFromArray()
{
    return new Instruction(this, Instruction::INSTRUCTION_baload);
}

Instruction* AttributeCode::LoadByteFromArray()
{
    return new Instruction(this, Instruction::INSTRUCTION_baload);
}

Instruction* AttributeCode::LoadCharFromArray()
{
    return new Instruction(this, Instruction::INSTRUCTION_caload);
}

Instruction* AttributeCode::LoadShortFromArray()
{
    return new Instruction(this, Instruction::INSTRUCTION_saload);
}

Instruction* AttributeCode::LoadIntFromArray()
{
    return new Instruction(this, Instruction::INSTRUCTION_iaload);
}

Instruction* AttributeCode::LoadLongFromArray()
{
    return new Instruction(this, Instruction::INSTRUCTION_laload);
}

Instruction* AttributeCode::LoadFloatFromArray()
{
    return new Instruction(this, Instruction::INSTRUCTION_faload);
}

Instruction* AttributeCode::LoadDoubleFromArray()
{
    return new Instruction(this, Instruction::INSTRUCTION_daload);
}

Instruction* AttributeCode::LoadReferenceFromArray()
{
    return new Instruction(this, Instruction::INSTRUCTION_aaload);
}

Instruction* AttributeCode::StoreInt(uint16_t index)
{
    switch (index)
    {
    case 0:
        return new Instruction(this, Instruction::INSTRUCTION_istore_0);
    case 1:
        return new Instruction(this, Instruction::INSTRUCTION_istore_1);
    case 2:
        return new Instruction(this, Instruction::INSTRUCTION_istore_2);
    case 3:
        return new Instruction(this, Instruction::INSTRUCTION_istore_3);
    default:
        {
            if (index <= UINT8_MAX)
            {
                return new InstructionValue(this, Instruction::INSTRUCTION_istore, static_cast<uint8_t>(index));
            }
            // ToDo: Implement support for the "INSTRUCTION_wide" instruction
            throw std::logic_error("\"wide\" instruction not implemented yet.");
            return new Instruction(this, Instruction::INSTRUCTION_wide);
        }
    }
}

Instruction* AttributeCode::StoreLong(uint16_t index)
{
    switch (index)
    {
    case 0:
        return new Instruction(this, Instruction::INSTRUCTION_lstore_0);
    case 1:
        return new Instruction(this, Instruction::INSTRUCTION_lstore_1);
    case 2:
        return new Instruction(this, Instruction::INSTRUCTION_lstore_2);
    case 3:
        return new Instruction(this, Instruction::INSTRUCTION_lstore_3);
    default:
        {
            if (index <= UINT8_MAX)
            {
                return new InstructionValue(this, Instruction::INSTRUCTION_lstore, static_cast<uint8_t>(index));
            }
            // ToDo: Implement support for the "INSTRUCTION_wide" instruction
            throw std::logic_error("\"wide\" instruction not implemented yet.");
            return new Instruction(this, Instruction::INSTRUCTION_wide);
        }
    }
}

Instruction* AttributeCode::StoreFloat(uint16_t index)
{
    switch (index)
    {
    case 0:
        return new Instruction(this, Instruction::INSTRUCTION_fstore_0);
    case 1:
        return new Instruction(this, Instruction::INSTRUCTION_fstore_1);
    case 2:
        return new Instruction(this, Instruction::INSTRUCTION_fstore_2);
    case 3:
        return new Instruction(this, Instruction::INSTRUCTION_fstore_3);
    default:
        {
            if (index <= UINT8_MAX)
            {
                return new InstructionValue(this, Instruction::INSTRUCTION_fstore, static_cast<uint8_t>(index));
            }
            // ToDo: Implement support for the "INSTRUCTION_wide" instruction
            throw std::logic_error("\"wide\" instruction not implemented yet.");
            return new Instruction(this, Instruction::INSTRUCTION_wide);
        }
    }
}

Instruction* AttributeCode::StoreDouble(uint16_t index)
{
    switch (index)
    {
    case 0:
        return new Instruction(this, Instruction::INSTRUCTION_dstore_0);
    case 1:
        return new Instruction(this, Instruction::INSTRUCTION_dstore_1);
    case 2:
        return new Instruction(this, Instruction::INSTRUCTION_dstore_2);
    case 3:
        return new Instruction(this, Instruction::INSTRUCTION_dstore_3);
    default:
        {
            if (index <= UINT8_MAX)
            {
                return new InstructionValue(this, Instruction::INSTRUCTION_dstore, static_cast<uint8_t>(index));
            }
            // ToDo: Implement support for the "INSTRUCTION_wide" instruction
            throw std::logic_error("\"wide\" instruction not implemented yet.");
            return new Instruction(this, Instruction::INSTRUCTION_wide);
        }
    }
}

Instruction* AttributeCode::StoreReference(uint16_t index)
{
    switch (index)
    {
    case 0:
        return new Instruction(this, Instruction::INSTRUCTION_astore_0);
    case 1:
        return new Instruction(this, Instruction::INSTRUCTION_astore_1);
    case 2:
        return new Instruction(this, Instruction::INSTRUCTION_astore_2);
    case 3:
        return new Instruction(this, Instruction::INSTRUCTION_astore_3);
    default:
        {
            if (index <= UINT8_MAX)
            {
                return new InstructionValue(this, Instruction::INSTRUCTION_astore, static_cast<uint8_t>(index));
            }
            // ToDo: Implement support for the "INSTRUCTION_wide" instruction
            throw std::logic_error("\"wide\" instruction not implemented yet.");
            return new Instruction(this, Instruction::INSTRUCTION_wide);
        }
    }
}

Instruction* AttributeCode::StoreIntToArray()
{
    return new Instruction(this, Instruction::INSTRUCTION_iastore);
}

Instruction* AttributeCode::StoreLongToArray()
{
    return new Instruction(this, Instruction::INSTRUCTION_lastore);
}

Instruction* AttributeCode::StoreFloatToArray()
{
    return new Instruction(this, Instruction::INSTRUCTION_fastore);
}

Instruction* AttributeCode::StoreDoubleToArray()
{
    return new Instruction(this, Instruction::INSTRUCTION_dastore);
}

Instruction* AttributeCode::StoreReferenceToArray()
{
    return new Instruction(this, Instruction::INSTRUCTION_aastore);
}

Instruction* AttributeCode::StoreBooleanToArray()
{
    return new Instruction(this, Instruction::INSTRUCTION_bastore);
}

Instruction* AttributeCode::StoreByteToArray()
{
    return new Instruction(this, Instruction::INSTRUCTION_bastore);
}

Instruction* AttributeCode::StoreCharToArray()
{
    return new Instruction(this, Instruction::INSTRUCTION_castore);
}

Instruction* AttributeCode::StoreShortToArray()
{
    return new Instruction(this, Instruction::INSTRUCTION_sastore);
}

Instruction* AttributeCode::PopOne()
{
    return new Instruction(this, Instruction::INSTRUCTION_pop);
}

Instruction* AttributeCode::PopTwo()
{
    return new Instruction(this, Instruction::INSTRUCTION_pop2);
}

Instruction* AttributeCode::Duplicate()
{
    return new Instruction(this, Instruction::INSTRUCTION_dup);
}

Instruction* AttributeCode::DuplicateBeforeOne()
{
    return new Instruction(this, Instruction::INSTRUCTION_dup_x1);
}

Instruction* AttributeCode::DuplicateBeforeTwo()
{
    return new Instruction(this, Instruction::INSTRUCTION_dup_x2);
}

Instruction* AttributeCode::DuplicateDouble()
{
    return new Instruction(this, Instruction::INSTRUCTION_dup2);
}

Instruction* AttributeCode::DuplicateDoubleBeforeOne()
{
    return new Instruction(this, Instruction::INSTRUCTION_dup2_x1);
}

Instruction* AttributeCode::DuplicateDoubleBeforeTwo()
{
    return new Instruction(this, Instruction::INSTRUCTION_dup2_x2);
}

Instruction* AttributeCode::Swap()
{
    return new Instruction(this, Instruction::INSTRUCTION_swap);
}

Instruction* AttributeCode::AddInt()
{
    return new Instruction(this, Instruction::INSTRUCTION_iadd);
}

Instruction* AttributeCode::AddLong()
{
    return new Instruction(this, Instruction::INSTRUCTION_ladd);
}

Instruction* AttributeCode::AddFloat()
{
    return new Instruction(this, Instruction::INSTRUCTION_fadd);
}

Instruction* AttributeCode::AddDouble()
{
    return new Instruction(this, Instruction::INSTRUCTION_dadd);
}

Instruction* AttributeCode::SubInt()
{
    return new Instruction(this, Instruction::INSTRUCTION_isub);
}

Instruction* AttributeCode::SubLong()
{
    return new Instruction(this, Instruction::INSTRUCTION_lsub);
}

Instruction* AttributeCode::SubFloat()
{
    return new Instruction(this, Instruction::INSTRUCTION_fsub);
}

Instruction* AttributeCode::SubDouble()
{
    return new Instruction(this, Instruction::INSTRUCTION_dsub);
}

Instruction* AttributeCode::MulInt()
{
    return new Instruction(this, Instruction::INSTRUCTION_imul);
}

Instruction* AttributeCode::MulLong()
{
    return new Instruction(this, Instruction::INSTRUCTION_lmul);
}

Instruction* AttributeCode::MulFloat()
{
    return new Instruction(this, Instruction::INSTRUCTION_fmul);
}

Instruction* AttributeCode::MulDouble()
{
    return new Instruction(this, Instruction::INSTRUCTION_dmul);
}

Instruction* AttributeCode::DivInt()
{
    return new Instruction(this, Instruction::INSTRUCTION_idiv);
}

Instruction* AttributeCode::DivLong()
{
    return new Instruction(this, Instruction::INSTRUCTION_ldiv);
}

Instruction* AttributeCode::DivFloat()
{
    return new Instruction(this, Instruction::INSTRUCTION_fdiv);
}

Instruction* AttributeCode::DivDouble()
{
    return new Instruction(this, Instruction::INSTRUCTION_ddiv);
}

Instruction* AttributeCode::RemInt()
{
    return new Instruction(this, Instruction::INSTRUCTION_irem);
}

Instruction* AttributeCode::RemLong()
{
    return new Instruction(this, Instruction::INSTRUCTION_lrem);
}

Instruction* AttributeCode::RemFloat()
{
    return new Instruction(this, Instruction::INSTRUCTION_frem);
}

Instruction* AttributeCode::RemDouble()
{
    return new Instruction(this, Instruction::INSTRUCTION_drem);
}

Instruction* AttributeCode::NegInt()
{
    return new Instruction(this, Instruction::INSTRUCTION_ineg);
}

Instruction* AttributeCode::NegLong()
{
    return new Instruction(this, Instruction::INSTRUCTION_lneg);
}

Instruction* AttributeCode::NegFloat()
{
    return new Instruction(this, Instruction::INSTRUCTION_fneg);
}

Instruction* AttributeCode::NegDouble()
{
    return new Instruction(this, Instruction::INSTRUCTION_dneg);
}

Instruction* AttributeCode::LeftShiftInt()
{
    return new Instruction(this, Instruction::INSTRUCTION_ishl);
}

Instruction* AttributeCode::RightArithmeticShiftInt()
{
    return new Instruction(this, Instruction::INSTRUCTION_ishr);
}

Instruction* AttributeCode::RightLogicShiftInt()
{
    return new Instruction(this, Instruction::INSTRUCTION_iushr);
}

Instruction* AttributeCode::LeftShiftLong()
{
    return new Instruction(this, Instruction::INSTRUCTION_lshl);
}

Instruction* AttributeCode::RightArithmeticShiftLong()
{
    return new Instruction(this, Instruction::INSTRUCTION_lshr);
}

Instruction* AttributeCode::RightLogicShiftLong()
{
    return new Instruction(this, Instruction::INSTRUCTION_lushr);
}

Instruction* AttributeCode::BitwiseAndInt()
{
    return new Instruction(this, Instruction::INSTRUCTION_iand);
}

Instruction* AttributeCode::BitwiseAndLong()
{
    return new Instruction(this, Instruction::INSTRUCTION_land);
}

Instruction* AttributeCode::BitwiseOrInt()
{
    return new Instruction(this, Instruction::INSTRUCTION_ior);
}

Instruction* AttributeCode::BitwiseOrLong()
{
    return new Instruction(this, Instruction::INSTRUCTION_lor);
}

Instruction* AttributeCode::BitwiseXorInt()
{
    return new Instruction(this, Instruction::INSTRUCTION_ixor);
}

Instruction* AttributeCode::BitwiseXorLong()
{
    return new Instruction(this, Instruction::INSTRUCTION_lxor);
}

Instruction* AttributeCode::IncrementLocalVariable(uint16_t index, int16_t value)
{
    if (index <= UINT8_MAX && INT8_MIN <= value && value <= INT8_MAX)
    {
        return new InstructionValue(
            this,
            Instruction::INSTRUCTION_iinc,
            static_cast<uint8_t>(index),
            static_cast<int8_t>(value)
        );
    }
    // ToDo: Implement support for the "INSTRUCTION_wide" instruction
    throw std::logic_error("\"wide\" instruction not implemented yet.");
    return new Instruction(this, Instruction::INSTRUCTION_wide);
}

Instruction* AttributeCode::IntToLong()
{
    return new Instruction(this, Instruction::INSTRUCTION_i2l);
}

Instruction* AttributeCode::IntToFloat()
{
    return new Instruction(this, Instruction::INSTRUCTION_i2f);
}

Instruction* AttributeCode::IntToDouble()
{
    return new Instruction(this, Instruction::INSTRUCTION_i2d);
}

Instruction* AttributeCode::LongToInt()
{
    return new Instruction(this, Instruction::INSTRUCTION_l2i);
}

Instruction* AttributeCode::LongToFloat()
{
    return new Instruction(this, Instruction::INSTRUCTION_l2f);
}

Instruction* AttributeCode::LongToDouble()
{
    return new Instruction(this, Instruction::INSTRUCTION_l2d);
}

Instruction* AttributeCode::FloatToInt()
{
    return new Instruction(this, Instruction::INSTRUCTION_f2i);
}

Instruction* AttributeCode::FloatToLong()
{
    return new Instruction(this, Instruction::INSTRUCTION_f2l);
}

Instruction* AttributeCode::FloatToDouble()
{
    return new Instruction(this, Instruction::INSTRUCTION_f2d);
}

Instruction* AttributeCode::DoubleToInt()
{
    return new Instruction(this, Instruction::INSTRUCTION_d2i);
}

Instruction* AttributeCode::DoubleToLong()
{
    return new Instruction(this, Instruction::INSTRUCTION_d2l);
}

Instruction* AttributeCode::DoubleToFloat()
{
    return new Instruction(this, Instruction::INSTRUCTION_d2f);
}

Instruction* AttributeCode::IntToByte()
{
    return new Instruction(this, Instruction::INSTRUCTION_i2b);
}

Instruction* AttributeCode::IntToChar()
{
    return new Instruction(this, Instruction::INSTRUCTION_i2c);
}

Instruction* AttributeCode::IntToShort()
{
    return new Instruction(this, Instruction::INSTRUCTION_i2s);
}

Instruction* AttributeCode::CompareLong()
{
    return new Instruction(this, Instruction::INSTRUCTION_lcmp);
}

Instruction* AttributeCode::CompareFloat(Instruction::StrictCompare nanResult)
{
    switch (nanResult)
    {
    case Instruction::Greater:
        return new Instruction(this, Instruction::INSTRUCTION_fcmpg);
    case Instruction::Less:
        return new Instruction(this, Instruction::INSTRUCTION_fcmpl);
    default:
        throw std::runtime_error("Unknown instruction type");
    }
}

Instruction* AttributeCode::CompareDouble(Instruction::StrictCompare nanResult)
{
    switch (nanResult)
    {
    case Instruction::Greater:
        return new Instruction(this, Instruction::INSTRUCTION_dcmpg);
    case Instruction::Less:
        return new Instruction(this, Instruction::INSTRUCTION_dcmpl);
    default:
        throw std::runtime_error("Unknown instruction type");
    }
}

InstructionJump* AttributeCode::If(Instruction::Compare operation, Label* label)
{
    switch (operation)
    {
    case Instruction::Equal:
        return new InstructionJump(this, Instruction::INSTRUCTION_ifeq, label);
    case Instruction::NotEqual:
        return new InstructionJump(this, Instruction::INSTRUCTION_ifne, label);
    case Instruction::LessThan:
        return new InstructionJump(this, Instruction::INSTRUCTION_iflt, label);
    case Instruction::GreaterThan:
        return new InstructionJump(this, Instruction::INSTRUCTION_ifgt, label);
    case Instruction::LessEqual:
        return new InstructionJump(this, Instruction::INSTRUCTION_ifle, label);
    case Instruction::GreaterEqual:
        return new InstructionJump(this, Instruction::INSTRUCTION_ifge, label);
    default:
        throw std::runtime_error("Unknown compare type.");
    }
}

InstructionJump* AttributeCode::IfWithCompare(Instruction::Compare operation, Label* label)
{
    switch (operation)
    {
    case Instruction::Equal:
        return new InstructionJump(this, Instruction::INSTRUCTION_if_icmpeq, label);
    case Instruction::NotEqual:
        return new InstructionJump(this, Instruction::INSTRUCTION_if_icmpne, label);
    case Instruction::LessThan:
        return new InstructionJump(this, Instruction::INSTRUCTION_if_icmplt, label);
    case Instruction::GreaterThan:
        return new InstructionJump(this, Instruction::INSTRUCTION_if_icmpgt, label);
    case Instruction::LessEqual:
        return new InstructionJump(this, Instruction::INSTRUCTION_if_icmple, label);
    case Instruction::GreaterEqual:
        return new InstructionJump(this, Instruction::INSTRUCTION_if_icmpge, label);
    default:
        throw std::runtime_error("Unknown compare type.");
    }
}

InstructionJump* AttributeCode::IfReferenceEqual(Label* label)
{
    return new InstructionJump(this, Instruction::INSTRUCTION_if_acmpeq, label);
}

InstructionJump* AttributeCode::IfReferenceNotEqual(Label* label)
{
    return new InstructionJump(this, Instruction::INSTRUCTION_if_acmpne, label);
}

InstructionJump* AttributeCode::GoTo(Label* label)
{
    // ToDo: Implement support for the "goto_w" instruction
    return new InstructionJump(this, Instruction::INSTRUCTION_goto, label);
}

Instruction* AttributeCode::Switch(int32_t low, int32_t high, Label* defaultLabel, const std::vector<Label*>& labels)
{
    // ToDo: Implement support for the "tableswitch" instruction
    throw std::logic_error("Switch() not implemented yet.");
    return new Instruction(this, Instruction::INSTRUCTION_tableswitch);
}

Instruction* AttributeCode::Switch(Label* defaultLabel, const std::map<int32_t, Label*>& labels)
{
    // ToDo: Implement support for the "lookupswitch" instruction
    throw std::logic_error("Switch() not implemented yet.");
    return new Instruction(this, Instruction::INSTRUCTION_lookupswitch);
}

Instruction* AttributeCode::ReturnBoolean()
{
    return new Instruction(this, Instruction::INSTRUCTION_ireturn);
}

Instruction* AttributeCode::ReturnByte()
{
    return new Instruction(this, Instruction::INSTRUCTION_ireturn);
}

Instruction* AttributeCode::ReturnChar()
{
    return new Instruction(this, Instruction::INSTRUCTION_ireturn);
}

Instruction* AttributeCode::ReturnShort()
{
    return new Instruction(this, Instruction::INSTRUCTION_ireturn);
}

Instruction* AttributeCode::ReturnInt()
{
    return new Instruction(this, Instruction::INSTRUCTION_ireturn);
}

Instruction* AttributeCode::ReturnLong()
{
    return new Instruction(this, Instruction::INSTRUCTION_lreturn);
}

Instruction* AttributeCode::ReturnFloat()
{
    return new Instruction(this, Instruction::INSTRUCTION_freturn);
}

Instruction* AttributeCode::ReturnDouble()
{
    return new Instruction(this, Instruction::INSTRUCTION_dreturn);
}

Instruction* AttributeCode::ReturnReference()
{
    return new Instruction(this, Instruction::INSTRUCTION_areturn);
}

Instruction* AttributeCode::ReturnVoid()
{
    return new Instruction(this, Instruction::INSTRUCTION_return);
}

Instruction* AttributeCode::GetStatic(ConstantFieldref* field)
{
    return new InstructionWithConstant(
        this,
        Instruction::INSTRUCTION_getstatic,
        field,
        InstructionWithConstant::TwoByte
    );
}

Instruction* AttributeCode::PutStatic(ConstantFieldref* field)
{
    return new InstructionWithConstant(
        this,
        Instruction::INSTRUCTION_putstatic,
        field,
        InstructionWithConstant::TwoByte
    );
}

Instruction* AttributeCode::GetField(ConstantFieldref* field)
{
    return new InstructionWithConstant(
        this,
        Instruction::INSTRUCTION_getfield,
        field,
        InstructionWithConstant::TwoByte
    );
}

Instruction* AttributeCode::PutField(ConstantFieldref* field)
{
    return new InstructionWithConstant(
        this,
        Instruction::INSTRUCTION_putfield,
        field,
        InstructionWithConstant::TwoByte
    );
}

Instruction* AttributeCode::InvokeVirtual(ConstantMethodref* method)
{
    return new InstructionWithConstant(
        this,
        Instruction::INSTRUCTION_invokevirtual,
        method,
        InstructionWithConstant::TwoByte
    );
}

Instruction* AttributeCode::InvokeSpecial(ConstantMethodref* method)
{
    return new InstructionWithConstant(
        this,
        Instruction::INSTRUCTION_invokespecial,
        method,
        InstructionWithConstant::TwoByte
    );
}

Instruction* AttributeCode::InvokeStatic(ConstantMethodref* method)
{
    return new InstructionWithConstant(
        this,
        Instruction::INSTRUCTION_invokestatic,
        method,
        InstructionWithConstant::TwoByte
    );
}

Instruction* AttributeCode::InvokeInterface(ConstantInterfaceMethodref* method)
{
    return new InstructionWithConstant(
        this,
        Instruction::INSTRUCTION_invokeinterface,
        method,
        InstructionWithConstant::TwoByte
    );
}

Instruction* AttributeCode::New(ConstantClass* classConstant)
{
    return new InstructionWithConstant(
        this,
        Instruction::INSTRUCTION_new,
        classConstant,
        InstructionWithConstant::TwoByte
    );
}

Instruction* AttributeCode::NewArray(Instruction::Type type)
{
    return new InstructionValue(this, Instruction::INSTRUCTION_newarray, static_cast<uint8_t>(type));
}

Instruction* AttributeCode::NewArray(ConstantClass* classConstant)
{
    return new InstructionWithConstant(
        this,
        Instruction::INSTRUCTION_anewarray,
        classConstant,
        InstructionWithConstant::TwoByte
    );
}

Instruction* AttributeCode::ArrayLength()
{
    return new Instruction(this, Instruction::INSTRUCTION_arraylength);
}

Instruction* AttributeCode::Throw()
{
    return new Instruction(this, Instruction::INSTRUCTION_athrow);
}

Instruction* AttributeCode::CheckCast(ConstantClass* classConstant)
{
    return new InstructionWithConstant(
        this,
        Instruction::INSTRUCTION_checkcast,
        classConstant,
        InstructionWithConstant::TwoByte
    );
}

Instruction* AttributeCode::InstanceOf(ConstantClass* classConstant)
{
    return new InstructionWithConstant(
        this,
        Instruction::INSTRUCTION_instanceof,
        classConstant,
        InstructionWithConstant::TwoByte
    );
}

Instruction* AttributeCode::MonitorEnter()
{
    return new Instruction(this, Instruction::INSTRUCTION_monitorenter);
}

Instruction* AttributeCode::MonitorExit()
{
    return new Instruction(this, Instruction::INSTRUCTION_monitorexit);
}

Instruction* AttributeCode::MultiNewArray(ConstantClass* classConstant, uint8_t dimensions)
{
    // ToDo: Implement support for the "multianewarray" instruction
    throw std::logic_error("\"multianewarray\" instruction not implemented yet.");
    return new Instruction(this, Instruction::INSTRUCTION_multianewarray);
}

InstructionJump* AttributeCode::IfNull(Label* label)
{
    return new InstructionJump(this, Instruction::INSTRUCTION_ifnull, label);
}

InstructionJump* AttributeCode::IfNotNull(Label* label)
{
    return new InstructionJump(this, Instruction::INSTRUCTION_ifnonnull, label);
}

AttributeCode& AttributeCode::addInstruction(Instruction* instruction)
{
    if (isFinalized())
    {
        throw std::logic_error("Attribute code has already finished. New instructions cannot be added.");
    }

    for (auto* label : labelsOnCurrentStep_)
    {
        label->setInstruction(instruction);
    }
    labelsOnCurrentStep_.clear();

    code_.push_back(instruction);
    return *this;
}

Label* AttributeCode::CodeLabel()
{
    return new Label(this);
}

AttributeCode& AttributeCode::addLabel(Label* label)
{
    if (isFinalized())
    {
        throw std::logic_error("Attribute code has already finished. New instructions cannot be added.");
    }

    assert(label->getOwner() == this);

    if (label->getInstruction() != nullptr)
    {
        throw std::invalid_argument("The label already references the instruction");
    }
    labelsOnCurrentStep_.insert(label);
    allRegisteredLabels_.insert(label);

    return *this;
}

ExceptionHandler* AttributeCode::addTryCatch(Label* tryStartLabel, Label* tryFinishLabel, Label* catchStartLabel,
                                             ConstantClass* catchClass)
{
    auto* handler = new ExceptionHandler(tryStartLabel, tryFinishLabel, catchStartLabel, catchClass, this);
    exceptionHandlers_.insert(handler);
    return handler;
}

void AttributeCode::writeTo(std::ostream& os) const
{
    REQUIRE_FINALIZED();

    Attribute::writeTo(os);

    // u2 max_stack;
    internal::Utils::writeBigEndian(os, static_cast<uint16_t>(maxStack_));

    // u2 max_locals;
    internal::Utils::writeBigEndian(os, static_cast<uint16_t>(maxLocals_));

    // u4 code_length;
    internal::Utils::writeBigEndian(os, static_cast<uint32_t>(instructionsByteSize_));

    // u1 code[code_length];
    for (auto* instruction : code_)
    {
        os << *instruction;
    }

    // u2 exception_table_length;
    internal::Utils::writeBigEndian(os, static_cast<uint16_t>(exceptionHandlers_.size()));

    // exception_table[exception_table_length];
    for (auto* handler : exceptionHandlers_)
    {
        os << *handler;
    }

    // u2 attributes_count;
    internal::Utils::writeBigEndian(os, static_cast<uint16_t>(attributes_.size()));

    // attribute_info attributes[attributes_count];
    for (auto* attribute : attributes_)
    {
        os << *attribute;
    }
}

size_t AttributeCode::getContentSizeInBytes() const
{
    REQUIRE_FINALIZED();

    uint32_t size = 0;


    // u2 max_stack;
    size += 2;

    // u2 max_locals;
    size += 2;

    // u4 code_length;
    size += 4;

    // u1 code[code_length];
    size += instructionsByteSize_;

    // u2 exception_table_length;
    size += 2;

    // exception_table[exception_table_length];
    size += exceptionsHandlersByteSize_;

    // u2 attributes_count;
    size += 2;

    // attribute_info attributes[attributes_count];
    for (auto* attribute : attributes_)
    {
        size += attribute->getByteSize();
    }


    return size;
}

AttributeCode::AttributeCode(Method* methodOwner) :
    Attribute(methodOwner->getOwner()->getOrCreateUtf8Constant("Code")),
    ClassFileElement(methodOwner)
{
}

std::size_t AttributeCode::getByteSize() const
{
    return Attribute::getByteSize();
}

bool AttributeCode::isFinalized() const
{
    return isFinalized_;
}

void AttributeCode::finalize()
{
    // return if already finalized
    if (isFinalized()) { return; }

    // check for unlinked labels
    if (!labelsOnCurrentStep_.empty())
    {
        throw std::logic_error(
            "The most recently added labels do not have instructions after them. The class cannot complete initialization.");
    }

    // update instructions with constants from constant pool
    for (auto* instruction : code_)
    {
        auto* constantInstruction = dynamic_cast<InstructionWithConstant*>(instruction);
        if (constantInstruction != nullptr)
        {
            constantInstruction->update();
        }
    }

    // set index to all instructions
    // calculate size of all instructions
    instructionsByteSize_ = 0;
    for (auto* instruction : code_)
    {
        instruction->setIndex(instructionsByteSize_);
        instructionsByteSize_ += instruction->getByteSize();
    }

    // check code size
    if (instructionsByteSize_ > UINT16_MAX)
    {
        throw std::runtime_error("Too large instructions size.");
    }

    // calculate size of all exceptions handlers
    exceptionsHandlersByteSize_ = ExceptionHandler::sizeInBytes * exceptionHandlers_.size();

    // check code size
    if (instructionsByteSize_ > UINT16_MAX)
    {
        throw std::runtime_error("Too large attribute size.");
    }

    // finalize code attribute
    isFinalized_ = true;
}
