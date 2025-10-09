import std.stdio;

class MyClass {
    int value;
    string name;
    
    this() {
        this.value = 0;
        this.name = "default";
        writeln("Default constructor called");
    }

    this(int v, string n) {
        this.value = v;
        this.name = n;
        writeln("Parameterized constructor called with ", v, ", ", n);
    }
    
    ~this() {
        writeln("Destructor called for ", name);
    }
    
    void display() {
        writeln("Value: ", value, ", Name: ", name);
    }
}

void main() {
    writeln("Creating object with default constructor:");
    auto obj1 = new MyClass();
    obj1.display();
    
    writeln("Creating object with parameterized constructor:");
    auto obj2 = new MyClass(42, "test");
    obj2.display();
    
    writeln("End of main function - destructors will be called");
}