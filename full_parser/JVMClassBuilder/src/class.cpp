#include "jvm/class.h"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <jni.h>
#include <ostream>
#include <sstream>
#include <utility>
#include <fstream>
#include <filesystem>
#include <iostream>

#include "jvm/constant.h"
#include "jvm/constant-class.h"
#include "jvm/constant-double.h"
#include "jvm/constant-fieldref.h"
#include "jvm/constant-float.h"
#include "jvm/constant-integer.h"
#include "jvm/constant-interface-methodref.h"
#include "jvm/constant-long.h"
#include "jvm/constant-methodref.h"
#include "jvm/constant-name-and-type.h"
#include "jvm/constant-string.h"
#include "jvm/constant-utf-8-info.h"
#include "jvm/descriptor-method.h"
#include "jvm/descriptor.h"
#include "jvm/field.h"
#include "jvm/method.h"
#include "jvm/internal/utils.h"
#include "java-internal-paths.h"

namespace fs = std::filesystem;
using namespace jvm;

MajorVersion Class::majorVersion = MAJOR_VERSION_16;

Class::Class(const std::string& className, const std::string& parentName)
{
    thisClassConstant_ = getOrCreateClassConstant(className);
    superClassConstant_ = getOrCreateClassConstant(parentName);
}

uint16_t Class::minorVersion = 0x0000;

ConstantClass* Class::getOrCreateClassConstant(const std::string& name)
{
    ConstantUtf8Info* nameConstant = getOrCreateUtf8Constant(name);
    return getOrCreateClassConstant(nameConstant);
}

ConstantClass* Class::getOrCreateClassConstant(ConstantUtf8Info* name)
{
    assert(this == name->getOwner());

    // try search constant
    for (auto* constant : constants_)
    {
        if (constant->getTag() == Constant::CONSTANT_Class)
        {
            // Use static method because only one tag can be associated with only one class type.
            auto* classConstant = static_cast<ConstantClass*>(constant);
            if (classConstant->getName() == name)
            {
                return classConstant;
            }
        }
    }

    // create new
    auto* classConstant = new ConstantClass(name);
    addNewConstant(classConstant);
    return classConstant;
}

ConstantFieldref* Class::getOrCreateFieldrefConstant(const std::string& className, const std::string& fieldName,
                                                     const DescriptorField& fieldDescriptor)
{
    ConstantClass* classConstant = getOrCreateClassConstant(className);
    ConstantNameAndType* nameAndTypeConstant = getOrCreateNameAndTypeConstant(fieldName, fieldDescriptor);
    return getOrCreateFieldrefConstant(classConstant, nameAndTypeConstant);
}

ConstantFieldref* Class::getOrCreateFieldrefConstant(ConstantClass* classConstant, const std::string& fieldName,
                                                     const DescriptorField& fieldDescriptor)
{
    ConstantNameAndType* nameAndTypeConstant = getOrCreateNameAndTypeConstant(fieldName, fieldDescriptor);
    return getOrCreateFieldrefConstant(classConstant, nameAndTypeConstant);
}

ConstantFieldref* Class::getOrCreateFieldrefConstant(ConstantClass* classConstant, ConstantUtf8Info* fieldNameConstant,
                                                     const DescriptorField& fieldDescriptor)
{
    ConstantNameAndType* nameAndTypeConstant = getOrCreateNameAndTypeConstant(fieldNameConstant, fieldDescriptor);
    return getOrCreateFieldrefConstant(classConstant, nameAndTypeConstant);
}

ConstantFieldref* Class::getOrCreateFieldrefConstant(ConstantClass* classConstant, ConstantUtf8Info* fieldNameConstant,
                                                     ConstantUtf8Info* fieldDescriptorConstant)
{
    ConstantNameAndType* nameAndTypeConstant = getOrCreateNameAndTypeConstant(
        fieldNameConstant, fieldDescriptorConstant);
    return getOrCreateFieldrefConstant(classConstant, nameAndTypeConstant);
}

