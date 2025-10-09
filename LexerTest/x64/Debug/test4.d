import std.stdio;

void main() {
    int a = 10;
    int b = 5;
    
    // Simple if
    if (a > 5) {
        writeln("a is greater than 5");
    }
    
    // if-else
    if (a > b) {
        writeln("a is greater than b");
    } else {
        writeln("a is not greater than b");
    }
    
    // else-if ladder
    if (a > 15) {
        writeln("a > 15");
    } else if (a > 10) {
        writeln("a > 10");
    } else if (a > 5) {
        writeln("a > 5");
    } else {
        writeln("a <= 5");
    }
    
    // Switch statement
    int x = 2;
    switch (x) {
        case 1:
            writeln("x = 1");
            break;
        case 2:
            writeln("x = 2");
            break;
        default:
            writeln("x is not 1 or 2");
            break;
    }
}