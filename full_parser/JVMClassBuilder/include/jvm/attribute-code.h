#ifndef JVM__ATTRIBUTE_CODE_H
#define JVM__ATTRIBUTE_CODE_H

#include <map>
#include <memory>

#include "attribute.h"
#include "exception-handler.h"
#include "instruction.h"
#include "instruction-jump.h"


namespace jvm
{
    /**
     * @brief Code attribute.
     *
     * Manages the complete bytecode body of a method, including:
     * - the ordered instruction stream,
     * - labels used as control-flow targets,
     * - exception handlers (exception_table),
     * - nested attributes of the Code attribute.
     *
     * Instructions and labels are created by the user and may be optionally
     * registered into this instance using @ref addInstruction and @ref addLabel.
     * Once registered, their lifetime is fully managed by AttributeCode.
     *
     * Exception handlers registered via @ref addTryCatch are always owned
     * by AttributeCode and are never transferred to the user.
     *
     * The typical workflow is:
     * 1) Create instructions and labels.
     * 2) Register them into the code stream. Add exception handlers.
     * 3) Call @ref finalize to bind labels, update instructions, and compute offsets.
     * 4) Serialize the attribute via @ref toBinary.
     *
     * @note After @ref finalize is called, the instance becomes immutable.
     * Example (ownership):
     * @code
     * auto* L = code->CodeLabel();              // caller owns label
     * auto* I = code->Nop();                    // caller owns instruction
     * *code << L << I;                          // ownership transferred to AttributeCode
     * @endcode
     */
    class AttributeCode final : public Attribute, public ClassFileElement<Method>
    {
        friend class Method;

    public:
        ~AttributeCode() override;

        // region INSTRUCTIONS
        // region STATIC CONSTRUCTORS
        /**
         * @brief Do nothing.
         *
         * Command: @ref INSTRUCTION_nop.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* Nop();

        /**
         * @brief Push \b null onto stack.
         *
         * Before:
         * @code
         * ...
         * @endcode
         *
         * After:
         * @code
         * ..., null
         * @endcode
         *
         * Command: @ref INSTRUCTION_aconst_null.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* PushNull();

        /**
         * @brief Push @b integer value onto stack.
         *
         * Before:
         * @code
         * ...
         * @endcode
         *
         * After:
         * @code
         * ..., value (integer)
         * @endcode
         *
         * Use specialized instructions:
         * | Value | Command |
         * |-------|---------|
         * | -1    | @ref INSTRUCTION_iconst_m1 |
         * | 0     | @ref INSTRUCTION_iconst_0  |
         * | 1     | @ref INSTRUCTION_iconst_1  |
         * | 2     | @ref INSTRUCTION_iconst_2  |
         * | 3     | @ref INSTRUCTION_iconst_3  |
         * | 4     | @ref INSTRUCTION_iconst_4  |
         * | 5     | @ref INSTRUCTION_iconst_5  |
         *
         * For <b>value in ([-128, -1) ∪ (5, 127])</b> use @ref INSTRUCTION_bipush.
         *
         * For <b>value in ([-32_768, -128) ∪ (127, 32_767])</b> use @ref INSTRUCTION_sipush.
         *
         * For <b>value in ([-2_147_483_648, -32_768) ∪ (32_767, 2_147_483_647])</b>
         * use @ref INSTRUCTION_ldc or @ref INSTRUCTION_ldc_w. Can create new @ref ConstantInteger.
         *
         * @param value Integer value.
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* PushInt(int32_t value);

        /**
         * @brief Push @b long value onto stack.
         * @note Long values occupy two words on the operand stack (category 2).
         *
         * Before:
         * @code
         * ...
         * @endcode
         *
         * After:
         * @code
         * ..., value (long)
         * @endcode <br>
         *
         * Use specialized instructions:
         * | Value | Command |
         * |-------|---------|
         * | 0     | @ref INSTRUCTION_lconst_0  |
         * | 1     | @ref INSTRUCTION_lconst_1  |
         *
         * For <b>values other than 0, 1</b> use @ref INSTRUCTION_ldc2_w. Can create new @ref ConstantLong.
         *
         * @param value Long value.
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* PushLong(int64_t value);

        /**
         * @brief Push @b float value onto stack.
         *
         * Before:
         * @code
         * ...
         * @endcode
         *
         * After:
         * @code
         * ..., value (float)
         * @endcode
         *
         * Use specialized instructions:
         * | Value | Command |
         * |-------|---------|
         * | 0     | @ref INSTRUCTION_fconst_0  |
         * | 1     | @ref INSTRUCTION_fconst_1  |
         * | 2     | @ref INSTRUCTION_fconst_2  |
         *
         * For <b>values other than 0, 1, 2</b> use @ref INSTRUCTION_ldc or @ref INSTRUCTION_ldc_w. Can create new @ref ConstantFloat.
         *
         * @param value Float value.
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* PushFloat(float value);

        /**
         * @brief Push @b double value onto stack.
         * @note Long values occupy two words on the operand stack (category 2).
         *
         * Before:
         * @code
         * ...
         * @endcode
         *
         * After:
         * @code
         * ..., value (double)
         * @endcode <br>
         *
         * Use specialized instructions:
         * | Value | Command |
         * |-------|---------|
         * | 0     | @ref INSTRUCTION_dconst_0  |
         * | 1     | @ref INSTRUCTION_dconst_1  |
         *
         * For <b>values other than 0 and 1</b> use @ref INSTRUCTION_ldc2_w. Can create new @ref ConstantDouble.
         *
         * @param value Double value.
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* PushDouble(double value);

        /**
         * @brief Push a {@c java.lang.String} reference onto the operand stack.
         *
         * Pushes a string constant from the constant pool onto the operand stack.
         *
         * Before:
         * @code
         * ...
         * @endcode
         *
         * After:
         * @code
         * ..., value (reference to String)
         * @endcode
         *
         * Uses {@ref INSTRUCTION_ldc} or {@ref INSTRUCTION_ldc_w} depending on the constant pool index size.
         *
         * The {@ref ConstantString} must belong to the same class constant pool as this code attribute.
         *
         * @param stringConstant String constant to push.
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using {@ref addInstruction}.
         */
        [[nodiscard]] Instruction* PushString(ConstantString* stringConstant);

        /**
         * @brief Push a {@c java.lang.String} reference onto the operand stack.
         *
         * Pushes a string constant from the constant pool onto the operand stack.
         *
         * Before:
         * @code
         * ...
         * @endcode
         *
         * After:
         * @code
         * ..., value (reference to String)
         * @endcode
         *
         * Uses {@ref INSTRUCTION_ldc} or {@ref INSTRUCTION_ldc_w} depending on
         * the constant pool index size. Can create new @ref ConstantDouble.
         *
         * @param value String value to push.
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using {@ref addInstruction}.
         */
        [[nodiscard]] Instruction* PushString(const std::string& value);

