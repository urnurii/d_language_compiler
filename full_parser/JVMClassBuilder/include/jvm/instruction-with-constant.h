#ifndef JVM__INSTRUCTION_WITH_CONSTANT_H
#define JVM__INSTRUCTION_WITH_CONSTANT_H

#include "constant.h"
#include "instruction.h"

namespace jvm
{
    /**
     * @brief Bytecode instruction that references an entry in the constant pool.
     *
     * @note Instances are created and configured by @ref AttributeCode.
     */
    class InstructionWithConstant : public Instruction
    {
        friend class AttributeCode;

    public:
        /**
         * @brief Get the constant referenced by this instruction.
         *
         * @return Referenced constant pool entry.
         */
        [[nodiscard]] Constant* getConstant() const;

    protected:
        /**
         * @brief Supported operand sizes for encoding a constant pool index.
         */
        enum AvailableReferenceSize : uint8_t
        {
            OneByte = 1, ///< Encode constant pool index as one byte.
            TwoByte = 2, ///< Encode constant pool index as two bytes.
        };

        /**
         * @brief Construct an instruction that references a constant pool entry.
         *
         * @param attributeCode Owning code attribute.
         * @param command Opcode / instruction kind.
         * @param constant Referenced constant pool entry.
         * @param size Operand size used to encode the constant pool index.
         */
        InstructionWithConstant(AttributeCode* attributeCode,
                                Command command,
                                Constant* constant,
                                AvailableReferenceSize size);

        /**
         * @brief Change the operand size used to encode the constant pool index.
         *
         * @param size New reference size.
         */
        void setAvailableReferenceSize(AvailableReferenceSize size);

        /**
         * Heirs can overload this method for update available reference size and other parameters.
         * Calls by @ref AttributeCode::finalize.
         */
        virtual void update();

        /**
         * @throws std::out_of_range If the reference size is 1 byte and the constant index does not fit.
         */
        void writeTo(std::ostream& os) const override;

        [[nodiscard]] std::size_t getByteSize() const override;

    private:
        Constant* constant_; ///< Referenced constant pool entry.
        AvailableReferenceSize size_; ///< Operand size used to encode the constant pool index.
    };
} // jvm

#endif //JVM__INSTRUCTION_WITH_CONSTANT_H
