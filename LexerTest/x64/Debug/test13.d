import std.stdio;

class MyClass {
    int value;
    string name;
    
    this() {
        this.value = 0;
        this.name = "default";
    }

    this(int v, string n) {
        this.value = v;
        this.name = n;

    }
    
    ~this() {

    }
    
    void display() {
    }
}
