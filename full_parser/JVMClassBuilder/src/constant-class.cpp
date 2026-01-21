#include "jvm/constant-class.h"

#include <ostream>

#include "jvm/internal/utils.h"

using namespace jvm;

ConstantUtf8Info* ConstantClass::getName() const
{
    return name_;
}

void ConstantClass::writeTo(std::ostream& os) const
{
    Constant::writeTo(os);
    uint16_t nameIndex = name_->getIndex();
    internal::Utils::writeBigEndian(os, nameIndex);
}

std::size_t ConstantClass::getByteSize() const
{
    return Constant::getByteSize() + 2;
}

ConstantClass::ConstantClass(ConstantUtf8Info* name) : Constant(CONSTANT_Class, name->getOwner()), name_(name)
{
}
