class Base
{
    int value;

    this(int v)
    {
        value = v;
    }

    int get()
    {
        return value;
    }

    int calc()
    {
        return get() + 1;
    }
}

class Derived : Base
{
    this(int v)
    {
        super(v);
    }

    override int get()
    {
        return super.get() + 10;
    }

    override int calc()
    {
        return super.calc() + 100;
    }
}

void main()
{
    Base d = new Derived(5);
    writeln(d.calc()); // ожидается: 116
}