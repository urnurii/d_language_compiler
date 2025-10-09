import std.stdio;

void main() {
    // Static arrays
    int[3] staticArr = [1, 2, 3];
    
    // Dynamic arrays
    int[] dynamicArr = [4, 5, 6, 7];
    int[] emptyArr = [];
    
    // Array access
    int firstElement = staticArr[0];
    int lastElement = dynamicArr[3];
    
    // Array slices
    int[] slice = dynamicArr[1..3]; // [5, 6]
    
    // Modifying through slice
    slice[0] = 50;
    
    writeln("Static array: ", staticArr);
    writeln("Dynamic array: ", dynamicArr);
    writeln("First element: ", firstElement);
    writeln("Last element: ", lastElement);
    writeln("Slice: ", slice);
    writeln("Original after slice modification: ", dynamicArr);
}