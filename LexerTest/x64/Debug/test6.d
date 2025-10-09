import std.stdio;

// Function with return value
int add(int a, int b) {
    return a + b;
}

// Procedure (void function)
void printMessage(string msg) {
    writeln("Message: ", msg);
}

// Function with reference parameter
void increment(ref int x) {
    x++;
}

// Function with default argument
void greet(string name = "Guest") {
    writeln("Hello, ", name);
}

// Variable arguments
void printNumbers(int first, int... rest) {
    writeln("First: ", first);
    foreach (num; rest) {
        writeln("Rest: ", num);
    }
}

// Local function
void outer() {
    int a = 5;
    void inner() {
        writeln("Inner accessing outer a = ", a);
    }
    inner();
}

// Recursive function
int factorial(int n) {
    if (n == 0) return 1;
    return n * factorial(n - 1);
}

void main() {
    // Function calls
    int result = add(5, 3);
    writeln("5 + 3 = ", result);
    
    printMessage("Testing functions");
    
    int x = 10;
    increment(x);
    writeln("After increment: ", x);
    
    greet();
    greet("Daniil");
    
    printNumbers(1, 2, 3, 4);
    
    outer();
    
    writeln("Factorial of 5: ", factorial(5));
}