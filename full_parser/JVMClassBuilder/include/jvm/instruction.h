#ifndef JVM__INSTRUCTION_H
#define JVM__INSTRUCTION_H

#include <cstdint>
#include <iosfwd>

#include "class-file-element.h"

namespace jvm
{
    class AttributeCode;

    /**
     * @brief Base class for JVM bytecode instructions.
     *
     * An @c Instruction represents a single JVM bytecode operation (opcode) and belongs to exactly one
     * @ref AttributeCode instance.
     *
     * The code attribute assigns a bytecode position (index) to each instruction during code layout.
     *
     * @note Instances are created and managed by @ref AttributeCode.
     */
    class Instruction : public ClassFileElement<AttributeCode>
    {
        friend class AttributeCode;

    public:
        enum StrictCompare
        {
            Greater, ///< '>'
            Less, ///< '<'
        };

        enum Compare
        {
            Equal, ///< '=='
            NotEqual, ///< '!='
            LessThan, ///< '<'
            GreaterThan, ///< '>'
            LessEqual, ///< '<='
            GreaterEqual, ///< '>='
        };

        /**
         * @brief Primitive array element type codes for @c newarray.
         *
         * Values match the JVM specification "atype" operand.
         */
        enum Type:uint8_t
        {
            BOOLEAN = 4,
            CHAR = 5,
            FLOAT = 6,
            DOUBLE = 7,
            BYTE = 8,
            SHORT = 9,
            INT = 10,
            LONG = 11,
        };