ConstantFieldref* Class::getOrCreateFieldrefConstant(ConstantClass* classConstant,
                                                     ConstantNameAndType* nameAndTypeConstant)
{
    assert(this == classConstant->getOwner());
    assert(this == nameAndTypeConstant -> getOwner());

    // search constant
    for (auto* constant : constants_)
    {
        if (constant->getTag() == Constant::CONSTANT_Fieldref)
        {
            // Use static method because only one tag can be associated with only one class type.
            auto* fieldrefConstant = static_cast<ConstantFieldref*>(constant);
            if (fieldrefConstant->getClass() == classConstant && fieldrefConstant->getNameAndType() ==
                nameAndTypeConstant)
            {
                return fieldrefConstant;
            }
        }
    }

    // create new
    auto* fieldrefConstant = new ConstantFieldref(classConstant, nameAndTypeConstant);
    addNewConstant(fieldrefConstant);
    return fieldrefConstant;
}


ConstantMethodref* Class::getOrCreateMethodrefConstant(const std::string& className, const std::string& methodName,
                                                       const DescriptorMethod& methodDescriptor)
{
    ConstantClass* classConstant = getOrCreateClassConstant(className);
    ConstantNameAndType* nameAndTypeConstant = getOrCreateNameAndTypeConstant(methodName, methodDescriptor);
    return getOrCreateMethodrefConstant(classConstant, nameAndTypeConstant);
}

ConstantMethodref* Class::getOrCreateMethodrefConstant(ConstantClass* classConstant, const std::string& methodName,
                                                       const DescriptorMethod& methodDescriptor)
{
    ConstantNameAndType* nameAndTypeConstant = getOrCreateNameAndTypeConstant(methodName, methodDescriptor);
    return getOrCreateMethodrefConstant(classConstant, nameAndTypeConstant);
}

ConstantMethodref* Class::getOrCreateMethodrefConstant(ConstantClass* classConstant,
                                                       ConstantUtf8Info* methodNameConstant,
                                                       const DescriptorMethod& methodDescriptor)
{
    ConstantNameAndType* nameAndTypeConstant = getOrCreateNameAndTypeConstant(methodNameConstant, methodDescriptor);
    return getOrCreateMethodrefConstant(classConstant, nameAndTypeConstant);
}

ConstantMethodref* Class::getOrCreateMethodrefConstant(ConstantClass* classConstant,
                                                       ConstantUtf8Info* methodNameConstant,
                                                       ConstantUtf8Info* methodDescriptorConstant)
{
    ConstantNameAndType* nameAndTypeConstant = getOrCreateNameAndTypeConstant(
        methodNameConstant, methodDescriptorConstant);
    return getOrCreateMethodrefConstant(classConstant, nameAndTypeConstant);
}


ConstantMethodref* Class::getOrCreateMethodrefConstant(ConstantClass* classConstant,
                                                       ConstantNameAndType* nameAndTypeConstant)
{
    assert(this == classConstant->getOwner());
    assert(this == nameAndTypeConstant->getOwner());

    // search constant
    for (auto* constant : constants_)
    {
        if (constant->getTag() == Constant::CONSTANT_Methodref)
        {
            // Use static method because only one tag can be associated with only one class type.
            auto* constantMethodref = static_cast<ConstantMethodref*>(constant);
            if (constantMethodref->getClass() == classConstant && constantMethodref->getNameAndType() ==
                nameAndTypeConstant)
            {
                return constantMethodref;
            }
        }
    }

    // create new
    auto* methodrefConstant = new ConstantMethodref(classConstant, nameAndTypeConstant);
    addNewConstant(methodrefConstant);
    return methodrefConstant;
}

ConstantInterfaceMethodref* Class::getOrCreateInterfaceMethodrefConstant(const std::string& className,
                                                                         const std::string& methodName,
                                                                         const DescriptorMethod& methodDescriptor)
{
    ConstantClass* classConstant = getOrCreateClassConstant(className);
    ConstantNameAndType* nameAndTypeConstant = getOrCreateNameAndTypeConstant(methodName, methodDescriptor);
    return getOrCreateInterfaceMethodrefConstant(classConstant, nameAndTypeConstant);
}

ConstantInterfaceMethodref* Class::getOrCreateInterfaceMethodrefConstant(ConstantClass* classConstant,
                                                                         const std::string& methodName,
                                                                         const DescriptorMethod& methodDescriptor)
{
    ConstantNameAndType* nameAndTypeConstant = getOrCreateNameAndTypeConstant(methodName, methodDescriptor);
    return getOrCreateInterfaceMethodrefConstant(classConstant, nameAndTypeConstant);
}

