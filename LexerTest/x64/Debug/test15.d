import std.stdio;

enum Color { RED, GREEN, BLUE }

enum Status { PENDING = 1, APPROVED = 2, REJECTED = 3 }

enum { SUCCESS = 0, FAILURE = 1 }

enum Weekday : byte { MON, TUE, WED, THU, FRI, SAT, SUN }

void main() {

    Color favorite = Color.GREEN;

    Status current = Status.APPROVED;

    int result = SUCCESS;

    Weekday today = Weekday.MON;
}