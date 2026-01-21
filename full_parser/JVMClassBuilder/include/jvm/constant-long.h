#ifndef JVM__CONSTANT_LONG_H
#define JVM__CONSTANT_LONG_H

#include "constant.h"

namespace jvm
{
    class ConstantLong : public Constant
    {
        friend class Class;

    public:
        /**
         * @return Long value.
         */
        [[nodiscard]] int64_t getValue() const;

        /**
        * @return the number of slots by the constant
        */
        [[nodiscard]] uint16_t getOccupiedSlots() const override;

    protected:
        void writeTo(std::ostream& os) const override;

        [[nodiscard]] std::size_t getByteSize() const override;

    private:
        /**
         * Create long constant.
         * @param value Long value.
         * @param classOwner Class owner.
         */
        ConstantLong(int64_t value, Class* classOwner);

        int64_t value_; ///< Long value.
    };
} // namespace jvm


#endif // JVM__CONSTANT_LONG_H
