#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "semantic_passes.h"
#include "name_resolution.h"
#include "type_inference.h"
#include "error_reporting.h"


static int ReportOutOfMemory(SemanticContext *ctx) {
    if (ctx != NULL && ctx->errors != NULL) {
        SemanticError err = CreateCustomError(SEMANTIC_ERROR_OTHER, "Out of memory", 0, 0);
        AddError(ctx->errors, &err);
    }
    return 1;
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

static const char *OpToString(OpType op) {
    switch (op) {
        case OP_PLUS: return "+";
        case OP_MINUS: return "-";
        case OP_MUL: return "*";
        case OP_DIV: return "/";
        case OP_LT: return "<";
        case OP_GT: return ">";
        case OP_LE: return "<=";
        case OP_GE: return ">=";
        case OP_EQ: return "==";
        case OP_NEQ: return "!=";
        case OP_AND: return "&&";
        case OP_OR: return "||";
        case OP_NOT: return "!";
        case OP_ASSIGN: return "=";
        case OP_PLUS_ASSIGN: return "+=";
        case OP_MINUS_ASSIGN: return "-=";
        case OP_MUL_ASSIGN: return "*=";
        case OP_DIV_ASSIGN: return "/=";
        case OP_BITWISE_NOT_ASSIGN: return "~=";
        default: return "?";
    }
}

/* ============================================================================
   ПЕРВЫЙ ПРОХОД: СБОР ВСЕХ ДЕКЛАРАЦИЙ
   ============================================================================ */

int ProcessSourceItems(NProgram *root, SemanticContext *ctx) {
    int had_error = 0;
    NSourceItem *item;

    if (root == NULL) {
        return 0;
    }
    if (ctx == NULL) {
        return 1;
    }

    item = root->first_item;
    while (item != NULL) {
        if (item->type == SOURCE_ITEM_CLASS) {
            if (ProcessClassDefinition(item->value.class_def, ctx) != 0) {
                had_error = 1;
            }
        }
        item = item->next;
    }

    item = root->first_item;
    while (item != NULL) {
        switch (item->type) {
            case SOURCE_ITEM_FUNC:
                if (ProcessFunctionDefinition(item->value.func, ctx) != 0) {
                    had_error = 1;
                }
                break;
            case SOURCE_ITEM_CLASS:
                break;
            case SOURCE_ITEM_DECL:
                if (ProcessGlobalVariables(item->value.decl.item_type,
                                           item->value.decl.init_decls,
                                           ctx) != 0) {
                    had_error = 1;
                }
                break;
            case SOURCE_ITEM_ENUM:
                if (ProcessEnumDefinition(item->value.enum_def, ctx) != 0) {
                    had_error = 1;
                }
                break;
        }
        item = item->next;
    }

    return had_error;
}


int ProcessFunctionDefinition(NFuncDef *func_def, SemanticContext *ctx) {
    FunctionInfo *info;

    if (func_def == NULL) {
        return 0;
    }
    if (ctx == NULL) {
        return 1;
    }

    if (func_def->return_type != NULL &&
        (func_def->return_type->kind == TYPE_KIND_CLASS ||
         func_def->return_type->kind == TYPE_KIND_CLASS_ARRAY)) {
        if (LookupClass(ctx, func_def->return_type->class_name) == NULL) {
            if (ctx->errors != NULL) {
                SemanticError err = CreateUndefinedTypeError(func_def->return_type->class_name,
                                                            0,
                                                            0);
                AddError(ctx->errors, &err);
            }
            return 1;
        }
    }

    info = (FunctionInfo*)malloc(sizeof(FunctionInfo));
    if (info == NULL) {
        return ReportOutOfMemory(ctx);
    }
    memset(info, 0, sizeof(FunctionInfo));
    info->name = func_def->func_name;
    info->return_type = func_def->return_type;
    info->params = func_def->params;
    info->line = 0;
    info->column = 0;

    return AddFunctionToContext(ctx, info);
}


int ProcessClassDefinition(NClassDef *class_def, SemanticContext *ctx) {
    ClassInfo *info;

    if (class_def == NULL) {
        return 0;
    }
    if (ctx == NULL) {
        return 1;
    }

    info = (ClassInfo*)malloc(sizeof(ClassInfo));
    if (info == NULL) {
        return ReportOutOfMemory(ctx);
    }
    memset(info, 0, sizeof(ClassInfo));
    info->name = class_def->class_name;
    info->base_class = class_def->base_class_name;
    info->fields = NULL;
    info->field_count = 0;
    info->methods = NULL;
    info->method_count = 0;
    info->constructor = NULL;
    info->destructor = NULL;
    info->line = 0;
    info->column = 0;

    return AddClassToContext(ctx, info);
}


int ProcessClassMembers(NClassMember *members, ClassInfo *class_info, SemanticContext *ctx) {
    int had_error = 0;
    NClassMember *member;

    if (members == NULL) {
        return 0;
    }
    if (class_info == NULL || ctx == NULL) {
        return 1;
    }

    member = members;
    while (member != NULL) {
        switch (member->type) {
            case CLASS_MEMBER_FIELD:
                if (ProcessFieldDeclaration(member->value.field.init_decls,
                                            member->value.field.field_type,
                                            member->access,
                                            class_info,
                                            ctx) != 0) {
                    had_error = 1;
                }
                break;
            case CLASS_MEMBER_METHOD:
                if (ProcessMethodDefinition(member->value.method,
                                            member->access,
                                            class_info,
                                            ctx) != 0) {
                    had_error = 1;
                }
                break;
            case CLASS_MEMBER_CTOR:
                class_info->constructor = member->value.ctor;
                break;
            case CLASS_MEMBER_DTOR:
                class_info->destructor = member->value.dtor;
                break;
            case CLASS_MEMBER_ENUM:
                break;
        }
        member = member->next;
    }

    return had_error;
}


int ProcessFieldDeclaration(NInitDeclList *init_decls, NType *field_type, 
                           AccessSpec access, ClassInfo *class_info, SemanticContext *ctx) {
    int had_error = 0;

    if (field_type == NULL) {
        return 1;
    }
    if (class_info == NULL || ctx == NULL) {
        return 1;
    }

    if (field_type->kind == TYPE_KIND_CLASS || field_type->kind == TYPE_KIND_CLASS_ARRAY) {
        if (LookupClass(ctx, field_type->class_name) == NULL) {
            if (ctx->errors != NULL) {
                SemanticError err = CreateUndefinedTypeError(field_type->class_name, 0, 0);
                AddError(ctx->errors, &err);
            }
            return 1;
        }
    }

    if (init_decls == NULL) {
        return 0;
    }

    for (int i = 0; i < init_decls->count; i++) {
        NInitDecl *decl = init_decls->decls[i];
        FieldInfo *field;
        FieldInfo **grown;
        if (decl == NULL) {
            continue;
        }
        field = (FieldInfo*)malloc(sizeof(FieldInfo));
        if (field == NULL) {
            return ReportOutOfMemory(ctx);
        }
        memset(field, 0, sizeof(FieldInfo));
        field->name = decl->name;
        field->type = field_type;
        field->access = access;
        field->line = 0;
        field->column = 0;

        grown = (FieldInfo**)realloc(class_info->fields,
                                     sizeof(FieldInfo*) * (size_t)(class_info->field_count + 1));
        if (grown == NULL) {
            free(field);
            return ReportOutOfMemory(ctx);
        }
        class_info->fields = grown;
        class_info->fields[class_info->field_count] = field;
        class_info->field_count += 1;
    }

    return had_error;
}


int ProcessMethodDefinition(NMethodDef *method_def, AccessSpec access, 
                           ClassInfo *class_info, SemanticContext *ctx) {
    MethodInfo *info;
    MethodInfo **grown;
    ClassInfo *base_info = NULL;
    MethodInfo *base_method = NULL;

    if (method_def == NULL) {
        return 0;
    }
    if (class_info == NULL || ctx == NULL) {
        return 1;
    }

    if (method_def->return_type != NULL &&
        (method_def->return_type->kind == TYPE_KIND_CLASS ||
         method_def->return_type->kind == TYPE_KIND_CLASS_ARRAY)) {
        if (LookupClass(ctx, method_def->return_type->class_name) == NULL) {
            if (ctx->errors != NULL) {
                SemanticError err = CreateUndefinedTypeError(method_def->return_type->class_name,
                                                            0,
                                                            0);
                AddError(ctx->errors, &err);
            }
            return 1;
        }
    }

    if (method_def->is_override) {
        if (class_info->base_class == NULL) {
            if (ctx->errors != NULL) {
                SemanticError err = CreateInvalidOverrideError(method_def->method_name,
                                                              "no base class",
                                                              0,
                                                              0);
                AddError(ctx->errors, &err);
            }
            return 1;
        }
        base_info = LookupClass(ctx, class_info->base_class);
        if (base_info == NULL) {
            if (ctx->errors != NULL) {
                SemanticError err = CreateInvalidOverrideError(method_def->method_name,
                                                              "base class not found",
                                                              0,
                                                              0);
                AddError(ctx->errors, &err);
            }
            return 1;
        }
        base_method = LookupClassMethod(base_info, method_def->method_name);
        if (base_method == NULL) {
            if (ctx->errors != NULL) {
                SemanticError err = CreateInvalidOverrideError(method_def->method_name,
                                                              "method not found in base class",
                                                              0,
                                                              0);
                AddError(ctx->errors, &err);
            }
            return 1;
        }
        if (!TypesEqual(base_method->return_type, method_def->return_type)) {
            if (ctx->errors != NULL) {
                SemanticError err = CreateInvalidOverrideError(method_def->method_name,
                                                              "return type mismatch",
                                                              0,
                                                              0);
                AddError(ctx->errors, &err);
            }
            return 1;
        }
        {
            int base_params = base_method->params ? base_method->params->count : 0;
            int this_params = method_def->params ? method_def->params->count : 0;
            if (base_params != this_params) {
                if (ctx->errors != NULL) {
                    SemanticError err = CreateInvalidOverrideError(method_def->method_name,
                                                                  "parameter count mismatch",
                                                                  0,
                                                                  0);
                    AddError(ctx->errors, &err);
                }
                return 1;
            }
            for (int i = 0; i < this_params; i++) {
                NParam *p = method_def->params->params[i];
                NParam *bp = base_method->params->params[i];
                if (p == NULL || bp == NULL) {
                    continue;
                }
                if (!TypesEqual(p->param_type, bp->param_type)) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateInvalidOverrideError(method_def->method_name,
                                                                      "parameter type mismatch",
                                                                      0,
                                                                      0);
                        AddError(ctx->errors, &err);
                    }
                    return 1;
                }
            }
        }
    }

    info = (MethodInfo*)malloc(sizeof(MethodInfo));
    if (info == NULL) {
        return ReportOutOfMemory(ctx);
    }
    memset(info, 0, sizeof(MethodInfo));
    info->name = method_def->method_name;
    info->return_type = method_def->return_type;
    info->params = method_def->params;
    info->access = access;
    info->is_override = method_def->is_override;
    info->line = 0;
    info->column = 0;

    grown = (MethodInfo**)realloc(class_info->methods,
                                  sizeof(MethodInfo*) * (size_t)(class_info->method_count + 1));
    if (grown == NULL) {
        free(info);
        return ReportOutOfMemory(ctx);
    }
    class_info->methods = grown;
    class_info->methods[class_info->method_count] = info;
    class_info->method_count += 1;
    return 0;
}


