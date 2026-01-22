bool hi(bool a, int b)
{
    writef("hello, %d\n", b);
    return a;
}

void main()
{
    int number;
    readf("%d", number);

    if (number > 10 && hi(true, 1))
        writeln("(1)number > 10 && hi(true)");

    if (number > 5 && hi(true, 2))
        writeln("(2)number > 5 && hi(true)");

    if (number > 10 || hi(true, 3))
        writeln("(3)number > 10 || hi(true)");

    if (number > 5 || hi(true, 4))
        writeln("(4)number > 5 || hi(true)");

    if (number > 10 && hi(false, 5))
        writeln("(5)number > 10 && hi(false)");

    if (number > 5 && hi(false, 6))
        writeln("(6)number > 5 && hi(false)");

    if (number > 10 || hi(false, 7))
        writeln("(7)number > 10 || hi(false)");

    if (number > 5 || hi(false, 8))
        writeln("(8)number > 5 || hi(false)");
}
