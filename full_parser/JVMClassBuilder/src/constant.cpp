#include "jvm/constant.h"

#include <ostream>

#include "jvm/internal/utils.h"

using namespace jvm;


Constant::Tag Constant::getTag() const
{
    return tag_;
}

uint16_t Constant::getOccupiedSlots() const
{
    return 1;
}

uint16_t Constant::getIndex() const
{
    return index_;
}

Constant::Constant(Tag tag, Class* classOwner) : ClassFileElement(classOwner), tag_(tag)
{
}

void Constant::writeTo(std::ostream& os) const
{
    internal::Utils::writeBigEndian(os, tag_);
}

void Constant::setIndex(uint32_t index)
{
    index_ = index;
}
