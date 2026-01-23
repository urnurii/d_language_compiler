#include "jvm_layout.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static char *DuplicateStringLocal(const char *str) {
    char *copy;
    size_t len;

    if (str == NULL) {
        return NULL;
    }
    len = strlen(str);
    copy = (char *)malloc(len + 1);
    if (copy == NULL) {
        return NULL;
    }
    memcpy(copy, str, len + 1);
    return copy;
}

static int AppendToBuffer(char **buf, size_t *len, size_t *cap, const char *text) {
    size_t text_len;
    size_t needed;
    char *new_buf;

    if (buf == NULL || len == NULL || cap == NULL || text == NULL) {
        return 0;
    }

    text_len = strlen(text);
    needed = *len + text_len + 1;
    if (needed > *cap) {
        size_t new_cap = (*cap == 0) ? 32 : *cap;
        while (new_cap < needed) {
            new_cap *= 2;
        }
        new_buf = (char *)realloc(*buf, new_cap);
        if (new_buf == NULL) {
            return 0;
        }
        *buf = new_buf;
        *cap = new_cap;
    }
    memcpy(*buf + *len, text, text_len);
    *len += text_len;
    (*buf)[*len] = '\0';
    return 1;
}

static char *WrapDescriptorAsRefContainer(char *desc) {
    size_t len;
    char *wrapped;

    if (desc == NULL) {
        return NULL;
    }
    len = strlen(desc);
    wrapped = (char *)malloc(len + 2);
    if (wrapped == NULL) {
        free(desc);
        return NULL;
    }
    wrapped[0] = '[';
    memcpy(wrapped + 1, desc, len + 1);
    free(desc);
    return wrapped;
}

static char *BuildJvmParamDescriptor(const NParam *param) {
    char *desc;
    if (param == NULL) {
        return NULL;
    }
    desc = BuildJvmTypeDescriptor(param->param_type);
    if (desc == NULL) {
        return NULL;
    }
    if (param->is_ref) {
        if (param->param_type != NULL &&
            param->param_type->kind == TYPE_KIND_BASE &&
            param->param_type->base_type == TYPE_CHAR) {
            free(desc);
            return DuplicateStringLocal("[I");
        }
        return WrapDescriptorAsRefContainer(desc);
    }
    return desc;
}

JvmLayoutContext *CreateJvmLayoutContext(void) {
    JvmLayoutContext *ctx = (JvmLayoutContext *)malloc(sizeof(JvmLayoutContext));
    if (ctx == NULL) {
        return NULL;
    }
    ctx->reserved = 0;
    return ctx;
}

void DestroyJvmLayoutContext(JvmLayoutContext *ctx) {
    if (ctx == NULL) {
        return;
    }
    free(ctx);
}

char *BuildJvmInternalName(const char *name) {
    char *copy;
    size_t len;

    if (name == NULL || name[0] == '\0') {
        return DuplicateStringLocal("java/lang/Object");
    }
    len = strlen(name);
    copy = (char *)malloc(len + 1);
    if (copy == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < len; i++) {
        copy[i] = (name[i] == '.') ? '/' : name[i];
    }
    copy[len] = '\0';
    return copy;
}

static const char *GetBaseDescriptor(BaseType base) {
    switch (base) {
        case TYPE_INT:    return "I";
        case TYPE_CHAR:   return "C";
        case TYPE_STRING: return "Ljava/lang/String;";
        case TYPE_BOOL:   return "Z";
        case TYPE_FLOAT:  return "F";
        case TYPE_DOUBLE: return "D";
        case TYPE_REAL:   return "D";
        case TYPE_VOID:   return "V";
        case TYPE_CLASS:  return "Ljava/lang/Object;";
        default:          return "Ljava/lang/Object;";
    }
}

static char *BuildClassDescriptorFromName(const char *class_name) {
    char *internal;
    char *desc;
    size_t len;

    internal = BuildJvmInternalName(class_name);
    if (internal == NULL) {
        return NULL;
    }
    len = strlen(internal);
    desc = (char *)malloc(len + 3);
    if (desc == NULL) {
        free(internal);
        return NULL;
    }
    desc[0] = 'L';
    memcpy(desc + 1, internal, len);
    desc[len + 1] = ';';
    desc[len + 2] = '\0';
    free(internal);
    return desc;
}

