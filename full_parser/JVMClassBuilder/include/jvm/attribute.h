#ifndef JVM__ATTRIBUTE_H
#define JVM__ATTRIBUTE_H
#include <iosfwd>

#include "constant-utf-8-info.h"
#include "serializable.h"

namespace jvm
{
    class Attribute : public Serializable
    {
        friend class Class;
        friend class Field;
        friend class Method;
        friend class AttributeCode;

    public:
        Attribute(ConstantUtf8Info* name);

        /**
         * @return Attribute name constant.
         */
        [[nodiscard]] ConstantUtf8Info* getName() const noexcept { return name_; }

        /**
         * Is it class @{jvm::Class} attribute.
         * @return Is class attribute.
         */
        [[nodiscard]] virtual bool isClassAttribute() const noexcept { return false; }

        /**
         * Is it field @{jvm::Field} attribute.
         * @return Is field attribute.
         */
        [[nodiscard]] virtual bool isFieldAttribute() const noexcept { return false; }

        /**
         * Is it method @{jvm::Method} attribute.
         * @return Is method attribute.
         */
        [[nodiscard]] virtual bool isMethodAttribute() const noexcept { return false; }

        /**
         * Is it record component attribute.
         * @return Is record component attribute.
         */
        [[nodiscard]] virtual bool isRecordComponentAttribute() const noexcept { return false; }

        /**
         * Is it code attribute.
         * @return Is code attribute.
         */
        [[nodiscard]] virtual bool isCodeAttribute() const noexcept { return false; }

    protected:
        void writeTo(std::ostream& os) const override;

        [[nodiscard]] size_t getByteSize() const override;

        [[nodiscard]] virtual size_t getContentSizeInBytes() const = 0;

    private:
        ConstantUtf8Info* name_ = nullptr; ///< Attribute name constant.
    };
} // jvm

#endif //JVM__ATTRIBUTE_H
