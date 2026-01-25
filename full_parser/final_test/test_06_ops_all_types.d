void main()
{
    // int literals
    int dec = 123;
    int bin = 0b101010;   // 42
    int hex = 0x2A;       // 42
    int million = 1_000_000;

    writeln("INT LITERALS:");
    writeln(dec);
    writeln(bin);
    writeln(hex);
    writeln(million);

    // int ops
    int a = 10;
    int b = 4;
    writeln("INT OPS:");
    writeln(a + b);
    writeln(a - b);
    writeln(a * b);
    writeln(a / b);

    int c = a;
    c += 2;
    writeln(c);
    c -= 1;
    writeln(c);
    c *= 3;
    writeln(c);
    c /= 2;
    writeln(c);

    writeln("INT COMPARE:");
    writeln(a == 10);
    writeln(a != b);
    writeln(a < b);
    writeln(a <= b);
    writeln(a > b);
    writeln(a >= b);

    // float ops
    float xf = 5.0;
    float yf = 2.0;
    float sf = xf + yf;
    float df = xf - yf;
    float pf = xf * yf;
    float qf = xf / yf;

    writeln("FLOAT OPS:");
    writeln(sf);
    writeln(df);
    writeln(pf);
    writeln(qf);

    // char + escapes
    char ch = 'A';
    char nl = '\n';
    char tab = '\t';
    char quote = '\"';
    char slash = '\\';

    writeln("CHAR:");
    writeln(ch);
    writeln(nl);
    writeln(tab);
    writeln(quote);
    writeln(slash);

    // string + escapes
    string s1 = "Hello, D!";
    string s2 = "Line1\nLine2\tTabbed\\Backslash\"Quote\"";
    writeln("STRING:");
    writeln(s1);
    writeln(s2);

    // logic
    bool t = true;
    bool f = false;
    writeln("LOGIC:");
    writeln(t && f);
    writeln(t || f);
    writeln(!t);

    // if/switch minimal usage
    int x = 2;
    if (x > 1) writeln("if-ok"); else writeln("if-else");

    switch (x)
    {
        case 1: writeln("case1"); break;
        case 2: writeln("case2"); break;
        default: writeln("default");
    }
}
