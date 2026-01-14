#ifndef AST_VISUALIZER_H
#define AST_VISUALIZER_H

#include <stdio.h>
#include "tree_nodes.h"

// ----- Функции для ввизуализации -----

/**
 * Генерирует DOT код для всей программы
 * @param file - файловый дескриптор для записи
 * @param program - корень дерева программы
 */
void ProgramToDoT(FILE *file, NProgram *program);

/**
 * Генерирует DOT код для отдельного выражения
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

// Сброс счетчика ID узлов
void ResetNodeCounter(void);

#endif