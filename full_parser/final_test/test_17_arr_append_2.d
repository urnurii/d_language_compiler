void main()
{
    int[] a = null;
    int[] b = [1, 2, 3];

    a ~= b;

    writeln(a.length); // 3
    writeln(a[0]);     // 1
    writeln(a[1]);     // 2
    writeln(a[2]);     // 3
}