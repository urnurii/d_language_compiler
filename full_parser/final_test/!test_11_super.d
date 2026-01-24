module full_parser.final_test.!test_11_super;

class Base
{
    int value;

    this(int v)
    {
        init(v);
    }

    void init(int v)
    {
        value = v;
        writeln("Base.init");
    }

    string kind()
    {
        return "Base";
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
        writeln("Derived.ctor");
    }

    override string kind()
    {
        return "Derived";
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
    Base b = new Base(5);
    Derived d = new Derived(5);

    Base[] arr = [b, d];
    int i = 0;
    while (i < arr.length)
    {
        writeln(arr[i].kind());
        writeln(arr[i].calc());
        i = i + 1;
        }
    }