#include "ast_visualizer.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

static long g_node_id = 0;      // Счётчик ID для уникальности узлов
static FILE *g_output = NULL;   // Текущий файл вывода

static long GenerateNodeId(void) {
    return g_node_id++;
}

// Вспомогательная функция для вывода в DOT с форматированием
static void DotPrintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(g_output, format, args);
    va_end(args);
}

// Экранирует специальные символы для DOT
static const char* EscapeString(const char *str) {
    if (!str) return "";
    
    static char buffer[1024];
    int j = 0;
    
    for (int i = 0; str[i] && j < 1022; i++) {
        switch (str[i]) {
            case '"':  buffer[j++] = '\\'; buffer[j++] = '"'; break;
            case '\\': buffer[j++] = '\\'; buffer[j++] = '\\'; break;
            case '\n': buffer[j++] = '\\'; buffer[j++] = 'n'; break;
            case '\r': buffer[j++] = '\\'; buffer[j++] = 'r'; break;
            case '\t': buffer[j++] = '\\'; buffer[j++] = 't'; break;
            default:   buffer[j++] = str[i];
        }
    }
    buffer[j] = '\0';
    return buffer;
}

// Получает строковое представление типа базового типа
static const char* BaseTypeToString(BaseType type) {
    switch (type) {
        case TYPE_INT:      return "int";
        case TYPE_CHAR:     return "char";
        case TYPE_STRING:   return "string";
        case TYPE_BOOL:     return "bool";
        case TYPE_FLOAT:    return "float";
        case TYPE_DOUBLE:   return "double";
        case TYPE_REAL:     return "real";
        case TYPE_VOID:     return "void";
        case TYPE_CLASS:    return "class";
        default:            return "unknown";
    }
}

// Получает строковое представление типа операции
static const char* OpTypeToString(OpType op) {
    switch (op) {
        case OP_PLUS:           return "+";
        case OP_MINUS:          return "-";
        case OP_MUL:            return "*";
        case OP_DIV:            return "/";
        case OP_LT:             return "<";
        case OP_GT:             return ">";
        case OP_LE:             return "<=";
        case OP_GE:             return ">=";
        case OP_EQ:             return "==";
        case OP_NEQ:            return "!=";
        case OP_AND:            return "&&";
        case OP_OR:             return "||";
        case OP_NOT:            return "!";
        case OP_ASSIGN:         return "=";
        case OP_PLUS_ASSIGN:    return "+=";
        case OP_MINUS_ASSIGN:   return "-=";
        case OP_MUL_ASSIGN:     return "*=";
        case OP_DIV_ASSIGN:     return "/=";
        case OP_BITWISE_NOT_ASSIGN: return "~=";
        default:                return "?";
    }
}

// Получает строковое представление типа выражения
static const char* ExprTypeToString(ExprType type) {
    switch (type) {
        case EXPR_IDENT:          return "ident";
        case EXPR_INT:            return "int_lit";
        case EXPR_FLOAT:          return "float_lit";
        case EXPR_CHAR:           return "char_lit";
        case EXPR_STRING:         return "string_lit";
        case EXPR_BOOL:           return "bool_lit";
        case EXPR_NULL:           return "null";
        case EXPR_NAN:            return "nan";
        case EXPR_THIS:           return "this";
        case EXPR_PAREN:          return "paren";
        case EXPR_NEW:            return "new";
        case EXPR_ARRAY_ACCESS:   return "array_access";
        case EXPR_MEMBER_ACCESS:  return "member";
        case EXPR_METHOD_CALL:    return "method_call";
        case EXPR_FUNC_CALL:      return "func_call";
        case EXPR_SUPER:          return "super";
        case EXPR_SUPER_METHOD:   return "super_method";
        case EXPR_UNARY_OP:       return "unary_op";
        case EXPR_BINARY_OP:      return "binary_op";
        case EXPR_ASSIGN:         return "assign";
        default:                  return "expr_unknown";
    }
}

