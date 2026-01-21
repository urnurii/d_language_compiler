bool hi(bool a)
{
    writeln("hello");
    return a;
}

void main()
{
    int number;
    readf("%d", number);

    if (number > 10 && hi(true))
        writeln("number > 10 && hi(true)");

    if (number > 5 && hi(true))
        writeln("number > 5 && hi(true)");

    if (number > 10 || hi(true))
        writeln("number > 10 || hi(true)");

    if (number > 5 || hi(true))
        writeln("number > 5 || hi(true)");

    if (number > 10 && hi(false))
        writeln("number > 10 && hi(false)");

    if (number > 5 && hi(false))
        writeln("number > 5 && hi(false)");

    if (number > 10 || hi(false))
        writeln("number > 10 || hi(false)");

    if (number > 5 || hi(false))
        writeln("number > 5 || hi(false)");
}
