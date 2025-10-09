import std.stdio;
import std.math : abs;

void main() {
    float f = 3.14f;
    double d = 2.718281828459045;
    real r = 1.618033988749895;

    float sum = f + 1.0f;
    double diff = d - 1.0;
    real prod = r * 2.0;
    double quot = d / 2.0;

    float a = 0.1f + 0.2f;
    float b = 0.3f;
    
    if (abs(a - b) < 1e-6) {
    }

    f += 1.5f;
    d *= 2.0;
}