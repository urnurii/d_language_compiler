#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "semantic_types.h"

/* ============================================================================
   ГЛАВНЫЙ ИНТЕРФЕЙС СЕМАНТИЧЕСКОГО АНАЛИЗА
   ============================================================================ */

/* Создание контекста анализа */
SemanticContext* CreateSemanticContext(void);

/* Уничтожение контекста анализа */
void DestroySemanticContext(SemanticContext *ctx);

/* ГЛАВНАЯ ФУНКЦИЯ АНАЛИЗА */
/* Выполняет полный семантический анализ программы
   Входные данные: готовое AST из парсера (root)
   Выходные данные: контекст анализа с таблицами символов, атрибутированное AST
   Возвращает: 0 если ошибок нет, 1 если есть ошибки */
int AnalyzeProgram(NProgram *root, SemanticContext **ctx);

/* ПЕРВЫЙ ПРОХОД: СБОР ДЕКЛАРАЦИЙ */
/* Собирает все объявления верхнего уровня:
   - Функции
   - Классы (с их полями и методами)
   - Глобальные переменные
   - Enum'ы
   
   Использует: AddSymbolToTable, AddClassToContext
   Ошибки: дублирование имён, неопределённые базовые классы */
int FirstPassCollectDeclarations(NProgram *root, SemanticContext *ctx);

/* ВТОРОЙ ПРОХОД: ПРОВЕРКА ДЕКЛАРАЦИЙ И ИСПОЛЬЗОВАНИЯ */
/* Проходит по всему дереву и проверяет:
   - Использование переменных (разрешение имён)
   - Использование функций
   - Использование методов
   - Совместимость типов
   
   Использует: LookupSymbol, CheckExpressionTypes, CheckFunctionCall
   Ошибки: undefined variable, function not found, type mismatch и т.д. */
int SecondPassCheckSemantics(NProgram *root, SemanticContext *ctx);

/* ТРЕТИЙ ПРОХОД: АТРИБУТИРОВАНИЕ AST */
/* Добавляет информацию в узлы дерева:
   - Вычисленные типы выражений
   - Индексы символов в таблице для быстрого доступа при генерации кода
   - Информацию о scope'е
   
   Использует: InferExpressionType, AttributeExpressions
   Результат: готовое AST для генератора кода */
int ThirdPassAttributeAST(NProgram *root, SemanticContext *ctx);

/* ВЫВОД ОШИБОК */
/* Выводит все найденные семантические ошибки в stderr
   Формат: "SEMANTIC ERROR at line X, column Y: description"
   Если нет ошибок - выводит "No semantic errors found" */
void PrintSemanticErrors(SemanticContext *ctx);

/* ВЫВОД ТАБЛИЦ СИМВОЛОВ (для отладки) */
/* Выводит все таблицы символов в stdout для проверки корректности анализа
   Включает: глобальные переменные, функции, классы, enum'ы */
void PrintSymbolTables(SemanticContext *ctx);

#endif /* SEMANTIC_ANALYZER_H */