        /**
         * @brief Loads an @b integer value from the local variable array at the given index
         *        and pushes it onto the operand stack.
         * @note In the JVM local variable array, long values occupy two consecutive slots; index refers to the first slot.
         *
         * Uses specialized instructions for small indexes:
         * | Index | Instruction |
         * |-------|-------------|
         * | 0     | @ref INSTRUCTION_iload_0 |
         * | 1     | @ref INSTRUCTION_iload_1 |
         * | 2     | @ref INSTRUCTION_iload_2 |
         * | 3     | @ref INSTRUCTION_iload_3 |
         *
         * For indexes other than 0–3, uses @ref INSTRUCTION_iload.
         *
         * @param index The local variable index.
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* LoadInt(uint16_t index);

        /**
         * @brief Loads a @b long value from the local variable array at the given index
         *        and pushes it onto the operand stack.
         *
         * Uses specialized instructions for small indexes:
         * | Index | Instruction |
         * |-------|-------------|
         * | 0     | @ref INSTRUCTION_lload_0 |
         * | 1     | @ref INSTRUCTION_lload_1 |
         * | 2     | @ref INSTRUCTION_lload_2 |
         * | 3     | @ref INSTRUCTION_lload_3 |
         *
         * For indexes other than 0–3, uses @ref INSTRUCTION_lload.
         *
         * @param index The local variable index.
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* LoadLong(uint16_t index);

        /**
         * @brief Loads a @b float value from the local variable array at the given index
         *        and pushes it onto the operand stack.
         *
         * Uses specialized instructions for small indexes:
         * | Index | Instruction |
         * |-------|-------------|
         * | 0     | @ref INSTRUCTION_fload_0 |
         * | 1     | @ref INSTRUCTION_fload_1 |
         * | 2     | @ref INSTRUCTION_fload_2 |
         * | 3     | @ref INSTRUCTION_fload_3 |
         *
         * For indexes other than 0–3, uses @ref INSTRUCTION_fload.
         *
         * @param index The local variable index.
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* LoadFloat(uint16_t index);

        /**
         * @brief Loads a @b double value from the local variable array at the given index
         *        and pushes it onto the operand stack.
         * @note In the JVM local variable array, double values occupy two consecutive slots; index refers to the first slot.
         *
         * Uses specialized instructions for small indexes:
         * | Index | Instruction |
         * |-------|-------------|
         * | 0     | @ref INSTRUCTION_dload_0 |
         * | 1     | @ref INSTRUCTION_dload_1 |
         * | 2     | @ref INSTRUCTION_dload_2 |
         * | 3     | @ref INSTRUCTION_dload_3 |
         *
         * For indexes other than 0–3, uses @ref INSTRUCTION_dload.
         *
         * @param index The local variable index.
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* LoadDouble(uint16_t index);

        /**
         * @brief Loads a @b reference value from the local variable array at the given index
         *        and pushes it onto the operand stack.
         *
         * Uses specialized instructions for small indexes:
         * | Index | Instruction |
         * |-------|-------------|
         * | 0     | @ref INSTRUCTION_aload_0 |
         * | 1     | @ref INSTRUCTION_aload_1 |
         * | 2     | @ref INSTRUCTION_aload_2 |
         * | 3     | @ref INSTRUCTION_aload_3 |
         *
         * For indexes other than 0–3, uses @ref INSTRUCTION_aload.
         *
         * @param index The local variable index.
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* LoadReference(uint16_t index);

        /**
         * @brief Loads a @b boolean value from an array and pushes it onto the operand stack as an integer.
         *
         * Before:
         * @code
         * ..., arrayref, index
         * @endcode
         *
         * After:
         * @code
         * ..., value (integer)
         * @endcode
         *
         * Command: @ref INSTRUCTION_baload.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* LoadBooleanFromArray();

        /**
         * @brief Loads a @b byte value from an array and pushes it onto the operand stack as an integer.
         *
         * Before:
         * @code
         * ..., arrayref, index
         * @endcode
         *
         * After:
         * @code
         * ..., value (integer)
         * @endcode
         *
         * Command: @ref INSTRUCTION_baload.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* LoadByteFromArray();

        /**
         * @brief Loads a @b char value from an array and pushes it onto the operand stack as an integer.
         *
         * Before:
         * @code
         * ..., arrayref, index
         * @endcode
         *
         * After:
         * @code
         * ..., value (integer)
         * @endcode
         *
         * Command: @ref INSTRUCTION_caload.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* LoadCharFromArray();

        /**
         * @brief Loads a @b short value from an array and pushes it onto the operand stack as an integer.
         *
         * Before:
         * @code
         * ..., arrayref, index
         * @endcode
         *
         * After:
         * @code
         * ..., value (integer)
         * @endcode
         *
         * Command: @ref INSTRUCTION_saload.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* LoadShortFromArray();

        /**
         * @brief Loads an @b integer value from an array and pushes it onto the operand stack.
         *
         * Before:
         * @code
         * ..., arrayref, index
         * @endcode
         *
         * After:
         * @code
         * ..., value (integer)
         * @endcode
         *
         * Command: @ref INSTRUCTION_iaload.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* LoadIntFromArray();

        /**
         * @brief Loads a @b long value from an array and pushes it onto the operand stack.
         * @note Long values occupy two words on the operand stack (category 2).
         *
         * Before:
         * @code
         * ..., arrayref, index
         * @endcode
         *
         * After:
         * @code
         * ..., value (long)
         * @endcode
         *
         * Command: @ref INSTRUCTION_laload.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* LoadLongFromArray();

        /**
         * @brief Loads a @b float value from an array and pushes it onto the operand stack.
         *
         * Before:
         * @code
         * ..., arrayref, index
         * @endcode
         *
         * After:
         * @code
         * ..., value (float)
         * @endcode
         *
         * Command: @ref INSTRUCTION_faload.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* LoadFloatFromArray();

        /**
         * @brief Loads a @b double value from an array and pushes it onto the operand stack.
         * @note Double values occupy two words on the operand stack (category 2).
         *
         * Before:
         * @code
         * ..., arrayref, index
         * @endcode
         *
         * After:
         * @code
         * ..., value (double)
         * @endcode
         *
         * Command: @ref INSTRUCTION_daload.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* LoadDoubleFromArray();

        /**
         * @brief Loads a @b reference value from an array and pushes it onto the operand stack.
         *
         * Before:
         * @code
         * ..., arrayref, index
         * @endcode
         *
         * After:
         * @code
         * ..., value (reference)
         * @endcode
         *
         * Command: @ref INSTRUCTION_aaload.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* LoadReferenceFromArray();

        /**
         * @brief Pop an @b integer value from the top of the stack and store it in the local variable array at the given index.
         *
         * Before:
         * @code
         * ..., value (integer)
         * @endcode
         *
         * After:
         * @code
         * ...
         * @endcode
         *
         * Uses specialized instructions for small indexes:
         * | Index | Instruction |
         * |-------|-------------|
         * | 0     | @ref INSTRUCTION_istore_0 |
         * | 1     | @ref INSTRUCTION_istore_1 |
         * | 2     | @ref INSTRUCTION_istore_2 |
         * | 3     | @ref INSTRUCTION_istore_3 |
         *
         * For <b>index in [4, 255]</b> use @ref INSTRUCTION_istore.
         *
         * For <b>index in [256, 65535]</b> use @ref INSTRUCTION_wide.
         *
         * @param index The local variable index.
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* StoreInt(uint16_t index);

        /**
         * @brief Pop a @b long value from the top of the stack and store it in the local variable array at the given index.
         * @note Long values occupy two words on the operand stack (category 2).
         * @note In the JVM local variable array, long values occupy two consecutive slots; index refers to the first slot.
         *
         * Before:
         * @code
         * ..., value (long)
         * @endcode
         *
         * After:
         * @code
         * ...
         * @endcode
         *
         * Uses specialized instructions for small indexes:
         * | Index | Instruction |
         * |-------|-------------|
         * | 0     | @ref INSTRUCTION_lstore_0 |
         * | 1     | @ref INSTRUCTION_lstore_1 |
         * | 2     | @ref INSTRUCTION_lstore_2 |
         * | 3     | @ref INSTRUCTION_lstore_3 |
         *
         * For <b>index in [4, 255]</b> use @ref INSTRUCTION_lstore.
         *
         * For <b>index in [256, 65535]</b> use @ref INSTRUCTION_wide.
         *
         * @param index The local variable index.
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* StoreLong(uint16_t index);

        /**
         * @brief Pop a @b float value from the top of the stack and store it in the local variable array at the given index.
         *
         * Before:
         * @code
         * ..., value (float)
         * @endcode
         *
         * After:
         * @code
         * ...
         * @endcode
         *
         * Uses specialized instructions for small indexes:
         * | Index | Instruction |
         * |-------|-------------|
         * | 0     | @ref INSTRUCTION_fstore_0 |
         * | 1     | @ref INSTRUCTION_fstore_1 |
         * | 2     | @ref INSTRUCTION_fstore_2 |
         * | 3     | @ref INSTRUCTION_fstore_3 |
         *
         * For <b>index in [4, 255]</b> use @ref INSTRUCTION_fstore.
         *
         * For <b>index in [256, 65535]</b> use @ref INSTRUCTION_wide.
         *
         * @param index The local variable index.
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* StoreFloat(uint16_t index);

        /**
         * @brief Pop a @b double value from the top of the stack and store it in the local variable array at the given index.
         * @note Double values occupy two words on the operand stack (category 2).
         * @note In the JVM local variable array, double values occupy two consecutive slots; index refers to the first slot.
         *
         * Before:
         * @code
         * ..., value (double)
         * @endcode
         *
         * After:
         * @code
         * ...
         * @endcode
         *
         * Uses specialized instructions for small indexes:
         * | Index | Instruction |
         * |-------|-------------|
         * | 0     | @ref INSTRUCTION_dstore_0 |
         * | 1     | @ref INSTRUCTION_dstore_1 |
         * | 2     | @ref INSTRUCTION_dstore_2 |
         * | 3     | @ref INSTRUCTION_dstore_3 |
         *
         * For <b>index in [4, 255]</b> use @ref INSTRUCTION_dstore.
         *
         * For <b>index in [256, 65535]</b> use @ref INSTRUCTION_wide.
         *
         * @param index The local variable index.
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* StoreDouble(uint16_t index);

        /**
         * @brief Pop a @b reference value from the top of the stack and store it in the local variable array at the given index.
         *
         * Before:
         * @code
         * ..., value (reference)
         * @endcode
         *
         * After:
         * @code
         * ...
         * @endcode
         *
         * Uses specialized instructions for small indexes:
         * | Index | Instruction |
         * |-------|-------------|
         * | 0     | @ref INSTRUCTION_astore_0 |
         * | 1     | @ref INSTRUCTION_astore_1 |
         * | 2     | @ref INSTRUCTION_astore_2 |
         * | 3     | @ref INSTRUCTION_astore_3 |
         *
         * For <b>index in [4, 255]</b> use @ref INSTRUCTION_astore.
         *
         * For <b>index in [256, 65535]</b> use @ref INSTRUCTION_wide.
         *
         * @param index The local variable index.
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* StoreReference(uint16_t index);

        /**
         * @brief Store an @b integer value into an array.
         *
         * Before:
         * @code
         * ..., arrayref, index, value (integer)
         * @endcode
         *
         * After:
         * @code
         * ...
         * @endcode
         *
         * Command: @ref INSTRUCTION_iastore.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* StoreIntToArray();

        /**
         * @brief Store a @b long value into an array.
         * @note Long values occupy two words on the operand stack (category 2).
         *
         * Before:
         * @code
         * ..., arrayref, index, value (long)
         * @endcode
         *
         * After:
         * @code
         * ...
         * @endcode
         *
         * Command: @ref INSTRUCTION_lastore.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* StoreLongToArray();

        /**
         * @brief Store a @b float value into an array.
         *
         * Before:
         * @code
         * ..., arrayref, index, value (float)
         * @endcode
         *
         * After:
         * @code
         * ...
         * @endcode
         *
         * Command: @ref INSTRUCTION_fastore.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* StoreFloatToArray();

        /**
         * @brief Store a @b double value into an array.
         * @note Double values occupy two words on the operand stack (category 2).
         *
         * Before:
         * @code
         * ..., arrayref, index, value (double)
         * @endcode
         *
         * After:
         * @code
         * ...
         * @endcode
         *
         * Command: @ref INSTRUCTION_dastore.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* StoreDoubleToArray();

        /**
         * @brief Store a @b reference value into an array.
         *
         * Before:
         * @code
         * ..., arrayref, index, value (reference)
         * @endcode
         *
         * After:
         * @code
         * ...
         * @endcode
         *
         * Command: @ref INSTRUCTION_aastore.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* StoreReferenceToArray();

        /**
         * @brief Store a @b boolean value into an array.
         *
         * Before:
         * @code
         * ..., arrayref, index, value (boolean)
         * @endcode
         *
         * After:
         * @code
         * ...
         * @endcode
         *
         * Command: @ref INSTRUCTION_bastore.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* StoreBooleanToArray();

        /**
         * @brief Store a @b byte value into an array.
         *
         * Before:
         * @code
         * ..., arrayref, index, value (byte)
         * @endcode
         *
         * After:
         * @code
         * ...
         * @endcode
         *
         * Command: @ref INSTRUCTION_bastore.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* StoreByteToArray();

        /**
         * @brief Store a @b char value into an array.
         *
         * Before:
         * @code
         * ..., arrayref, index, value (char)
         * @endcode
         *
         * After:
         * @code
         * ...
         * @endcode
         *
         * Command: @ref INSTRUCTION_castore.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* StoreCharToArray();

        /**
         * @brief Store a @b short value into an array.
         *
         * Before:
         * @code
         * ..., arrayref, index, value (short)
         * @endcode
         *
         * After:
         * @code
         * ...
         * @endcode
         *
         * Command: @ref INSTRUCTION_sastore.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* StoreShortToArray();

        /**
         * @brief Pop @b one value (category 1) from the top of operand stack.
         *
         * Before:
         * @code
         * ..., value
         * @endcode
         *
         * After:
         * @code
         * ...
         * @endcode
         *
         * Command: @ref INSTRUCTION_pop.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* PopOne();

        /**
         * @brief Pop @b two values (category 2) from the top of operand stack.
         *
         * Before:
         * @code
         * ..., value2, value1
         * @endcode
         *
         * After:
         * @code
         * ...
         * @endcode
         *
         * Command: @ref INSTRUCTION_pop2.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* PopTwo();

        /**
         * @brief Duplicate the top operand stack value (one slot value).
         *
         * Duplicates the value currently on top of the operand stack
         * and pushes the duplicate back onto the stack.
         *
         * Before:
         * @code
         * ..., value
         * @endcode
         *
         * After:
         * @code
         * ..., value, value
         * @endcode
         *
         * Command: @ref INSTRUCTION_dup.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* Duplicate();

        /**
         * @brief Duplicate the top operand stack value (one slot value) and insert it beneath the next value.
         *
         * Duplicates the value currently on top of the operand stack and inserts the duplicate
         * one position below the top (i.e., beneath value2).
         *
         * Before:
         * @code
         * ..., value2, value1
         * @endcode
         *
         * After:
         * @code
         * ..., value1, value2, value1
         * @endcode
         *
         * Command: @ref INSTRUCTION_dup_x1.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* DuplicateBeforeOne();

        /**
         * @brief Duplicate the top operand stack value (one slot value) and insert it two values down.
         *
         * Duplicates the value currently on top of the operand stack and inserts the duplicate
         * two positions below the top (i.e., beneath value3).
         *
         * Before:
         * @code
         * ..., value3, value2, value1
         * @endcode
         *
         * After:
         * @code
         * ..., value1, value3, value2, value1
         * @endcode
         *
         * Command: @ref INSTRUCTION_dup_x2.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* DuplicateBeforeTwo();

        /**
         * @brief Duplicate the top operand stack value (two slot value).
         *
         * Duplicates the value currently on top of the operand stack that occupies two stack slots
         * and pushes the duplicate back onto the stack (i.e., duplicates both slots as a single value).
         *
         *
         * Before:
         * @code
         * ..., value
         * @endcode
         *
         * After:
         * @code
         * ..., value, value
         * @endcode
         *
         * Command: @ref INSTRUCTION_dup2.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* DuplicateDouble();

        /**
         * @brief Duplicate the top operand stack value (two slot value) and insert it beneath the next value.
         *
         * Duplicates the value currently on top of the operand stack that occupies two stack slots and inserts the duplicate
         * one position below the top (i.e., beneath value2). The duplicated value is treated as a single two-slot value.
         *
         * Before:
         * @code
         * ..., value2, value1
         * @endcode
         *
         * After:
         * @code
         * ..., value1, value2, value1
         * @endcode
         *
         * Command: @ref INSTRUCTION_dup2_x1.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* DuplicateDoubleBeforeOne();

        /**
         * @brief Duplicate the top operand stack value (two slot value) and insert it two values down.
         *
         * Duplicates the value currently on top of the operand stack that occupies two stack slots and inserts the duplicate
         * two positions below the top (i.e., beneath value3). The duplicated value is treated as a single two-slot value.
         *
         * Before:
         * @code
         * ..., value3, value2, value1
         * @endcode
         *
         * After:
         * @code
         * ..., value1, value3, value2, value1
         * @endcode
         *
         * Command: @ref INSTRUCTION_dup2_x2.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* DuplicateDoubleBeforeTwo();

        /**
         * Swap the top two operand stack values.
         *
         * Before:
         * @code
         * ..., value2, value1
         * @endcode
         *
         * After:
         * @code
         * ..., value1, value2
         * @endcode
         *
         * Command: @ref INSTRUCTION_swap.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* Swap();

        /**
         * @brief Add two integer values on the top of the stack.
         *
         * Before:
         * @code
         * ..., value1 (int), value2 (int)
         * @endcode
         *
         * After:
         * @code
         * ..., result (int)
         * @endcode
         *
         * Command: @ref INSTRUCTION_iadd.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* AddInt();

        /**
         * @brief Add two long values on the top of the stack.
         * @note Long values occupy two words on the operand stack (category 2).
         *
         * Before:
         * @code
         * ..., value1 (long), value2 (long)
         * @endcode
         *
         * After:
         * @code
         * ..., result (long)
         * @endcode
         *
         * Command: @ref INSTRUCTION_ladd.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* AddLong();

        /**
         * @brief Add two float values on the top of the stack.
         *
         * Before:
         * @code
         * ..., value1 (float), value2 (float)
         * @endcode
         *
         * After:
         * @code
         * ..., result (float)
         * @endcode
         *
         * Command: @ref INSTRUCTION_fadd.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* AddFloat();

        /**
         * @brief Add two double values on the top of the stack.
         * @note Double values occupy two words on the operand stack (category 2).
         *
         * Before:
         * @code
         * ..., value1 (double), value2 (double)
         * @endcode
         *
         * After:
         * @code
         * ..., result (double)
         * @endcode
         *
         * Command: @ref INSTRUCTION_dadd.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* AddDouble();

        /**
         * @brief Subtract two integer values on the top of the stack.
         *
         * Before:
         * @code
         * ..., value1 (int), value2 (int)
         * @endcode
         *
         * After:
         * @code
         * ..., result (int)
         * @endcode
         *
         * Semantic:
         * @code
         * result = value1 - value2
         * @endcode
         *
         * Command: @ref INSTRUCTION_isub.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* SubInt();

        /**
         * @brief Subtract two long values on the top of the stack.
         * @note Long values occupy two words on the operand stack (category 2).
         *
         * Before:
         * @code
         * ..., value1 (long), value2 (long)
         * @endcode
         *
         * After:
         * @code
         * ..., result (long)
         * @endcode
         *
         * Semantic:
         * @code
         * result = value1 - value2
         * @endcode
         *
         * Command: @ref INSTRUCTION_lsub.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* SubLong();

        /**
         * @brief Subtract two float values on the top of the stack.
         *
         * Before:
         * @code
         * ..., value1 (float), value2 (float)
         * @endcode
         *
         * After:
         * @code
         * ..., result (float)
         * @endcode
         *
         * Semantic:
         * @code
         * result = value1 - value2
         * @endcode
         *
         * Command: @ref INSTRUCTION_fsub.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* SubFloat();

        /**
         * @brief Subtract two double values on the top of the stack.
         * @note Double values occupy two words on the operand stack (category 2).
         *
         * Before:
         * @code
         * ..., value1 (double), value2 (double)
         * @endcode
         *
         * After:
         * @code
         * ..., result (double)
         * @endcode
         *
         * Semantic:
         * @code
         * result = value1 - value2
         * @endcode
         *
         * Command: @ref INSTRUCTION_dsub.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* SubDouble();

        /**
         * @brief Multiply two integer values on the top of the stack.
         *
         * Before:
         * @code
         * ..., value1 (int), value2 (int)
         * @endcode
         *
         * After:
         * @code
         * ..., result (int)
         * @endcode
         *
         * Command: @ref INSTRUCTION_imul.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* MulInt();

        /**
         * @brief Multiply two long values on the top of the stack.
         * @note Long values occupy two words on the operand stack (category 2).
         *
         * Before:
         * @code
         * ..., value1 (long), value2 (long)
         * @endcode
         *
         * After:
         * @code
         * ..., result (long)
         * @endcode
         *
         * Command: @ref INSTRUCTION_lmul.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* MulLong();

        /**
         * @brief Multiply two float values on the top of the stack.
         *
         * Before:
         * @code
         * ..., value1 (float), value2 (float)
         * @endcode
         *
         * After:
         * @code
         * ..., result (float)
         * @endcode
         *
         * Command: @ref INSTRUCTION_fmul.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* MulFloat();

        /**
         * @brief Multiply two double values on the top of the stack.
         * @note Double values occupy two words on the operand stack (category 2).
         *
         * Before:
         * @code
         * ..., value1 (double), value2 (double)
         * @endcode
         *
         * After:
         * @code
         * ..., result (double)
         * @endcode
         *
         * Command: @ref INSTRUCTION_dmul.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* MulDouble();

        /**
         * @brief Divide two integer values on the top of the stack.
         *
         * Before:
         * @code
         * ..., value1 (int), value2 (int)
         * @endcode
         *
         * After:
         * @code
         * ..., result (int)
         * @endcode
         *
         * Semantic:
         * @code
         * result = value1 / value2
         * @endcode
         *
         * Command: @ref INSTRUCTION_idiv.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* DivInt();

        /**
         * @brief Divide two long values on the top of the stack.
         * @note Long values occupy two words on the operand stack (category 2).
         *
         * Before:
         * @code
         * ..., value1 (long), value2 (long)
         * @endcode
         *
         * After:
         * @code
         * ..., result (long)
         * @endcode
         *
         * Semantic:
         * @code
         * result = value1 / value2
         * @endcode
         *
         * Command: @ref INSTRUCTION_ldiv.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* DivLong();

        /**
         * @brief Divide two float values on the top of the stack.
         *
         * Before:
         * @code
         * ..., value1 (float), value2 (float)
         * @endcode
         *
         * After:
         * @code
         * ..., result (float)
         * @endcode
         *
         * Semantic:
         * @code
         * result = value1 / value2
         * @endcode
         *
         * Command: @ref INSTRUCTION_fdiv.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* DivFloat();

        /**
         * @brief Divide two double values on the top of the stack.
         * @note Double values occupy two words on the operand stack (category 2).
         *
         * Before:
         * @code
         * ..., value1 (double), value2 (double)
         * @endcode
         *
         * After:
         * @code
         * ..., result (double)
         * @endcode
         *
         * Semantic:
         * @code
         * result = value1 / value2
         * @endcode
         *
         * Command: @ref INSTRUCTION_ddiv.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* DivDouble();

        /**
         * @brief Computes the remainder of two integer values on the top of the stack.
         *
         * Before:
         * @code
         * ..., value1 (int), value2 (int)
         * @endcode
         *
         * After:
         * @code
         * ..., result (int)
         * @endcode
         *
         * Semantic:
         * @code
         * result = value1 - (value1 / value2) * value2   // integer division
         * or
         * result = value1 % value2
         * @endcode
         *
         * Command: @ref INSTRUCTION_irem.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* RemInt();

        /**
         * @brief Computes the remainder of two long values on the top of the stack.
         * @note Long values occupy two words on the operand stack (category 2).
         *
         * Before:
         * @code
         * ..., value1 (long), value2 (long)
         * @endcode
         *
         * After:
         * @code
         * ..., result (long)
         * @endcode
         *
         * Semantic:
         * @code
         * result = value1 - (value1 / value2) * value2   // integer division
         * or
         * result = value1 % value2
         * @endcode
         *
         * Command: @ref INSTRUCTION_lrem.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* RemLong();

        /**
         * @brief Computes the remainder of two float values on the top of the stack.
         *
         * Before:
         * @code
         * ..., value1 (float), value2 (float)
         * @endcode
         *
         * After:
         * @code
         * ..., result (float)
         * @endcode
         *
         * Semantic:
         * @code
         * result = value1 % value2
         * @endcode
         *
         * Command: @ref INSTRUCTION_frem.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* RemFloat();

        /**
         * @brief Computes the remainder of two double values on the top of the stack.
         * @note Double values occupy two words on the operand stack (category 2).
         *
         * Before:
         * @code
         * ..., value1 (double), value2 (double)
         * @endcode
         *
         * After:
         * @code
         * ..., result (double)
         * @endcode
         *
         * Semantic:
         * @code
         * result = value1 % value2
         * @endcode
         *
         * Command: @ref INSTRUCTION_drem.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* RemDouble();

        /**
         * @brief Negate an integer value on the top of the stack.
         *
         * Before:
         * @code
         * ..., value (int)
         * @endcode
         *
         * After:
         * @code
         * ..., result (int)
         * @endcode
         *
         * Semantic:
         * @code
         * result = -value
         * @endcode
         *
         * Command: @ref INSTRUCTION_ineg.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* NegInt();

        /**
         * @brief Negate a long value on the top of the stack.
         * @note Long values occupy two words on the operand stack (category 2).
         *
         * Before:
         * @code
         * ..., value (long)
         * @endcode
         *
         * After:
         * @code
         * ..., result (long)
         * @endcode
         *
         * Semantic:
         * @code
         * result = -value
         * @endcode
         *
         * Command: @ref INSTRUCTION_lneg.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* NegLong();

        /**
         * @brief Negate a float value on the top of the stack.
         *
         * Before:
         * @code
         * ..., value (float)
         * @endcode
         *
         * After:
         * @code
         * ..., result (float)
         * @endcode
         *
         * Semantic:
         * @code
         * result = -value
         * @endcode
         *
         * Command: @ref INSTRUCTION_fneg.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* NegFloat();

        /**
         * @brief Negate a double value on the top of the stack.
         * @note Double values occupy two words on the operand stack (category 2).
         *
         * Before:
         * @code
         * ..., value (double)
         * @endcode
         *
         * After:
         * @code
         * ..., result (double)
         * @endcode
         *
         * Semantic:
         * @code
         * result = -value
         * @endcode
         *
         * Command: @ref INSTRUCTION_dneg.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* NegDouble();

        /**
         * @brief Left shift an integer value on the top of the stack.
         *
         * Before:
         * @code
         * ..., value1 (int), value2 (int)
         * @endcode
         *
         * After:
         * @code
         * ..., result (int)
         * @endcode
         *
         * Semantic:
         * @code
         * // For int shift operations only the low 5 bits of the shift distance are used.
         * // This is equivalent to masking with 0x1F (0b11111).
         * result = value1 << (value2 & 0x1F)
         * @endcode
         *
         * Command: @ref INSTRUCTION_ishl.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* LeftShiftInt();

        /**
         * @brief Right arithmetic shift an integer value on the top of the stack.
         *
         * Before:
         * @code
         * ..., value1 (int), value2 (int)
         * @endcode
         *
         * After:
         * @code
         * ..., result (int)
         * @endcode
         *
         * Semantic:
         * @code
         * // For int shift operations only the low 5 bits of the shift distance are used.
         * // This is equivalent to masking with 0x1F (0b11111).
         * result = value1 >> (value2 & 0x1F)
         * @endcode
         *
         * Command: @ref INSTRUCTION_ishr.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* RightArithmeticShiftInt();

        /**
         * @brief Right logical shift an integer value on the top of the stack.
         *
         * Before:
         * @code
         * ..., value1 (int), value2 (int)
         * @endcode
         *
         * After:
         * @code
         * ..., result (int)
         * @endcode
         *
         * Semantic:
         * @code
         * // For int shift operations only the low 5 bits of the shift distance are used.
         * // This is equivalent to masking with 0x1F (0b11111).
         * result = (int)((unsigned int)value1 >> (value2 & 0x1F))
         * @endcode
         *
         * Command: @ref INSTRUCTION_iushr.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* RightLogicShiftInt();

        /**
         * @brief Left shift a long value on the top of the stack.
         * @note Long values occupy two words on the operand stack (category 2).
         *
         * Before:
         * @code
         * ..., value1 (long), value2 (int)
         * @endcode
         *
         * After:
         * @code
         * ..., result (long)
         * @endcode
         *
         * Semantic:
         * @code
         * // For long shift operations only the low 6 bits of the shift distance are used.
         * // This is equivalent to masking with 0x3F (0b111111).
         * result = value1 << (value2 & 0x3F)
         * @endcode
         *
         * Command: @ref INSTRUCTION_lshl.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* LeftShiftLong();

        /**
         * @brief Right arithmetic shift a long value on the top of the stack.
         * @note Long values occupy two words on the operand stack (category 2).
         *
         * Before:
         * @code
         * ..., value1 (long), value2 (int)
         * @endcode
         *
         * After:
         * @code
         * ..., result (long)
         * @endcode
         *
         * Semantic:
         * @code
         * // For long shift operations only the low 6 bits of the shift distance are used.
         * // This is equivalent to masking with 0x3F (0b111111).
         * result = value1 >> (value2 & 0x3F)
         * @endcode
         *
         * Command: @ref INSTRUCTION_lshr.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* RightArithmeticShiftLong();

        /**
         * @brief Right logical shift a long value on the top of the stack.
         * @note Long values occupy two words on the operand stack (category 2).
         *
         * Before:
         * @code
         * ..., value1 (long), value2 (int)
         * @endcode
         *
         * After:
         * @code
         * ..., result (long)
         * @endcode
         *
         * Semantic:
         * @code
         * // For long shift operations only the low 6 bits of the shift distance are used.
         * // This is equivalent to masking with 0x3F (0b111111).
         * result = (long)((unsigned long)value1 >> (value2 & 0x3F))
         * @endcode
         *
         * Command: @ref INSTRUCTION_lushr.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* RightLogicShiftLong();

        /**
         * @brief Bitwise AND two integer values on the top of the stack.
         *
         * Before:
         * @code
         * ..., value1 (int), value2 (int)
         * @endcode
         *
         * After:
         * @code
         * ..., result (int)
         * @endcode
         *
         * Semantic:
         * @code
         * result = value1 & value2
         * @endcode
         *
         * Command: @ref INSTRUCTION_iand.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* BitwiseAndInt();

        /**
         * @brief Bitwise AND two long values on the top of the stack.
         * @note Long values occupy two words on the operand stack (category 2).
         *
         * Before:
         * @code
         * ..., value1 (long), value2 (long)
         * @endcode
         *
         * After:
         * @code
         * ..., result (long)
         * @endcode
         *
         * Semantic:
         * @code
         * result = value1 & value2
         * @endcode
         *
         * Command: @ref INSTRUCTION_land.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* BitwiseAndLong();

        /**
         * @brief Bitwise OR two integer values on the top of the stack.
         *
         * Before:
         * @code
         * ..., value1 (int), value2 (int)
         * @endcode
         *
         * After:
         * @code
         * ..., result (int)
         * @endcode
         *
         * Semantic:
         * @code
         * result = value1 | value2
         * @endcode
         *
         * Command: @ref INSTRUCTION_ior.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* BitwiseOrInt();

        /**
         * @brief Bitwise OR two long values on the top of the stack.
         * @note Long values occupy two words on the operand stack (category 2).
         *
         * Before:
         * @code
         * ..., value1 (long), value2 (long)
         * @endcode
         *
         * After:
         * @code
         * ..., result (long)
         * @endcode
         *
         * Semantic:
         * @code
         * result = value1 | value2
         * @endcode
         *
         * Command: @ref INSTRUCTION_lor.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* BitwiseOrLong();

        /**
         * @brief Bitwise XOR two integer values on the top of the stack.
         *
         * Before:
         * @code
         * ..., value1 (int), value2 (int)
         * @endcode
         *
         * After:
         * @code
         * ..., result (int)
         * @endcode
         *
         * Semantic:
         * @code
         * result = value1 ^ value2
         * @endcode
         *
         * Command: @ref INSTRUCTION_ixor.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* BitwiseXorInt();

        /**
         * @brief Bitwise XOR two long values on the top of the stack.
         * @note Long values occupy two words on the operand stack (category 2).
         *
         * Before:
         * @code
         * ..., value1 (long), value2 (long)
         * @endcode
         *
         * After:
         * @code
         * ..., result (long)
         * @endcode
         *
         * Semantic:
         * @code
         * result = value1 ^ value2
         * @endcode
         *
         * Command: @ref INSTRUCTION_lxor.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* BitwiseXorLong();

        /**
         * @brief Increment a local integer variable by value.
         *
         * Increments the local variable at the specified index by the given signed value.
         * This instruction does not use the operand stack.
         *
         * Semantic:
         * @code
         * locals[index] = locals[index] + value
         * @endcode
         *
         * Command: @ref INSTRUCTION_iinc.
         *
         * @param index Index of the local variable.
         * @param value Signed increment value.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* IncrementLocalVariable(uint16_t index, int16_t value);

        /**
         * @brief Convert an integer value on the top of the stack to a long value.
         * @note Long values occupy two words on the operand stack (category 2).
         *
         * Before:
         * @code
         * ..., value (int)
         * @endcode
         *
         * After:
         * @code
         * ..., result (long)
         * @endcode
         *
         * Command: @ref INSTRUCTION_i2l.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* IntToLong();

        /**
         * @brief Convert an integer value on the top of the stack to a float value.
         *
         * Before:
         * @code
         * ..., value (int)
         * @endcode
         *
         * After:
         * @code
         * ..., result (float)
         * @endcode
         *
         * Command: @ref INSTRUCTION_i2f.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* IntToFloat();

        /**
         * @brief Convert an integer value on the top of the stack to a double value.
         * @note Double values occupy two words on the operand stack (category 2).
         *
         * Before:
         * @code
         * ..., value (int)
         * @endcode
         *
         * After:
         * @code
         * ..., result (double)
         * @endcode
         *
         * Command: @ref INSTRUCTION_i2d.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* IntToDouble();

        /**
         * @brief Convert a long value on the top of the stack to an integer value.
         * @note Long values occupy two words on the operand stack (category 2).
         *
         * Before:
         * @code
         * ..., value (long)
         * @endcode
         *
         * After:
         * @code
         * ..., result (int)
         * @endcode
         *
         * Command: @ref INSTRUCTION_l2i.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* LongToInt();

        /**
         * @brief Convert a long value on the top of the stack to a float value.
         * @note Long values occupy two words on the operand stack (category 2).
         *
         * Before:
         * @code
         * ..., value (long)
         * @endcode
         *
         * After:
         * @code
         * ..., result (float)
         * @endcode
         *
         * Command: @ref INSTRUCTION_l2f.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* LongToFloat();

        /**
        * @brief Convert a long value on the top of the stack to a double value.
         * @note Long values occupy two words on the operand stack (category 2).
         * @note Double values occupy two words on the operand stack (category 2).
         *
         * Before:
         * @code
         * ..., value (long)
         * @endcode
         *
         * After:
         * @code
         * ..., result (double)
         * @endcode
         *
         * Command: @ref INSTRUCTION_l2d.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* LongToDouble();

        /**
         * @brief Convert a float value on the top of the stack to an integer value.
         *
         * Before:
         * @code
         * ..., value (float)
         * @endcode
         *
         * After:
         * @code
         * ..., result (int)
         * @endcode
         *
         * Command: @ref INSTRUCTION_f2i.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* FloatToInt();

        /**
         * @brief Convert a float value on the top of the stack to a long value.
         * @note Long values occupy two words on the operand stack (category 2).
         *
         * Before:
         * @code
         * ..., value (float)
         * @endcode
         *
         * After:
         * @code
         * ..., result (long)
         * @endcode
         *
         * Command: @ref INSTRUCTION_f2l.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* FloatToLong();

        /**
         * @brief Convert a float value on the top of the stack to a double value.
         * @note Double values occupy two words on the operand stack (category 2).
         *
         * Before:
         * @code
         * ..., value (float)
         * @endcode
         *
         * After:
         * @code
         * ..., result (double)
         * @endcode
         *
         * Command: @ref INSTRUCTION_f2d.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* FloatToDouble();

        /**
         * @brief Convert a double value on the top of the stack to an integer value.
         * @note Double values occupy two words on the operand stack (category 2).
         *
         * Before:
         * @code
         * ..., value (double)
         * @endcode
         *
         * After:
         * @code
         * ..., result (int)
         * @endcode
         *
         * Command: @ref INSTRUCTION_d2i.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* DoubleToInt();

        /**
         * @brief Convert a double value on the top of the stack to a long value.
         * @note Double values occupy two words on the operand stack (category 2).
         * @note Long values occupy two words on the operand stack (category 2).
         *
         * Before:
         * @code
         * ..., value (double)
         * @endcode
         *
         * After:
         * @code
         * ..., result (long)
         * @endcode
         *
         * Command: @ref INSTRUCTION_d2l.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* DoubleToLong();

        /**
         * @brief Convert a double value on the top of the stack to a float value.
         * @note Double values occupy two words on the operand stack (category 2).
         *
         * Before:
         * @code
         * ..., value (double)
         * @endcode
         *
         * After:
         * @code
         * ..., result (float)
         * @endcode
         *
         * Command: @ref INSTRUCTION_d2f.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* DoubleToFloat();

        /**
         * @brief Convert an integer value on the top of the stack to a byte value.
         *
         * Before:
         * @code
         * ..., value (int)
         * @endcode
         *
         * After:
         * @code
         * ..., result (int)
         * @endcode
         *
         * Command: @ref INSTRUCTION_i2b.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* IntToByte();

        /**
         * @brief Convert an integer value on the top of the stack to a char value.
         *
         * Before:
         * @code
         * ..., value (int)
         * @endcode
         *
         * After:
         * @code
         * ..., result (int)
         * @endcode
         *
         * Command: @ref INSTRUCTION_i2c.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* IntToChar();

        /**
         * @brief Convert an integer value on the top of the stack to a short value.
         *
         * Before:
         * @code
         * ..., value (int)
         * @endcode
         *
         * After:
         * @code
         * ..., result (int)
         * @endcode
         *
         * Command: @ref INSTRUCTION_i2s.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* IntToShort();

        /**
         * @brief Compare two long values on the top of the stack.
         * @note Long values occupy two words on the operand stack (category 2).
         *
         * Before:
         * @code
         * ..., value1 (long), value2 (long)
         * @endcode
         *
         * After:
         * @code
         * ..., result (int)
         * @endcode
         *
         * Semantic:
         * @code
         * if (value1 > value2) result = 1
         * if (value1 == value2) result = 0
         * if (value1 < value2) result = -1
         * @endcode
         *
         * Command: @ref INSTRUCTION_lcmp.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* CompareLong();

        /**
         * @brief Compare two float values on the top of the stack.
         *
         * Compares two float values and pushes an integer result onto the operand stack.
         * If either value is NaN, the result depends on the <i>nanResult</i> parameter and
         * this method selects the appropriate comparison instruction.
         *
         * Before:
         * @code
         * ..., value1 (float), value2 (float)
         * @endcode
         *
         * After:
         * @code
         * ..., result (int)
         * @endcode
         *
         * Semantic:
         * @code
         * if (value1 > value2) result = 1
         * if (value1 == value2) result = 0
         * if (value1 < value2) result = -1
         * if (value1 is NaN or value2 is NaN) result = (nanResult == StrictCompare::less) ? -1 : 1
         * @endcode
         *
         * Command:
         * | nanResult | Command |
         * |-----------|---------|
         * | @ref StrictCompare::less    | @ref INSTRUCTION_fcmpl |
         * | @ref StrictCompare::greater | @ref INSTRUCTION_fcmpg |
         *
         * @param nanResult Result when either operand is NaN.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* CompareFloat(Instruction::StrictCompare nanResult);

        /**
         * @brief Compare two double values on the top of the stack.
         * @note Double values occupy two words on the operand stack (category 2).
         *
         * Compares two double values and pushes an integer result onto the operand stack.
         * If either value is NaN, the result depends on the <i>nanResult</i> parameter and
         * this method selects the appropriate comparison instruction.
         *
         * Before:
         * @code
         * ..., value1 (double), value2 (double)
         * @endcode
         *
         * After:
         * @code
         * ..., result (int)
         * @endcode
         *
         * Semantic:
         * @code
         * if (value1 > value2) result = 1
         * if (value1 == value2) result = 0
         * if (value1 < value2) result = -1
         * if (value1 is NaN or value2 is NaN) result = (nanResult == StrictCompare::less) ? -1 : 1
         * @endcode
         *
         * Command:
         * | nanResult | Command |
         * |-----------|---------|
         * | @ref StrictCompare::less    | @ref INSTRUCTION_dcmpl |
         * | @ref StrictCompare::greater | @ref INSTRUCTION_dcmpg |
         *
         * @param nanResult Result when either operand is NaN.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* CompareDouble(Instruction::StrictCompare nanResult);

        /**
         * @brief Conditional branch based on comparison of an integer value with zero.
         *
         * Pops one integer value from the operand stack and compares it with zero using the selected operation.
         * If the condition is true, execution continues at the specified @p label.
         *
         * Before:
         * @code
         * ..., value (int)
         * @endcode
         *
         * After:
         * @code
         * ...
         * @endcode
         *
         * Command:
         * | operation | command |
         * |-----------|---------|
         * | @ref Instruction::equal "equal"               | @ref Instruction::INSTRUCTION_ifeq "ifeq" |
         * | @ref Instruction::notEqual "notEqual"         | @ref Instruction::INSTRUCTION_ifne "ifne" |
         * | @ref Instruction::lessThan "lessThan"         | @ref Instruction::INSTRUCTION_iflt "iflt" |
         * | @ref Instruction::greaterEqual "greaterEqual" | @ref Instruction::INSTRUCTION_ifge "ifge" |
         * | @ref Instruction::greaterThan "greaterThan"   | @ref Instruction::INSTRUCTION_ifgt "ifgt" |
         * | @ref Instruction::lessEqual "lessEqual"       | @ref Instruction::INSTRUCTION_ifle "ifle" |
         *
         * @param operation Comparison operation against zero.
         * @param label Target label to branch to when the condition is true.
         * @note The target label must be placed into the code stream using @ref AttributeCode::addLabel.
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] InstructionJump* If(Instruction::Compare operation, Label* label);

        /**
         * @brief Conditional branch based on comparison of two integer values.
         *
         * Pops two integer values from the operand stack and compares them using the selected operation.
         * If the condition is true, execution continues at the specified @p label.
         *
         * Before:
         * @code
         * ..., value1 (int), value2 (int)
         * @endcode
         *
         * After:
         * @code
         * ...
         * @endcode
         *
         * Command:
         * | operation | Command |
         * |-----------|---------|
         * | @ref Compare::equal        | @ref INSTRUCTION_if_icmpeq |
         * | @ref Compare::notEqual     | @ref INSTRUCTION_if_icmpne |
         * | @ref Compare::lessThan     | @ref INSTRUCTION_if_icmplt |
         * | @ref Compare::greaterEqual | @ref INSTRUCTION_if_icmpge |
         * | @ref Compare::greaterThan  | @ref INSTRUCTION_if_icmpgt |
         * | @ref Compare::lessEqual    | @ref INSTRUCTION_if_icmple |
         *
         * @param operation Comparison operation between two integer values.
         * @param label Target label to branch to when the condition is true.
         * @note The target label must be placed into the code stream using @ref AttributeCode::addLabel.
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] InstructionJump* IfWithCompare(Instruction::Compare operation, Label* label);

        /**
         * @brief Conditional branch if two reference values are equal.
         *
         * Pops two reference values from the operand stack and compares them for reference equality.
         * If the references are equal, execution continues at the specified @p label.
         *
         * Before:
         * @code
         * ..., value1 (reference), value2 (reference)
         * @endcode
         *
         * After:
         * @code
         * ...
         * @endcode
         *
         * Command: @ref INSTRUCTION_if_acmpeq.
         *
         * @param label Target label to branch to when the references are equal.
         * @note The target label must be placed into the code stream using @ref AttributeCode::addLabel.
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] InstructionJump* IfReferenceEqual(Label* label);

        /**
         * @brief Conditional branch if two reference values are not equal.
         *
         * Pops two reference values from the operand stack and compares them for reference inequality.
         * If the references are not equal, execution continues at the specified @p label.
         *
         * Before:
         * @code
         * ..., value1 (reference), value2 (reference)
         * @endcode
         *
         * After:
         * @code
         * ...
         * @endcode
         *
         * Command: @ref INSTRUCTION_if_acmpne.
         *
         * @param label Target label to branch to when the references are not equal.
         * @note The target label must be placed into the code stream using @ref AttributeCode::addLabel.
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] InstructionJump* IfReferenceNotEqual(Label* label);


        /**
         * @brief Unconditional branch to the specified label.
         *
         * Transfers control unconditionally to the bytecode position marked by @p label.
         *
         * Command: @ref INSTRUCTION_goto or @ref INSTRUCTION_goto_w (selected automatically by the encoder).
         *
         * @param label Target label to branch to.
         * @note The target label must be placed into the code stream using @ref AttributeCode::addLabel.
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] InstructionJump* GoTo(Label* label);


        /**
         * @brief Jump to one of several targets based on an integer key (tableswitch).
         *
         * Pops an integer key from the operand stack and transfers control to the target label selected by the key value.
         * If the key is outside the range [@p low, @p high], execution transfers to @p defaultLabel.
         *
         * Before:
         * @code
         * ..., key (int)
         * @endcode
         *
         * After:
         * @code
         * ...
         * @endcode
         *
         * Command: @ref INSTRUCTION_tableswitch.
         *
         * @param low Lowest key value.
         * @param high Highest key value.
         * @param defaultLabel Target label for keys outside the [low, high] range.
         * @param labels Target labels for keys in range [low, high].
         *               The element labels[i] corresponds to key value (low + i).
         * @note The target labels must be placed into the code stream using @ref AttributeCode::addLabel.
         * @throws std::invalid_argument If labels.size() != (high - low + 1).
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* Switch(int32_t low, int32_t high, Label* defaultLabel,
                                          const std::vector<Label*>& labels);

        /**
         * @brief Jump to one of several targets based on an integer key (lookupswitch).
         *
         * Pops an integer key from the operand stack and transfers control to the target label associated with the key value.
         * If the key does not match any entry, execution transfers to @p defaultLabel.
         *
         * Before:
         * @code
         * ..., key (int)
         * @endcode
         *
         * After:
         * @code
         * ...
         * @endcode
         *
         * Command: @ref INSTRUCTION_lookupswitch.
         *
         * @param defaultLabel Target label when the key does not match any case.
         * @param labels Key-to-label mapping. The map key is the case value and the map value is the target label.
         * @note The target labels must be placed into the code stream using @ref AttributeCode::addLabel.
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* Switch(Label* defaultLabel, const std::map<int32_t, Label*>& labels);


        /**
         * @brief Return a boolean value from a method.
         *
         * Pops a boolean value from the operand stack and returns it to the caller.
         *
         * Before:
         * @code
         * ..., value (boolean)
         * @endcode
         *
         * After:
         * @code
         * ...
         * @endcode
         *
         * Command: @ref INSTRUCTION_ireturn.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* ReturnBoolean();

        /**
         * @brief Return a byte value from a method.
         *
         * Pops a byte value from the operand stack and returns it to the caller.
         *
         * Before:
         * @code
         * ..., value (byte)
         * @endcode
         *
         * After:
         * @code
         * ...
         * @endcode
         *
         * Command: @ref INSTRUCTION_ireturn.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* ReturnByte();

        /**
         * @brief Return a char value from a method.
         *
         * Pops a char value from the operand stack and returns it to the caller.
         *
         * Before:
         * @code
         * ..., value (char)
         * @endcode
         *
         * After:
         * @code
         * ...
         * @endcode
         *
         * Command: @ref INSTRUCTION_ireturn.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* ReturnChar();

        /**
         * @brief Return a short value from a method.
         *
         * Pops a short value from the operand stack and returns it to the caller.
         *
         * Before:
         * @code
         * ..., value (short)
         * @endcode
         *
         * After:
         * @code
         * ...
         * @endcode
         *
         * Command: @ref INSTRUCTION_ireturn.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* ReturnShort();

        /**
         * @brief Return an integer value from a method.
         *
         * Pops an integer value from the operand stack and returns it to the caller.
         *
         * Before:
         * @code
         * ..., value (int)
         * @endcode
         *
         * After:
         * @code
         * ...
         * @endcode
         *
         * Command: @ref INSTRUCTION_ireturn.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* ReturnInt();

        /**
         * @brief Return a long value from a method.
         *
         * Pops a long value from the operand stack and returns it to the caller.
         *
         * Before:
         * @code
         * ..., value (long)
         * @endcode
         *
         * After:
         * @code
         * ...
         * @endcode
         *
         * Command: @ref INSTRUCTION_lreturn.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         * @note Long values occupy two words on the operand stack (category 2).
         */
        [[nodiscard]] Instruction* ReturnLong();

