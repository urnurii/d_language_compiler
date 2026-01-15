module full_parser.tests.test_super;

class Child {
    void method() {
        super.field;
        super.method();
        super.method(1);
        super.method(1, 2);
    }
}
