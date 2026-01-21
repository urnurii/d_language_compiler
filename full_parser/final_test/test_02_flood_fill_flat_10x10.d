int idx(int x, int y, int m)
{
    return x * m + y;
}

void printMatrix(int[] grid, int n, int m)
{
    for (int i = 0; i < n; i += 1)
    {
        for (int j = 0; j < m; j += 1)
        {
            if (j != 0) write(" ");
            write(grid[idx(i, j, m)]);
        }
        writeln("");
    }
}

void floodFill(int[] grid, int n, int m, int x, int y, int oldColor, int newColor)
{
    if (x < 0 || x >= n || y < 0 || y >= m) return;

    int p = idx(x, y, m);
    if (grid[p] != oldColor) return;

    grid[p] = newColor;

    floodFill(grid, n, m, x + 1, y, oldColor, newColor);
    floodFill(grid, n, m, x - 1, y, oldColor, newColor);
    floodFill(grid, n, m, x, y + 1, oldColor, newColor);
    floodFill(grid, n, m, x, y - 1, oldColor, newColor);
}

void main()
{
    int n = 10;
    int m = 10;

    int[] grid = [
        0,0,0,0,0,0,0,0,0,0,
        1,1,1,1,1,1,1,1,1,0,
        0,0,0,0,0,0,0,0,1,0,
        0,1,1,1,1,1,1,0,1,0,
        0,1,0,0,0,0,1,0,1,0,
        0,1,0,1,1,0,1,0,1,0,
        0,1,0,1,1,1,1,0,1,0,
        0,1,0,0,0,0,0,0,1,0,
        0,1,1,1,1,1,1,1,1,0,
        0,0,0,0,0,0,0,0,0,0
    ];

    int x, y, newColor;
    readf("%d", x);
    readf("%d", y);
    readf("%d", newColor);

    int oldColor = grid[idx(x, y, m)];
    if (oldColor != newColor)
        floodFill(grid, n, m, x, y, oldColor, newColor);

    printMatrix(grid, n, m);
}
