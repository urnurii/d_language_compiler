import std.stdio;

class Animal {
    string name;
    int age;
    
    void speak() {
        writeln(name, " makes a sound");
    }
}

class Dog : Animal {
    override void speak() {
        writeln(name, " barks");
    }
    
    void callSuper() {
        super.speak();
    }
}

void main() {

    auto animal = new Animal();
    animal.name = "Generic Animal";
    animal.age = 3;
    
    auto dog = new Dog();
    dog.name = "Buddy";
    dog.age = 5;
    
    animal.speak();
    dog.speak();
    dog.callSuper();

    writeln("Animal age: ", animal.age);
    writeln("Dog name: ", dog.name);
}