class Base { int f(int a) { return a; } }
class Derived : Base { override int f(int a, int b) { return a + b; } }