ConstantInterfaceMethodref* Class::getOrCreateInterfaceMethodrefConstant(ConstantClass* classConstant,
                                                                         ConstantUtf8Info* methodNameConstant,
                                                                         const DescriptorMethod& methodDescriptor)
{
    ConstantNameAndType* nameAndTypeConstant = getOrCreateNameAndTypeConstant(methodNameConstant, methodDescriptor);
    return getOrCreateInterfaceMethodrefConstant(classConstant, nameAndTypeConstant);
}

ConstantInterfaceMethodref* Class::getOrCreateInterfaceMethodrefConstant(ConstantClass* classConstant,
                                                                         ConstantUtf8Info* methodNameConstant,
                                                                         ConstantUtf8Info* methodDescriptorConstant)
{
    ConstantNameAndType* nameAndTypeConstant = getOrCreateNameAndTypeConstant(
        methodNameConstant, methodDescriptorConstant);
    return getOrCreateInterfaceMethodrefConstant(classConstant, nameAndTypeConstant);
}

ConstantInterfaceMethodref* Class::getOrCreateInterfaceMethodrefConstant(ConstantClass* classConstant,
                                                                         ConstantNameAndType* nameAndTypeConstant)
{
    assert(this == classConstant->getOwner());
    assert(this == nameAndTypeConstant->getOwner());

    // search constant
    for (auto* constant : constants_)
    {
        if (constant->getTag() == Constant::CONSTANT_InterfaceMethodref)
        {
            // Use static method because only one tag can be associated with only one class type.
            auto* interfaceMethodrefConstant = static_cast<ConstantInterfaceMethodref*>(constant);
            if (interfaceMethodrefConstant->getClass() == classConstant && interfaceMethodrefConstant->getNameAndType()
                ==
                nameAndTypeConstant)
            {
                return interfaceMethodrefConstant;
            }
        }
    }

    // create new
    auto* interfaceMethodrefConstant = new ConstantInterfaceMethodref(classConstant, nameAndTypeConstant);
    addNewConstant(interfaceMethodrefConstant);
    return interfaceMethodrefConstant;
}

ConstantString* Class::getOrCreateStringConstant(const std::string& value)
{
    ConstantUtf8Info* stringConstant = getOrCreateUtf8Constant(value);
    return getOrCreateStringConstant(stringConstant);
}

ConstantString* Class::getOrCreateStringConstant(ConstantUtf8Info* utf8Constant)
{
    assert(this == utf8Constant->getOwner());

    // search constant
    for (auto* constant : constants_)
    {
        if (constant->getTag() == Constant::CONSTANT_String)
        {
            // Use static method because only one tag can be associated with only one class type.
            auto* stringConstant = static_cast<ConstantString*>(constant);
            if (stringConstant->getString() == utf8Constant)
            {
                return stringConstant;
            }
        }
    }

    // create new
    auto* stringConstant = new ConstantString(utf8Constant);
    addNewConstant(stringConstant);
    return stringConstant;
}

ConstantInteger* Class::getOrCreateIntegerConstant(int32_t value)
{
    // search constant
    for (auto* constant : constants_)
    {
        if (constant->getTag() == Constant::CONSTANT_Integer)
        {
            // Use static method because only one tag can be associated with only one class type.
            auto* integerConstant = static_cast<ConstantInteger*>(constant);
            if (integerConstant->getValue() == value)
            {
                return integerConstant;
            }
        }
    }

    // create new
    auto* integerConstant = new ConstantInteger(value, this);
    addNewConstant(integerConstant);
    return integerConstant;
}

ConstantFloat* Class::getOrCreateFloatConstant(float value)
{
    // search constant
    for (auto* constant : constants_)
    {
        if (constant->getTag() == Constant::CONSTANT_Float)
        {
            // Use static method because only one tag can be associated with only one class type.
            auto* floatConstant = static_cast<ConstantFloat*>(constant);
            auto valueFromConstant = floatConstant->getValue();
            if (std::memcmp(&valueFromConstant, &value, sizeof(float)) == 0)
            {
                return floatConstant;
            }
        }
    }

    // create new
    auto* floatConstant = new ConstantFloat(value, this);
    addNewConstant(floatConstant);
    return floatConstant;
}

ConstantLong* Class::getOrCreateLongConstant(int64_t value)
{
    // search constant
    for (auto* constant : constants_)
    {
        if (constant->getTag() == Constant::CONSTANT_Long)
        {
            // Use static method because only one tag can be associated with only one class type.
            auto* longConstant = static_cast<ConstantLong*>(constant);
            if (longConstant->getValue() == value)
            {
                return longConstant;
            }
        }
    }

    // create new
    auto* longConstant = new ConstantLong(value, this);
    addNewConstant(longConstant);
    return longConstant;
}

