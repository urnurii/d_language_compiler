#ifndef TREE_NODES_H
#define TREE_NODES_H

#include <stdlib.h>
#include <string.h>

typedef enum {
    JVM_REF_FIELD,
    JVM_REF_METHOD
} JvmRefKind;

typedef struct {
    int has_key;
    char *owner_internal_name;
    char *member_name;
    char *member_descriptor;
    JvmRefKind kind;
} JvmRefKey;

// Базовые типы
typedef enum {
    TYPE_INT,
    TYPE_CHAR,
    TYPE_STRING,
    TYPE_BOOL,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_REAL,
    TYPE_VOID,
    TYPE_CLASS
} BaseType;

// Типы выражений
typedef enum {
    EXPR_IDENT,
    EXPR_INT,
    EXPR_FLOAT,
    EXPR_CHAR,
    EXPR_STRING,
    EXPR_BOOL,           // TRUE или FALSE
    EXPR_NULL,
    EXPR_NAN,
    EXPR_THIS,
    EXPR_PAREN,          // (expr)
    EXPR_NEW,
    EXPR_ARRAY_ACCESS,   // expr[expr] или expr[expr..expr]
    EXPR_MEMBER_ACCESS,  // expr.IDENT
    EXPR_METHOD_CALL,    // expr.IDENT(...)
    EXPR_FUNC_CALL,      // IDENT(...)
    EXPR_SUPER,          // SUPER.IDENT
    EXPR_SUPER_METHOD,   // SUPER.IDENT(...)
    EXPR_UNARY_OP,       // -expr, +expr, !expr
    EXPR_BINARY_OP,      // expr op expr
    EXPR_ASSIGN          // expr = expr и все составные присваивания
} ExprType;

// Типы операций (для бинарных и унарных операций)
typedef enum {
    OP_PLUS,
    OP_MINUS,
    OP_MUL,
    OP_DIV,
    OP_LT,
    OP_GT,
    OP_LE,
    OP_GE,
    OP_EQ,
    OP_NEQ,
    OP_AND,
    OP_OR,
    OP_NOT,
    OP_ASSIGN,
    OP_PLUS_ASSIGN,
    OP_MINUS_ASSIGN,
    OP_MUL_ASSIGN,
    OP_DIV_ASSIGN,
    OP_BITWISE_NOT_ASSIGN
} OpType;

// Типы типов

typedef enum {
    TYPE_KIND_BASE,
    TYPE_KIND_BASE_ARRAY,
    TYPE_KIND_CLASS,
    TYPE_KIND_CLASS_ARRAY
} TypeKind;

// Структуры для типов и объявлений

// Объявление массива
typedef struct {
    int has_size;       // если указан размер 1, иначе 0
    int size;           // размер массива если has_size == 1 
} NArrayDecl;

// Полное описание типа: base_type [array] или CLASSNAME [array] 
typedef struct NType {
    TypeKind kind;
    BaseType base_type; // если kind == TYPE_KIND_BASE или TYPE_KIND_BASE_ARRAY
    char *class_name;   // если kind == TYPE_KIND_CLASS или TYPE_KIND_CLASS_ARRAY
    NArrayDecl *array_decl;  // указатель на объявление массива если оно есть
} NType;

// Инициализаторы: expr или [expr, expr, ...]
typedef struct NInitializer {
    int is_array;       // если это инициализация массива 1, для expr 0
    struct NExpr *expr; // если is_array == 0
    struct {
        struct NExpr **elements;
        int count;
    } array_init;       // если is_array == 1
} NInitializer;

