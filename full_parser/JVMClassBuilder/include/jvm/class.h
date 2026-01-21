#ifndef JVM__CLASS_H
#define JVM__CLASS_H

#include <cstdint>
#include <set>
#include <span>
#include <string>
#include <vector>

#include "serializable.h"

namespace jvm
{
    class DescriptorMethod;
    class ConstantDouble;
    class ConstantLong;
    class ConstantFloat;
    class ConstantInteger;
    class ConstantString;
    class ConstantInterfaceMethodref;
    class ConstantMethodref;
    class ConstantNameAndType;
    class ConstantFieldref;
    class ConstantUtf8Info;
    class ConstantClass;
    class Constant;
    class Field;
    class Method;
    class Attribute;
    class Descriptor;
    class DescriptorField;
    class DescriptorMethod;

    enum MajorVersion:uint16_t
    {
        MAJOR_VERSION_1_0_2 = 45,
        MAJOR_VERSION_1_1 = 45,
        MAJOR_VERSION_1_2 = 46,
        MAJOR_VERSION_1_3 = 47,
        MAJOR_VERSION_1_4 = 48,
        MAJOR_VERSION_5 = 49,
        MAJOR_VERSION_6 = 50,
        MAJOR_VERSION_7 = 51,
        MAJOR_VERSION_8 = 52,
        MAJOR_VERSION_9 = 53,
        MAJOR_VERSION_10 = 54,
        MAJOR_VERSION_11 = 55,
        MAJOR_VERSION_12 = 56,
        MAJOR_VERSION_13 = 57,
        MAJOR_VERSION_14 = 58,
        MAJOR_VERSION_15 = 59,
        MAJOR_VERSION_16 = 60,
    };

    class Class : Serializable
    {
        static uint16_t minorVersion;
        static MajorVersion majorVersion;

    public:
        enum AccessFlag
        {
            ACC_PUBLIC = 0x0001, // Declared public; may be accessed from outside its package.
            ACC_FINAL = 0x0010, // Declared final; no subclasses allowed.
            ACC_SUPER = 0x0020, // Treat superclass methods specially when invoked by the invokespecial instruction.
            ACC_INTERFACE = 0x0200, // Is an interface, not a class.
            ACC_ABSTRACT = 0x0400, // Declared abstract; must not be instantiated.
            ACC_SYNTHETIC = 0x1000, // Declared synthetic; not present in the source code.
            ACC_ANNOTATION = 0x2000, // Declared as an annotation interface.
            ACC_ENUM = 0x4000, // Declared as an enum class.
            ACC_MODULE = 0x8000, // Is a module, not a class or interface.
        };

        Class(const std::string& className, const std::string& parentName);

        //region GET OR CREATE CLASS CONSTANT
        /**
         * @brief Returns an existing @ref ConstantClass "Class constant" from this class's constant pool,
         *        or creates and returns a new one.
         * @note May create a new @ref ConstantUtf8Info entry for the class name.
         * @param name Class internal name (e.g. "java/lang/String").
         * @return Class constant.
         */
        ConstantClass* getOrCreateClassConstant(const std::string& name);

        /**
         * @brief Returns an existing @ref ConstantClass "Class constant" from this class's constant pool,
         *        or creates and returns a new one.
         * @note The provided constant must have this @c Class instance as its owner.
         * @param name UTF-8 constant containing the class internal name.
         * @return Class constant.
         */
        ConstantClass* getOrCreateClassConstant(ConstantUtf8Info* name);
        //endregion
        //region GET OR CREATE FIELDREF CONSTANT

        /**
         * @brief Returns an existing @ref ConstantFieldref "Fieldref constant" from this class's constant pool,
         *        or creates and returns a new one.
         * @note May create @ref ConstantClass and @ref ConstantNameAndType entries as needed.
         * @param className Class internal name (e.g. "java/lang/System").
         * @param fieldName Field name.
         * @param fieldDescriptor Descriptor object representing a field descriptor.
         * @return Fieldref constant.
         */
        ConstantFieldref* getOrCreateFieldrefConstant(const std::string& className, const std::string& fieldName,
                                                      const DescriptorField& fieldDescriptor);

