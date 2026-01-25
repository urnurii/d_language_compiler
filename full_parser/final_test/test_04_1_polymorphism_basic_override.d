class Base
{
    string kind() { return "Base"; }
    int calc() { return 1; }
}

class Derived : Base
{
    override string kind() { return "Derived"; }
    override int calc() { return 100; }
}

void main()
{
    Base b = new Base();
    Base d = new Derived();

    writeln(b.kind());
    writeln(b.calc());

    writeln(d.kind());
    writeln(d.calc());
}
