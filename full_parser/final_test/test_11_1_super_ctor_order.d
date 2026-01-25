class Base
{
    this(int v)
    {
        writeln("Base.ctor");
    }
}

class Derived : Base
{
    this(int v)
    {
        super(v);
        writeln("Derived.ctor");
    }
}

void main()
{
    Derived d = new Derived(1);

    // ожидается:
    // Base.ctor
    // Derived.ctor
}