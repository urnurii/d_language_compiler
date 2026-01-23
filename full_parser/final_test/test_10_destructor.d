class MyClass {
    int number = 10;

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
    //destroy(obj);

    //writef("number obj: %d", obj.number);
}

void main() {
    myFunction();
    writeln("Функция myFunction заканчивается.");
}