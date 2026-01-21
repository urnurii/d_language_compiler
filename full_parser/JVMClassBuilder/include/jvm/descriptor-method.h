#ifndef JVM__DESCRIPTOR_METHOD_H
#define JVM__DESCRIPTOR_METHOD_H

#include <optional>
#include <vector>

#include "descriptor.h"
#include "descriptor-field.h"

namespace jvm
{
    /**
     * @brief JVM method descriptor.
     *
     * Represents a complete JVM method signature, consisting of:
     * - zero or more parameter field descriptors,
     * - an optional return type (absence means void).
     */
    class DescriptorMethod : public Descriptor
    {
    public:
        /**
         * @brief Construct a method descriptor from parameters range.
         *
         * @param returnType Optional return type (@c std::nullopt means void).
         * @param parameters Range of parameter descriptors.
         */
        template <std::ranges::input_range R>
            requires std::same_as<std::ranges::range_value_t<R>, DescriptorField>
        DescriptorMethod(const std::optional<DescriptorField>& returnType, R&& parameters)
            : returnType_(returnType), parameters_(std::ranges::begin(parameters), std::ranges::end(parameters))
        {
        }

        /**
         * @brief Construct a method descriptor from initializer list.
         *
         * @param returnType Optional return type (@c std::nullopt means void).
         * @param parameters Parameter descriptors.
         */
        DescriptorMethod(const std::optional<DescriptorField>& returnType,
                         std::initializer_list<DescriptorField> parameters);

        /**
         * @brief Convert method descriptor to JVM string form.
         *
         * @return JVM method descriptor string.
         */
        [[nodiscard]] std::string toString() const override;

        /**
         * @return Method parameter descriptors.
         */
        [[nodiscard]] const std::vector<DescriptorField>& getParameters() const { return parameters_; }

        /**
         * @return Method return type descriptor, or std::nullopt for void.
         */
        [[nodiscard]] const std::optional<DescriptorField>& getReturn() const { return returnType_; }

    private:
        std::optional<DescriptorField> returnType_;
        std::vector<DescriptorField> parameters_;
    };
} //jvm

#endif //JVM__DESCRIPTOR_METHOD_H
