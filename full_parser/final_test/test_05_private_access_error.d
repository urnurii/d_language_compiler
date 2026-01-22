module full_parser.final_test.test_05_private_access_error;

class Test
{
private string prop = "Private";
}

class Another : Test
{
    void call()
    {
        string x = prop; // должно быть запрещено
        writeln(x);
    }
}

void main()
{
    Another obj = new Another();
    obj.call();
}
