import std.stdio;

enum Color { RED, GREEN, BLUE }

enum Status { PENDING = 1, APPROVED = 2, REJECTED = 3 }

enum { SUCCESS = 0, FAILURE = 1 }

enum Weekday : byte { MON, TUE, WED, THU, FRI, SAT, SUN }

void main() {

    Color favorite = Color.GREEN;
    writeln("Favorite color: ", favorite);

    Status current = Status.APPROVED;
    writeln("Current status: ", current, " (value: ", cast(int)current, ")");

    int result = SUCCESS;
    writeln("Result: ", result);

    Weekday today = Weekday.MON;
    writeln("Today: ", today, " (byte value: ", cast(byte)today, ")");

    switch (favorite) {
        case Color.RED:
            writeln("It's red!");
            break;
        case Color.GREEN:
            writeln("It's green!");
            break;
        case Color.BLUE:
            writeln("It's blue!");
            break;
        default:
            break;
    }

    writeln("All colors:");
    writeln("RED: ", Color.RED);
    writeln("GREEN: ", Color.GREEN);
    writeln("BLUE: ", Color.BLUE);
}