// Получает строковое представление типа оператора
static const char* StmtTypeToString(StmtType type) {
    switch (type) {
        case STMT_EXPR:       return "expr_stmt";
        case STMT_DECL:       return "decl";
        case STMT_COMPOUND:   return "compound";
        case STMT_IF:         return "if";
        case STMT_WHILE:      return "while";
        case STMT_DO_WHILE:   return "do_while";
        case STMT_FOR:        return "for";
        case STMT_FOREACH:    return "foreach";
        case STMT_SWITCH:     return "switch";
        case STMT_RETURN:     return "return";
        case STMT_BREAK:      return "break";
        case STMT_CONTINUE:   return "continue";
        default:              return "stmt_unknown";
    }
}

/* ============================================================================
   ВИЗУАЛИЗАЦИЯ ВЫРАЖЕНИЙ
   ============================================================================ */

// Функция для визуализации выражений
static long VisualizeExpr(NExpr *expr) {
    if (!expr) return -1;
    
    long node_id = GenerateNodeId();
    const char *type_str = ExprTypeToString(expr->type);
    
    DotPrintf("    node_%ld [label=\"%s\"];\n", node_id, type_str);
    
    switch (expr->type) {
        case EXPR_IDENT:
            DotPrintf("    node_%ld [label=\"ident: %s\"];\n", node_id, 
                     EscapeString(expr->value.ident_name));
            break;
            
        case EXPR_INT:
            DotPrintf("    node_%ld [label=\"int: %d\"];\n", node_id, 
                     expr->value.int_value);
            break;
            
        case EXPR_FLOAT:
            DotPrintf("    node_%ld [label=\"float: %g\"];\n", node_id, 
                     expr->value.float_value);
            break;
            
        case EXPR_CHAR:
            DotPrintf("    node_%ld [label=\"char: '%c'\"];\n", node_id, 
                     expr->value.char_value);
            break;
            
        case EXPR_STRING:
            DotPrintf("    node_%ld [label=\"string: \\\"%s\\\"\"];\n", node_id, 
                     EscapeString(expr->value.string_value));
            break;
            
        case EXPR_BOOL:
            DotPrintf("    node_%ld [label=\"bool: %s\"];\n", node_id, 
                     expr->value.int_value ? "true" : "false");
            break;
            
        case EXPR_NULL:
            DotPrintf("    node_%ld [label=\"null\"];\n", node_id);
            break;
            
        case EXPR_NAN:
            DotPrintf("    node_%ld [label=\"nan\"];\n", node_id);
            break;
            
        case EXPR_THIS:
            DotPrintf("    node_%ld [label=\"this\"];\n", node_id);
            break;
            
        case EXPR_PAREN: {
            DotPrintf("    node_%ld [label=\"paren\"];\n", node_id);
            long inner_id = VisualizeExpr(expr->value.inner_expr);
            if (inner_id >= 0) {
                DotPrintf("    node_%ld -> node_%ld [label=\"expr\"];\n", node_id, inner_id);
            }
            break;
        }
            
        case EXPR_UNARY_OP: {
            const char *op_str = OpTypeToString(expr->value.unary.op);
            DotPrintf("    node_%ld [label=\"unary: %s\"];\n", node_id, op_str);
            long operand_id = VisualizeExpr(expr->value.unary.operand);
            if (operand_id >= 0) {
                DotPrintf("    node_%ld -> node_%ld [label=\"operand\"];\n", node_id, operand_id);
            }
            break;
        }
            
        case EXPR_BINARY_OP: {
            const char *op_str = OpTypeToString(expr->value.binary.op);
            DotPrintf("    node_%ld [label=\"binary: %s\"];\n", node_id, op_str);
            long left_id = VisualizeExpr(expr->value.binary.left);
            long right_id = VisualizeExpr(expr->value.binary.right);
            if (left_id >= 0) {
                DotPrintf("    node_%ld -> node_%ld [label=\"left\"];\n", node_id, left_id);
            }
            if (right_id >= 0) {
                DotPrintf("    node_%ld -> node_%ld [label=\"right\"];\n", node_id, right_id);
            }
            break;
        }
            
        case EXPR_ASSIGN: {
            const char *op_str = OpTypeToString(expr->value.binary.op);
            DotPrintf("    node_%ld [label=\"assign: %s\"];\n", node_id, op_str);
            long left_id = VisualizeExpr(expr->value.binary.left);
            long right_id = VisualizeExpr(expr->value.binary.right);
            if (left_id >= 0) {
                DotPrintf("    node_%ld -> node_%ld [label=\"target\"];\n", node_id, left_id);
            }
            if (right_id >= 0) {
                DotPrintf("    node_%ld -> node_%ld [label=\"value\"];\n", node_id, right_id);
            }
            break;
        }
            
        case EXPR_ARRAY_ACCESS: {
            DotPrintf("    node_%ld [label=\"[]\"];\n", node_id);
            long array_id = VisualizeExpr(expr->value.array_access.array);
            long index_id = VisualizeExpr(expr->value.array_access.index);
            if (array_id >= 0) {
                DotPrintf("    node_%ld -> node_%ld [label=\"array\"];\n", node_id, array_id);
            }
            if (index_id >= 0) {
                DotPrintf("    node_%ld -> node_%ld [label=\"index\"];\n", node_id, index_id);
            }
            if (expr->value.array_access.index_end) {
                long index_end_id = VisualizeExpr(expr->value.array_access.index_end);
                if (index_end_id >= 0) {
                    DotPrintf("    node_%ld -> node_%ld [label=\"index_end\"];\n", node_id, index_end_id);
                }
            }
            break;
        }
            
        case EXPR_MEMBER_ACCESS: {
            DotPrintf("    node_%ld [label=\"member: %s\"];\n", node_id, 
                     EscapeString(expr->value.member_access.member_name));
            long obj_id = VisualizeExpr(expr->value.member_access.object);
            if (obj_id >= 0) {
                DotPrintf("    node_%ld -> node_%ld [label=\"object\"];\n", node_id, obj_id);
            }
            break;
        }
            
        case EXPR_METHOD_CALL: {
            DotPrintf("    node_%ld [label=\"method_call: %s\"];\n", node_id, 
                     EscapeString(expr->value.member_access.member_name));
            long obj_id = VisualizeExpr(expr->value.member_access.object);
            if (obj_id >= 0) {
                DotPrintf("    node_%ld -> node_%ld [label=\"object\"];\n", node_id, obj_id);
            }
            for (int i = 0; i < expr->value.member_access.arg_count; i++) {
                long arg_id = VisualizeExpr(expr->value.member_access.args[i]);
                if (arg_id >= 0) {
                    DotPrintf("    node_%ld -> node_%ld [label=\"arg_%d\"];\n", node_id, arg_id, i);
                }
            }
            break;
        }
            
        case EXPR_FUNC_CALL: {
            DotPrintf("    node_%ld [label=\"func_call: %s\"];\n", node_id, 
                     EscapeString(expr->value.func_call.func_name));
            for (int i = 0; i < expr->value.func_call.arg_count; i++) {
                long arg_id = VisualizeExpr(expr->value.func_call.args[i]);
                if (arg_id >= 0) {
                    DotPrintf("    node_%ld -> node_%ld [label=\"arg_%d\"];\n", node_id, arg_id, i);
                }
            }
            break;
        }
            
        case EXPR_NEW: {
            DotPrintf("    node_%ld [label=\"new\"];\n", node_id);
            for (int i = 0; i < expr->value.new_expr.init_count; i++) {
                long init_id = VisualizeExpr(expr->value.new_expr.init_exprs[i]);
                if (init_id >= 0) {
                    DotPrintf("    node_%ld -> node_%ld [label=\"init_%d\"];\n", node_id, init_id, i);
                }
            }
            break;
        }
            
        case EXPR_SUPER:
            DotPrintf("    node_%ld [label=\"super.%s\"];\n", node_id, 
                     EscapeString(expr->value.ident_name));
            break;
            
        case EXPR_SUPER_METHOD: {
            DotPrintf("    node_%ld [label=\"super.method: %s\"];\n", node_id, 
                     EscapeString(expr->value.member_access.member_name));
            for (int i = 0; i < expr->value.member_access.arg_count; i++) {
                long arg_id = VisualizeExpr(expr->value.member_access.args[i]);
                if (arg_id >= 0) {
                    DotPrintf("    node_%ld -> node_%ld [label=\"arg_%d\"];\n", node_id, arg_id, i);
                }
            }
            break;
        }
            
        default:
            break;
    }
    
    return node_id;
}


