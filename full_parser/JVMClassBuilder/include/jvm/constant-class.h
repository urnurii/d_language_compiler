#ifndef JVM__CONSTANT_CLASS_H
#define JVM__CONSTANT_CLASS_H

#include "constant-utf-8-info.h"
#include "constant.h"

namespace jvm
{
    /**
     * Class constant.
     */
    class ConstantClass final : public Constant
    {
        friend class Class;

    public:
        /**
         * @return Utf-8 constant.
         */
        ConstantUtf8Info* getName() const;

    protected:
        void writeTo(std::ostream& os) const override;

        [[nodiscard]] std::size_t getByteSize() const override;

    private:
        /**
         * Create constant class.
         * @param name Utf-8 constant with class name.
         */
        ConstantClass(ConstantUtf8Info* name);

        /**
         * Utf-8 constant with class name.
         */
        ConstantUtf8Info* name_ = nullptr;
    };
} // jvm

#endif //JVM__CONSTANT_CLASS_H
