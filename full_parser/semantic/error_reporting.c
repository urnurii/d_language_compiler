#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "error_reporting.h"

static char* DuplicateString(const char *src) {
    size_t len;
    char *dst;

    if (src == NULL) {
        return NULL;
    }

    len = strlen(src);
    dst = (char*)malloc(len + 1);
    if (dst == NULL) {
        return NULL;
    }
    memcpy(dst, src, len + 1);
    return dst;
}

static char* FormatString(const char *fmt, ...) {
    va_list args;
    va_list args_copy;
    int needed;
    char *buffer;

    if (fmt == NULL) {
        return NULL;
    }

    va_start(args, fmt);
    va_copy(args_copy, args);
    needed = vsnprintf(NULL, 0, fmt, args_copy);
    va_end(args_copy);

    if (needed < 0) {
        va_end(args);
        return NULL;
    }

    buffer = (char*)malloc((size_t)needed + 1);
    if (buffer == NULL) {
        va_end(args);
        return NULL;
    }

    vsnprintf(buffer, (size_t)needed + 1, fmt, args);
    va_end(args);
    return buffer;
}

static const char* ErrorKindToCode(SemanticErrorKind kind) {
    switch (kind) {
        case SEMANTIC_ERROR_UNDEFINED_VARIABLE: return "E_UNDEFINED_VARIABLE";
        case SEMANTIC_ERROR_UNDEFINED_FUNCTION: return "E_UNDEFINED_FUNCTION";
        case SEMANTIC_ERROR_UNDEFINED_CLASS: return "E_UNDEFINED_CLASS";
        case SEMANTIC_ERROR_UNDEFINED_TYPE: return "E_UNDEFINED_TYPE";
        case SEMANTIC_ERROR_DUPLICATE_SYMBOL: return "E_DUPLICATE_SYMBOL";
        case SEMANTIC_ERROR_TYPE_MISMATCH: return "E_TYPE_MISMATCH";
        case SEMANTIC_ERROR_INVALID_OPERANDS: return "E_INVALID_OPERANDS";
        case SEMANTIC_ERROR_WRONG_ARG_COUNT: return "E_WRONG_ARG_COUNT";
        case SEMANTIC_ERROR_ARG_TYPE_MISMATCH: return "E_ARG_TYPE_MISMATCH";
        case SEMANTIC_ERROR_ACCESS_VIOLATION: return "E_ACCESS_VIOLATION";
        case SEMANTIC_ERROR_INVALID_ARRAY_INDEX: return "E_INVALID_ARRAY_INDEX";
        case SEMANTIC_ERROR_INVALID_ARRAY_SIZE: return "E_INVALID_ARRAY_SIZE";
        case SEMANTIC_ERROR_METHOD_NOT_FOUND: return "E_METHOD_NOT_FOUND";
        case SEMANTIC_ERROR_FIELD_NOT_FOUND: return "E_FIELD_NOT_FOUND";
        case SEMANTIC_ERROR_INVALID_BASE_CLASS: return "E_INVALID_BASE_CLASS";
        case SEMANTIC_ERROR_INVALID_OVERRIDE: return "E_INVALID_OVERRIDE";
        case SEMANTIC_ERROR_ENUM_DUPLICATE_ITEM: return "E_ENUM_DUPLICATE_ITEM";
        case SEMANTIC_ERROR_CONSTRUCTOR_RETURN_TYPE: return "E_CTOR_RETURN_TYPE";
        case SEMANTIC_ERROR_OUT_OF_SCOPE: return "E_OUT_OF_SCOPE";
        case SEMANTIC_ERROR_OTHER: return "E_OTHER";
        default: return "E_UNKNOWN";
    }
}

static SemanticError CreateBaseError(SemanticErrorKind kind, int line, int column) {
    SemanticError err;

    memset(&err, 0, sizeof(SemanticError));
    err.kind = kind;
    err.code = DuplicateString(ErrorKindToCode(kind));
    if (line > 0 || column > 0) {
        err.has_position = 1;
        err.position.line = line;
        err.position.column = column;
    }
    return err;
}

static void FreeErrorFields(SemanticError *error) {
    if (error == NULL) {
        return;
    }

    free(error->code);
    free(error->message);
    free(error->explanation);
    free(error->context.expected);
    free(error->context.actual);
    free(error->context.name);
    free(error->context.signature);
    free(error->context.details);
    free(error->source_text);
}

