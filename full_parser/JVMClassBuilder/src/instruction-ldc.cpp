#include "jvm/instruction-ldc.h"

#include <stdexcept>
#include <strings.h>

using namespace jvm;

void InstructionLdc::update()
{
    InstructionWithConstant::update();

    switch (getConstant()->getTag())
    {
    case Constant::CONSTANT_Integer:
    case Constant::CONSTANT_Float:
    case Constant::CONSTANT_String:
    case Constant::CONSTANT_Class:
    case Constant::CONSTANT_MethodHandle:
    case Constant::CONSTANT_MethodType:
    case Constant::CONSTANT_Dynamic:
        {
            auto index = getConstant()->getIndex();
            if (index <= UINT8_MAX)
            {
                setAvailableReferenceSize(OneByte);
                setCommand(INSTRUCTION_ldc);
            }
            else
            {
                setAvailableReferenceSize(TwoByte);
                setCommand(INSTRUCTION_ldc_w);
            }
            break;
        }
    case Constant::CONSTANT_Long:
    case Constant::CONSTANT_Double:
        {
            setAvailableReferenceSize(TwoByte);
            setCommand(INSTRUCTION_ldc2_w);
            break;
        }
    default:
        {
            throw std::invalid_argument("ldc* instructions do not support this type of constants.");
        };
    }
}

InstructionLdc::InstructionLdc(AttributeCode* attributeCode, Constant* constant) : InstructionWithConstant(
    attributeCode, selectInstruction(constant), constant, TwoByte)
{
}

Instruction::Command InstructionLdc::selectInstruction(Constant* constant)
{
    switch (constant->getTag())
    {
    case Constant::CONSTANT_Integer:
    case Constant::CONSTANT_Float:
    case Constant::CONSTANT_String:
    case Constant::CONSTANT_Class:
    case Constant::CONSTANT_MethodHandle:
    case Constant::CONSTANT_MethodType:
    case Constant::CONSTANT_Dynamic:
        return INSTRUCTION_ldc_w;
    case Constant::CONSTANT_Long:
    case Constant::CONSTANT_Double:
        return INSTRUCTION_ldc2_w;
    default:
        throw std::invalid_argument("ldc* instructions do not support this type of constants.");
    }
}
