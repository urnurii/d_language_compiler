import std.stdio;

void main() {
    // Integer literals
    int decimal = 123;
    int binary = 0b101010;
    int octal = 0o52;
    int hex = 0x2A;
    int withUnderscore = 1_000_000;
    
    // String literals
    string normal = "Hello, World!";
    string multiline = `This is a
multi-line
string.`;
    
    // Escape sequences
    string escapes = "Line1\nLine2\tTabbed\\Backslash\"Quote";
    
    writeln("Decimal: ", decimal);
    writeln("Binary: ", binary);
    writeln("Octal: ", octal);
    writeln("Hex: ", hex);
    writeln("With underscore: ", withUnderscore);
    writeln("Normal string: ", normal);
    writeln("Multiline: ", multiline);
    writeln("Escapes: ", escapes);
}