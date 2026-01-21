#ifndef JVM__DESCRIPTOR_FIELD_H
#define JVM__DESCRIPTOR_FIELD_H

#include <cassert>
#include <utility>

#include "descriptor.h"

namespace jvm
{
    /**
     * @brief JVM field type descriptor.
     *
     * Represents a JVM field descriptor, including:
     * - primitive types,
     * - reference types,
     * - array types with arbitrary depth.
     */
    class DescriptorField : public Descriptor
    {
    public:
        /**
         * @brief Construct a primitive field descriptor.
         *
         * @param primitiveFieldType Primitive JVM type.
         * @param arrayDepth Number of array dimensions.
         */
        DescriptorField(Type primitiveFieldType, uint8_t arrayDepth = 0);

        /**
         * @brief Construct a reference field descriptor.
         *
         * @param classReference Fully qualified JVM class name (e.g. "java/lang/String").
         * @param arrayDepth Number of array dimensions.
         */
        DescriptorField(std::string classReference, uint8_t arrayDepth = 0);

        /**
         * @return Field base type.
         */
        [[nodiscard]] Type getType() const { return primitiveFieldType_; }

        /**
         * @return Number of array dimensions.
         */
        [[nodiscard]] uint8_t getArrayDepth() const { return arrayDepth_; }

        /**
         * @return True if this descriptor represents a reference type.
         */
        [[nodiscard]] bool isReferenceType() const { return getType() == Type::Reference; }

        /**
         * @return JVM class reference name (valid only for reference types).
         */
        [[nodiscard]] std::string getClassReference() const { return classReference_; }

        /**
         * @brief Convert field descriptor to JVM string form.
         *
         * @return JVM field descriptor string.
         */
        [[nodiscard]] std::string toString() const override;

    private:
        Type primitiveFieldType_;
        uint8_t arrayDepth_;
        std::string classReference_;
    };
} //jvm

#endif //JVM__DESCRIPTOR_FIELD_H
