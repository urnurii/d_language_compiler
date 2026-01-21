#include "jvm/constant-name-and-type.h"

#include <cassert>

#include "jvm/internal/utils.h"

namespace jvm
{
    ConstantUtf8Info* ConstantNameAndType::getName() const
    {
        return name_;
    }

    ConstantUtf8Info* ConstantNameAndType::getDescriptor() const
    {
        return descriptor_;
    }

    void ConstantNameAndType::writeTo(std::ostream& os) const
    {
        Constant::writeTo(os);

        uint16_t nameIndex = name_->getIndex();
        internal::Utils::writeBigEndian(os, nameIndex);

        uint16_t descriptorIndex = descriptor_->getIndex();
        internal::Utils::writeBigEndian(os, descriptorIndex);
    }

    std::size_t ConstantNameAndType::getByteSize() const
    {
        return Constant::getByteSize() + 4;
    }

    ConstantNameAndType::ConstantNameAndType(ConstantUtf8Info* name, ConstantUtf8Info* descriptor) :
        Constant(CONSTANT_NameAndType, name->getOwner()), name_(name), descriptor_(descriptor)
    {
        bool equalClassOwners = name->getOwner() == descriptor->getOwner();
        assert(equalClassOwners);
    }
} // jvm
