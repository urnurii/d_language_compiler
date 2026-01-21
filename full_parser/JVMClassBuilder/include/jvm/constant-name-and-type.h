#ifndef JVM__CONSTANT_NAME_AND_TYPE_H
#define JVM__CONSTANT_NAME_AND_TYPE_H

#include "constant-utf-8-info.h"
#include "constant.h"

namespace jvm
{
    /**
     * Name and type constant.
     * Contains information about name and descriptor.
     */
    class ConstantNameAndType : public Constant
    {
        friend class Class;

    public:
        /**
         * @return UTF-8 constant with name.
         */
        [[nodiscard]] ConstantUtf8Info* getName() const;

        /**
         * @return UTF-8 constant with descriptor.
         */
        [[nodiscard]] ConstantUtf8Info* getDescriptor() const;

    protected:
        void writeTo(std::ostream& os) const override;

        [[nodiscard]] std::size_t getByteSize() const override;

    private:
        /**
         * Create name and type constant.
         * @param name UTF-8 constant with name.
         * @param descriptor UTF-8 constant with descriptor.
         */
        ConstantNameAndType(ConstantUtf8Info* name, ConstantUtf8Info* descriptor);

        ConstantUtf8Info* name_; ///< UTF-8 constant with name.
        ConstantUtf8Info* descriptor_; ///< UTF-8 constant with descriptor.
    };
} // jvm

#endif //JVM__CONSTANT_NAME_AND_TYPE_H
