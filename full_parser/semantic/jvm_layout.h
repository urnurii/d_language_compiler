#ifndef JVM_LAYOUT_H
#define JVM_LAYOUT_H

#include "tree_nodes.h"

typedef struct JvmLayoutContext {
    int reserved;
} JvmLayoutContext;

typedef struct {
    int next_slot;
} JvmSlotAllocator;

JvmLayoutContext *CreateJvmLayoutContext(void);
void DestroyJvmLayoutContext(JvmLayoutContext *ctx);

char *BuildJvmInternalName(const char *name);
char *BuildJvmTypeDescriptor(const NType *type);
char *BuildJvmMethodDescriptor(const NType *return_type, const NParamList *params);

int AssignJvmDescriptorToFunc(NFuncDef *func);
int AssignJvmDescriptorToMethod(NMethodDef *method);
int AssignJvmDescriptorToParams(NParamList *params);
int AssignJvmDescriptorToInitDecls(const NType *decl_type, NInitDeclList *init_decls);

int GetJvmSlotWidthForType(const NType *type);
void InitSlotAllocator(JvmSlotAllocator *alloc, int include_this);
int AssignThisSlot(JvmSlotAllocator *alloc);
int AssignParamSlots(JvmSlotAllocator *alloc, NParamList *params);
int AssignLocalSlotsForDecl(JvmSlotAllocator *alloc, const NType *decl_type, NInitDeclList *init_decls);

int SetJvmRefKey(JvmRefKey *key, const char *owner_internal_name, const char *member_name,
                 const char *member_descriptor, JvmRefKind kind);
void ClearJvmRefKey(JvmRefKey *key);

#endif /* JVM_LAYOUT_H */
