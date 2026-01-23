void main()
{
    writeln("Enter a line:");
    string line = readln();

    writeln("Enter: int float char word");
    int i;
    float f;
    char c;
    string s;

    readf("%d %f %c %s", i, f, c, s);

    writeln("ECHO:");
    writeln(line);
    writeln(i);
    writeln(f);
    writeln(c);
    writeln(s);

    write("WRITE: ");
    write(i);
    write(" ");
    write(f);
    write(" ");
    write(c);
    write(" ");
    writeln(s);

    writef("FORMAT: i=%d f=%f c=%c s=%s\n", i, f, c, s);
    writeln("DONE");
}
