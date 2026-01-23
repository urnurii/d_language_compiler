%option noyywrap
%option never-interactive

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <float.h>
#include <errno.h>
#include <ctype.h>
#include "tree_nodes.h"
#include "d_parser.tab.h"

void add_class_name(const char* name);
int is_class_name(const char* name);
void reset_class_table();
void print_class_table();
void add_enum_name(const char* name);
int is_enum_name(const char* name);
void reset_enum_table();
void handle_hex_escape(char* buffer, char* yytext);
void handle_octal_escape(char* buffer, char* yytext);
void finish_char(char* buffer, int line);
void strip_underscores(char* buf, const char* src);
long parse_binary(const char* yytext);
long parse_octal(const char* yytext);
long parse_hexadecimal(const char* yytext);
long parse_decimal(const char* yytext);
int get_int(const char* yytext);
float get_float(const char* yytext);
void set_lexer_first_pass(void);
void set_lexer_initial(void);
%}

DIGIT [0-9]
IDENTIFIER [A-Za-z_][A-Za-z0-9_]*

%x FIRST_PASS
%x FP_COMMENT FP_STRING FP_CHAR FP_MULTILINE_COMMENT
%x COMMENT STR_LIT CHAR_LIT MULTILINE_COMMENT

%%

%{
    char tmp[8192];
    int string_pos = 0;
    int comment_pos = 0;
%}

