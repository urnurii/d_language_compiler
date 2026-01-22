#ifndef CODEGEN_JVM_H
#define CODEGEN_JVM_H

#include "semantic/semantic_types.h"

int GenerateClassFiles(NProgram *root, SemanticContext *ctx);

#endif
