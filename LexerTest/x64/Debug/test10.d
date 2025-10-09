import std.stdio;

void main() {
    int[2][3] matrix = [
        [1, 2],
        [3, 4],
        [5, 6]
    ];
    
    int[][] dynamicMatrix;
    dynamicMatrix ~= [7, 8];
    dynamicMatrix ~= [9, 10, 11];

    int element = matrix[1][1];
    int dynElement = dynamicMatrix[0][1];

    int[] rowSlice = matrix[0..2][0];
    
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