        /**
         * @brief Return a float value from a method.
         *
         * Pops a float value from the operand stack and returns it to the caller.
         *
         * Before:
         * @code
         * ..., value (float)
         * @endcode
         *
         * After:
         * @code
         * ...
         * @endcode
         *
         * Command: @ref INSTRUCTION_freturn.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* ReturnFloat();

        /**
         * @brief Return a double value from a method.
         *
         * Pops a double value from the operand stack and returns it to the caller.
         *
         * Before:
         * @code
         * ..., value (double)
         * @endcode
         *
         * After:
         * @code
         * ...
         * @endcode
         *
         * Command: @ref INSTRUCTION_dreturn.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         * @note Double values occupy two words on the operand stack (category 2).
         */
        [[nodiscard]] Instruction* ReturnDouble();

        /**
         * @brief Return a reference value from a method.
         *
         * Pops a reference value from the operand stack and returns it to the caller.
         *
         * Before:
         * @code
         * ..., value (reference)
         * @endcode
         *
         * After:
         * @code
         * ...
         * @endcode
         *
         * Command: @ref INSTRUCTION_areturn.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* ReturnReference();

        /**
         * @brief Return void from a method.
         *
         * Returns from the current method. This instruction does not use the operand stack.
         *
         * Command: @ref INSTRUCTION_return.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* ReturnVoid();

        /**
         * @brief Get a static field value from a class.
         *
         * Pushes the value of the specified static field onto the operand stack.
         *
         * Before:
         * @code
         * ...
         * @endcode
         *
         * After:
         * @code
         * ..., value
         * @endcode
         *
         * Command: @ref INSTRUCTION_getstatic.
         *
         * @param field Fieldref constant for a class static field.
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* GetStatic(ConstantFieldref* field);

        /**
         * @brief Set a static field value in a class.
         *
         * Pops a value from the operand stack and stores it into the specified static field.
         *
         * Before:
         * @code
         * ..., value
         * @endcode
         *
         * After:
         * @code
         * ...
         * @endcode
         *
         * Command: @ref INSTRUCTION_putstatic.
         *
         * @param field Fieldref constant for a class static field.
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* PutStatic(ConstantFieldref* field);

        /**
         * @brief Get a field value from an object.
         *
         * Pops an object reference from the operand stack and pushes the value of the specified field onto the operand stack.
         *
         * Before:
         * @code
         * ..., objectref (ref)
         * @endcode
         *
         * After:
         * @code
         * ..., value
         * @endcode
         *
         * Command: @ref INSTRUCTION_getfield.
         *
         * @param field Fieldref constant for a class instance field.
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* GetField(ConstantFieldref* field);

        /**
         * @brief Set a field value in an object.
         *
         * Pops a value and an object reference from the operand stack and stores the value into the specified field.
         *
         * Before:
         * @code
         * ..., objectref (ref), value
         * @endcode
         *
         * After:
         * @code
         * ...
         * @endcode
         *
         * Command: @ref INSTRUCTION_putfield.
         *
         * @param field Fieldref constant for a class instance field.
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* PutField(ConstantFieldref* field);

        /**
         * @brief Invoke an instance method using virtual dispatch.
         *
         * Invokes the specified instance method. The object reference and arguments are taken from the operand stack.
         *
         * Before:
         * @code
         * ..., objectref (reference), [arg1, arg2, ...]
         * @endcode
         *
         * After:
         * @code
         * ..., [result]
         * @endcode
         *
         * Command: @ref INSTRUCTION_invokevirtual.
         *
         * @param method Methodref constant for an instance method.
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* InvokeVirtual(ConstantMethodref* method);

        /**
         * @brief Invoke an instance method using special handling.
         *
         * Invokes the specified instance method (e.g., a constructor, a private method, or a superclass method).
         * The object reference and arguments are taken from the operand stack.
         *
         * Before:
         * @code
         * ..., objectref (reference), [arg1, arg2, ...]
         * @endcode
         *
         * After:
         * @code
         * ..., [result]
         * @endcode
         *
         * Command: @ref INSTRUCTION_invokespecial.
         *
         * @param method Methodref constant for an instance method.
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* InvokeSpecial(ConstantMethodref* method);

        /**
         * @brief Invoke a static method.
         *
         * Invokes the specified static method. The arguments are taken from the operand stack.
         *
         * Before:
         * @code
         * ..., [arg1, arg2, ...]
         * @endcode
         *
         * After:
         * @code
         * ..., [result]
         * @endcode
         *
         * Command: @ref INSTRUCTION_invokestatic.
         *
         * @param method Methodref constant for a static method.
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* InvokeStatic(ConstantMethodref* method);

        /**
         * @brief Invoke an interface method.
         *
         * Invokes the specified interface method. The object reference and arguments are taken from the operand stack.
         *
         * Before:
         * @code
         * ..., objectref (reference), [arg1, arg2, ...]
         * @endcode
         *
         * After:
         * @code
         * ..., [result]
         * @endcode
         *
         * Command: @ref INSTRUCTION_invokeinterface.
         *
         * @param method AttributeCode constant for an interface method.
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* InvokeInterface(ConstantInterfaceMethodref* method);

        // ToDo Introduce InvokeDynamic static constructor.
        // [[nodiscard]] Instruction* InvokeDynamic();

        /**
         * @brief Create a new object.
         *
         * Allocates a new object of the specified class and pushes an uninitialized object reference onto the operand stack.
         *
         * Before:
         * @code
         * ...
         * @endcode
         *
         * After:
         * @code
         * ..., objectref (ref)
         * @endcode
         *
         * Command: @ref INSTRUCTION_new.
         *
         * @param classConstant Class constant for object allocation.
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* New(ConstantClass* classConstant);

        /**
         * @brief Create a new primitive type array.
         *
         * Pops an integer length from the operand stack, allocates a new array of the specified primitive type,
         * and pushes the array reference onto the operand stack.
         *
         * Before:
         * @code
         * ..., length (integer)
         * @endcode
         *
         * After:
         * @code
         * ..., arrayref (reference)
         * @endcode
         *
         * Command: @ref INSTRUCTION_newarray.
         *
         * @param type Primitive array element type.
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* NewArray(Instruction::Type type);

        /**
         * @brief Create a new reference type array.
         *
         * Pops an integer length from the operand stack, allocates a new array of the specified reference type,
         * and pushes the array reference onto the operand stack.
         *
         * Before:
         * @code
         * ..., length (integer)
         * @endcode
         *
         * After:
         * @code
         * ..., arrayref (reference)
         * @endcode
         *
         * Command: @ref INSTRUCTION_anewarray.
         *
         * @param classConstant Class constant for array element type.
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* NewArray(ConstantClass* classConstant);

        /**
         * @brief Get the length of an array.
         *
         * Pops an array reference from the operand stack and pushes the array length onto the operand stack.
         *
         * Before:
         * @code
         * ..., arrayref (ref)
         * @endcode
         *
         * After:
         * @code
         * ..., length (int)
         * @endcode
         *
         * Command: @ref INSTRUCTION_arraylength.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* ArrayLength();

        /**
         * @brief Throw an exception.
         *
         * Pops a throwable object reference from the operand stack and throws it.
         *
         * Before:
         * @code
         * ..., objectref (reference)
         * @endcode
         *
         * After:
         * @code
         * ...
         * @endcode
         *
         * Command: @ref INSTRUCTION_athrow.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* Throw();

        /**
         * @brief Check whether an object is of the specified type.
         *
         * Pops an object reference from the operand stack and checks whether it can be cast to the specified type.
         * Pushes the same object reference back onto the operand stack.
         *
         * Before:
         * @code
         * ..., objectref (reference)
         * @endcode
         *
         * After:
         * @code
         * ..., objectref (reference)
         * @endcode
         *
         * Command: @ref INSTRUCTION_checkcast.
         *
         * @param classConstant Class constant for cast target type.
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* CheckCast(ConstantClass* classConstant);

        /**
         * @brief Determine whether an object is of the specified type.
         *
         * Pops an object reference from the operand stack and tests whether it is an instance of the specified type.
         * Pushes a boolean result onto the operand stack (0 for false, 1 for true).
         *
         * Before:
         * @code
         * ..., objectref (ref)
         * @endcode
         *
         * After:
         * @code
         * ..., result (boolean)
         * @endcode
         *
         * Command: @ref INSTRUCTION_instanceof.
         *
         * @param classConstant Class constant for test target type.
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* InstanceOf(ConstantClass* classConstant);

        /**
         * @brief Enter a monitor associated with an object.
         *
         * Pops an object reference from the operand stack and enters its monitor.
         *
         * Before:
         * @code
         * ..., objectref (reference)
         * @endcode
         *
         * After:
         * @code
         * ...
         * @endcode
         *
         * Command: @ref INSTRUCTION_monitorenter.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* MonitorEnter();

        /**
         * @brief Exit a monitor associated with an object.
         *
         * Pops an object reference from the operand stack and exits its monitor.
         *
         * Before:
         * @code
         * ..., objectref (reference)
         * @endcode
         *
         * After:
         * @code
         * ...
         * @endcode
         *
         * Command: @ref INSTRUCTION_monitorexit.
         *
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* MonitorExit();

        /**
         * @brief Create a new multidimensional array.
         *
         * Pops <i>dimensions</i> integer lengths from the operand stack, allocates a new multidimensional array
         * of the specified type, and pushes the array reference onto the operand stack.
         *
         * Before:
         * @code
         * ..., count1 (int), count2 (int), ..., countN (int)
         * @endcode
         *
         * After:
         * @code
         * ..., arrayref (ref)
         * @endcode
         *
         * Command: @ref INSTRUCTION_multianewarray.
         *
         * @param classConstant Class constant for array type.
         * @param dimensions Number of array dimensions.
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] Instruction* MultiNewArray(ConstantClass* classConstant, uint8_t dimensions);

        /**
         * @brief Conditional branch if a reference value is null.
         *
         * Pops one reference value from the operand stack. If the value is null, execution continues at the specified @p label.
         *
         * Before:
         * @code
         * ..., value (reference)
         * @endcode
         *
         * After:
         * @code
         * ...
         * @endcode
         *
         * Command: @ref INSTRUCTION_ifnull.
         *
         * @param label Target label to branch to when the value is null.
         * @note The target label must be placed into the code stream using @ref AttributeCode::addLabel.
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] InstructionJump* IfNull(Label* label);

        /**
         * @brief Conditional branch if a reference value is not null.
         *
         * Pops one reference value from the operand stack. If the value is not null, execution continues at the specified @p label.
         *
         * Before:
         * @code
         * ..., value (reference)
         * @endcode
         *
         * After:
         * @code
         * ...
         * @endcode
         *
         * Command: @ref INSTRUCTION_ifnonnull.
         *
         * @param label Target label to branch to when the value is not null.
         * @note The target label must be placed into the code stream using @ref AttributeCode::addLabel.
         * @return A new instruction for this code attribute.
         * @note The returned instruction is owned by the caller until it is registered using @ref addInstruction.
         */
        [[nodiscard]] InstructionJump* IfNotNull(Label* label);

