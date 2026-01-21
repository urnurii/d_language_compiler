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
#include "ast_builder.h"

static int FirstPassCollectDeclarations(NProgram *root, SemanticContext *ctx);
static int SecondPassCheckSemantics(NProgram *root, SemanticContext *ctx);
static int ThirdPassAttributeAST(NProgram *root, SemanticContext *ctx);
static int FourthPassTransformAST(NProgram *root, SemanticContext *ctx);
static int RegisterBuiltinClasses(SemanticContext *ctx);
static int RegisterBuiltinFunctions(SemanticContext *ctx);

static void ReportAddLocalFailure(SemanticContext *ctx, const char *name, int line, int column) {
    Scope *scope;
    Symbol *existing;
    SemanticError err;

    if (ctx == NULL || ctx->errors == NULL || name == NULL) {
        return;
    }

    scope = GetCurrentScope(ctx);
    existing = LookupSymbol(ctx, name);
    if (existing != NULL && scope != NULL && existing->scope_depth == scope->depth) {
        err = CreateDuplicateSymbolError(name, line, column);
    } else {
        err = CreateCustomError(SEMANTIC_ERROR_OTHER, "Out of memory", line, column);
    }
    AddError(ctx->errors, &err);
}

static int AddParamVariable(SemanticContext *ctx, NParam *param) {
    int before_errors;
    int after_errors;
    VariableInfo *var;

    if (ctx == NULL || param == NULL || param->param_name == NULL) {
        return 1;
    }

    var = (VariableInfo*)malloc(sizeof(VariableInfo));
    if (var == NULL) {
        ReportAddLocalFailure(ctx, param->param_name, 0, 0);
        return 1;
    }
    memset(var, 0, sizeof(VariableInfo));
    var->name = param->param_name;
    var->type = param->param_type;
    var->is_initialized = 1;
    var->is_param = 1;
    var->is_ref = param->is_ref;
    var->line = 0;
    var->column = 0;

    before_errors = GetErrorCount(ctx->errors);
    if (AddLocalVariable(ctx, var) != 0) {
        after_errors = GetErrorCount(ctx->errors);
        if (after_errors == before_errors) {
            ReportAddLocalFailure(ctx, param->param_name, 0, 0);
        }
        return 1;
    }

    return 0;
}

// ----- Создание и инциализация контекста -----

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
    ctx->loop_depth = 0;
    ctx->switch_depth = 0;
    ctx->classes = NULL;
    ctx->class_count = 0;
    ctx->functions = NULL;
    ctx->function_count = 0;
    ctx->enums = NULL;
    ctx->enum_count = 0;

    if (RegisterBuiltinClasses(ctx) != 0) {
        DestroySemanticContext(ctx);
        return NULL;
    }

    if (RegisterBuiltinFunctions(ctx) != 0) {
        DestroySemanticContext(ctx);
        return NULL;
    }

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

// ----- Главная функция анализа -----

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

// ----- Главная функция первого прохода -----

static int FirstPassCollectDeclarations(NProgram *root, SemanticContext *ctx) {
    if (ctx == NULL) {
        return 1;
    }

    if (ProcessSourceItems(root, ctx) != 0) {
        return 1;
    }

    return HasErrors(ctx->errors) ? 1 : 0;
}

// ----- Главная функция 2го прохода -----

static int SecondPassCheckSemantics(NProgram *root, SemanticContext *ctx) {
    if (ctx == NULL) {
        return 1;
    }

    if (CheckAllSemantics(root, ctx) != 0) {
        return 1;
    }

    return HasErrors(ctx->errors) ? 1 : 0;
}

// ----- Главная функция 3его прохода -----

