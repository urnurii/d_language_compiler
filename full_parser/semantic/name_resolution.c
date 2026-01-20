
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "name_resolution.h"
#include "error_reporting.h"
#include "type_inference.h"

static char* DuplicateStringLocal(const char *str) {
    size_t len;
    char *copy;

    if (str == NULL) {
        return NULL;
    }
    len = strlen(str);
    copy = (char*)malloc(len + 1);
    if (copy == NULL) {
        return NULL;
    }
    memcpy(copy, str, len + 1);
    return copy;
}

static int IsValidLValueExprLocal(const NExpr *expr) {
    if (expr == NULL) {
        return 0;
    }
    switch (expr->type) {
        case EXPR_IDENT:
        case EXPR_MEMBER_ACCESS:
        case EXPR_ARRAY_ACCESS:
            return 1;
        case EXPR_PAREN:
            return IsValidLValueExprLocal(expr->value.inner_expr);
        default:
            return 0;
    }
}

static int AreParamListsCompatible(const NParamList *a, const NParamList *b) {
    int count_a;
    int count_b;

    count_a = (a != NULL) ? a->count : 0;
    count_b = (b != NULL) ? b->count : 0;
    if (count_a != count_b) {
        return 0;
    }
    for (int i = 0; i < count_a; i++) {
        NParam *pa = a->params[i];
        NParam *pb = b->params[i];
        if (pa == NULL || pb == NULL) {
            continue;
        }
        if (pa->is_ref != pb->is_ref) {
            return 0;
        }
        if (!TypesEqual(pa->param_type, pb->param_type)) {
            return 0;
        }
    }
    return 1;
}

static int AreFunctionSignaturesCompatible(const FunctionInfo *a, const FunctionInfo *b) {
    if (a == NULL || b == NULL) {
        return 0;
    }
    if (!TypesEqual(a->return_type, b->return_type)) {
        return 0;
    }
    return AreParamListsCompatible(a->params, b->params);
}

static void FreeFunctionInfoShallow(FunctionInfo *info) {
    free(info);
}

static int CanMatchFunction(FunctionInfo *func, NExpr **args, int arg_count,
                            SemanticContext *ctx, int *score_out) {
    int expected;
    int count;
    int score = 0;

    if (func == NULL) {
        return 0;
    }

    expected = func->params ? func->params->count : 0;

    if (func->allow_extra_args) {
        if (arg_count < expected) {
            return 0;
        }
    } else {
        if (arg_count > expected) {
            return 0;
        }
        if (arg_count < expected && func->params != NULL) {
            for (int i = arg_count; i < expected; i++) {
                NParam *param = func->params->params[i];
                if (param == NULL || param->default_value == NULL) {
                    return 0;
                }
            }
        }
    }

    if (func->params == NULL) {
        if (score_out != NULL) {
            *score_out = score;
        }
        return 1;
    }

    count = expected < arg_count ? expected : arg_count;
    for (int i = 0; i < count; i++) {
        NParam *param = func->params->params[i];
        NExpr *arg = (args != NULL) ? args[i] : NULL;
        NType *arg_type;
        if (param == NULL || arg == NULL || param->param_type == NULL) {
            return 0;
        }
        if (param->is_ref && !IsValidLValueExprLocal(arg)) {
            return 0;
        }
        arg_type = InferExpressionTypeSilent(arg, ctx);
        if (arg_type == NULL) {
            return 0;
        }
        if (!IsArgumentCompatibleWithParameter(param->param_type, arg_type, param->is_ref)) {
            return 0;
        }
        if (TypesEqual(param->param_type, arg_type)) {
            score += 2;
        } else {
            score += 1;
        }
    }

    if (score_out != NULL) {
        *score_out = score;
    }
    return 1;
}

static Symbol* LookupSymbolInTable(SymbolTable *table, const char *name) {
    int i;

    if (table == NULL || name == NULL) {
        return NULL;
    }

    for (i = 0; i < table->count; i++) {
        Symbol *sym = table->symbols[i];
        if (sym != NULL && sym->name != NULL && strcmp(sym->name, name) == 0) {
            return sym;
        }
    }
    return NULL;
}

