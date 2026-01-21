#ifndef JVM__CONSTANT_INTEGER_H
#define JVM__CONSTANT_INTEGER_H

#include "constant.h"

namespace jvm
{
    class ConstantInteger : public Constant
    {
        friend class Class;

    public:
        /**
         * @return Integer value.
         */
        [[nodiscard]] int32_t getValue();

    protected:
        void writeTo(std::ostream& os) const override;

        [[nodiscard]] std::size_t getByteSize() const override;

    private:
        /**
         * Create integer constant.
         * @param value Integer value.
         * @param classOwner Class owner.
         */
        ConstantInteger(int32_t value, Class* classOwner);

        int32_t value_; ///< Integer value.
    };
} // jvm

#endif //JVM__CONSTANT_INTEGER_H
