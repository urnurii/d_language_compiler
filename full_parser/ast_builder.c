#include "ast_builder.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylineno;

// ----- Вспомогательные -----

char* DuplicateString(const char *str) {
    if (str == NULL) return NULL;
    
    char *dup = (char *)malloc(strlen(str) + 1);
    if (dup == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    strcpy(dup, str);
    return dup;
}

static void InitExprAttrs(NExpr *expr) {
    if (expr == NULL) return;
    expr->resolved_symbol_id = -1;
    expr->scope_id = -1;
    expr->jvm_slot_index = -1;
    expr->jvm_descriptor = NULL;
    expr->inferred_type = NULL;
    expr->jvm_ref_key.has_key = 0;
    expr->jvm_ref_key.owner_internal_name = NULL;
    expr->jvm_ref_key.member_name = NULL;
    expr->jvm_ref_key.member_descriptor = NULL;
    expr->jvm_ref_key.kind = JVM_REF_FIELD;
    expr->enum_value = 0;
    expr->enum_value_is_set = 0;
}

static void InitStmtAttrs(NStmt *stmt) {
    if (stmt == NULL) return;
    stmt->scope_id = -1;
}

static void InitInitDeclAttrs(NInitDecl *decl) {
    if (decl == NULL) return;
    decl->resolved_symbol_id = -1;
    decl->jvm_slot_index = -1;
    decl->jvm_descriptor = NULL;
}

static void InitParamAttrs(NParam *param) {
    if (param == NULL) return;
    param->resolved_symbol_id = -1;
    param->jvm_slot_index = -1;
    param->jvm_descriptor = NULL;
}

static void InitFuncAttrs(NFuncDef *func) {
    if (func == NULL) return;
    func->jvm_descriptor = NULL;
}

static void InitMethodAttrs(NMethodDef *method) {
    if (method == NULL) return;
    method->jvm_descriptor = NULL;
}

// ----- Функции для типов -----

NType* CreateBaseType(BaseType base_type) {
    NType *type = (NType *)malloc(sizeof(NType));
    if (type == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    type->kind = TYPE_KIND_BASE;
    type->base_type = base_type;
    type->class_name = NULL;
    type->enum_name = NULL;
    type->array_decl = NULL;
    
    return type;
}

NType* CreateClassType(char *class_name) {
    NType *type = (NType *)malloc(sizeof(NType));
    if (type == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    type->kind = TYPE_KIND_CLASS;
    type->base_type = TYPE_INT;
    type->class_name = DuplicateString(class_name);
    type->enum_name = NULL;
    type->array_decl = NULL;
    
    return type;
}

NType* CreateEnumType(char *enum_name) {
    NType *type = (NType *)malloc(sizeof(NType));
    if (type == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }

    type->kind = TYPE_KIND_ENUM;
    type->base_type = TYPE_INT;
    type->class_name = NULL;
    type->enum_name = DuplicateString(enum_name);
    type->array_decl = NULL;

    return type;
}

NArrayDecl* CreateArrayDecl(int has_size, int size) {
    NArrayDecl *array = (NArrayDecl *)malloc(sizeof(NArrayDecl));
    if (array == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    array->has_size = has_size;
    array->size = size;
    
    return array;
}

NType* AddArrayToType(NType *type, NArrayDecl *array) {
    if (type == NULL || array == NULL) return type;
    
    type->array_decl = array;
    
    if (type->kind == TYPE_KIND_BASE) {
        type->kind = TYPE_KIND_BASE_ARRAY;
    } else if (type->kind == TYPE_KIND_CLASS) {
        type->kind = TYPE_KIND_CLASS_ARRAY;
    } else if (type->kind == TYPE_KIND_ENUM) {
        type->kind = TYPE_KIND_ENUM_ARRAY;
    }
    
    return type;
}

// ----- Функции для инициализаторов -----

NInitializer* CreateExprInitializer(NExpr *expr) {
    NInitializer *init = (NInitializer *)malloc(sizeof(NInitializer));
    if (init == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    init->is_array = 0;
    init->expr = expr;
    init->array_init.elements = NULL;
    init->array_init.count = 0;
    
    return init;
}

NInitializer* CreateArrayInitializer(NExpr **exprs, int count) {
    NInitializer *init = (NInitializer *)malloc(sizeof(NInitializer));
    if (init == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    init->is_array = 1;
    init->expr = NULL;
    init->array_init.elements = exprs;
    init->array_init.count = count;
    
    return init;
}

// ----- Функции для выражений - литералы -----

NExpr* CreateIdentExpr(const char *name) {
    NExpr *expr = (NExpr *)malloc(sizeof(NExpr));
    if (expr == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    expr->type = EXPR_IDENT;
    expr->line = yylineno;
    expr->column = 0;
    InitExprAttrs(expr);
    expr->value.ident_name = DuplicateString(name);
    
    return expr;
}

NExpr* CreateIntExpr(int value) {
    NExpr *expr = (NExpr *)malloc(sizeof(NExpr));
    if (expr == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    expr->type = EXPR_INT;
    expr->line = yylineno;
    expr->column = 0;
    InitExprAttrs(expr);
    expr->value.int_value = value;
    
    return expr;
}

NExpr* CreateFloatExpr(double value) {
    NExpr *expr = (NExpr *)malloc(sizeof(NExpr));
    if (expr == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    expr->type = EXPR_FLOAT;
    expr->line = yylineno;
    expr->column = 0;
    InitExprAttrs(expr);
    expr->value.float_value = value;
    
    return expr;
}

NExpr* CreateCharExpr(char value) {
    NExpr *expr = (NExpr *)malloc(sizeof(NExpr));
    if (expr == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    expr->type = EXPR_CHAR;
    expr->line = yylineno;
    expr->column = 0;
    InitExprAttrs(expr);
    expr->value.char_value = value;
    
    return expr;
}

NExpr* CreateStringExpr(const char *value) {
    NExpr *expr = (NExpr *)malloc(sizeof(NExpr));
    if (expr == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    expr->type = EXPR_STRING;
    expr->line = yylineno;
    expr->column = 0;
    InitExprAttrs(expr);
    expr->value.string_value = DuplicateString(value);
    
    return expr;
}

NExpr* CreateBoolExpr(int value) {
    NExpr *expr = (NExpr *)malloc(sizeof(NExpr));
    if (expr == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    expr->type = EXPR_BOOL;
    expr->line = yylineno;
    expr->column = 0;
    InitExprAttrs(expr);
    expr->value.int_value = value;
    
    return expr;
}

NExpr* CreateNullExpr(void) {
    NExpr *expr = (NExpr *)malloc(sizeof(NExpr));
    if (expr == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    expr->type = EXPR_NULL;
    expr->line = yylineno;
    expr->column = 0;
    InitExprAttrs(expr);
    
    return expr;
}

NExpr* CreateNanExpr(void) {
    NExpr *expr = (NExpr *)malloc(sizeof(NExpr));
    if (expr == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    expr->type = EXPR_NAN;
    expr->line = yylineno;
    expr->column = 0;
    InitExprAttrs(expr);
    expr->value.float_value = 0.0 / 0.0;
    
    return expr;
}

NExpr* CreateThisExpr(void) {
    NExpr *expr = (NExpr *)malloc(sizeof(NExpr));
    if (expr == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    expr->type = EXPR_THIS;
    expr->line = yylineno;
    expr->column = 0;
    InitExprAttrs(expr);
    
    return expr;
}

// ----- Функции для выражений - операции -----

NExpr* CreateParenExpr(NExpr *expr) {
    NExpr *paren = (NExpr *)malloc(sizeof(NExpr));
    if (paren == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    paren->type = EXPR_PAREN;
    paren->line = yylineno;
    paren->column = 0;
    paren->value.inner_expr = expr;
    
    return paren;
}

NExpr* CreateCastExpr(NType *target_type, NExpr *expr) {
    NExpr *cast_expr = (NExpr *)malloc(sizeof(NExpr));
    if (cast_expr == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }

    cast_expr->type = EXPR_CAST;
    cast_expr->line = yylineno;
    cast_expr->column = 0;
    InitExprAttrs(cast_expr);
    cast_expr->value.cast.target_type = target_type;
    cast_expr->value.cast.expr = expr;

    return cast_expr;
}

NExpr* CreateUnaryOpExpr(OpType op, NExpr *operand) {
    NExpr *expr = (NExpr *)malloc(sizeof(NExpr));
    if (expr == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    expr->type = EXPR_UNARY_OP;
    expr->line = yylineno;
    expr->column = 0;
    InitExprAttrs(expr);
    expr->value.unary.op = op;
    expr->value.unary.operand = operand;
    
    return expr;
}

NExpr* CreateBinaryOpExpr(OpType op, NExpr *left, NExpr *right) {
    NExpr *expr = (NExpr *)malloc(sizeof(NExpr));
    if (expr == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    expr->type = EXPR_BINARY_OP;
    expr->line = yylineno;
    expr->column = 0;
    InitExprAttrs(expr);
    expr->value.binary.op = op;
    expr->value.binary.left = left;
    expr->value.binary.right = right;
    
    return expr;
}

NExpr* CreateAssignExpr(OpType op, NExpr *left, NExpr *right) {
    NExpr *expr = (NExpr *)malloc(sizeof(NExpr));
    if (expr == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    expr->type = EXPR_ASSIGN;
    expr->line = yylineno;
    expr->column = 0;
    InitExprAttrs(expr);
    expr->value.binary.op = op;
    expr->value.binary.left = left;
    expr->value.binary.right = right;
    
    return expr;
}

// ----- Функции для выражений - доступ и вызовы -----

NExpr* CreateArrayAccessExpr(NExpr *array, NExpr *index, NExpr *index_end) {
    NExpr *expr = (NExpr *)malloc(sizeof(NExpr));
    if (expr == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    expr->type = EXPR_ARRAY_ACCESS;
    expr->line = yylineno;
    expr->column = 0;
    InitExprAttrs(expr);
    expr->value.array_access.array = array;
    expr->value.array_access.index = index;
    expr->value.array_access.index_end = index_end;
    
    return expr;
}

NExpr* CreateMemberAccessExpr(NExpr *object, const char *member_name) {
    NExpr *expr = (NExpr *)malloc(sizeof(NExpr));
    if (expr == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    expr->type = EXPR_MEMBER_ACCESS;
    expr->line = yylineno;
    expr->column = 0;
    InitExprAttrs(expr);
    expr->value.member_access.object = object;
    expr->value.member_access.member_name = DuplicateString(member_name);
    expr->value.member_access.args = NULL;
    expr->value.member_access.arg_count = 0;
    
    return expr;
}

NExpr* CreateMethodCallExpr(NExpr *object, const char *method_name, NExpr **args, int arg_count) {
    NExpr *expr = (NExpr *)malloc(sizeof(NExpr));
    if (expr == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    expr->type = EXPR_METHOD_CALL;
    expr->line = yylineno;
    expr->column = 0;
    InitExprAttrs(expr);
    expr->value.member_access.object = object;
    expr->value.member_access.member_name = DuplicateString(method_name);
    expr->value.member_access.args = args;
    expr->value.member_access.arg_count = arg_count;
    
    return expr;
}

NExpr* CreateFuncCallExpr(const char *func_name, NExpr **args, int arg_count) {
    NExpr *expr = (NExpr *)malloc(sizeof(NExpr));
    if (expr == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    expr->type = EXPR_FUNC_CALL;
    expr->line = yylineno;
    expr->column = 0;
    InitExprAttrs(expr);
    expr->value.func_call.func_name = DuplicateString(func_name);
    expr->value.func_call.args = args;
    expr->value.func_call.arg_count = arg_count;
    
    return expr;
}

NExpr* CreateNewExpr(NType *type, NExpr **init_exprs, int init_count) {
    NExpr *expr = (NExpr *)malloc(sizeof(NExpr));
    if (expr == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    expr->type = EXPR_NEW;
    expr->line = yylineno;
    expr->column = 0;
    InitExprAttrs(expr);
    expr->value.new_expr.type = type;
    expr->value.new_expr.init_exprs = init_exprs;
    expr->value.new_expr.init_count = init_count;
    
    return expr;
}

NExpr* CreateSuperExpr(const char *member_name) {
    NExpr *expr = (NExpr *)malloc(sizeof(NExpr));
    if (expr == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    expr->type = EXPR_SUPER;
    expr->line = yylineno;
    expr->column = 0;
    InitExprAttrs(expr);
    expr->value.ident_name = DuplicateString(member_name);
    
    return expr;
}

NExpr* CreateSuperMethodCallExpr(const char *method_name, NExpr **args, int arg_count) {
    NExpr *expr = (NExpr *)malloc(sizeof(NExpr));
    if (expr == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    expr->type = EXPR_SUPER_METHOD;
    expr->line = yylineno;
    expr->column = 0;
    InitExprAttrs(expr);
    expr->value.member_access.object = NULL;
    expr->value.member_access.member_name = DuplicateString(method_name);
    expr->value.member_access.args = args;
    expr->value.member_access.arg_count = arg_count;
    
    return expr;
}

// ----- Функции для управления списками выражений ------

NExprList* CreateExprList(void) {
    NExprList *list = (NExprList *)malloc(sizeof(NExprList));
    if (list == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    list->elements = NULL;
    list->count = 0;
    list->capacity = 0;
    
    return list;
}

void AddExprToList(NExprList *list, NExpr *expr) {
    if (list == NULL || expr == NULL) return;
    
    if (list->count >= list->capacity) {
        list->capacity = (list->capacity == 0) ? 10 : list->capacity * 2;
        list->elements = (NExpr **)realloc(list->elements, list->capacity * sizeof(NExpr *));
        if (list->elements == NULL) {
            fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
            exit(1);
        }
    }
    
    list->elements[list->count++] = expr;
}

// ----- Функции для параметров -----

NParam* CreateParam(NType *param_type, const char *param_name, int is_ref, NExpr *default_value) {
    NParam *param = (NParam *)malloc(sizeof(NParam));
    if (param == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    param->param_type = param_type;
    param->param_name = DuplicateString(param_name);
    param->is_ref = is_ref;
    param->default_value = default_value;
    InitParamAttrs(param);
    
    return param;
}

NParamList* CreateParamList(void) {
    NParamList *list = (NParamList *)malloc(sizeof(NParamList));
    if (list == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    list->params = NULL;
    list->count = 0;
    list->capacity = 0;
    
    return list;
}

void AddParamToList(NParamList *list, NParam *param) {
    if (list == NULL || param == NULL) return;
    
    if (list->count >= list->capacity) {
        list->capacity = (list->capacity == 0) ? 5 : list->capacity * 2;
        list->params = (NParam **)realloc(list->params, list->capacity * sizeof(NParam *));
        if (list->params == NULL) {
            fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
            exit(1);
        }
    }
    
    list->params[list->count++] = param;
}

// void FreeParamList(NParamList *list) {
//     if (list == NULL) return;
    
//     for (int i = 0; i < list->count; i++) {
//         if (list->params[i] != NULL) {
//             if (list->params[i]->param_type != NULL) {
//                 FreeType(list->params[i]->param_type);
//             }
//             if (list->params[i]->param_name != NULL) {
//                 free(list->params[i]->param_name);
//             }
//             if (list->params[i]->default_value != NULL) {
//                 FreeExpr(list->params[i]->default_value);
//             }
//             free(list->params[i]);
//         }
//     }
//     if (list->params != NULL) {
//         free(list->params);
//     }
//     free(list);
// }

// ----- Функции для объявлений переменных -----

NInitDecl* CreateInitDecl(const char *name, NInitializer *initializer) {
    NInitDecl *decl = (NInitDecl *)malloc(sizeof(NInitDecl));
    if (decl == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    decl->name = DuplicateString(name);
    decl->initializer = initializer;
    InitInitDeclAttrs(decl);
    
    return decl;
}

NInitDeclList* CreateInitDeclList(void) {
    NInitDeclList *list = (NInitDeclList *)malloc(sizeof(NInitDeclList));
    if (list == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    list->decls = NULL;
    list->count = 0;
    list->capacity = 0;
    
    return list;
}

void AddInitDeclToList(NInitDeclList *list, NInitDecl *decl) {
    if (list == NULL || decl == NULL) return;
    
    if (list->count >= list->capacity) {
        list->capacity = (list->capacity == 0) ? 5 : list->capacity * 2;
        list->decls = (NInitDecl **)realloc(list->decls, list->capacity * sizeof(NInitDecl *));
        if (list->decls == NULL) {
            fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
            exit(1);
        }
    }
    
    list->decls[list->count++] = decl;
}

// void FreeInitDeclList(NInitDeclList *list) {
//     if (list == NULL) return;
    
//     for (int i = 0; i < list->count; i++) {
//         if (list->decls[i] != NULL) {
//             if (list->decls[i]->name != NULL) {
//                 free(list->decls[i]->name);
//             }
//             if (list->decls[i]->initializer != NULL) {
//                 if (list->decls[i]->initializer->is_array == 0) {
//                     if (list->decls[i]->initializer->expr != NULL) {
//                         FreeExpr(list->decls[i]->initializer->expr);
//                     }
//                 } else {
//                     for (int j = 0; j < list->decls[i]->initializer->array_init.count; j++) {
//                         if (list->decls[i]->initializer->array_init.elements[j] != NULL) {
//                             FreeExpr(list->decls[i]->initializer->array_init.elements[j]);
//                         }
//                     }
//                     if (list->decls[i]->initializer->array_init.elements != NULL) {
//                         free(list->decls[i]->initializer->array_init.elements);
//                     }
//                 }
//                 free(list->decls[i]->initializer);
//             }
//             free(list->decls[i]);
//         }
//     }
//     if (list->decls != NULL) {
//         free(list->decls);
//     }
//     free(list);
// }

// ----- Функции для операторов -----

NStmt* CreateExprStmt(NExpr *expr) {
    NStmt *stmt = (NStmt *)malloc(sizeof(NStmt));
    if (stmt == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    stmt->type = STMT_EXPR;
    stmt->line = yylineno;
    stmt->column = 0;
    InitStmtAttrs(stmt);
    stmt->value.expr = expr;
    stmt->next = NULL;
    
    return stmt;
}

NStmt* CreateSuperCtorCallStmt(NExprList *args) {
    NStmt *stmt = (NStmt *)malloc(sizeof(NStmt));
    if (stmt == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }

    stmt->type = STMT_SUPER_CTOR_CALL;
    stmt->line = yylineno;
    stmt->column = 0;
    InitStmtAttrs(stmt);
    stmt->value.super_ctor.args = args;
    stmt->next = NULL;

    return stmt;
}

NStmt* CreateThisCtorCallStmt(NExprList *args) {
    NStmt *stmt = (NStmt *)malloc(sizeof(NStmt));
    if (stmt == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }

    stmt->type = STMT_THIS_CTOR_CALL;
    stmt->line = yylineno;
    stmt->column = 0;
    InitStmtAttrs(stmt);
    stmt->value.this_ctor.args = args;
    stmt->next = NULL;

    return stmt;
}

NStmt* CreateDeclStmt(NType *decl_type, NInitDeclList *init_decls) {
    NStmt *stmt = (NStmt *)malloc(sizeof(NStmt));
    if (stmt == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    stmt->type = STMT_DECL;
    stmt->line = yylineno;
    stmt->column = 0;
    InitStmtAttrs(stmt);
    stmt->value.decl.decl_type = decl_type;
    stmt->value.decl.init_decls = init_decls;
    stmt->next = NULL;
    
    return stmt;
}

NStmt* CreateCompoundStmt(NStmtList *stmt_list) {
    NStmt *stmt = (NStmt *)malloc(sizeof(NStmt));
    if (stmt == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    stmt->type = STMT_COMPOUND;
    stmt->line = yylineno;
    stmt->column = 0;
    InitStmtAttrs(stmt);
    stmt->value.stmt_list = stmt_list;
    stmt->next = NULL;
    
    return stmt;
}

NStmt* CreateReturnStmt(NExpr *return_expr) {
    NStmt *stmt = (NStmt *)malloc(sizeof(NStmt));
    if (stmt == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    stmt->type = STMT_RETURN;
    stmt->line = yylineno;
    stmt->column = 0;
    InitStmtAttrs(stmt);
    stmt->value.expr = return_expr;
    stmt->next = NULL;
    
    return stmt;
}

NStmt* CreateBreakStmt(void) {
    NStmt *stmt = (NStmt *)malloc(sizeof(NStmt));
    if (stmt == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    stmt->type = STMT_BREAK;
    stmt->line = yylineno;
    stmt->column = 0;
    InitStmtAttrs(stmt);
    stmt->next = NULL;
    
    return stmt;
}

NStmt* CreateContinueStmt(void) {
    NStmt *stmt = (NStmt *)malloc(sizeof(NStmt));
    if (stmt == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    stmt->type = STMT_CONTINUE;
    stmt->line = yylineno;
    stmt->column = 0;
    InitStmtAttrs(stmt);
    stmt->next = NULL;
    
    return stmt;
}

// ----- Функции для условных операторов -----

NStmt* CreateIfStmt(NExpr *condition, NStmt *then_stmt, NStmt *else_stmt) {
    NStmt *stmt = (NStmt *)malloc(sizeof(NStmt));
    if (stmt == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    stmt->type = STMT_IF;
    stmt->line = yylineno;
    stmt->column = 0;
    InitStmtAttrs(stmt);
    stmt->value.if_stmt.condition = condition;
    stmt->value.if_stmt.then_stmt = then_stmt;
    stmt->value.if_stmt.else_stmt = else_stmt;
    stmt->next = NULL;
    
    return stmt;
}

// ----- Функции для циклов -----

NStmt* CreateWhileStmt(NExpr *condition, NStmt *body) {
    NStmt *stmt = (NStmt *)malloc(sizeof(NStmt));
    if (stmt == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    stmt->type = STMT_WHILE;
    stmt->line = yylineno;
    stmt->column = 0;
    InitStmtAttrs(stmt);
    stmt->value.while_stmt.condition = condition;
    stmt->value.while_stmt.body = body;
    stmt->next = NULL;
    
    return stmt;
}

NStmt* CreateDoWhileStmt(NStmt *body, NExpr *condition) {
    NStmt *stmt = (NStmt *)malloc(sizeof(NStmt));
    if (stmt == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    stmt->type = STMT_DO_WHILE;
    stmt->line = yylineno;
    stmt->column = 0;
    InitStmtAttrs(stmt);
    stmt->value.do_while_stmt.body = body;
    stmt->value.do_while_stmt.condition = condition;
    stmt->next = NULL;
    
    return stmt;
}

NStmt* CreateForStmt(NExpr *init_expr, NType *init_decl_type, NInitDeclList *init_decls,
                    NExpr *cond_expr, NExpr *iter_expr, NStmt *body) {
    NStmt *stmt = (NStmt *)malloc(sizeof(NStmt));
    if (stmt == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    stmt->type = STMT_FOR;
    stmt->line = yylineno;
    stmt->column = 0;
    InitStmtAttrs(stmt);
    stmt->value.for_stmt.init_expr = init_expr;
    stmt->value.for_stmt.init_decl_type = init_decl_type;
    stmt->value.for_stmt.init_decls = init_decls;
    stmt->value.for_stmt.cond_expr = cond_expr;
    stmt->value.for_stmt.iter_expr = iter_expr;
    stmt->value.for_stmt.body = body;
    stmt->next = NULL;
    
    return stmt;
}

NStmt* CreateForeachStmt(int is_typed, NType *var_type, const char *var_name,
                        NExpr *collection, NStmt *body) {
    NStmt *stmt = (NStmt *)malloc(sizeof(NStmt));
    if (stmt == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    stmt->type = STMT_FOREACH;
    stmt->line = yylineno;
    stmt->column = 0;
    InitStmtAttrs(stmt);
    stmt->value.foreach_stmt.is_typed = is_typed;
    stmt->value.foreach_stmt.var_type = var_type;
    stmt->value.foreach_stmt.var_name = DuplicateString(var_name);
    stmt->value.foreach_stmt.collection = collection;
    stmt->value.foreach_stmt.body = body;
    stmt->next = NULL;
    
    return stmt;
}

NStmt* CreateSwitchStmt(NExpr *expr, NCaseItem **case_items, int case_count) {
    NStmt *stmt = (NStmt *)malloc(sizeof(NStmt));
    if (stmt == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    stmt->type = STMT_SWITCH;
    stmt->line = yylineno;
    stmt->column = 0;
    InitStmtAttrs(stmt);
    stmt->value.switch_stmt.expr = expr;
    stmt->value.switch_stmt.cases.items = case_items;
    stmt->value.switch_stmt.cases.count = case_count;
    stmt->next = NULL;
    
    return stmt;
}

NCaseItem* CreateCaseItem(NExpr *case_expr, NStmtList *stmts) {
    NCaseItem *item = (NCaseItem *)malloc(sizeof(NCaseItem));
    if (item == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    item->type = CASE_ITEM_CASE;
    item->case_expr = case_expr;
    item->stmts = stmts;
    
    return item;
}

NCaseItem* CreateDefaultItem(NStmtList *stmts) {
    NCaseItem *item = (NCaseItem *)malloc(sizeof(NCaseItem));
    if (item == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    item->type = CASE_ITEM_DEFAULT;
    item->case_expr = NULL;
    item->stmts = stmts;
    
    return item;
}

NCaseList* CreateCaseList(void) {
    NCaseList *list = (NCaseList *)malloc(sizeof(NCaseList));
    if (list == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    list->capacity = 10;
    list->count = 0;
    list->items = (NCaseItem **)malloc(sizeof(NCaseItem*) * list->capacity);
    if (list->items == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    return list;
}

void AddCaseItemToList(NCaseList *list, NCaseItem *item) {
    if (list == NULL || item == NULL) return;
    
    if (list->count >= list->capacity) {
        list->capacity *= 2;
        list->items = (NCaseItem **)realloc(list->items, sizeof(NCaseItem*) * list->capacity);
        if (list->items == NULL) {
            fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
            exit(1);
        }
    }
    
    list->items[list->count] = item;
    list->count++;
}

void FreeCaseList(NCaseList *list) {
    if (list == NULL) return;
    free(list->items);
    free(list);
}

// ----- Функции для управления списками операторов -----

NStmtList* CreateStmtList(void) {
    NStmtList *list = (NStmtList *)malloc(sizeof(NStmtList));
    if (list == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    list->first = NULL;
    list->last = NULL;
    
    return list;
}

void AddStmtToList(NStmtList *list, NStmt *stmt) {
    if (list == NULL || stmt == NULL) return;
    
    if (list->first == NULL) {
        list->first = stmt;
        list->last = stmt;
    } else {
        list->last->next = stmt;
        list->last = stmt;
    }
    
    stmt->next = NULL;
}

// ----- Функции для функций -----

NFuncDef* CreateFuncDef(NType *return_type, const char *func_name,
                       NParamList *params, NStmt *body) {
    NFuncDef *func = (NFuncDef *)malloc(sizeof(NFuncDef));
    if (func == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    func->return_type = return_type;
    func->func_name = DuplicateString(func_name);
    func->params = params;
    func->body = body;
    InitFuncAttrs(func);
    
    return func;
}

NFuncDef* CreateVoidFuncDef(const char *func_name, NParamList *params, NStmt *body) {
    return CreateFuncDef(NULL, func_name, params, body);
}

// ----- Функции для методов -----

NMethodDef* CreateMethodDef(int is_override, NType *return_type, const char *method_name,
                           NParamList *params, NStmt *body) {
    NMethodDef *method = (NMethodDef *)malloc(sizeof(NMethodDef));
    if (method == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    method->is_override = is_override;
    method->return_type = return_type;
    method->method_name = DuplicateString(method_name);
    method->params = params;
    method->body = body;
    InitMethodAttrs(method);
    
    return method;
}

NMethodDef* CreateVoidMethodDef(int is_override, const char *method_name,
                               NParamList *params, NStmt *body) {
    return CreateMethodDef(is_override, NULL, method_name, params, body);
}

// ----- Функции для конструкторов и деструкторов

NCtorDef* CreateCtorDef(NParamList *params, NStmt *body) {
    NCtorDef *ctor = (NCtorDef *)malloc(sizeof(NCtorDef));
    if (ctor == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    ctor->params = params;
    ctor->body = body;
    
    return ctor;
}

NDtorDef* CreateDtorDef(NStmt *body) {
    NDtorDef *dtor = (NDtorDef *)malloc(sizeof(NDtorDef));
    if (dtor == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    dtor->body = body;
    
    return dtor;
}

// ----- Функции для членов класса -----

NClassMember* CreateFieldMember(AccessSpec access, NType *field_type,
                               NInitDeclList *init_decls) {
    NClassMember *member = (NClassMember *)malloc(sizeof(NClassMember));
    if (member == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    member->type = CLASS_MEMBER_FIELD;
    member->access = access;
    member->value.field.field_type = field_type;
    member->value.field.init_decls = init_decls;
    member->next = NULL;
    
    return member;
}

NClassMember* CreateMethodMember(AccessSpec access, NMethodDef *method) {
    NClassMember *member = (NClassMember *)malloc(sizeof(NClassMember));
    if (member == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    member->type = CLASS_MEMBER_METHOD;
    member->access = access;
    member->value.method = method;
    member->next = NULL;
    
    return member;
}

NClassMember* CreateCtorMember(AccessSpec access, NCtorDef *ctor) {
    NClassMember *member = (NClassMember *)malloc(sizeof(NClassMember));
    if (member == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    member->type = CLASS_MEMBER_CTOR;
    member->access = access;
    member->value.ctor = ctor;
    member->next = NULL;
    
    return member;
}

NClassMember* CreateDtorMember(AccessSpec access, NDtorDef *dtor) {
    NClassMember *member = (NClassMember *)malloc(sizeof(NClassMember));
    if (member == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    member->type = CLASS_MEMBER_DTOR;
    member->access = access;
    member->value.dtor = dtor;
    member->next = NULL;
    
    return member;
}

NClassMember* CreateEnumMember(AccessSpec access, NEnumDef *enum_def) {
    NClassMember *member = (NClassMember *)malloc(sizeof(NClassMember));
    if (member == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    member->type = CLASS_MEMBER_ENUM;
    member->access = access;
    member->value.enum_def = enum_def;
    member->next = NULL;
    
    return member;
}

NClassMember* AppendClassMember(NClassMember *list, NClassMember *member) {
    if (!list) return member;

    NClassMember *current = list;
    while (current->next) {
        current = current->next;
    }

    current->next = member;
    member->next = NULL;
    return list;
}

void AddClassMemberToList(NClassMember **first, NClassMember **last, NClassMember *member) {
    if (first == NULL || member == NULL) return;
    
    if (*first == NULL) {
        *first = member;
        *last = member;
    } else {
        (*last)->next = member;
        *last = member;
    }
    
    member->next = NULL;
}

// ----- Функции для классов -----

NClassDef* CreateClassDef(const char *class_name, const char *base_class_name,
                         NClassMember *first_member) {
    NClassDef *class_def = (NClassDef *)malloc(sizeof(NClassDef));
    if (class_def == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    class_def->class_name = DuplicateString(class_name);
    class_def->base_class_name = (base_class_name != NULL) ? DuplicateString(base_class_name) : NULL;
    
    if (first_member != NULL) {
        class_def->members.first = first_member;
        
        NClassMember *current = first_member;
        while (current->next != NULL) {
            current = current->next;
        }
        class_def->members.last = current;
    } else {
        class_def->members.first = NULL;
        class_def->members.last = NULL;
    }
    
    return class_def;
}

// ----- Функции для enum -----

NEnumItem* CreateEnumItem(const char *name, int has_value, int value) {
    NEnumItem *item = (NEnumItem *)malloc(sizeof(NEnumItem));
    if (item == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    item->name = DuplicateString(name);
    item->has_value = has_value;
    item->value = value;
    
    return item;
}

NEnumItemList* CreateEnumItemList(void) {
    NEnumItemList *list = (NEnumItemList *)malloc(sizeof(NEnumItemList));
    if (list == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
    
    return list;
}

void AddEnumItemToList(NEnumItemList *list, NEnumItem *item) {
    if (list == NULL || item == NULL) return;
    
    if (list->count >= list->capacity) {
        list->capacity = (list->capacity == 0) ? 8 : list->capacity * 2;
        list->items = (NEnumItem **)realloc(list->items, list->capacity * sizeof(NEnumItem *));
        if (list->items == NULL) {
            fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
            exit(1);
        }
    }
    
    list->items[list->count++] = item;
}

NEnumDef* CreateEnumDef(const char *enum_name, NEnumItem **items, int item_count) {
    NEnumDef *enum_def = (NEnumDef *)malloc(sizeof(NEnumDef));
    if (enum_def == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    enum_def->enum_name = (enum_name != NULL) ? DuplicateString(enum_name) : NULL;
    enum_def->items.items = items;
    enum_def->items.count = item_count;
    
    return enum_def;
}

// ----- Функции для программы -----

NSourceItem* CreateFuncSourceItem(NFuncDef *func) {
    NSourceItem *item = (NSourceItem *)malloc(sizeof(NSourceItem));
    if (item == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    item->type = SOURCE_ITEM_FUNC;
    item->value.func = func;
    item->next = NULL;
    
    return item;
}

NSourceItem* CreateClassSourceItem(NClassDef *class_def) {
    NSourceItem *item = (NSourceItem *)malloc(sizeof(NSourceItem));
    if (item == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    item->type = SOURCE_ITEM_CLASS;
    item->value.class_def = class_def;
    item->next = NULL;
    
    return item;
}

NSourceItem* CreateDeclSourceItem(NType *item_type, NInitDeclList *init_decls) {
    NSourceItem *item = (NSourceItem *)malloc(sizeof(NSourceItem));
    if (item == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    item->type = SOURCE_ITEM_DECL;
    item->value.decl.item_type = item_type;
    item->value.decl.init_decls = init_decls;
    item->next = NULL;
    
    return item;
}

NSourceItem* CreateEnumSourceItem(NEnumDef *enum_def) {
    NSourceItem *item = (NSourceItem *)malloc(sizeof(NSourceItem));
    if (item == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    item->type = SOURCE_ITEM_ENUM;
    item->value.enum_def = enum_def;
    item->next = NULL;
    
    return item;
}

NProgram* CreateProgram(NSourceItem *first_item) {
    NProgram *prog = (NProgram *)malloc(sizeof(NProgram));
    if (prog == NULL) {
        fprintf(stderr, "Error: Memory allocation failed at line %d\n", yylineno);
        exit(1);
    }
    
    prog->first_item = first_item;
    prog->last_item = first_item;
    
    return prog;
}

void AddSourceItemToProgram(NProgram *program, NSourceItem *item) {
    if (program == NULL || item == NULL) return;
    
    if (program->first_item == NULL) {
        program->first_item = item;
        program->last_item = item;
    } else {
        program->last_item->next = item;
        program->last_item = item;
    }
    
    item->next = NULL;
}