static int AddSymbolToTableInternal(SymbolTable *table, const Symbol *symbol, SemanticContext *ctx) {
    Symbol *stored;
    Symbol **grown;

    if (table == NULL || symbol == NULL || symbol->name == NULL) {
        return 1;
    }

    if (LookupSymbolInTable(table, symbol->name) != NULL) {
        if (ctx != NULL && ctx->errors != NULL) {
            SemanticError err = CreateDuplicateSymbolError(symbol->name, 0, 0);
            AddError(ctx->errors, &err);
        }
        return 1;
    }

    if (table->count >= table->capacity) {
        int new_capacity = (table->capacity == 0) ? 8 : table->capacity * 2;
        grown = (Symbol**)realloc(table->symbols, sizeof(Symbol*) * (size_t)new_capacity);
        if (grown == NULL) {
            return 1;
        }
        table->symbols = grown;
        table->capacity = new_capacity;
    }

    stored = (Symbol*)malloc(sizeof(Symbol));
    if (stored == NULL) {
        return 1;
    }
    memset(stored, 0, sizeof(Symbol));

    stored->kind = symbol->kind;
    stored->name = DuplicateStringLocal(symbol->name);
    stored->info = symbol->info;
    stored->scope_depth = symbol->scope_depth;

    if (stored->name == NULL) {
        free(stored);
        return 1;
    }

    table->symbols[table->count++] = stored;
    return 0;
}

SymbolTable* CreateSymbolTable(int initial_capacity) {
    SymbolTable *table;
    int cap = (initial_capacity > 0) ? initial_capacity : 8;

    table = (SymbolTable*)malloc(sizeof(SymbolTable));
    if (table == NULL) {
        return NULL;
    }
    table->count = 0;
    table->capacity = cap;
    table->symbols = (Symbol**)malloc(sizeof(Symbol*) * (size_t)table->capacity);
    if (table->symbols == NULL) {
        free(table);
        return NULL;
    }
    return table;
}

void DestroySymbolTable(SymbolTable *table) {
    int i;

    if (table == NULL) {
        return;
    }

    for (i = 0; i < table->count; i++) {
        Symbol *sym = table->symbols[i];
        if (sym != NULL) {
            free(sym->name);
            free(sym);
        }
    }
    free(table->symbols);
    free(table);
}

Scope* CreateScope(const char *scope_name, int depth, Scope *parent) {
    Scope *scope = (Scope*)malloc(sizeof(Scope));
    if (scope == NULL) {
        return NULL;
    }
    scope->depth = depth;
    scope->scope_name = DuplicateStringLocal(scope_name);
    scope->locals = CreateSymbolTable(8);
    scope->parent = parent;
    if (scope->locals == NULL) {
        free(scope->scope_name);
        free(scope);
        return NULL;
    }
    return scope;
}

void DestroyScope(Scope *scope) {
    if (scope == NULL) {
        return;
    }
    free(scope->scope_name);
    if (scope->locals != NULL) {
        DestroySymbolTable(scope->locals);
    }
    free(scope);
}

ScopeStack* CreateScopeStack(SymbolTable *global_table) {
    ScopeStack *stack;
    Scope *global_scope;

    stack = (ScopeStack*)malloc(sizeof(ScopeStack));
    if (stack == NULL) {
        return NULL;
    }
    stack->count = 0;
    stack->capacity = 8;
    stack->scopes = (Scope**)malloc(sizeof(Scope*) * (size_t)stack->capacity);
    if (stack->scopes == NULL) {
        free(stack);
        return NULL;
    }

    stack->global = global_table;
    global_scope = (Scope*)malloc(sizeof(Scope));
    if (global_scope == NULL) {
        free(stack->scopes);
        free(stack);
        return NULL;
    }
    global_scope->depth = 0;
    global_scope->scope_name = DuplicateStringLocal("global");
    global_scope->locals = global_table;
    global_scope->parent = NULL;

    stack->scopes[0] = global_scope;
    stack->count = 1;
    return stack;
}