// Структуры для выражений
typedef struct NExpr {
    ExprType type;
    int line;           // строка для ошибок
    int column;         // колонка для ошибок
    int resolved_symbol_id;
    int scope_id;
    int jvm_slot_index;
    char *jvm_descriptor;
    struct NType *inferred_type;
    JvmRefKey jvm_ref_key;
    char *resolved_owner_internal;
    char *resolved_member_descriptor;
    unsigned char *resolved_arg_is_ref;
    int resolved_arg_count;
    int enum_value;
    int enum_value_is_set;
    
    union {
        char *ident_name;          // EXPR_IDENT, EXPR_SUPER
        int int_value;             // EXPR_INT
        double float_value;        // EXPR_FLOAT
        char char_value;           // EXPR_CHAR
        char *string_value;        // EXPR_STRING
        struct NExpr *inner_expr;  // EXPR_PAREN
        
        // EXPR_NEW
        struct {
            NType *type;
            struct NExpr **init_exprs;
            int init_count;
        } new_expr;
        
        // EXPR_ARRAY_ACCESS
        struct {
            struct NExpr *array;
            struct NExpr *index;
            struct NExpr *index_end;  // для slice expr[a..b]
        } array_access;
        
        // EXPR_MEMBER_ACCESS, EXPR_METHOD_CALL, EXPR_SUPER_METHOD
        struct {
            struct NExpr *object;
            char *member_name;
            struct NExpr **args;
            int arg_count;
        } member_access;
        
        // EXPR_FUNC_CALL
        struct {
            char *func_name;
            struct NExpr **args;
            int arg_count;
        } func_call;
        
        // EXPR_UNARY_OP
        struct {
            OpType op;
            struct NExpr *operand;
        } unary;
        
        // EXPR_BINARY_OP, EXPR_ASSIGN
        struct {
            OpType op;
            struct NExpr *left;
            struct NExpr *right;
        } binary;
        
    } value;
    
} NExpr;

// Список выражений arg_list
typedef struct {
    NExpr **elements;
    int count;
    int capacity;
} NExprList;

// Параметры функции и списки параметров
typedef struct {
    NType *param_type;
    char *param_name;
    int is_ref;              // 1 если параметр с ref
    NExpr *default_value;    // NULL если нет значения по умолчанию
    int resolved_symbol_id;
    int jvm_slot_index;
    char *jvm_descriptor;
} NParam;

typedef struct {
    NParam **params;
    int count;
    int capacity;
} NParamList;

// Объявление переменной: IDENT или IDENT = initializer
typedef struct {
    char *name;
    NInitializer *initializer;  // NULL если нет инициализации
    int resolved_symbol_id;
    int jvm_slot_index;
    char *jvm_descriptor;
} NInitDecl;

// Список объявлений init_decl_list
typedef struct {
    NInitDecl **decls;
    int count;
    int capacity;
} NInitDeclList;

// Структура для результата правила decl
typedef struct {
    NType *type;
    NInitDeclList *init_decls;
} NDeclStmt;

// Типы операторов stmt
typedef enum {
    STMT_EXPR,
    STMT_DECL,
    STMT_COMPOUND,  // { ... } 
    STMT_IF,
    STMT_WHILE,
    STMT_DO_WHILE,
    STMT_FOR,
    STMT_FOREACH,
    STMT_SWITCH,
    STMT_RETURN,
    STMT_BREAK,
    STMT_CONTINUE
} StmtType;

// Структура оператора
typedef struct NStmt {
    StmtType type;
    int line;
    int column;
    int scope_id;
    
    union {
        NExpr *expr;  // STMT_EXPR, STMT_RETURN
        
        // STMT_DECL
        struct {
            NType *decl_type;
            NInitDeclList *init_decls;
        } decl;
        
        struct NStmtList *stmt_list;  // STMT_COMPOUND
        
        // STMT_IF
        struct {
            NExpr *condition;
            struct NStmt *then_stmt;
            struct NStmt *else_stmt;
        } if_stmt;
        
        // STMT_WHILE
        struct {
            NExpr *condition;
            struct NStmt *body;
        } while_stmt;
        
        // STMT_DO_WHILE
        struct {
            struct NStmt *body;
            NExpr *condition;
        } do_while_stmt;
        
        // STMT_FOR
        struct {
            NExpr *init_expr;
            NType *init_decl_type;
            NInitDeclList *init_decls;
            NExpr *cond_expr;
            NExpr *iter_expr;
            struct NStmt *body;
        } for_stmt;
        
        // STMT_FOREACH 
        struct {
            int is_typed;           // 1 если указан тип переменной
            NType *var_type;        // если is_typed == 1
            char *var_name;
            NExpr *collection;
            struct NStmt *body;
        } foreach_stmt;
        
        // STMT_SWITCH
        struct {
            NExpr *expr;
            struct {
                struct NCaseItem **items;
                int count;
            } cases;
        } switch_stmt;
        
    } value;
    
    struct NStmt *next;  // связный список
} NStmt;

