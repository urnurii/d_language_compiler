import std.stdio;

class AccessExample {
    public string publicField = "public";
    private string privateField = "private";
    protected string protectedField = "protected";
    
    public void publicMethod() {
    }
    
    private void privateMethod() {
    }
    
    protected void protectedMethod() {
    }
    
    public void accessPrivate() {
        privateMethod();
    }
}

class Derived : AccessExample {
    public void testProtected() {
        protectedMethod();
    }
}
