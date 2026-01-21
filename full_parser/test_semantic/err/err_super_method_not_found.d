class A { int f() { return 1; } }
class B : A { int g() { return super.g(); } }