static char *BuildTypeDescriptorNoArray(const NType *type) {
    if (type == NULL) {
        return NULL;
    }
    if (type->kind == TYPE_KIND_CLASS || type->kind == TYPE_KIND_CLASS_ARRAY) {
        return BuildClassDescriptorFromName(type->class_name);
    }
    if (type->kind == TYPE_KIND_ENUM || type->kind == TYPE_KIND_ENUM_ARRAY) {
        return DuplicateStringLocal("I");
    }
    if (type->kind == TYPE_KIND_BASE || type->kind == TYPE_KIND_BASE_ARRAY) {
        return DuplicateStringLocal(GetBaseDescriptor(type->base_type));
    }
    return DuplicateStringLocal("Ljava/lang/Object;");
}

char *BuildJvmTypeDescriptor(const NType *type) {
    char *elem_desc;
    char *desc;
    size_t len;

    if (type == NULL) {
        return NULL;
    }

    if (type->kind == TYPE_KIND_BASE_ARRAY ||
        type->kind == TYPE_KIND_CLASS_ARRAY ||
        type->kind == TYPE_KIND_ENUM_ARRAY) {
        if (type->kind == TYPE_KIND_BASE_ARRAY && type->base_type == TYPE_CHAR) {
            return DuplicateStringLocal("[I");
        }
        elem_desc = BuildTypeDescriptorNoArray(type);
        if (elem_desc == NULL) {
            return NULL;
        }
        len = strlen(elem_desc);
        desc = (char *)malloc(len + 2);
        if (desc == NULL) {
            free(elem_desc);
            return NULL;
        }
        desc[0] = '[';
        memcpy(desc + 1, elem_desc, len + 1);
        free(elem_desc);
        return desc;
    }

    return BuildTypeDescriptorNoArray(type);
}

char *BuildJvmMethodDescriptor(const NType *return_type, const NParamList *params) {
    char *buf = NULL;
    size_t len = 0;
    size_t cap = 0;

    if (!AppendToBuffer(&buf, &len, &cap, "(")) {
        free(buf);
        return NULL;
    }
    if (params != NULL) {
        for (int i = 0; i < params->count; i++) {
            NParam *param = params->params[i];
            char *pdesc = BuildJvmParamDescriptor(param);
            if (pdesc == NULL) {
                free(buf);
                return NULL;
            }
            if (!AppendToBuffer(&buf, &len, &cap, pdesc)) {
                free(pdesc);
                free(buf);
                return NULL;
            }
            free(pdesc);
        }
    }
    if (!AppendToBuffer(&buf, &len, &cap, ")")) {
        free(buf);
        return NULL;
    }

    if (return_type == NULL) {
        if (!AppendToBuffer(&buf, &len, &cap, "V")) {
            free(buf);
            return NULL;
        }
    } else {
        char *rdesc = BuildJvmTypeDescriptor(return_type);
        if (rdesc == NULL) {
            free(buf);
            return NULL;
        }
        if (!AppendToBuffer(&buf, &len, &cap, rdesc)) {
            free(rdesc);
            free(buf);
            return NULL;
        }
        free(rdesc);
    }
    return buf;
}

static int ReplaceString(char **target, char *value) {
    if (target == NULL) {
        free(value);
        return 0;
    }
    if (*target != NULL) {
        free(*target);
    }
    *target = value;
    return value != NULL;
}

int AssignJvmDescriptorToFunc(NFuncDef *func) {
    char *desc;
    if (func == NULL) {
        return 0;
    }
    desc = BuildJvmMethodDescriptor(func->return_type, func->params);
    return ReplaceString(&func->jvm_descriptor, desc);
}

int AssignJvmDescriptorToMethod(NMethodDef *method) {
    char *desc;
    if (method == NULL) {
        return 0;
    }
    desc = BuildJvmMethodDescriptor(method->return_type, method->params);
    return ReplaceString(&method->jvm_descriptor, desc);
}

int AssignJvmDescriptorToParams(NParamList *params) {
    if (params == NULL) {
        return 1;
    }
    for (int i = 0; i < params->count; i++) {
        NParam *param = params->params[i];
        char *desc;
        if (param == NULL) {
            continue;
        }
        desc = BuildJvmParamDescriptor(param);
        if (!ReplaceString(&param->jvm_descriptor, desc)) {
            return 0;
        }
    }
    return 1;
}

