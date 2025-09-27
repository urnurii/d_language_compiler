%option noyywrap
%option never-interactive

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int line = 1;
char string_buf[8192];
int string_pos = 0;
int comment_depth = 0;
char comment_buf[8192];
int comment_pos = 0;
%}

%x COMMENT STRING

%%

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

"read"              { printf("Keyword: %s\n", yytext); }
"readf"             { printf("Keyword: %s\n", yytext); }
"readln"            { printf("Keyword: %s\n", yytext); }
"write"             { printf("Keyword: %s\n", yytext); }
"writef"            { printf("Keyword: %s\n", yytext); }
"writeln"           { printf("Keyword: %s\n", yytext); }

"null"              { printf("Null: null\n"); }
"NaN"               { printf("NaN: NaN\n"); }

"true"              { printf("Bool: true\n"); }
"false"             { printf("Bool: false\n"); }

(?:[1-9]\d*)|0      { printf("Int: %d\n", atoi(yytext)); }
"0b" [01]+          { printf("Int: %s (binary)\n", yytext); }
"0o" [0-7]+         { printf("Int: %s (octal)\n", yytext); }
"0x" [0-9A-Fa-f]+   { printf("Int: %s (hex)\n", yytext); }
(?:[1-9][\d_]*)|0   { printf("Int: %s (with separators)\n", yytext); }

"\""                { BEGIN(STRING); }
<STRING>[^\"\\\n]+  { printf("String part: %s\n", yytext); }
<STRING>\\n         { printf("String part: \\n\n"); }
<STRING>\\t         { printf("String part: \\t\n"); }
<STRING>\\b         { printf("String part: \\b\n"); }
<STRING>\\r         { printf("String part: \\r\n"); }
<STRING>\\\\        { printf("String part: \\\n"); }
<STRING>\\\"        { printf("String part: \\\"\n"); }
<STRING>\n          { printf("ERROR unterminated_string line %d\n", line); BEGIN(INITIAL); }
<STRING>\"          { printf("String: %s\n", yytext); BEGIN(INITIAL); }

[A-Z][A-Za-z0-9_]*  { printf("TypeID: %s\n", yytext); }
[a-z][A-Za-z0-9_]*  { printf("ObjectID: %s\n", yytext); }

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
"&&"                { printf("Operator: %s\n", yytext); }
"||"                { printf("Operator: %s\n", yytext); }
"!"                 { printf("Operator: %s\n", yytext); }
".."                { printf("Operator: %s\n", yytext); }

"("                 { printf("Operator: %s\n", yytext); }
")"                 { printf("Operator: %s\n", yytext); }
"{"                 { printf("Operator: %s\n", yytext); }
"}"                 { printf("Operator: %s\n", yytext); }
"["                 { printf("Operator: %s\n", yytext); }
"]"                 { printf("Operator: %s\n", yytext); }
";"                 { printf("Operator: %s\n", yytext); }
","                 { printf("Operator: %s\n", yytext); }
":"                 { printf("Operator: %s\n", yytext); }

"//"                { BEGIN(COMMENT); }
<COMMENT>[^*]*      { /* Вложенные комментарии на одну строку игнорируются */ }
<COMMENT>\n         { line++; BEGIN(INITIAL); }
<COMMENT>.          { /* Игнорируем другие символы внутри комментария */ }

.                   { printf("ERROR unexpected_symbol %s line %d\n", yytext, line); }

%%

int yywrap(void) {
    return 1;
}

int main(void) {
    yylex();
    return 0;
}