<FIRST_PASS>"class"[ \t]+{IDENTIFIER}[ \t\n]*(\{|:) {
    char class_name[256];
    sscanf(yytext, "class %s", class_name);
    char* end = strchr(class_name, '{');
    if (!end) end = strchr(class_name, ':');
    if (end) *end = '\0';
    add_class_name(class_name);
}

<FIRST_PASS>"enum"[ \t]+{IDENTIFIER}[ \t\n]*(\{|:) {
    char enum_name[256];
    sscanf(yytext, "enum %s", enum_name);
    char* end = strchr(enum_name, '{');
    if (!end) end = strchr(enum_name, ':');
    if (end) *end = '\0';
    add_enum_name(enum_name);
}

<FIRST_PASS>"//"               { BEGIN(FP_COMMENT); }
<FP_COMMENT>.                  ;
<FP_COMMENT>\n                 { BEGIN(FIRST_PASS); }

<FIRST_PASS>"/*"               { BEGIN(FP_MULTILINE_COMMENT); }
<FP_MULTILINE_COMMENT>[^*\n]+  ;
<FP_MULTILINE_COMMENT>[^*\n]*\n ;
<FP_MULTILINE_COMMENT>"*"+\n   ;
<FP_MULTILINE_COMMENT>"*"+[^*/] ;
<FP_MULTILINE_COMMENT>"*"+"/"  { BEGIN(FIRST_PASS); }
<FP_MULTILINE_COMMENT><<EOF>>  { BEGIN(FIRST_PASS); return -1; }

<FIRST_PASS>\"                 { BEGIN(FP_STRING); }
<FP_STRING>[^\\\n\"]+          ;
<FP_STRING>\\[^\n]             ;
<FP_STRING>\n                  ;
<FP_STRING>\"                  { BEGIN(FIRST_PASS); }
<FP_STRING><<EOF>>             { BEGIN(FIRST_PASS); return -1; }

<FIRST_PASS>\'                 { BEGIN(FP_CHAR); }
<FP_CHAR>[^\\\n\']+            ;
<FP_CHAR>\\[^\n]               ;
<FP_CHAR>\n                    { BEGIN(FIRST_PASS); }
<FP_CHAR>\'                    { BEGIN(FIRST_PASS); }
<FP_CHAR><<EOF>>               { BEGIN(FIRST_PASS); return -1; }

<FIRST_PASS>[\n]                ;
<FIRST_PASS>[ \t\r]+            ;
<FIRST_PASS>.                   ;

[ \t\r]+            ;
\n                  { yylineno++; }

"int"               { return INT; }
"char"              { return CHAR; }
"string"            { return STRING; }
"bool"              { return BOOL; }
"if"                { return IF; }
"else"              { return ELSE; }
"switch"            { return SWITCH; }
"case"              { return CASE; }
"default"           { return DEFAULT; }
"do"                { return DO; }
"while"             { return WHILE; }
"for"               { return FOR; }
"foreach"           { return FOREACH; }
"return"            { return RETURN; }
"void"              { return VOID; }
"new"               { return NEW; }
"continue"          { return CONTINUE; }
"break"             { return BREAK; }
"class"             { return CLASS; }
"super"             { return SUPER; }
"override"          { return OVERRIDE; }
"float"             { return FLOAT_TYPE; }
"double"            { return DOUBLE; }
"real"              { return REAL; }
"private"           { return PRIVATE; }
"public"            { return PUBLIC; }
"protected"         { return PROTECTED; }
"this"              { return THIS; }
"ref"               { return REF; }
"enum"              { return ENUM; }

"null"              { return NULL_CONST; }
"NaN"               { return NAN_CONST; }

"true"              { return TRUE_VAL; }
"false"             { return FALSE_VAL; }

\'                               { strcpy(tmp,""); BEGIN(CHAR_LIT); }
\"                               { strcpy(tmp,""); string_pos = yylineno; BEGIN(STR_LIT); }

<STR_LIT>[^\\\n\"]+               { strcat(tmp,yytext); }
<CHAR_LIT>[^\\\n\']+                 { strcat(tmp,yytext); }

<STR_LIT,CHAR_LIT>\\b                 { strcat(tmp,"\b"); }
<STR_LIT,CHAR_LIT>\\f                 { strcat(tmp,"\f"); }
<STR_LIT,CHAR_LIT>\\n                 { strcat(tmp,"\n"); }
<STR_LIT,CHAR_LIT>\\r                 { strcat(tmp,"\r"); }
<STR_LIT,CHAR_LIT>\\t                 { strcat(tmp,"\t"); }
<STR_LIT,CHAR_LIT>\\v                 { strcat(tmp,"\v"); }
<STR_LIT,CHAR_LIT>\\\'                { strcat(tmp,"\'"); }
<STR_LIT,CHAR_LIT>\\\"                { strcat(tmp,"\""); }
<STR_LIT,CHAR_LIT>\\\\                { strcat(tmp,"\\"); }
<STR_LIT,CHAR_LIT>\\\?                { strcat(tmp,"\?"); }
<STR_LIT,CHAR_LIT>\\x[0-9a-fA-F]{1,2} { handle_hex_escape( tmp, yytext ); }
<STR_LIT,CHAR_LIT>\\[0-7]{1,3}        { handle_octal_escape( tmp, yytext ); }

<STR_LIT>\n                       { yylineno++; strcat(tmp,"\n"); }
<CHAR_LIT>\n                         { fprintf(stderr, "LEXER ERROR: Line break found in char literal at line %d\n", yylineno); yylineno++; }
<STR_LIT,CHAR_LIT><<EOF>>             { fprintf(stderr, "LEXER ERROR: String/char starting at line %d not closed\n", string_pos); BEGIN(INITIAL); return -1; }

<STR_LIT>\"                       { 
                                    yylval.string_val = (char*)malloc(strlen(tmp) + 1);
                                    if (yylval.string_val == NULL) {
                                        fprintf(stderr, "Error: Memory allocation failed\n");
                                        exit(1);
                                    }
                                    strcpy(yylval.string_val, tmp);
                                    BEGIN(INITIAL);
                                    return STR;
                                 }

<CHAR_LIT>\'                         {
                                    if(strlen(tmp) != 1) {
                                        fprintf(stderr, "LEXER ERROR: char length != 1 at line %d\n", yylineno);
                                        BEGIN(INITIAL);
                                        return -1;
                                    } else {
                                        yylval.char_val = tmp[0];
                                        BEGIN(INITIAL);
                                        return CHARVAL;
                                    }
                                 }

"=="                { return EQ; }
"!="                { return NEQ; }
"<="                { return LE; }
">="                { return GE; }
"+="                { return PLUSEQ; }
"-="                { return MINUSEQ; }
"*="                { return STAREQ; }
"/="                { return SLASHEQ; }
"~="                { return TILDEQ; }
"&&"                { return LAND; }
"||"                { return LOR; }
".."                { return DOTDOT; }

"<"                 { return LT; }
">"                 { return GT; }
"+"                 { return PLUS; }
"-"                 { return MINUS; }
"*"                 { return STAR; }
"/"                 { return SLASH; }
"="                 { return '='; }
"!"                 { return UNOT; }
"."                 { return '.'; }
"~"                 { return '~'; }

"("                 { return '('; }
")"                 { return ')'; }
"{"                 { return '{'; }
"}"                 { return '}'; }
"["                 { return '['; }
"]"                 { return ']'; }
";"                 { return ';'; }
","                 { return ','; }
":"                 { return ':'; }

0|(?:[1-9]{DIGIT}*(?:_{DIGIT}+)*)                                           { 
                                    yylval.int_val = get_int(yytext);
                                    return INTEGER;
                                 }

0[bB][01]+(_[01]+)*                                                         { 
                                    yylval.int_val = get_int(yytext);
                                    return INTEGER;
                                 }

0[0-7]+                                                                     { 
                                    yylval.int_val = get_int(yytext);
                                    return INTEGER;
                                 }

0[0-9]+                                                                     { 
                                    fprintf(stderr, "LEXER ERROR: Incorrect octal number: %s at line %d\n", yytext, yylineno);
                                    return -1;
                                 }

0[xX][0-9a-fA-F]+(_[0-9a-fA-F]+)*                                           { 
                                    yylval.int_val = get_int(yytext);
                                    return INTEGER;
                                 }

{DIGIT}+([.]{DIGIT}+)?([eE][+-]?{DIGIT}+)?[fF]?                             { 
                                    yylval.float_val = get_float(yytext);
                                    return FLOAT_VAL;
                                 }

(?:{DIGIT}(?:_{DIGIT}|{DIGIT})*\.{DIGIT}(?:_{DIGIT}|{DIGIT})*)(?:[eE][+\-]?{DIGIT}(?:_{DIGIT}|{DIGIT})*)?(?:[fFlL])?    { 
                                    yylval.float_val = get_float(yytext);
                                    return FLOAT_VAL;
                                 }

(?:\.{DIGIT}(?:_{DIGIT}|{DIGIT})*)(?:[eE][+\-]?{DIGIT}(?:_{DIGIT}|{DIGIT})*)?(?:[fFlL])?                                { 
                                    yylval.float_val = get_float(yytext);
                                    return FLOAT_VAL;
                                 }

(?:{DIGIT}(?:_{DIGIT}|{DIGIT})*)(?:[eE][+\-]?{DIGIT}(?:_{DIGIT}|{DIGIT})*)(?:[fFlL])?                                   { 
                                    yylval.float_val = get_float(yytext);
                                    return FLOAT_VAL;
                                 }

(?:{DIGIT}(?:_{DIGIT}|{DIGIT})*)(?:[fFlL])                                  { 
                                    yylval.float_val = get_float(yytext);
                                    return FLOAT_VAL;
                                 }

"//"                { tmp[0] = '\0'; BEGIN(COMMENT); }
<COMMENT>.          { strcat(tmp, yytext); }
<COMMENT>\n         { yylineno++; BEGIN(INITIAL); }

"/*"                         { tmp[0] = '\0'; comment_pos = yylineno; BEGIN(MULTILINE_COMMENT); }
<MULTILINE_COMMENT>[^*\n]+   { strcat(tmp, yytext); }
<MULTILINE_COMMENT>[^*\n]*\n { yylineno++; strcat(tmp, yytext); }
<MULTILINE_COMMENT>"*"+\n    { yylineno++; strcat(tmp, yytext); }
<MULTILINE_COMMENT>"*"+[^*/] { strcat(tmp, yytext); }
<MULTILINE_COMMENT>"*"+"/"   { BEGIN(INITIAL); }

<MULTILINE_COMMENT><<EOF>>   { fprintf(stderr, "LEXER ERROR: Multiline comment starting at %d not closed\n", comment_pos); BEGIN(INITIAL); return -1; }

{IDENTIFIER} {
    if (is_class_name(yytext)) {
        yylval.string_val = (char*)malloc(strlen(yytext) + 1);
        if (yylval.string_val == NULL) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            exit(1);
        }
        strcpy(yylval.string_val, yytext);
        return CLASSNAME;
    } else if (is_enum_name(yytext)) {
        yylval.string_val = (char*)malloc(strlen(yytext) + 1);
        if (yylval.string_val == NULL) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            exit(1);
        }
        strcpy(yylval.string_val, yytext);
        return ENUMNAME;
    } else {
        yylval.string_val = (char*)malloc(strlen(yytext) + 1);
        if (yylval.string_val == NULL) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            exit(1);
        }
        strcpy(yylval.string_val, yytext);
        return IDENT;
    }
}

