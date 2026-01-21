class Obj
{
    int x;

    this(int v)
    {
        x = v;
    }
}

void mutateByValue(int i, string s, float f, bool b, int[] arr, Obj o)
{
    i = 111;
    s = "changed";
    f = 2.5;
    b = !b;

    // arr — динамический массив/срез: изменение элемента проверяет “ссылочность” данных
    if (arr.length > 0) arr[0] = 999;

    // o — объект: изменение поля проверяет ссылочность объектов
    o.x = 777;

    writeln("Inside mutateByValue:");
    writeln(i);
    writeln(s);
    writeln(f);
    writeln(b);
    if (arr.length > 0) writeln(arr[0]);
    writeln(o.x);
}

void mutateByRef(ref int i, ref string s, ref float f, ref bool b, ref int[] arr, ref Obj o)
{
    i = 222;
    s = "ref_changed";
    f = 3.5;
    b = !b;

    // переприсваивание массива должно быть видно снаружи при ref
    arr = [10, 20, 30];

    // перепривязка объекта должна быть видна снаружи при ref
    o = new Obj(888);

    writeln("Inside mutateByRef:");
    writeln(i);
    writeln(s);
    writeln(f);
    writeln(b);
    writeln(arr[0]);
    writeln(o.x);
}

void main()
{
    int i = 1;
    string s = "orig";
    float f = 1.5;
    bool b = true;

    int[] arr = [1, 2, 3];
    Obj o = new Obj(5);

    writeln("Initial:");
    writeln(i);
    writeln(s);
    writeln(f);
    writeln(b);
    writeln(arr[0]);
    writeln(o.x);

    mutateByValue(i, s, f, b, arr, o);

    writeln("After mutateByValue:");
    writeln(i);       // ожидается: 1
    writeln(s);       // ожидается: "orig"
    writeln(f);       // ожидается: 1.5
    writeln(b);       // ожидается: true
    writeln(arr[0]);  // проверяет семантику массивов/срезов
    writeln(o.x);     // ожидается: 777 (если классы ссылочные)

    mutateByRef(i, s, f, b, arr, o);

    writeln("After mutateByRef:");
    writeln(i);       // ожидается: 222
    writeln(s);       // ожидается: "ref_changed"
    writeln(f);       // ожидается: 3.5
    writeln(b);       // ожидается: false
    writeln(arr[0]);  // ожидается: 10
    writeln(o.x);     // ожидается: 888
}
