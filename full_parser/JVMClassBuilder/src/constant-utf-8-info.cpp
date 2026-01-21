#include "jvm/constant-utf-8-info.h"

#include <ostream>

#include "jvm/internal/utils.h"

using namespace jvm;

std::string ConstantUtf8Info::getString() const
{
    return string_;
}

ConstantUtf8Info::ConstantUtf8Info(std::string string, Class* classOwner) :
    Constant(CONSTANT_Utf8, classOwner), string_(string)
{
}

void ConstantUtf8Info::writeTo(std::ostream& os) const
{
    Constant::writeTo(os);
    uint16_t size = string_.size();
    internal::Utils::writeBigEndian(os, size);
    os.write(string_.c_str(), size);
}

std::size_t ConstantUtf8Info::getByteSize() const
{
    return Constant::getByteSize() + sizeof(uint16_t) + string_.size();
}
