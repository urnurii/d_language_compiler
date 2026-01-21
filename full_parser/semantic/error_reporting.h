#ifndef ERROR_REPORTING_H
#define ERROR_REPORTING_H

#include "semantic_types.h"

// ----- Система сообщений ошибкаих и отладки -----

/* СОЗДАНИЕ И УПРАВЛЕНИЕ СПИСКОМ ОШИБОК */

/* Создать пустой список для сбора ошибок
   
   Возвращает: новый ErrorList или NULL при ошибке памяти */
ErrorList* CreateErrorList(void);

/* Добавить ошибку в список
   Функция копирует сообщение об ошибке
   
   error_list: список для добавления
   error: структура ошибки с информацией
   
   Возвращает: 0 если успешно, 1 при ошибке памяти */
int AddError(ErrorList *error_list, const SemanticError *error);

/* Уничтожить список ошибок и освободить память
   
   error_list: список для удаления */
void DestroyErrorList(ErrorList *error_list);

/* СОЗДАНИЕ ОШИБОК */

/* Создать ошибку типа "undefined variable"
   
   Возвращает: инициализированная структура SemanticError */
SemanticError CreateUndefinedVariableError(const char *var_name, int line, int column);

/* Создать ошибку типа "undefined function"
   
   Возвращает: инициализированная структура SemanticError */
SemanticError CreateUndefinedFunctionError(const char *func_name, int line, int column);

/* Создать ошибку типа "undefined class"
   
   Возвращает: инициализированная структура SemanticError */
SemanticError CreateUndefinedClassError(const char *class_name, int line, int column);

/* Создать ошибку типа "undefined type"
   
   Возвращает: инициализированная структура SemanticError */
SemanticError CreateUndefinedTypeError(const char *type_name, int line, int column);

/* Создать ошибку типа "duplicate symbol"
   
   Возвращает: инициализированная структура SemanticError */
SemanticError CreateDuplicateSymbolError(const char *symbol_name, int line, int column);

/* Создать ошибку типа "type mismatch"
   expected_type: строка с ожидаемым типом (например, "int")
   actual_type: строка с фактическим типом (например, "string")
   context: контекст ошибки (например, "in assignment" или "as function argument")
   
   Возвращает: инициализированная структура SemanticError */
SemanticError CreateTypeMismatchError(const char *expected_type, const char *actual_type, 
                                      const char *context, int line, int column);

/* Создать ошибку типа "invalid operands for operator"
   op_name: строка с названием оператора (например, "+")
   operand1_type: тип первого операнда
   operand2_type: тип второго операнда (для унарных операций NULL)
   
   Возвращает: инициализированная структура SemanticError */
SemanticError CreateInvalidOperandsError(const char *op_name, const char *operand1_type, 
                                        const char *operand2_type, int line, int column);

/* Создать ошибку типа "wrong number of arguments"
   func_name: имя функции
   expected: ожидаемое количество аргументов
   actual: фактическое количество аргументов
   
   Возвращает: инициализированная структура SemanticError */
SemanticError CreateWrongArgCountError(const char *func_name, int expected, int actual, 
                                       int line, int column);

/* Создать ошибку типа "access violation"
   Возникает при попытке доступа к private/protected членам класса
   
   member_name: имя члена класса
   access_spec: спецификатор доступа (например, "private")
   
   Возвращает: инициализированная структура SemanticError */
SemanticError CreateAccessViolationError(const char *member_name, const char *access_spec, 
                                        int line, int column);

/* Создать ошибку типа "invalid array index"
   index_type: тип индекса, который не может использоваться
   
   Возвращает: инициализированная структура SemanticError */
SemanticError CreateInvalidArrayIndexError(const char *index_type, int line, int column);

/* Создать ошибку типа "invalid array size"
   Возникает при некорректном описании размера массива
   
   Возвращает: инициализированная структура SemanticError */
SemanticError CreateInvalidArraySizeError(const char *reason, int line, int column);

/* Создать ошибку типа "method not found"
   method_name: имя метода
   class_name: имя класса
   
   Возвращает: инициализированная структура SemanticError */
SemanticError CreateMethodNotFoundError(const char *method_name, const char *class_name, 
                                       int line, int column);

/* Создать ошибку типа "field not found"
   field_name: имя поля
   class_name: имя класса
   
   Возвращает: инициализированная структура SemanticError */
SemanticError CreateFieldNotFoundError(const char *field_name, const char *class_name, 
                                      int line, int column);

/* Создать ошибку типа "invalid base class"
   base_class_name: имя указанного базового класса
   
   Возвращает: инициализированная структура SemanticError */
SemanticError CreateInvalidBaseClassError(const char *base_class_name, int line, int column);

/* Создать ошибку типа "invalid override"
   method_name: имя переопределяемого метода
   reason: причина ошибки (например, "return type mismatch")
   
   Возвращает: инициализированная структура SemanticError */
SemanticError CreateInvalidOverrideError(const char *method_name, const char *reason, 
                                        int line, int column);

/* Создать ошибку типа "enum duplicate item"
   item_name: имя дублирующегося элемента enum
   
   Возвращает: инициализированная структура SemanticError */
SemanticError CreateEnumDuplicateItemError(const char *item_name, int line, int column);

/* Создать ошибку типа "constructor return type"
   Конструктор не должен иметь return type
   
   Возвращает: инициализированная структура SemanticError */
SemanticError CreateConstructorReturnTypeError(int line, int column);

/* Создать ошибку типа "out of scope"
   Возникает при использовании символа вне области видимости
   
   symbol_name: имя символа
   
   Возвращает: инициализированная структура SemanticError */
SemanticError CreateOutOfScopeError(const char *symbol_name, int line, int column);

/* Создать пользовательскую ошибку
   kind: вид ошибки
   message: текст сообщения об ошибке
   
   Возвращает: инициализированная структура SemanticError */
SemanticError CreateCustomError(SemanticErrorKind kind, const char *message, int line, int column);

// ----- Вывод ошибок -----

/* Вывести одну ошибку в stderr в стандартном формате
   "SEMANTIC ERROR at line X, column Y: message"
   
   error: ошибка для вывода */
void PrintError(const SemanticError *error);

/* Вывести все ошибки из списка
   
   error_list: список ошибок для вывода */
void PrintAllErrors(const ErrorList *error_list);

/* ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ */

/* Получить строковое описание вида ошибки
   kind: вид ошибки
   
   Возвращает: строка с названием вида ошибки */
const char* ErrorKindToString(SemanticErrorKind kind);

/* Получить количество ошибок в списке
   
   error_list: список ошибок
   
   Возвращает: количество ошибок */
int GetErrorCount(const ErrorList *error_list);

/* Проверить, есть ли критические ошибки (блокирующие компиляцию)
   Все ошибки считаются критическими
   
   error_list: список ошибок
   
   Возвращает: 1 если есть ошибки, 0 если список пуст */
int HasErrors(const ErrorList *error_list);

#endif /* ERROR_REPORTING_H */