// Функция для визуализации типов
static long VisualizeType(NType *type) {
    if (!type) return -1;
    
    long node_id = GenerateNodeId();
    
    if (type->kind == TYPE_KIND_BASE || type->kind == TYPE_KIND_BASE_ARRAY) {
        const char *base = BaseTypeToString(type->base_type);
        if (type->kind == TYPE_KIND_BASE_ARRAY) {
            int size = type->array_decl ? type->array_decl->size : 0;
            DotPrintf("    node_%ld [label=\"type: %s[%d]\"];\n", node_id, base, size);
        } else {
            DotPrintf("    node_%ld [label=\"type: %s\"];\n", node_id, base);
        }
    } else if (type->kind == TYPE_KIND_CLASS || type->kind == TYPE_KIND_CLASS_ARRAY) {
        if (type->kind == TYPE_KIND_CLASS_ARRAY) {
            int size = type->array_decl ? type->array_decl->size : 0;
            DotPrintf("    node_%ld [label=\"type: %s[%d]\"];\n", node_id, 
                     EscapeString(type->class_name), size);
        } else {
            DotPrintf("    node_%ld [label=\"type: %s\"];\n", node_id, 
                     EscapeString(type->class_name));
        }
    }
    
    return node_id;
}

// Функция для визуализации операторов
static long VisualizeStmt(NStmt *stmt) {
    if (!stmt) return -1;
    
    long node_id = GenerateNodeId();
    const char *type_str = StmtTypeToString(stmt->type);
    
    DotPrintf("    node_%ld [label=\"%s\"];\n", node_id, type_str);
    
    switch (stmt->type) {
        case STMT_EXPR: {
            long expr_id = VisualizeExpr(stmt->value.expr);
            if (expr_id >= 0) {
                DotPrintf("    node_%ld -> node_%ld [label=\"expr\"];\n", node_id, expr_id);
            }
            break;
        }
            
        case STMT_DECL: {
            DotPrintf("    node_%ld [label=\"decl\"];\n", node_id);
            long type_id = VisualizeType(stmt->value.decl.decl_type);
            if (type_id >= 0) {
                DotPrintf("    node_%ld -> node_%ld [label=\"type\"];\n", node_id, type_id);
            }
            if (stmt->value.decl.init_decls) {
                for (int i = 0; i < stmt->value.decl.init_decls->count; i++) {
                    NInitDecl *init_decl = stmt->value.decl.init_decls->decls[i];
                    long var_id = GenerateNodeId();
                    DotPrintf("    node_%ld [label=\"var: %s\"];\n", var_id, 
                             EscapeString(init_decl->name));
                    DotPrintf("    node_%ld -> node_%ld [label=\"var_%d\"];\n", node_id, var_id, i);
                    
                    if (init_decl->initializer && init_decl->initializer->expr) {
                        long init_id = VisualizeExpr(init_decl->initializer->expr);
                        if (init_id >= 0) {
                            DotPrintf("    node_%ld -> node_%ld [label=\"init\"];\n", var_id, init_id);
                        }
                    }
                }
            }
            break;
        }
            
        case STMT_COMPOUND: {
            if (stmt->value.stmt_list && stmt->value.stmt_list->first) {
                NStmt *s = stmt->value.stmt_list->first;
                int idx = 0;
                while (s) {
                    long stmt_id = VisualizeStmt(s);
                    if (stmt_id >= 0) {
                        DotPrintf("    node_%ld -> node_%ld [label=\"stmt_%d\"];\n", 
                                 node_id, stmt_id, idx++);
                    }
                    s = s->next;
                }
            }
            break;
        }
            
        case STMT_IF: {
            long cond_id = VisualizeExpr(stmt->value.if_stmt.condition);
            long then_id = VisualizeStmt(stmt->value.if_stmt.then_stmt);
            if (cond_id >= 0) {
                DotPrintf("    node_%ld -> node_%ld [label=\"condition\"];\n", node_id, cond_id);
            }
            if (then_id >= 0) {
                DotPrintf("    node_%ld -> node_%ld [label=\"then\"];\n", node_id, then_id);
            }
            if (stmt->value.if_stmt.else_stmt) {
                long else_id = VisualizeStmt(stmt->value.if_stmt.else_stmt);
                if (else_id >= 0) {
                    DotPrintf("    node_%ld -> node_%ld [label=\"else\"];\n", node_id, else_id);
                }
            }
            break;
        }
            
        case STMT_WHILE: {
            long cond_id = VisualizeExpr(stmt->value.while_stmt.condition);
            long body_id = VisualizeStmt(stmt->value.while_stmt.body);
            if (cond_id >= 0) {
                DotPrintf("    node_%ld -> node_%ld [label=\"condition\"];\n", node_id, cond_id);
            }
            if (body_id >= 0) {
                DotPrintf("    node_%ld -> node_%ld [label=\"body\"];\n", node_id, body_id);
            }
            break;
        }
            
        case STMT_DO_WHILE: {
            long body_id = VisualizeStmt(stmt->value.do_while_stmt.body);
            long cond_id = VisualizeExpr(stmt->value.do_while_stmt.condition);
            if (body_id >= 0) {
                DotPrintf("    node_%ld -> node_%ld [label=\"body\"];\n", node_id, body_id);
            }
            if (cond_id >= 0) {
                DotPrintf("    node_%ld -> node_%ld [label=\"condition\"];\n", node_id, cond_id);
            }
            break;
        }
            
        case STMT_FOR: {
            if (stmt->value.for_stmt.init_expr) {
                long init_id = VisualizeExpr(stmt->value.for_stmt.init_expr);
                if (init_id >= 0) {
                    DotPrintf("    node_%ld -> node_%ld [label=\"init\"];\n", node_id, init_id);
                }
            } else if (stmt->value.for_stmt.init_decl_type) {
                long type_id = VisualizeType(stmt->value.for_stmt.init_decl_type);
                if (type_id >= 0) {
                    DotPrintf("    node_%ld -> node_%ld [label=\"init_type\"];\n", node_id, type_id);
                }
            }
            
            if (stmt->value.for_stmt.cond_expr) {
                long cond_id = VisualizeExpr(stmt->value.for_stmt.cond_expr);
                if (cond_id >= 0) {
                    DotPrintf("    node_%ld -> node_%ld [label=\"condition\"];\n", node_id, cond_id);
                }
            }
            
            if (stmt->value.for_stmt.iter_expr) {
                long iter_id = VisualizeExpr(stmt->value.for_stmt.iter_expr);
                if (iter_id >= 0) {
                    DotPrintf("    node_%ld -> node_%ld [label=\"iterate\"];\n", node_id, iter_id);
                }
            }
            
            long body_id = VisualizeStmt(stmt->value.for_stmt.body);
            if (body_id >= 0) {
                DotPrintf("    node_%ld -> node_%ld [label=\"body\"];\n", node_id, body_id);
            }
            break;
        }
            
        case STMT_FOREACH: {
            long coll_id = VisualizeExpr(stmt->value.foreach_stmt.collection);
            long body_id = VisualizeStmt(stmt->value.foreach_stmt.body);
            
            long var_id = GenerateNodeId();
            DotPrintf("    node_%ld [label=\"var: %s\"];\n", var_id, 
                     EscapeString(stmt->value.foreach_stmt.var_name));
            DotPrintf("    node_%ld -> node_%ld [label=\"var\"];\n", node_id, var_id);
            
            if (coll_id >= 0) {
                DotPrintf("    node_%ld -> node_%ld [label=\"collection\"];\n", node_id, coll_id);
            }
            if (body_id >= 0) {
                DotPrintf("    node_%ld -> node_%ld [label=\"body\"];\n", node_id, body_id);
            }
            break;
        }
            
        case STMT_SWITCH: {
            long expr_id = VisualizeExpr(stmt->value.switch_stmt.expr);
            if (expr_id >= 0) {
                DotPrintf("    node_%ld -> node_%ld [label=\"expr\"];\n", node_id, expr_id);
            }
            
            for (int i = 0; i < stmt->value.switch_stmt.cases.count; i++) {
                NCaseItem *case_item = stmt->value.switch_stmt.cases.items[i];
                long case_id = GenerateNodeId();
                
                if (case_item->type == CASE_ITEM_CASE) {
                    DotPrintf("    node_%ld [label=\"case\"];\n", case_id);
                    long val_id = VisualizeExpr(case_item->case_expr);
                    if (val_id >= 0) {
                        DotPrintf("    node_%ld -> node_%ld [label=\"value\"];\n", case_id, val_id);
                    }
                } else {
                    DotPrintf("    node_%ld [label=\"default\"];\n", case_id);
                }
                
                DotPrintf("    node_%ld -> node_%ld [label=\"case_%d\"];\n", node_id, case_id, i);
                
                if (case_item->stmts && case_item->stmts->first) {
                    NStmt *s = case_item->stmts->first;
                    int idx = 0;
                    while (s) {
                        long stmt_id = VisualizeStmt(s);
                        if (stmt_id >= 0) {
                            DotPrintf("    node_%ld -> node_%ld [label=\"stmt_%d\"];\n", 
                                     case_id, stmt_id, idx++);
                        }
                        s = s->next;
                    }
                }
            }
            break;
        }
            
        case STMT_RETURN: {
            if (stmt->value.expr) {
                long expr_id = VisualizeExpr(stmt->value.expr);
                if (expr_id >= 0) {
                    DotPrintf("    node_%ld -> node_%ld [label=\"value\"];\n", node_id, expr_id);
                }
            }
            break;
        }
            
        case STMT_BREAK:
        case STMT_CONTINUE:
            break;
            
        default:
            break;
    }
    
    return node_id;
}

