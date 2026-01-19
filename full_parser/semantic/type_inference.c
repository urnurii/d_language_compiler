#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "type_inference.h"
#include "error_reporting.h"

/* ============================================================================
   ВЫВОД ТИПОВ ВЫРАЖЕНИЙ
   ============================================================================ */

NType* InferExpressionType(NExpr *expr, SemanticContext *ctx) {
    /* TODO: Вычислить тип выражения
       - Если expr == NULL - вернуть NULL
       - В зависимости от expr->expr_type вызвать нужную функцию:
         - EXPR_INT, EXPR_FLOAT, EXPR_CHAR, EXPR_STRING, EXPR_BOOL, EXPR_NULL, EXPR_NAN:
           InferLiteralType
         - EXPR_IDENT: найти в таблице символов, вернуть тип переменной
         - EXPR_BINARY_OP: InferBinaryOperationType
         - EXPR_UNARY_OP: InferUnaryOperationType
         - EXPR_FUNC_CALL: найти функцию, вернуть её возвращаемый тип
         - EXPR_METHOD_CALL: найти метод, вернуть его тип
         - EXPR_MEMBER_ACCESS: найти поле, вернуть его тип
         - EXPR_ARRAY_ACCESS: найти тип массива, вернуть тип элемента
         - EXPR_NEW: вернуть тип класса
         - EXPR_PAREN: вернуть тип выражения в скобках
         - EXPR_THIS: вернуть тип текущего класса
         - Другое: сообщить ошибку и вернуть NULL
       - Использует: CopyType для сохранения типа (нельзя возвращать указатель на временный тип)
       Ошибки: undefined variable, undefined function, type mismatch */
    return NULL;
}

NType* InferLiteralType(NExpr *expr) {
    /* TODO: Вычислить тип литерального значения
       - В зависимости от expr->expr_type:
         - EXPR_INT: CreateBaseType(TYPE_INT)
         - EXPR_FLOAT: CreateBaseType(TYPE_FLOAT)
         - EXPR_CHAR: CreateBaseType(TYPE_CHAR)
         - EXPR_STRING: CreateBaseType(TYPE_STRING)
         - EXPR_BOOL: CreateBaseType(TYPE_BOOL)
         - EXPR_NULL: CreateClassType(NULL) или специальный тип
         - EXPR_NAN: CreateBaseType(TYPE_FLOAT)
       - Возвращает: новый NType объект */
    return NULL;
}

NType* InferBinaryOperationType(OpType op, NType *left_type, NType *right_type) {
    /* TODO: Вычислить тип результата бинарной операции
       - Для арифметических операций (+, -, *, /):
         - int + int -> int
         - float + float -> float
         - int + float -> float (проверить совместимость и вернуть float)
         - Другие комбинации -> ошибка
       - Для сравнений и логических операций (==, !=, <, >, <=, >=, &&, ||):
         - результат всегда bool
       - Для присваивания (=, +=, -=, *=, /=):
         - результат тип левого операнда (если совместимо)
       - Для битовых операций (если поддерживаются): int only
       
       Возвращает: новый NType для результата или NULL если операция неправильная
       Использует: AreTypesCompatible, CreateBaseType, CopyType */
    return NULL;
}

NType* InferUnaryOperationType(OpType op, NType *operand_type) {
    /* TODO: Вычислить тип результата унарной операции
       - Для унарного минуса (-expr):
         - -int -> int
         - -float -> float
         - Другие типы -> ошибка
       - Для унарного плюса (+expr):
         - +int -> int
         - +float -> float
       - Для логического НЕ (!expr):
         - !любой_тип -> bool
       
       Возвращает: новый NType для результата или NULL если операция неправильная
       Использует: CreateBaseType, CopyType */
    return NULL;
}

/* ============================================================================
   ПРОВЕРКА СОВМЕСТИМОСТИ ТИПОВ
   ============================================================================ */

int AreTypesCompatible(NType *type1, NType *type2, int strict) {
    /* TODO: Проверить совместимость двух типов
       - Если strict == 1: типы должны быть идентичны (TypesEqual)
       - Если strict == 0: допускаются преобразования:
         - int совместим с float (для чтения)
         - float совместим с int только если нет потери данных (в строгом смысле - нет)
         - Одинаковые типы всегда совместимы
         - Для классов: учитывать наследование (если тип1 - базовый класс тип2)
       
       Возвращает: 1 если совместимы, 0 если нет
       Использует: TypesEqual, IsNumericType */
    return 0;
}

