#ifndef JVM__SERIALIZABLE_H
#define JVM__SERIALIZABLE_H

#include <cstddef>
#include <iosfwd>

namespace jvm
{
    /**
     * @brief Interface for binary-serializable objects.
     *
     * Represents an object that can be written to a binary output stream
     * and can report its serialized size in bytes.
     *
     * Implementations must ensure that @ref writeTo writes exactly
     * @ref getByteSize bytes to the output stream.
     *
     * This interface is used as a common abstraction for JVM class-file
     * structures (attributes, instructions, constants, etc.).
     */
    class Serializable
    {
    public:
        /**
         * @brief Virtual destructor.
         */
        virtual ~Serializable() = default;

        /**
         * @brief Write the object to a binary output stream.
         *
         * The object is serialized in its binary JVM class-file representation.
         *
         * @param os Output stream to write to.
         */
        virtual void writeTo(std::ostream& os) const = 0;

        /**
         * @brief Get the size of the serialized object in bytes.
         *
         * @return Number of bytes written by @ref writeTo.
         */
        [[nodiscard]] virtual std::size_t getByteSize() const = 0;
    };

    /**
     * @brief Write a Serializable object to a stream.
     *
     * Calls @ref Serializable::writeTo.
     */
    inline std::ostream& operator<<(std::ostream& os, const Serializable& obj)
    {
        obj.writeTo(os);
        return os;
    }

    /**
     * @brief Write a Serializable object pointer to a stream.
     *
     * Equivalent to dereferencing the pointer and calling the reference overload.
     */
    inline std::ostream& operator<<(std::ostream& os, const Serializable* obj)
    {
        return os << *obj;
    }
} //jvm

#endif //JVM__SERIALIZABLE_H
