#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "semantic_passes.h"
#include "name_resolution.h"
#include "type_inference.h"
#include "error_reporting.h"

/* ============================================================================
   ПЕРВЫЙ ПРОХОД: СБОР ВСЕХ ДЕКЛАРАЦИЙ
   ============================================================================ */

int ProcessSourceItems(NProgram *root, SemanticContext *ctx) {
    /* TODO: Обойти все элементы верхнего уровня программы
       - Если root == NULL - вернуть 0
       - Пройти по root->items (это может быть связный список или массив - проверить структуру)
       - Для каждого source_item:
         - Если тип FUNC - вызвать ProcessFunctionDefinition
         - Если тип CLASS - вызвать ProcessClassDefinition
         - Если тип DECL - вызвать ProcessGlobalVariables
         - Если тип ENUM - вызвать ProcessEnumDefinition
       - Собрать результаты всех обработок
       - Вернуть 0 если все успешно, 1 если были ошибки
       Использует: ProcessFunctionDefinition, ProcessClassDefinition,
                   ProcessGlobalVariables, ProcessEnumDefinition */
    return 0;
}

int ProcessFunctionDefinition(NFuncDef *func_def, SemanticContext *ctx) {
    /* TODO: Обработать определение функции
       - Если func_def == NULL - вернуть 0
       - Создать FunctionInfo структуру с информацией из func_def
       - Заполнить: name, return_type, params, line, column
       - Если return_type != NULL: проверить что тип существует (если пользовательский класс)
       - Вызвать AddFunctionToContext
       - Вернуть результат AddFunctionToContext
       Использует: AddFunctionToContext, LookupClass (для проверки типов)
       Ошибки: дублирование, неопределённый возвращаемый тип */
    return 0;
}

int ProcessClassDefinition(NClassDef *class_def, SemanticContext *ctx) {
    /* TODO: Обработать определение класса
       - Если class_def == NULL - вернуть 0
       - Создать ClassInfo структуру с информацией из class_def
       - Заполнить: name, base_class (если есть), line, column
       - Если base_class указан: проверить что класс существует (LookupClass)
         - Если не существует: добавить ошибку CreateInvalidBaseClassError и вернуть 1
       - Вызвать ProcessClassMembers для обработки полей и методов
       - Вызвать AddClassToContext
       - Вернуть результат AddClassToContext
       Использует: ProcessClassMembers, AddClassToContext, LookupClass
       Ошибки: дублирование, неопределённый базовый класс */
    return 0;
}

int ProcessClassMembers(NClassMember *members, ClassInfo *class_info, SemanticContext *ctx) {
    /* TODO: Обработать членов класса
       - Если members == NULL - вернуть 0
       - Пройти по связному списку members
       - Для каждого члена:
         - Если тип FIELD - вызвать ProcessFieldDeclaration
         - Если тип METHOD - вызвать ProcessMethodDefinition
         - Если тип CTOR - обработать конструктор (сохранить в class_info->constructor)
         - Если тип DTOR - обработать деструктор (сохранить в class_info->destructor)
       - Вернуть 0 если все успешно, 1 если были ошибки
       Использует: ProcessFieldDeclaration, ProcessMethodDefinition */
    return 0;
}

int ProcessFieldDeclaration(NInitDeclList *init_decls, NType *field_type, 
                           AccessSpec access, ClassInfo *class_info, SemanticContext *ctx) {
    /* TODO: Обработать объявление поля класса
       - Если field_type == NULL - вернуть 1 (ошибка)
       - Если field_type это пользовательский класс: проверить что класс существует
       - Для каждого объявления в init_decls:
         - Создать FieldInfo структуру
         - Заполнить: name, type, access, line, column
         - Добавить в class_info->fields[]
       - Вернуть 0 если успешно
       Использует: LookupClass (для проверки типов)
       Ошибки: неопределённый тип */
    return 0;
}

