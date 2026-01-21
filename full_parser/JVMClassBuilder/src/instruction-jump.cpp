#include "jvm/instruction-jump.h"

#include <cassert>
#include <ostream>

#include "jvm/attribute-code.h"
#include "jvm/internal/utils.h"

using namespace jvm;

Label* InstructionJump::getJumpLabel() const
{
    return label_;
}

InstructionJump::InstructionJump(AttributeCode* attributeCode, Command command, Label* label) :
    Instruction(attributeCode, command), label_(label)
{
    assert(label_ != nullptr);
}


void InstructionJump::writeTo(std::ostream& os) const
{
    assert(label_ != nullptr);

    Instruction::writeTo(os);

    Instruction* toTarget = label_->getInstruction();
    if (!toTarget)
    {
        throw std::logic_error("Jump label is not bound to any instruction.");
    }

    internal::Utils::writeBigEndian(os, static_cast<uint16_t>(calculateShift(toTarget)));
}

size_t InstructionJump::getByteSize() const
{
    return Instruction::getByteSize() + 2;
}
