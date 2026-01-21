#include "jvm/constant-string.h"

#include <ostream>

#include "jvm/internal/utils.h"

using namespace jvm;

ConstantUtf8Info* ConstantString::getString()
{
    return string_;
}

void ConstantString::writeTo(std::ostream& os) const
{
    Constant::writeTo(os);
    uint16_t stringIndex = string_->getIndex();
    internal::Utils::writeBigEndian(os, stringIndex);
}

std::size_t ConstantString::getByteSize() const
{
    return Constant::getByteSize() + sizeof(uint16_t);
}

ConstantString::ConstantString(ConstantUtf8Info* utf8StringConstant) :
    Constant(CONSTANT_String, utf8StringConstant->getOwner()), string_(utf8StringConstant)
{
}
