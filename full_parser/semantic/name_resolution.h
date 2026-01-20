#ifndef NAME_RESOLUTION_H
#define NAME_RESOLUTION_H

#include "semantic_types.h"

/* ============================================================================
   УПРАВЛЕНИЕ ТАБЛИЦАМИ СИМВОЛОВ И РАЗРЕШЕНИЕ ИМЁН
   ============================================================================ */

/* СОЗДАНИЕ И ОЧИСТКА БАЗОВЫХ СТРУКТУР */

SymbolTable* CreateSymbolTable(int initial_capacity);
void DestroySymbolTable(SymbolTable *table);

ScopeStack* CreateScopeStack(SymbolTable *global_table);
void DestroyScopeStack(ScopeStack *stack);

Scope* CreateScope(const char *scope_name, int depth, Scope *parent);
void DestroyScope(Scope *scope);

/* УПРАВЛЕНИЕ ТАБЛИЦЕЙ СИМВОЛОВ */

/* Добавить символ в глобальную таблицу
   Проверяет на дублирование в текущей области видимости
   Возвращает: 0 если успешно, 1 если дублирование */
int AddSymbolToTable(SemanticContext *ctx, const Symbol *symbol);

/* Найти символ в таблице, учитывая текущий scope
   Сначала ищет в локальной области, потом в родительских
   Возвращает: указатель на Symbol или NULL если не найден */
Symbol* LookupSymbol(SemanticContext *ctx, const char *name);

/* Найти символ ТОЛЬКО в глобальной таблице
   Используется при проверке глобальных деклараций
   Возвращает: указатель на Symbol или NULL если не найден */
Symbol* LookupGlobalSymbol(SemanticContext *ctx, const char *name);

/* УПРАВЛЕНИЕ ОБЛАСТЯМИ ВИДИМОСТИ */

/* Создать новую область видимости (например, при входе в функцию)
   scope_name: имя области (имя функции, имя класса и т.д.)
   
   Использует: CreateScope
   Результат: новая область видимости в стеке */
int PushScope(SemanticContext *ctx, const char *scope_name);

/* Вернуться к родительской области видимости (при выходе из функции)
   
   Результат: старая область удаляется из стека */
int PopScope(SemanticContext *ctx);

/* Получить текущую область видимости */
Scope* GetCurrentScope(SemanticContext *ctx);

/* ДОБАВЛЕНИЕ ИНФОРМАЦИИ В КОНТЕКСТ */

/* Добавить информацию о функции в контекст
   Проверяет дублирование функций
   Создаёт запись для быстрого поиска
   Возвращает: 0 если успешно, 1 если ошибка */
int AddFunctionToContext(SemanticContext *ctx, FunctionInfo *func_info);

/* Добавить информацию о классе в контекст
   Проверяет, что базовый класс существует (если указан)
   Добавляет информацию о полях и методах класса
   Возвращает: 0 если успешно, 1 если ошибка */
int AddClassToContext(SemanticContext *ctx, ClassInfo *class_info);

/* Добавить информацию об enum в контекст
   Проверяет на дублирование элементов enum
   Возвращает: 0 если успешно, 1 если ошибка */
int AddEnumToContext(SemanticContext *ctx, EnumInfo *enum_info);

/* Добавить локальную переменную в текущий scope
   Проверяет на дублирование в текущей области
   Возвращает: 0 если успешно, 1 если ошибка */
int AddLocalVariable(SemanticContext *ctx, VariableInfo *var_info);

/* ПОИСК ИНФОРМАЦИИ ПО ИМЕНИ */

/* Найти информацию о функции по имени
   Возвращает: указатель на FunctionInfo или NULL если не найдена */
FunctionInfo* LookupFunction(SemanticContext *ctx, const char *name);

/* Найти информацию о классе по имени
   Возвращает: указатель на ClassInfo или NULL если класс не найден */
ClassInfo* LookupClass(SemanticContext *ctx, const char *name);

/* Найти информацию об enum по имени
   Возвращает: указатель на EnumInfo или NULL если enum не найден */
EnumInfo* LookupEnum(SemanticContext *ctx, const char *name);

/* Найти поле класса по имени
   class_info: информация о классе
   field_name: имя поля для поиска
   Возвращает: указатель на FieldInfo или NULL если поле не найдено */
FieldInfo* LookupClassField(ClassInfo *class_info, const char *field_name);

/* Найти метод класса по имени
   class_info: информация о классе
   method_name: имя метода для поиска
   Возвращает: указатель на MethodInfo или NULL если метод не найден */
MethodInfo* LookupClassMethod(ClassInfo *class_info, const char *method_name);

/* Найти элемент enum по имени
   enum_info: информация об enum
   item_name: имя элемента для поиска
   Возвращает: указатель на EnumItemInfo или NULL если элемент не найден */
EnumItemInfo* LookupEnumItem(EnumInfo *enum_info, const char *item_name);

/* ПРОВЕРКА ДОСТУПА */

/* Проверить, доступно ли поле/метод класса из текущего контекста
   Учитывает: public/private/protected, контекст вызова (внутри класса или снаружи)
   Возвращает: 1 если доступно, 0 если нет */
int IsFieldAccessible(FieldInfo *field, int inside_class);

int IsMethodAccessible(MethodInfo *method, int inside_class);

#endif /* NAME_RESOLUTION_H */