        /**
         * @brief Returns an existing @ref ConstantFieldref "Fieldref constant" from this class's constant pool,
         *        or creates and returns a new one.
         * @note May create @ref ConstantClass and @ref ConstantNameAndType entries as needed.
         * @param classConstant Class constant.
         * @param fieldName Field name.
         * @param fieldDescriptor Descriptor object representing a field descriptor.
         * @return Fieldref constant.
         */
        ConstantFieldref* getOrCreateFieldrefConstant(ConstantClass* classConstant, const std::string& fieldName,
                                                      const DescriptorField& fieldDescriptor);

        /**
         * @brief Returns an existing @ref ConstantFieldref "Fieldref constant" from this class's constant pool,
         *        or creates and returns a new one.
         * @note May create @ref ConstantClass and @ref ConstantNameAndType entries as needed.
         * @param classConstant Class constant.
         * @param fieldNameConstant Field constant.
         * @param fieldDescriptor Descriptor object representing a field descriptor.
         * @return Fieldref constant.
         */
        ConstantFieldref* getOrCreateFieldrefConstant(ConstantClass* classConstant, ConstantUtf8Info* fieldNameConstant,
                                                      const DescriptorField& fieldDescriptor);

        /**
         * @brief Returns an existing @ref ConstantFieldref "Fieldref constant" from this class's constant pool,
         *        or creates and returns a new one.
         * @note May create @ref ConstantClass and @ref ConstantNameAndType entries as needed.
         * @param classConstant Class constant.
         * @param fieldNameConstant Field constant.
         * @param fieldDescriptorConstant Descriptor constant.
         * @return Fieldref constant.
         */
        ConstantFieldref* getOrCreateFieldrefConstant(ConstantClass* classConstant, ConstantUtf8Info* fieldNameConstant,
                                                      ConstantUtf8Info* fieldDescriptorConstant);

        /**
         * @brief Returns an existing @ref ConstantFieldref "Fieldref constant" from this class's constant pool,
         *        or creates and returns a new one.
         * @note The provided constants must have this @c Class instance as their owner.
         * @param classConstant Class constant.
         * @param nameAndTypeConstant Name-and-type constant for the field.
         * @return Fieldref constant.
         */
        ConstantFieldref* getOrCreateFieldrefConstant(ConstantClass* classConstant,
                                                      ConstantNameAndType* nameAndTypeConstant);
        //endregion
        //region GET OR CREATE METHODREF CONSTANT

        /**
         * @brief Returns an existing @ref ConstantMethodref "Methodref constant" from this class's constant pool,
         *        or creates and returns a new one.
         * @note May create @ref ConstantClass and @ref ConstantNameAndType entries as needed.
         * @param className Class internal name (e.g. "java/lang/String").
         * @param methodName Method name (e.g. "hashCode", "&lt;init&gt;", "&lt;clinit&gt;").
         * @param methodDescriptor Descriptor object representing a method descriptor.
         * @return Methodref constant.
         */
        ConstantMethodref* getOrCreateMethodrefConstant(const std::string& className, const std::string& methodName,
                                                        const DescriptorMethod& methodDescriptor);

        /**
         * @brief Returns an existing @ref ConstantMethodref "Methodref constant" from this class's constant pool,
         *        or creates and returns a new one.
         * @note May create @ref ConstantClass and @ref ConstantNameAndType entries as needed.
         * @param classConstant Class constant.
         * @param methodName Method name (e.g. "hashCode", "&lt;init&gt;", "&lt;clinit&gt;").
         * @param methodDescriptor Descriptor object representing a method descriptor.
         * @return Methodref constant.
         */
        ConstantMethodref* getOrCreateMethodrefConstant(ConstantClass* classConstant,
                                                        const std::string& methodName,
                                                        const DescriptorMethod& methodDescriptor);

