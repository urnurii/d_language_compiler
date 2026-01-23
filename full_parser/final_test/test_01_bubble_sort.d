void main()
{
    int n;
    write("Enter array length: ");
    readf("%d", n);

    int[] a = new int[n];

    for (int i = 0; i < n; i += 1)
        readf("%d", a[i]);

    write("Original array: ");
    for (int i = 0; i < n; i += 1)
    {
        if (i != 0) write(" ");
        write(a[i]);
    }
    writeln("");

    for (int i = 0; i < n - 1; i += 1)
    {
        for (int j = 0; j < n - i - 1; j += 1)
        {
            if (a[j] > a[j + 1])
            {
                int tmp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = tmp;
            }
        }
    }

    write("Sorted array: ");
    for (int i = 0; i < n; i += 1)
    {
        if (i != 0) write(" ");
        write(a[i]);
    }
    writeln("");
}
