import std.stdio;

// Overloaded functions
void print(int n) {
    writeln("Integer: ", n);
}

void print(string s) {
    writeln("String: ", s);
}

void print(double d) {
    writeln("Double: ", d);
}

void print(int a, int b) {
    writeln("Two integers: ", a, ", ", b);
}

void print(string s, int n) {
    writeln("String and integer: ", s, ", ", n);
}

class Printer {
    void print(int n) {
        writeln("Class method - Integer: ", n);
    }
    
    void print(string s) {
        writeln("Class method - String: ", s);
    }
}

void main() {
    // Function overloading
    print(10);
    print("Hello");
    print(3.14);
    print(5, 10);
    print("Number", 42);
    
    // Method overloading
    auto printer = new Printer();
    printer.print(100);
    printer.print("World");
}