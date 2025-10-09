import std.stdio;

void main() {
    bool a = true;
    bool b = false;
    int x = 5;
    int y = 0;
    
    // Logical AND
    bool andResult1 = a && b;     // false
    bool andResult2 = a && true;  // true
    bool andResult3 = (x > 0) && (y == 0); // true
    
    // Logical OR
    bool orResult1 = a || b;      // true
    bool orResult2 = false || b;  // false
    bool orResult3 = (x == 0) || (y == 0); // true
    
    // Logical NOT
    bool notResult1 = !a;         // false
    bool notResult2 = !b;         // true
    bool notResult3 = !(x == 0);  // true
    
    // Combined operations
    bool combined = (a && !b) || (x > 0);
    
    writeln("a && b: ", andResult1);
    writeln("a && true: ", andResult2);
    writeln("(x>0) && (y==0): ", andResult3);
    writeln("a || b: ", orResult1);
    writeln("false || b: ", orResult2);
    writeln("(x==0) || (y==0): ", orResult3);
    writeln("!a: ", notResult1);
    writeln("!b: ", notResult2);
    writeln("!(x==0): ", notResult3);
    writeln("Combined: ", combined);
}