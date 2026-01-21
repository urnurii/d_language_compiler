#include "jvm/method.h"

#include <cassert>
#include <ostream>
#include <utility>

#include "jvm/internal/utils.h"

using namespace jvm;

void Method::addFlag(AccessFlag flag)
{
    uint16_t newFlags = 0;
    for (auto f : accessFlags_)
        newFlags |= f;

    newFlags |= flag;

    validateFlags(newFlags);
    accessFlags_.insert(flag);
}

void Method::removeFlag(AccessFlag flag)
{
    accessFlags_.erase(flag);
}

void Method::addAttribute(Attribute* attribute)
{
    attributes_.insert(attribute);
}

void Method::removeAttribute(Attribute* attribute)
{
    attributes_.erase(attribute);
}

const std::set<Method::AccessFlag>* Method::getAccessFlags() const
{
    return &accessFlags_;
}

ConstantUtf8Info* Method::getName() const
{
    return name_;
}

ConstantUtf8Info* Method::getDescriptor() const
{
    return descriptor_;
}

const std::set<Attribute*>* Method::getAttributes() const
{
    return &attributes_;
}

AttributeCode* Method::getCodeAttribute()
{
    if (codeAttribute_ == nullptr)
    {
        codeAttribute_ = new AttributeCode(this);
        attributes_.insert(codeAttribute_);
    }
    return codeAttribute_;
}

void Method::writeTo(std::ostream& os) const
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

    if (codeAttribute_ != nullptr) { codeAttribute_->finalize(); }
    // attribute_info attributes[attributes_count];
    for (auto* attribute : attributes_)
    {
        os << *attribute;
    }
}

std::size_t Method::getByteSize() const
{
    size_t size = sizeof(AccessFlag) + sizeof(uint16_t) + sizeof(uint16_t) + sizeof(uint16_t);
    for (auto* attribute : attributes_)
    {
        size += attribute->getByteSize();
    }
    return size;
}

Method::Method(ConstantUtf8Info* name, ConstantUtf8Info* descriptor) :
    ClassFileElement(name->getOwner()), name_(name), descriptor_(descriptor)
{
    Class* nameOwner = name->getOwner();
    Class* descriptorOwner = descriptor->getOwner();
    assert(nameOwner == descriptorOwner);
}

void Method::validateFlags(uint16_t flags)
{
    using internal::Utils;

    // Validate public/protected/private
    int visibilityFlags = Utils::hasFlag(flags, ACC_PUBLIC) + Utils::hasFlag(flags, ACC_PRIVATE) + Utils::hasFlag(
        flags, ACC_PROTECTED);
    if (visibilityFlags > 1)
    {
        throw std::logic_error(
            "Method cannot have more than one of public/private/protected flags");
    }

    // Validate abstract with final/native/synchronized
    if (Utils::hasFlag(flags, ACC_ABSTRACT))
    {
        if (Utils::hasFlag(flags, ACC_FINAL))
            throw std::logic_error("Abstract method cannot be final");

        if (Utils::hasFlag(flags, ACC_NATIVE))
            throw std::logic_error("Abstract method cannot be native");

        if (Utils::hasFlag(flags, ACC_SYNCHRONIZED))
            throw std::logic_error("Abstract method cannot be synchronized");
    }

    // Validate native with synchronized
    if (Utils::hasFlag(flags, ACC_NATIVE) && Utils::hasFlag(flags, ACC_SYNCHRONIZED))
    {
        throw std::logic_error("Native method cannot be synchronized");
    }
}
