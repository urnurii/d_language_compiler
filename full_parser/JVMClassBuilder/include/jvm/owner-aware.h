#ifndef JVM__OWNER_AWARE_H
#define JVM__OWNER_AWARE_H

#include <cassert>

namespace jvm
{
    /**
     * @brief Base class for objects that have an explicit owning object.
     *
     * Stores a non-owning pointer to an owner object and provides read-only access to it.
     * This class does not manage the lifetime of the owner; it only establishes a logical
     * ownership relationship.
     *
     * Intended to be used as a lightweight building block for expressing ownership
     * hierarchies between class-file elements (e.g. attributes, constants, instructions).
     *
     * @tparam Owner Type of the owning object.
     */
    template <class Owner>
    class OwnerAware
    {
    public:
        /**
         * @brief Construct an object with the given owner.
         *
         * @param owner Pointer to the owning object.
         */
        explicit OwnerAware(Owner* owner) : owner_(owner)
        {
            assert(owner != nullptr);
        }

        /**
         * @return Pointer to the owning object.
         */
        [[nodiscard]] Owner* getOwner() const { return owner_; }

    private:
        Owner* owner_;
    };
} //jvm

#endif //JVM__OWNER_AWARE_H