        /**
         * @brief Returns an existing @ref ConstantMethodref "Methodref constant" from this class's constant pool,
         *        or creates and returns a new one.
         * @note May create @ref ConstantClass and @ref ConstantNameAndType entries as needed.
         * @param classConstant Class constant.
         * @param methodNameConstant Method constant.
         * @param methodDescriptor Descriptor object representing a method descriptor.
         * @return Methodref constant.
         */
        ConstantMethodref* getOrCreateMethodrefConstant(ConstantClass* classConstant,
                                                        ConstantUtf8Info* methodNameConstant,
                                                        const DescriptorMethod& methodDescriptor);

        /**
         * @brief Returns an existing @ref ConstantMethodref "Methodref constant" from this class's constant pool,
         *        or creates and returns a new one.
         * @note May create @ref ConstantClass and @ref ConstantNameAndType entries as needed.
         * @param classConstant Class constant.
         * @param methodNameConstant Method constant.
         * @param methodDescriptorConstant Descriptor constant.
         * @return Methodref constant.
         */
        ConstantMethodref* getOrCreateMethodrefConstant(ConstantClass* classConstant,
                                                        ConstantUtf8Info* methodNameConstant,
                                                        ConstantUtf8Info* methodDescriptorConstant);

        /**
         * @brief Returns an existing @ref ConstantMethodref "Methodref constant" from this class's constant pool,
         *        or creates and returns a new one.
         * @note The provided constants must have this @c Class instance as their owner.
         * @param classConstant Class constant.
         * @param nameAndTypeConstant Name-and-type constant for the method.
         * @return Methodref constant.
         */
        ConstantMethodref* getOrCreateMethodrefConstant(ConstantClass* classConstant,
                                                        ConstantNameAndType* nameAndTypeConstant);
        //endregion
        //region GET OR CREATE INTERFACE METHODREF CONSTANT


        /**
         * @brief Returns an existing @ref ConstantInterfaceMethodref "InterfaceMethodref constant" from this class's constant pool,
         *        or creates and returns a new one.
         * @note May create @ref ConstantClass and @ref ConstantNameAndType entries as needed.
         * @param className Interface internal name (e.g. "java/lang/Runnable").
         * @param methodName Interface method name.
         * @param methodDescriptor Descriptor object representing a method descriptor.
         * @return Interface methodref constant.
         */
        ConstantInterfaceMethodref* getOrCreateInterfaceMethodrefConstant(
            const std::string& className, const std::string& methodName, const DescriptorMethod& methodDescriptor);

        /**
         * @brief Returns an existing @ref ConstantInterfaceMethodref "InterfaceMethodref constant" from this class's constant pool,
         *        or creates and returns a new one.
         * @note May create @ref ConstantClass and @ref ConstantNameAndType entries as needed.
         * @param classConstant Class constant.
         * @param methodName Interface method name.
         * @param methodDescriptor Descriptor object representing a method descriptor.
         * @return Interface methodref constant.
         */
        ConstantInterfaceMethodref* getOrCreateInterfaceMethodrefConstant(
            ConstantClass* classConstant, const std::string& methodName, const DescriptorMethod& methodDescriptor);

        /**
         * @brief Returns an existing @ref ConstantInterfaceMethodref "InterfaceMethodref constant" from this class's constant pool,
         *        or creates and returns a new one.
         * @note May create @ref ConstantClass and @ref ConstantNameAndType entries as needed.
         * @param classConstant Interface class constant.
         * @param methodNameConstant Interface method name constant.
         * @param methodDescriptor Descriptor object representing a method descriptor.
         * @return Interface methodref constant.
         */
        ConstantInterfaceMethodref* getOrCreateInterfaceMethodrefConstant(
            ConstantClass* classConstant, ConstantUtf8Info* methodNameConstant,
            const DescriptorMethod& methodDescriptor);

        /**
         * @brief Returns an existing @ref ConstantInterfaceMethodref "InterfaceMethodref constant" from this class's constant pool,
         *        or creates and returns a new one.
         * @note May create @ref ConstantClass and @ref ConstantNameAndType entries as needed.
         * @param classConstant Interface class constant.
         * @param methodNameConstant Interface method name constant.
         * @param methodDescriptorConstant Descriptor constant.
         * @return Interface methodref constant.
         */
        ConstantInterfaceMethodref* getOrCreateInterfaceMethodrefConstant(
            ConstantClass* classConstant, ConstantUtf8Info* methodNameConstant,
            ConstantUtf8Info* methodDescriptorConstant);


