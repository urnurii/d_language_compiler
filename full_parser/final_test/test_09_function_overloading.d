
void myFunction(int number1, int number2) {
    writef("hello, %d, %d\n", number1, number2);
}

void myFunction(int number) {
    writef("hello, %d\n", number);
}

void myFunction(float number) {
    writef("hello, %f\n", number);
}

void myFunction(string name) {
    writef("hello, %s\n", name);
}

void myFunction(char character) {
    writef("hello, %c\n", character);
}

void main() {
    int one = 1;
    int two = 2;
    int three = 3;
    string name = "Hector";
    char letter = 't';
    float zero_three = 0.3;

    myFunction(one, two);
    myFunction(three);
    myFunction(name);
    myFunction(letter);
    myFunction(zero_three);
}