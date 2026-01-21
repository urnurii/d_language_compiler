#ifndef JVM__CONSTANT_INTERFACE_METHODREF_H
#define JVM__CONSTANT_INTERFACE_METHODREF_H

#include "constant-class.h"
#include "constant-methodref.h"
#include "constant-name-and-type.h"
#include "constant.h"

namespace jvm
{
    class ConstantInterfaceMethodref : public Constant
    {
        friend class Class;

    public:
        /**
         * @return Class constant.
         */
        [[nodiscard]] ConstantClass* getClass() const;

        /**
         * @return Interface method name and type constant.
         */
        [[nodiscard]] ConstantNameAndType* getNameAndType() const;

    protected:
        void writeTo(std::ostream& os) const override;

        [[nodiscard]] std::size_t getByteSize() const override;

    private:
        /**
         * Create interface methodref constant.
         * @param classConstant Class constant.
         * @param nameAndTypeConstant Interface method name and type constant.
         */
        ConstantInterfaceMethodref(ConstantClass* classConstant, ConstantNameAndType* nameAndTypeConstant);

        ConstantClass* class_; ///< Class constant.
        ConstantNameAndType* nameAndType_; ///< Interface method name and type constant.
    };
} //jvm


#endif //JVM__CONSTANT_INTERFACE_METHODREF_H