ErrorList* CreateErrorList(void) {
    ErrorList *list = (ErrorList*)malloc(sizeof(ErrorList));
    if (list == NULL) {
        return NULL;
    }

    list->count = 0;
    list->capacity = 10;
    list->errors = (SemanticError**)malloc(sizeof(SemanticError*) * (size_t)list->capacity);
    if (list->errors == NULL) {
        free(list);
        return NULL;
    }

    return list;
}

int AddError(ErrorList *error_list, const SemanticError *error) {
    SemanticError *stored;
    SemanticError **grown;

    if (error_list == NULL || error == NULL) {
        return 1;
    }

    if (error_list->count >= error_list->capacity) {
        int new_capacity = error_list->capacity * 2;
        grown = (SemanticError**)realloc(error_list->errors, sizeof(SemanticError*) * (size_t)new_capacity);
        if (grown == NULL) {
            return 1;
        }
        error_list->errors = grown;
        error_list->capacity = new_capacity;
    }

    stored = (SemanticError*)malloc(sizeof(SemanticError));
    if (stored == NULL) {
        return 1;
    }
    memset(stored, 0, sizeof(SemanticError));

    stored->kind = error->kind;
    stored->code = DuplicateString(error->code);
    stored->message = DuplicateString(error->message);
    stored->explanation = DuplicateString(error->explanation);
    stored->has_position = error->has_position;
    stored->position = error->position;
    stored->has_range = error->has_range;
    stored->range = error->range;
    stored->has_related_position = error->has_related_position;
    stored->related_position = error->related_position;
    stored->has_related_range = error->has_related_range;
    stored->related_range = error->related_range;
    stored->context.expected = DuplicateString(error->context.expected);
    stored->context.actual = DuplicateString(error->context.actual);
    stored->context.name = DuplicateString(error->context.name);
    stored->context.signature = DuplicateString(error->context.signature);
    stored->context.details = DuplicateString(error->context.details);
    stored->source_text = DuplicateString(error->source_text);

    if ((error->code && stored->code == NULL) ||
        (error->message && stored->message == NULL) ||
        (error->explanation && stored->explanation == NULL) ||
        (error->context.expected && stored->context.expected == NULL) ||
        (error->context.actual && stored->context.actual == NULL) ||
        (error->context.name && stored->context.name == NULL) ||
        (error->context.signature && stored->context.signature == NULL) ||
        (error->context.details && stored->context.details == NULL) ||
        (error->source_text && stored->source_text == NULL)) {
        FreeErrorFields(stored);
        free(stored);
        return 1;
    }

    error_list->errors[error_list->count] = stored;
    error_list->count += 1;
    return 0;
}

void DestroyErrorList(ErrorList *error_list) {
    int i;

    if (error_list == NULL) {
        return;
    }

    for (i = 0; i < error_list->count; i++) {
        SemanticError *err = error_list->errors[i];
        if (err == NULL) {
            continue;
        }
        FreeErrorFields(err);
        free(err);
    }

    free(error_list->errors);
    free(error_list);
}

SemanticError CreateUndefinedVariableError(const char *var_name, int line, int column) {
    SemanticError err = CreateBaseError(SEMANTIC_ERROR_UNDEFINED_VARIABLE, line, column);

    err.message = FormatString("Variable '%s' is not defined", var_name);
    err.explanation = DuplicateString("Declare the variable in the current or enclosing scope.");
    err.context.name = DuplicateString(var_name);
    return err;
}

SemanticError CreateUndefinedFunctionError(const char *func_name, int line, int column) {
    SemanticError err = CreateBaseError(SEMANTIC_ERROR_UNDEFINED_FUNCTION, line, column);

    err.message = FormatString("Function '%s' not found", func_name);
    err.explanation = DuplicateString("Check the function name and its declaration visibility.");
    err.context.name = DuplicateString(func_name);
    return err;
}

SemanticError CreateUndefinedClassError(const char *class_name, int line, int column) {
    SemanticError err = CreateBaseError(SEMANTIC_ERROR_UNDEFINED_CLASS, line, column);

    err.message = FormatString("Class '%s' not found", class_name);
    err.explanation = DuplicateString("Check the class name and its declaration order.");
    err.context.name = DuplicateString(class_name);
    return err;
}

