// Однострочный комментарий с разными символами !@#$%
/* Многострочный комментарий
   может содержать * символы
   и переносы строк */

// Тестирование ключевых слов
int main() {
    // Все типы данных
    int x;
    char c;
    string s;
    bool flag;
    float f;
    double d;
    real r;
    
    // Модификаторы доступа и другие ключевые слова
    public class MyClass {
        private int value;
        protected string name;
        
        public override void method() {
            auto temp = new MyClass();
            this.value = 42;
            super.method();
        }
    }
    
    enum Color { RED, GREEN, BLUE }
    
    // Управляющие конструкции
    if (true) {
        return 0;
    } else {
        while (false) {
            break;
        }
        
        for (int i = 0; i < 10; i++) {
            continue;
        }
        
        foreach (item; collection) {
            // тело цикла
        }
        
        do {
            // что-то делаем
        } while (condition);
        
        switch (value) {
            case 1:
                break;
            default:
                break;
        }
    }
    
    // Спецзначения
    void* ptr = null;
    double nan = NaN;
    bool t = true;
    bool f = false;
    
    // ТЕСТ ЦЕЛЫХ ЧИСЕЛ - спорные случаи
    
    // Обычные десятичные
    int dec1 = 0;           // ноль
    int dec2 = 123;         // обычное число
    int dec3 = 1_000_000;   // с разделителями
    
    // Двоичные
    int bin1 = 0b1010;      // двоичное
    int bin2 = 0B1111_0000; // двоичное с разделителями (если поддерживается)
    
    // Восьмеричные - СПОРНЫЕ СЛУЧАИ
    int oct1 = 0777;        // корректное восьмеричное
    int oct2 = 0123;        // корректное восьмеричное
    int oct3 = 0899;        // ОШИБКА! содержит недопустимые цифры 8,9
    
    // Шестнадцатеричные
    int hex1 = 0xFF;        // шестнадцатеричное
    int hex2 = 0xDEAD_BEEF; // с разделителями (если поддерживается)
    int hex3 = 0X123abc;    // смешанный регистр
    
    // ТЕСТ ЧИСЕЛ С ПЛАВАЮЩЕЙ ТОЧКОЙ
    
    // Обычные формы
    float f1 = 3.14;        // обычная дробь
    float f2 = 123.;        // без дробной части
    float f3 = .5;          // СПОРНЫЙ! начинается с точки
    float f4 = 1.5f;        // с суффиксом f
    float f5 = 2.0F;        // с суффиксом F
    
    // Экспоненциальная запись
    double e1 = 1e10;       // простая экспонента
    double e2 = 2.5e-3;     // отрицательная экспонента
    double e3 = 1E+5;       // положительная экспонента
    double e4 = 6.02e23f;   // с суффиксом
    
    // ТЕСТ СИМВОЛЬНЫХ И СТРОКОВЫХ ЛИТЕРАЛОВ
    
    // Обычные символы
    char ch1 = 'a';
    char ch2 = 'Z';
    char ch3 = '0';
    
    // Спецсимволы
    char esc1 = '\n';       // перенос строки
    char esc2 = '\t';       // табуляция
    char esc3 = '\\';       // обратный слеш
    char esc4 = '\'';       // апостроф
    char esc5 = '\"';       // кавычка
    char esc6 = '\b';       // backspace
    char esc7 = '\f';       // form feed
    char esc8 = '\r';       // возврат каретки
    char esc9 = '\v';       // вертикальная табуляция
    char esc10 = '\?';      // знак вопроса
    
    // Восьмеричные и шестнадцатеричные коды
    char oct_char = '\101'; // восьмеричный код (A)
    char hex_char = '\x41'; // шестнадцатеричный код (A)
    
    // СПОРНЫЕ символы - проверка обработки ошибок
    char error1 = 'ab';     // ОШИБКА! более одного символа
    char error2 = '';       // ОШИБКА! пустой символ
    
    // Строки
    string str1 = "Hello World";
    string str2 = "String with\nescapes\tand\rspecial\\chars";
    string str3 = "Hex: \x48\x65\x6C\x6C\x6F"; // Hello в hex
    string str4 = "Octal: \110\145\154\154\157"; // Hello в восьмеричном
    
    // Строки с форматированием
    string fmt1 = "Number: %d";
    string fmt2 = "Float: %f";
    string fmt3 = "String: %s";
    string fmt4 = "Char: %c";
    
    // СПОРНЫЕ строки
    string empty = "";      // пустая строка
    string quote = "String with \" quote inside";
    
    // ТЕСТ ОПЕРАТОРОВ
    
    // Арифметические
    int result = a + b - c * d / e;
    
    // Присваивания
    x = 5;
    x += 10;
    x -= 3;
    x *= 2;
    x /= 4;
    
    // Сравнения
    bool cmp1 = (a == b);
    bool cmp2 = (a != b);
    bool cmp3 = (a < b);
    bool cmp4 = (a <= b);
    bool cmp5 = (a > b);
    bool cmp6 = (a >= b);
    
    // Логические
    bool logic1 = flag1 && flag2;
    bool logic2 = flag1 || flag2;
    bool logic3 = !flag1;
    
    // Доступ к членам и диапазоны
    obj.member = value;
    int range = 1..10;      // СПОРНЫЙ! диапазон
    
    // РАЗДЕЛИТЕЛИ И СКОБКИ
    array[index] = {element1, element2};
    function(param1, param2);
    
    label: statement;
    
    // ТЕСТ ИДЕНТИФИКАТОРОВ
    
    // Обычные идентификаторы
    int variable;
    int _private;
    int snake_case_name;
    int camelCaseName;
    int name123;
    int _123invalid;        // начинается с подчеркивания - валидно
    
    // СПОРНЫЕ идентификаторы
    int класс;              // Unicode символы - зависит от реализации
    int α_β_γ;              // греческие буквы
    
    // Граничные случаи для проверки приоритета правил
    int ifvar;              // содержит "if" но не является ключевым словом
    int class_name;         // содержит "class"
    int string_value;       // содержит "string"
    
    // Тест комбинированных токенов
    if(x>=0&&y<=100||z!=null) { // слитные операторы
        // проверка разбиения
    }
    
    // Числа рядом с операторами - спорные случаи
    result = 5.0+3.14-2.7*1.5/0.5; // слитно
    
    return 0;
}

// Тест незакрытых конструкций для проверки обработки ошибок
/* Этот комментарий не закрыт...
string unclosed = "This string is not closed...
char bad_char = 'multiple chars';
