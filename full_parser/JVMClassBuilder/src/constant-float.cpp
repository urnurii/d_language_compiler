#include "jvm/constant-float.h"

#include <cstring>

#include "jvm/internal/utils.h"

using namespace jvm;

float ConstantFloat::getValue() const
{
    return value_;
}

void ConstantFloat::writeTo(std::ostream& os) const
{
    Constant::writeTo(os);
    internal::Utils::writeBigEndian(os, value_);
}

std::size_t ConstantFloat::getByteSize() const
{
    return Constant::getByteSize() + sizeof(value_);
}

ConstantFloat::ConstantFloat(float value, Class* classOwner) :
    Constant(CONSTANT_Float, classOwner),
    value_(value)
{
}
