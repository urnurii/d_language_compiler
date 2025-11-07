import std.stdio;

class Animal
{
    private string name;
    protected int age;
    public string species;

    this(string name, int age, string species)
    {
        this.name = name;
        this.age = age;
        this.species = species;
    }

    public virtual void makeSound()
    {
        writeln(name, " издает звук");
    }

    public string getName()
    {
        return name;
    }
}

class Dog : Animal
{
    this(string name, int age)
    {
        super(name, age, "Собака");
    }

    public override void makeSound()
    {
        writeln(getName(), " гавкает: Гав-гав!");
    }
}

void callAnimalSound(Animal animal)
{
    animal.makeSound();
}

void callAllSounds(Animal[] animals)
{
    foreach (animal; animals)
    {
        animal.makeSound();
    }
}

Animal createRandomAnimal(int choice)
{
    if (choice == 1)
    {
        return new Dog("Барбос", 4);
    }
    else
    {
        return new Animal("Неизвестное животное", 1, "Неизвестный вид");
    }
}

Animal[] createAnimalArray()
{
    Animal[] animals = new Animal[2];
    animals[0] = new Dog("Рекс", 5);
    animals[1] = new Animal("Ящерица", 2, "Рептилия");
    return animals;
}

void main()
{
    Animal animal = new Animal("Животное", 5, "Неизвестный вид");
    Dog dog = new Dog("Шарик", 3);

    Animal[] animalsArray = [new Dog("Филипп", 4), new Dog("Джек", 2), new Animal("Ослик", 6, "Осел")];

    callAnimalSound(dog);
    callAllSounds(animalsArray);

    Animal returnedAnimal = createRandomAnimal(1);
    Animal[] returnedArray = createAnimalArray();
}