        /**
         * @brief Returns an existing @ref ConstantInterfaceMethodref "InterfaceMethodref constant" from this class's constant pool,
         *        or creates and returns a new one.
         * @note The provided constants must have this @c Class instance as their owner.
         * @param classConstant Class constant.
         * @param nameAndTypeConstant Name-and-type constant for the interface method.
         * @return Interface methodref constant.
         */
        ConstantInterfaceMethodref* getOrCreateInterfaceMethodrefConstant(ConstantClass* classConstant,
                                                                          ConstantNameAndType* nameAndTypeConstant);
        //endregion
        //region GET OR CREATE STRING CONSTANT
        /**
         * @brief Returns an existing @ref ConstantString "String constant" from this class's constant pool,
         *        or creates and returns a new one.
         * @note May create a new @ref ConstantUtf8Info entry for the string contents.
         * @param value String contents.
         * @return String constant.
         */
        ConstantString* getOrCreateStringConstant(const std::string& value);

        /**
         * @brief Returns an existing @ref ConstantString "String constant" from this class's constant pool,
         *        or creates and returns a new one.
         * @note The provided constant must have this @c Class instance as its owner.
         * @param utf8Constant UTF-8 constant containing the string contents.
         * @return String constant.
         */
        ConstantString* getOrCreateStringConstant(ConstantUtf8Info* utf8Constant);
        //endregion
        //region GET OR CREATE INTEGER CONSTANT
        /**
         * @brief Returns an existing @ref ConstantInteger "Integer constant" from this class's constant pool,
         *        or creates and returns a new one.
         * @param value Integer value.
         * @return Integer constant.
         */
        ConstantInteger* getOrCreateIntegerConstant(int32_t value);
        //endregion
        //region GET OR CREATE FLOAT CONSTANT
        /**
         * @brief Returns an existing @ref ConstantFloat "Float constant" from this class's constant pool,
         *        or creates and returns a new one.
         * @param value Float value.
         * @return Float constant.
         */
        ConstantFloat* getOrCreateFloatConstant(float value);
        //endregion
        //region GET OR CREATE LONG CONSTANT
        /**
         * @brief Returns an existing @ref ConstantLong "Long constant" from this class's constant pool,
         *        or creates and returns a new one.
         *
         * @param value Long value.
         * @return Long constant.
         */
        ConstantLong* getOrCreateLongConstant(int64_t value);
        //endregion
        //region GET OR CREATE DOUBLE CONSTANT
        /**
         * @brief Returns an existing @ref ConstantDouble "Double constant" from this class's constant pool,
         *        or creates and returns a new one.
         * @param value Double value.
         * @return Double constant.
         */
        ConstantDouble* getOrCreateDoubleConstant(double value);
        //endregion
        //region GET OR CREATE NAME AND TYPE CONSTANT

        /**
         * @brief Returns an existing @ref ConstantNameAndType "Name-and-type constant" from this class's constant pool,
         *        or creates and returns a new one.
         * @note May create a new @ref ConstantUtf8Info entry for the name.
         * @note The provided constant must have this @c Class instance as its owner.
         * @param name Name.
         * @param descriptorConstant UTF-8 constant containing the descriptor.
         * @return Name-and-type constant.
         */
        ConstantNameAndType* getOrCreateNameAndTypeConstant(const std::string& name,
                                                            ConstantUtf8Info* descriptorConstant);

        /**
         * @brief Returns an existing @ref ConstantNameAndType "Name-and-type constant" from this class's constant pool,
         *        or creates and returns a new one.
         * @note May create a new @ref ConstantUtf8Info entry for the name.
         * @note The provided constant must have this @c Class instance as its owner.
         * @param name Name.
         * @param descriptor Descriptor object representing a field or method descriptor.
         * @return Name-and-type constant.
         */
        ConstantNameAndType* getOrCreateNameAndTypeConstant(const std::string& name, const Descriptor& descriptor);

