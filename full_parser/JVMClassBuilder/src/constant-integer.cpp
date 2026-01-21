#include "jvm/constant-integer.h"

#include <ostream>

#include "jvm/internal/utils.h"

using namespace jvm;

int32_t ConstantInteger::getValue()
{
    return value_;
}

void ConstantInteger::writeTo(std::ostream& os) const
{
    Constant::writeTo(os);
    internal::Utils::writeBigEndian(os, value_);
}

std::size_t ConstantInteger::getByteSize() const
{
    return Constant::getByteSize() + sizeof(value_);
}

ConstantInteger::ConstantInteger(int32_t value, Class* classOwner) : Constant(CONSTANT_Integer, classOwner),
                                                                     value_(value)
{
}
