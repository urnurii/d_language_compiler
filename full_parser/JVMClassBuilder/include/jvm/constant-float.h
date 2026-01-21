#ifndef JVM__CONSTANT_FLOAT_H
#define JVM__CONSTANT_FLOAT_H

#include "constant.h"

namespace jvm
{
    class ConstantFloat : public Constant
    {
        friend class Class;

    public:
        /**
         * @return Float value.
         */
        [[nodiscard]] float getValue() const;

    protected:
        void writeTo(std::ostream& os) const override;

        [[nodiscard]] std::size_t getByteSize() const override;

    private:
        /**
         * Create float constant.
         * @param value Float value.
         * @param classOwner Class owner.
         */
        ConstantFloat(float value, Class* classOwner);

        float value_; ///< Float value.
    };
} // namespace jvm

#endif // JVM__CONSTANT_FLOAT_H
