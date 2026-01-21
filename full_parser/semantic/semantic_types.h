#ifndef SEMANTIC_TYPES_H
#define SEMANTIC_TYPES_H

#include <time.h>
#include "tree_nodes.h"

typedef struct JvmLayoutContext JvmLayoutContext;

/* ============================================================================
   ТИПЫ ДЛЯ ТАБЛИЦ СИМВОЛОВ И УПРАВЛЕНИЯ ОБЛАСТЯМИ ВИДИМОСТИ
   ============================================================================ */

/* Вид символа в таблице */
typedef enum {
    SYMBOL_VARIABLE,      /* переменная */
    SYMBOL_FUNCTION,      /* функция */
    SYMBOL_CLASS,         /* класс */
    SYMBOL_ENUM_TYPE,     /* тип enum */
    SYMBOL_ENUM_ITEM      /* элемент enum */
} SymbolKind;

/* Информация о переменной */
typedef struct {
    char *name;           /* имя переменной */
    NType *type;          /* тип переменной */
    int is_initialized;   /* инициализирована ли при объявлении */
    int is_param;         /* является ли параметром функции */
    int is_ref;           /* параметр с ref */
    int line;             /* строка объявления */
    int column;           /* колонка объявления */
} VariableInfo;

/* Информация о функции */
typedef struct {
    char *name;           /* имя функции */
    NType *return_type;   /* тип возвращаемого значения (NULL для void) */
    NParamList *params;   /* список параметров */
    int is_prototype;     /* 1 если есть только прототип (нет тела) */
    int is_builtin;       /* 1 если это встроенная функция */
    int allow_extra_args; /* 1 если допускаются дополнительные аргументы */
    int line;             /* строка объявления */
    int column;           /* колонка объявления */
} FunctionInfo;

/* Информация о методе класса */
typedef struct {
    char *name;           /* имя метода */
    NType *return_type;   /* тип возвращаемого значения */
    NParamList *params;   /* список параметров */
    AccessSpec access;    /* public/private/protected */
    int is_override;      /* является ли override методом */
    int line;
    int column;
} MethodInfo;

/* Информация о поле класса */
typedef struct {
    char *name;           /* имя поля */
    NType *type;          /* тип поля */
    AccessSpec access;    /* public/private/protected */
    int line;
    int column;
} FieldInfo;

/* Информация о классе */
typedef struct {
    char *name;           /* имя класса */
    char *base_class;     /* имя базового класса (NULL если нет) */
    FieldInfo **fields;   /* массив полей */
    int field_count;
    MethodInfo **methods; /* массив методов */
    int method_count;
    NCtorDef **constructors; /* массив конструкторов */
    int constructor_count;
    NDtorDef *destructor;  /* деструктор (может быть NULL) */
    int members_processed; /* обработаны ли члены класса */
    int line;
    int column;
} ClassInfo;

/* Информация об элементе enum */
typedef struct {
    char *name;           /* имя элемента */
    int value;            /* значение элемента */
    int has_explicit_value; /* был ли указан явно */
} EnumItemInfo;

/* Информация об enum */
typedef struct {
    char *name;           /* имя enum (может быть NULL для анонимного) */
    EnumItemInfo **items; /* элементы enum */
    int item_count;
    int line;
    int column;
} EnumInfo;

/* Структура символа в таблице */
typedef struct {
    SymbolKind kind;      /* вид символа */
    char *name;           /* имя символа */
    union {
        VariableInfo *var_info;
        FunctionInfo *func_info;
        ClassInfo *class_info;
        EnumInfo *enum_info;
        EnumItemInfo *enum_item_info;
    } info;
    int scope_depth;      /* глубина области видимости (0 = глобальная) */
} Symbol;

/* Таблица символов - массив с линейным поиском.
   Владение: таблица владеет массивом symbols и самими Symbol объектами.
   Symbol владеет своей копией имени (name). Info-указатели не принадлежат таблице. */
typedef struct {
    Symbol **symbols;     /* массив символов */
    int count;            /* текущее количество */
    int capacity;         /* ёмкость массива */
} SymbolTable;

