#ifndef JVM__INSTRUCTION_JUMP_H
#define JVM__INSTRUCTION_JUMP_H

#include "instruction.h"
#include "label.h"

namespace jvm
{
    /**
     * @brief Branch instruction that transfers control to a target @ref Label.
     *
     * Represents JVM control-flow instructions whose operand is a 2-byte signed branch offset.
     *
     * The target is specified symbolically via a @ref Label. During serialization the label must
     * already be bound to a concrete target instruction; otherwise the instruction cannot be encoded.
     *
     * @note Instances are created by @ref AttributeCode.
     */
    class InstructionJump : public Instruction
    {
        friend class AttributeCode;

    public:
        /**
         * @brief Get the target label of this jump.
         *
         * @return Target label.
         */
        [[nodiscard]] Label* getJumpLabel() const;

    protected:
        /**
         * @brief Construct a 2-byte branch instruction.
         *
         * @param attributeCode Owning code attribute.
         * @param command Jump opcode (e.g. @c ifeq, @c goto).
         * @param label Target label to branch to.
         */
        InstructionJump(AttributeCode* attributeCode, Command command, Label* label);

        /**
         * @throws std::logic_error If the target label is not bound to any instruction.
         */
        void writeTo(std::ostream& os) const override;

        [[nodiscard]] size_t getByteSize() const override;

        Label* label_; ///< Target label (non-owning).
    };
} // jvm

#endif //JVM__INSTRUCTION_JUMP_H
