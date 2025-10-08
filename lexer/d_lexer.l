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

%x COMMENT STRING CHAR MULTILINE_COMMENT

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

"null"              { printf("Null: null\n"); }
"NaN"               { printf("NaN: NaN\n"); }

"true"              { printf("Bool: true\n"); }
"false"             { printf("Bool: false\n"); }

0|(?:[1-9]{DIGIT}*(?:_{DIGIT}+)*)  { printf("Int: %d\n", atoi(yytext)); }
0[bB][01]+                         { printf("Int: %s (binary)\n", yytext); }
0[0-7]+                            { printf("Int: %s (octal)\n", yytext); }
0[0-9]+                            { printf("ERROR! Incorrect octal number: %s in row: %d\n", yytext, line); }
0[xX][0-9A-Fa-f]+                  { printf("Int: %s (hex)\n", yytext); }

{DIGIT}+([.]{DIGIT}*)?([eE][+-]?{DIGIT}+)?[fF]? { printf("Float: %s\n", yytext); }


\' strcpy(tmp,""); BEGIN(CHAR);
\" strcpy(tmp,""); BEGIN(STRING);
<STRING>[^\\\n\"]+ strcat(tmp,yytext);
<STRING> %d { printf("Format specifier: %s\n", yytext); strcat(tmp,yytext) }
<STRING> %f { printf("Format specifier: %s\n", yytext); strcat(tmp,yytext) }
<STRING> %s { printf("Format specifier: %s\n", yytext); strcat(tmp,yytext) }
<STRING> %c { printf("Format specifier: %s\n", yytext); strcat(tmp,yytext) }
<CHAR>[^\\\n\'] strcat(tmp,yytext);
<STRING,CHAR>\\b strcat(tmp,"\b");
<STRING,CHAR>\\f strcat(tmp,"\f");
<STRING,CHAR>\\n strcat(tmp,"\n");
<STRING,CHAR>\\r strcat(tmp,"\r");
<STRING,CHAR>\\t strcat(tmp,"\t");
<STRING,CHAR>\\v strcat(tmp,"\v");
<STRING,CHAR>\\\' strcat(tmp,"\'");
<STRING,CHAR>\\\" strcat(tmp,"\"");
<STRING,CHAR>\\\\ strcat(tmp,"\\");
<STRING,CHAR>\\\? strcat(tmp,"\?");
<STRING,CHAR>\\x[0-9a-fA-F]{1,2} {
    char dig[2];
    char *a;
    dig[0]=strtol(yytext+2, &a, 16);
    dig[1]='\0';
    strcat(tmp, dig);}
<STRING,CHAR>\\[0-7]{1,3} {
    char dig[2];
    char *a;
    dig[0]=strtol(yytext+1, &a, 8);
    dig[1]='\0';
strcat(tmp, dig);}
<STRING,CHAR>\n { line++; }
<STRING,CHAR><<EOF>> { printf("ERROR! haven't found closing quotes in row: %d\n", line); BEGIN(INITIAL); }
<STRING>\" { printf("Found String literal: %s\n",tmp); BEGIN(INITIAL); }
<CHAR>\' {
    if(strlen(tmp) != 1)
    {
        printf("ERROR! char length != 1 in row: %d\n", line); BEGIN(INITIAL);
    }
    else
    {
        printf("Found Char literal: %s\n",tmp); BEGIN(INITIAL);
    }
}

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

"("                 { printf("Separator: %s\n", yytext); }
")"                 { printf("Separator: %s\n", yytext); }
"{"                 { printf("Operator: %s\n", yytext); }
"}"                 { printf("Operator: %s\n", yytext); }
"["                 { printf("Operator: %s\n", yytext); }
"]"                 { printf("Operator: %s\n", yytext); }
";"                 { printf("Separator: %s\n", yytext); }
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

.                          { printf("LEXICAL ERROR: Unknown character '%s' at line %d\n", yytext, line); }

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
