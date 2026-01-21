#ifndef JVM__CONSTANT_DOUBLE_H
#define JVM__CONSTANT_DOUBLE_H

#include "constant.h"

namespace jvm
{
    class ConstantDouble : public Constant
    {
        friend class Class;

    public:
        /**
         * @return Double value.
         */
        [[nodiscard]] double getValue() const;

        /**
        * @return the number of slots by the constant
        */
        [[nodiscard]] uint16_t getOccupiedSlots() const override;

    protected:
        void writeTo(std::ostream& os) const override;

        [[nodiscard]] std::size_t getByteSize() const override;

    private:
        /**
         * Create double constant.
         * @param value Double value.
         * @param classOwner Class owner.
         */
        ConstantDouble(double value, Class* classOwner);

        double value_; ///< Double value.
    };
} // namespace jvm


#endif // JVM__CONSTANT_DOUBLE_H