        /**
         * @brief Returns an existing @ref ConstantNameAndType "Name-and-type constant" from this class's constant pool,
         *        or creates and returns a new one.
         * @note May create a new @ref ConstantUtf8Info entry for the name.
         * @note The provided constant must have this @c Class instance as its owner.
         * @param nameConstant UTF-8 constant containing the name.
         * @param descriptor Descriptor object representing a field or method descriptor.
         * @return Name-and-type constant.
         */
        ConstantNameAndType* getOrCreateNameAndTypeConstant(ConstantUtf8Info* nameConstant,
                                                            const Descriptor& descriptor);

        /**
         * @brief Returns an existing @ref ConstantNameAndType "Name-and-type constant" from this class's constant pool,
         *        or creates and returns a new one.
         * @note The provided constants must have this @c Class instance as their owner.
         * @param nameConstant UTF-8 constant containing the name.
         * @param descriptorConstant UTF-8 constant containing the descriptor.
         * @return Name-and-type constant.
         */
        ConstantNameAndType* getOrCreateNameAndTypeConstant(ConstantUtf8Info* nameConstant,
                                                            ConstantUtf8Info* descriptorConstant);
        //endregion
        //region GET OR CREATE UTF-8 CONSTANT
        /**
         * @brief Returns an existing @ref ConstantUtf8Info "UTF-8 constant" from this class's constant pool,
         *        or creates and returns a new one.
         * @param value UTF-8 string value (stored using the class-file UTF-8 format).
         * @return UTF-8 constant.
         */
        ConstantUtf8Info* getOrCreateUtf8Constant(const std::string& value);
        //endregion

        //region GET OR CREATE FIELD

        /**
        * @brief Returns an existing @ref Field "field" with the specified name and descriptor,
         *        or creates and returns a new one.
         *
         * The field is identified by its name and descriptor within this class.
         *
         * @param name Field name (e.g. "a", "object", "&lt;init&gt;", "&lt;clinit&gt;").
         * @param descriptor Descriptor object representing a field descriptor.
         * @return Field instance owned by this class.
         */
        Field* getOrCreateField(const std::string& name, const DescriptorField& descriptor);

        /**
        * @brief Returns an existing @ref Field "field" with the specified name and descriptor,
         *        or creates and returns a new one.
         *
         * The field is identified by its name and descriptor within this class.
         *
         * @param name Field name (e.g. "a", "object", "&lt;init&gt;", "&lt;clinit&gt;").
         * @param descriptorConstant UTF-8 constant containing the field descriptor.
         * @return Field instance owned by this class.
         */
        Field* getOrCreateField(const std::string& name, ConstantUtf8Info* descriptorConstant);

        /**
         * @brief Returns an existing @ref Field "field" with the specified name and descriptor,
          *        or creates and returns a new one.
          *
          * The field is identified by its name and descriptor within this class.
          *
          * @param nameConstant UTF-8 constant containing the method name.
          * @param descriptor Descriptor object representing a field descriptor.
          * @return Field instance owned by this class.
          */
        Field* getOrCreateField(ConstantUtf8Info* nameConstant, const DescriptorField& descriptor);

        /**
         * @brief Returns an existing @ref Field "field" with the specified name and descriptor,
          *        or creates and returns a new one.
          *
          * The field is identified by its name and descriptor within this class.
          *
          * @param nameConstant UTF-8 constant containing the method name.
          * @param descriptorConstant UTF-8 constant containing the field descriptor.
          * @return Field instance owned by this class.
          */
        Field* getOrCreateField(ConstantUtf8Info* nameConstant, ConstantUtf8Info* descriptorConstant);

        //region GET OR CREATE METHOD

        /**
        * @brief Returns an existing @ref Method "method" with the specified name and descriptor,
         *        or creates and returns a new one.
         *
         * The method is identified by its name and descriptor within this class.
         *
         * @param name Method name (e.g. "run", "main", "&lt;init&gt;", "&lt;clinit&gt;").
         * @param descriptor Descriptor object representing a method descriptor.
         * @return Method instance owned by this class.
         */
        Method* getOrCreateMethod(const std::string& name, const DescriptorMethod& descriptor);

