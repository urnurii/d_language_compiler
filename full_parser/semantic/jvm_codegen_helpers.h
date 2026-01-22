#ifndef JVM_CODEGEN_HELPERS_H
#define JVM_CODEGEN_HELPERS_H

#include "../tree_nodes.h"
#include "jvmc/jvmc.h"

int JvmEmitLoadByType(jvmc_code *code, const NType *type, int slot);
int JvmEmitStoreByType(jvmc_code *code, const NType *type, int slot);
int JvmEmitReturnByType(jvmc_code *code, const NType *type);
int JvmEmitNumericCast(jvmc_code *code, const NType *from, const NType *to);
int JvmEmitCompareIntResult(jvmc_code *code, jvmc_compare cmp);
const char *JvmPrintDescriptorForType(const NType *type);

#endif
