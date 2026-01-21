#ifndef JVM__INSTRUCTION_VALUE_H
#define JVM__INSTRUCTION_VALUE_H

#include <concepts>
#include <tuple>

#include "jvm/instruction.h"
#include "jvm/class-file-element.h"
#include "jvm/internal/utils.h"

namespace jvm
{
    /**
     * @brief Operand type constraint for bytecode instruction parameters.
     */
    template <typename T>
    concept BytecodeParameters =
        std::integral<T> || std::floating_point<T>;

    /**
     * @brief Bytecode instruction with inline encoded operand values.
     *
     * Stores a fixed set of operand values (template parameters) and serializes them
     * after the opcode in big-endian order.
     *
     * @tparam Args Operand value types written after the opcode.
     * @note Instances are created by @ref AttributeCode.
     */
    template <BytecodeParameters... Args>
    class InstructionValue : public Instruction
    {
        friend class AttributeCode;

    public:
        [[nodiscard]] size_t getByteSize() const override
        {
            static_assert(
                (sizeof(Args) + ...) <= UINT8_MAX,
                "InstructionValue is too large."
            );

            return Instruction::getByteSize() + (sizeof(Args) + ...);
        }

        void writeTo(std::ostream& os) const override
        {
            Instruction::writeTo(os);

            std::apply([&os](auto&&... xs)
            {
                (internal::Utils::writeBigEndian(os, xs), ...);
            }, values_);
        }

        /**
         * @brief Get the operand value by index.
         *
         * @tparam I Operand index.
         * @return The I-th stored operand value.
         */
        template <std::size_t I>
        decltype(auto) getValue() const
        {
            static_assert(I < sizeof...(Args));
            return std::get<I>(values_);
        }

        /**
         * @brief Convenience accessor for the first operand.
         *
         * @return The first operand value.
         */
        decltype(auto) getFirstValue() const
            requires (sizeof...(Args) >= 1)
        {
            return getValue<0>();
        }

        /**
         * @brief Convenience accessor for the second operand.
         *
         * @return The second operand value.
         */
        decltype(auto) getSecondValue() const
            requires (sizeof...(Args) >= 2)
        {
            return getValue<1>();
        }

        /**
         * @brief Convenience accessor for the third operand.
         *
         * @return The third operand value.
         */
        decltype(auto) getThirdValue() const
            requires (sizeof...(Args) >= 3)
        {
            return getValue<2>();
        }

    protected:
        /**
         * @brief Construct an instruction with inline operand values.
         *
         * @param attributeCode Owning code attribute.
         * @param command Opcode of the instruction.
         * @param values Operand values to be serialized after the opcode.
         */
        InstructionValue(AttributeCode* attributeCode, Command command, Args... values)
            : Instruction(attributeCode, command), values_(values...)
        {
        }

        std::tuple<Args...> values_; ///< Stored operand values.
    };
} //jvm

#endif //JVM__INSTRUCTION_VALUE_H
