#include "jvm/constant-methodref.h"

#include <cassert>

#include "jvm/internal/utils.h"

using namespace jvm;

ConstantClass* ConstantMethodref::getClass() const
{
    return class_;
}

ConstantNameAndType* ConstantMethodref::getNameAndType() const
{
    return nameAndType_;
}

void ConstantMethodref::writeTo(std::ostream& os) const
{
    Constant::writeTo(os);

    uint16_t classIndex = class_->getIndex();
    internal::Utils::writeBigEndian(os, classIndex);

    uint16_t nameAndTypeIndex = nameAndType_->getIndex();
    internal::Utils::writeBigEndian(os, nameAndTypeIndex);
}

std::size_t ConstantMethodref::getByteSize() const
{
    return Constant::getByteSize() + 4;
}

ConstantMethodref::ConstantMethodref(ConstantClass* classConstant, ConstantNameAndType* nameAndTypeConstant) :
    Constant(CONSTANT_Methodref, classConstant->getOwner()), class_(classConstant),
    nameAndType_(nameAndTypeConstant)
{
    bool equalClassOwner = classConstant->getOwner() == nameAndTypeConstant->getOwner();
    assert(equalClassOwner);
}
