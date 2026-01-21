#include "jvm/label.h"

#include <cassert>

using namespace jvm;

Instruction* Label::getInstruction() const
{
    return instruction_;
}

Label::Label(AttributeCode* codeAttributeOwner) : OwnerAware(codeAttributeOwner)
{
    assert(codeAttributeOwner != nullptr);
}

bool Label::isInitialized() const
{
    return instruction_ != nullptr;
}

void Label::setInstruction(Instruction* instruction)
{
    assert(instruction != nullptr);
    assert(instruction_ == nullptr);

    instruction_ = instruction;
}
