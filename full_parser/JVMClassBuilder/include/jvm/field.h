#ifndef JVM__FIELD_H
#define JVM__FIELD_H
#include <iosfwd>

#include "class-file-element.h"
#include "constant-utf-8-info.h"

namespace jvm
{
    class Attribute;

    class Field final : public ClassFileElement<Class>
    {
        friend class Class;
    public:
        enum AccessFlag
        {
            ACC_PUBLIC = 0x0001, ///< Declared public; may be accessed from outside its package.
            ACC_PRIVATE = 0x0002,
            ///< Declared private; accessible only within the defining class and other classes belonging to the same nest.
            ACC_PROTECTED = 0x0004, ///< Declared protected; may be accessed within subclasses.
            ACC_STATIC = 0x0008, ///< Declared static.
            ACC_FINAL = 0x0010, ///< Declared final; never directly assigned to after object construction (JLS §17.5).
            ACC_VOLATILE = 0x0040, ///< Declared volatile; cannot be cached.
            ACC_TRANSIENT = 0x0080, ///< Declared transient; not written or read by a persistent object manager.
            ACC_SYNTHETIC = 0x1000, ///< Declared synthetic; not present in the source code.
            ACC_ENUM = 0x4000, ///< Declared as an element of an enum class.
        };

        /**
         * Add access flag to field.
         * @param flag Access flag.
         */
        void addFlag(AccessFlag flag);

        /**
         * Remove access flag from field.
         * @param flag Access flag.
         */
        void removeFlag(AccessFlag flag);

        /**
         * Add attribute to field.
         * @param attribute Field attribute.
         */
        void addAttribute(Attribute* attribute);

        /**
         * Remove attribute from field.
         * @param attribute Field attribute.
         */
        void removeAttribute(Attribute* attribute);

        /**
         * @return Access flags set.
         */
        [[nodiscard]] const std::set<AccessFlag>* getAccessFlags() const;

        /**
         * @return Constant of field name.
         */
        [[nodiscard]] ConstantUtf8Info* getName() const;

        /**
         * @return Constant of field descriptor.
         */
        [[nodiscard]] ConstantUtf8Info* getDescriptor() const;

        /**
         * @return Field attributes set.
         */
        [[nodiscard]] const std::set<Attribute*>* getAttributes() const;

        /**
         * @return Class owner.
         */
        [[nodiscard]] Class* getClass() const;

    protected:
        void writeTo(std::ostream& os) const override;

        [[nodiscard]] std::size_t getByteSize() const override;

    private:
        /**
         * Create field with selected name and descriptor.
         * @param name Pointer to UTF-8 constant with field name.
         * @param descriptor Pointer to UTF-8 constant with field descriptor.
         * @note The name and descriptor must belong to the same  owner.
         */
        Field(ConstantUtf8Info* name, ConstantUtf8Info* descriptor);

        /**
         * @brief Validates a raw JVM field access_flags bitmask.
         *
         * Checks that the provided flags set is valid for a field_info structure
         * according to the JVM specification (JVMS §4.5).
         *
         * The validation includes (but is not limited to):
         * - ensuring that at most one of @ref ACC_PUBLIC, @ref ACC_PRIVATE, @ref ACC_PROTECTED is present;
         * - rejecting invalid combinations such as @ref ACC_FINAL together with @ref ACC_VOLATILE;
         * - applying additional restrictions for enum fields marked with @ref ACC_ENUM.
         *
         * @param flags Combined access flags bitmask (OR-ed values of @ref AccessFlag).
         *
         * @throws std::logic_error If the flag set is invalid or contains conflicting flags.
         *
         * @note This function validates only the semantic correctness of the bitmask.
         *       It does not check whether referenced constants (name/descriptor) are valid,
         *       nor does it validate field attributes.
         *
         * @see Field::addFlag
         * @see Field::removeFlag
         */
        static void validateFlags(uint16_t flags);

        std::set<AccessFlag> accessFlags_{}; ///< Access flags.
        ConstantUtf8Info* name_ = nullptr; ///< String constant with field name.
        ConstantUtf8Info* descriptor_ = nullptr; ///< String constant with field descriptor.
        std::set<Attribute*> attributes_{}; ///< Attributes.
    };
} // jvm

#endif //JVM__FIELD_H
