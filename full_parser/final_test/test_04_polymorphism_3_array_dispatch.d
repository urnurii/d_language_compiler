class Base
{
    string kind() { return "Base"; }
    int calc() { return 10; }
}

class Derived : Base
{
    override string kind() { return "Derived"; }
    override int calc() { return 20; }
}

class Derived2 : Base
{
    override string kind() { return "Derived2"; }
    override int calc() { return 30; }
}

void main()
{
    Base[] arr = [new Base(), new Derived(), new Derived2()];

    int i = 0;
    while (i < arr.length)
    {
        writeln(arr[i].kind());
        writeln(arr[i].calc());
        i = i + 1;
    }

    // ожидается вывод:
    // Base
    // 10
    // Derived
    // 20
    // Derived2
    // 30
}