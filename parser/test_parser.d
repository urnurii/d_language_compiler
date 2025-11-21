import std.stdio;
import std.math;

enum Color {
    Red,
    Green,
    Blue
}

enum Status {
    Pending = 1,
    Approved = 2,
    Rejected = 3
}

enum Direction {
    North,
    South,
    East,
    West
}

class Animal {
    public string name;
    protected int age;
    private double weight;
    
    this() {
        this.name = "Unknown";
        this.age = 0;
        this.weight = 0.0;
    }
    
    this(string name, int age, double weight) {
        this.name = name;
        this.age = age;
        this.weight = weight;
    }
    
    ~this() {
        int x = 0;
    }
    
    public void speak() {
        int x = -1;
    }
    
    protected int getAge() {
        return this.age;
    }
    
    private void privateMethod() {
        int x = 5;
    }
}

class Dog : Animal {
    private string breed;
    
    this(string name, int age) {
        super(name, age, 0.0);
        this.breed = "Mixed";
    }
    
    override void speak() {
        super.speak();
        int x = 2;
    }
}

class NumberSet {
    public int[] numbers;
    
    this(int[] nums) {
        this.numbers = nums;
    }
    
    public int sum() {
        int result = 0;
        foreach (val; numbers) {
            result += val;
        }
        return result;
    }
    
    public double average() {
        if (numbers.length == 0) return 0.0;
        double sum = 0.0;
        foreach (val; numbers) {
            sum += val;
        }
        return sum / numbers.length;
    }
}

int min(int a, int b) {
    if (a < b) {
        return a;
    } else {
        return b;
    }
}

double min(double a, double b) {
    if (a < b) {
        return a;
    }
    return b;
}

int add(int x, int y) {
    return x + y;
}

float add(float x, float y) {
    return x + y;
}

bool allPositive(int[] arr) {
    foreach (val; arr) {
        if (val <= 0) {
            return false;
        }
    }
    return true;
}

int findMin(int[] arr) {
    if (arr.length == 0) {
        return 0;
    }
    int minVal = arr[0];
    foreach (val; arr) {
        if (val < minVal) {
            minVal = val;
        }
    }
    return minVal;
}

int countNegatives(int[] arr) {
    int count = 0;
    foreach (val; arr) {
        if (val < 0) {
            count += 1;
        }
    }
    return count;
}

real powerReal(real base, int exp) {
    real result = 1.0;
    for (int i = 0; i < exp; i+=1) {
        result *= base;
    }
    return result;
}

real calcTriangleArea(real a, real b, real c) {
    real s = (a + b + c) / 2;
    return sqrt(s * (s - a) * (s - b) * (s - c));
}

double average(double[] arr) {
    if (arr.length == 0) {
        return 0.0;
    }
    double sum = 0.0;
    foreach (val; arr) {
        sum += val;
    }
    return sum / arr.length;
}

void demonstrateVariables() {
    int a = 10;
    char b = 'A';
    string s = "Hello, D!";
    float f = 3.14f;
    double d = 2.718281828459045;
    real r = 1.618033988749895;
    long lng = 1234567890L;
    bool flag = true;
}

// Выражения и операторы
void demonstrateExpressions() {
    int a = 10;
    int b = 5;
    int sum = a + b;
    int diff = a - b;
    int prod = a * b;
    int quot = a / b;
    
    int compound = a;
    compound += 5;
    compound -= 2;
    compound *= 3;
    compound /= 4;
    
    bool isEqual = (a == 10);
    bool isNotEqual = (a != b);
    bool isGreater = (a > b);
    bool isLess = (a < b);
    bool isGreaterEqual = (a >= b);
    bool isLessEqual = (a <= b);
}

void demonstrateFloatOperations() {
    float x = 5.0;
    float y = 2.0;
    float sum = x + y;
    float diff = x - y;
    float prod = x * y;
    float quot = x / y;
    
    double dx = 10.5;
    double dy = 3.2;
    double dsum = dx + dy;
    double ddiff = dx - dy;
    double dprod = dx * dy;
    double dquot = dx / dy;
}

void demonstrateArrays() {
    int[5] arr;
    int[3] arr2 = [1, 2, 3];
    int[] arr3 = new int[3];
    int[] arr4 = null;
    int[] arr5;
    
    arr[0] = 10;
    arr[1] = 20;
    int element = arr[0];
    
    int[] slice = arr2[1..3];
}

void demonstrateLiterals() {
    int dec = 123;
    int hex = 0xFF;
    int bin = 0b1010;
    int oct = 0511;
    
    float f1 = 3.14f;
    float f2 = -2.5f;
    double d1 = 1.23e10;
    double d2 = 5.67e-3;
    
    char c1 = 'A';
    char c2 = '\n';
    char c3 = '\t';
    char c4 = '\\';
    char c5 = '\'';
    
    string s1 = "Hello";
    string s2 = "Line1\nLine2";
    string s3 = "Tab\there";
    string s4 = "Quote\"test";
}

void demonstrateConditionals(int x) {
    if (x > 0) {
        int pos = 1;
    }
    
    if (x > 0) {
        int pos = 1;
    } else {
        int neg = -1;
    }
    
    if (x > 0) {
        int pos = 1;
    } else if (x < 0) {
        int neg = -1;
    } else {
        int zero = 0;
    }
}

void demonstrateLoops() {
    int[] arr = [1, 2, 3, 4, 5];
    foreach (elem; arr) {
        int val = elem;
    }
}