int ProcessMethodDefinition(NMethodDef *method_def, AccessSpec access, 
                           ClassInfo *class_info, SemanticContext *ctx) {
    /* TODO: Обработать определение метода класса
       - Если method_def == NULL - вернуть 0
       - Создать MethodInfo структуру с информацией из method_def
       - Заполнить: name, return_type, params, access, is_override, line, column
       - Если is_override == 1:
         - Если base_class не указан - добавить ошибку и вернуть 1
         - Найти base_class в контексте (LookupClass)
         - Если не найден - ошибка и вернуть 1
         - Проверить что в base_class есть метод с таким же именем (LookupClassMethod)
         - Проверить что типы совпадают (return type и параметры)
       - Добавить в class_info->methods[]
       - Вернуть 0 если успешно
       Использует: LookupClass, LookupClassMethod
       Ошибки: invalid override */
    return 0;
}

int ProcessEnumDefinition(NEnumDef *enum_def, SemanticContext *ctx) {
    /* TODO: Обработать определение enum
       - Если enum_def == NULL - вернуть 0
       - Создать EnumInfo структуру с информацией из enum_def
       - Заполнить: name (может быть NULL), items[], item_count, line, column
       - Для каждого элемента enum'а: создать EnumItemInfo
       - Проверить на дублирование имён элементов enum'а
       - Вызвать AddEnumToContext
       - Вернуть результат AddEnumToContext
       Использует: AddEnumToContext
       Ошибки: дублирование элементов */
    return 0;
}

int ProcessGlobalVariables(NType *type, NInitDeclList *init_decls, SemanticContext *ctx) {
    /* TODO: Обработать глобальные переменные
       - Если type == NULL - вернуть 1 (ошибка)
       - Если type это пользовательский класс: проверить что класс существует
       - Для каждого объявления в init_decls:
         - Создать VariableInfo структуру
         - Заполнить: name, type, is_initialized, line, column
         - Вызвать AddLocalVariable (в глобальной области)
       - Вернуть 0 если успешно
       Использует: AddLocalVariable, LookupClass
       Ошибки: неопределённый тип, дублирование */
    return 0;
}

/* ============================================================================
   ВТОРОЙ ПРОХОД: ПРОВЕРКА СЕМАНТИКИ
   ============================================================================ */

int CheckAllSemantics(NProgram *root, SemanticContext *ctx) {
    /* TODO: Главная функция второго прохода
       - Если root == NULL - вернуть 0
       - Пройти по всем источникам верхнего уровня
       - Проверить:
         - Инициализаторы глобальных переменных
         - Тела функций
         - Инициализаторы полей в классах
       - Вернуть 0 если ошибок нет, 1 если есть
       Использует: CheckSourceItems */
    return 0;
}

int CheckSourceItems(NSourceItem *items, SemanticContext *ctx) {
    /* TODO: Проверить элементы верхнего уровня
       - Если items == NULL - вернуть 0
       - Пройти по связному списку items
       - Для каждого элемента:
         - Если тип FUNC: вызвать CheckFunctionBody для тела функции
         - Если тип CLASS: пройти по полям класса, проверить инициализаторы
         - Если тип DECL: проверить инициализаторы переменных
       - Вернуть 0 если ошибок нет
       Использует: CheckFunctionBody, CheckExpressions */
    return 0;
}

int CheckFunctionBody(NStmt *func_body, FunctionInfo *func_info, SemanticContext *ctx) {
    /* TODO: Проверить тело функции
       - Если func_body == NULL - вернуть 0 (абстрактная функция)
       - Создать новый scope для функции (PushScope)
       - Добавить параметры функции в локальную таблицу
       - Проверить все операторы в теле (CheckStatements)
       - Вернуться к глобальному scope (PopScope)
       - Вернуть результат проверки
       Использует: PushScope, PopScope, CheckStatements, AddLocalVariable */
    return 0;
}

int CheckStatements(NStmt *stmts, SemanticContext *ctx) {
    /* TODO: Проверить список операторов
       - Если stmts == NULL - вернуть 0
       - Пройти по связному списку операторов
       - Для каждого оператора вызвать CheckStatement
       - Вернуть 0 если ошибок нет, 1 если есть
       Использует: CheckStatement */
    return 0;
}

