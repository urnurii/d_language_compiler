# Jvm Class Builder

C++ library for **programmatic generation of JVM `.class` files**

---

## Overview

This project implements a **binary Java class file generator** in C++.

It allows constructing valid JVM `.class` files by explicitly modeling:
- constant pool entries
- fields and methods
- bytecode instructions
- attribute code
- access flags and descriptors

The generated binaries can be:
- inspected with `javap -verbose`
- verified by the JVM
- executed using `java`

The project is designed as a **low-level, specification-oriented implementation**
of the *Java Virtual Machine Class File Format*.

---

# Features

- Constant pool management:
  - UTF8, Class, NameAndType
  - String, Integer, Float, Long, Double
  - Fieldref, Methodref, InterfaceMethodref
- JVM descriptors:
  - Field descriptor
  - Method descriptor
- Bytecode instructions builder:
  - Stack operations
  - Arithmetic instructions
  - Method invocation
  - Control flow primitives:
    - conditional branches (`if*`, `if_icmp*`)
    - unconditional jumps (`goto`)
    - labels and jump targets
    - loops (`for`, `while`)

---

# Example: Hello, World!

```cpp
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

```

---

# Generetad Bytecode

```bash
javap -verbose HelloWorld.class
```

---

# Output

```text
public class HelloWorld
  minor version: 0
  major version: 60
  flags: (0x0021) ACC_PUBLIC, ACC_SUPER
  this_class: #2                          // HelloWorld
  super_class: #4                         // java/lang/Object
  interfaces: 0, fields: 0, methods: 1, attributes: 0
Constant pool:
   #1 = Utf8               HelloWorld
   #2 = Class              #1             // HelloWorld
   #3 = Utf8               java/lang/Object
   #4 = Class              #3             // java/lang/Object
   #5 = Utf8               main
   #6 = Utf8               ([Ljava/lang/String;)V
   #7 = Utf8               java/lang/System
   #8 = Class              #7             // java/lang/System
   #9 = Utf8               out
  #10 = Utf8               Ljava/io/PrintStream;
  #11 = NameAndType        #9:#10         // out:Ljava/io/PrintStream;
  #12 = Fieldref           #8.#11         // java/lang/System.out:Ljava/io/PrintStream;
  #13 = Utf8               java/io/PrintStream
  #14 = Class              #13            // java/io/PrintStream
  #15 = Utf8               println
  #16 = Utf8               (Ljava/lang/String;)V
  #17 = NameAndType        #15:#16        // println:(Ljava/lang/String;)V
  #18 = Methodref          #14.#17        // java/io/PrintStream.println:(Ljava/lang/String;)V
  #19 = Utf8               Hello, world!
  #20 = String             #19            // Hello, world!
  #21 = Utf8               Code
{
  public static void main(java.lang.String[]);
    descriptor: ([Ljava/lang/String;)V
    flags: (0x0009) ACC_PUBLIC, ACC_STATIC
    Code:
      stack=0, locals=0, args_size=1
         0: getstatic     #12                 // Field java/lang/System.out:Ljava/io/PrintStream;
         3: ldc           #20                 // String Hello, world!
         5: invokevirtual #18                 // Method java/io/PrintStream.println:(Ljava/lang/String;)V
         8: return
}
```

---

 # Dependencies

- Maven
- Java 17+

---

# Developers

 - **Ilya Kolomoytsev**
 - **Mikhail Bykadorov**

---
