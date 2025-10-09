import std.stdio;

void main() {
    // While loop
    writeln("While loop:");
    int i = 0;
    while (i < 3) {
        writeln("i = ", i);
        i++;
    }
    
    // Do-while loop
    writeln("Do-while loop:");
    int j = 0;
    do {
        writeln("j = ", j);
        j++;
    } while (j < 3);
    
    // For loop
    writeln("For loop:");
    for (int k = 0; k < 3; k++) {
        writeln("k = ", k);
    }
    
    // Foreach loop
    writeln("Foreach loop:");
    int[] arr = [10, 20, 30];
    foreach (element; arr) {
        writeln("Element: ", element);
    }
    
    // Break and continue
    writeln("Break and continue:");
    for (int m = 0; m < 5; m++) {
        if (m == 2) continue;
        if (m == 4) break;
        writeln("m = ", m);
    }
}