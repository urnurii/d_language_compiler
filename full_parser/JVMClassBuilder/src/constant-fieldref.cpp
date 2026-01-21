#include "jvm/constant-fieldref.h"

#include <cassert>

#include "jvm/internal/utils.h"

using namespace jvm;

ConstantClass* ConstantFieldref::getClass() const
{
    return class_;
}

ConstantNameAndType* ConstantFieldref::getNameAndType() const
{
    return nameAndType_;
}

void ConstantFieldref::writeTo(std::ostream& os) const
{
    Constant::writeTo(os);

    uint16_t classIndex = class_->getIndex();
    internal::Utils::writeBigEndian(os, classIndex);

    uint16_t nameAndTypeIndex = nameAndType_->getIndex();
    internal::Utils::writeBigEndian(os, nameAndTypeIndex);
}

std::size_t ConstantFieldref::getByteSize() const
{
    return Constant::getByteSize() + 4;
}

ConstantFieldref::ConstantFieldref(ConstantClass* classConstant, ConstantNameAndType* nameAndTypeConstant) :
    Constant(CONSTANT_Fieldref, classConstant->getOwner()), class_(classConstant), nameAndType_(nameAndTypeConstant)
{
    bool equalClassOwner = classConstant->getOwner() == nameAndTypeConstant->getOwner();
    assert(equalClassOwner);
}
