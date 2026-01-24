module full_parser.final_test.!test_07_by_value_vs_ref;

class Box
{
    int v;

    this(int v)
    {
        this.v = v;
    }
}

void printState(string tag, int i, char c, string s, float f, bool b, int[] a, Box o)
{
    writeln("== ", tag, " ==");

    writeln("i = ", i);
    writeln("c = ", c);
    writeln("s = ", s);
    writeln("f = ", f);
    writeln("b = ", b);

    write("a = [");
    for (int k = 0; k < a.length; k += 1)
    {
        if (k != 0) write(", ");
        write(a[k]);
    }
    writeln("]");

    writeln("o.v = ", o.v);
    writeln("");
}

void mutateByValue(int i, char c, string s, float f, bool b, int[] a, Box o)
{
    i = 111;
    c = 'Z';
    s = "changed_value";
    f = 2.5f;
    b = !b;

    if (a.length > 0)
        a[0] = 101;

    int[] a1 = [9, 9, 9];

    a = a1;

    o.v = 501;

    o = new Box(999);

    printState("Inside mutateByValue", i, c, s, f, b, a, o);
}

void mutateByRef(ref int i, ref char c, ref string s, ref float f, ref bool b, ref int[] a, ref Box o)
{
    i = 222;
    c = 'R';
    s = "changed_ref";
    f = 3.5f;
    b = !b;

    if (a.length > 1)
        a[1] = 202;

    int[] a1 = [7, 7, 7, 7];
	
    a = a1;

    o.v = 777;

    o = new Box(888);

    printState("Inside mutateByRef", i, c, s, f, b, a, o);
}

void main()
{
    int i = 1;
    char c = 'A';
    string s = "orig";
    float f = 1.5f;
    bool b = true;

    int[] a = [1, 2, 3];
    Box o = new Box(10);

    printState("Initial", i, c, s, f, b, a, o);

    mutateByValue(i, c, s, f, b, a, o);

    printState("After mutateByValue", i, c, s, f, b, a, o);

    mutateByRef(i, c, s, f, b, a, o);

    printState("After mutateByRef", i, c, s, f, b, a, o);
}