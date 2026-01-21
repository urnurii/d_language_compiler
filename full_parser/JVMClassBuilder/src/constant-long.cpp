#include "jvm/constant-long.h"

#include "jvm/internal/utils.h"

using namespace jvm;

int64_t ConstantLong::getValue() const
{
    return value_;
}

uint16_t ConstantLong::getOccupiedSlots() const
{
    return 2;
}

void ConstantLong::writeTo(std::ostream& os) const
{
    Constant::writeTo(os);
    internal::Utils::writeBigEndian(os, value_);
}

std::size_t ConstantLong::getByteSize() const
{
    return Constant::getByteSize() + sizeof(value_);
}

ConstantLong::ConstantLong(int64_t value, Class* classOwner)
    : Constant(CONSTANT_Long, classOwner),
      value_(value)
{
}