// Визуализирует параметры функции
static void VisualizeParams(long parent_id, NParamList *params, const char *edge_label) {
    if (!params || params->count == 0) return;
    
    long params_id = GenerateNodeId();
    DotPrintf("    node_%ld [label=\"params\"];\n", params_id);
    DotPrintf("    node_%ld -> node_%ld [label=\"%s\"];\n", parent_id, params_id, edge_label);
    
    for (int i = 0; i < params->count; i++) {
        NParam *param = params->params[i];
        long param_id = GenerateNodeId();
        
        const char *ref_str = param->is_ref ? "ref " : "";
        DotPrintf("    node_%ld [label=\"param: %s%s\"];\n", param_id, ref_str, 
                 EscapeString(param->param_name));
        DotPrintf("    node_%ld -> node_%ld [label=\"param_%d\"];\n", params_id, param_id, i);
        
        if (param->param_type) {
            long type_id = VisualizeType(param->param_type);
            if (type_id >= 0) {
                DotPrintf("    node_%ld -> node_%ld [label=\"type\"];\n", param_id, type_id);
            }
        }
    }
}

// Визуализирует функцию
static long VisualizeFunc(NFuncDef *func) {
    if (!func) return -1;
    
    long func_id = GenerateNodeId();
    
    const char *ret_type = func->return_type ? BaseTypeToString(func->return_type->base_type) : "void";
    DotPrintf("    node_%ld [label=\"func: %s() -> %s\"];\n", func_id, 
             EscapeString(func->func_name), ret_type);
    
    VisualizeParams(func_id, func->params, "params");
    
    if (func->body) {
        long body_id = VisualizeStmt(func->body);
        if (body_id >= 0) {
            DotPrintf("    node_%ld -> node_%ld [label=\"body\"];\n", func_id, body_id);
        }
    }
    
    return func_id;
}

