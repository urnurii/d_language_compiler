#ifndef JVM__CONSTANT_H
#define JVM__CONSTANT_H

#include <cstdint>

#include "class-file-element.h"
#include "class.h"

namespace jvm
{
    /**
     * Base class of constant.
     */
    class Constant : public ClassFileElement<Class>
    {
        friend class Class;
    public:
        /**
         * Constant values tag.
         */
        enum Tag:uint8_t
        {
            CONSTANT_Class = 7,
            CONSTANT_Fieldref = 9,
            CONSTANT_Methodref = 10,
            CONSTANT_InterfaceMethodref = 11,
            CONSTANT_String = 8,
            CONSTANT_Integer = 3,
            CONSTANT_Float = 4,
            CONSTANT_Long = 5,
            CONSTANT_Double = 6,
            CONSTANT_NameAndType = 12,
            CONSTANT_Utf8 = 1,
            CONSTANT_MethodHandle = 15,
            CONSTANT_MethodType = 16,
            CONSTANT_Dynamic = 17,
            CONSTANT_InvokeDynamic = 18,
            CONSTANT_Module = 19,
            CONSTANT_Package = 20
        };

        /**
         * @return Tag of constant value.
         */
        Tag getTag() const;

        /**
         * @return the number of slots by the constant
         */
        [[nodiscard]] virtual uint16_t getOccupiedSlots() const;

        /**
         * @return Index in the table of constants.
         */
        uint16_t getIndex() const;

    protected:
        /**
         * Create constant with tag and class owner.
         * Also set index in the table of constants.
         * @param tag Tag of constant.
         * @param classOwner Constant owner class.
         */
        explicit Constant(Tag tag, Class* classOwner);

        void writeTo(std::ostream& os) const override;

        [[nodiscard]] std::size_t getByteSize() const override { return 1; };

    private:
        /**
         * @param index Index in the table of constants.
         */
        void setIndex(uint32_t index);

        Tag tag_; ///< Tag of constant.
        uint16_t index_ = 0; ///< Index in the table of constants.
    };
}
#endif //JVM__CONSTANT_H
