class MyClass {
    this() {
        writeln("MyClass объект создан.");
    }

    ~this() {
        writeln("Деструктор MyClass вызван.");
    }
}

void myFunction() {
    writeln("Функция myFunction начинается.");
    
    MyClass obj = new MyClass();
}

void main() {
    myFunction();
    writeln("Функция myFunction заканчивается.");
}