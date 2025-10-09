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
    
}