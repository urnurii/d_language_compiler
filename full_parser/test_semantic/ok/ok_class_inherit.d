class Base { public int f(int a) { return a; } }
class Derived : Base { override int f(int a) { return super.f(a); } }
void g() { Derived d; d.f(1); }