void DestroyScopeStack(ScopeStack *stack) {
    int i;

    if (stack == NULL) {
        return;
    }

    for (i = stack->count - 1; i >= 0; i--) {
        Scope *scope = stack->scopes[i];
        if (scope == NULL) {
            continue;
        }
        if (scope->depth == 0) {
            free(scope->scope_name);
            free(scope);
        } else {
            DestroyScope(scope);
        }
    }
    free(stack->scopes);
    free(stack);
}

/* ============================================================================
   SYMBOL TABLE MANAGEMENT
   ============================================================================ */

int AddSymbolToTable(SemanticContext *ctx, const Symbol *symbol) {
    if (ctx == NULL || ctx->global_symbols == NULL) {
        return 1;
    }
    return AddSymbolToTableInternal(ctx->global_symbols, symbol, ctx);
}

Symbol* LookupSymbol(SemanticContext *ctx, const char *name) {
    Scope *scope;
    Symbol *found;

    if (ctx == NULL || name == NULL) {
        return NULL;
    }

    scope = GetCurrentScope(ctx);
    while (scope != NULL) {
        found = LookupSymbolInTable(scope->locals, name);
        if (found != NULL) {
            return found;
        }
        scope = scope->parent;
    }

    if (ctx->global_symbols != NULL) {
        return LookupSymbolInTable(ctx->global_symbols, name);
    }

    return NULL;
}

Symbol* LookupGlobalSymbol(SemanticContext *ctx, const char *name) {
    if (ctx == NULL || ctx->global_symbols == NULL) {
        return NULL;
    }
    return LookupSymbolInTable(ctx->global_symbols, name);
}

/* ============================================================================
   SCOPE STACK MANAGEMENT
   ============================================================================ */

int PushScope(SemanticContext *ctx, const char *scope_name) {
    ScopeStack *stack;
    Scope *parent;
    Scope *scope;
    Scope **grown;

    if (ctx == NULL || ctx->scope_stack == NULL) {
        return 1;
    }

    stack = ctx->scope_stack;
    parent = GetCurrentScope(ctx);
    scope = CreateScope(scope_name, parent ? parent->depth + 1 : 1, parent);
    if (scope == NULL) {
        return 1;
    }

    if (stack->count >= stack->capacity) {
        int new_capacity = stack->capacity * 2;
        grown = (Scope**)realloc(stack->scopes, sizeof(Scope*) * (size_t)new_capacity);
        if (grown == NULL) {
            DestroyScope(scope);
            return 1;
        }
        stack->scopes = grown;
        stack->capacity = new_capacity;
    }

    stack->scopes[stack->count++] = scope;
    return 0;
}

int PopScope(SemanticContext *ctx) {
    ScopeStack *stack;
    Scope *scope;

    if (ctx == NULL || ctx->scope_stack == NULL) {
        return 1;
    }

    stack = ctx->scope_stack;
    if (stack->count <= 1) {
        return 1;
    }

    scope = stack->scopes[stack->count - 1];
    stack->scopes[stack->count - 1] = NULL;
    stack->count -= 1;

    if (scope != NULL) {
        DestroyScope(scope);
    }

    return 0;
}

Scope* GetCurrentScope(SemanticContext *ctx) {
    if (ctx == NULL || ctx->scope_stack == NULL || ctx->scope_stack->count == 0) {
        return NULL;
    }
    return ctx->scope_stack->scopes[ctx->scope_stack->count - 1];
}

/* ============================================================================
   CONTEXT HELPERS
   ============================================================================ */

int AddLocalVariable(SemanticContext *ctx, VariableInfo *var_info) {
    Scope *scope;
    Symbol sym;

    if (ctx == NULL || var_info == NULL || var_info->name == NULL) {
        return 1;
    }

    scope = GetCurrentScope(ctx);
    if (scope == NULL || scope->locals == NULL) {
        return 1;
    }

    memset(&sym, 0, sizeof(Symbol));
    sym.kind = SYMBOL_VARIABLE;
    sym.name = var_info->name;
    sym.info.var_info = var_info;
    sym.scope_depth = scope->depth;

    return AddSymbolToTableInternal(scope->locals, &sym, ctx);
}

/* ============================================================================
   ДОБАВЛЕНИЕ ИНФОРМАЦИИ В КОНТЕКСТ
   ============================================================================ */

