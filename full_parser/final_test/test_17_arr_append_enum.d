enum E
{
    A = 1,
    B = 2
}

void main()
{
    E[] a = null;
    a ~= E.A;
    a ~= E.B;

    writeln(a.length); // 2
    writeln(a[0]);     // 1
    writeln(a[1]);     // 2
}