int CheckStatement(NStmt *stmt, SemanticContext *ctx) {
    /* TODO: Проверить один оператор
       - Если stmt == NULL - вернуть 0
       - В зависимости от типа оператора:
         - STMT_EXPR: проверить выражение (CheckExpressions)
         - STMT_DECL: проверить объявления переменных и их инициализаторы
         - STMT_IF: проверить условие и ветви (рекурсивно)
         - STMT_WHILE, STMT_DO_WHILE, STMT_FOR, STMT_FOREACH: проверить условие и тело
         - STMT_SWITCH: проверить выражение и case'ы
         - STMT_RETURN: проверить возвращаемое значение (если есть)
         - STMT_BREAK, STMT_CONTINUE: просто OK (вне switch/loop проверяется отдельно)
         - STMT_COMPOUND: проверить все операторы в блоке
       - Вернуть 0 если ошибок нет
       Использует: CheckExpressions, CheckStatements */
    return 0;
}

int CheckExpressions(NExpr *root, SemanticContext *ctx) {
    /* TODO: Проверить все выражения в дереве
       - Если root == NULL - вернуть 0
       - Рекурсивно пройти по дереву выражений
       - Для каждого узла вызвать CheckExpression
       - Вернуть 0 если ошибок нет
       Использует: CheckExpression */
    return 0;
}

int CheckExpression(NExpr *expr, SemanticContext *ctx) {
    /* TODO: Проверить одно выражение
       - Если expr == NULL - вернуть 0
       - В зависимости от типа выражения:
         - EXPR_IDENT: разрешить имя (LookupSymbol), проверить доступность
         - EXPR_FUNC_CALL: найти функцию, проверить количество и типы аргументов
         - EXPR_METHOD_CALL: найти метод, проверить аргументы
         - EXPR_MEMBER_ACCESS: найти поле, проверить доступность
         - EXPR_ARRAY_ACCESS: проверить что левый операнд массив, индекс целый
         - EXPR_BINARY_OP, EXPR_UNARY_OP: проверить совместимость типов операндов
         - Для всех типов: рекурсивно проверить подвыражения
       - Вернуть 0 если ошибок нет
       Использует: LookupSymbol, LookupFunction, LookupClassMethod,
                   CheckExpressions, CheckExpression (рекурсивно)
       Ошибки: undefined variable, function not found, type mismatch */
    return 0;
}

int IsSymbolAccessible(Symbol *symbol, SemanticContext *ctx) {
    /* TODO: Проверить доступность символа из текущего контекста
       - Если symbol == NULL - вернуть 0
       - Если символ глобальный (scope_depth == 0) - обычно доступен
       - Если символ локальный - проверить что текущая область это его область или родительская
       - Для членов класса: проверить private/protected (с помощью IsFieldAccessible)
       - Вернуть 1 если доступен, 0 если нет */
    return 1;
}

/* ============================================================================
   ТРЕТИЙ ПРОХОД: АТРИБУТИРОВАНИЕ
   ============================================================================ */

int AttributeExpressions(NExpr *root, SemanticContext *ctx) {
    /* TODO: Атрибутировать выражения типами
       - Если root == NULL - вернуть 0
       - Рекурсивно пройти по дереву выражений
       - Для каждого узла:
         - Вычислить тип (InferExpressionType)
         - Сохранить тип в узел выражения (если в структуре есть поле для типа)
       - Вернуть 0 если успешно
       Использует: InferExpressionType
       Примечание: может потребоваться расширение структуры NExpr для хранения типа */
    return 0;
}

int AttributeStatements(NStmt *stmts, SemanticContext *ctx) {
    /* TODO: Атрибутировать операторы информацией о scope
       - Если stmts == NULL - вернуть 0
       - Рекурсивно пройти по дереву операторов
       - Для каждого узла:
         - Сохранить текущий scope в узел оператора
       - Вернуть 0 если успешно
       Примечание: может потребоваться расширение структуры NStmt для хранения scope */
    return 0;
}
