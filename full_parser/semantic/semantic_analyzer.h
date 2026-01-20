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

/* ГЛАВНАЯ ТОЧКА ВХОДА СЕМАНТИКИ
   Выполняет полный семантический анализ программы.
   Вход: готовое AST (root).
   Выход: контекст анализа (ctx_out), 0 если ошибок нет, 1 если есть ошибки. */
int AnalyzeProgram(NProgram *root, SemanticContext **ctx);

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
