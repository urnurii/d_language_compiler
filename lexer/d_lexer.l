%option noyywrap
%option never-interactive

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void handle_hex_escape( char* buffer, char* yytext );
void handle_octal_escape( char* buffer, char* yytext );
void finish_char(char* buffer);
void strip_underscores(char* buf, const char* src);
int get_int(const char* yytext);
float get_float(const chat* yytext);
%}

DIGIT [0-9]

%x COMMENT STRING CHAR MULTILINE_COMMENT

%%

%{
    int line = 1;
    char tmp[8192];
    char string_buf[8192];
    int string_pos = 0;
    int comment_pos = 0;
    int value_int;
    float value_float;
%}

[ \t\r]+            ;
\n                  { line++; }

"int"               { printf("Keyword: %s\n", yytext); }
"char"              { printf("Keyword: %s\n", yytext); }
"string"            { printf("Keyword: %s\n", yytext); }
"bool"              { printf("Keyword: %s\n", yytext); }
"if"                { printf("Keyword: %s\n", yytext); }
"else"              { printf("Keyword: %s\n", yytext); }
"switch"            { printf("Keyword: %s\n", yytext); }
"case"              { printf("Keyword: %s\n", yytext); }
"default"           { printf("Keyword: %s\n", yytext); }
"do"                { printf("Keyword: %s\n", yytext); }
"while"             { printf("Keyword: %s\n", yytext); }
"for"               { printf("Keyword: %s\n", yytext); }
"foreach"           { printf("Keyword: %s\n", yytext); }
"return"            { printf("Keyword: %s\n", yytext); }
"void"              { printf("Keyword: %s\n", yytext); }
"new"               { printf("Keyword: %s\n", yytext); }
"continue"          { printf("Keyword: %s\n", yytext); }
"break"             { printf("Keyword: %s\n", yytext); }
"class"             { printf("Keyword: %s\n", yytext); }
"super"             { printf("Keyword: %s\n", yytext); }
"override"          { printf("Keyword: %s\n", yytext); }
"float"             { printf("Keyword: %s\n", yytext); }
"double"            { printf("Keyword: %s\n", yytext); }
"real"              { printf("Keyword: %s\n", yytext); }
"private"           { printf("Keyword: %s\n", yytext); }
"public"            { printf("Keyword: %s\n", yytext); }
"protected"         { printf("Keyword: %s\n", yytext); }
"auto"              { printf("Keyword: %s\n", yytext); }
"this"              { printf("Keyword: %s\n", yytext); }
"ref"               { printf("Keyword: %s\n", yytext); }
"enum"              { printf("Keyword: %s\n", yytext); }

"null"              { printf("Null: null\n"); }
"NaN"               { printf("NaN: NaN\n"); }

"true"              { printf("Bool: true\n"); }
"false"             { printf("Bool: false\n"); }

\'                               { strcpy(tmp,""); BEGIN(CHAR); }
\"                               { strcpy(tmp,""); string_pos = line; BEGIN(STRING); }
<STRING>[^\\\n\"]+               { strcat(tmp,yytext); }
<CHAR>[^\\\n\']                  { strcat(tmp,yytext); }
<STRING,CHAR>\\b                 { strcat(tmp,"\b"); }
<STRING,CHAR>\\f                 { strcat(tmp,"\f"); } 
<STRING,CHAR>\\n                 { strcat(tmp,"\n"); }
<STRING,CHAR>\\r                 { strcat(tmp,"\r"); }
<STRING,CHAR>\\t                 { strcat(tmp,"\t"); }
<STRING,CHAR>\\v                 { strcat(tmp,"\v"); }
<STRING,CHAR>\\\'                { strcat(tmp,"\'"); }
<STRING,CHAR>\\\"                { strcat(tmp,"\""); }
<STRING,CHAR>\\\\                { strcat(tmp,"\\"); }
<STRING,CHAR>\\\?                { strcat(tmp,"\?"); }
<STRING,CHAR>\\x[0-9a-fA-F]{1,2} { handle_hex_escape( tmp, yytext ); }
<STRING,CHAR>\\[0-7]{1,3}        { handle_octal_escape( char* tmp, char* yytext ) }
<STRING,CHAR>\n                  { line++; }
<STRING,CHAR><<EOF>>             { printf("ERROR! Haven't found closing quotes in string with position: %d\n", string_pos); BEGIN(INITIAL); }
<STRING>\"                       { printf("Found String literal: %s\n",tmp); BEGIN(INITIAL); }
<CHAR>\'                         { finish_char(tmp); BEGIN(INITIAL); }

"=="                { printf("Operator: %s\n", yytext); }
"!="                { printf("Operator: %s\n", yytext); }
"<"                 { printf("Operator: %s\n", yytext); }
"<="                { printf("Operator: %s\n", yytext); }
">"                 { printf("Operator: %s\n", yytext); }
">="                { printf("Operator: %s\n", yytext); }
"+"                 { printf("Operator: %s\n", yytext); }
"-"                 { printf("Operator: %s\n", yytext); }
"*"                 { printf("Operator: %s\n", yytext); }
"/"                 { printf("Operator: %s\n", yytext); }
"="                 { printf("Operator: %s\n", yytext); }
"+="                { printf("Operator: %s\n", yytext); }
"-="                { printf("Operator: %s\n", yytext); }
"*="                { printf("Operator: %s\n", yytext); }
"/="                { printf("Operator: %s\n", yytext); }
"~="                { printf("Operator: %s\n", yytext); }
"&&"                { printf("Operator: %s\n", yytext); }
"||"                { printf("Operator: %s\n", yytext); }
"!"                 { printf("Operator: %s\n", yytext); }
"..."               { printf("Operator: %s\n", yytext); }
".."                { printf("Operator: %s\n", yytext); }
"."                 { printf("Operator: %s\n", yytext); }
"~"                 { printf("Operator: %s\n", yytext); }