int ProcessEnumDefinition(NEnumDef *enum_def, SemanticContext *ctx) {
    EnumInfo *info;
    int next_value = 0;

    if (enum_def == NULL) {
        return 0;
    }
    if (ctx == NULL) {
        return 1;
    }

    info = (EnumInfo*)malloc(sizeof(EnumInfo));
    if (info == NULL) {
        return ReportOutOfMemory(ctx);
    }
    memset(info, 0, sizeof(EnumInfo));
    info->name = enum_def->enum_name;
    info->items = NULL;
    info->item_count = 0;
    info->line = 0;
    info->column = 0;

    if (enum_def->items.items != NULL) {
        for (int i = 0; i < enum_def->items.count; i++) {
            NEnumItem *src = enum_def->items.items[i];
            EnumItemInfo *item;
            EnumItemInfo **grown;
            if (src == NULL) {
                continue;
            }
            for (int j = 0; j < info->item_count; j++) {
                EnumItemInfo *prev = info->items[j];
                if (prev != NULL && prev->name != NULL && src->name != NULL &&
                    strcmp(prev->name, src->name) == 0) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateEnumDuplicateItemError(src->name, 0, 0);
                        AddError(ctx->errors, &err);
                    }
                    return 1;
                }
            }
            item = (EnumItemInfo*)malloc(sizeof(EnumItemInfo));
            if (item == NULL) {
                return ReportOutOfMemory(ctx);
            }
            memset(item, 0, sizeof(EnumItemInfo));
            item->name = src->name;
            item->has_explicit_value = src->has_value;
            if (src->has_value) {
                item->value = src->value;
                next_value = src->value + 1;
            } else {
                item->value = next_value;
                next_value += 1;
            }

            grown = (EnumItemInfo**)realloc(info->items,
                                            sizeof(EnumItemInfo*) * (size_t)(info->item_count + 1));
            if (grown == NULL) {
                free(item);
                return ReportOutOfMemory(ctx);
            }
            info->items = grown;
            info->items[info->item_count] = item;
            info->item_count += 1;
        }
    }

    return AddEnumToContext(ctx, info);
}


