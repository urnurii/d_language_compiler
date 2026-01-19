class MyClass {
    void method() {
    }
    
    void methodWithArgs(int a, int b) {
    }
    
    int getValue() {
        return 42;
    }
}

void test() {
    MyClass obj = new MyClass();
    obj.method();
    obj.methodWithArgs(1, 2);
    int x = obj.getValue();
}