.                   { fprintf(stderr, "LEXICAL ERROR: Unknown character '%s' at line %d\n", yytext, yylineno); return -1; }

%%

#define MAX_CLASSES 1000
#define MAX_CLASS_NAME 256

typedef struct {
    char names[MAX_CLASSES][MAX_CLASS_NAME];
    int count;
} ClassTable;

ClassTable classTable = { .count = 0 };

typedef struct {
    char names[MAX_CLASSES][MAX_CLASS_NAME];
    int count;
} EnumTable;

EnumTable enumTable = { .count = 0 };

void add_class_name(const char* name) {
    if (classTable.count < MAX_CLASSES) {
        strncpy(classTable.names[classTable.count], name, MAX_CLASS_NAME - 1);
        classTable.names[classTable.count][MAX_CLASS_NAME - 1] = '\0';
        classTable.count++;
    }
}

int is_class_name(const char* name) {
    for (int i = 0; i < classTable.count; i++) {
        if (strcmp(classTable.names[i], name) == 0) {
            return 1;
        }
    }
    return 0;
}

void reset_class_table() {
    classTable.count = 0;
}

void print_class_table() {
    printf("\n----- FOUND CLASSES (%d total) -----\n", classTable.count);
    for (int i = 0; i < classTable.count; i++) {
        printf("  %d. %s\n", i + 1, classTable.names[i]);
    }
    printf("-----------------------------------\n\n");
}

