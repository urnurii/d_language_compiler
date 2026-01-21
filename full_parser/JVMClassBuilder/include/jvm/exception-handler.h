#ifndef JVM__EXCEPTION_HANDLER_H
#define JVM__EXCEPTION_HANDLER_H

#include <cstdint>
#include <iosfwd>

#include "class-file-element.h"

namespace jvm
{
    class AttributeCode;
    class ConstantClass;
    class Label;

    /**
     * @brief Exception table entry of a JVM Code attribute.
     *
     * Represents one entry in the Code attribute's exception_table:
     * <code>
     * u2 start_pc;
     * u2 end_pc;
     * u2 handler_pc;
     * u2 catch_type;
     * </code>
     *
     * The protected region is [start_pc, end_pc) (end is exclusive).
     *
     * If @c catchClass is @c nullptr, the handler is catch-all and @c catch_type is encoded as 0.
     */
    class ExceptionHandler final : public ClassFileElement<AttributeCode>
    {
        friend class AttributeCode;

    public:
        static constexpr size_t sizeInBytes = 8;

        /**
         * @brief Get the start label of the protected region.
         */
        [[nodiscard]] Label* getTryStartLabel() const;

        /**
         * @brief Get the end label of the protected region (end-exclusive).
         */
        [[nodiscard]] Label* getTryFinishLabel() const;

        /**
         * @brief Get the label of the handler entry point.
         */
        [[nodiscard]] Label* getCatchStartLabel() const;

        /**
         * @brief Get the caught exception class or @c nullptr for catch-all.
         */
        [[nodiscard]] ConstantClass* getCatchClass() const;

    protected:
        /**
         * @throws std::logic_error If any required label is not bound to an instruction
         *                          or if instruction positions are not finalized.
         */
        void writeTo(std::ostream& os) const override;

        [[nodiscard]] std::size_t getByteSize() const override;

    private:
        /**
         * @brief Construct an exception handler entry.
         *
         * @param tryStartLabel Label at the start of the protected region.
         * @param tryFinishLabel Label immediately after the protected region (end-exclusive).
         * @param catchStartLabel Label at the start of the handler code.
         * @param catchClass Exception class to catch, or @c nullptr for catch-all.
         * @param owner Owning code attribute.
         */
        ExceptionHandler(Label* tryStartLabel,
                         Label* tryFinishLabel,
                         Label* catchStartLabel,
                         ConstantClass* catchClass,
                         AttributeCode* owner);

        Label* tryStartLabel_;
        Label* tryFinishLabel_;
        Label* catchStartLabel_;
        ConstantClass* catchClass_; // nullptr => catch-all
    };
} // namespace jvm

#endif // JVM__EXCEPTION_HANDLER_H