// Визуализирует метод
static long VisualizeMethod(NMethodDef *method) {
    if (!method) return -1;
    
    long method_id = GenerateNodeId();
    
    const char *override_str = method->is_override ? "override " : "";
    const char *ret_type = method->return_type ? BaseTypeToString(method->return_type->base_type) : "void";
    DotPrintf("    node_%ld [label=\"method: %s%s() -> %s\"];\n", method_id, override_str,
             EscapeString(method->method_name), ret_type);
    
    VisualizeParams(method_id, method->params, "params");
    
    if (method->body) {
        long body_id = VisualizeStmt(method->body);
        if (body_id >= 0) {
            DotPrintf("    node_%ld -> node_%ld [label=\"body\"];\n", method_id, body_id);
        }
    }
    
    return method_id;
}

// Визуализирует конструктор
static long VisualizeCtor(NCtorDef *ctor) {
    if (!ctor) return -1;
    
    long ctor_id = GenerateNodeId();
    DotPrintf("    node_%ld [label=\"constructor\"];\n", ctor_id);
    
    VisualizeParams(ctor_id, ctor->params, "params");
    
    if (ctor->body) {
        long body_id = VisualizeStmt(ctor->body);
        if (body_id >= 0) {
            DotPrintf("    node_%ld -> node_%ld [label=\"body\"];\n", ctor_id, body_id);
        }
    }
    
    return ctor_id;
}

