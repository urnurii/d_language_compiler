#ifndef SEMANTIC_PASSES_H
#define SEMANTIC_PASSES_H

#include "semantic_types.h"

// ----- Вспомогательные проходы -----

// ----- Первый проход: сбор всех объявлений

/* Обойти корневой уровень программы и собрать все деклараций
   Проходит по source_item верхнего уровня
   
   root: корневой узел программы
   ctx: контекст анализа для добавления символов
   
   Возвращает: 0 если успешно, 1 если найдены ошибки */
int ProcessSourceItems(NProgram *root, SemanticContext *ctx);

/* Обработать объявление функции
   Добавляет FunctionInfo в контекст
   Проверяет, что тип возвращаемого значения существует
   
   func_def: определение функции
   ctx: контекст анализа
   
   Использует: AddFunctionToContext, LookupClass (для проверки пользовательских типов)
   Ошибки: дублирование функции, неопределённый возвращаемый тип */
int ProcessFunctionDefinition(NFuncDef *func_def, SemanticContext *ctx);

/* Обработать определение класса
   Добавляет ClassInfo с информацией о полях, методах, конструкторе, деструкторе
   Проверяет, что все типы полей существуют
   Проверяет, что базовый класс существует (если указан)
   
   class_def: определение класса
   ctx: контекст анализа
   
   Использует: AddClassToContext, ProcessClassMembers, LookupClass
   Ошибки: дублирование класса, неопределённый базовый класс, неопределённый тип поля */
int ProcessClassDefinition(NClassDef *class_def, SemanticContext *ctx);

/* Обработать членов класса (поля, методы, конструктор, деструктор)
   Собирает информацию о полях и методах класса
   
   members: первый член класса (связный список)
   class_info: структура для заполнения информацией о классе
   ctx: контекст анализа
   
   Использует: ProcessFieldDeclaration, ProcessMethodDefinition
   Результат: заполняется fields[], methods[], constructor, destructor в class_info */
int ProcessClassMembers(NClassMember *members, ClassInfo *class_info, SemanticContext *ctx);

/* Обработать объявление поля класса
   Добавляет информацию в class_info->fields[]
   Проверяет, что тип поля существует
   
   init_decls: список объявлений переменных в поле
   field_type: тип поля
   access: спецификатор доступа
   class_info: структура класса для добавления информации
   ctx: контекст анализа
   
   Использует: LookupClass (для проверки пользовательских типов)
   Ошибки: неопределённый тип */
int ProcessFieldDeclaration(NInitDeclList *init_decls, NType *field_type, 
                           AccessSpec access, ClassInfo *class_info, SemanticContext *ctx);

/* Обработать определение метода класса
   Добавляет информацию в class_info->methods[]
   Проверяет, что возвращаемый тип существует
   Проверяет, что для override методов существует переопределяемый метод в базовом классе
   
   method_def: определение метода
   access: спецификатор доступа
   class_info: структура класса для добавления информации
   ctx: контекст анализа
   
   Использует: AddMethodToClassInfo, LookupClass, LookupClassMethod (для override)
   Ошибки: неопределённый возвращаемый тип, invalid override */
int ProcessMethodDefinition(NMethodDef *method_def, AccessSpec access, 
                           ClassInfo *class_info, SemanticContext *ctx);

/* Обработать определение enum
   Добавляет EnumInfo в контекст
   Проверяет на дублирование имён элементов enum
   
   enum_def: определение enum
   ctx: контекст анализа
   
   Использует: AddEnumToContext
   Ошибки: дублирование элементов enum */
int ProcessEnumDefinition(NEnumDef *enum_def, SemanticContext *ctx);

/* Обработать глобальную переменную
   Добавляет переменную в глобальную таблицу
   Проверяет, что тип переменной существует
   
   type: тип переменной
   init_decls: список объявлений переменных
   ctx: контекст анализа
   
   Использует: AddLocalVariable (в глобальной области), LookupClass
   Ошибки: неопределённый тип, дублирование переменной */
int ProcessGlobalVariables(NType *type, NInitDeclList *init_decls, SemanticContext *ctx);

//  ----- Второй прозод: Проверка семантики атрибутирование имён -----