int ProcessGlobalVariables(NType *type, NInitDeclList *init_decls, SemanticContext *ctx) {
    int had_error = 0;

    if (type == NULL) {
        return 1;
    }
    if (ctx == NULL) {
        return 1;
    }

    if (type->kind == TYPE_KIND_CLASS || type->kind == TYPE_KIND_CLASS_ARRAY) {
        if (LookupClass(ctx, type->class_name) == NULL) {
            if (ctx->errors != NULL) {
                SemanticError err = CreateUndefinedTypeError(type->class_name, 0, 0);
                AddError(ctx->errors, &err);
            }
            return 1;
        }
    }

    if (init_decls == NULL) {
        return 0;
    }

    for (int i = 0; i < init_decls->count; i++) {
        NInitDecl *decl = init_decls->decls[i];
        VariableInfo *var;
        if (decl == NULL) {
            continue;
        }
        var = (VariableInfo*)malloc(sizeof(VariableInfo));
        if (var == NULL) {
            return ReportOutOfMemory(ctx);
        }
        memset(var, 0, sizeof(VariableInfo));
        var->name = decl->name;
        var->type = type;
        var->is_initialized = (decl->initializer != NULL);
        var->is_param = 0;
        var->is_ref = 0;
        var->line = 0;
        var->column = 0;

        if (AddLocalVariable(ctx, var) != 0) {
            had_error = 1;
        }
    }

    return had_error;
}


/* ============================================================================
   ВТОРОЙ ПРОХОД: ПРОВЕРКА СЕМАНТИКИ
   ============================================================================ */

int CheckAllSemantics(NProgram *root, SemanticContext *ctx) {

    int had_error;

    if (root == NULL) {
        return 0;
    }
    if (ctx == NULL) {
        return 1;
    }

    had_error = CheckSourceItems(root->first_item, ctx);
    if (had_error != 0) {
        return 1;
    }

    return HasErrors(ctx->errors) ? 1 : 0;
}