static int ThirdPassAttributeAST(NProgram *root, SemanticContext *ctx) {
    if (ctx == NULL) {
        return 1;
    }
    if (root == NULL) {
        return 0;
    }

    {
        NSourceItem *item = root->first_item;
        while (item != NULL) {
            switch (item->type) {
                case SOURCE_ITEM_FUNC:
                    if (item->value.func && item->value.func->body) {
                        if (PushScope(ctx, item->value.func->func_name) == 0) {
                            if (item->value.func->params != NULL) {
                                for (int i = 0; i < item->value.func->params->count; i++) {
                                    NParam *param = item->value.func->params->params[i];
                                    if (param == NULL || param->param_name == NULL) {
                                        continue;
                                    }
                                    AddParamVariable(ctx, param);
                                }
                            }
                            AttributeStatements(item->value.func->body, ctx);
                            PopScope(ctx);
                        }
                    }
                    break;
                case SOURCE_ITEM_CLASS: {
                    NClassDef *class_def = item->value.class_def;
                    NClassMember *member = class_def ? class_def->members.first : NULL;
                    ClassInfo *saved = ctx->current_class;
                    if (class_def && class_def->class_name) {
                        ctx->current_class = LookupClass(ctx, class_def->class_name);
                    }
                    while (member != NULL) {
                        if (member->type == CLASS_MEMBER_METHOD && member->value.method) {
                            NMethodDef *method = member->value.method;
                            if (method->body != NULL) {
                                if (PushScope(ctx, method->method_name) == 0) {
                                    if (method->params != NULL) {
                                        for (int i = 0; i < method->params->count; i++) {
                                            NParam *param = method->params->params[i];
                                            if (param == NULL || param->param_name == NULL) {
                                                continue;
                                            }
                                            AddParamVariable(ctx, param);
                                        }
                                    }
                                    AttributeStatements(method->body, ctx);
                                    PopScope(ctx);
                                }
                            }
                        } else if (member->type == CLASS_MEMBER_CTOR && member->value.ctor) {
                            NCtorDef *ctor = member->value.ctor;
                            if (ctor->body != NULL) {
                                if (PushScope(ctx, "ctor") == 0) {
                                    if (ctor->params != NULL) {
                                        for (int i = 0; i < ctor->params->count; i++) {
                                            NParam *param = ctor->params->params[i];
                                            if (param == NULL || param->param_name == NULL) {
                                                continue;
                                            }
                                            AddParamVariable(ctx, param);
                                        }
                                    }
                                    AttributeStatements(ctor->body, ctx);
                                    PopScope(ctx);
                                }
                            }
                        } else if (member->type == CLASS_MEMBER_DTOR && member->value.dtor) {
                            NDtorDef *dtor = member->value.dtor;
                            if (dtor->body != NULL) {
                                if (PushScope(ctx, "dtor") == 0) {
                                    AttributeStatements(dtor->body, ctx);
                                    PopScope(ctx);
                                }
                            }
                        }
                        member = member->next;
                    }
                    ctx->current_class = saved;
                    break;
                }
                case SOURCE_ITEM_DECL: {
                    NInitDeclList *init_decls = item->value.decl.init_decls;
                    if (init_decls != NULL) {
                        for (int i = 0; i < init_decls->count; i++) {
                            NInitDecl *decl = init_decls->decls[i];
                            if (decl && decl->initializer) {
                                if (decl->initializer->is_array) {
                                    for (int j = 0; j < decl->initializer->array_init.count; j++) {
                                        AttributeExpressions(decl->initializer->array_init.elements[j], ctx);
                                    }
                                } else {
                                    AttributeExpressions(decl->initializer->expr, ctx);
                                }
                            }
                        }
                    }
                    break;
                }
                case SOURCE_ITEM_ENUM:
                    break;
            }
            item = item->next;
        }
    }

    return HasErrors(ctx->errors) ? 1 : 0;
}

// ----- Чертвёртый проход: трансформация дерева в код ген -----

static int FourthPassTransformAST(NProgram *root, SemanticContext *ctx) {
    if (ctx == NULL) {
        return 1;
    }
    if (TransformProgram(root, ctx) != 0) {
        return 1;
    }
    return HasErrors(ctx->errors) ? 1 : 0;
}

// ----- Вывод информации -----

void PrintSemanticErrors(SemanticContext *ctx) {
    if (ctx == NULL) {
        return;
    }
    PrintAllErrors(ctx->errors);
}

static int AddBuiltinFunction(SemanticContext *ctx, const char *name, NType *return_type,
                              NParamList *params, int allow_extra_args) {
    FunctionInfo *info;

    if (ctx == NULL || name == NULL) {
        return 1;
    }

    info = (FunctionInfo*)malloc(sizeof(FunctionInfo));
    if (info == NULL) {
        return 1;
    }
    memset(info, 0, sizeof(FunctionInfo));
    info->name = (char *)name;
    info->return_type = return_type;
    info->params = params;
    info->is_prototype = 0;
    info->is_builtin = 1;
    info->allow_extra_args = allow_extra_args;
    info->line = 0;
    info->column = 0;

    if (AddFunctionToContext(ctx, info) != 0) {
        free(info);
        return 1;
    }
    return 0;
}

static NParamList *CreateSingleStringParam(const char *name) {
    NParamList *params = CreateParamList();
    if (params == NULL) {
        return NULL;
    }
    AddParamToList(params, CreateParam(CreateBaseType(TYPE_STRING), (char *)name, 0, NULL));
    return params;
}

static int RegisterBuiltinFunctions(SemanticContext *ctx) {
    if (AddBuiltinFunction(ctx, "readln", CreateBaseType(TYPE_STRING), NULL, 0) != 0) {
        return 1;
    }
    if (AddBuiltinFunction(ctx, "readf", CreateBaseType(TYPE_INT), CreateSingleStringParam("fmt"), 1) != 0) {
        return 1;
    }
    if (AddBuiltinFunction(ctx, "write", NULL, NULL, 1) != 0) {
        return 1;
    }
    if (AddBuiltinFunction(ctx, "writeln", NULL, NULL, 1) != 0) {
        return 1;
    }
    if (AddBuiltinFunction(ctx, "writef", NULL, CreateSingleStringParam("fmt"), 1) != 0) {
        return 1;
    }
    return 0;
}

static int RegisterBuiltinClasses(SemanticContext *ctx) {
    ClassInfo *info;

    if (ctx == NULL) {
        return 1;
    }
    if (LookupClass(ctx, "Object") != NULL) {
        return 0;
    }

    info = (ClassInfo*)malloc(sizeof(ClassInfo));
    if (info == NULL) {
        return 1;
    }
    memset(info, 0, sizeof(ClassInfo));
    info->name = "Object";
    info->base_class = NULL;
    info->fields = NULL;
    info->field_count = 0;
    info->methods = NULL;
    info->method_count = 0;
    info->constructors = NULL;
    info->constructor_count = 0;
    info->destructor = NULL;
    info->members_processed = 1;
    info->line = 0;
    info->column = 0;

    if (AddClassToContext(ctx, info) != 0) {
        free(info);
        return 1;
    }
    return 0;
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