        // endregion
        // region ADD TO CODE
        /**
         * Add an instruction to the end of the @ref code_ "list of instructions".
         * @param instruction New instruction.
         * @return This instance.
         * @note Transfers ownership of @p instruction to this @ref AttributeCode instance.
         */
        AttributeCode& addInstruction(Instruction* instruction);

        /**
         * Add instructions to the end of the @ref code_ "list of instructions".
         * @tparam Ptrs @c Instruction*
         * @param instructions The instructions are in the order they were written.
         * @return This instance.
         * @see AttributeCode::addInstruction
         */
        template <class... Ptrs>
        AttributeCode& addInstructions(Ptrs... instructions)
        {
            static_assert(((std::is_convertible_v<Ptrs, Instruction*>) && ...),
                          "All arguments must be convertible to Instruction*.");

            ((void)addInstruction(static_cast<Instruction*>(instructions)), ...);
            return *this;
        }

        /**
         * Add instructions to the end of the @ref code_ "list of instructions".
         * @tparam R Input range.
         * @param r Range of @c [[nodiscard]] Instruction* objects.
         * @return This instance.
         * @see AttributeCode::addInstruction
         */
        template <std::ranges::input_range R>
        AttributeCode& addInstructions(R&& r)
        {
            using V = std::remove_cvref_t<std::ranges::range_reference_t<R>>;
            static_assert(std::is_convertible_v<V, Instruction*>,
                          "Range elements must be convertible to Instruction*");

            for (auto&& x : r) addInstruction(static_cast<Instruction*>(x));
            return *this;
        }