SemanticError CreateUndefinedTypeError(const char *type_name, int line, int column) {
    SemanticError err = CreateBaseError(SEMANTIC_ERROR_UNDEFINED_TYPE, line, column);

    err.message = FormatString("Type '%s' not defined", type_name);
    err.explanation = DuplicateString("Check the type name or add its declaration.");
    err.context.name = DuplicateString(type_name);
    return err;
}

SemanticError CreateDuplicateSymbolError(const char *symbol_name, int line, int column) {
    SemanticError err = CreateBaseError(SEMANTIC_ERROR_DUPLICATE_SYMBOL, line, column);

    err.message = FormatString("Symbol '%s' already defined", symbol_name);
    err.explanation = DuplicateString("Rename the symbol or remove the duplicate declaration.");
    err.context.name = DuplicateString(symbol_name);
    return err;
}

SemanticError CreateTypeMismatchError(const char *expected_type, const char *actual_type,
                                      const char *context, int line, int column) {
    SemanticError err = CreateBaseError(SEMANTIC_ERROR_TYPE_MISMATCH, line, column);
    const char *ctx = (context != NULL && context[0] != '\0') ? context : "";

    err.message = FormatString("Type mismatch: expected '%s' but got '%s' %s",
                               expected_type, actual_type, ctx);
    err.explanation = DuplicateString("Ensure types match or insert an explicit cast.");
    err.context.expected = DuplicateString(expected_type);
    err.context.actual = DuplicateString(actual_type);
    err.context.details = DuplicateString(ctx);
    return err;
}

SemanticError CreateInvalidOperandsError(const char *op_name, const char *operand1_type,
                                        const char *operand2_type, int line, int column) {
    SemanticError err = CreateBaseError(SEMANTIC_ERROR_INVALID_OPERANDS, line, column);

    if (operand2_type == NULL) {
        err.message = FormatString("Invalid operand for operator '%s': %s", op_name, operand1_type);
    } else {
        err.message = FormatString("Invalid operands for operator '%s': %s and %s",
                                   op_name, operand1_type, operand2_type);
    }

    err.explanation = DuplicateString("Check operand types for this operator.");
    err.context.name = DuplicateString(op_name);
    err.context.expected = DuplicateString(operand1_type);
    err.context.actual = DuplicateString(operand2_type);
    return err;
}

SemanticError CreateWrongArgCountError(const char *func_name, int expected, int actual,
                                       int line, int column) {
    SemanticError err = CreateBaseError(SEMANTIC_ERROR_WRONG_ARG_COUNT, line, column);

    err.message = FormatString("Function '%s' expects %d arguments but got %d",
                               func_name, expected, actual);
    err.explanation = DuplicateString("Check the number of arguments in the call.");
    err.context.name = DuplicateString(func_name);
    err.context.expected = FormatString("%d", expected);
    err.context.actual = FormatString("%d", actual);
    return err;
}

SemanticError CreateAccessViolationError(const char *member_name, const char *access_spec,
                                        int line, int column) {
    SemanticError err = CreateBaseError(SEMANTIC_ERROR_ACCESS_VIOLATION, line, column);

    err.message = FormatString("Cannot access %s member '%s'", access_spec, member_name);
    err.explanation = DuplicateString("Check access modifiers and access context.");
    err.context.name = DuplicateString(member_name);
    err.context.details = DuplicateString(access_spec);
    return err;
}

SemanticError CreateInvalidArrayIndexError(const char *index_type, int line, int column) {
    SemanticError err = CreateBaseError(SEMANTIC_ERROR_INVALID_ARRAY_INDEX, line, column);

    err.message = FormatString("Array index must be integer, got %s", index_type);
    err.explanation = DuplicateString("Use an integral type for array indexing.");
    err.context.actual = DuplicateString(index_type);
    return err;
}

SemanticError CreateInvalidArraySizeError(const char *reason, int line, int column) {
    SemanticError err = CreateBaseError(SEMANTIC_ERROR_INVALID_ARRAY_SIZE, line, column);

    err.message = FormatString("Invalid array size: %s", reason);
    err.explanation = DuplicateString("Array size must be a valid non-negative integer.");
    err.context.details = DuplicateString(reason);
    return err;
}

