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


// Ключевые слова языка
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


// Типы данных
%token  INT
%token  FLOAT
%token  DOUBLE
%token  REAL
%token  BOOL
%token  CHAR
%token  STRING
%token  VOID


// Токены с семантическими значениями
%token  <int_lit>      INT_LIT
%token  <float_lit>    FLOAT_LIT
%token  <bool_lit>     BOOL_LIT
%token  <identifier>   ID
%token  <str_lit>      STRING_LIT


// Операторы (многосимвольные - как отдельные токены)
%token  PLUS_ASSIGN
%token  MINUS_ASSIGN
%token  MUL_ASSIGN
%token  DIV_ASSIGN
%token  TILDE_ASSIGN
%token  EQ
%token  NE
%token  LE
%token  GE
%token  AND_OP
%token  OR_OP
%token  INC
%token  DEC


%start program


%%


// Секция правил грамматики


program
    : module_clause ';' e_import_decl_list ';' e_top_level_decl_list
    ;


module_clause
    : MODULE ID
    ;


e_import_decl_list
    : import_decl_list
    |
    ;


import_decl_list
    : import_decl_list import_decl ';'
    | import_decl ';'
    ;


import_decl
    : IMPORT import_spec
    | IMPORT '(' e_import_spec_list ')'
    ;


e_import_spec_list
    : import_spec_list
    |
    ;


import_spec_list
    : import_spec_list import_spec ';'
    | import_spec ';'
    ;


import_spec
    : STRING_LIT
    | '.' STRING_LIT
    | STRING_LIT STRING_LIT
    ;


e_top_level_decl_list
    : top_level_decl_list
    |
    ;


top_level_decl_list
    : top_level_decl_list top_level_decl ';'
    | top_level_decl ';'
    ;


top_level_decl
    : decl
    | func_decl
    | class_decl
    | enum_decl
    ;


decl
    : const_decl
    | var_decl
    ;


func_decl
    : FUNC ID '(' param_list ')' block
    | FUNC ID '(' ')' block
    | FUNC ID '(' param_list ')' type block
    | FUNC ID '(' ')' type block
    ;


param_list
    : param_list ',' param_decl
    | param_decl
    ;


param_decl
    : id_list type_or_builtin
    | type_or_builtin
    ;


type_or_builtin
    : type_name
    | type_name '[' type_list ']'
    | FUNC '(' param_list ')'
    | FUNC '(' ')'
    | '[' expr ']' type
    | '[' ']' type
    ;


block
    : '{' stmt_list '}'
    | '{' '}'
    ;


stmt_list
    : stmt_list stmt ';'
    | stmt ';'
    ;


stmt
    : decl
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


simple_stmt
    : expr
    | expr INC
    | expr DEC
    | assignment_stmt
    ;


assignment_stmt
    : lvalue '=' expr_list
    | lvalue PLUS_ASSIGN expr_list
    | lvalue MINUS_ASSIGN expr_list
    | lvalue MUL_ASSIGN expr_list
    | lvalue DIV_ASSIGN expr_list
    | lvalue TILDE_ASSIGN expr_list
    ;


lvalue
    : primary_name
    | lvalue ',' primary_name
    ;


return_stmt
    : RETURN
    | RETURN expr_list
    ;


const_decl
    : CONST const_spec
    | CONST '(' const_spec_list ')'
    ;


const_spec_list
    : const_spec_list ';' const_spec
    | const_spec ';'
    ;


const_spec
    : id_list
    | id_list '=' expr_list
    | id_list type '=' expr_list
    ;


var_decl
    : VAR var_spec
    | VAR '(' var_spec_list ')'
    ;


var_spec_list
    : var_spec_list ';' var_spec
    | var_spec ';'
    ;


var_spec
    : id_list type
    | id_list type '=' expr_list
    | id_list '=' expr_list
    ;


id_list
    : id_list ',' ID
    | ID
    ;


type_name
    : INT
    | FLOAT
    | DOUBLE
    | REAL
    | BOOL
    | CHAR
    | STRING
    | VOID
    ;


type
    : type_name
    | type_name '[' type_list ']'
    | '[' expr ']' type
    | '[' ']' type
    | FUNC '(' param_list ')'
    | FUNC '(' ')'
    ;


type_list
    : type_list ',' type
    | type
    ;


expr_list
    : expr_list ',' expr
    | expr
    ;


expr
    : or_expr
    ;


or_expr
    : or_expr OR_OP and_expr
    | and_expr
    ;


and_expr
    : and_expr AND_OP eq_expr
    | eq_expr
    ;


eq_expr
    : eq_expr EQ rel_expr
    | eq_expr NE rel_expr
    | rel_expr
    ;


rel_expr
    : rel_expr '<' add_expr
    | rel_expr '>' add_expr
    | rel_expr LE add_expr
    | rel_expr GE add_expr
    | add_expr
    ;


add_expr
    : add_expr '+' mul_expr
    | add_expr '-' mul_expr
    | mul_expr
    ;


mul_expr
    : mul_expr '*' unary_expr
    | mul_expr '/' unary_expr
    | unary_expr
    ;


unary_expr
    : '!' unary_expr
    | '-' unary_expr
    | postfix_expr
    ;


postfix_expr
    : postfix_expr '[' expr ']'
    | postfix_expr '[' ':' ']'
    | postfix_expr '[' expr ':' ']'
    | postfix_expr '[' ':' expr ']'
    | postfix_expr '[' expr ':' expr ']'
    | postfix_expr '[' ':' expr ':' expr ']'
    | postfix_expr '[' expr ':' expr ':' expr ']'
    | postfix_expr '[' type_list ']'
    | primary_expr
    ;


primary_expr
    : operand
    | operand INC
    | operand DEC
    ;


primary_name
    : ID
    | THIS
    | SUPER
    ;


operand
    : primary_name
    | '(' expr ')'
    | INT_LIT
    | FLOAT_LIT
    | STRING_LIT
    | BOOL_LIT
    ;


if_stmt
    : IF '(' expr ')' block ELSE if_stmt
    | IF '(' expr ')' block ELSE block
    | IF '(' expr ')' block
    ;


switch_stmt
    : SWITCH '(' expr ')' '{' case_list '}'
    | SWITCH '{' case_list '}'
    ;


case_list
    : case_list case_item
    | case_item
    ;


case_item
    : CASE expr ':' stmt_list
    | DEFAULT ':' stmt_list
    ;


for_stmt
    : FOR '(' simple_stmt ';' expr ';' simple_stmt ')' block
    | FOR '(' ';' expr ';' ')' block
    | FOR '(' ';' ';' ')' block
    | FOR expr block
    ;


while_stmt
    : WHILE '(' expr ')' block
    ;


do_while_stmt
    : DO block WHILE '(' expr ')' ';'
    ;


foreach_stmt
    : FOREACH '(' ID ':' expr ')' block
    | FOREACH '(' type ID ':' expr ')' block
    ;


class_decl
    : CLASS ID class_inherit_opt '{' class_body_opt '}'
    ;


class_inherit_opt
    : ':' ID
    |
    ;


class_body_opt
    : class_body
    |
    ;


class_body
    : class_body class_member
    | class_member
    ;


class_member
    : decl ';'
    | func_decl
    | OVERRIDE func_decl
    ;


enum_decl
    : ENUM ID '{' enum_member_list '}'
    | ENUM ID '{' enum_member_list ',' '}'
    ;


enum_member_list
    : enum_member_list ',' enum_member
    | enum_member
    ;


enum_member
    : ID
    | ID '=' expr
    ;


%%


// Секция пользовательского кода