int CanAssign(NType *target_type, NType *source_type) {
    /* TODO: Проверить, может ли source_type быть присвоен target_type
       - Правила D:
         - Одинаковые типы: да
         - source = int, target = float: да
         - source = float, target = int: нет (потеря данных)
         - source = производный класс, target = базовый класс: да
         - source = NULL, target = класс: да
         - Другие комбинации: нет
       
       Возвращает: 1 если можно, 0 если нет
       Использует: TypesEqual, IsIntegralType, IsFloatingPointType */
    return 0;
}

int IsArgumentCompatibleWithParameter(NType *param_type, NType *arg_type, int is_ref) {
    /* TODO: Проверить совместимость аргумента с параметром
       - Если is_ref == 1: требуется точное совпадение типов
       - Если is_ref == 0: допускаются преобразования (AreTypesCompatible)
       
       Возвращает: 1 если совместимы, 0 если нет
       Использует: TypesEqual, AreTypesCompatible */
    return 0;
}

/* ============================================================================
   СПЕЦИАЛЬНЫЕ ПРОВЕРКИ ТИПОВ
   ============================================================================ */

int IsNumericType(NType *type) {
    /* TODO: Проверить, является ли тип числовым
       - Числовые типы: TYPE_INT, TYPE_FLOAT, TYPE_DOUBLE, TYPE_REAL, TYPE_CHAR
       - Вернуть 1 если числовой, 0 если нет */
    return 0;
}

int IsIntegralType(NType *type) {
    /* TODO: Проверить, является ли тип целым числом
       - Целые типы: TYPE_INT, TYPE_CHAR
       - Вернуть 1 если целый, 0 если нет */
    return 0;
}

int IsFloatingPointType(NType *type) {
    /* TODO: Проверить, является ли тип с плавающей точкой
       - Типы: TYPE_FLOAT, TYPE_DOUBLE, TYPE_REAL
       - Вернуть 1 если float, 0 если нет */
    return 0;
}

int IsBooleanType(NType *type) {
    /* TODO: Проверить, является ли тип булевым
       - Тип: TYPE_BOOL
       - Вернуть 1 если bool, 0 если нет */
    return 0;
}

int CanBeArrayIndex(NType *type) {
    /* TODO: Проверить, может ли тип использоваться как индекс массива
       - Индекс должен быть целым числом: TYPE_INT, TYPE_CHAR
       - Вернуть 1 если может, 0 если нет
       Использует: IsIntegralType */
    return 0;
}

int CanBeCondition(NType *type) {
    /* TODO: Проверить, может ли тип использоваться как условие
       - Условие должно быть boolean или числовым типом
       - TYPE_BOOL, TYPE_INT, TYPE_CHAR, TYPE_FLOAT и т.д.
       - Вернуть 1 если может, 0 если нет
       Использует: IsBooleanType, IsNumericType */
    return 0;
}

/* ============================================================================
   СОЗДАНИЕ И КОПИРОВАНИЕ ТИПОВ
   ============================================================================ */

NType* CopyType(NType *type) {
    /* TODO: Создать глубокую копию типа
       - Если type == NULL - вернуть NULL
       - Выделить память для новой NType
       - Скопировать все поля (base_type, kind, тип класса, размерность массива)
       - Если это класс - скопировать имя класса
       - Если это массив - скопировать информацию о размере
       - Вернуть новый NType или NULL при ошибке памяти */
    return NULL;
}

int TypesEqual(NType *type1, NType *type2) {
    /* TODO: Проверить, идентичны ли два типа
       - Если оба NULL - равны
       - Если один NULL, другой нет - не равны
       - Проверить base_type
       - Проверить kind (является ли классом)
       - Если класс - проверить имена классов (strcmp)
       - Проверить размерность массива (array_size, is_array)
       - Вернуть 1 если идентичны, 0 если нет */
    return 0;
}

const char* TypeToString(NType *type) {
    /* TODO: Преобразовать тип в строку для вывода ошибок
       - Если type == NULL - вернуть "unknown"
       - Для базовых типов: "int", "float", "string", "char", "bool", "double", "real", "void"
       - Для классов: имя класса
       - Для массивов: добавить "[]" в конец (например, "int[]", "MyClass[]")
       - Использовать статическую переменную или malloc для результата
       - Вернуть указатель на строку (не должна быть освобождена пользователем) */
    return "unknown";
}
