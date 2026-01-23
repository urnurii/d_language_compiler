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

Color nextColor(Color c)
{
    if (c == Color.Red) return Color.Green;
    if (c == Color.Green) return Color.Blue;
    return Color.Red;
}

void main()
{
    Color c = Color.Red;
    Status st = Status.Ok;

    writeln("Initial:");
    writeln(c);
    writeln(st);

    writeln("Switch on Color:");
    switch (c)
    {
        case Color.Red:
            writeln("Color is Red");
            break;
        case Color.Green:
            writeln("Color is Green");
            break;
        case Color.Blue:
            writeln("Color is Blue");
            break;
        default:
            writeln("Unknown color");
            break;
    }

    c = nextColor(c);
    writeln("After nextColor:");
    writeln(c);

    st = Status.Error;
    writeln("After status change:");
    writeln(st);
}