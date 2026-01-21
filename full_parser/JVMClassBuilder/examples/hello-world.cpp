#include <fstream>
#include <jvm/class.h>
#include <jvm/method.h>
#include <jvm/serializable.h>
#include <jvm/instruction-ldc.h>

#include <jvm/constant-fieldref.h>
#include <jvm/constant-methodref.h>
#include <jvm/constant-string.h>

#include "jvm/descriptor-method.h"

using namespace jvm;

int main()
{
    Class helloWorldClass("HelloWorld", "java/lang/Object");

    Method* mainMethod = helloWorldClass.getOrCreateMethod(
        "main",
        DescriptorMethod{
            std::nullopt,
            {{"java/lang/String", 1}} // String[] args
        }
    );

    helloWorldClass.addFlag(Class::ACC_SUPER);
    helloWorldClass.addFlag(Class::ACC_PUBLIC);

    mainMethod->addFlag(Method::ACC_PUBLIC);
    mainMethod->addFlag(Method::ACC_STATIC);

    // --- constants for System.out.println("Hello, world!") ---
    // Field: java/lang/System.out : Ljava/io/PrintStream;
    auto descriptor_field_out = DescriptorField("java/io/PrintStream");
    ConstantFieldref* systemOut = helloWorldClass.getOrCreateFieldrefConstant(
        "java/lang/System",
        "out",
        descriptor_field_out
    );

    auto descriptor_method_println = DescriptorMethod(
        std::nullopt,
        {{"java/lang/String"}}
    );

    // Method: java/io/PrintStream.println : (Ljava/lang/String;)V
    ConstantMethodref* println = helloWorldClass.getOrCreateMethodrefConstant(
        "java/io/PrintStream",
        "println",
        descriptor_method_println
    );

    AttributeCode* code = mainMethod->getCodeAttribute();
    *code
        << code->GetStatic(systemOut)
        << code->PushString("Hello, World!")
        << code->InvokeVirtual(println)
        << code->ReturnVoid();

    std::ofstream file("HelloWorld.class", std::ios::binary);
    helloWorldClass.writeTo(file);
    file.close();
}
