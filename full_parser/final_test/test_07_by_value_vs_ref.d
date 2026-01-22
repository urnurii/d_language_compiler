void mutateByValue(int i, string s, float f, bool b)
{
    i = 111;
    s = "changed";
    f = 2.5;
    b = !b;

    writeln("Inside mutateByValue:");
    writeln(i);
    writeln(s);
    writeln(f);
    writeln(b);
}

void mutateByRef(ref int i, ref string s, ref float f, ref bool b)
{
    i = 222;
    s = "ref_changed";
    f = 3.5;
    b = !b;

    writeln("Inside mutateByRef:");
    writeln(i);
    writeln(s);
    writeln(f);
    writeln(b);
}

void main()
{
    int i = 1;
    string s = "orig";
    float f = 1.5;
    bool b = true;

    writeln("Initial:");
    writeln(i);
    writeln(s);
    writeln(f);
    writeln(b);

    mutateByValue(i, s, f, b);

    writeln("After mutateByValue:");
    writeln(i);       // ожидается: 1
    writeln(s);       // ожидается: "orig"
    writeln(f);       // ожидается: 1.5
    writeln(b);       // ожидается: true

    mutateByRef(i, s, f, b);

    writeln("After mutateByRef:");
    writeln(i);       // ожидается: 222
    writeln(s);       // ожидается: "ref_changed"
    writeln(f);       // ожидается: 3.5
    writeln(b);       // ожидается: false
}