        /**
        * @brief Returns an existing @ref Method "method" with the specified name and descriptor,
         *        or creates and returns a new one.
         *
         * The method is identified by its name and descriptor within this class.
         *
         * @param name Method name (e.g. "run", "main", "&lt;init&gt;", "&lt;clinit&gt;").
         * @param descriptorConstant UTF-8 constant containing the method descriptor.
         * @return Method instance owned by this class.
         */
        Method* getOrCreateMethod(const std::string& name, ConstantUtf8Info* descriptorConstant);

        /**
        * @brief Returns an existing @ref Method "method" with the specified name and descriptor,
         *        or creates and returns a new one.
         *
         * The method is identified by its name and descriptor within this class.
         *
         * @param nameConstant UTF-8 constant containing the method name.
         * @param descriptor Descriptor object representing a method descriptor.
         * @return Method instance owned by this class.
         */
        Method* getOrCreateMethod(ConstantUtf8Info* nameConstant, const DescriptorMethod& descriptor);

        /**
         * @brief Returns an existing @ref Method "method" with the specified name and descriptor,
         *        or creates and returns a new one.
         *
         * The method is identified by its name and descriptor within this class.
         *
         * @note The provided constants must have this @c Class instance as their owner.
         * @param nameConstant UTF-8 constant containing the method name.
         * @param descriptorConstant UTF-8 constant containing the method descriptor.
         * @return Method instance owned by this class.
         */
        Method* getOrCreateMethod(ConstantUtf8Info* nameConstant, ConstantUtf8Info* descriptorConstant);
        //endregion

        std::span<Constant*> constants();

        /**
         * Add access flag to class.
         * @param flag Access flag.
         */
        void addFlag(AccessFlag flag);

        /**
         * Remove access flag from class.
         * @param flag Access flag.
         */
        void removeFlag(AccessFlag flag);

        void writeTo(std::ostream& os) const override;

        /**
         * @return Access flags set.
         */
        [[nodiscard]] const std::set<AccessFlag>* getAccessFlags() const;

        [[nodiscard]] std::size_t getByteSize() const override;

    private:
        /**
         * @brief Add a constant to the constant pool.
         * Add a constant to constant pool and set index to the constant.
         * @param constant New constant.
         */
        void addNewConstant(Constant* constant);

        /**
         * @brief Validates JVM class access flags for logical consistency.
         *
         * Checks that the combination of class @ref AccessFlag values is valid according to the JVM class file format.
         * This method does not modify the flags; it only throws an exception if an invalid combination is detected.
         *
         * The validation covers (at minimum):
         * - visibility rules (only @ref ACC_PUBLIC is applicable for top-level classes; no private/protected),
         * - mutual exclusivity constraints (e.g. abstract vs final),
         * - interface-related constraints (e.g. interface must also be abstract),
         * - annotation constraints (annotation implies interface),
         * - enum constraints,
         * - module constraints (module class-files may only use a very limited set of flags).
         *
         * Typical usage is to call this method right before serializing class access flags into the class file
         * (e.g. inside @ref writeTo).
         *
         * @param flags Bit mask containing class access flags (OR-combination of @ref AccessFlag).
         *
         * @throws std::logic_error If an invalid combination of flags is found.
         *
         * @note This validation follows the JVM Specification rules for @c ClassFile::access_flags.
         */
        static void validateFlags(uint16_t flags);

        /**
         * Fix class (code attributes) using java project.
         * @note Run JVM in this method.
         * @param os Output stream.
         * @param data @c Class in binary format.
         */
        static void fixClassBinary(std::ostream& os, const std::span<const unsigned char>& data);

        std::vector<Constant*> constants_{};
        uint16_t nextCpIndex = 1; // 0 index is not available for writing
        std::set<AccessFlag> accessFlags_{};
        Constant* thisClassConstant_ = nullptr;
        Constant* superClassConstant_ = nullptr;
        std::set<Constant*> interfacesConstant_{};
        std::set<Field*> fields_;
        std::set<Method*> methods_;
        std::set<Attribute*> attributes_;
    };
}
#endif //JVM__CLASS_H
