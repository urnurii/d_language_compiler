#ifndef AST_VISUALIZER_H
#define AST_VISUALIZER_H

#include "tree_nodes.h"
#include <stdio.h>

/**
 * Генерирует DOT код для визуализации синтаксического дерева
 * 
 * @param program - корень синтаксического дерева
 * @param output_file - имя файла для сохранения DOT кода (если NULL, выводит в stdout)
 * 
 * Функция преобразует AST в формат Graphviz DOT:
 * - Каждому узлу присваивается уникальный ID (автоинкрементный)
 * - Узлы называются по типам грамматики (expr, stmt, class, func и т.д.)
 * - Связи между узлами подписаны (например: "condition", "body", "left", "right")
 */
void VisualizeASTToFile(NProgram *program, const char *output_file);

/**
 * Выводит DOT код в стандартный поток вывода
 */
void VisualizeASTToStdout(NProgram *program);

#endif // AST_VISUALIZER_H
