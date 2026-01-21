#include "jvm/constant-double.h"

#include <cstring>

#include "jvm/internal/utils.h"

using namespace jvm;

double ConstantDouble::getValue() const
{
    return value_;
}

uint16_t ConstantDouble::getOccupiedSlots() const
{
    return 2;
}

void ConstantDouble::writeTo(std::ostream& os) const
{
    Constant::writeTo(os);
    internal::Utils::writeBigEndian(os, value_);
}

std::size_t ConstantDouble::getByteSize() const
{
    return Constant::getByteSize() + 8;
}

ConstantDouble::ConstantDouble(double value, Class* classOwner)
    : Constant(CONSTANT_Double, classOwner),
      value_(value)
{
}
