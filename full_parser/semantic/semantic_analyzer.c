#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "semantic_analyzer.h"
#include "name_resolution.h"
#include "type_inference.h"
#include "error_reporting.h"
#include "semantic_passes.h"
#include "jvm_layout.h"
#include "semantic_transforms.h"

static int FirstPassCollectDeclarations(NProgram *root, SemanticContext *ctx);
static int SecondPassCheckSemantics(NProgram *root, SemanticContext *ctx);
static int ThirdPassAttributeAST(NProgram *root, SemanticContext *ctx);
static int FourthPassTransformAST(NProgram *root, SemanticContext *ctx);

/* ============================================================================
   СОЗДАНИЕ И ИНИЦИАЛИЗАЦИЯ КОНТЕКСТА
   ============================================================================ */

SemanticContext* CreateSemanticContext(void) {
    SemanticContext *ctx = (SemanticContext*)malloc(sizeof(SemanticContext));
    SymbolTable *global = NULL;
    ScopeStack *stack = NULL;

    if (ctx == NULL) {
        return NULL;
    }
    memset(ctx, 0, sizeof(SemanticContext));

    global = CreateSymbolTable(16);
    if (global == NULL) {
        free(ctx);
        return NULL;
    }

    stack = CreateScopeStack(global);
    if (stack == NULL) {
        DestroySymbolTable(global);
        free(ctx);
        return NULL;
    }

    ctx->global_symbols = global;
    ctx->scope_stack = stack;
    ctx->errors = CreateErrorList();
    ctx->jvm = CreateJvmLayoutContext();
    if (ctx->jvm == NULL) {
        DestroyErrorList(ctx->errors);
        DestroyScopeStack(ctx->scope_stack);
        DestroySymbolTable(ctx->global_symbols);
        free(ctx);
        return NULL;
    }
    ctx->has_errors = 0;
    ctx->current_class = NULL;
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

    if (ctx->jvm != NULL) {
        DestroyJvmLayoutContext(ctx->jvm);
    }

    if (ctx->scope_stack != NULL) {
        DestroyScopeStack(ctx->scope_stack);
    }

    if (ctx->global_symbols != NULL) {
        DestroySymbolTable(ctx->global_symbols);
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

    if (FourthPassTransformAST(root, local_ctx) != 0) {
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
   FOURTH PASS: AST TRANSFORMS FOR CODEGEN
   ============================================================================ */

static int FourthPassTransformAST(NProgram *root, SemanticContext *ctx) {
    if (ctx == NULL) {
        return 1;
    }
    if (TransformProgram(root, ctx) != 0) {
        return 1;
    }
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


static const char *AccessSpecToString(AccessSpec access) {
    switch (access) {
        case ACCESS_PUBLIC:
            return "public";
        case ACCESS_PRIVATE:
            return "private";
        case ACCESS_PROTECTED:
            return "protected";
        default:
            return "unknown";
    }
}

static const char *BoolToYesNo(int value) {
    return value ? "yes" : "no";
}

void PrintSymbolTables(SemanticContext *ctx) {
    if (ctx == NULL) {
        return;
    }

    fprintf(stdout, "===== SYMBOL TABLES =====
");

    fprintf(stdout, "Global Variables:
");
    if (ctx->global_symbols != NULL) {
        for (int i = 0; i < ctx->global_symbols->count; i++) {
            Symbol *sym = ctx->global_symbols->symbols[i];
            if (sym == NULL || sym->kind != SYMBOL_VARIABLE || sym->info.var_info == NULL) {
                continue;
            }
            VariableInfo *var = sym->info.var_info;
            fprintf(stdout, "  %s: %s (initialized=%s, line=%d, column=%d)
",
                    var->name ? var->name : "<unnamed>",
                    TypeToString(var->type),
                    BoolToYesNo(var->is_initialized),
                    var->line,
                    var->column);
        }
    }

    fprintf(stdout, "Global Functions:
");
    if (ctx->functions != NULL) {
        for (int i = 0; i < ctx->function_count; i++) {
            FunctionInfo *func = ctx->functions[i];
            const char *ret_type = (func != NULL && func->return_type != NULL)
                ? TypeToString(func->return_type)
                : "void";
            int param_count = (func != NULL && func->params != NULL) ? func->params->count : 0;
            fprintf(stdout, "  %s: %s (params=%d, line=%d, column=%d)
",
                    (func && func->name) ? func->name : "<unnamed>",
                    ret_type,
                    param_count,
                    func ? func->line : 0,
                    func ? func->column : 0);
        }
    }

    fprintf(stdout, "Classes:
");
    if (ctx->classes != NULL) {
        for (int i = 0; i < ctx->class_count; i++) {
            ClassInfo *cls = ctx->classes[i];
            const char *base = (cls && cls->base_class) ? cls->base_class : "<none>";
            fprintf(stdout, "  %s: class (base=%s, line=%d, column=%d)
",
                    (cls && cls->name) ? cls->name : "<unnamed>",
                    base,
                    cls ? cls->line : 0,
                    cls ? cls->column : 0);
            if (cls == NULL) {
                continue;
            }
            for (int f = 0; f < cls->field_count; f++) {
                FieldInfo *field = cls->fields ? cls->fields[f] : NULL;
                if (field == NULL) {
                    continue;
                }
                fprintf(stdout, "    %s: %s (field, access=%s, line=%d, column=%d)
",
                        field->name ? field->name : "<unnamed>",
                        TypeToString(field->type),
                        AccessSpecToString(field->access),
                        field->line,
                        field->column);
            }
            for (int m = 0; m < cls->method_count; m++) {
                MethodInfo *method = cls->methods ? cls->methods[m] : NULL;
                if (method == NULL) {
                    continue;
                }
                fprintf(stdout,
                        "    %s: %s (method, access=%s, params=%d, override=%s, line=%d, column=%d)
",
                        method->name ? method->name : "<unnamed>",
                        method->return_type ? TypeToString(method->return_type) : "void",
                        AccessSpecToString(method->access),
                        method->params ? method->params->count : 0,
                        BoolToYesNo(method->is_override),
                        method->line,
                        method->column);
            }
        }
    }

    fprintf(stdout, "Enums:
");
    if (ctx->enums != NULL) {
        for (int i = 0; i < ctx->enum_count; i++) {
            EnumInfo *en = ctx->enums[i];
            fprintf(stdout, "  %s: enum (items=%d, line=%d, column=%d)
",
                    (en && en->name) ? en->name : "<anonymous>",
                    en ? en->item_count : 0,
                    en ? en->line : 0,
                    en ? en->column : 0);
            if (en == NULL) {
                continue;
            }
            for (int j = 0; j < en->item_count; j++) {
                EnumItemInfo *item = en->items ? en->items[j] : NULL;
                if (item == NULL) {
                    continue;
                }
                fprintf(stdout, "    %s: int (value=%d)
",
                        item->name ? item->name : "<unnamed>",
                        item->value);
            }
        }
    }
}