"("                 { printf("Separator: %s\n", yytext); }
")"                 { printf("Separator: %s\n", yytext); }
"{"                 { printf("Separator: %s\n", yytext); }
"}"                 { printf("Separator: %s\n", yytext); }
"["                 { printf("Operator: %s\n", yytext); }
"]"                 { printf("Operator: %s\n", yytext); }
";"                 { printf("Separator: %s\n", yytext); }
","                 { printf("Separator: %s\n", yytext); }
":"                 { printf("Operator: %s\n", yytext); }

0|(?:[1-9]{DIGIT}*(?:_{DIGIT}+)*)                { value_int = get_int(yytext); printf("Int: %d\n", value_int); }
0[bB][01]+                                                                  { printf("Int: %s (binary)\n", yytext); }
0[bB][01]{1,4}(_[01]{4})*                                                   { printf("Int: %s (binary with separators)\n", yytext); }
0[0-7]+                                                                     { printf("Int: %s (octal)\n", yytext); }
0[xX][0-9a-fA-F]+                                                           { printf("Int: %s (hex)\n", yytext); }
0[xX][0-9a-fA-F]{1,4}(_[0-9a-fA-F]{4})*                                     { printf("Int: %s (hex with separators)\n", yytext); }
{DIGIT}+([.]{DIGIT}+)?([eE][+-]?{DIGIT}+)?[fF]?                                                                         { printf("Float: %s\n", yytext); value_float = get_float(yytext); printf("Parsed Float: %f\n", value_float); }
(?:{DIGIT}(?:_{DIGIT}|{DIGIT})*\.{DIGIT}(?:_{DIGIT}|{DIGIT})*)(?:[eE][+\-]?{DIGIT}(?:_{DIGIT}|{DIGIT})*)?(?:[fFlL])?    { printf("Float: %s\n", yytext); value_float = get_float(yytext); printf("Parsed Float: %f\n", value_float); }
(?:\.{DIGIT}(?:_{DIGIT}|{DIGIT})*)(?:[eE][+\-]?{DIGIT}(?:_{DIGIT}|{DIGIT})*)?(?:[fFlL])?                                { printf("Float: %s\n", yytext); value_float = get_float(yytext); printf("Parsed Float: %f\n", value_float); }
(?:{DIGIT}(?:_{DIGIT}|{DIGIT})*)(?:[eE][+\-]?{DIGIT}(?:_{DIGIT}|{DIGIT})*)(?:[fFlL])?                                   { printf("Float: %s\n", yytext); value_float = get_float(yytext); printf("Parsed Float: %f\n", value_float); }
(?:{DIGIT}(?:_{DIGIT}|{DIGIT})*)(?:[fFlL])                                                                              { printf("Float: %s\n", yytext); value_float = get_float(yytext); printf("Parsed Float: %f\n", value_float); }                                                                              { printf("Float: %s\n", yytext); }


"//"                { tmp[0] = '\0'; BEGIN(COMMENT); }
<COMMENT>.          { strcat(tmp, yytext); }
<COMMENT>\n         { printf("Found comment: %s\n", tmp); line++; BEGIN(INITIAL); }

"/*"                         { tmp[0] = '\0'; comment_pos = line; BEGIN(MULTILINE_COMMENT); }
<MULTILINE_COMMENT>[^*\n]+   { strcat(tmp, yytext); }
<MULTILINE_COMMENT>[^*\n]*\n { line++; strcat(tmp, yytext); }
<MULTILINE_COMMENT>"*"+[^*/] { strcat(tmp, yytext); }
<MULTILINE_COMMENT>"*"+\n    { line++; strcat(tmp, yytext); }
<MULTILINE_COMMENT>"*"+"/"   { printf("Found multiline comment: %s\n", tmp); BEGIN(INITIAL); }

<MULTILINE_COMMENT><<EOF>>   { printf("ERROR! Multiline comment with position %d: \n\"%s\" \nisn't closed!\n", tmp, line); BEGIN(INITIAL); }

[A-Za-z_][A-Za-z0-9_]*       { printf("Found identifier: %s\n", yytext); }

.                            { printf("LEXICAL ERROR: Unknown character '%s' at line %d\n", yytext, line); }

%%

void handle_hex_escape( char* buffer, char* yytext ) {
    char dig[2];
    char *a;
    dig[0] = (char)strtol(yytext+2, &a, 16);
    dig[1] = '\0';
    strcat(tmp, dig);
}

void handle_octal_escape( char* buffer, char* yytext ) {
    char dig[2];
    char *a;
    dig[0] = (char)strtol(yytext+1, &a, 8);
    dig[1] = '\0';
    strcat(tmp, dig);
}

void finish_char(char* buffer) {
    if(strlen(buffer) != 1) {
        printf("ERROR! char length != 1 in row: %d\n", line);
    } else {
        printf("Found Char literal: %s\n", buffer);
    }
}

void strip_underscores(char* buf, const char* src)
{
    char* d = buf;
    for (const char* s = src; *s; s++)
    {
        if (*s != '_')
            *d++ = *s;
    }
    *d = '\0';
}

int get_int(const char* yytext)
{
    char clean[64];
    strip_underscores(clean, yytext);
    return strtol(clean, NULL, 0);
}

float get_float(const char* yytext)
{
    char clean[64];
    strip_underscores(clean, yytext);
    return strtof(clean, NULL);
}

int main(int argc, char* argv[])
{
    if ( argc > 1 )
            yyin = fopen( argv[1], "r" );
    else
            yyin = stdin;

    yylex();
}