        /// @copydoc AttributeCode::addInstruction
        AttributeCode& operator<<(Instruction* instruction)
        {
            return addInstruction(instruction);
        }

        /// @copydoc AttributeCode::addInstructions(R&&)
        template <std::ranges::input_range R>
        AttributeCode& operator<<(R&& r)
        {
            return addInstructions(r);
        }

        // endregion
        // endregion
        // region LABEL
        // region STATIC CONSTRUCTORS

        /**
         * @brief Create a new @c Label owned by this code attribute.
         *
         * A label is a symbolic marker that can be placed into the code stream using @ref addLabel.
         * When a label is placed, it marks the bytecode position of the next instruction that will be emitted.
         *
         * @note Creating a label does not affect the code stream. The label must be placed with @ref addLabel.
         *
         * @return A new label instance for this code attribute.
         * @see AttributeCode::addLabel
         * @note The returned label is owned by the caller until it is registered using @ref addLabel.
         */
        Label* CodeLabel();


        // endregion
        // region ADD TO CODE

        /**
         * @brief Place a label into the code stream at the current position.
         *
         * The label becomes part of the code chronology. A placed label marks the bytecode position of the
         * next instruction that will be emitted after it.
         *
         * Multiple labels may be placed consecutively; in this case they all refer to the same subsequent instruction.
         *
         * @note A label object can be placed into the code stream only once.
         * @note A placed label must be followed by at least one instruction in the code stream;
         *       otherwise the label has no target instruction to refer to.
         *
         * @param label The label to place.
         * @return This instance.
         * @note Transfers ownership of @p label to this @ref AttributeCode instance.
         */
        AttributeCode& addLabel(Label* label);


