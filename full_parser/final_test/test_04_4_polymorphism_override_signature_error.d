class Base
{
    int f(int x) { return x; }
}

class Derived : Base
{
    // ошибка: сигнатура не совпадает
    override int f() { return 0; }
}

void main() {}