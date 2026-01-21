class Counter
{
public:
    static int count = 0;
}

class Another : Counter {}

void main()
{
    writeln(Counter.count);
    writeln(Another.count);

    Another.count = 5;

    writeln(Counter.count);
    writeln(Another.count);
}
