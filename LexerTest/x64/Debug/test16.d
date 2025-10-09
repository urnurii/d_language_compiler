import std.stdio;
import std.format;

void main() {
    int age = 25;
    string name = "Daniil";
    double salary = 1234.56;
    char grade = 'A';
    
    string message1 = format("Name: %s, Age: %d", name, age);
    string message2 = format("Salary: %.2f, Grade: %c", salary, grade);
    
    string details = format("Employee: %s, Age: %d, Salary: $%.2f, Grade: %c", 
                           name, age, salary, grade);
    
    string intFormat = format("Decimal: %d", 255);
    string floatFormat = format("Float: %f", 123.456);
    
    writeln(message1);
    writeln(message2);
    writeln(details);
    writeln(intFormat);
    writeln(floatFormat);

    writefln("Formatted output: %s is %d years old", name, age);
    writefln("Floating point: %.3f", 3.14159);
}