ConstantDouble* Class::getOrCreateDoubleConstant(double value)
{
    // search constant
    for (auto* constant : constants_)
    {
        if (constant->getTag() == Constant::CONSTANT_Double)
        {
            // Use static method because only one tag can be associated with only one class type.
            auto* doubleConstant = static_cast<ConstantDouble*>(constant);
            auto valueFromConstant = doubleConstant->getValue();
            if (std::memcmp(&valueFromConstant, &value, sizeof(double)) == 0)
            {
                return doubleConstant;
            }
        }
    }

    // create new
    auto doubleConstant = new ConstantDouble(value, this);
    addNewConstant(doubleConstant);
    return doubleConstant;
}

ConstantNameAndType* Class::getOrCreateNameAndTypeConstant(const std::string& name,
                                                           ConstantUtf8Info* descriptorConstant)
{
    assert(this == descriptorConstant->getOwner());

    ConstantUtf8Info* nameConstant = getOrCreateUtf8Constant(name);
    return getOrCreateNameAndTypeConstant(nameConstant, descriptorConstant);
}

ConstantNameAndType* Class::getOrCreateNameAndTypeConstant(const std::string& name, const Descriptor& descriptor)
{
    ConstantUtf8Info* nameConstant = getOrCreateUtf8Constant(name);
    ConstantUtf8Info* descriptorConstant = getOrCreateUtf8Constant(descriptor.toString());
    return getOrCreateNameAndTypeConstant(nameConstant, descriptorConstant);
}

ConstantNameAndType* Class::getOrCreateNameAndTypeConstant(ConstantUtf8Info* nameConstant, const Descriptor& descriptor)
{
    assert(this == nameConstant->getOwner());

    ConstantUtf8Info* descriptorConstant = getOrCreateUtf8Constant(descriptor.toString());
    return getOrCreateNameAndTypeConstant(nameConstant, descriptorConstant);
}

ConstantNameAndType* Class::getOrCreateNameAndTypeConstant(ConstantUtf8Info* nameConstant,
                                                           ConstantUtf8Info* descriptorConstant)
{
    assert(this == nameConstant->getOwner());
    assert(this == descriptorConstant->getOwner());

    // search constant
    for (auto* constant : constants_)
    {
        if (constant->getTag() == Constant::CONSTANT_NameAndType)
        {
            // Use static method because only one tag can be associated with only one class type.
            auto* nameAndTypeConstant = static_cast<ConstantNameAndType*>(constant);
            if (nameAndTypeConstant->getName() == nameConstant &&
                nameAndTypeConstant->getDescriptor() == descriptorConstant)
            {
                return nameAndTypeConstant;
            }
        }
    }

    // create new
    auto* nameAndTypeConstant = new ConstantNameAndType(nameConstant, descriptorConstant);
    addNewConstant(nameAndTypeConstant);
    return nameAndTypeConstant;
}

ConstantUtf8Info* Class::getOrCreateUtf8Constant(const std::string& value)
{
    // search constant
    for (auto* constant : constants_)
    {
        if (constant->getTag() == Constant::CONSTANT_Utf8)
        {
            // Use static method because only one tag can be associated with only one class type.
            auto* utf8Constant = static_cast<ConstantUtf8Info*>(constant);
            if (utf8Constant->getString() == value)
            {
                return utf8Constant;
            }
        }
    }

    // create new
    auto* utf8Constant = new ConstantUtf8Info(value, this);
    addNewConstant(utf8Constant);
    return utf8Constant;
}

Field* Class::getOrCreateField(const std::string& name, const DescriptorField& descriptor)
{
    auto* nameConstant = getOrCreateUtf8Constant(name);
    auto* descriptorConstant = getOrCreateUtf8Constant(descriptor.toString());
    return getOrCreateField(nameConstant, descriptorConstant);
}

Field* Class::getOrCreateField(const std::string& name, ConstantUtf8Info* descriptorConstant)
{
    auto* nameConstant = getOrCreateUtf8Constant(name);
    return getOrCreateField(nameConstant, descriptorConstant);
}

