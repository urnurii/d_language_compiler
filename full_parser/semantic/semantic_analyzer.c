#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "semantic_analyzer.h"
#include "name_resolution.h"
#include "type_inference.h"
#include "error_reporting.h"
#include "semantic_passes.h"

static int FirstPassCollectDeclarations(NProgram *root, SemanticContext *ctx);
static int SecondPassCheckSemantics(NProgram *root, SemanticContext *ctx);
static int ThirdPassAttributeAST(NProgram *root, SemanticContext *ctx);

/* ============================================================================
   СОЗДАНИЕ И ИНИЦИАЛИЗАЦИЯ КОНТЕКСТА
   ============================================================================ */

SemanticContext* CreateSemanticContext(void) {
    SemanticContext *ctx = (SemanticContext*)malloc(sizeof(SemanticContext));
    SymbolTable *global = NULL;
    ScopeStack *scopes = NULL;

    if (ctx == NULL) {
        return NULL;
    }
    memset(ctx, 0, sizeof(SemanticContext));

    global = (SymbolTable*)malloc(sizeof(SymbolTable));
    if (global == NULL) {
        free(ctx);
        return NULL;
    }
    global->count = 0;
    global->capacity = 16;
    global->symbols = (Symbol**)malloc(sizeof(Symbol*) * (size_t)global->capacity);
    if (global->symbols == NULL) {
        free(global);
        free(ctx);
        return NULL;
    }

    scopes = (ScopeStack*)malloc(sizeof(ScopeStack));
    if (scopes == NULL) {
        free(global->symbols);
        free(global);
        free(ctx);
        return NULL;
    }
    scopes->count = 0;
    scopes->capacity = 8;
    scopes->scopes = (Scope**)malloc(sizeof(Scope*) * (size_t)scopes->capacity);
    if (scopes->scopes == NULL) {
        free(scopes);
        free(global->symbols);
        free(global);
        free(ctx);
        return NULL;
    }
    scopes->global = global;

    ctx->global_symbols = global;
    ctx->scope_stack = scopes;
    ctx->errors = CreateErrorList();
    ctx->has_errors = 0;
    ctx->classes = NULL;
    ctx->class_count = 0;
    ctx->functions = NULL;
    ctx->function_count = 0;
    ctx->enums = NULL;
    ctx->enum_count = 0;

    return ctx;
}

void DestroySemanticContext(SemanticContext *ctx) {
    if (ctx == NULL) {
        return;
    }

    if (ctx->errors != NULL) {
        DestroyErrorList(ctx->errors);
    }

    if (ctx->scope_stack != NULL) {
        free(ctx->scope_stack->scopes);
        free(ctx->scope_stack);
    }

    if (ctx->global_symbols != NULL) {
        free(ctx->global_symbols->symbols);
        free(ctx->global_symbols);
    }

    free(ctx->classes);
    free(ctx->functions);
    free(ctx->enums);
    free(ctx);
}

/* ============================================================================
   ГЛАВНАЯ ФУНКЦИЯ АНАЛИЗА
   ============================================================================ */

int AnalyzeProgram(NProgram *root, SemanticContext **ctx) {
    SemanticContext *local_ctx;
    int has_errors;

    if (ctx == NULL) {
        return 1;
    }

    local_ctx = CreateSemanticContext();
    if (local_ctx == NULL) {
        return 1;
    }

    if (FirstPassCollectDeclarations(root, local_ctx) != 0) {
        *ctx = local_ctx;
        return 1;
    }

    if (SecondPassCheckSemantics(root, local_ctx) != 0) {
        *ctx = local_ctx;
        return 1;
    }

    if (ThirdPassAttributeAST(root, local_ctx) != 0) {
        *ctx = local_ctx;
        return 1;
    }

    has_errors = HasErrors(local_ctx->errors);
    *ctx = local_ctx;
    return has_errors ? 1 : 0;
}

/* ============================================================================
   ГЛАВНАЯ ФУНКЦИЯ ПЕРВОГО ПРОХОДА
   ============================================================================ */

static int FirstPassCollectDeclarations(NProgram *root, SemanticContext *ctx) {
    if (ctx == NULL) {
        return 1;
    }

    if (ProcessSourceItems(root, ctx) != 0) {
        return 1;
    }

    return HasErrors(ctx->errors) ? 1 : 0;
}

/* ============================================================================
   ГЛАВНАЯ ФУНКЦИЯ ВТОРОГО ПРОХОДА
   ============================================================================ */

static int SecondPassCheckSemantics(NProgram *root, SemanticContext *ctx) {
    if (ctx == NULL) {
        return 1;
    }

    if (CheckAllSemantics(root, ctx) != 0) {
        return 1;
    }

    return HasErrors(ctx->errors) ? 1 : 0;
}

/* ============================================================================
   ГЛАВНАЯ ФУНКЦИЯ ТРЕТЬЕГО ПРОХОДА
   ============================================================================ */

static int ThirdPassAttributeAST(NProgram *root, SemanticContext *ctx) {
    if (ctx == NULL) {
        return 1;
    }
    (void)root;
    return HasErrors(ctx->errors) ? 1 : 0;
}

/* ============================================================================
   ВЫВОД ИНФОРМАЦИИ
   ============================================================================ */

void PrintSemanticErrors(SemanticContext *ctx) {
    if (ctx == NULL) {
        return;
    }
    PrintAllErrors(ctx->errors);
}

void PrintSymbolTables(SemanticContext *ctx) {
    /* TODO: Вывести все таблицы символов для отладки
       - Вывести заголовок "===== SYMBOL TABLES ====="
       - Вывести глобальные переменные
       - Вывести глобальные функции
       - Вывести все классы (и их поля, методы)
       - Вывести все enum'ы
       - Каждый символ в формате: "  name: type (details)"
       Результат: красивый вывод для отладки */
}
