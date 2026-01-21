#include "jvm/instruction-with-constant.h"

#include <ostream>

#include "jvm/internal/utils.h"

using namespace jvm;

Constant* InstructionWithConstant::getConstant() const
{
    return constant_;
}

InstructionWithConstant::InstructionWithConstant(AttributeCode* attributeCode,
                                                 Command command,
                                                 Constant* constant,
                                                 AvailableReferenceSize size) :
    Instruction(attributeCode, command), constant_(constant), size_(size)
{
}

void InstructionWithConstant::setAvailableReferenceSize(AvailableReferenceSize size)
{
    size_ = size;
}

void InstructionWithConstant::update()
{
}

void InstructionWithConstant::writeTo(std::ostream& os) const
{
    Instruction::writeTo(os);

    uint16_t index = constant_->getIndex();
    if (size_ == OneByte)
    {
        if (index > UINT8_MAX)
        {
            throw std::out_of_range("Constant index bigger then available reference size.");
        }
        internal::Utils::writeBigEndian(os, static_cast<uint8_t>(index));
    }
    else
    {
        internal::Utils::writeBigEndian(os, index);
    }
}

size_t InstructionWithConstant::getByteSize() const
{
    return Instruction::getByteSize() + size_;
}
