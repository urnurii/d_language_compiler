#ifndef JVM__LABEL_H
#define JVM__LABEL_H

#include "instruction.h"

namespace jvm
{
    class AttributeCode;

    /**
     * @brief Symbolic position marker used for control-flow targets and exception table ranges.
     *
     * A @c Label is created by @ref AttributeCode and may be placed into the code stream.
     * When placed, it is bound to the next instruction that will be emitted after it.
     * Multiple labels may be placed consecutively; in this case they all refer to the same subsequent instruction.
     *
     * A label may be uninitialized (not yet bound to an instruction). In this case @ref getInstruction
     * returns @c nullptr and @ref isInitialized returns @c false.
     *
     * @note A label belongs to a single @ref AttributeCode instance and must not be used with another owner.
     */
    class Label : public OwnerAware<AttributeCode>
    {
        friend class AttributeCode;

    public:
        /**
         * @brief Get the instruction this label refers to.
         *
         * @return The target instruction, or @c nullptr if the label has not been bound yet.
         */
        [[nodiscard]] Instruction* getInstruction() const;

        /**
         * @brief Check whether this label is already bound to an instruction.
         *
         * @return @c true if the label refers to an instruction; otherwise @c false.
         */
        [[nodiscard]] bool isInitialized() const;

    private:
        /**
         * @brief Create a new label owned by the specified code attribute.
         *
         * Labels are created and configured only by @ref AttributeCode.
         *
         * @param codeAttributeOwner Owning code attribute.
         */
        Label(AttributeCode* codeAttributeOwner);

        /**
         * @brief Bind this label to a concrete instruction.
         *
         * This is called by @ref AttributeCode when emitting an instruction after one or more placed labels.
         *
         * @param instruction Instruction to bind to.
         */
        void setInstruction(Instruction* instruction);

        Instruction* instruction_ = nullptr; ///< Instruction indicated by the label, or @c nullptr if unbound.
    };
} // jvm

#endif //JVM__LABEL_H
