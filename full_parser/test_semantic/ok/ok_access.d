class A { public int x; public int f() { return x; } }
void g() { A a; a.x = 1; a.f(); }
