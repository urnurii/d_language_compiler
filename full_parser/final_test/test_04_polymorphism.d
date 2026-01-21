class BaseClass
{
    void cool()
    {
        writeln("BaseClass.cool()");
    }
}

class ChildClass : BaseClass
{
    override void cool()
    {
        writeln("ChildClass.cool()");
    }
}

class AnotherClass : BaseClass
{
    override void cool()
    {
        writeln("AnotherClass.cool()");
        super.cool();
    }
}

void main()
{
    BaseClass[] arr = [new BaseClass(), new ChildClass(), new AnotherClass()];

    foreach (obj; arr)
        obj.cool();
}
