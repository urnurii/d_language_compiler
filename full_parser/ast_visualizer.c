/* ============================================================================
 * ast_visualizer.c
 * 
 * Реализация функций для визуализации синтаксического дерева в DOT формат
 * 
 * DOT - язык описания графов для Graphviz
 * Пример использования:
 *   $ dot -Tpng output.dot -o output.png
 *   $ dot -Tsvg output.dot -o output.svg
 *   $ dotty output.dot  (интерактивный просмотр)
 * ============================================================================ */

#include "ast_visualizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Глобальный счётчик для генерации уникальных ID узлов */
static int node_counter = 0;

void ResetNodeCounter(void) {
    node_counter = 0;
}

static int GetNextNodeId(void) {
    return node_counter++;
}

/* ============================================================================
 * ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ
 * ============================================================================ */

/**
 * Экранирует спецсимволы для DOT формата
 */
static const char* EscapeForDot(const char *str) {
    static char buffer[256];
    if (str == NULL) return "NULL";
    
    int j = 0;
    for (int i = 0; str[i] != '\0' && j < 250; i++) {
        if (str[i] == '"' || str[i] == '\\') {
            buffer[j++] = '\\';
        }
        buffer[j++] = str[i];
    }
    buffer[j] = '\0';
    return buffer;
}

/**
 * Возвращает строковое представление типа выражения
 */
static const char* ExprTypeToString(ExprType type) {
    switch (type) {
        case EXPR_IDENT: return "IDENT";
        case EXPR_INT: return "INT";
        case EXPR_FLOAT: return "FLOAT";
        case EXPR_CHAR: return "CHAR";
        case EXPR_STRING: return "STRING";
        case EXPR_BOOL: return "BOOL";
        case EXPR_NULL: return "NULL";
        case EXPR_NAN: return "NAN";
        case EXPR_THIS: return "THIS";
        case EXPR_PAREN: return "PAREN";
        case EXPR_UNARY_OP: return "UNARY_OP";
        case EXPR_BINARY_OP: return "BINARY_OP";
        case EXPR_ASSIGN: return "ASSIGN";
        case EXPR_ARRAY_ACCESS: return "ARRAY_ACCESS";
        case EXPR_MEMBER_ACCESS: return "MEMBER_ACCESS";
        case EXPR_METHOD_CALL: return "METHOD_CALL";
        case EXPR_FUNC_CALL: return "FUNC_CALL";
        case EXPR_NEW: return "NEW";
        case EXPR_SUPER: return "SUPER";
        case EXPR_SUPER_METHOD: return "SUPER_METHOD";
        default: return "UNKNOWN_EXPR";
    }
}

/**
 * Возвращает строковое представление типа оператора
 */
static const char* StmtTypeToString(StmtType type) {
    switch (type) {
        case STMT_EXPR: return "EXPR_STMT";
        case STMT_DECL: return "DECL_STMT";
        case STMT_COMPOUND: return "COMPOUND";
        case STMT_RETURN: return "RETURN";
        case STMT_BREAK: return "BREAK";
        case STMT_CONTINUE: return "CONTINUE";
        case STMT_IF: return "IF";
        case STMT_WHILE: return "WHILE";
        case STMT_DO_WHILE: return "DO_WHILE";
        case STMT_FOR: return "FOR";
        case STMT_FOREACH: return "FOREACH";
        case STMT_SWITCH: return "SWITCH";
        default: return "UNKNOWN_STMT";
    }
}

/**
 * Возвращает строковое представление типа операции
 */