Field* Class::getOrCreateField(ConstantUtf8Info* nameConstant, const DescriptorField& descriptor)
{
    auto* descriptorConstant = getOrCreateUtf8Constant(descriptor.toString());
    return getOrCreateField(nameConstant, descriptorConstant);
}

Field* Class::getOrCreateField(ConstantUtf8Info* nameConstant, ConstantUtf8Info* descriptorConstant)
{
    assert(this == nameConstant->getOwner());
    assert(this == descriptorConstant->getOwner());

    // search field
    for (auto* field : fields_)
    {
        if (field->getName() == nameConstant && field->getDescriptor() == descriptorConstant)
        {
            return field;
        }
    }

    // create new
    auto* field = new Field(nameConstant, descriptorConstant);
    fields_.insert(field);
    return field;
}

Method* Class::getOrCreateMethod(const std::string& name, const DescriptorMethod& descriptor)
{
    auto* nameConstant = getOrCreateUtf8Constant(name);
    auto* descriptorConstant = getOrCreateUtf8Constant(descriptor.toString());
    return getOrCreateMethod(nameConstant, descriptorConstant);
}

Method* Class::getOrCreateMethod(const std::string& name, ConstantUtf8Info* descriptorConstant)
{
    auto* nameConstant = getOrCreateUtf8Constant(name);
    return getOrCreateMethod(nameConstant, descriptorConstant);
}

Method* Class::getOrCreateMethod(ConstantUtf8Info* nameConstant, const DescriptorMethod& descriptor)
{
    auto* descriptorConstant = getOrCreateUtf8Constant(descriptor.toString());
    return getOrCreateMethod(nameConstant, descriptorConstant);
}

Method* Class::getOrCreateMethod(ConstantUtf8Info* nameConstant, ConstantUtf8Info* descriptorConstant)
{
    assert(this == nameConstant->getOwner());
    assert(this == descriptorConstant->getOwner());

    // search method
    for (auto* method : methods_)
    {
        if (method->getName() == nameConstant && method->getDescriptor() == descriptorConstant)
        {
            return method;
        }
    }

    // create new
    auto* method = new Method(nameConstant, descriptorConstant);
    methods_.insert(method);
    return method;
}

std::span<Constant*> Class::constants()
{
    return constants_;
}

void Class::addFlag(AccessFlag flag)
{
    uint16_t newFlags = 0;
    for (auto f : accessFlags_)
        newFlags |= f;

    newFlags |= flag;

    validateFlags(newFlags);
    accessFlags_.insert(flag);
}

void Class::removeFlag(AccessFlag flag)
{
    accessFlags_.erase(flag);
}

void Class::writeTo(std::ostream& os) const
{
    std::ostringstream buffer;

    // u4             magic;
    static uint32_t magicNumber = 0xCAFEBABE;
    internal::Utils::writeBigEndian(buffer, magicNumber);

    // u2             minor_version;
    internal::Utils::writeBigEndian(buffer, minorVersion);

    // u2             major_version;
    internal::Utils::writeBigEndian(buffer, static_cast<uint16_t>(majorVersion));

    // u2             constant_pool_count;
    uint16_t constantCount = static_cast<uint16_t>(nextCpIndex);
    internal::Utils::writeBigEndian(buffer, constantCount);

    // cp_info        constant_pool[constant_pool_count-1];
    for (const auto& constant : constants_)
    {
        buffer << *constant;
    }

    // u2             access_flags;
    uint16_t accessFlags = 0x0000;
    for (auto flag : accessFlags_)
    {
        accessFlags = accessFlags | flag;
    }
    internal::Utils::writeBigEndian(buffer, accessFlags);

    // u2             this_class;
    uint16_t thisClass = thisClassConstant_->getIndex();
    internal::Utils::writeBigEndian(buffer, thisClass);

    // u2             super_class;
    uint16_t superClass = superClassConstant_->getIndex();
    internal::Utils::writeBigEndian(buffer, superClass);

    // u2             interfaces_count;
    uint16_t interfacesCount = static_cast<uint16_t>(interfacesConstant_.size());
    internal::Utils::writeBigEndian(buffer, interfacesCount);

    // u2             interfaces[interfaces_count];
    for (const auto& interface : interfacesConstant_)
    {
        uint16_t interfaceIndex = interface->getIndex();
        internal::Utils::writeBigEndian(buffer, interfaceIndex);
    }

    // u2             fields_count;
    uint16_t fieldsCount = static_cast<uint16_t>(fields_.size());
    internal::Utils::writeBigEndian(buffer, fieldsCount);

    // field_info     fields[fields_count];
    for (const auto& field : fields_)
    {
        buffer << *field;
    }

    // u2             methods_count;
    uint16_t methodsCount = static_cast<uint16_t>(methods_.size());
    internal::Utils::writeBigEndian(buffer, methodsCount);

    // method_info    methods[methods_count];
    for (const auto& method : methods_)
    {
        buffer << *method;
    }

    // u2             attributes_count;
    uint16_t attributesCount = static_cast<uint16_t>(attributes_.size());
    internal::Utils::writeBigEndian(buffer, attributesCount);

    // attribute_info attributes[attributes_count];
    for (const auto& attribute : attributes_)
    {
        buffer << *attribute;
    }

    // fix data and write to stream
    auto str = buffer.str();
    fixClassBinary(os, std::vector<unsigned char>{str.begin(), str.end()});
}

