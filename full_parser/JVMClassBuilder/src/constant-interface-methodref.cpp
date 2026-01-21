#include "jvm/constant-interface-methodref.h"

#include <cassert>

#include "jvm/internal/utils.h"

using namespace jvm;

ConstantClass* ConstantInterfaceMethodref::getClass() const
{
    return class_;
}

ConstantNameAndType* ConstantInterfaceMethodref::getNameAndType() const
{
    return nameAndType_;
}

void ConstantInterfaceMethodref::writeTo(std::ostream& os) const
{
    Constant::writeTo(os);

    uint16_t classIndex = class_->getIndex();
    internal::Utils::writeBigEndian(os, classIndex);

    uint16_t nameAndTypeIndex = nameAndType_->getIndex();
    internal::Utils::writeBigEndian(os, nameAndTypeIndex);
}

std::size_t ConstantInterfaceMethodref::getByteSize() const
{
    return Constant::getByteSize() + sizeof(uint16_t) + sizeof(uint16_t);
}

ConstantInterfaceMethodref::ConstantInterfaceMethodref(ConstantClass* classConstant,
                                                       ConstantNameAndType* nameAndTypeConstant) :
    Constant(CONSTANT_InterfaceMethodref, classConstant->getOwner()), class_(classConstant),
    nameAndType_(nameAndTypeConstant)
{
    bool equalClassOwner = classConstant->getOwner() == nameAndTypeConstant->getOwner();
    assert(equalClassOwner);
}