        /**
         * @brief JVM opcode values.
         */
        enum Command: uint8_t
        {
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.nop">nop instruction</a>.
            INSTRUCTION_nop = 0x00,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.aconst_null">aconst_null instruction</a>.
            INSTRUCTION_aconst_null = 0x01,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.iconst_i">iconst_m1 instruction</a>.
            INSTRUCTION_iconst_m1 = 0x02,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.iconst_i">iconst_0 instruction</a>.
            INSTRUCTION_iconst_0 = 0x03,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.iconst_i">iconst_1 instruction</a>.
            INSTRUCTION_iconst_1 = 0x04,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.iconst_i">iconst_2 instruction</a>.
            INSTRUCTION_iconst_2 = 0x05,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.iconst_i">iconst_3 instruction</a>.
            INSTRUCTION_iconst_3 = 0x06,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.iconst_i">iconst_4 instruction</a>.
            INSTRUCTION_iconst_4 = 0x07,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.iconst_i">iconst_5 instruction</a>.
            INSTRUCTION_iconst_5 = 0x08,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.lconst_l">lconst_0 instruction</a>.
            INSTRUCTION_lconst_0 = 0x09,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.lconst_l">lconst_1 instruction</a>.
            INSTRUCTION_lconst_1 = 0x0a,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.fconst_f">fconst_0 instruction</a>.
            INSTRUCTION_fconst_0 = 0x0b,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.fconst_f">fconst_1 instruction</a>.
            INSTRUCTION_fconst_1 = 0x0c,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.fconst_f">fconst_2 instruction</a>.
            INSTRUCTION_fconst_2 = 0x0d,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.dconst_d">dconst_0 instruction</a>.
            INSTRUCTION_dconst_0 = 0x0e,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.dconst_d">dconst_1 instruction</a>.
            INSTRUCTION_dconst_1 = 0x0f,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.bipush">bipush instruction</a>.
            INSTRUCTION_bipush = 0x10,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.sipush">sipush instruction</a>.
            INSTRUCTION_sipush = 0x11,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.ldc">ldc instruction</a>.
            INSTRUCTION_ldc = 0x12,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.ldc_w">ldc_w instruction</a>.
            INSTRUCTION_ldc_w = 0x13,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.ldc2_w">ldc2_w instruction</a>.
            INSTRUCTION_ldc2_w = 0x14,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.iload">iload instruction</a>.
            INSTRUCTION_iload = 0x15,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.lload">lload instruction</a>.
            INSTRUCTION_lload = 0x16,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.fload">fload instruction</a>.
            INSTRUCTION_fload = 0x17,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.dload">dload instruction</a>.
            INSTRUCTION_dload = 0x18,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.aload">aload instruction</a>.
            INSTRUCTION_aload = 0x19,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.iload_n">iload_0 instruction</a>.
            INSTRUCTION_iload_0 = 0x1a,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.iload_n">iload_1 instruction</a>.
            INSTRUCTION_iload_1 = 0x1b,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.iload_n">iload_2 instruction</a>.
            INSTRUCTION_iload_2 = 0x1c,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.iload_n">iload_3 instruction</a>.
            INSTRUCTION_iload_3 = 0x1d,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.lload_n">lload_0 instruction</a>.
            INSTRUCTION_lload_0 = 0x1e,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.lload_n">lload_1 instruction</a>.
            INSTRUCTION_lload_1 = 0x1f,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.lload_n">lload_2 instruction</a>.
            INSTRUCTION_lload_2 = 0x20,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.lload_n">lload_3 instruction</a>.
            INSTRUCTION_lload_3 = 0x21,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.fload_n">fload_0 instruction</a>.
            INSTRUCTION_fload_0 = 0x22,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.fload_n">fload_1 instruction</a>.
            INSTRUCTION_fload_1 = 0x23,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.fload_n">fload_2 instruction</a>.
            INSTRUCTION_fload_2 = 0x24,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.fload_n">fload_3 instruction</a>.
            INSTRUCTION_fload_3 = 0x25,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.dload_n">dload_0 instruction</a>.
            INSTRUCTION_dload_0 = 0x26,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.dload_n">dload_1 instruction</a>.
            INSTRUCTION_dload_1 = 0x27,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.dload_n">dload_2 instruction</a>.
            INSTRUCTION_dload_2 = 0x28,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.dload_n">dload_3 instruction</a>.
            INSTRUCTION_dload_3 = 0x29,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.aload_n">aload_0 instruction</a>.
            INSTRUCTION_aload_0 = 0x2a,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.aload_n">aload_1 instruction</a>.
            INSTRUCTION_aload_1 = 0x2b,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.aload_n">aload_2 instruction</a>.
            INSTRUCTION_aload_2 = 0x2c,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.aload_n">aload_3 instruction</a>.
            INSTRUCTION_aload_3 = 0x2d,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.iaload">iaload instruction</a>.
            INSTRUCTION_iaload = 0x2e,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.laload">laload instruction</a>.
            INSTRUCTION_laload = 0x2f,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.faload">faload instruction</a>.
            INSTRUCTION_faload = 0x30,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.daload">daload instruction</a>.
            INSTRUCTION_daload = 0x31,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.aaload">aaload instruction</a>.
            INSTRUCTION_aaload = 0x32,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.baload">baload instruction</a>.
            INSTRUCTION_baload = 0x33,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.caload">caload instruction</a>.
            INSTRUCTION_caload = 0x34,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.saload">saload instruction</a>.
            INSTRUCTION_saload = 0x35,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.istore">istore instruction</a>.
            INSTRUCTION_istore = 0x36,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.lstore">lstore instruction</a>.
            INSTRUCTION_lstore = 0x37,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.fstore">fstore instruction</a>.
            INSTRUCTION_fstore = 0x38,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.dstore">dstore instruction</a>.
            INSTRUCTION_dstore = 0x39,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.astore">astore instruction</a>.
            INSTRUCTION_astore = 0x3a,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.istore_n">istore_0 instruction</a>.
            INSTRUCTION_istore_0 = 0x3b,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.istore_n">istore_1 instruction</a>.
            INSTRUCTION_istore_1 = 0x3c,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.istore_n">istore_2 instruction</a>.
            INSTRUCTION_istore_2 = 0x3d,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.istore_n">istore_3 instruction</a>.
            INSTRUCTION_istore_3 = 0x3e,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.lstore_n">lstore_0 instruction</a>.
            INSTRUCTION_lstore_0 = 0x3f,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.lstore_n">lstore_1 instruction</a>.
            INSTRUCTION_lstore_1 = 0x40,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.lstore_n">lstore_2 instruction</a>.
            INSTRUCTION_lstore_2 = 0x41,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.lstore_n">lstore_3 instruction</a>.
            INSTRUCTION_lstore_3 = 0x42,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.fstore_n">fstore_0 instruction</a>.
            INSTRUCTION_fstore_0 = 0x43,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.fstore_n">fstore_1 instruction</a>.
            INSTRUCTION_fstore_1 = 0x44,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.fstore_n">fstore_2 instruction</a>.
            INSTRUCTION_fstore_2 = 0x45,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.fstore_n">fstore_3 instruction</a>.
            INSTRUCTION_fstore_3 = 0x46,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.dstore_n">dstore_0 instruction</a>.
            INSTRUCTION_dstore_0 = 0x47,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.dstore_n">dstore_1 instruction</a>.
            INSTRUCTION_dstore_1 = 0x48,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.dstore_n">dstore_2 instruction</a>.
            INSTRUCTION_dstore_2 = 0x49,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.dstore_n">dstore_3 instruction</a>.
            INSTRUCTION_dstore_3 = 0x4a,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.astore_n">astore_0 instruction</a>.
            INSTRUCTION_astore_0 = 0x4b,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.astore_n">astore_1 instruction</a>.
            INSTRUCTION_astore_1 = 0x4c,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.astore_n">astore_2 instruction</a>.
            INSTRUCTION_astore_2 = 0x4d,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.astore_n">astore_3 instruction</a>.
            INSTRUCTION_astore_3 = 0x4e,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.iastore">iastore instruction</a>.
            INSTRUCTION_iastore = 0x4f,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.lastore">lastore instruction</a>.
            INSTRUCTION_lastore = 0x50,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.fastore">fastore instruction</a>.
            INSTRUCTION_fastore = 0x51,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.dastore">dastore instruction</a>.
            INSTRUCTION_dastore = 0x52,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.aastore">aastore instruction</a>.
            INSTRUCTION_aastore = 0x53,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.bastore">bastore instruction</a>.
            INSTRUCTION_bastore = 0x54,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.castore">castore instruction</a>.
            INSTRUCTION_castore = 0x55,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.sastore">sastore instruction</a>.
            INSTRUCTION_sastore = 0x56,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.pop">pop instruction</a>.
            INSTRUCTION_pop = 0x57,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.pop2">pop2 instruction</a>.
            INSTRUCTION_pop2 = 0x58,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.dup">dup instruction</a>.
            INSTRUCTION_dup = 0x59,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.dup_x1">dup_x1 instruction</a>.
            INSTRUCTION_dup_x1 = 0x5a,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.dup_x2">dup_x2 instruction</a>.
            INSTRUCTION_dup_x2 = 0x5b,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.dup2">dup2 instruction</a>.
            INSTRUCTION_dup2 = 0x5c,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.dup2_x1">dup2_x1 instruction</a>.
            INSTRUCTION_dup2_x1 = 0x5d,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.dup2_x2">dup2_x2 instruction</a>.
            INSTRUCTION_dup2_x2 = 0x5e,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.swap">swap instruction</a>.
            INSTRUCTION_swap = 0x5f,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.iadd">iadd instruction</a>.
            INSTRUCTION_iadd = 0x60,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.ladd">ladd instruction</a>.
            INSTRUCTION_ladd = 0x61,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.fadd">fadd instruction</a>.
            INSTRUCTION_fadd = 0x62,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.dadd">dadd instruction</a>.
            INSTRUCTION_dadd = 0x63,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.isub">isub instruction</a>.
            INSTRUCTION_isub = 0x64,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.lsub">lsub instruction</a>.
            INSTRUCTION_lsub = 0x65,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.fsub">fsub instruction</a>.
            INSTRUCTION_fsub = 0x66,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.dsub">dsub instruction</a>.
            INSTRUCTION_dsub = 0x67,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.imul">imul instruction</a>.
            INSTRUCTION_imul = 0x68,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.lmul">lmul instruction</a>.
            INSTRUCTION_lmul = 0x69,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.fmul">fmul instruction</a>.
            INSTRUCTION_fmul = 0x6a,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.dmul">dmul instruction</a>.
            INSTRUCTION_dmul = 0x6b,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.idiv">idiv instruction</a>.
            INSTRUCTION_idiv = 0x6c,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.ldiv">ldiv instruction</a>.
            INSTRUCTION_ldiv = 0x6d,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.fdiv">fdiv instruction</a>.
            INSTRUCTION_fdiv = 0x6e,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.ddiv">ddiv instruction</a>.
            INSTRUCTION_ddiv = 0x6f,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.irem">irem instruction</a>.
            INSTRUCTION_irem = 0x70,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.lrem">lrem instruction</a>.
            INSTRUCTION_lrem = 0x71,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.frem">frem instruction</a>.
            INSTRUCTION_frem = 0x72,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.drem">drem instruction</a>.
            INSTRUCTION_drem = 0x73,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.ineg">ineg instruction</a>.
            INSTRUCTION_ineg = 0x74,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.lneg">lneg instruction</a>.
            INSTRUCTION_lneg = 0x75,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.fneg">fneg instruction</a>.
            INSTRUCTION_fneg = 0x76,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.dneg">dneg instruction</a>.
            INSTRUCTION_dneg = 0x77,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.ishl">ishl instruction</a>.
            INSTRUCTION_ishl = 0x78,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.lshl">lshl instruction</a>.
            INSTRUCTION_lshl = 0x79,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.ishr">ishr instruction</a>.
            INSTRUCTION_ishr = 0x7a,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.lshr">lshr instruction</a>.
            INSTRUCTION_lshr = 0x7b,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.iushr">iushr instruction</a>.
            INSTRUCTION_iushr = 0x7c,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.lushr">lushr instruction</a>.
            INSTRUCTION_lushr = 0x7d,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.iand">iand instruction</a>.
            INSTRUCTION_iand = 0x7e,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.land">land instruction</a>.
            INSTRUCTION_land = 0x7f,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.ior">ior instruction</a>.
            INSTRUCTION_ior = 0x80,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.lor">lor instruction</a>.
            INSTRUCTION_lor = 0x81,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.ixor">ixor instruction</a>.
            INSTRUCTION_ixor = 0x82,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.lxor">lxor instruction</a>.
            INSTRUCTION_lxor = 0x83,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.iinc">iinc instruction</a>.
            INSTRUCTION_iinc = 0x84,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.i2l">i2l instruction</a>.
            INSTRUCTION_i2l = 0x85,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.i2f">i2f instruction</a>.
            INSTRUCTION_i2f = 0x86,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.i2d">i2d instruction</a>.
            INSTRUCTION_i2d = 0x87,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.l2i">l2i instruction</a>.
            INSTRUCTION_l2i = 0x88,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.l2f">l2f instruction</a>.
            INSTRUCTION_l2f = 0x89,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.l2d">l2d instruction</a>.
            INSTRUCTION_l2d = 0x8a,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.f2i">f2i instruction</a>.
            INSTRUCTION_f2i = 0x8b,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.f2l">f2l instruction</a>.
            INSTRUCTION_f2l = 0x8c,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.f2d">f2d instruction</a>.
            INSTRUCTION_f2d = 0x8d,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.d2i">d2i instruction</a>.
            INSTRUCTION_d2i = 0x8e,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.d2l">d2l instruction</a>.
            INSTRUCTION_d2l = 0x8f,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.d2f">d2f instruction</a>.
            INSTRUCTION_d2f = 0x90,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.i2b">i2b instruction</a>.
            INSTRUCTION_i2b = 0x91,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.i2c">i2c instruction</a>.
            INSTRUCTION_i2c = 0x92,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.i2s">i2s instruction</a>.
            INSTRUCTION_i2s = 0x93,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.lcmp">lcmp instruction</a>.
            INSTRUCTION_lcmp = 0x94,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.fcmp_op">fcmpl instruction</a>.
            INSTRUCTION_fcmpl = 0x95,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.fcmp_op">fcmpg instruction</a>.
            INSTRUCTION_fcmpg = 0x96,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.dcmp_op">dcmpl instruction</a>.
            INSTRUCTION_dcmpl = 0x97,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.dcmp_op">dcmpg instruction</a>.
            INSTRUCTION_dcmpg = 0x98,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.if_cond">ifeq instruction</a>.
            INSTRUCTION_ifeq = 0x99,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.if_cond">ifne instruction</a>.
            INSTRUCTION_ifne = 0x9a,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.if_cond">iflt instruction</a>.
            INSTRUCTION_iflt = 0x9b,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.if_cond">ifge instruction</a>.
            INSTRUCTION_ifge = 0x9c,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.if_cond">ifgt instruction</a>.
            INSTRUCTION_ifgt = 0x9d,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.if_cond">ifle instruction</a>.
            INSTRUCTION_ifle = 0x9e,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.if_icmp_cond">if_icmpeq instruction</a>.
            INSTRUCTION_if_icmpeq = 0x9f,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.if_icmp_cond">if_icmpne instruction</a>.
            INSTRUCTION_if_icmpne = 0xa0,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.if_icmp_cond">if_icmplt instruction</a>.
            INSTRUCTION_if_icmplt = 0xa1,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.if_icmp_cond">if_icmpge instruction</a>.
            INSTRUCTION_if_icmpge = 0xa2,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.if_icmp_cond">if_icmpgt instruction</a>.
            INSTRUCTION_if_icmpgt = 0xa3,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.if_icmp_cond">if_icmple instruction</a>.
            INSTRUCTION_if_icmple = 0xa4,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.if_icmp_cond">if_acmpeq instruction</a>.
            INSTRUCTION_if_acmpeq = 0xa5,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.if_icmp_cond">if_acmpne instruction</a>.
            INSTRUCTION_if_acmpne = 0xa6,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.goto">goto instruction</a>.
            INSTRUCTION_goto = 0xa7,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.jsr">jsr instruction</a>.
            INSTRUCTION_jsr = 0xa8,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.ret">ret instruction</a>.
            INSTRUCTION_ret = 0xa9,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.tableswitch">tableswitch instruction</a>.
            INSTRUCTION_tableswitch = 0xaa,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.lookupswitch">lookupswitch instruction</a>.
            INSTRUCTION_lookupswitch = 0xab,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.ireturn">ireturn instruction</a>.
            INSTRUCTION_ireturn = 0xac,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.lreturn">lreturn instruction</a>.
            INSTRUCTION_lreturn = 0xad,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.freturn">freturn instruction</a>.
            INSTRUCTION_freturn = 0xae,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.dreturn">dreturn instruction</a>.
            INSTRUCTION_dreturn = 0xaf,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.areturn">areturn instruction</a>.
            INSTRUCTION_areturn = 0xb0,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.return">return instruction</a>.
            INSTRUCTION_return = 0xb1,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.getstatic">getstatic instruction</a>.
            INSTRUCTION_getstatic = 0xb2,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.putstatic">putstatic instruction</a>.
            INSTRUCTION_putstatic = 0xb3,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.getfield">getfield instruction</a>.
            INSTRUCTION_getfield = 0xb4,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.putfield">putfield instruction</a>.
            INSTRUCTION_putfield = 0xb5,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.invokevirtual">invokevirtual instruction</a>.
            INSTRUCTION_invokevirtual = 0xb6,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.invokespecial">invokespecial instruction</a>.
            INSTRUCTION_invokespecial = 0xb7,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.invokestatic">invokestatic instruction</a>.
            INSTRUCTION_invokestatic = 0xb8,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.invokeinterface">invokeinterface instruction</a>.
            INSTRUCTION_invokeinterface = 0xb9,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.invokedynamic">invokedynamic instruction</a>.
            INSTRUCTION_invokedynamic = 0xba,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.new">new instruction</a>.
            INSTRUCTION_new = 0xbb,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.newarray">newarray instruction</a>.
            INSTRUCTION_newarray = 0xbc,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.anewarray">anewarray instruction</a>.
            INSTRUCTION_anewarray = 0xbd,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.arraylength">arraylength instruction</a>.
            INSTRUCTION_arraylength = 0xbe,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.athrow">athrow instruction</a>.
            INSTRUCTION_athrow = 0xbf,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.checkcast">checkcast instruction</a>.
            INSTRUCTION_checkcast = 0xc0,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.instanceof">instanceof instruction</a>.
            INSTRUCTION_instanceof = 0xc1,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.monitorenter">monitorenter instruction</a>.
            INSTRUCTION_monitorenter = 0xc2,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.monitorexit">monitorexit instruction</a>.
            INSTRUCTION_monitorexit = 0xc3,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.wide">wide instruction</a>.
            INSTRUCTION_wide = 0xc4,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.multianewarray">multianewarray instruction</a>.
            INSTRUCTION_multianewarray = 0xc5,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.ifnull">ifnull instruction</a>.
            INSTRUCTION_ifnull = 0xc6,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.ifnonnull">ifnonnull instruction</a>.
            INSTRUCTION_ifnonnull = 0xc7,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.goto_w">goto_w instruction</a>.
            INSTRUCTION_goto_w = 0xc8,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.5.jsr_w">jsr_w instruction</a>.
            INSTRUCTION_jsr_w = 0xc9,
            /// Oracle docs: <a href="https://docs.oracle.com/javase/specs/jvms/se16/html/jvms-6.html#jvms-6.2">breakpoint instruction</a>.
            INSTRUCTION_breakpoint = 0xca,
        };

