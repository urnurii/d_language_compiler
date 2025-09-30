%option noyywrap
%option never-interactive

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int line = 1;
char tmp[8192];

char string_buf[8192];
int string_pos = 0;
%}

DIGIT [0-9]

%x COMMENT STRING MULTILINE_COMMENT

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
"enum"              { printf("Keyword: %s\n", yytext); }

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

(?:[1-9]{DIGIT}*)|0      { printf("Int: %d\n", atoi(yytext)); }
0[bB][01]+               { printf("Int: %s (binary)\n", yytext); }
0[0-7]+                  { printf("Int: %s (octal)\n", yytext); }
0[xX][0-9A-Fa-f]+        { printf("Int: %s (hex)\n", yytext); }
(?:[1-9][{DIGIT}_]*)|0   { printf("Int: %s (with separators)\n", yytext); }

[+-]?{DIGIT}+([.]{DIGIT}*)?([eE][+-]?{DIGIT}+)?[fF]? { printf("Float: %s\n", yytext); }


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
"."                 { printf("Operator: %s\n", yytext); }

"("                 { printf("Operator: %s\n", yytext); }
")"                 { printf("Operator: %s\n", yytext); }
"{"                 { printf("Operator: %s\n", yytext); }
"}"                 { printf("Operator: %s\n", yytext); }
"["                 { printf("Operator: %s\n", yytext); }
"]"                 { printf("Operator: %s\n", yytext); }
";"                 { printf("Operator: %s\n", yytext); }
","                 { printf("Operator: %s\n", yytext); }
":"                 { printf("Operator: %s\n", yytext); }

"//"                { tmp[0] = '\0'; BEGIN(COMMENT); }
<COMMENT>.          { strcat(tmp, yytext); }
<COMMENT>\n         { printf("Found comment: %s\n", tmp); line++; BEGIN(INITIAL); }

"/*" { tmp[0] = '\0'; BEGIN(MULTILINE_COMMENT); }
<MULTILINE_COMMENT>[^*\n]+ { strcat(tmp, yytext); }
<MULTILINE_COMMENT>[^*\n]*\n { line++; strcat(tmp, yytext); }
<MULTILINE_COMMENT>"*"+[^*/] { strcat(tmp, yytext); }
<MULTILINE_COMMENT>"*"+\n { line++; strcat(tmp, yytext); }
<MULTILINE_COMMENT>"*"+"/" { printf("Found multiline comment: %s\n", tmp); BEGIN(INITIAL); }

<MULTILINE_COMMENT><<EOF>> { printf("ERROR! Multiline comment \"%s\" isn't closed in row: %d\n", tmp, line); BEGIN(INITIAL); }

[A-Za-z_][A-Za-z0-9_]*     { printf("Found identifier: %s\n", yytext); }

%%

int yywrap(void) {
    return 1;
}

int main(int argc, char* argv[])
{
    if ( argc > 1 )
            yyin = fopen( argv[1], "r" );
    else
            yyin = stdin;

    yylex();
}