static const char* OpTypeToString(OpType op) {
    switch (op) {
        case OP_PLUS: return "+";
        case OP_MINUS: return "-";
        case OP_MUL: return "*";
        case OP_DIV: return "/";
        case OP_EQ: return "==";
        case OP_NEQ: return "!=";
        case OP_LT: return "<";
        case OP_GT: return ">";
        case OP_LE: return "<=";
        case OP_GE: return ">=";
        case OP_AND: return "&&";
        case OP_OR: return "||";
        case OP_NOT: return "!";
        case OP_NEG: return "neg";
        case OP_BITWISE_AND: return "&";
        case OP_BITWISE_OR: return "|";
        case OP_BITWISE_XOR: return "^";
        case OP_BITWISE_NOT: return "~";
        case OP_LSHIFT: return "<<";
        case OP_RSHIFT: return ">>";
        case OP_ASSIGN: return "=";
        case OP_PLUS_ASSIGN: return "+=";
        case OP_MINUS_ASSIGN: return "-=";
        case OP_MUL_ASSIGN: return "*=";
        case OP_DIV_ASSIGN: return "/=";
        case OP_MOD_ASSIGN: return "%=";
        case OP_BITWISE_AND_ASSIGN: return "&=";
        case OP_BITWISE_OR_ASSIGN: return "|=";
        case OP_BITWISE_XOR_ASSIGN: return "^=";
        case OP_LSHIFT_ASSIGN: return "<<=";
        case OP_RSHIFT_ASSIGN: return ">>=";
        default: return "OP_UNKNOWN";
    }
}

/**
 * Возвращает строковое представление базового типа
 */
static const char* BaseTypeToString(BaseType type) {
    switch (type) {
        case TYPE_INT: return "int";
        case TYPE_CHAR: return "char";
        case TYPE_FLOAT: return "float";
        case TYPE_DOUBLE: return "double";
        case TYPE_BOOL: return "bool";
        case TYPE_STRING: return "string";
        case TYPE_REAL: return "real";
        case TYPE_AUTO: return "auto";
        default: return "unknown";
    }
}

/* ============================================================================
 * ФУНКЦИИ ВИЗУАЛИЗАЦИИ ВЫРАЖЕНИЙ
 * ============================================================================ */

