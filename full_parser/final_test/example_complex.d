enum Color
{
    Red = 1,
    Green,
    Blue
}

int sum(int a, int b)
{
    return a + b;
}

float sum(float a, float b)
{
    return a + b;
}

int fact(int n)
{
    if (n <= 1)
        return 1;
    return n * fact(n - 1);
}

void inc(ref int x)
{
    x += 1;
}

string greet(string name = "Guest")
{
    string msg = "Hello, ";
    msg ~= name;
    msg ~= '!';
    return msg;
}

class Animal
{
    string name;

    this()
    {
        name = "noname";
    }

    this(string name)
    {
        this.name = name;
    }

    void speak()
    {
        writeln("Animal: ", name);
    }
}

class Dog : Animal
{
    string breed;

    this(string name, string breed)
    {
        this.name = name;
        this.breed = breed;
    }

    override void speak()
    {
        super.speak();
        writeln("Dog breed: ", breed);
    }

    ~this()
    {
        writeln("Dog destroyed: ", name);
    }
}

void main()
{
    writeln("=== BASIC I/O ===");
    writeln("Enter a line:");
    string line = readln();
    writeln("Line: ", line);

    writeln("Enter two ints, one float, one char, one word:");
    int i1;
    int i2;
    float f1;
    char ch;
    string word;
    readf("%d %d %f %c %s", i1, i2, f1, ch, word);

    writef("readf: i1=%d i2=%d f1=%f ch=%c word=%s\n", i1, i2, f1, ch, word);
    writeln(greet());
    writeln(greet("Codex"));

    writeln("=== OVERLOADS ===");
    writeln("sum(int): ", sum(i1, i2));
    writeln("sum(float): ", sum(f1, 2.5));

    writeln("=== REF PARAM ===");
    inc(i1);
    writeln("after inc: ", i1);

    writeln("=== RECURSION ===");
    writeln("fact(5) = ", fact(5));

    writeln("=== ARRAYS ===");
    int[3] arrStatic = [1, 2, 3];
    writeln("arrStatic = ", arrStatic);
    writeln("arrStatic[0..2] = ", arrStatic[0..2]);

    int[] arr = [10, 20, 30];
    arr ~= 40;
    arr ~= 50;
    writeln("arr = ", arr);
    writeln("arr.length = ", arr.length);
    writeln("arr[1..4] = ", arr[1..4]);
    writeln("arr[0..0] = ", arr[0..0]);
    writeln("arr[3..1] = ", arr[3..1]);

    writeln("=== FOREACH ===");
    int sumArr = 0;
    foreach (x; arr)
    {
        sumArr += x;
    }
    writeln("foreach sum = ", sumArr);

    writeln("=== LOOPS ===");
    int forSum = 0;
    for (int k = 0; k < 5; k += 1)
    {
        if (k == 2) continue;
        forSum += k;
    }
    writeln("forSum = ", forSum);

    int w = 3;
    int whileSum = 0;
    while (w > 0)
    {
        whileSum += w;
        w -= 1;
    }
    writeln("whileSum = ", whileSum);

    int d = 0;
    int doSum = 0;
    do
    {
        d += 1;
        doSum += d;
    } while (d < 3);
    writeln("doSum = ", doSum);

    writeln("=== SWITCH ===");
    Color c = Color.Green;
    switch (c)
    {
        case Color.Red:
            writeln("Color is red");
            break;
        case Color.Green:
            writeln("Color is green");
            break;
        default:
            writeln("Color is blue");
            break;
    }

    writeln("=== CLASSES ===");
    Dog d1 = new Dog("Rex", "Shepherd");
    d1.speak();
    destroy(d1);

    writeln("=== LOGIC ===");
    bool ok = (i1 > 0 && i2 > 0) || (i1 == 0);
    if (!ok)
    {
        writeln("logic result: false");
    }
    else
    {
        writeln("logic result: true");
    }

    string msg = format("format: %s %d %f", word, i2, f1);
    writeln(msg);

    writeln("DONE");
}
