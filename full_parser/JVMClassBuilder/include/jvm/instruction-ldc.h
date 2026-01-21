#ifndef JVM__INSTRUCTION_LDC_H
#define JVM__INSTRUCTION_LDC_H

#include "instruction-with-constant.h"

namespace jvm
{
    /**
     * @brief Instruction "ldc*". Load a constant from the constant pool onto the operand stack.
     *
     * Represents the family of JVM instructions:
     * - @ref INSTRUCTION_ldc   (u1 index)
     * - @ref INSTRUCTION_ldc_w (u2 index)
     * - @ref INSTRUCTION_ldc2_w (u2 index, category-2 constants)
     *
     * The actual opcode and operand size are selected automatically based on:
     * - the constant tag (type of constant pool entry),
     * - and the constant pool index (whether it fits into 1 byte for @c ldc).
     *
     * Supported constant tags:
     * - For @c ldc / @c ldc_w:
     *   @ref Constant::CONSTANT_Integer,
     *   @ref Constant::CONSTANT_Float,
     *   @ref Constant::CONSTANT_String,
     *   @ref Constant::CONSTANT_Class,
     *   @ref Constant::CONSTANT_MethodHandle,
     *   @ref Constant::CONSTANT_MethodType,
     *   @ref Constant::CONSTANT_Dynamic.
     * - For @c ldc2_w:
     *   @ref Constant::CONSTANT_Long,
     *   @ref Constant::CONSTANT_Double.
     *
     * Any other constant tag is rejected.
     *
     * @note Instances are created by @ref AttributeCode. The instruction assumes that the referenced
     *       constant has (or will have) a valid constant pool index by the time the code is serialized.
     */
    class InstructionLdc final : public InstructionWithConstant
    {
        friend class AttributeCode;

    private:
        /**
         * @brief Construct an LDC-family instruction for the given constant.
         *
         * The initial opcode is selected from the constant tag. The final opcode and reference size
         * are refined in @ref update once the constant pool index is known.
         *
         * @param attributeCode Owning code attribute.
         * @param constant Constant pool entry to load.
         *
         * @throws std::invalid_argument If the constant tag is not supported by ldc* instructions.
         */
        InstructionLdc(AttributeCode* attributeCode, Constant* constant);

        /**
         * @brief Select an initial opcode based on the constant tag.
         *
         * Returns @ref INSTRUCTION_ldc_w for category-1 supported tags, and @ref INSTRUCTION_ldc2_w
         * for category-2 constants (long/double). The final choice between @c ldc and @c ldc_w
         * depends on the constant pool index and is performed in @ref update.
         *
         * @param constant Constant pool entry to load.
         * @return Selected opcode for initial construction.
         *
         * @throws std::invalid_argument If the constant tag is not supported by ldc* instructions.
         */
        [[nodiscard]] static Command selectInstruction(Constant* constant);

    protected:
        /**
         * @brief Update opcode and operand size according to the constant tag and constant pool index.
         *
         * @throws std::invalid_argument If the constant tag is not supported by ldc* instructions.
         */
        void update() override;
    };
} // jvm

#endif //JVM__INSTRUCTION_LDC_H