std::size_t Class::getByteSize() const
{
    size_t size = 0;

    // u4 magic;
    size += 4;
    // u2 minor_version;
    size += 2;
    // u2 major_version;
    size += 2;
    // u2 constant_pool_count;
    size += 2;
    // cp_info constant_pool[constant_pool_count-1];
    for (const auto& constant : constants_)
    {
        size += constant->getByteSize();
    }
    // u2 access_flags;
    size += 2;
    // u2 this_class;
    size += 2;
    // u2 super_class;
    size += 2;
    // u2 interfaces_count;
    size += 2;
    // u2 interfaces[interfaces_count];
    size += 2;
    // u2 fields_count;
    size += 2;
    // field_info fields[fields_count];
    for (auto* field : fields_)
    {
        size += field->getByteSize();
    }
    // u2 methods_count;
    size += 2;
    // method_info methods[methods_count];
    for (auto* method : methods_)
    {
        size += method->getByteSize();
    }
    // u2 attributes_count;
    size += 2;
    // attribute_info attributes[attributes_count];
    for (auto* attribute : attributes_)
    {
        size += attribute->getByteSize();
    }

    return size;
}

const std::set<Class::AccessFlag>* Class::getAccessFlags() const
{
    return &accessFlags_;
}

void Class::addNewConstant(Constant* constant)
{
    constants_.push_back(constant);
    constant->setIndex(nextCpIndex);

    nextCpIndex += constant->getOccupiedSlots();
}

void Class::validateFlags(uint16_t flags)
{
    using internal::Utils;

    // Validate public with private/protected
    // Для class допустим только PUBLIC или package-private
    // (PRIVATE / PROTECTED не существуют для top-level class)
    if (Utils::hasFlag(flags, 0x0002 /* private */) ||
        Utils::hasFlag(flags, 0x0004 /* protected */))
    {
        throw std::logic_error("Class cannot be private or protected");
    }

    // Validate abstract with final
    if (Utils::hasFlag(flags, ACC_ABSTRACT) &&
        Utils::hasFlag(flags, ACC_FINAL))
    {
        throw std::logic_error("Class cannot be both abstract and final");
    }

    // Validate interface with abstract and final and enum
    if (Utils::hasFlag(flags, ACC_INTERFACE))
    {
        if (!Utils::hasFlag(flags, ACC_ABSTRACT))
        {
            throw std::logic_error("Interface class must be abstract");
        }

        if (Utils::hasFlag(flags, ACC_FINAL))
        {
            throw std::logic_error("Interface cannot be final");
        }

        if (Utils::hasFlag(flags, ACC_ENUM))
        {
            throw std::logic_error("Interface cannot be enum");
        }
    }

    // Validate annotation with interface
    if (Utils::hasFlag(flags, ACC_ANNOTATION))
    {
        if (!Utils::hasFlag(flags, ACC_INTERFACE))
        {
            throw std::logic_error("Annotation must also be an interface");
        }
    }

    // Validate enum with interface and module
    if (Utils::hasFlag(flags, ACC_ENUM))
    {
        if (Utils::hasFlag(flags, ACC_INTERFACE))
        {
            throw std::logic_error("Enum cannot be interface");
        }

        if (Utils::hasFlag(flags, ACC_MODULE))
        {
            throw std::logic_error("Enum cannot be module");
        }
    }

    // Validate module with synthetic
    if (Utils::hasFlag(flags, ACC_MODULE))
    {
        constexpr uint16_t allowed =
            ACC_MODULE | ACC_SYNTHETIC;

        if ((flags & ~allowed) != 0)
        {
            throw std::logic_error("Module class cannot have class-related flags");
        }
    }
}

