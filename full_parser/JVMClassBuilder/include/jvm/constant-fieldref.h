#ifndef JVM__CONSTANT_FIELDREF_H
#define JVM__CONSTANT_FIELDREF_H

#include "constant-class.h"
#include "constant-name-and-type.h"
#include "constant.h"

namespace jvm
{
    class ConstantFieldref final : public Constant
    {
        friend class Class;

    public:
        /**
         * @return Class constant.
         */
        [[nodiscard]] ConstantClass* getClass() const;

        /**
         * @return Field name and type constant.
         */
        [[nodiscard]] ConstantNameAndType* getNameAndType() const;

    protected:
        void writeTo(std::ostream& os) const override;

        [[nodiscard]] std::size_t getByteSize() const override;

    private:
        /**
         * Create fieldref constant.
         * @param classConstant Class constant.
         * @param nameAndTypeConstant Field name and type constant.
         */
        ConstantFieldref(ConstantClass* classConstant, ConstantNameAndType* nameAndTypeConstant);

        ConstantClass* class_; ///< Class constant.
        ConstantNameAndType* nameAndType_; ///< Field name and type constant.
    };
} //jvm


#endif //JVM__CONSTANT_FIELDREF_H