// Визуализирует деструктор
static long VisualizeDtor(NDtorDef *dtor) {
    if (!dtor) return -1;
    
    long dtor_id = GenerateNodeId();
    DotPrintf("    node_%ld [label=\"destructor\"];\n", dtor_id);
    
    if (dtor->body) {
        long body_id = VisualizeStmt(dtor->body);
        if (body_id >= 0) {
            DotPrintf("    node_%ld -> node_%ld [label=\"body\"];\n", dtor_id, body_id);
        }
    }
    
    return dtor_id;
}

// Визуализирует класс
static long VisualizeClass(NClassDef *class_def) {
    if (!class_def) return -1;
    
    long class_id = GenerateNodeId();
    
    const char *base_str = class_def->base_class_name ? class_def->base_class_name : "Object";
    DotPrintf("    node_%ld [label=\"class: %s extends %s\"];\n", class_id, 
             EscapeString(class_def->class_name), EscapeString(base_str));
    
    if (class_def->members.first) {
        NClassMember *member = class_def->members.first;
        int member_idx = 0;
        
        while (member) {
            long member_id = GenerateNodeId();
            
            switch (member->type) {
                case CLASS_MEMBER_FIELD: {
                    const char *access_str = "";
                    switch (member->access) {
                        case ACCESS_PUBLIC:    access_str = "public "; break;
                        case ACCESS_PRIVATE:   access_str = "private "; break;
                        case ACCESS_PROTECTED: access_str = "protected "; break;
                    }
                    DotPrintf("    node_%ld [label=\"%sfield\"];\n", member_id, access_str);
                    long type_id = VisualizeType(member->value.field.field_type);
                    if (type_id >= 0) {
                        DotPrintf("    node_%ld -> node_%ld [label=\"type\"];\n", member_id, type_id);
                    }
                    break;
                }
                    
                case CLASS_MEMBER_METHOD: {
                    long method_id = VisualizeMethod(member->value.method);
                    if (method_id >= 0) {
                        member_id = method_id;
                    }
                    break;
                }
                    
                case CLASS_MEMBER_CTOR: {
                    long ctor_id = VisualizeCtor(member->value.ctor);
                    if (ctor_id >= 0) {
                        member_id = ctor_id;
                    }
                    break;
                }
                    
                case CLASS_MEMBER_DTOR: {
                    long dtor_id = VisualizeDtor(member->value.dtor);
                    if (dtor_id >= 0) {
                        member_id = dtor_id;
                    }
                    break;
                }
                    
                case CLASS_MEMBER_ENUM: {
                    long enum_id = GenerateNodeId();
                    DotPrintf("    node_%ld [label=\"enum: %s\"];\n", enum_id, 
                             EscapeString(member->value.enum_def->enum_name));
                    member_id = enum_id;
                    break;
                }
            }
            
            DotPrintf("    node_%ld -> node_%ld [label=\"member_%d\"];\n", class_id, member_id, member_idx++);
            member = member->next;
        }
    }
    
    return class_id;
}

