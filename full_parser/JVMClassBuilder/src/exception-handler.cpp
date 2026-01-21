#include "jvm/exception-handler.h"

#include <cassert>
#include <stdexcept>

#include "jvm/attribute-code.h"
#include "jvm/constant-class.h"
#include "jvm/instruction.h"
#include "jvm/label.h"
#include "jvm/internal/utils.h"

using namespace jvm;

Label* ExceptionHandler::getTryStartLabel() const
{
    return tryStartLabel_;
}

Label* ExceptionHandler::getTryFinishLabel() const
{
    return tryFinishLabel_;
}

Label* ExceptionHandler::getCatchStartLabel() const
{
    return catchStartLabel_;
}

ConstantClass* ExceptionHandler::getCatchClass() const
{
    return catchClass_;
}

void ExceptionHandler::writeTo(std::ostream& os) const
{
    // Labels must be bound to instructions.
    Instruction* startInstruction = tryStartLabel_->getInstruction();
    Instruction* endInstruction = tryFinishLabel_->getInstruction();
    Instruction* handlerInstruction = catchStartLabel_->getInstruction();

    if (!startInstruction || !endInstruction || !handlerInstruction)
    {
        throw std::logic_error("ExceptionHandler labels must be bound to instructions before serialization.");
    }

    if (!startInstruction->isIndexSet() || !endInstruction->isIndexSet() || !handlerInstruction->isIndexSet())
    {
        throw std::logic_error("ExceptionHandler label instructions must have their index set before serialization.");
    }

    const uint16_t start_pc = startInstruction->getIndex();
    const uint16_t end_pc = endInstruction->getIndex();
    const uint16_t handler_pc = handlerInstruction->getIndex();

    // catch_type: 0 => catch-all, otherwise constant pool index of CONSTANT_Class
    uint16_t catch_type = 0;
    if (catchClass_ != nullptr)
    {
        catch_type = catchClass_->getIndex();
    }

    internal::Utils::writeBigEndian(os, start_pc);
    internal::Utils::writeBigEndian(os, end_pc);
    internal::Utils::writeBigEndian(os, handler_pc);
    internal::Utils::writeBigEndian(os, catch_type);
}

std::size_t ExceptionHandler::getByteSize() const
{
    return sizeInBytes;
}

ExceptionHandler::ExceptionHandler(Label* tryStartLabel,
                                   Label* tryFinishLabel,
                                   Label* catchStartLabel,
                                   ConstantClass* catchClass,
                                   AttributeCode* owner)
    : ClassFileElement(owner),
      tryStartLabel_(tryStartLabel),
      tryFinishLabel_(tryFinishLabel),
      catchStartLabel_(catchStartLabel),
      catchClass_(catchClass)
{
    assert(tryStartLabel_ != nullptr);
    assert(tryFinishLabel_ != nullptr);
    assert(catchStartLabel_ != nullptr);
}