int ExprToDoT(FILE *file, NExpr *expr) {
    if (expr == NULL) {
        return -1;
    }

    int node_id = GetNextNodeId();
    const char *type_str = ExprTypeToString(expr->type);
    
    fprintf(file, "  node_%d [label=\"%s\"", node_id, type_str);
    
    /* Добавляем значение для литералов */
    switch (expr->type) {
        case EXPR_INT:
            fprintf(file, " (%d)", expr->value.int_value);
            break;
        case EXPR_FLOAT:
            fprintf(file, " (%.2f)", expr->value.float_value);
            break;
        case EXPR_CHAR:
            fprintf(file, " ('%c')", expr->value.char_value);
            break;
        case EXPR_STRING:
            fprintf(file, " (\\\"%s\\\")", EscapeForDot(expr->value.string_value));
            break;
        case EXPR_IDENT:
            fprintf(file, " (%s)", EscapeForDot(expr->value.ident_name));
            break;
        default:
            break;
    }
    
    fprintf(file, ", shape=box, style=filled, fillcolor=lightblue];\n");

    /* Обработка подузлов в зависимости от типа выражения */
    switch (expr->type) {
        case EXPR_PAREN: {
            int child_id = ExprToDoT(file, expr->value.inner_expr);
            if (child_id >= 0) {
                fprintf(file, "  node_%d -> node_%d [label=\"expr\"];\n", node_id, child_id);
            }
            break;
        }

        case EXPR_UNARY_OP: {
            fprintf(file, "  node_%d -> node_%d [label=\"op: %s\"];\n", 
                    node_id, node_id + 1, OpTypeToString(expr->value.unary.op));
            int operand_id = ExprToDoT(file, expr->value.unary.operand);
            if (operand_id >= 0) {
                fprintf(file, "  node_%d -> node_%d [label=\"operand\"];\n", node_id, operand_id);
            }
            break;
        }

        case EXPR_BINARY_OP: {
            int left_id = ExprToDoT(file, expr->value.binary.left);
            int right_id = ExprToDoT(file, expr->value.binary.right);
            if (left_id >= 0) {
                fprintf(file, "  node_%d -> node_%d [label=\"left\", color=green];\n", node_id, left_id);
            }
            if (right_id >= 0) {
                fprintf(file, "  node_%d -> node_%d [label=\"right\", color=red];\n", node_id, right_id);
            }
            break;
        }

        case EXPR_ASSIGN: {
            int left_id = ExprToDoT(file, expr->value.binary.left);
            int right_id = ExprToDoT(file, expr->value.binary.right);
            if (left_id >= 0) {
                fprintf(file, "  node_%d -> node_%d [label=\"target\", color=blue];\n", node_id, left_id);
            }
            if (right_id >= 0) {
                fprintf(file, "  node_%d -> node_%d [label=\"value\", color=purple];\n", node_id, right_id);
            }
            break;
        }

        case EXPR_ARRAY_ACCESS: {
            int array_id = ExprToDoT(file, expr->value.array_access.array);
            int index_id = ExprToDoT(file, expr->value.array_access.index);
            if (array_id >= 0) {
                fprintf(file, "  node_%d -> node_%d [label=\"array\"];\n", node_id, array_id);
            }
            if (index_id >= 0) {
                fprintf(file, "  node_%d -> node_%d [label=\"index\"];\n", node_id, index_id);
            }
            if (expr->value.array_access.index_end != NULL) {
                int index_end_id = ExprToDoT(file, expr->value.array_access.index_end);
                if (index_end_id >= 0) {
                    fprintf(file, "  node_%d -> node_%d [label=\"end_index\"];\n", node_id, index_end_id);
                }
            }
            break;
        }

        case EXPR_MEMBER_ACCESS: {
            int obj_id = ExprToDoT(file, expr->value.member_access.object);
            if (obj_id >= 0) {
                fprintf(file, "  node_%d -> node_%d [label=\"object\"];\n", node_id, obj_id);
            }
            fprintf(file, "  node_%d [label=\"member: %s\", shape=ellipse];\n", 
                    GetNextNodeId(), EscapeForDot(expr->value.member_access.member_name));
            break;
        }

        case EXPR_METHOD_CALL: {
            int obj_id = ExprToDoT(file, expr->value.member_access.object);
            if (obj_id >= 0) {
                fprintf(file, "  node_%d -> node_%d [label=\"object\"];\n", node_id, obj_id);
            }
            fprintf(file, "  node_%d [label=\"method: %s\", shape=ellipse, fillcolor=lightyellow];\n", 
                    GetNextNodeId(), EscapeForDot(expr->value.member_access.member_name));
            
            for (int i = 0; i < expr->value.member_access.arg_count; i++) {
                if (expr->value.member_access.args[i] != NULL) {
                    int arg_id = ExprToDoT(file, expr->value.member_access.args[i]);
                    if (arg_id >= 0) {
                        fprintf(file, "  node_%d -> node_%d [label=\"arg[%d]\"];\n", node_id, arg_id, i);
                    }
                }
            }
            break;
        }

        case EXPR_FUNC_CALL: {
            fprintf(file, "  node_%d [label=\"func: %s\", shape=ellipse, fillcolor=lightyellow];\n", 
                    GetNextNodeId(), EscapeForDot(expr->value.func_call.func_name));
            
            for (int i = 0; i < expr->value.func_call.arg_count; i++) {
                if (expr->value.func_call.args[i] != NULL) {
                    int arg_id = ExprToDoT(file, expr->value.func_call.args[i]);
                    if (arg_id >= 0) {
                        fprintf(file, "  node_%d -> node_%d [label=\"arg[%d]\"];\n", node_id, arg_id, i);
                    }
                }
            }
            break;
        }

        case EXPR_NEW: {
            int type_id = TypeToDoT(file, expr->value.new_expr.type);
            if (type_id >= 0) {
                fprintf(file, "  node_%d -> node_%d [label=\"type\"];\n", node_id, type_id);
            }
            for (int i = 0; i < expr->value.new_expr.init_count; i++) {
                if (expr->value.new_expr.init_exprs[i] != NULL) {
                    int init_id = ExprToDoT(file, expr->value.new_expr.init_exprs[i]);
                    if (init_id >= 0) {
                        fprintf(file, "  node_%d -> node_%d [label=\"init[%d]\"];\n", node_id, init_id, i);
                    }
                }
            }
            break;
        }

        case EXPR_SUPER_METHOD: {
            fprintf(file, "  node_%d [label=\"method: %s\", shape=ellipse, fillcolor=lightcyan];\n", 
                    GetNextNodeId(), EscapeForDot(expr->value.member_access.member_name));
            
            for (int i = 0; i < expr->value.member_access.arg_count; i++) {
                if (expr->value.member_access.args[i] != NULL) {
                    int arg_id = ExprToDoT(file, expr->value.member_access.args[i]);
                    if (arg_id >= 0) {
                        fprintf(file, "  node_%d -> node_%d [label=\"arg[%d]\"];\n", node_id, arg_id, i);
                    }
                }
            }
            break;
        }

        default:
            break;
    }

    return node_id;
}

