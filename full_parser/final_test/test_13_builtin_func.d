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

    writeln("ARRAY OUTPUT:");
    int[] arr = [1, 2, 3, 4, 5];
    writeln("arr = ", arr);
    writeln("arr[1..4] = ", arr[1..4]);
    writeln("arr[0..0] = ", arr[0..0]);
    writeln("arr[0..arr.length] = ", arr[0..arr.length]);

    int[3] arr2 = [7, 8, 9];
    writeln("arr2 = ", arr2);
    writeln("arr2[0..2] = ", arr2[0..2]);
    writeln("arr2[1..3] = ", arr2[1..3]);

    writeln("INPUT ARRAY:");
    int[] arr_in = new int[5];
    writeln("Enter 5 ints:");
    for (int idx = 0; idx < 5; idx += 1) {
        readf("%d", arr_in[idx]);
    }
    writeln("arr_in = ", arr_in);
    writeln("arr_in[1..4] = ", arr_in[1..4]);

    writeln("DONE");
}