// Основная функция визуализации программы
static void VisualizeProgram(NProgram *program) {
    if (!program) {
        DotPrintf("    // Пустая программа\n");
        return;
    }
    
    long prog_id = GenerateNodeId();
    DotPrintf("    node_%ld [label=\"Program\"];\n", prog_id);
    
    if (program->first_item) {
        NSourceItem *item = program->first_item;
        int item_idx = 0;
        
        while (item) {
            long item_id = -1;
            
            switch (item->type) {
                case SOURCE_ITEM_FUNC:
                    item_id = VisualizeFunc(item->value.func);
                    break;
                    
                case SOURCE_ITEM_CLASS:
                    item_id = VisualizeClass(item->value.class_def);
                    break;
                    
                case SOURCE_ITEM_DECL: {
                    long decl_id = GenerateNodeId();
                    DotPrintf("    node_%ld [label=\"global_decl\"];\n", decl_id);
                    long type_id = VisualizeType(item->value.decl.item_type);
                    if (type_id >= 0) {
                        DotPrintf("    node_%ld -> node_%ld [label=\"type\"];\n", decl_id, type_id);
                    }
                    item_id = decl_id;
                    break;
                }
                    
                case SOURCE_ITEM_ENUM: {
                    long enum_id = GenerateNodeId();
                    DotPrintf("    node_%ld [label=\"enum: %s\"];\n", enum_id, 
                             EscapeString(item->value.enum_def->enum_name));
                    item_id = enum_id;
                    break;
                }
            }
            
            if (item_id >= 0) {
                DotPrintf("    node_%ld -> node_%ld [label=\"item_%d\"];\n", prog_id, item_id, item_idx++);
            }
            
            item = item->next;
        }
    }
}

