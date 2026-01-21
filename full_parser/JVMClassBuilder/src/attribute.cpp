#include "jvm/attribute.h"

#include <ostream>

#include "jvm/internal/utils.h"

using namespace jvm;

Attribute::Attribute(ConstantUtf8Info* name) : name_(name)
{
    assert(name_ != nullptr);
}

void Attribute::writeTo(std::ostream& os) const
{
    // u2 attribute_name_index;
    auto nameIndex = getName()->getIndex();
    internal::Utils::writeBigEndian(os, static_cast<uint16_t>(nameIndex));

    // u4 attribute_length;
    auto length = getContentSizeInBytes();
    internal::Utils::writeBigEndian(os, static_cast<uint32_t>(length));
}

size_t Attribute::getByteSize() const
{
    return 6 + getContentSizeInBytes();
}