int CheckSourceItems(NSourceItem *items, SemanticContext *ctx) {

    int had_error = 0;
    NSourceItem *item = items;

    if (items == NULL) {
        return 0;
    }
    if (ctx == NULL) {
        return 1;
    }

    while (item != NULL) {
        switch (item->type) {
            case SOURCE_ITEM_FUNC: {
                NFuncDef *func = item->value.func;
                FunctionInfo *func_info = NULL;
                if (func != NULL && func->func_name != NULL) {
                    func_info = LookupFunction(ctx, func->func_name);
                }
                if (CheckFunctionBody(func ? func->body : NULL, func_info, ctx) != 0) {
                    had_error = 1;
                }
                break;
            }
            case SOURCE_ITEM_CLASS: {
                NClassDef *class_def = item->value.class_def;
                NClassMember *member = class_def ? class_def->members.first : NULL;
                ClassInfo *saved_class = ctx->current_class;
                if (class_def != NULL && class_def->class_name != NULL) {
                    ctx->current_class = LookupClass(ctx, class_def->class_name);
                }
                if (ctx->current_class != NULL && ctx->current_class->base_class != NULL) {
                    if (LookupClass(ctx, ctx->current_class->base_class) == NULL) {
                        if (ctx->errors != NULL) {
                            SemanticError err = CreateInvalidBaseClassError(ctx->current_class->base_class,
                                                                           0,
                                                                           0);
                            AddError(ctx->errors, &err);
                        }
                        had_error = 1;
                    }
                }
                if (ctx->current_class != NULL) {
                    if (ProcessClassMembers(member, ctx->current_class, ctx) != 0) {
                        had_error = 1;
                    }
                }
                while (member != NULL) {
                    if (member->type == CLASS_MEMBER_FIELD) {
                        NInitDeclList *init_decls = member->value.field.init_decls;
                        if (init_decls != NULL) {
                            for (int i = 0; i < init_decls->count; i++) {
                                NInitDecl *decl = init_decls->decls[i];
                                if (decl && decl->initializer) {
                                    if (decl->initializer->is_array) {
                                        for (int j = 0; j < decl->initializer->array_init.count; j++) {
                                            if (CheckExpressions(decl->initializer->array_init.elements[j], ctx) != 0) {
                                                had_error = 1;
                                            }
                                        }
                                    } else {
                                        if (CheckExpressions(decl->initializer->expr, ctx) != 0) {
                                            had_error = 1;
                                        }
                                    }
                                }
                            }
                        }
                    } else if (member->type == CLASS_MEMBER_METHOD) {
                        NMethodDef *method = member->value.method;
                        FunctionInfo tmp_info;
                        if (method != NULL) {
                            memset(&tmp_info, 0, sizeof(FunctionInfo));
                            tmp_info.name = method->method_name;
                            tmp_info.return_type = method->return_type;
                            tmp_info.params = method->params;
                            if (CheckFunctionBody(method->body, &tmp_info, ctx) != 0) {
                                had_error = 1;
                            }
                        }
                    } else if (member->type == CLASS_MEMBER_CTOR) {
                        NCtorDef *ctor = member->value.ctor;
                        FunctionInfo tmp_info;
                        if (ctor != NULL) {
                            memset(&tmp_info, 0, sizeof(FunctionInfo));
                            tmp_info.name = "ctor";
                            tmp_info.return_type = NULL;
                            tmp_info.params = ctor->params;
                            if (CheckFunctionBody(ctor->body, &tmp_info, ctx) != 0) {
                                had_error = 1;
                            }
                        }
                    } else if (member->type == CLASS_MEMBER_DTOR) {
                        NDtorDef *dtor = member->value.dtor;
                        FunctionInfo tmp_info;
                        if (dtor != NULL) {
                            memset(&tmp_info, 0, sizeof(FunctionInfo));
                            tmp_info.name = "dtor";
                            tmp_info.return_type = NULL;
                            tmp_info.params = NULL;
                            if (CheckFunctionBody(dtor->body, &tmp_info, ctx) != 0) {
                                had_error = 1;
                            }
                        }
                    }
                    member = member->next;
                }
                ctx->current_class = saved_class;
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
                                    if (CheckExpressions(decl->initializer->array_init.elements[j], ctx) != 0) {
                                        had_error = 1;
                                    }
                                }
                            } else {
                                if (CheckExpressions(decl->initializer->expr, ctx) != 0) {
                                    had_error = 1;
                                }
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

    return had_error;
}

int CheckFunctionBody(NStmt *func_body, FunctionInfo *func_info, SemanticContext *ctx) {

    int had_error = 0;
    const char *scope_name = (func_info && func_info->name) ? func_info->name : "function";

    if (func_body == NULL) {
        return 0;
    }
    if (ctx == NULL) {
        return 1;
    }

    if (PushScope(ctx, scope_name) != 0) {
        return 1;
    }

    if (func_info != NULL && func_info->params != NULL) {
        for (int i = 0; i < func_info->params->count; i++) {
            NParam *param = func_info->params->params[i];
            VariableInfo *var;
            if (param == NULL || param->param_name == NULL) {
                continue;
            }
            var = (VariableInfo*)malloc(sizeof(VariableInfo));
            if (var == NULL) {
                had_error = ReportOutOfMemory(ctx);
                continue;
            }
            memset(var, 0, sizeof(VariableInfo));
            var->name = param->param_name;
            var->type = param->param_type;
            var->is_initialized = 1;
            var->is_param = 1;
            var->is_ref = param->is_ref;
            var->line = 0;
            var->column = 0;

            if (AddLocalVariable(ctx, var) != 0) {
                had_error = 1;
            }
        }
    }

    if (CheckStatements(func_body, ctx) != 0) {
        had_error = 1;
    }

    if (PopScope(ctx) != 0) {
        return 1;
    }

    return had_error;
}

int CheckStatements(NStmt *stmts, SemanticContext *ctx) {

    int had_error = 0;
    NStmt *stmt = stmts;

    if (stmts == NULL) {
        return 0;
    }
    if (ctx == NULL) {
        return 1;
    }

    while (stmt != NULL) {
        if (CheckStatement(stmt, ctx) != 0) {
            had_error = 1;
        }
        stmt = stmt->next;
    }

    return had_error;
}

int CheckStatement(NStmt *stmt, SemanticContext *ctx) {

    int had_error = 0;
    Scope *scope;

    if (stmt == NULL) {
        return 0;
    }
    if (ctx == NULL) {
        return 1;
    }

    scope = GetCurrentScope(ctx);
    stmt->scope_id = scope ? scope->depth : -1;

    switch (stmt->type) {
        case STMT_EXPR:
            if (CheckExpressions(stmt->value.expr, ctx) != 0) {
                had_error = 1;
            }
            break;
        case STMT_DECL: {
            NType *decl_type = stmt->value.decl.decl_type;
            NInitDeclList *init_decls = stmt->value.decl.init_decls;
            if (decl_type != NULL &&
                (decl_type->kind == TYPE_KIND_CLASS || decl_type->kind == TYPE_KIND_CLASS_ARRAY)) {
                if (LookupClass(ctx, decl_type->class_name) == NULL) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateUndefinedTypeError(decl_type->class_name,
                                                                    stmt->line,
                                                                    stmt->column);
                        AddError(ctx->errors, &err);
                    }
                    had_error = 1;
                }
            }
            if (init_decls != NULL) {
                for (int i = 0; i < init_decls->count; i++) {
                    NInitDecl *decl = init_decls->decls[i];
                    VariableInfo *var;
                    if (decl == NULL || decl->name == NULL) {
                        continue;
                    }
                    var = (VariableInfo*)malloc(sizeof(VariableInfo));
                    if (var == NULL) {
                        had_error = ReportOutOfMemory(ctx);
                        continue;
                    }
                    memset(var, 0, sizeof(VariableInfo));
                    var->name = decl->name;
                    var->type = decl_type;
                    var->is_initialized = (decl->initializer != NULL);
                    var->is_param = 0;
                    var->is_ref = 0;
                    var->line = stmt->line;
                    var->column = stmt->column;

                    if (AddLocalVariable(ctx, var) != 0) {
                        had_error = 1;
                    }

                    if (decl->initializer != NULL) {
                        if (decl->initializer->is_array) {
                            for (int j = 0; j < decl->initializer->array_init.count; j++) {
                                if (CheckExpressions(decl->initializer->array_init.elements[j], ctx) != 0) {
                                    had_error = 1;
                                }
                            }
                        } else {
                            if (CheckExpressions(decl->initializer->expr, ctx) != 0) {
                                had_error = 1;
                            }
                            if (decl_type != NULL) {
                                NType *expr_type = InferExpressionType(decl->initializer->expr, ctx);
                                if (expr_type != NULL && !CanAssign(decl_type, expr_type)) {
                                    if (ctx->errors != NULL) {
                                        SemanticError err = CreateTypeMismatchError(TypeToString(decl_type),
                                                                                    TypeToString(expr_type),
                                                                                    "in declaration",
                                                                                    stmt->line,
                                                                                    stmt->column);
                                        AddError(ctx->errors, &err);
                                    }
                                    had_error = 1;
                                }
                            }
                        }
                    }
                }
            }
            break;
        }
        case STMT_IF: {
            NExpr *cond = stmt->value.if_stmt.condition;
            if (CheckExpressions(cond, ctx) != 0) {
                had_error = 1;
            }
            if (cond != NULL) {
                NType *cond_type = InferExpressionType(cond, ctx);
                if (cond_type != NULL && !CanBeCondition(cond_type)) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateTypeMismatchError("bool or numeric",
                                                                    TypeToString(cond_type),
                                                                    "as condition",
                                                                    cond->line,
                                                                    cond->column);
                        AddError(ctx->errors, &err);
                    }
                    had_error = 1;
                }
            }
            if (CheckStatement(stmt->value.if_stmt.then_stmt, ctx) != 0) {
                had_error = 1;
            }
            if (CheckStatement(stmt->value.if_stmt.else_stmt, ctx) != 0) {
                had_error = 1;
            }
            break;
        }
        case STMT_WHILE: {
            NExpr *cond = stmt->value.while_stmt.condition;
            if (CheckExpressions(cond, ctx) != 0) {
                had_error = 1;
            }
            if (cond != NULL) {
                NType *cond_type = InferExpressionType(cond, ctx);
                if (cond_type != NULL && !CanBeCondition(cond_type)) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateTypeMismatchError("bool or numeric",
                                                                    TypeToString(cond_type),
                                                                    "as condition",
                                                                    cond->line,
                                                                    cond->column);
                        AddError(ctx->errors, &err);
                    }
                    had_error = 1;
                }
            }
            if (CheckStatement(stmt->value.while_stmt.body, ctx) != 0) {
                had_error = 1;
            }
            break;
        }
        case STMT_DO_WHILE: {
            NExpr *cond = stmt->value.do_while_stmt.condition;
            if (CheckStatement(stmt->value.do_while_stmt.body, ctx) != 0) {
                had_error = 1;
            }
            if (CheckExpressions(cond, ctx) != 0) {
                had_error = 1;
            }
            if (cond != NULL) {
                NType *cond_type = InferExpressionType(cond, ctx);
                if (cond_type != NULL && !CanBeCondition(cond_type)) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateTypeMismatchError("bool or numeric",
                                                                    TypeToString(cond_type),
                                                                    "as condition",
                                                                    cond->line,
                                                                    cond->column);
                        AddError(ctx->errors, &err);
                    }
                    had_error = 1;
                }
            }
            break;
        }
        case STMT_FOR: {
            NExpr *cond = stmt->value.for_stmt.cond_expr;
            if (CheckExpressions(stmt->value.for_stmt.init_expr, ctx) != 0) {
                had_error = 1;
            }
            if (stmt->value.for_stmt.init_decl_type != NULL) {
                NType *decl_type = stmt->value.for_stmt.init_decl_type;
                NInitDeclList *init_decls = stmt->value.for_stmt.init_decls;
                if (decl_type->kind == TYPE_KIND_CLASS || decl_type->kind == TYPE_KIND_CLASS_ARRAY) {
                    if (LookupClass(ctx, decl_type->class_name) == NULL) {
                        if (ctx->errors != NULL) {
                            SemanticError err = CreateUndefinedTypeError(decl_type->class_name,
                                                                        stmt->line,
                                                                        stmt->column);
                            AddError(ctx->errors, &err);
                        }
                        had_error = 1;
                    }
                }
                if (init_decls != NULL) {
                    for (int i = 0; i < init_decls->count; i++) {
                        NInitDecl *decl = init_decls->decls[i];
                        VariableInfo *var;
                        if (decl == NULL || decl->name == NULL) {
                            continue;
                        }
                        var = (VariableInfo*)malloc(sizeof(VariableInfo));
                        if (var == NULL) {
                            had_error = ReportOutOfMemory(ctx);
                            continue;
                        }
                        memset(var, 0, sizeof(VariableInfo));
                        var->name = decl->name;
                        var->type = decl_type;
                        var->is_initialized = (decl->initializer != NULL);
                        var->is_param = 0;
                        var->is_ref = 0;
                        var->line = stmt->line;
                        var->column = stmt->column;

                        if (AddLocalVariable(ctx, var) != 0) {
                            had_error = 1;
                        }

                        if (decl->initializer != NULL) {
                            if (decl->initializer->is_array) {
                                for (int j = 0; j < decl->initializer->array_init.count; j++) {
                                    if (CheckExpressions(decl->initializer->array_init.elements[j], ctx) != 0) {
                                        had_error = 1;
                                    }
                                }
                            } else {
                                if (CheckExpressions(decl->initializer->expr, ctx) != 0) {
                                    had_error = 1;
                                }
                                if (decl_type != NULL) {
                                    NType *expr_type = InferExpressionType(decl->initializer->expr, ctx);
                                    if (expr_type != NULL && !CanAssign(decl_type, expr_type)) {
                                        if (ctx->errors != NULL) {
                                            SemanticError err = CreateTypeMismatchError(TypeToString(decl_type),
                                                                                        TypeToString(expr_type),
                                                                                        "in for init",
                                                                                        stmt->line,
                                                                                        stmt->column);
                                            AddError(ctx->errors, &err);
                                        }
                                        had_error = 1;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if (CheckExpressions(cond, ctx) != 0) {
                had_error = 1;
            }
            if (cond != NULL) {
                NType *cond_type = InferExpressionType(cond, ctx);
                if (cond_type != NULL && !CanBeCondition(cond_type)) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateTypeMismatchError("bool or numeric",
                                                                    TypeToString(cond_type),
                                                                    "as condition",
                                                                    cond->line,
                                                                    cond->column);
                        AddError(ctx->errors, &err);
                    }
                    had_error = 1;
                }
            }
            if (CheckExpressions(stmt->value.for_stmt.iter_expr, ctx) != 0) {
                had_error = 1;
            }
            if (CheckStatement(stmt->value.for_stmt.body, ctx) != 0) {
                had_error = 1;
            }
            break;
        }
        case STMT_FOREACH: {
            if (CheckExpressions(stmt->value.foreach_stmt.collection, ctx) != 0) {
                had_error = 1;
            }
            if (stmt->value.foreach_stmt.is_typed && stmt->value.foreach_stmt.var_type != NULL &&
                stmt->value.foreach_stmt.var_name != NULL) {
                VariableInfo *var = (VariableInfo*)malloc(sizeof(VariableInfo));
                if (var == NULL) {
                    had_error = ReportOutOfMemory(ctx);
                } else {
                    memset(var, 0, sizeof(VariableInfo));
                    var->name = stmt->value.foreach_stmt.var_name;
                    var->type = stmt->value.foreach_stmt.var_type;
                    var->is_initialized = 1;
                    var->is_param = 0;
                    var->is_ref = 0;
                    var->line = stmt->line;
                    var->column = stmt->column;
                    if (AddLocalVariable(ctx, var) != 0) {
                        had_error = 1;
                    }
                }
            }
            if (CheckStatement(stmt->value.foreach_stmt.body, ctx) != 0) {
                had_error = 1;
            }
            break;
        }
        case STMT_SWITCH: {
            if (CheckExpressions(stmt->value.switch_stmt.expr, ctx) != 0) {
                had_error = 1;
            }
            for (int i = 0; i < stmt->value.switch_stmt.cases.count; i++) {
                NCaseItem *item = stmt->value.switch_stmt.cases.items[i];
                if (item == NULL) {
                    continue;
                }
                if (item->case_expr != NULL) {
                    if (CheckExpressions(item->case_expr, ctx) != 0) {
                        had_error = 1;
                    }
                }
                if (item->stmts != NULL) {
                    if (CheckStatements(item->stmts->first, ctx) != 0) {
                        had_error = 1;
                    }
                }
            }
            break;
        }
        case STMT_RETURN:
            if (CheckExpressions(stmt->value.expr, ctx) != 0) {
                had_error = 1;
            }
            break;
        case STMT_BREAK:
        case STMT_CONTINUE:
            break;
        case STMT_COMPOUND:
            if (PushScope(ctx, "block") != 0) {
                had_error = 1;
                break;
            }
            if (CheckStatements(stmt->value.stmt_list ? stmt->value.stmt_list->first : NULL, ctx) != 0) {
                had_error = 1;
            }
            if (PopScope(ctx) != 0) {
                had_error = 1;
            }
            break;
    }

    return had_error;
}

int CheckExpressions(NExpr *root, SemanticContext *ctx) {

    if (root == NULL) {
        return 0;
    }
    if (ctx == NULL) {
        return 1;
    }
    return CheckExpression(root, ctx);
}

int CheckExpression(NExpr *expr, SemanticContext *ctx) {

    int had_error = 0;

    if (expr == NULL) {
        return 0;
    }
    if (ctx == NULL) {
        return 1;
    }

    switch (expr->type) {
        case EXPR_IDENT: {
            Symbol *sym = LookupSymbol(ctx, expr->value.ident_name);
            if (sym == NULL) {
                if (ctx->errors != NULL) {
                    SemanticError err = CreateUndefinedVariableError(expr->value.ident_name,
                                                                    expr->line,
                                                                    expr->column);
                    AddError(ctx->errors, &err);
                }
                had_error = 1;
            } else if (!IsSymbolAccessible(sym, ctx)) {
                if (ctx->errors != NULL) {
                    SemanticError err = CreateOutOfScopeError(expr->value.ident_name,
                                                              expr->line,
                                                              expr->column);
                    AddError(ctx->errors, &err);
                }
                had_error = 1;
            }
            break;
        }
        case EXPR_FUNC_CALL: {
            FunctionInfo *func = LookupFunction(ctx, expr->value.func_call.func_name);
            if (func == NULL) {
                if (ctx->errors != NULL) {
                    SemanticError err = CreateUndefinedFunctionError(expr->value.func_call.func_name,
                                                                    expr->line,
                                                                    expr->column);
                    AddError(ctx->errors, &err);
                }
                had_error = 1;
            } else {
                int expected = func->params ? func->params->count : 0;
                int actual = expr->value.func_call.arg_count;
                if (expected != actual) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateWrongArgCountError(func->name,
                                                                    expected,
                                                                    actual,
                                                                    expr->line,
                                                                    expr->column);
                        AddError(ctx->errors, &err);
                    }
                    had_error = 1;
                }
                for (int i = 0; i < expr->value.func_call.arg_count; i++) {
                    if (CheckExpression(expr->value.func_call.args[i], ctx) != 0) {
                        had_error = 1;
                    }
                }
                if (func->params != NULL) {
                    int count = expected < actual ? expected : actual;
                    for (int i = 0; i < count; i++) {
                        NParam *param = func->params->params[i];
                        NExpr *arg = expr->value.func_call.args[i];
                        if (param == NULL || arg == NULL) {
                            continue;
                        }
                        if (param->param_type != NULL) {
                            NType *arg_type = InferExpressionType(arg, ctx);
                            if (arg_type != NULL &&
                                !IsArgumentCompatibleWithParameter(param->param_type, arg_type, param->is_ref)) {
                                if (ctx->errors != NULL) {
                                    SemanticError err = CreateTypeMismatchError(TypeToString(param->param_type),
                                                                                TypeToString(arg_type),
                                                                                "as argument",
                                                                                arg->line,
                                                                                arg->column);
                                    AddError(ctx->errors, &err);
                                }
                                had_error = 1;
                            }
                        }
                    }
                }
            }
            break;
        }
        case EXPR_METHOD_CALL: {
            NExpr *obj = expr->value.member_access.object;
            if (CheckExpression(obj, ctx) != 0) {
                had_error = 1;
            }
            for (int i = 0; i < expr->value.member_access.arg_count; i++) {
                if (CheckExpression(expr->value.member_access.args[i], ctx) != 0) {
                    had_error = 1;
                }
            }
            if (obj != NULL) {
                NType *obj_type = InferExpressionType(obj, ctx);
                if (obj_type != NULL &&
                    (obj_type->kind == TYPE_KIND_CLASS || obj_type->kind == TYPE_KIND_CLASS_ARRAY)) {
                    const char *class_name = obj_type->class_name;
                    ClassInfo *class_info = LookupClass(ctx, class_name);
                    if (class_info == NULL) {
                        if (ctx->errors != NULL) {
                            SemanticError err = CreateUndefinedClassError(class_name,
                                                                          expr->line,
                                                                          expr->column);
                            AddError(ctx->errors, &err);
                        }
                        had_error = 1;
                    } else {
                        MethodInfo *method = LookupClassMethod(class_info, expr->value.member_access.member_name);
                        if (method == NULL) {
                            if (ctx->errors != NULL) {
                                SemanticError err = CreateMethodNotFoundError(expr->value.member_access.member_name,
                                                                              class_name,
                                                                              expr->line,
                                                                              expr->column);
                                AddError(ctx->errors, &err);
                            }
                            had_error = 1;
                        } else {
                            int inside_class = 0;
                            if (!IsMethodAccessible(method, inside_class)) {
                                if (ctx->errors != NULL) {
                                    SemanticError err = CreateAccessViolationError(method->name,
                                                                                  AccessSpecToString(method->access),
                                                                                  expr->line,
                                                                                  expr->column);
                                    AddError(ctx->errors, &err);
                                }
                                had_error = 1;
                            }
                            {
                                int expected = method->params ? method->params->count : 0;
                                int actual = expr->value.member_access.arg_count;
                                if (expected != actual) {
                                    if (ctx->errors != NULL) {
                                        SemanticError err = CreateWrongArgCountError(method->name,
                                                                                    expected,
                                                                                    actual,
                                                                                    expr->line,
                                                                                    expr->column);
                                        AddError(ctx->errors, &err);
                                    }
                                    had_error = 1;
                                }
                                if (method->params != NULL) {
                                    int count = expected < actual ? expected : actual;
                                    for (int i = 0; i < count; i++) {
                                        NParam *param = method->params->params[i];
                                        NExpr *arg = expr->value.member_access.args[i];
                                        if (param == NULL || arg == NULL) {
                                            continue;
                                        }
                                        if (param->param_type != NULL) {
                                            NType *arg_type = InferExpressionType(arg, ctx);
                                            if (arg_type != NULL &&
                                                !IsArgumentCompatibleWithParameter(param->param_type, arg_type, param->is_ref)) {
                                                if (ctx->errors != NULL) {
                                                    SemanticError err = CreateTypeMismatchError(TypeToString(param->param_type),
                                                                                                TypeToString(arg_type),
                                                                                                "as argument",
                                                                                                arg->line,
                                                                                                arg->column);
                                                    AddError(ctx->errors, &err);
                                                }
                                                had_error = 1;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                } else if (obj_type != NULL) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateMethodNotFoundError(expr->value.member_access.member_name,
                                                                      TypeToString(obj_type),
                                                                      expr->line,
                                                                      expr->column);
                        AddError(ctx->errors, &err);
                    }
                    had_error = 1;
                }
            }
            break;
        }
        case EXPR_MEMBER_ACCESS: {
            NExpr *obj = expr->value.member_access.object;
            if (CheckExpression(obj, ctx) != 0) {
                had_error = 1;
            }
            if (obj != NULL) {
                NType *obj_type = InferExpressionType(obj, ctx);
                if (obj_type != NULL &&
                    (obj_type->kind == TYPE_KIND_CLASS || obj_type->kind == TYPE_KIND_CLASS_ARRAY)) {
                    const char *class_name = obj_type->class_name;
                    ClassInfo *class_info = LookupClass(ctx, class_name);
                    if (class_info == NULL) {
                        if (ctx->errors != NULL) {
                            SemanticError err = CreateUndefinedClassError(class_name,
                                                                          expr->line,
                                                                          expr->column);
                            AddError(ctx->errors, &err);
                        }
                        had_error = 1;
                    } else {
                        FieldInfo *field = LookupClassField(class_info, expr->value.member_access.member_name);
                        if (field == NULL) {
                            if (ctx->errors != NULL) {
                                SemanticError err = CreateFieldNotFoundError(expr->value.member_access.member_name,
                                                                            class_name,
                                                                            expr->line,
                                                                            expr->column);
                                AddError(ctx->errors, &err);
                            }
                            had_error = 1;
                        } else {
                            int inside_class = 0;
                            if (!IsFieldAccessible(field, inside_class)) {
                                if (ctx->errors != NULL) {
                                    SemanticError err = CreateAccessViolationError(field->name,
                                                                                  AccessSpecToString(field->access),
                                                                                  expr->line,
                                                                                  expr->column);
                                    AddError(ctx->errors, &err);
                                }
                                had_error = 1;
                            }
                        }
                    }
                } else if (obj_type != NULL) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateFieldNotFoundError(expr->value.member_access.member_name,
                                                                    TypeToString(obj_type),
                                                                    expr->line,
                                                                    expr->column);
                        AddError(ctx->errors, &err);
                    }
                    had_error = 1;
                }
            }
            break;
        }
        case EXPR_ARRAY_ACCESS: {
            NExpr *array_expr = expr->value.array_access.array;
            NExpr *index_expr = expr->value.array_access.index;
            NExpr *index_end = expr->value.array_access.index_end;
            if (CheckExpression(array_expr, ctx) != 0) {
                had_error = 1;
            }
            if (CheckExpression(index_expr, ctx) != 0) {
                had_error = 1;
            }
            if (CheckExpression(index_end, ctx) != 0) {
                had_error = 1;
            }
            if (array_expr != NULL) {
                NType *array_type = InferExpressionType(array_expr, ctx);
                if (array_type != NULL &&
                    array_type->kind != TYPE_KIND_BASE_ARRAY &&
                    array_type->kind != TYPE_KIND_CLASS_ARRAY) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateInvalidOperandsError("[]",
                                                                      TypeToString(array_type),
                                                                      NULL,
                                                                      expr->line,
                                                                      expr->column);
                        AddError(ctx->errors, &err);
                    }
                    had_error = 1;
                }
            }
            if (index_expr != NULL) {
                NType *index_type = InferExpressionType(index_expr, ctx);
                if (index_type != NULL && !CanBeArrayIndex(index_type)) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateInvalidArrayIndexError(TypeToString(index_type),
                                                                        index_expr->line,
                                                                        index_expr->column);
                        AddError(ctx->errors, &err);
                    }
                    had_error = 1;
                }
            }
            if (index_end != NULL) {
                NType *index_type = InferExpressionType(index_end, ctx);
                if (index_type != NULL && !CanBeArrayIndex(index_type)) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateInvalidArrayIndexError(TypeToString(index_type),
                                                                        index_end->line,
                                                                        index_end->column);
                        AddError(ctx->errors, &err);
                    }
                    had_error = 1;
                }
            }
            break;
        }
        case EXPR_BINARY_OP: {
            NExpr *left = expr->value.binary.left;
            NExpr *right = expr->value.binary.right;
            if (CheckExpression(left, ctx) != 0) {
                had_error = 1;
            }
            if (CheckExpression(right, ctx) != 0) {
                had_error = 1;
            }
            if (left != NULL && right != NULL) {
                NType *left_type = InferExpressionType(left, ctx);
                NType *right_type = InferExpressionType(right, ctx);
                NType *result_type = InferBinaryOperationType(expr->value.binary.op,
                                                             left_type,
                                                             right_type);
                if (result_type == NULL && left_type != NULL && right_type != NULL) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateInvalidOperandsError(OpToString(expr->value.binary.op),
                                                                      TypeToString(left_type),
                                                                      TypeToString(right_type),
                                                                      expr->line,
                                                                      expr->column);
                        AddError(ctx->errors, &err);
                    }
                    had_error = 1;
                }
            }
            break;
        }
        case EXPR_UNARY_OP: {
            NExpr *operand = expr->value.unary.operand;
            if (CheckExpression(operand, ctx) != 0) {
                had_error = 1;
            }
            if (operand != NULL) {
                NType *operand_type = InferExpressionType(operand, ctx);
                NType *result_type = InferUnaryOperationType(expr->value.unary.op, operand_type);
                if (result_type == NULL && operand_type != NULL) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateInvalidOperandsError(OpToString(expr->value.unary.op),
                                                                      TypeToString(operand_type),
                                                                      NULL,
                                                                      expr->line,
                                                                      expr->column);
                        AddError(ctx->errors, &err);
                    }
                    had_error = 1;
                }
            }
            break;
        }
        case EXPR_ASSIGN: {
            NExpr *left = expr->value.binary.left;
            NExpr *right = expr->value.binary.right;
            if (CheckExpression(left, ctx) != 0) {
                had_error = 1;
            }
            if (CheckExpression(right, ctx) != 0) {
                had_error = 1;
            }
            if (left != NULL && right != NULL) {
                NType *left_type = InferExpressionType(left, ctx);
                NType *right_type = InferExpressionType(right, ctx);
                if (left_type != NULL && right_type != NULL && !CanAssign(left_type, right_type)) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateTypeMismatchError(TypeToString(left_type),
                                                                    TypeToString(right_type),
                                                                    "in assignment",
                                                                    expr->line,
                                                                    expr->column);
                        AddError(ctx->errors, &err);
                    }
                    had_error = 1;
                }
            }
            break;
        }
        case EXPR_NEW: {
            if (expr->value.new_expr.type != NULL &&
                (expr->value.new_expr.type->kind == TYPE_KIND_CLASS ||
                 expr->value.new_expr.type->kind == TYPE_KIND_CLASS_ARRAY)) {
                if (LookupClass(ctx, expr->value.new_expr.type->class_name) == NULL) {
                    if (ctx->errors != NULL) {
                        SemanticError err = CreateUndefinedTypeError(expr->value.new_expr.type->class_name,
                                                                    expr->line,
                                                                    expr->column);
                        AddError(ctx->errors, &err);
                    }
                    had_error = 1;
                }
            }
            for (int i = 0; i < expr->value.new_expr.init_count; i++) {
                if (CheckExpression(expr->value.new_expr.init_exprs[i], ctx) != 0) {
                    had_error = 1;
                }
            }
            break;
        }
        case EXPR_PAREN:
            if (CheckExpression(expr->value.inner_expr, ctx) != 0) {
                had_error = 1;
            }
            break;
        case EXPR_SUPER_METHOD:
            for (int i = 0; i < expr->value.member_access.arg_count; i++) {
                if (CheckExpression(expr->value.member_access.args[i], ctx) != 0) {
                    had_error = 1;
                }
            }
            break;
        case EXPR_SUPER:
        case EXPR_THIS:
        case EXPR_INT:
        case EXPR_FLOAT:
        case EXPR_CHAR:
        case EXPR_STRING:
        case EXPR_BOOL:
        case EXPR_NULL:
        case EXPR_NAN:
            break;
    }

    return had_error;
}