SemanticError CreateMethodNotFoundError(const char *method_name, const char *class_name,
                                       int line, int column) {
    SemanticError err = CreateBaseError(SEMANTIC_ERROR_METHOD_NOT_FOUND, line, column);

    err.message = FormatString("Method '%s' not found in class '%s'", method_name, class_name);
    err.explanation = DuplicateString("Check method name, parameters, and class definition.");
    err.context.name = DuplicateString(method_name);
    err.context.signature = DuplicateString(class_name);
    return err;
}

SemanticError CreateFieldNotFoundError(const char *field_name, const char *class_name,
                                      int line, int column) {
    SemanticError err = CreateBaseError(SEMANTIC_ERROR_FIELD_NOT_FOUND, line, column);

    err.message = FormatString("Field '%s' not found in class '%s'", field_name, class_name);
    err.explanation = DuplicateString("Check field name and class definition.");
    err.context.name = DuplicateString(field_name);
    err.context.signature = DuplicateString(class_name);
    return err;
}

SemanticError CreateInvalidBaseClassError(const char *base_class_name, int line, int column) {
    SemanticError err = CreateBaseError(SEMANTIC_ERROR_INVALID_BASE_CLASS, line, column);

    err.message = FormatString("Base class '%s' not found", base_class_name);
    err.explanation = DuplicateString("Declare the base class or fix its name.");
    err.context.name = DuplicateString(base_class_name);
    return err;
}

SemanticError CreateInvalidOverrideError(const char *method_name, const char *reason,
                                        int line, int column) {
    SemanticError err = CreateBaseError(SEMANTIC_ERROR_INVALID_OVERRIDE, line, column);

    err.message = FormatString("Cannot override method '%s': %s", method_name, reason);
    err.explanation = DuplicateString("Ensure the base method exists and signatures match.");
    err.context.name = DuplicateString(method_name);
    err.context.details = DuplicateString(reason);
    return err;
}

SemanticError CreateEnumDuplicateItemError(const char *item_name, int line, int column) {
    SemanticError err = CreateBaseError(SEMANTIC_ERROR_ENUM_DUPLICATE_ITEM, line, column);

    err.message = FormatString("Enum item '%s' already defined", item_name);
    err.explanation = DuplicateString("Enum items must have unique names.");
    err.context.name = DuplicateString(item_name);
    return err;
}

SemanticError CreateConstructorReturnTypeError(int line, int column) {
    SemanticError err = CreateBaseError(SEMANTIC_ERROR_CONSTRUCTOR_RETURN_TYPE, line, column);

    err.message = DuplicateString("Constructor should not have return type");
    err.explanation = DuplicateString("Remove the return type from the constructor declaration.");
    return err;
}

SemanticError CreateOutOfScopeError(const char *symbol_name, int line, int column) {
    SemanticError err = CreateBaseError(SEMANTIC_ERROR_OUT_OF_SCOPE, line, column);

    err.message = FormatString("Symbol '%s' is out of scope", symbol_name);
    err.explanation = DuplicateString("Ensure the symbol is declared in the current or enclosing scope.");
    err.context.name = DuplicateString(symbol_name);
    return err;
}

SemanticError CreateCustomError(SemanticErrorKind kind, const char *message, int line, int column) {
    SemanticError err = CreateBaseError(kind, line, column);

    err.message = DuplicateString(message);
    return err;
}