void Class::fixClassBinary(std::ostream& os, const std::span<const unsigned char>& data)
{
#ifdef _WIN32
    fs::path jarPath = JAVA_INTERNAL_JAR;
    auto pathToTempFile = std::filesystem::temp_directory_path() / "jvm_class_builder_temp_class.class";
    try
    {
        // write input
        {
            std::ofstream inputDataFile(pathToTempFile, std::ios::binary);
            if (!inputDataFile) throw std::runtime_error("can't open input.class");
            inputDataFile.write(reinterpret_cast<const char*>(data.data()),
                                static_cast<std::streamsize>(data.size()));
            inputDataFile.close();
        }

        // java.exe
        const char* jh = std::getenv("JAVA_HOME");
        if (!jh) throw std::runtime_error("JAVA_HOME not set");

        fs::path javaExe = fs::path(jh) / "bin" / "java.exe";

        std::string cmd =
            javaExe.string() + " -jar " +
            jarPath.string() + " " +
            pathToTempFile.string() + " " +
            pathToTempFile.string();


        int code = std::system(cmd.c_str());
        if (code != 0)
            throw std::runtime_error("java -jar failed");

        // read result
        {
            std::ifstream in(pathToTempFile, std::ios::binary);
            if (!in) throw std::runtime_error("can't open finalize.class");
            os << in.rdbuf();
        }
    }
    catch (...)
    {
        // delete temp file
        std::filesystem::remove(pathToTempFile);
        throw;
    }
    //delete temp file
    std::filesystem::remove(pathToTempFile);
#else

    JavaVM* jvm = nullptr;
    JNIEnv* env = nullptr;

    std::string classpath = std::string("-Djava.class.path=") + JAVA_INTERNAL_JAR;
    JavaVMOption options[1];
    options[0].optionString = classpath.data();
    JavaVMInitArgs vm_args{};
    vm_args.version = JNI_VERSION_1_8;
    vm_args.nOptions = 1;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = JNI_FALSE;

    // run jvm
    jint correctJvmCreation = JNI_CreateJavaVM(&jvm, reinterpret_cast<void**>(&env), &vm_args);
    if (correctJvmCreation != JNI_OK || !env)
    {
        throw std::runtime_error("Failed to create JVM");
    }

    try
    {
        // find class
        jclass fixClass = env->FindClass("compilator/fix/FixClass");
        if (!fixClass)
        {
            throw std::logic_error("FixClass not found");
        }

        // find static method
        jmethodID fixMethod = env->GetStaticMethodID(fixClass, "fix", "([B)[B");
        if (!fixMethod)
        {
            throw std::logic_error("FixClass.fix(byte[]) not found");
        }

        // convert c++ byte array to jvm byte array
        jbyteArray inputArray = env->NewByteArray(static_cast<jsize>(data.size()));
        env->SetByteArrayRegion(
            inputArray, 0,
            static_cast<jsize>(data.size()),
            reinterpret_cast<const jbyte*>(data.data()));

        // call fix method (method provide jByteArray)
        auto resultArray = static_cast<jbyteArray>(env->CallStaticObjectMethod(fixClass, fixMethod, inputArray));

        if (env->ExceptionCheck())
        {
            env->ExceptionDescribe();
            env->ExceptionClear();
            throw std::runtime_error("Java exception in FixClass.fix()");
        }

        // convert jvm byte array to c++ byte array
        jsize resultSize = env->GetArrayLength(resultArray);
        std::vector<unsigned char> result(resultSize);
        env->GetByteArrayRegion(
            resultArray, 0, resultSize,
            reinterpret_cast<jbyte*>(result.data()));

        // write data to stream
        os.write(reinterpret_cast<const char*>(result.data()), static_cast<std::streamsize>(result.size()));

        // clear refs
        env->DeleteLocalRef(inputArray);
        env->DeleteLocalRef(resultArray);
        env->DeleteLocalRef(fixClass);
    }
    catch (...)
    {
        // destroy jvm
        jvm->DestroyJavaVM();
        throw;
    }

    // destroy jvm
    jvm->DestroyJavaVM();
#endif
}