/* Список операторов (stmt_list) */
typedef struct NStmtList {
    NStmt *first;
    NStmt *last;
} NStmtList;

// switch
typedef enum {
    CASE_ITEM_CASE,
    CASE_ITEM_DEFAULT
} CaseItemType;

typedef struct NCaseItem {
    CaseItemType type;
    NExpr *case_expr;
    NStmtList *stmts;
} NCaseItem;

typedef struct {
    NCaseItem **items;
    int count;
    int capacity;
} NCaseList;

// Определения функций
typedef struct {
    NType *return_type;   // NULL если VOID
    char *func_name;
    NParamList *params;
    NStmt *body;          // составной оператор или NULL
    char *jvm_descriptor;
} NFuncDef;

// Методы класса
typedef struct {
    int is_override;
    NType *return_type;   // NULL если VOID
    char *method_name;
    NParamList *params;
    NStmt *body;
    char *jvm_descriptor;
} NMethodDef;

// Конструктор и деструктор
typedef struct {
    NParamList *params;
    NStmt *body;
} NCtorDef;

typedef struct {
    NStmt *body;
} NDtorDef;

// Спецификаторы доступа
typedef enum {
    ACCESS_PUBLIC,
    ACCESS_PRIVATE,
    ACCESS_PROTECTED
} AccessSpec;

// Члены класса
typedef enum {
    CLASS_MEMBER_FIELD,
    CLASS_MEMBER_METHOD,
    CLASS_MEMBER_CTOR,
    CLASS_MEMBER_DTOR,
    CLASS_MEMBER_ENUM
} ClassMemberType;

typedef struct NClassMember {
    ClassMemberType type;
    AccessSpec access;
    
    union {
        // CLASS_MEMBER_FIELD
        struct {
            NType *field_type;
            NInitDeclList *init_decls;
        } field;

        NMethodDef *method;         // CLASS_MEMBER_METHOD 
        NCtorDef *ctor;             // CLASS_MEMBER_CTOR
        NDtorDef *dtor;             // CLASS_MEMBER_DTOR
        struct NEnumDef *enum_def;  // CLASS_MEMBER_ENUM
        
    } value;
    
    struct NClassMember *next;
} NClassMember;

// Определения классов
typedef struct NClassDef {
    char *class_name;
    char *base_class_name;
    struct {
        NClassMember *first;
        NClassMember *last;
    } members;
} NClassDef;

// Элементы enum
typedef struct {
    char *name;
    int has_value;
    int value;  // значение если has_value == 1
} NEnumItem;

typedef struct {
    NEnumItem **items;
    int count;
    int capacity;
} NEnumItemList;

// Определения enum
typedef struct NEnumDef {
    char *enum_name;
    struct {
        NEnumItem **items;
        int count;
    } items;
} NEnumDef;

// Элементы верхнего уровня
typedef enum {
    SOURCE_ITEM_FUNC,
    SOURCE_ITEM_CLASS,
    SOURCE_ITEM_DECL,
    SOURCE_ITEM_ENUM
} SourceItemType;

typedef struct NSourceItem {
    SourceItemType type;
    
    union {
        NFuncDef *func;
        NClassDef *class_def;
        struct {
            NType *item_type;
            NInitDeclList *init_decls;
        } decl;
        NEnumDef *enum_def;
    } value;
    
    struct NSourceItem *next;
} NSourceItem;

// Точка входа - синтаксическое дерево
typedef struct {
    NSourceItem *first_item;
    NSourceItem *last_item;
} NProgram;

#endif
