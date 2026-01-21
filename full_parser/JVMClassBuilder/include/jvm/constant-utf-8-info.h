#ifndef JVM__CONSTANT_UTF_8_INFO_H
#define JVM__CONSTANT_UTF_8_INFO_H

#include <string>

#include "constant.h"

namespace jvm
{
    /**
     * Constant of utf8 string.
     */
    class ConstantUtf8Info : public Constant
    {
        friend class Class;

    public:
        /**
         * String getter.
         * @return Utf8 string constant.
         */
        std::string getString() const;

    private:
        /**
         * Create Utf8 constant object.
         * @param string Utf8 string constant.
         * @param classOwner Pointer to class owner object.
         */
        ConstantUtf8Info(std::string string, Class* classOwner);

    protected:
        void writeTo(std::ostream& os) const override;

        [[nodiscard]] std::size_t getByteSize() const override;

    private:
        /**
         * Utf8 string content.
         */
        std::string string_;
    };
}

#endif //JVM__CONSTANT_UTF_8_INFO_H
