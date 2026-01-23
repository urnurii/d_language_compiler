enum Color
{
    Red = 1,
    Green = 2,
    Blue = 3
}

enum Status
{
    Ok = 0,
    Error = 1
}

void main()
{
    Color c = Color.Red;
    Status st = Status.Error;

    c = st;

    // st = c;

    writeln(c);
    writeln(st);
}
