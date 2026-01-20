
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "name_resolution.h"
#include "error_reporting.h"

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
    /* TODO: Добавить функцию в контекст
       - Проверить, что функция не дублируется (LookupGlobalSymbol)
       - Если дублируется - добавить ошибку и вернуть 1
       - Создать Symbol с типом SYMBOL_FUNCTION
       - Добавить в глобальную таблицу (AddSymbolToTable)
       - Добавить в ctx->functions для быстрого доступа
       - Вернуть 0 если успешно
       Использует: CreateDuplicateSymbolError, AddSymbolToTable */
    return 0;
}

int AddClassToContext(SemanticContext *ctx, ClassInfo *class_info) {
    /* TODO: Добавить класс в контекст
       - Проверить, что класс не дублируется
       - Если базовый класс указан - проверить что он существует (LookupClass)
       - Если базовый класс не найден - добавить ошибку и вернуть 1
       - Создать Symbol с типом SYMBOL_CLASS
       - Добавить в глобальную таблицу
       - Добавить в ctx->classes для быстрого доступа
       - Вернуть 0 если успешно
       Использует: CreateDuplicateSymbolError, CreateInvalidBaseClassError,
                   AddSymbolToTable, LookupClass */
    return 0;
}

int AddEnumToContext(SemanticContext *ctx, EnumInfo *enum_info) {
    /* TODO: Добавить enum в контекст
       - Если enum имеет имя - проверить что оно не дублируется
       - Проверить, что элементы enum'а не дублируются
       - Если дублирование - добавить ошибку и вернуть 1
       - Создать Symbol с типом SYMBOL_ENUM_TYPE
       - Добавить в глобальную таблицу
       - Добавить в ctx->enums для быстрого доступа
       - Вернуть 0 если успешно
       Использует: CreateDuplicateSymbolError, CreateEnumDuplicateItemError,
                   AddSymbolToTable */
    return 0;
}

/* ============================================================================
   ПОИСК ИНФОРМАЦИИ ПО ИМЕНИ
   ============================================================================ */

FunctionInfo* LookupFunction(SemanticContext *ctx, const char *name) {
    /* TODO: Найти функцию по имени
       - Пройти по ctx->functions
       - Найти функцию с нужным именем
       - Вернуть найденную FunctionInfo или NULL */
    return NULL;
}

ClassInfo* LookupClass(SemanticContext *ctx, const char *name) {
    /* TODO: Найти класс по имени
       - Пройти по ctx->classes
       - Найти класс с нужным именем
       - Вернуть найденный ClassInfo или NULL */
    return NULL;
}

EnumInfo* LookupEnum(SemanticContext *ctx, const char *name) {
    /* TODO: Найти enum по имени
       - Пройти по ctx->enums
       - Найти enum с нужным именем
       - Вернуть найденный EnumInfo или NULL */
    return NULL;
}

FieldInfo* LookupClassField(ClassInfo *class_info, const char *field_name) {
    /* TODO: Найти поле класса по имени
       - Пройти по class_info->fields
       - Найти поле с нужным именем
       - Вернуть найденное FieldInfo или NULL */
    return NULL;
}

MethodInfo* LookupClassMethod(ClassInfo *class_info, const char *method_name) {
    /* TODO: Найти метод класса по имени
       - Пройти по class_info->methods
       - Найти метод с нужным именем
       - Вернуть найденный MethodInfo или NULL */
    return NULL;
}

EnumItemInfo* LookupEnumItem(EnumInfo *enum_info, const char *item_name) {
    /* TODO: Найти элемент enum по имени
       - Пройти по enum_info->items
       - Найти элемент с нужным именем
       - Вернуть найденный EnumItemInfo или NULL */
    return NULL;
}

/* ============================================================================
   ПРОВЕРКА ДОСТУПА
   ============================================================================ */

int IsFieldAccessible(FieldInfo *field, int inside_class) {
    /* TODO: Проверить доступность поля класса
       - Если поле public - всегда доступно
       - Если поле private - доступно только изнутри класса
       - Если поле protected - доступно изнутри класса и наследников (упрощённо: только изнутри)
       - Вернуть 1 если доступно, 0 если нет
       Использует: inside_class (1 если код находится внутри класса) */
    return 1;
}

int IsMethodAccessible(MethodInfo *method, int inside_class) {
    /* TODO: Проверить доступность метода класса
       - Та же логика как для полей
       - Вернуть 1 если доступно, 0 если нет */
    return 1;
}