/* ============================================================================
 * ФУНКЦИИ ВИЗУАЛИЗАЦИИ ТИПОВ
 * ============================================================================ */

int TypeToDoT(FILE *file, NType *type) {
    if (type == NULL) {
        return -1;
    }

    int node_id = GetNextNodeId();
    const char *kind_str = "";
    
    switch (type->kind) {
        case TYPE_KIND_BASE:
            kind_str = "BASE";
            break;
        case TYPE_KIND_BASE_ARRAY:
            kind_str = "BASE_ARRAY";
            break;
        case TYPE_KIND_CLASS:
            kind_str = "CLASS";
            break;
        case TYPE_KIND_CLASS_ARRAY:
            kind_str = "CLASS_ARRAY";
            break;
    }

    fprintf(file, "  node_%d [label=\"%s\", shape=diamond, fillcolor=lightgreen];\n", 
            node_id, kind_str);

    if (type->kind == TYPE_KIND_BASE || type->kind == TYPE_KIND_BASE_ARRAY) {
        fprintf(file, "  node_%d [label=\"%s\", shape=record];\n", 
                GetNextNodeId(), BaseTypeToString(type->base_type));
    } else if (type->kind == TYPE_KIND_CLASS || type->kind == TYPE_KIND_CLASS_ARRAY) {
        fprintf(file, "  node_%d [label=\"class: %s\", shape=record];\n", 
                GetNextNodeId(), EscapeForDot(type->class_name));
    }

    return node_id;
}

/* ============================================================================
 * ФУНКЦИИ ВИЗУАЛИЗАЦИИ ОПЕРАТОРОВ
 * ============================================================================ */

