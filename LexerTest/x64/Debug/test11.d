import std.stdio;

void main() {
    bool a = true;
    bool b = false;
    int x = 5;
    int y = 0;
    
    bool andResult1 = a && b;
    bool andResult2 = a && true;
    bool andResult3 = (x > 0) && (y == 0);

    bool orResult1 = a || b;
    bool orResult2 = false || b;
    bool orResult3 = (x == 0) || (y == 0);
    
    bool notResult1 = !a;
    bool notResult2 = !b;
    bool notResult3 = !(x == 0);
    
    bool combined = (a && !b) || (x > 0);
}