/* Обойти всё дерево и проверить использование всех сущностей
   
   root: корневой узел программы
   ctx: контекст анализа (со сформированными таблицами из первого прохода)
   
   Возвращает: 0 если ошибок нет, 1 если есть ошибки
   Использует: CheckSourceItems, CheckExpressions, CheckStatements */
int CheckAllSemantics(NProgram *root, SemanticContext *ctx);

/* Проверить элементы верхнего уровня (глобальные переменные инициализаторы, тела функций)
   
   items: элемент верхнего уровня
   ctx: контекст анализа
   
   Использует: CheckFunctionBody, CheckInitializers */
int CheckSourceItems(NSourceItem *items, SemanticContext *ctx);

/* Проверить тело функции
   Устанавливает scope функции, проверяет все операторы и выражения
   
   func_body: операторы функции (может быть NULL для абстрактных функций)
   func_info: информация о функции (для контекста)
   ctx: контекст анализа
   
   Использует: PushScope, PopScope, CheckStatements
   Результат: все переменные функции добавлены в локальный scope */
int CheckFunctionBody(NStmt *func_body, FunctionInfo *func_info, SemanticContext *ctx);

/* Проверить все операторы в связном списке
   
   stmts: список операторов
   ctx: контекст анализа
   
   Использует: CheckStatement для каждого оператора
   Ошибки: все ошибки, обнаруженные при проверке операторов */
int CheckStatements(NStmt *stmts, SemanticContext *ctx, NType *expected_return_type, int *return_seen);

/* Проверить один оператор
   
   stmt: оператор для проверки
   ctx: контекст анализа
   
   Использует: CheckExpression (для STMT_EXPR), CheckStatement (рекурсивно)
   Ошибки: все ошибки, обнаруженные в операторе и его подоператорах */
int CheckStatement(NStmt *stmt, SemanticContext *ctx, NType *expected_return_type, int *return_seen);

/* Проверить все выражения в дереве
   Разрешает имена переменных, проверяет вызовы функций, проверяет типы
   
   root: корневое выражение для проверки (или NULL)
   ctx: контекст анализа
   
   Использует: CheckExpression, InferExpressionType
   Ошибки: undefined variables, undefined functions, type mismatches и т.д. */
int CheckExpressions(NExpr *root, SemanticContext *ctx);

/* Проверить одно выражение
   
   expr: выражение для проверки (или NULL)
   ctx: контекст анализа
   
   Использует: LookupSymbol (для переменных), LookupFunction (для функций)
   Ошибки: все ошибки, обнаруженные в выражении и его подвыражениях */
int CheckExpression(NExpr *expr, SemanticContext *ctx);

/* Проверить доступность переменной/функции из текущего контекста
   Учитывает: видимость scope'а, public/private/protected
   
   symbol: символ для проверки доступности
   ctx: контекст анализа (текущий scope)
   
   Возвращает: 1 если доступна, 0 если нет */
int IsSymbolAccessible(Symbol *symbol, SemanticContext *ctx);

// ----- Третий проход: атрибутирвоание AST -----

/* Атрибутировать все узлы выражений типами
   Добавляет вычисленный тип в каждый узел выражения
   
   root: корневое выражение
   ctx: контекст анализа
   
   Использует: InferExpressionType для каждого выражения
   Результат: каждый узел NExpr получает поле для хранения типа */
int AttributeExpressions(NExpr *root, SemanticContext *ctx);

/* Атрибутировать все узлы операторов информацией о scope
   Добавляет информацию о текущем scope в каждый узел оператора
   
   stmts: список операторов
   ctx: контекст анализа
   
   Использует: GetCurrentScope
   Результат: каждый узел NStmt может быть связан со своей областью видимости */
int AttributeStatements(NStmt *stmts, SemanticContext *ctx);

/* Проставить JVM RefKey для обращений к полям/методам/функциям.
   Использует ClassInfo/FieldInfo/MethodInfo и JVM дескрипторы.
   Возвращает 0 при успехе, 1 при наличии ошибок. */
int AttributeJvmRefKeys(NProgram *root, SemanticContext *ctx);

#endif /* SEMANTIC_PASSES_H */