void PrintError(const SemanticError *error) {
    const char *code;

    if (error == NULL) {
        return;
    }

    code = (error->code != NULL) ? error->code : ErrorKindToString(error->kind);

    if (error->has_range) {
        fprintf(stderr,
                "SEMANTIC ERROR [%s] at line %d, column %d - line %d, column %d: %s\n",
                code,
                error->range.start.line,
                error->range.start.column,
                error->range.end.line,
                error->range.end.column,
                error->message != NULL ? error->message : "Unknown error");
    } else if (error->has_position) {
        fprintf(stderr,
                "SEMANTIC ERROR [%s] at line %d, column %d: %s\n",
                code,
                error->position.line,
                error->position.column,
                error->message != NULL ? error->message : "Unknown error");
    } else {
        fprintf(stderr,
                "SEMANTIC ERROR [%s]: %s\n",
                code,
                error->message != NULL ? error->message : "Unknown error");
    }

    if (error->explanation != NULL) {
        fprintf(stderr, "  Explanation: %s\n", error->explanation);
    }

    if (error->context.expected || error->context.actual || error->context.name ||
        error->context.signature || error->context.details) {
        fprintf(stderr, "  Context:\n");
        if (error->context.expected) {
            fprintf(stderr, "    expected: %s\n", error->context.expected);
        }
        if (error->context.actual) {
            fprintf(stderr, "    actual: %s\n", error->context.actual);
        }
        if (error->context.name) {
            fprintf(stderr, "    name: %s\n", error->context.name);
        }
        if (error->context.signature) {
            fprintf(stderr, "    signature: %s\n", error->context.signature);
        }
        if (error->context.details) {
            fprintf(stderr, "    details: %s\n", error->context.details);
        }
    }

    if (error->has_related_range) {
        fprintf(stderr,
                "  Related: line %d, column %d - line %d, column %d\n",
                error->related_range.start.line,
                error->related_range.start.column,
                error->related_range.end.line,
                error->related_range.end.column);
    } else if (error->has_related_position) {
        fprintf(stderr,
                "  Related: line %d, column %d\n",
                error->related_position.line,
                error->related_position.column);
    }

    if (error->source_text != NULL) {
        fprintf(stderr, "  Source: %s\n", error->source_text);
    }
}

void PrintAllErrors(const ErrorList *error_list) {
    int i;

    if (error_list == NULL) {
        return;
    }

    if (error_list->count == 0) {
        fprintf(stderr, "No semantic errors found\n");
        return;
    }

    fprintf(stderr, "SEMANTIC ANALYSIS ERRORS:\n");
    for (i = 0; i < error_list->count; i++) {
        PrintError(error_list->errors[i]);
    }
    fprintf(stderr, "\nTotal: %d errors\n", error_list->count);
}

const char* ErrorKindToString(SemanticErrorKind kind) {
    switch (kind) {
        case SEMANTIC_ERROR_UNDEFINED_VARIABLE: return "Undefined variable";
        case SEMANTIC_ERROR_UNDEFINED_FUNCTION: return "Undefined function";
        case SEMANTIC_ERROR_UNDEFINED_CLASS: return "Undefined class";
        case SEMANTIC_ERROR_UNDEFINED_TYPE: return "Undefined type";
        case SEMANTIC_ERROR_DUPLICATE_SYMBOL: return "Duplicate symbol";
        case SEMANTIC_ERROR_TYPE_MISMATCH: return "Type mismatch";
        case SEMANTIC_ERROR_INVALID_OPERANDS: return "Invalid operands";
        case SEMANTIC_ERROR_WRONG_ARG_COUNT: return "Wrong argument count";
        case SEMANTIC_ERROR_ARG_TYPE_MISMATCH: return "Argument type mismatch";
        case SEMANTIC_ERROR_ACCESS_VIOLATION: return "Access violation";
        case SEMANTIC_ERROR_INVALID_ARRAY_INDEX: return "Invalid array index";
        case SEMANTIC_ERROR_INVALID_ARRAY_SIZE: return "Invalid array size";
        case SEMANTIC_ERROR_METHOD_NOT_FOUND: return "Method not found";
        case SEMANTIC_ERROR_FIELD_NOT_FOUND: return "Field not found";
        case SEMANTIC_ERROR_INVALID_BASE_CLASS: return "Invalid base class";
        case SEMANTIC_ERROR_INVALID_OVERRIDE: return "Invalid override";
        case SEMANTIC_ERROR_ENUM_DUPLICATE_ITEM: return "Enum duplicate item";
        case SEMANTIC_ERROR_CONSTRUCTOR_RETURN_TYPE: return "Constructor return type";
        case SEMANTIC_ERROR_OUT_OF_SCOPE: return "Out of scope";
        case SEMANTIC_ERROR_OTHER: return "Other error";
        default: return "Unknown error";
    }
}

int GetErrorCount(const ErrorList *error_list) {
    if (error_list == NULL) {
        return 0;
    }
    return error_list->count;
}

int HasErrors(const ErrorList *error_list) {
    if (error_list == NULL) {
        return 0;
    }
    return (error_list->count > 0) ? 1 : 0;
}