int IsSymbolAccessible(Symbol *symbol, SemanticContext *ctx) {

    Scope *scope;

    if (symbol == NULL || ctx == NULL) {
        return 0;
    }

    if (symbol->scope_depth == 0) {
        return 1;
    }

    scope = GetCurrentScope(ctx);
    while (scope != NULL) {
        if (scope->depth == symbol->scope_depth) {
            return 1;
        }
        scope = scope->parent;
    }

    return 0;
}

/* ============================================================================
   ТРЕТИЙ ПРОХОД: АТРИБУТИРОВАНИЕ
   ============================================================================ */

int AttributeExpressions(NExpr *root, SemanticContext *ctx) {

    if (root == NULL) {
        return 0;
    }
    if (ctx == NULL) {
        return 1;
    }

    switch (root->type) {
        case EXPR_PAREN:
            AttributeExpressions(root->value.inner_expr, ctx);
            break;
        case EXPR_NEW:
            for (int i = 0; i < root->value.new_expr.init_count; i++) {
                AttributeExpressions(root->value.new_expr.init_exprs[i], ctx);
            }
            break;
        case EXPR_ARRAY_ACCESS:
            AttributeExpressions(root->value.array_access.array, ctx);
            AttributeExpressions(root->value.array_access.index, ctx);
            AttributeExpressions(root->value.array_access.index_end, ctx);
            break;
        case EXPR_MEMBER_ACCESS:
            AttributeExpressions(root->value.member_access.object, ctx);
            break;
        case EXPR_METHOD_CALL:
            AttributeExpressions(root->value.member_access.object, ctx);
            for (int i = 0; i < root->value.member_access.arg_count; i++) {
                AttributeExpressions(root->value.member_access.args[i], ctx);
            }
            break;
        case EXPR_FUNC_CALL:
            for (int i = 0; i < root->value.func_call.arg_count; i++) {
                AttributeExpressions(root->value.func_call.args[i], ctx);
            }
            break;
        case EXPR_SUPER_METHOD:
            for (int i = 0; i < root->value.member_access.arg_count; i++) {
                AttributeExpressions(root->value.member_access.args[i], ctx);
            }
            break;
        case EXPR_UNARY_OP:
            AttributeExpressions(root->value.unary.operand, ctx);
            break;
        case EXPR_BINARY_OP:
        case EXPR_ASSIGN:
            AttributeExpressions(root->value.binary.left, ctx);
            AttributeExpressions(root->value.binary.right, ctx);
            break;
        case EXPR_IDENT:
        case EXPR_INT:
        case EXPR_FLOAT:
        case EXPR_CHAR:
        case EXPR_STRING:
        case EXPR_BOOL:
        case EXPR_NULL:
        case EXPR_NAN:
        case EXPR_THIS:
        case EXPR_SUPER:
            break;
    }

    root->inferred_type = InferExpressionType(root, ctx);
    return 0;
}

