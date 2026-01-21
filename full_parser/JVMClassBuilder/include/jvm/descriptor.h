#ifndef JVM__DESCRIPTOR_H
#define JVM__DESCRIPTOR_H

#include <cstdint>
#include <string>

namespace jvm
{
    class Descriptor;

    template <class T>
    concept DescriptorType = std::same_as<T, Descriptor> || std::derived_from<T, Descriptor>;

    /**
     * @brief Base interface for JVM descriptors.
     *
     * Represents a JVM type descriptor as defined in the Java Virtual Machine Specification.
     * A descriptor provides a textual JVM encoding of a type or a method signature.
     */
    class Descriptor
    {
    public:
        /**
         * @brief Virtual destructor.
         */
        virtual ~Descriptor() = default;

        /**
         * @brief JVM primitive and reference type tags.
         *
         * Values correspond directly to JVM descriptor characters.
         */
        enum Type : char
        {
            Byte = 'B',
            Char = 'C',
            Double = 'D',
            Float = 'F',
            Int = 'I',
            Long = 'J',
            Short = 'S',
            Boolean = 'Z',
            Reference = 'L'
        };

        /**
         * @brief Convert descriptor to its JVM string representation.
         *
         * @return JVM descriptor string.
         */
        [[nodiscard]] virtual std::string toString() const = 0;
    };
}

#endif //JVM__DESCRIPTOR_H