        /// @copydoc AttributeCode::addLabel
        AttributeCode& operator<<(Label* label)
        {
            return addLabel(label);
        }

        // endregion
        // endregion
        // region EXCEPTION HANDLER

        /**
         * @brief Register a @c try..catch handler (exception_table entry).
         *
         * Registers an exception table entry for the protected region and a handler entry point.
         *
         * The protected region is defined by two labels placed in the code stream:
         * - @p tryStartLabel marks the first instruction of the protected region;
         * - @p tryFinishLabel marks the first instruction after the protected region (end is exclusive).
         *
         * The handler starts at @p catchStartLabel.
         *
         * Minimal layout:
         * @code
         * auto* L_tryStart   = code->CodeLabel();
         * auto* L_tryEnd     = code->CodeLabel();
         * auto* L_catch      = code->CodeLabel();
         * auto* L_afterCatch = code->CodeLabel();
         *
         * *code << L_tryStart << ... try block ... << L_tryEnd
         *       << code->GoTo(L_afterCatch)
         *       << L_catch << ... catch block ... << L_afterCatch;
         *
         * code->addTryCatch(L_tryStart, L_tryEnd, L_catch, exceptionClass);
         * @endcode
         *
         * @note All provided labels must be placed into the code stream using @ref addLabel (or @c operator<<).
         * @note The label @p tryFinishLabel should be placed after the last instruction of the protected region.
         *
         * @param tryStartLabel Label placed at the start of the protected region.
         * @param tryFinishLabel Label placed immediately after the protected region (end-exclusive).
         * @param catchStartLabel Label placed at the start of the handler.
         * @param catchClass Exception class to catch. If @c nullptr, the handler is catch-all (catches any throwable).
         * @return Created exception handler.
         */
        ExceptionHandler* addTryCatch(Label* tryStartLabel,
                                      Label* tryFinishLabel,
                                      Label* catchStartLabel,
                                      ConstantClass* catchClass);

