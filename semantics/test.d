int globalVar = 42;
float globalFloat = 3.14;

class Base {
    int baseField;
    
    void baseMethod() {
    }
}

class MyClass : Base {
    int x;
    float y = 2.5;
    
    this() {
    }
    
    this(int val) {
        x = val;
    }
    
    ~this() {
    }
    
    void setValue(int v) {
        x = v;
    }
    
    int getValue() {
        return x;
    }
    
    override void baseMethod() {
        super.baseMethod();
    }
    
    enum Status { Active = 1, Inactive = 0 }
}

int add(int a, int b) {
    return a + b;
}

void processArray(int[] arr) {
    int sum = 0;
    foreach (int val; arr) {
        sum = sum + val;
    }
}

void main() {
    int a = 5;
    int b = 10;
    float c = 3.14;
    bool flag = true;
    string text = "hello";
    Base obj1 = null;
    
    int result = a + b * 2;
    bool check = a < b && flag;
    
    int[] array = new int[5];
    int first = array[0];
    int[] slice = array[0 .. 3];
    
    MyClass instance = new MyClass(5);
    int val = instance.getValue();
    instance.setValue(20);
    
    MyClass obj2 = new MyClass();
    obj2.x = 100;
    
    if (a > 0) {
        a += 5;
    } else {
        a -= 3;
    }
    
    while (b < 50) {
        b *= 2;
    }
    
    do {
        c += 1.0;
    } while (c < 10.0);
    
    for (int i = 0; i < 10; i+=1) {
        a = a + i;
    }
    
    for (;;) {
        if (flag) break;
        flag = false;
    }
    
    switch (a) {
        case 1:
            result = 10;
            break;
        case 2:
            result = 20;
            break;
        default:
            result = 0;
            break;
    }
    
    {
        int nested = 5;
        {
            int deep = 10;
        }
    }

    writef("not Pi: %.1f ", c);
    
    return;
}