class A
{
    int x;

    this(int v)
    {
        x = v;
    }

    int f()
    {
        return x + 1;
    }
}

class B : A
{
    this(int v)
    {
        x = v;
    }

    override int f()
    {
        return super.f() + 1;
    }
}

void main()
{
    A a = new B(5);
    writeln(a.f());
}

