import std.stdio;

// Named enum
enum Color { RED, GREEN, BLUE }

// Enum with explicit values
enum Status { PENDING = 1, APPROVED = 2, REJECTED = 3 }

// Anonymous enum
enum { SUCCESS = 0, FAILURE = 1 }

// Enum with base type
enum Weekday : byte { MON, TUE, WED, THU, FRI, SAT, SUN }

void main() {
    // Using named enum
    Color favorite = Color.GREEN;
    writeln("Favorite color: ", favorite);
    
    // Enum with values
    Status current = Status.APPROVED;
    writeln("Current status: ", current, " (value: ", cast(int)current, ")");
    
    // Anonymous enum usage
    int result = SUCCESS;
    writeln("Result: ", result);
    
    // Weekday enum
    Weekday today = Weekday.MON;
    writeln("Today: ", today, " (byte value: ", cast(byte)today, ")");
    
    // Switch with enum
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
    
    // Iterating through enum values
    writeln("All colors:");
    writeln("RED: ", Color.RED);
    writeln("GREEN: ", Color.GREEN);
    writeln("BLUE: ", Color.BLUE);
}