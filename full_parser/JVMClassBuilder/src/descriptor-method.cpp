#include "jvm/descriptor-method.h"

#include <stdexcept>

using namespace jvm;

DescriptorMethod::DescriptorMethod(const std::optional<DescriptorField>& returnType,
    std::initializer_list<DescriptorField> parameters): returnType_(returnType), parameters_(parameters)
{
}

std::string DescriptorMethod::toString() const
{
    std::string result;

    result.push_back('(');
    for (const auto& param : parameters_)
    {
        result += param.toString();
    }
    result.push_back(')');

    if (returnType_)
    {
        result += returnType_->toString();
    }
    else
    {
        result.push_back('V');
    }

    return result;
}