void demonstrateLogical() {
    bool a = true;
    bool b = false;
    
    bool andResult = a && b;
    bool orResult = a || b;
    bool notResult = !a;
    
    int x = 5;
    int y = 10;
    bool complex = (x > 0) && (y < 20);
    bool complex2 = (x < 0) || (y > 5);
    bool complex3 = !(x == y);
}

void demonstrateEnums() {
    Color col = Color.Red;
    Status stat = Status.Pending;
    Direction dir = Direction.North;
    
    if (col == Color.Red) {
        int r = 1;
    }
    
    switch (stat) {
        case Status.Pending:
            int p = 1;
            break;
        case Status.Approved:
            int a = 2;
            break;
        default:
            int d = 0;
            break;
    }
}

void demonstrateClasses() {
    Animal animal1 = new Animal();
    Animal animal2 = new Animal("Cat", 3, 5.0);
    
    Dog dog1 = new Dog("Rex", 4);
    dog1.speak();
}

void nestedLoops() {
    for (int i = 0; i < 3; i+=1) {
        for (int j = 0; j < 3; j+=1) {
            int prod = i * j;
        }
    }
}

void nestedConditionals(int x, int y) {
    if (x > 0) {
        if (y > 0) {
            int both = 1;
        } else {
            int onlyX = 2;
        }
    } else {
        if (y > 0) {
            int onlyY = 3;
        } else {
            int none = 4;
        }
    }
}

void arrayOperations() {
    int[] arr = [10, 20, 30, 40, 50];
    
    for (int i = 0; i < arr.length; i+=1) {
        arr[i] = arr[i] * 2;
    }
    
    foreach (ref elem; arr) {
        elem = elem + 1;
    }
}

void floatComparison() {
    float a = 0.1 + 0.2;
    float b = 0.3;
    
    if (abs(a - b) < 1e-6) {
        int equal = 1;
    } else {
        int notEqual = 0;
    }
    
    double d1 = 10.5;
    double d2 = 20.3;
    
    if (d1 < d2) {
        int less = 1;
    }
}

void complexExpressions() {
    int a = 5;
    int b = 10;
    int c = 15;
    
    int result1 = (a + b) * c;
    int result2 = a + (b * c);
    int result3 = (a + b) / (c - a);
    
    bool logic1 = (a < b) && (b < c);
    bool logic2 = (a > b) || (c > b);
    bool logic3 = !(a == b) && (c > a);
    
    float f1 = 3.14f;
    float f2 = 2.71f;
    float f3 = 1.41f;
    
    float fresult = (f1 + f2) * f3;
    bool fcomp = (f1 > f2) && (f2 > f3);
}

void demonstrateNegativeNumbers() {
    int neg1 = -10;
    int neg2 = -5;
    int pos = 15;
    
    int sumNegNeg = neg1 + neg2;
    int sumNegPos = neg1 + pos;
    int diffNegNeg = neg1 - neg2;
    int prodNegNeg = neg1 * neg2;
    int quotNegNeg = neg1 / neg2;
    
    int negation = -neg1;
    int doubleNeg = -(-neg1);
    
    bool comp1 = neg1 < neg2;
    bool comp2 = neg1 > neg2;
    bool comp3 = neg1 == -10;
    
    float negFloat1 = -3.14f;
    float negFloat2 = -2.71f;
    
    float floatSum = negFloat1 + negFloat2;
    float floatProd = negFloat1 * negFloat2;
    
    double negDouble1 = -10.5;
    double negDouble2 = -3.2;
    
    double doubleSum = negDouble1 + negDouble2;
    
    int complex1 = (neg1 + pos) * neg2;
    bool logic1 = (neg1 < 0) && (neg2 < 0);
    
    int[] negArray = [-10, -5, -3, -1];
    int sumNeg = 0;
    foreach (val; negArray) {
        sumNeg += val;
    }
}

void validateInput(int x, int y);

void main() {
    demonstrateVariables();
    demonstrateExpressions();
    demonstrateFloatOperations();
    demonstrateArrays();
    demonstrateLiterals();
    demonstrateConditionals(5);
    demonstrateLoops();
    demonstrateLogical();
    demonstrateEnums();
    demonstrateClasses();
    nestedLoops();
    nestedConditionals(5, 10);
    arrayOperations();
    floatComparison();
    complexExpressions();
    demonstrateNegativeNumbers();
    validateInput(5, 10);
    
    int x = min(5, 10);
    double y = min(5.5, 10.3);
    
    int sum1 = add(5, 3);
    float sum2 = add(3.14f, 2.71f);
    
    int[] testArr = [1, 2, 3, 4, 5];
    bool allPos = allPositive(testArr);
    
    real power = powerReal(2.0, 3);
    real triArea = calcTriangleArea(3.0, 4.0, 5.0);
    
    double[] doubleArr = [1.5, 2.5, 3.5, 4.5];
    double avg = average(doubleArr);
    
    NumberSet ns = new NumberSet(testArr);
    int nsSum = ns.sum();
    double nsAvg = ns.average();
    
    int[] negTestArr = [-10, -5, 3, -2, 8, -1];
    int minVal = findMin(negTestArr);
    int negCount = countNegatives(negTestArr);
}

void validateInput(int x, int y) {
    if (x == 0 || y == 0) {
        return;
    }
    
    int sum = x + y;
    int product = x * y;
}