void add_enum_name(const char* name) {
    if (enumTable.count < MAX_CLASSES) {
        strncpy(enumTable.names[enumTable.count], name, MAX_CLASS_NAME - 1);
        enumTable.names[enumTable.count][MAX_CLASS_NAME - 1] = '\0';
        enumTable.count++;
    }
}

int is_enum_name(const char* name) {
    for (int i = 0; i < enumTable.count; i++) {
        if (strcmp(enumTable.names[i], name) == 0) {
            return 1;
        }
    }
    return 0;
}

void reset_enum_table() {
    enumTable.count = 0;
}

void set_lexer_first_pass(void) {
    BEGIN(FIRST_PASS);
}

void set_lexer_initial(void) {
    BEGIN(INITIAL);
}

void handle_hex_escape(char* buffer, char* yytext) {
    char dig[2];
    char *a;
    dig[0] = (char)strtol(yytext+2, &a, 16);
    dig[1] = '\0';
    strcat(buffer, dig);
}

void handle_octal_escape(char* buffer, char* yytext) {
    char dig[2];
    char *a;
    dig[0] = (char)strtol(yytext+1, &a, 8);
    dig[1] = '\0';
    strcat(buffer, dig);
}

void strip_underscores(char* buf, const char* src) {
    char* d = buf;
    for (const char* s = src; *s; s++) {
        if (*s != '_')
            *d++ = *s;
    }
    *d = '\0';
}

long parse_binary(const char* yytext) {
    for (int i = 2; yytext[i] != '\0'; i++) {
        if (yytext[i] != '0' && yytext[i] != '1' && yytext[i] != '_') {
            fprintf(stderr, "Error: Invalid binary digit '%c' in '%s'\n", yytext[i], yytext);
            return 0;
        }
    }
    char clean[64];
    strip_underscores(clean, yytext);
    return strtol(clean + 2, NULL, 2);
}

long parse_octal(const char* yytext) {
    for (int i = 1; yytext[i] != '\0'; i++) {
        if ((yytext[i] < '0' || yytext[i] > '7') && yytext[i] != '_') {
            fprintf(stderr, "Error: Invalid octal digit '%c' in '%s'\n", yytext[i], yytext);
            return 0;
        }
    }
    char clean[64];
    strip_underscores(clean, yytext);
    return strtol(clean, NULL, 8);
}

long parse_hexadecimal(const char* yytext) {
    for (int i = 2; yytext[i] != '\0'; i++) {
        if (!isxdigit(yytext[i]) && yytext[i] != '_') {
            fprintf(stderr, "Error: Invalid hex digit '%c' in '%s'\n", yytext[i], yytext);
            return 0;
        }
    }
    char clean[64];
    strip_underscores(clean, yytext);
    return strtol(clean + 2, NULL, 16);
}

long parse_decimal(const char* yytext) {
    for (int i = 0; yytext[i] != '\0'; i++) {
        if (!isdigit(yytext[i]) && yytext[i] != '_') {
            fprintf(stderr, "Error: Invalid digit '%c' in '%s'\n", yytext[i], yytext);
            return 0;
        }
    }
    char clean[64];
    strip_underscores(clean, yytext);
    return strtol(clean, NULL, 10);
}

int get_int(const char* yytext) {
    long val;

    if (strncmp(yytext, "0b", 2) == 0 || strncmp(yytext, "0B", 2) == 0) {
        val = parse_binary(yytext);
    }
    else if (strncmp(yytext, "0x", 2) == 0 || strncmp(yytext, "0X", 2) == 0) {
        val = parse_hexadecimal(yytext);
    }
    else if (yytext[0] == '0' && strlen(yytext) > 1) {
        val = parse_octal(yytext);
    }
    else {
        val = parse_decimal(yytext);
    }

    if (val > INT_MAX) {
        fprintf(stderr, "Warning: Number exceeds int range at line %d, truncated to INT_MAX.\n", yylineno);
        return INT_MAX;
    }

    return (int)val;
}

float get_float(const char* yytext) {
    char clean[64];
    strip_underscores(clean, yytext);
    errno = 0;
    char* endptr;
    float val = strtof(clean, &endptr);

    if ((errno == ERANGE && val == HUGE_VALF) || val > FLT_MAX) {
        fprintf(stderr, "Warning: Float value exceeds range at line %d, truncated to FLT_MAX.\n", yylineno);
        return FLT_MAX;
    }

    return val;
}
