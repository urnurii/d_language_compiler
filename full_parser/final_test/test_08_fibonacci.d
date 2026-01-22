int fib(int n)
{
    if (n <= 1) return n;
    return fib(n - 1) + fib(n - 2);
}

void main()
{
    int n;
    readf("%d", n);

    for (int i = 0; i <= n; i += 1)
    {
        if (i != 0) write(" ");
        write(fib(i));
    }
    writeln("");
}
