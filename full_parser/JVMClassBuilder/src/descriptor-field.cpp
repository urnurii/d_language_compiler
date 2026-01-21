#include "jvm/descriptor-field.h"

#include <stdexcept>
#include <utility>

using namespace jvm;

DescriptorField::DescriptorField(Type primitiveFieldType, uint8_t arrayDepth):
    primitiveFieldType_(primitiveFieldType),
    arrayDepth_(arrayDepth)
{
    assert(primitiveFieldType_ != Type::Reference);
}

DescriptorField::DescriptorField(std::string classReference, uint8_t arrayDepth):
    primitiveFieldType_(Type::Reference),
    arrayDepth_(arrayDepth),
    classReference_(std::move(classReference))
{
}

std::string DescriptorField::toString() const
{
    std::string result;

    // write array
    for (uint8_t i = 0; i < arrayDepth_; i++)
    {
        result.push_back('[');
    }

    // write reference content
    result.push_back(primitiveFieldType_);
    if (primitiveFieldType_ == Reference)
    {
        result += classReference_;
        result.push_back(';');
    }

    return result;
}
