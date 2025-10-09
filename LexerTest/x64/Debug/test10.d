import std.stdio;

void main() {
    // Fixed-size 2D array
    int[2][3] matrix = [
        [1, 2],
        [3, 4],
        [5, 6]
    ];
    
    // Dynamic 2D array
    int[][] dynamicMatrix;
    dynamicMatrix ~= [7, 8];
    dynamicMatrix ~= [9, 10, 11]; // Jagged array
    
    // Access elements
    int element = matrix[1][1]; // 4
    int dynElement = dynamicMatrix[0][1]; // 8
    
    // Slices
    int[] rowSlice = matrix[0..2][0]; // First elements of first two rows
    
    writeln("Fixed matrix:");
    foreach (i; 0..3) {
        foreach (j; 0..2) {
            write(matrix[i][j], " ");
        }
        writeln();
    }
    
    writeln("Dynamic matrix:");
    foreach (row; dynamicMatrix) {
        writeln(row);
    }
    
    writeln("Element [1][1]: ", element);
    writeln("Dynamic element [0][1]: ", dynElement);
    writeln("Row slice: ", rowSlice);
}