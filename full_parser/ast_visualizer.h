/* ============================================================================
 * ast_visualizer.h
 * 
 * Заголовочный файл для визуализации синтаксического дерева в DOT формат
 * Генерирует код для Graphviz, который можно визуализировать
 * 
 * Использование:
 *   FILE *dot_file = fopen("output.dot", "w");
 *   ProgramToDoT(dot_file, root);
 *   fclose(dot_file);
 *   
 *   Затем в консоли:
 *   $ dot -Tpng output.dot -o output.png
 *   $ dot -Tsvg output.dot -o output.svg
 * ============================================================================ */

#ifndef AST_VISUALIZER_H
#define AST_VISUALIZER_H

#include <stdio.h>
#include "tree_nodes.h"

/* ============================================================================
 * ОСНОВНЫЕ ФУНКЦИИ ДЛЯ ВИЗУАЛИЗАЦИИ
 * ============================================================================ */

/**
 * Генерирует DOT код для всей программы
 * @param file - файловый дескриптор для записи
 * @param program - корень дерева программы
 */
void ProgramToDoT(FILE *file, NProgram *program);

/**
 * Генерирует DOT код для отдельного выражения
 * Используется внутри других функций
 * @param file - файловый дескриптор для записи
 * @param expr - выражение
 * @return - ID узла в графе
 */
int ExprToDoT(FILE *file, NExpr *expr);

/**
 * Генерирует DOT код для оператора
 * @param file - файловый дескриптор для записи
 * @param stmt - оператор
 * @return - ID узла в графе
 */
int StmtToDoT(FILE *file, NStmt *stmt);

/**
 * Генерирует DOT код для типа
 * @param file - файловый дескриптор для записи
 * @param type - тип
 * @return - ID узла в графе
 */
int TypeToDoT(FILE *file, NType *type);

/**
 * Сбрасывает счётчик ID узлов
 * Используется в начале визуализации
 */
void ResetNodeCounter(void);

#endif /* AST_VISUALIZER_H */
