#ifndef JVM__CLASS_FILE_ELEMENT_H
#define JVM__CLASS_FILE_ELEMENT_H

#include <cassert>
#include <cstddef>
#include <iosfwd>

#include "owner-aware.h"
#include "serializable.h"

namespace jvm
{
    /**
     * @brief Base class for elements of a JVM class file with explicit ownership.
     *
     * @tparam Owner Type of the logical owner of this element.
     *
     * Copying is disabled to preserve unique ownership semantics.
     *
     * @note The owner type is declared as a friend to allow controlled construction
     *       and internal modification.
     */
    template <class Owner>
    class ClassFileElement : public OwnerAware<Owner>, protected Serializable
    {
        friend Owner;

    public:
        /**
         * @brief Construct a class file element bound to an owner.
         *
         * @param owner Pointer to the owning object.
         * @pre @p owner must not be null.
         */
        explicit ClassFileElement(Owner* owner) : OwnerAware<Owner>(owner)
        {
        }

        /// Copying is disabled to preserve ownership semantics.
        ClassFileElement(const ClassFileElement&) = delete;
        ClassFileElement& operator=(const ClassFileElement&) = delete;

        ClassFileElement(ClassFileElement&&) = default;
        ClassFileElement& operator=(ClassFileElement&&) = default;
    };


    /**
     * @brief Specialization for root class-file elements without an owner.
     *
     * Used for top-level structures that do not belong to any parent element
     * and are responsible for serializing other elements.
     *
     * This specialization exposes the @ref Serializable interface publicly,
     * allowing external serialization of the root object.
     */
    template <>
    class ClassFileElement<void> : public Serializable
    {
    public:
        ClassFileElement(const ClassFileElement&) = delete;
        ClassFileElement& operator=(const ClassFileElement&) = delete;

        ClassFileElement(ClassFileElement&&) = default;
        ClassFileElement& operator=(ClassFileElement&&) = default;
    };
} // jvm

#endif //JVM__CLASS_FILE_ELEMENT_H