int AddFunctionToContext(SemanticContext *ctx, FunctionInfo *func_info) {
    
    Symbol sym;
    FunctionInfo **grown;
    Symbol *existing;
    int has_function_symbol = 0;

    if (ctx == NULL || func_info == NULL || func_info->name == NULL) {
        return 1;
    }

    existing = LookupGlobalSymbol(ctx, func_info->name);
    if (existing != NULL) {
        if (existing->kind != SYMBOL_FUNCTION) {
            if (ctx->errors != NULL) {
                SemanticError err = CreateDuplicateSymbolError(func_info->name,
                                                              func_info->line,
                                                              func_info->column);
                AddError(ctx->errors, &err);
            }
            return 1;
        }
        has_function_symbol = 1;
    }

    for (int i = 0; i < ctx->function_count; i++) {
        FunctionInfo *prev = ctx->functions[i];
        if (prev == NULL || prev->name == NULL) {
            continue;
        }
        if (strcmp(prev->name, func_info->name) != 0) {
            continue;
        }
        if (!AreFunctionSignaturesCompatible(prev, func_info)) {
            continue;
        }
        if (!prev->is_prototype && !func_info->is_prototype) {
            if (ctx->errors != NULL) {
                SemanticError err = CreateDuplicateSymbolError(func_info->name,
                                                              func_info->line,
                                                              func_info->column);
                AddError(ctx->errors, &err);
            }
            return 1;
        }
        if (prev->is_prototype && !func_info->is_prototype) {
            prev->is_prototype = 0;
        }
        FreeFunctionInfoShallow(func_info);
        return 0;
    }

    if (!has_function_symbol) {
        memset(&sym, 0, sizeof(Symbol));
        sym.kind = SYMBOL_FUNCTION;
        sym.name = func_info->name;
        sym.info.func_info = func_info;
        sym.scope_depth = 0;

        if (AddSymbolToTable(ctx, &sym) != 0) {
            return 1;
        }
    }

    grown = (FunctionInfo**)realloc(ctx->functions,
                                    sizeof(FunctionInfo*) * (size_t)(ctx->function_count + 1));
    if (grown == NULL) {
        return 1;
    }
    ctx->functions = grown;
    ctx->functions[ctx->function_count] = func_info;
    ctx->function_count += 1;
    return 0;
}

int AddClassToContext(SemanticContext *ctx, ClassInfo *class_info) {
    Symbol sym;
    ClassInfo **grown;

    if (ctx == NULL || class_info == NULL || class_info->name == NULL) {
        return 1;
    }

    if (LookupGlobalSymbol(ctx, class_info->name) != NULL) {
        if (ctx->errors != NULL) {
            SemanticError err = CreateDuplicateSymbolError(class_info->name,
                                                          class_info->line,
                                                          class_info->column);
            AddError(ctx->errors, &err);
        }
        return 1;
    }

    memset(&sym, 0, sizeof(Symbol));
    sym.kind = SYMBOL_CLASS;
    sym.name = class_info->name;
    sym.info.class_info = class_info;
    sym.scope_depth = 0;

    if (AddSymbolToTable(ctx, &sym) != 0) {
        return 1;
    }

    grown = (ClassInfo**)realloc(ctx->classes,
                                 sizeof(ClassInfo*) * (size_t)(ctx->class_count + 1));
    if (grown == NULL) {
        return 1;
    }
    ctx->classes = grown;
    ctx->classes[ctx->class_count] = class_info;
    ctx->class_count += 1;
    return 0;
}

