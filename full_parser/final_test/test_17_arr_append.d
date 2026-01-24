void main()
{
    int[] a = [];
    a ~= 1;
    a ~= 2;

    write("LEN(expected 2)=");
    writeln(a.length);
    write("A0(expected 1)=");
    writeln(a[0]);
    write("A1(expected 2)=");
    writeln(a[1]);
}
