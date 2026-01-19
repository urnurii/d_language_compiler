#ifndef AST_VISUALIZER_H
#define AST_VISUALIZER_H

#include "tree_nodes.h"
#include <stdio.h>

void VisualizeASTToFile(NProgram *program, const char *output_file);

void VisualizeASTToStdout(NProgram *program);

#endif
