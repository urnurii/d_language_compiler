import std.stdio;

void main() {
    // Output functions
    write("This is write - ");
    writeln("This is writeln");
    
    writef("Formatted output: %s %d\n", "test", 42);
    
    // Simple input simulation (commented for testing)
    /*
    writeln("Enter your name:");
    string name = readln().strip();
    writeln("Hello, ", name);
    
    writeln("Enter a number:");
    int number;
    readf!" %d"(number);
    writeln("You entered: ", number);
    */
    
    // Using stderr
    stderr.writeln("This is an error message");
}