int StmtToDoT(FILE *file, NStmt *stmt) {
    if (stmt == NULL) {
        return -1;
    }

    int node_id = GetNextNodeId();
    const char *stmt_type_str = StmtTypeToString(stmt->type);

    fprintf(file, "  node_%d [label=\"%s\", shape=box, fillcolor=lightcoral];\n", 
            node_id, stmt_type_str);

    switch (stmt->type) {
        case STMT_EXPR: {
            int expr_id = ExprToDoT(file, stmt->value.expr);
            if (expr_id >= 0) {
                fprintf(file, "  node_%d -> node_%d [label=\"expr\"];\n", node_id, expr_id);
            }
            break;
        }

        case STMT_DECL: {
            int type_id = TypeToDoT(file, stmt->value.decl.decl_type);
            if (type_id >= 0) {
                fprintf(file, "  node_%d -> node_%d [label=\"type\"];\n", node_id, type_id);
            }
            
            if (stmt->value.decl.init_decls != NULL) {
                for (int i = 0; i < stmt->value.decl.init_decls->count; i++) {
                    int decl_id = GetNextNodeId();
                    fprintf(file, "  node_%d [label=\"%s\", shape=ellipse];\n", 
                            decl_id, EscapeForDot(stmt->value.decl.init_decls->decls[i]->name));
                    fprintf(file, "  node_%d -> node_%d [label=\"decl[%d]\"];\n", node_id, decl_id, i);
                    
                    if (stmt->value.decl.init_decls->decls[i]->initializer != NULL) {
                        NInitializer *init = stmt->value.decl.init_decls->decls[i]->initializer;
                        if (init->is_array == 0 && init->expr != NULL) {
                            int expr_id = ExprToDoT(file, init->expr);
                            if (expr_id >= 0) {
                                fprintf(file, "  node_%d -> node_%d [label=\"init\"];\n", decl_id, expr_id);
                            }
                        }
                    }
                }
            }
            break;
        }

        case STMT_COMPOUND: {
            if (stmt->value.stmt_list != NULL && stmt->value.stmt_list->first != NULL) {
                NStmt *current = stmt->value.stmt_list->first;
                int idx = 0;
                while (current != NULL) {
                    int stmt_id = StmtToDoT(file, current);
                    if (stmt_id >= 0) {
                        fprintf(file, "  node_%d -> node_%d [label=\"stmt[%d]\"];\n", node_id, stmt_id, idx);
                    }
                    current = current->next;
                    idx++;
                }
            }
            break;
        }

        case STMT_RETURN: {
            int expr_id = ExprToDoT(file, stmt->value.expr);
            if (expr_id >= 0) {
                fprintf(file, "  node_%d -> node_%d [label=\"value\"];\n", node_id, expr_id);
            }
            break;
        }

        case STMT_IF: {
            int cond_id = ExprToDoT(file, stmt->value.if_stmt.condition);
            if (cond_id >= 0) {
                fprintf(file, "  node_%d -> node_%d [label=\"condition\"];\n", node_id, cond_id);
            }
            
            int then_id = StmtToDoT(file, stmt->value.if_stmt.then_stmt);
            if (then_id >= 0) {
                fprintf(file, "  node_%d -> node_%d [label=\"then\"];\n", node_id, then_id);
            }
            
            if (stmt->value.if_stmt.else_stmt != NULL) {
                int else_id = StmtToDoT(file, stmt->value.if_stmt.else_stmt);
                if (else_id >= 0) {
                    fprintf(file, "  node_%d -> node_%d [label=\"else\"];\n", node_id, else_id);
                }
            }
            break;
        }

        case STMT_WHILE: {
            int cond_id = ExprToDoT(file, stmt->value.while_stmt.condition);
            if (cond_id >= 0) {
                fprintf(file, "  node_%d -> node_%d [label=\"condition\"];\n", node_id, cond_id);
            }
            
            int body_id = StmtToDoT(file, stmt->value.while_stmt.body);
            if (body_id >= 0) {
                fprintf(file, "  node_%d -> node_%d [label=\"body\"];\n", node_id, body_id);
            }
            break;
        }

        case STMT_DO_WHILE: {
            int body_id = StmtToDoT(file, stmt->value.do_while_stmt.body);
            if (body_id >= 0) {
                fprintf(file, "  node_%d -> node_%d [label=\"body\"];\n", node_id, body_id);
            }
            
            int cond_id = ExprToDoT(file, stmt->value.do_while_stmt.condition);
            if (cond_id >= 0) {
                fprintf(file, "  node_%d -> node_%d [label=\"condition\"];\n", node_id, cond_id);
            }
            break;
        }

        case STMT_FOR: {
            if (stmt->value.for_stmt.init_expr != NULL) {
                int init_id = ExprToDoT(file, stmt->value.for_stmt.init_expr);
                if (init_id >= 0) {
                    fprintf(file, "  node_%d -> node_%d [label=\"init\"];\n", node_id, init_id);
                }
            }
            
            if (stmt->value.for_stmt.cond_expr != NULL) {
                int cond_id = ExprToDoT(file, stmt->value.for_stmt.cond_expr);
                if (cond_id >= 0) {
                    fprintf(file, "  node_%d -> node_%d [label=\"condition\"];\n", node_id, cond_id);
                }
            }
            
            if (stmt->value.for_stmt.iter_expr != NULL) {
                int iter_id = ExprToDoT(file, stmt->value.for_stmt.iter_expr);
                if (iter_id >= 0) {
                    fprintf(file, "  node_%d -> node_%d [label=\"iteration\"];\n", node_id, iter_id);
                }
            }
            
            int body_id = StmtToDoT(file, stmt->value.for_stmt.body);
            if (body_id >= 0) {
                fprintf(file, "  node_%d -> node_%d [label=\"body\"];\n", node_id, body_id);
            }
            break;
        }

        case STMT_FOREACH: {
            fprintf(file, "  node_%d [label=\"var: %s\", shape=ellipse];\n", 
                    GetNextNodeId(), EscapeForDot(stmt->value.foreach_stmt.var_name));
            
            int coll_id = ExprToDoT(file, stmt->value.foreach_stmt.collection);
            if (coll_id >= 0) {
                fprintf(file, "  node_%d -> node_%d [label=\"collection\"];\n", node_id, coll_id);
            }
            
            int body_id = StmtToDoT(file, stmt->value.foreach_stmt.body);
            if (body_id >= 0) {
                fprintf(file, "  node_%d -> node_%d [label=\"body\"];\n", node_id, body_id);
            }
            break;
        }

        case STMT_SWITCH: {
            int expr_id = ExprToDoT(file, stmt->value.switch_stmt.expr);
            if (expr_id >= 0) {
                fprintf(file, "  node_%d -> node_%d [label=\"expr\"];\n", node_id, expr_id);
            }
            
            for (int i = 0; i < stmt->value.switch_stmt.cases.count; i++) {
                int case_id = GetNextNodeId();
                if (stmt->value.switch_stmt.cases.items[i]->type == CASE_ITEM_CASE) {
                    fprintf(file, "  node_%d [label=\"case\", shape=box];\n", case_id);
                    int val_id = ExprToDoT(file, stmt->value.switch_stmt.cases.items[i]->case_expr);
                    if (val_id >= 0) {
                        fprintf(file, "  node_%d -> node_%d [label=\"value\"];\n", case_id, val_id);
                    }
                } else {
                    fprintf(file, "  node_%d [label=\"default\", shape=box];\n", case_id);
                }
                fprintf(file, "  node_%d -> node_%d [label=\"case[%d]\"];\n", node_id, case_id, i);
            }
            break;
        }

        case STMT_BREAK:
        case STMT_CONTINUE:
            /* Нет подузлов */
            break;
    }

    return node_id;
}

