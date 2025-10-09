import std.stdio;

class AccessExample {
    public string publicField = "public";
    private string privateField = "private";
    protected string protectedField = "protected";
    
    public void publicMethod() {
        writeln("Public method called");
    }
    
    private void privateMethod() {
        writeln("Private method called");
    }
    
    protected void protectedMethod() {
        writeln("Protected method called");
    }
    
    // Method to access private members
    public void accessPrivate() {
        writeln("Accessing private field: ", privateField);
        privateMethod();
    }
}

class Derived : AccessExample {
    public void testProtected() {
        writeln("Accessing protected field: ", protectedField);
        protectedMethod();
    }
}

void main() {
    auto obj = new AccessExample();
    
    // Public access
    obj.publicField = "modified public";
    obj.publicMethod();
    obj.accessPrivate();
    
    // Private access (would cause error if uncommented)
    // obj.privateField = "error"; // Compilation error
    // obj.privateMethod();        // Compilation error
    
    // Protected access test
    auto derived = new Derived();
    derived.testProtected();
    
    writeln("Public field: ", obj.publicField);
}