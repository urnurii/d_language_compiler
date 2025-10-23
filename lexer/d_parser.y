%{ // Пролог
#include <iostream>
#include <cstdlib>
#include <cstdio>

using namespace std;

extern int yylex(void);

void yyerror(char const* s) {
    cout << s << endl;
}

%}
// Секция объявлений

%union {
    int int_lit;
    char *identifier;
    char *str_lit;
    float float_lit;
    bool bool_lit;
}

%token  MODULE
%token  IMPORT
%token  CONST
%token  FUNC
%token  VAR
%token  RETURN
%token  BREAK
%token  CONTINUE
%token  IF
%token  ELSE
%token  SWITCH
%token  CASE
%token  DEFAULT
%token  DO
%token  WHILE
%token  FOR
%token  FOREACH
%token  CLASS
%token  ENUM
%token  NEW
%token  THIS
%token  SUPER
%token  OVERRIDE
%token  PUBLIC
%token  PRIVATE
%token  PROTECTED
%token  AUTO
%token  REF

%token  INT
%token  FLOAT
%token  DOUBLE
%token  REAL
%token  BOOL
%token  CHAR
%token  STRING
%token  VOID

%token  <int_lit>      INT_LIT
%token  <float_lit>    FLOAT_LIT
%token  <bool_lit>     BOOL_LIT
%token  <identifier>   ID
%token  <str_lit>      STRING_LIT

%right  '=' '+=' '-=' '*=' '/=' '~='
%left   OR_OP
%left   AND_OP
%left   EQ NE '<' LE '>' GE
%left   '+' '-'
%left   '*' '/'
%right  INC DEC '!' UMINUS

%start program

%%

program                : module_clause ';' e_import_decl_list ';' e_top_level_decl_list
                      ;

module_clause          : MODULE ID
                      ;

e_import_decl_list     : import_decl_list
                      |
                      ;

import_decl_list       : import_decl_list import_decl ';'
                      | import_decl ';'
                      ;

import_decl            : IMPORT import_spec
                      | IMPORT '(' e_import_spec_list ')'
                      ;

e_import_spec_list     : import_spec_list
                      |
                      ;

import_spec_list       : import_spec_list import_spec ';'
                      | import_spec ';'
                      ;

import_spec            : STRING_LIT
                      | '.' STRING_LIT
                      | STRING_LIT STRING_LIT
                      ;

e_top_level_decl_list  : top_level_decl_list
                      |
                      ;

top_level_decl_list    : top_level_decl_list top_level_decl ';'
                      | top_level_decl ';'
                      ;

top_level_decl         : decl
                      | func_decl
                      | class_decl
                      | enum_decl
                      ;

decl                   : const_decl
                      | var_decl
                      ;

func_decl              : FUNC ID signature
                      | FUNC ID signature block
                      ;

signature              : params results
                      | params
                      ;

params                 : '(' param_list ')'
                      ;

param_list             : param_list ',' param_decl
                      | param_decl
                      ;

param_decl             : id_list type
                      | type
                      ;

results                : params
                      | type
                      ;

block                  : '{' stmt_list '}'
                      ;

stmt_list              : stmt_list stmt ';'
                      | stmt ';'
                      ;

stmt                   : decl
                      | simple_stmt
                      | return_stmt
                      | BREAK
                      | CONTINUE
                      | block
                      | if_stmt
                      | switch_stmt
                      | for_stmt
                      | while_stmt
                      | do_while_stmt
                      | foreach_stmt
                      ;

simple_stmt            : expr
                      | expr INC
                      | expr DEC
                      | expr_list '=' expr_list
                      | id_list '+=' expr_list
                      | id_list '-=' expr_list
                      | id_list '*=' expr_list
                      | id_list '/=' expr_list
                      | id_list '~=' expr_list
                      ;

return_stmt            : RETURN
                      | RETURN expr_list
                      ;

const_decl             : CONST const_spec
                      | '(' const_spec_list ')'
                      ;

const_spec_list        : const_spec_list const_spec ';'
                      | const_spec ';'
                      ;

const_spec             : id_list
                      | id_list '=' expr_list
                      | id_list type '=' expr_list
                      ;

var_decl               : VAR var_spec
                      | VAR '(' var_spec_list ')'
                      ;

var_spec_list          : var_spec_list ';' var_spec
                      | var_spec ';'
                      ;

var_spec               : id_list type
                      | id_list type '=' expr_list
                      | id_list '=' expr_list
                      ;

id_list                : id_list ',' ID
                      | ID
                      ;

type                   : type_name
                      | type_name '[' type_list ']'
                      | type_lit
                      ;

type_name              : INT
                      | FLOAT
                      | DOUBLE
                      | REAL
                      | BOOL
                      | CHAR
                      | STRING
                      | VOID
                      | ID
                      ;

type_list              : type_list ',' type
                      | type
                      ;

type_lit               : array_type
                      | func_type
                      | slice_type
                      ;

array_type             : '[' expr ']' type
                      ;

func_type              : FUNC signature
                      ;

slice_type             : '[' ']' type
                      ;

expr_list              : expr_list ',' expr
                      | expr
                      ;

expr                   : primary_expr
                      | INT_LIT
                      | FLOAT_LIT
                      | STRING_LIT
                      | BOOL_LIT
                      | expr '+' expr
                      | expr '-' expr
                      | expr '*' expr
                      | expr '/' expr
                      | expr EQ expr
                      | expr NE expr
                      | expr '<' expr
                      | expr '>' expr
                      | expr LE expr
                      | expr GE expr
                      | expr AND_OP expr
                      | expr OR_OP expr
                      | '!' expr
                      | '-' expr %prec UMINUS
                      ;

primary_expr           : operand
                      | primary_expr '[' expr ']'
                      | primary_expr '[' ':' ']'
                      | primary_expr '[' expr ':' ']'
                      | primary_expr '[' ':' expr ']'
                      | primary_expr '[' expr ':' expr ']'
                      | primary_expr '[' ':' expr ':' expr ']'
                      | primary_expr '[' expr ':' expr ':' expr ']'
                      ;

operand                : operand_name
                      | operand_name '[' type_list ']'
                      | '(' expr ')'
                      ;

operand_name           : ID
                      | THIS
                      | SUPER
                      ;

class_decl             : CLASS ID class_inherit_opt '{' class_body_opt '}'
                      ;
class_inherit_opt      : ':' ID
                      |
                      ;
class_body_opt         : class_body
                      |
                      ;
class_body             : class_body class_member
                      | class_member
                      ;
class_member           : decl ';'
                      | func_decl
                      ;

enum_decl              : ENUM ID '{' enum_member_list '}'
                      ;
enum_member_list       : enum_member_list ',' enum_member
                      | enum_member
                      ;
enum_member            : ID
                      | ID '=' expr
                      ;

while_stmt             : WHILE '(' expr ')' block
                      ;
do_while_stmt          : DO block WHILE '(' expr ')' ';'
                      ;
foreach_stmt           : FOREACH '(' ID ':' expr ')' block
                      ;

%%
// Секция пользовательского кода