/* ============================================================================
 * ФУНКЦИЯ ВИЗУАЛИЗАЦИИ ПРОГРАММЫ
 * ============================================================================ */

void ProgramToDoT(FILE *file, NProgram *program) {
    if (program == NULL || file == NULL) {
        return;
    }

    ResetNodeCounter();

    fprintf(file, "digraph program {\n");
    fprintf(file, "  rankdir=TB;\n");
    fprintf(file, "  node [shape=box, style=filled, fillcolor=white];\n");
    fprintf(file, "  bgcolor=lightgray;\n\n");

    int root_id = GetNextNodeId();
    fprintf(file, "  node_%d [label=\"PROGRAM\", shape=house, fillcolor=gold];\n", root_id);

    if (program->first_item != NULL) {
        NSourceItem *current = program->first_item;
        int idx = 0;
        while (current != NULL) {
            int item_id = GetNextNodeId();
            
            switch (current->type) {
                case SOURCE_ITEM_FUNC:
                    fprintf(file, "  node_%d [label=\"FUNC: %s\", shape=component, fillcolor=lightblue];\n", 
                            item_id, EscapeForDot(current->value.func->func_name));
                    break;
                case SOURCE_ITEM_CLASS:
                    fprintf(file, "  node_%d [label=\"CLASS: %s\", shape=component, fillcolor=lightyellow];\n", 
                            item_id, EscapeForDot(current->value.class_def->class_name));
                    break;
                case SOURCE_ITEM_DECL:
                    fprintf(file, "  node_%d [label=\"DECL\", shape=component, fillcolor=lightgreen];\n", item_id);
                    break;
                case SOURCE_ITEM_ENUM:
                    fprintf(file, "  node_%d [label=\"ENUM\", shape=component, fillcolor=lightyellow];\n", item_id);
                    break;
            }
            
            fprintf(file, "  node_%d -> node_%d [label=\"item[%d]\"];\n", root_id, item_id, idx);
            current = current->next;
            idx++;
        }
    }

    fprintf(file, "}\n");
}