/* Область видимости (scope).
   Владение: scope владеет таблицей locals (кроме глобального scope),
   scope_name хранится как копия строки (если задана). */
typedef struct Scope {
    int depth;            /* глубина (0 = глобальная, 1+ = локальная) */
    char *scope_name;     /* имя области (имя функции/класса) */
    SymbolTable *locals;  /* локальные символы этой области */
    struct Scope *parent; /* ссылка на родительскую область */
} Scope;

/* Стек областей видимости.
   Владение: стек владеет всеми Scope в массиве scopes.
   Глобальная таблица символов хранится отдельно в SemanticContext. */
typedef struct {
    Scope **scopes;       /* стек областей */
    int count;            /* текущая глубина */
    int capacity;         /* ёмкость массива */
    SymbolTable *global;  /* глобальная таблица символов */
} ScopeStack;

/* Ошибка семантического анализа */
typedef enum {
    SEMANTIC_ERROR_UNDEFINED_VARIABLE,
    SEMANTIC_ERROR_UNDEFINED_FUNCTION,
    SEMANTIC_ERROR_UNDEFINED_CLASS,
    SEMANTIC_ERROR_UNDEFINED_TYPE,
    SEMANTIC_ERROR_DUPLICATE_SYMBOL,
    SEMANTIC_ERROR_TYPE_MISMATCH,
    SEMANTIC_ERROR_INVALID_OPERANDS,
    SEMANTIC_ERROR_WRONG_ARG_COUNT,
    SEMANTIC_ERROR_ARG_TYPE_MISMATCH,
    SEMANTIC_ERROR_ACCESS_VIOLATION,
    SEMANTIC_ERROR_INVALID_ARRAY_INDEX,
    SEMANTIC_ERROR_INVALID_ARRAY_SIZE,
    SEMANTIC_ERROR_METHOD_NOT_FOUND,
    SEMANTIC_ERROR_FIELD_NOT_FOUND,
    SEMANTIC_ERROR_INVALID_BASE_CLASS,
    SEMANTIC_ERROR_INVALID_OVERRIDE,
    SEMANTIC_ERROR_ENUM_DUPLICATE_ITEM,
    SEMANTIC_ERROR_CONSTRUCTOR_RETURN_TYPE,
    SEMANTIC_ERROR_OUT_OF_SCOPE,
    SEMANTIC_ERROR_OTHER
} SemanticErrorKind;

typedef struct {
    int line;
    int column;
} SemanticPosition;

typedef struct {
    SemanticPosition start;
    SemanticPosition end;
} SemanticRange;

typedef struct {
    char *expected;
    char *actual;
    char *name;
    char *signature;
    char *details;
} SemanticErrorContext;

typedef struct {
    SemanticErrorKind kind;
    char *code;
    char *message;
    char *explanation;
    int has_position;
    SemanticPosition position;
    int has_range;
    SemanticRange range;
    int has_related_position;
    SemanticPosition related_position;
    int has_related_range;
    SemanticRange related_range;
    SemanticErrorContext context;
    char *source_text;
} SemanticError;

/* Список ошибок */
typedef struct {
    SemanticError **errors;
    int count;
    int capacity;
} ErrorList;

/* Контекст семантического анализа */
typedef struct {
    SymbolTable *global_symbols;  /* глобальные символы */
    ScopeStack *scope_stack;      /* стек областей видимости */
    ErrorList *errors;            /* список ошибок */
    JvmLayoutContext *jvm;        /* JVM-attribution (descriptors/slots/RefKey) */
    int has_errors;               /* была ли хоть одна ошибка */
    ClassInfo *current_class;     /* текущий класс (для this/super) */
    int loop_depth;               /* loop depth */
    int switch_depth;             /* switch depth */
    /* Таблицы для быстрого поиска */
    ClassInfo **classes;          /* массив информации о классах */
    int class_count;
    FunctionInfo **functions;     /* массив информации о функциях */
    int function_count;
    EnumInfo **enums;             /* массив информации об enum */
    int enum_count;
} SemanticContext;

#endif /* SEMANTIC_TYPES_H */
