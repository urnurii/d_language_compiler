
class Animal {
    int age;
    
    void speak() {
    }
}

class Dog : Animal {
    override void speak() {
    }
    
    void callSuper() {
        super.speak();
    }
}

    Animal animal = new Animal();
    animal.age = 3;
    animal.speak();


    
