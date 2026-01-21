#include "jvm/field.h"

#include <cassert>
#include <ostream>
#include <utility>

#include "jvm/attribute.h"
#include "jvm/internal/utils.h"

using namespace jvm;


Field::Field(ConstantUtf8Info* name, ConstantUtf8Info* descriptor) :
    ClassFileElement(name->getOwner()), name_(name), descriptor_(descriptor)
{
    bool equalClassOwner = name->getOwner() == descriptor->getOwner();
    assert(equalClassOwner);
}

void Field::addFlag(AccessFlag flag)
{
    uint16_t newFlags = 0;
    for (auto f : accessFlags_)
        newFlags |= f;

    newFlags |= flag;

    validateFlags(newFlags);
    accessFlags_.insert(flag);
}

void Field::removeFlag(AccessFlag flag)
{
    accessFlags_.erase(flag);
}

void Field::addAttribute(Attribute* attribute)
{
    attributes_.insert(attribute);
}

void Field::removeAttribute(Attribute* attribute)
{
    attributes_.erase(attribute);
}

const std::set<Field::AccessFlag>* Field::getAccessFlags() const
{
    return &accessFlags_;
}

ConstantUtf8Info* Field::getName() const
{
    return name_;
}

ConstantUtf8Info* Field::getDescriptor() const
{
    return descriptor_;
}

const std::set<Attribute*>* Field::getAttributes() const
{
    return &attributes_;
}

void Field::writeTo(std::ostream& os) const
{
    // u2             access_flags;
    uint16_t accessFlags = 0x0000;
    for (auto flag : accessFlags_)
    {
        accessFlags = accessFlags | flag;
    }
    internal::Utils::writeBigEndian(os, accessFlags);

    // u2             name_index;
    uint16_t nameIndex = name_->getIndex();
    internal::Utils::writeBigEndian(os, nameIndex);

    // u2             descriptor_index;
    uint16_t descriptorIndex = descriptor_->getIndex();
    internal::Utils::writeBigEndian(os, descriptorIndex);

    // u2             attributes_count;
    uint16_t attributeCount = attributes_.size();
    internal::Utils::writeBigEndian(os, attributeCount);

    // attribute_info attributes[attributes_count];
    for (auto* attribute : attributes_)
    {
        os << *attribute;
    }
}

std::size_t Field::getByteSize() const
{
    size_t size = sizeof(AccessFlag) + sizeof(uint16_t) + sizeof(uint16_t) + sizeof(uint16_t);
    for (auto* attribute : attributes_)
    {
        size += attribute->getByteSize();
    }
    return size;
}

void Field::validateFlags(uint16_t flags)
{
    using internal::Utils;

    // Validate public/protected/private
    int visibilityFlags =
        Utils::hasFlag(flags, ACC_PUBLIC) +
        Utils::hasFlag(flags, ACC_PRIVATE) +
        Utils::hasFlag(flags, ACC_PROTECTED);

    if (visibilityFlags > 1)
    {
        throw std::logic_error(
            "Field cannot have more than one of public/private/protected flags");
    }

    // Validate final and volatile
    if (Utils::hasFlag(flags, ACC_FINAL) && Utils::hasFlag(flags, ACC_VOLATILE))
    {
        throw std::logic_error(
            "Field cannot be both final and volatile");
    }

    // Validate enum with volatile and transient
    if (Utils::hasFlag(flags, ACC_ENUM))
    {
        if (Utils::hasFlag(flags, ACC_VOLATILE))
        {
            throw std::logic_error(
                "Enum field cannot be volatile");
        }

        if (Utils::hasFlag(flags, ACC_TRANSIENT))
        {
            throw std::logic_error(
                "Enum field cannot be transient");
        }
    }
}
