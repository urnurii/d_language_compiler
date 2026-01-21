#ifndef JVM__CONSTANT_STRING_H
#define JVM__CONSTANT_STRING_H

#include "constant-utf-8-info.h"
#include "constant.h"

namespace jvm
{
    /**
     * String constant.
     */
    class ConstantString : public Constant
    {
        friend class Class;

    public:
        /**
         * @return UTF-8 string constant.
         */
        [[nodiscard]] ConstantUtf8Info* getString();

    protected:
        void writeTo(std::ostream& os) const override;

        [[nodiscard]] std::size_t getByteSize() const override;

    private:
        /**
         * Create string constant.
         * @param utf8StringConstant UTF-8 string constant.
         */
        ConstantString(ConstantUtf8Info* utf8StringConstant);

        ConstantUtf8Info* string_; ///< UTF-8 string constant.
    };
} // jvm

#endif //JVM__CONSTANT_STRING_H