int AssignJvmDescriptorToInitDecls(const NType *decl_type, NInitDeclList *init_decls) {
    char *desc;
    if (init_decls == NULL) {
        return 1;
    }
    desc = BuildJvmTypeDescriptor(decl_type);
    if (desc == NULL) {
        return 0;
    }
    for (int i = 0; i < init_decls->count; i++) {
        NInitDecl *decl = init_decls->decls[i];
        char *copy;
        if (decl == NULL) {
            continue;
        }
        copy = DuplicateStringLocal(desc);
        if (!ReplaceString(&decl->jvm_descriptor, copy)) {
            free(desc);
            return 0;
        }
    }
    free(desc);
    return 1;
}

int GetJvmSlotWidthForType(const NType *type) {
    if (type == NULL) {
        return 1;
    }
    if (type->kind == TYPE_KIND_CLASS || type->kind == TYPE_KIND_CLASS_ARRAY) {
        return 1;
    }
    if (type->kind == TYPE_KIND_BASE || type->kind == TYPE_KIND_BASE_ARRAY) {
        switch (type->base_type) {
            case TYPE_DOUBLE:
            case TYPE_REAL:
                return 2;
            case TYPE_VOID:
                return 0;
            default:
                return 1;
        }
    }
    return 1;
}

void InitSlotAllocator(JvmSlotAllocator *alloc, int include_this) {
    if (alloc == NULL) {
        return;
    }
    alloc->next_slot = include_this ? 1 : 0;
}

int AssignThisSlot(JvmSlotAllocator *alloc) {
    int slot;
    if (alloc == NULL) {
        return -1;
    }
    slot = alloc->next_slot;
    alloc->next_slot += 1;
    return slot;
}

int AssignParamSlots(JvmSlotAllocator *alloc, NParamList *params) {
    if (alloc == NULL) {
        return -1;
    }
    if (params == NULL) {
        return alloc->next_slot;
    }
    for (int i = 0; i < params->count; i++) {
        NParam *param = params->params[i];
        int width = 1;
        if (param == NULL) {
            continue;
        }
        if (param->is_ref) {
            width = 1;
        } else {
            width = GetJvmSlotWidthForType(param->param_type);
        }
        param->jvm_slot_index = alloc->next_slot;
        alloc->next_slot += width;
    }
    return alloc->next_slot;
}

int AssignLocalSlotsForDecl(JvmSlotAllocator *alloc, const NType *decl_type, NInitDeclList *init_decls) {
    int width;
    if (alloc == NULL) {
        return -1;
    }
    if (init_decls == NULL) {
        return alloc->next_slot;
    }
    width = GetJvmSlotWidthForType(decl_type);
    for (int i = 0; i < init_decls->count; i++) {
        NInitDecl *decl = init_decls->decls[i];
        if (decl == NULL) {
            continue;
        }
        decl->jvm_slot_index = alloc->next_slot;
        alloc->next_slot += width;
    }
    return alloc->next_slot;
}

int SetJvmRefKey(JvmRefKey *key, const char *owner_internal_name, const char *member_name,
                 const char *member_descriptor, JvmRefKind kind) {
    if (key == NULL) {
        return 0;
    }
    ClearJvmRefKey(key);
    key->owner_internal_name = DuplicateStringLocal(owner_internal_name);
    key->member_name = DuplicateStringLocal(member_name);
    key->member_descriptor = DuplicateStringLocal(member_descriptor);
    if ((owner_internal_name && key->owner_internal_name == NULL) ||
        (member_name && key->member_name == NULL) ||
        (member_descriptor && key->member_descriptor == NULL)) {
        ClearJvmRefKey(key);
        return 0;
    }
    key->has_key = 1;
    key->kind = kind;
    return 1;
}

void ClearJvmRefKey(JvmRefKey *key) {
    if (key == NULL) {
        return;
    }
    if (key->owner_internal_name != NULL) {
        free(key->owner_internal_name);
    }
    if (key->member_name != NULL) {
        free(key->member_name);
    }
    if (key->member_descriptor != NULL) {
        free(key->member_descriptor);
    }
    key->owner_internal_name = NULL;
    key->member_name = NULL;
    key->member_descriptor = NULL;
    key->has_key = 0;
    key->kind = JVM_REF_FIELD;
}