static int AttributeStatement(NStmt *stmt, SemanticContext *ctx) {
    Scope *scope;

    if (stmt == NULL) {
        return 0;
    }
    if (ctx == NULL) {
        return 1;
    }

    scope = GetCurrentScope(ctx);
    stmt->scope_id = scope ? scope->depth : -1;

    switch (stmt->type) {
        case STMT_EXPR:
        case STMT_RETURN:
            AttributeExpressions(stmt->value.expr, ctx);
            break;
        case STMT_DECL:
            if (stmt->value.decl.init_decls) {
                for (int i = 0; i < stmt->value.decl.init_decls->count; i++) {
                    NInitDecl *decl = stmt->value.decl.init_decls->decls[i];
                    if (decl && decl->initializer && decl->initializer->expr) {
                        AttributeExpressions(decl->initializer->expr, ctx);
                    }
                }
            }
            break;
        case STMT_COMPOUND:
            if (PushScope(ctx, "block") != 0) {
                return 1;
            }
            AttributeStatements(stmt->value.stmt_list ? stmt->value.stmt_list->first : NULL, ctx);
            if (PopScope(ctx) != 0) {
                return 1;
            }
            break;
        case STMT_IF:
            AttributeExpressions(stmt->value.if_stmt.condition, ctx);
            AttributeStatement(stmt->value.if_stmt.then_stmt, ctx);
            AttributeStatement(stmt->value.if_stmt.else_stmt, ctx);
            break;
        case STMT_WHILE:
            AttributeExpressions(stmt->value.while_stmt.condition, ctx);
            AttributeStatement(stmt->value.while_stmt.body, ctx);
            break;
        case STMT_DO_WHILE:
            AttributeStatement(stmt->value.do_while_stmt.body, ctx);
            AttributeExpressions(stmt->value.do_while_stmt.condition, ctx);
            break;
        case STMT_FOR:
            AttributeExpressions(stmt->value.for_stmt.init_expr, ctx);
            if (stmt->value.for_stmt.init_decls) {
                for (int i = 0; i < stmt->value.for_stmt.init_decls->count; i++) {
                    NInitDecl *decl = stmt->value.for_stmt.init_decls->decls[i];
                    if (decl && decl->initializer && decl->initializer->expr) {
                        AttributeExpressions(decl->initializer->expr, ctx);
                    }
                }
            }
            AttributeExpressions(stmt->value.for_stmt.cond_expr, ctx);
            AttributeExpressions(stmt->value.for_stmt.iter_expr, ctx);
            AttributeStatement(stmt->value.for_stmt.body, ctx);
            break;
        case STMT_FOREACH:
            AttributeExpressions(stmt->value.foreach_stmt.collection, ctx);
            AttributeStatement(stmt->value.foreach_stmt.body, ctx);
            break;
        case STMT_SWITCH:
            AttributeExpressions(stmt->value.switch_stmt.expr, ctx);
            for (int i = 0; i < stmt->value.switch_stmt.cases.count; i++) {
                NCaseItem *item = stmt->value.switch_stmt.cases.items[i];
                if (item && item->case_expr) {
                    AttributeExpressions(item->case_expr, ctx);
                }
                if (item && item->stmts) {
                    AttributeStatements(item->stmts->first, ctx);
                }
            }
            break;
        case STMT_BREAK:
        case STMT_CONTINUE:
            break;
    }

    return 0;
}

int AttributeStatements(NStmt *stmts, SemanticContext *ctx) {
    NStmt *stmt;

    if (stmts == NULL) {
        return 0;
    }
    if (ctx == NULL) {
        return 1;
    }

    stmt = stmts;
    while (stmt != NULL) {
        AttributeStatement(stmt, ctx);
        stmt = stmt->next;
    }
    return 0;
}
