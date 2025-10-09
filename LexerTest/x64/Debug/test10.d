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
}