int AddEnumToContext(SemanticContext *ctx, EnumInfo *enum_info) {
    Symbol sym;
    EnumInfo **grown;

    if (ctx == NULL || enum_info == NULL) {
        return 1;
    }

    if (enum_info->name != NULL) {
        if (LookupGlobalSymbol(ctx, enum_info->name) != NULL) {
            if (ctx->errors != NULL) {
                SemanticError err = CreateDuplicateSymbolError(enum_info->name,
                                                              enum_info->line,
                                                              enum_info->column);
                AddError(ctx->errors, &err);
            }
            return 1;
        }
    }

    if (enum_info->items != NULL) {
        for (int i = 0; i < enum_info->item_count; i++) {
            EnumItemInfo *item = enum_info->items[i];
            if (item == NULL || item->name == NULL) {
                continue;
            }
            for (int j = i + 1; j < enum_info->item_count; j++) {
                EnumItemInfo *other = enum_info->items[j];
                if (other == NULL || other->name == NULL) {
                    continue;
                }
                if (strcmp(item->name, other->name) == 0) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateEnumDuplicateItemError(item->name,
                                                                         enum_info->line,
                                                                         enum_info->column);
                        AddError(ctx->errors, &err);
                    }
                    return 1;
                }
            }
        }
    }

    if (enum_info->name != NULL) {
        memset(&sym, 0, sizeof(Symbol));
        sym.kind = SYMBOL_ENUM_TYPE;
        sym.name = enum_info->name;
        sym.info.enum_info = enum_info;
        sym.scope_depth = 0;

        if (AddSymbolToTable(ctx, &sym) != 0) {
            return 1;
        }
    }

    grown = (EnumInfo**)realloc(ctx->enums,
                                sizeof(EnumInfo*) * (size_t)(ctx->enum_count + 1));
    if (grown == NULL) {
        return 1;
    }
    ctx->enums = grown;
    ctx->enums[ctx->enum_count] = enum_info;
    ctx->enum_count += 1;

    if (enum_info->items != NULL) {
        for (int i = 0; i < enum_info->item_count; i++) {
            EnumItemInfo *item = enum_info->items[i];
            if (item == NULL || item->name == NULL) {
                continue;
            }
            memset(&sym, 0, sizeof(Symbol));
            sym.kind = SYMBOL_ENUM_ITEM;
            sym.name = item->name;
            sym.info.enum_item_info = item;
            sym.scope_depth = 0;
            if (AddSymbolToTable(ctx, &sym) != 0) {
                return 1;
            }
        }
    }
    return 0;
}

/* ============================================================================
   ПОИСК ИНФОРМАЦИИ ПО ИМЕНИ
   ============================================================================ */

FunctionInfo* LookupFunction(SemanticContext *ctx, const char *name) {
    if (ctx == NULL || name == NULL) {
        return NULL;
    }
    if (ctx->functions == NULL) {
        return NULL;
    }
    for (int i = 0; i < ctx->function_count; i++) {
        FunctionInfo *func = ctx->functions[i];
        if (func != NULL && func->name != NULL && strcmp(func->name, name) == 0) {
            return func;
        }
    }
    return NULL;
}

int HasFunctionName(SemanticContext *ctx, const char *name) {
    if (ctx == NULL || name == NULL) {
        return 0;
    }
    if (ctx->functions == NULL) {
        return 0;
    }
    for (int i = 0; i < ctx->function_count; i++) {
        FunctionInfo *func = ctx->functions[i];
        if (func != NULL && func->name != NULL && strcmp(func->name, name) == 0) {
            return 1;
        }
    }
    return 0;
}

FunctionInfo* LookupFunctionOverload(SemanticContext *ctx, const char *name,
                                     NExpr **args, int arg_count, int *is_ambiguous) {
    FunctionInfo *best = NULL;
    int best_score = -1;
    int ambiguous = 0;

    if (is_ambiguous != NULL) {
        *is_ambiguous = 0;
    }
    if (ctx == NULL || name == NULL || ctx->functions == NULL) {
        return NULL;
    }

    for (int i = 0; i < ctx->function_count; i++) {
        FunctionInfo *func = ctx->functions[i];
        int score = 0;
        if (func == NULL || func->name == NULL) {
            continue;
        }
        if (strcmp(func->name, name) != 0) {
            continue;
        }
        if (!CanMatchFunction(func, args, arg_count, ctx, &score)) {
            continue;
        }
        if (score > best_score) {
            best = func;
            best_score = score;
            ambiguous = 0;
        } else if (score == best_score) {
            ambiguous = 1;
        }
    }

    if (is_ambiguous != NULL) {
        *is_ambiguous = ambiguous;
    }
    if (ambiguous) {
        return NULL;
    }
    return best;
}

