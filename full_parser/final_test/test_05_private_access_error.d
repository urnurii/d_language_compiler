class Test
{
protected string prop = "Private";
}

class Another : Test
{
    void call()
    {
        string x = prop;
        writeln(x);
    }
}

void main()
{
    Another obj = new Another();
    obj.call();
    string x = obj.prop;
}