// Генерирует DOT код для визуализации синтаксического дерева
void VisualizeASTToFile(NProgram *program, const char *output_file) {
    if (!output_file) {
        fprintf(stderr, "Error: output file name is NULL\n");
        return;
    }
    
    g_output = fopen(output_file, "w");
    if (!g_output) {
        fprintf(stderr, "Error: cannot open file '%s' for writing\n", output_file);
        return;
    }
    
    g_node_id = 0;
    
    DotPrintf("digraph AST {\n");
    DotPrintf("    rankdir=TB;\n");
    DotPrintf("    node [shape=box, style=rounded, fontname=\"Arial\"];\n");
    DotPrintf("    edge [fontname=\"Arial\", fontsize=10];\n\n");
    
    VisualizeProgram(program);
    
    DotPrintf("}\n");
    
    fclose(g_output);
    g_output = NULL;
    
    printf("AST visualization saved to '%s'\n", output_file);
    printf("To convert to PNG, run: dot -Tpng %s -o %s.png\n", output_file, output_file);
}

// Выводит DOT код в стандартный поток вывода
void VisualizeASTToStdout(NProgram *program) {
    g_output = stdout;
    g_node_id = 0;
    
    DotPrintf("digraph AST {\n");
    DotPrintf("    rankdir=TB;\n");
    DotPrintf("    node [shape=box, style=rounded, fontname=\"Arial\"];\n");
    DotPrintf("    edge [fontname=\"Arial\", fontsize=10];\n\n");
    
    VisualizeProgram(program);
    
    DotPrintf("}\n");
}
