#ifndef JVM__CONSTANT_METHODREF_H
#define JVM__CONSTANT_METHODREF_H

#include "constant-class.h"
#include "constant-name-and-type.h"
#include "constant.h"

namespace jvm
{
    class ConstantMethodref final : public Constant
    {
        friend class Class;

    public:
        /**
         * @return Class constant.
         */
        [[nodiscard]] ConstantClass* getClass() const;

        /**
         * @return Method name and type constant.
         */
        [[nodiscard]] ConstantNameAndType* getNameAndType() const;

    protected:
        void writeTo(std::ostream& os) const override;

        [[nodiscard]] std::size_t getByteSize() const override;

    private:
        /**
         * Create methodref constant.
         * @param classConstant Class constant.
         * @param nameAndTypeConstant Method name and type constant.
         */
        ConstantMethodref(ConstantClass* classConstant, ConstantNameAndType* nameAndTypeConstant);

        ConstantClass* class_; ///< Class constant.
        ConstantNameAndType* nameAndType_; ///< Method name and type constant.
    };
} // jvm

#endif //JVM__CONSTANT_METHODREF_H
