#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "name_resolution.h"
#include "error_reporting.h"

/* ============================================================================
   УПРАВЛЕНИЕ ТАБЛИЦЕЙ СИМВОЛОВ
   ============================================================================ */

int AddSymbolToTable(SemanticContext *ctx, const Symbol *symbol) {
    /* TODO: Добавить символ в таблицу
       - Проверить, что символ не дублируется в текущей области
       - Если дублируется - добавить ошибку и вернуть 1
       - Выделить память для копии символа
       - Скопировать символ в таблицу
       - Увеличить счётчик
       - Вернуть 0
       Использует: LookupSymbol (для проверки дублирования),
                   CreateDuplicateSymbolError, AddError */
    return 0;
}

Symbol* LookupSymbol(SemanticContext *ctx, const char *name) {
    /* TODO: Найти символ в таблице с учётом scope
       - Начать с текущего scope
       - Если не найдено - перейти к родительскому scope
       - Продолжать пока не найдём или не закончатся области
       - Вернуть найденный Symbol или NULL
       Использует: GetCurrentScope */
    return NULL;
}

Symbol* LookupGlobalSymbol(SemanticContext *ctx, const char *name) {
    /* TODO: Найти символ ТОЛЬКО в глобальной таблице
       - Пройти по ctx->global_symbols->symbols
       - Найти символ с нужным именем
       - Вернуть найденный Symbol или NULL */
    return NULL;
}

/* ============================================================================
   УПРАВЛЕНИЕ ОБЛАСТЯМИ ВИДИМОСТИ
   ============================================================================ */

int PushScope(SemanticContext *ctx, const char *scope_name) {
    /* TODO: Создать новую область видимости
       - Выделить память для новой Scope
       - Установить depth = текущий depth + 1
       - Установить scope_name
       - Создать новую таблицу символов для локальных переменных
       - Установить parent на текущую область
       - Добавить в стек ctx->scope_stack
       - Вернуть 0 если успешно, 1 при ошибке памяти */
    return 0;
}

int PopScope(SemanticContext *ctx) {
    /* TODO: Вернуться к родительской области
       - Если это не глобальная область (depth > 0)
       - Удалить текущую область из стека
       - Освободить её память
       - Вернуть 0 если успешно, 1 если ошибка */
    return 0;
}

Scope* GetCurrentScope(SemanticContext *ctx) {
    /* TODO: Получить текущую область видимости
       - Вернуть последний элемент в ctx->scope_stack->scopes
       - Или NULL если стек пуст */
    return NULL;
}

/* ============================================================================
   ДОБАВЛЕНИЕ ИНФОРМАЦИИ В КОНТЕКСТ
   ============================================================================ */

int AddFunctionToContext(SemanticContext *ctx, FunctionInfo *func_info) {
    /* TODO: Добавить функцию в контекст
       - Проверить, что функция не дублируется (LookupGlobalSymbol)
       - Если дублируется - добавить ошибку и вернуть 1
       - Создать Symbol с типом SYMBOL_FUNCTION
       - Добавить в глобальную таблицу (AddSymbolToTable)
       - Добавить в ctx->functions для быстрого доступа
       - Вернуть 0 если успешно
       Использует: CreateDuplicateSymbolError, AddSymbolToTable */
    return 0;
}

int AddClassToContext(SemanticContext *ctx, ClassInfo *class_info) {
    /* TODO: Добавить класс в контекст
       - Проверить, что класс не дублируется
       - Если базовый класс указан - проверить что он существует (LookupClass)
       - Если базовый класс не найден - добавить ошибку и вернуть 1
       - Создать Symbol с типом SYMBOL_CLASS
       - Добавить в глобальную таблицу
       - Добавить в ctx->classes для быстрого доступа
       - Вернуть 0 если успешно
       Использует: CreateDuplicateSymbolError, CreateInvalidBaseClassError,
                   AddSymbolToTable, LookupClass */
    return 0;
}

int AddEnumToContext(SemanticContext *ctx, EnumInfo *enum_info) {
    /* TODO: Добавить enum в контекст
       - Если enum имеет имя - проверить что оно не дублируется
       - Проверить, что элементы enum'а не дублируются
       - Если дублирование - добавить ошибку и вернуть 1
       - Создать Symbol с типом SYMBOL_ENUM_TYPE
       - Добавить в глобальную таблицу
       - Добавить в ctx->enums для быстрого доступа
       - Вернуть 0 если успешно
       Использует: CreateDuplicateSymbolError, CreateEnumDuplicateItemError,
                   AddSymbolToTable */
    return 0;
}

int AddLocalVariable(SemanticContext *ctx, VariableInfo *var_info) {
    /* TODO: Добавить локальную переменную в текущий scope
       - Получить текущий scope (GetCurrentScope)
       - Проверить, что переменная не дублируется в текущей области
       - Если дублируется - добавить ошибку и вернуть 1
       - Создать Symbol с типом SYMBOL_VARIABLE
       - Добавить в локальную таблицу текущего scope
       - Вернуть 0 если успешно
       Использует: GetCurrentScope, CreateDuplicateSymbolError, AddSymbolToTable */
    return 0;
}

/* ============================================================================
   ПОИСК ИНФОРМАЦИИ ПО ИМЕНИ
   ============================================================================ */

FunctionInfo* LookupFunction(SemanticContext *ctx, const char *name) {
    /* TODO: Найти функцию по имени
       - Пройти по ctx->functions
       - Найти функцию с нужным именем
       - Вернуть найденную FunctionInfo или NULL */
    return NULL;
}

ClassInfo* LookupClass(SemanticContext *ctx, const char *name) {
    /* TODO: Найти класс по имени
       - Пройти по ctx->classes
       - Найти класс с нужным именем
       - Вернуть найденный ClassInfo или NULL */
    return NULL;
}

EnumInfo* LookupEnum(SemanticContext *ctx, const char *name) {
    /* TODO: Найти enum по имени
       - Пройти по ctx->enums
       - Найти enum с нужным именем
       - Вернуть найденный EnumInfo или NULL */
    return NULL;
}

FieldInfo* LookupClassField(ClassInfo *class_info, const char *field_name) {
    /* TODO: Найти поле класса по имени
       - Пройти по class_info->fields
       - Найти поле с нужным именем
       - Вернуть найденное FieldInfo или NULL */
    return NULL;
}

MethodInfo* LookupClassMethod(ClassInfo *class_info, const char *method_name) {
    /* TODO: Найти метод класса по имени
       - Пройти по class_info->methods
       - Найти метод с нужным именем
       - Вернуть найденный MethodInfo или NULL */
    return NULL;
}

EnumItemInfo* LookupEnumItem(EnumInfo *enum_info, const char *item_name) {
    /* TODO: Найти элемент enum по имени
       - Пройти по enum_info->items
       - Найти элемент с нужным именем
       - Вернуть найденный EnumItemInfo или NULL */
    return NULL;
}

/* ============================================================================
   ПРОВЕРКА ДОСТУПА
   ============================================================================ */

int IsFieldAccessible(FieldInfo *field, int inside_class) {
    /* TODO: Проверить доступность поля класса
       - Если поле public - всегда доступно
       - Если поле private - доступно только изнутри класса
       - Если поле protected - доступно изнутри класса и наследников (упрощённо: только изнутри)
       - Вернуть 1 если доступно, 0 если нет
       Использует: inside_class (1 если код находится внутри класса) */
    return 1;
}

int IsMethodAccessible(MethodInfo *method, int inside_class) {
    /* TODO: Проверить доступность метода класса
       - Та же логика как для полей
       - Вернуть 1 если доступно, 0 если нет */
    return 1;
}