        /**
         * @brief Register a catch-all handler for a protected region.
         *
         * Equivalent to calling @ref addTryCatch with @c catchClass == nullptr.
         *
         * @param tryStartLabel Label placed at the start of the protected region.
         * @param tryFinishLabel Label placed immediately after the protected region (end-exclusive).
         * @param handlerStartLabel Label placed at the start of the handler.
         * @return Created exception handler.
         *
         * @see addTryCatch
         */
        inline ExceptionHandler* addCatchAll(Label* tryStartLabel,
                                             Label* tryFinishLabel,
                                             Label* handlerStartLabel)
        {
            return addTryCatch(tryStartLabel, tryFinishLabel, handlerStartLabel, nullptr);
        }

        //endregion
        // region FINALIZATION
        /**
         * @brief Check whether the code attribute is finalized.
         *
         * @return @c true if finalized; otherwise @c false.
         */
        [[nodiscard]] bool isFinalized() const;

        /**
         * @brief Finalize the code attribute.
         *
         * @throws std::logic_error If there are pending labels without a following instruction.
         * @throws std::runtime_error If the resulting code is too large to fit into JVM limits.
         *
         * @note Safe to call multiple times; subsequent calls have no effect.
         */
        void finalize();
        // endregion
        // region ATTRIBUTES
        // ToDo operations with code's attributes are not implemented
        // endregion