        /**
         * @brief Get the opcode of this instruction.
         *
         * @return Opcode value.
         */
        [[nodiscard]] Command getCommandCode() const;

        /**
         * @brief Check whether a bytecode position has been assigned to this instruction.
         *
         * The position is assigned by @ref AttributeCode during code layout.
         *
         * @return @c true if position is set; otherwise @c false.
         */
        [[nodiscard]] bool isIndexSet() const;

        /**
         * @brief Get the bytecode position assigned to this instruction.
         *
         * @return Instruction position in the code stream.
         * @throws std::logic_error if position is not set (typically checked via @ref isIndexSet).
         */
        [[nodiscard]] uint16_t getIndex() const;

    protected:
        /**
         * @brief Construct an instruction with the given opcode.
         *
         * @param attributeCode Owning code attribute.
         * @param command Instruction opcode.
         */
        Instruction(AttributeCode* attributeCode, Command command);

        /**
         * @brief Update the opcode of this instruction.
         *
         * @param newCommand New opcode.
         */
        void setCommand(Command newCommand);

        /**
         * @brief Compute a relative branch displacement to the target instruction.
         *
         * @param target Target instruction.
         * @return Relative displacement.
         * @throws std::logic_error If the source or target instruction position is not set.
         */
        [[nodiscard]] uint16_t calculateShift(Instruction* target) const;

        [[nodiscard]] std::size_t getByteSize() const override;

        void writeTo(std::ostream& os) const override;

    private:
        /**
         * @brief Assign a bytecode position to this instruction.
         *
         * Used internally by @ref AttributeCode during code layout.
         *
         * @param index Bytecode position.
         */
        void setIndex(uint16_t index);

        /**
         * @brief Reset the assigned bytecode position.
         */
        void resetIndex();

        Command command_; ///< Opcode of this instruction.
        uint16_t index_ = 0; ///< Bytecode position assigned during layout.
        bool isIndexSet_ = false; ///< True if @ref index_ is valid.
    };
} // jvm

#endif //JVM__INSTRUCTION_H
