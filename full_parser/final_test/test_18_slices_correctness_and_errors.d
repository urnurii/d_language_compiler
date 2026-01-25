void printArray(string tag, int[] a)
{
    write(tag);
    write("[");
    for (int i = 0; i < a.length; i += 1)
    {
        if (i != 0) write(", ");
        write(a[i]);
    }
    writeln("]");
}

void main()
{
    int[] a = [10, 11, 12, 13, 14];

    printArray("a initial = ", a);

    int[] s = a[1..4]; // ожидается: [11, 12, 13]
    printArray("s = a[1..4] = ", s);

    s[0] = 999;
    printArray("s after s[0]=999 = ", s);
    printArray("a after changing s = ", a); // ожидается: [10,11,12,13,14]

    int[] t = a[0..2]; // [10,11]
    printArray("t = a[0..2] = ", t);

    int[] n = null;
    int[] z = n[0..0];
    writeln("z.length = ", z.length); // ожидается: 0

    // ERROR: start < 0 
    // int[] e1 = a[-1..2];
    // printArray("e1 = ", e1);

    // ERROR: end > length
    // int[] e2 = a[0..10];
    // printArray("e2 = ", e2);

    // ERROR: end < start
    // int[] e3 = a[3..1];
    // printArray("e3 = ", e3);

    // ERROR: Неверные типы индексов
    // float k = 1.2;
    // int[] e4 = a[k..2];
    // printArray("e4 = ", e4);

    // ERROR: Присваивание в срез не массива
    // a[1..3] = 123;
    // printArray("a after invalid rhs = ", a);
}