        [[nodiscard]] bool isMethodAttribute() const noexcept override { return true; };

    protected:
        /**
         * @pre The attribute must be finalized via @ref finalize.
         * @throws std::logic_error If called before finalization.
         */
        void writeTo(std::ostream& os) const override;

        /**
         * @pre The attribute must be finalized via @ref finalize.
         * @throws std::logic_error If called before finalization.
         */
        [[nodiscard]] size_t getContentSizeInBytes() const override;

    private:
        /**
         * @brief Construct a сode attribute.
         *
         * @param methodOwner Owning method for this сode attribute.
         */
        explicit AttributeCode(Method* methodOwner);

    public:
        [[nodiscard]] std::size_t getByteSize() const override;

    private:
        uint16_t maxStack_ = 0; ///< Max stack size.
        uint16_t maxLocals_ = 0; ///< Max size of local variable array.
        uint16_t instructionsByteSize_ = 0; ///< Size of bytecode instruction array measured in bytes.
        /// Size of exceptions handlers set measured in bytes (exception_table structure).
        uint16_t exceptionsHandlersByteSize_ = 0;
        std::vector<Instruction*> code_{}; ///< Bytecode instructions array.
        std::set<ExceptionHandler*> exceptionHandlers_{}; ///< Exception handlers set.
        std::set<Attribute*> attributes_{}; ///< Attributes.

        std::set<Label*> labelsOnCurrentStep_{}; ///< Set of labels on current step.

        bool isFinalized_ = false; ///< Flag of completed attribute initialization.
        std::set<Label*> allRegisteredLabels_; ///< Registered labels
    };
} // jvm

#endif //JVM__ATTRIBUTE_CODE_H
