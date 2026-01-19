int globalVar = 42;

class Base {
    int x;
    
    void method() {
    }
}

class MyClass : Base {
    int value;
    
    this() {
    }
    
    ~this() {
    }
    
    void setValue(int v) {
        value = v;
    }
    
    enum Status { Active = 1, Inactive = 0 }
}

int add(int a, int b) {
    return a + b;
}

void main() {
    int a = 5;
    float b = 3.14;
    bool flag = true;
    string text = "hello";
    
    int result = a + 2;
    bool check = a < 10 && flag;
    
    int[] arr = new int[5];
    int val = arr[0];
    int[] slice = arr[0 .. 3];
    
    MyClass obj = new MyClass();
    obj.setValue(20);
    
    if (a > 0) {
        a += 5;
    } else {
        a -= 3;
    }
    
    while (b < 10.0) {
        b += 1.0;
    }
    
    for (int i = 0; i < 5; i += 1) {
        a = a + i;
    }
    
    switch (a) {
        case 1:
            result = 10;
            break;
        default:
            result = 0;
            break;
    }
    
    return;
}