ClassInfo* LookupClass(SemanticContext *ctx, const char *name) {
    if (ctx == NULL || name == NULL) {
        return NULL;
    }
    if (ctx->classes == NULL) {
        return NULL;
    }
    for (int i = 0; i < ctx->class_count; i++) {
        ClassInfo *cls = ctx->classes[i];
        if (cls != NULL && cls->name != NULL && strcmp(cls->name, name) == 0) {
            return cls;
        }
    }
    return NULL;
}

EnumInfo* LookupEnum(SemanticContext *ctx, const char *name) {
    if (ctx == NULL || name == NULL) {
        return NULL;
    }
    if (ctx->enums == NULL) {
        return NULL;
    }
    for (int i = 0; i < ctx->enum_count; i++) {
        EnumInfo *en = ctx->enums[i];
        if (en != NULL && en->name != NULL && strcmp(en->name, name) == 0) {
            return en;
        }
    }
    return NULL;
}

FieldInfo* LookupClassField(ClassInfo *class_info, const char *field_name) {
    if (class_info == NULL || field_name == NULL) {
        return NULL;
    }
    if (class_info->fields == NULL) {
        return NULL;
    }
    for (int i = 0; i < class_info->field_count; i++) {
        FieldInfo *field = class_info->fields[i];
        if (field != NULL && field->name != NULL && strcmp(field->name, field_name) == 0) {
            return field;
        }
    }
    return NULL;
}

FieldInfo* LookupClassFieldInHierarchy(SemanticContext *ctx, ClassInfo *class_info, const char *field_name) {
    ClassInfo *current = class_info;
    while (current != NULL) {
        FieldInfo *field = LookupClassField(current, field_name);
        if (field != NULL) {
            return field;
        }
        if (current->base_class == NULL || ctx == NULL) {
            break;
        }
        current = LookupClass(ctx, current->base_class);
    }
    return NULL;
}

MethodInfo* LookupClassMethod(ClassInfo *class_info, const char *method_name) {
    if (class_info == NULL || method_name == NULL) {
        return NULL;
    }
    if (class_info->methods == NULL) {
        return NULL;
    }
    for (int i = 0; i < class_info->method_count; i++) {
        MethodInfo *method = class_info->methods[i];
        if (method != NULL && method->name != NULL && strcmp(method->name, method_name) == 0) {
            return method;
        }
    }
    return NULL;
}

MethodInfo* LookupClassMethodInHierarchy(SemanticContext *ctx, ClassInfo *class_info, const char *method_name) {
    ClassInfo *current = class_info;
    while (current != NULL) {
        MethodInfo *method = LookupClassMethod(current, method_name);
        if (method != NULL) {
            return method;
        }
        if (current->base_class == NULL || ctx == NULL) {
            break;
        }
        current = LookupClass(ctx, current->base_class);
    }
    return NULL;
}

EnumItemInfo* LookupEnumItem(EnumInfo *enum_info, const char *item_name) {
    if (enum_info == NULL || item_name == NULL) {
        return NULL;
    }
    if (enum_info->items == NULL) {
        return NULL;
    }
    for (int i = 0; i < enum_info->item_count; i++) {
        EnumItemInfo *item = enum_info->items[i];
        if (item != NULL && item->name != NULL && strcmp(item->name, item_name) == 0) {
            return item;
        }
    }
    return NULL;
}

/* ============================================================================
   ПРОВЕРКА ДОСТУПА
   ============================================================================ */

int IsFieldAccessible(FieldInfo *field, int inside_class, int inside_hierarchy) {

    if (field == NULL) {
        return 0;
    }
    switch (field->access) {
        case ACCESS_PUBLIC:
            return 1;
        case ACCESS_PRIVATE:
            return inside_class ? 1 : 0;
        case ACCESS_PROTECTED:
            return inside_hierarchy ? 1 : 0;
        default:
            return 0;
    }
}

int IsMethodAccessible(MethodInfo *method, int inside_class, int inside_hierarchy) {
    if (method == NULL) {
        return 0;
    }
    switch (method->access) {
        case ACCESS_PUBLIC:
            return 1;
        case ACCESS_PRIVATE:
            return inside_class ? 1 : 0;
        case ACCESS_PROTECTED:
            return inside_hierarchy ? 1 : 0;
        default:
            return 0;
    }
}
