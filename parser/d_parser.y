%{
extern int yylex(void);
extern int yylineno;
extern char *yytext;

void yyerror(const char *s);
%}

%token INT CHAR STRING BOOL FLOAT_TYPE DOUBLE REAL VOID
%token IF ELSE SWITCH CASE DEFAULT
%token WHILE DO FOR FOREACH
%token RETURN BREAK CONTINUE
%token CLASS OVERRIDE SUPER PUBLIC PRIVATE PROTECTED
%token THIS REF NEW ENUM
%token TRUE FALSE NULL_CONST NAN_CONST
%token DOTDOT

%token INTEGER
%token FLOATVAL
%token STR CHARVAL IDENT F_IDENT
%token CLASSNAME

%type type base_type array_decl
%type expr
%type stmt compound_stmt stmt_list decl init_decl init_decl_list
%type param param_list arg_list func_def func_body class_def enum_def
%type class_members class_member access_spec if_stmt while_stmt do_while_stmt
%type for_stmt foreach_stmt switch_stmt case_list case_item default_item
%type method_def ctor_def dtor_def initializer array_init

%right ASSIGN PLUSEQ MINUSEQ STAREQ SLASHEQ TILDEQ
%left LOR
%left LAND
%left EQ NEQ
%left LT GT LE GE
%left PLUS MINUS
%left STAR SLASH
%right UNOT UMINUS UPLUS
%left '.' '[' ']'
%right NEW
%nonassoc '(' ')'
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%start program

%%

program
    : translation_unit
    | %empty
    ;

translation_unit
    : source_item
    | translation_unit source_item
    ;

source_item
    : func_def
    | class_def
    | decl
	| enum_def
    ;

base_type
    : INT
    | CHAR
    | STRING
    | BOOL
    | FLOAT_TYPE
    | DOUBLE
    | REAL
    ;

array_decl
    : '[' ']'
    | '[' INTEGER ']'
    ;

type
    : base_type
    | base_type array_decl
    | CLASSNAME
    | CLASSNAME array_decl
    ;

array_init
    : '[' ']'
    | '[' arg_list ']'
    ;

initializer
    : expr
    | array_init
    ;
	
e_expr
	: expr
	| %empty
	;

expr
    : IDENT
    | INTEGER
    | FLOATVAL
    | CHARVAL
    | STR
    | TRUE
    | FALSE
    | NULL_CONST
    | NAN_CONST
    | THIS
    | '(' expr ')'
    | NEW CLASSNAME '[' INTEGER ']'
    | NEW F_IDENT '(' ')'
    | NEW F_IDENT '(' arg_list ')'
    | NEW base_type '[' INTEGER ']'
    | expr '[' expr ']'
    | expr '[' expr DOTDOT expr ']'
    | expr '.' IDENT
    | expr '.' F_IDENT '(' ')'
    | expr '.' F_IDENT '(' arg_list ')'
    | expr '(' ')'
    | expr '(' arg_list ')'
    | SUPER '.' IDENT
    | SUPER '.' F_IDENT '(' ')'
    | SUPER '.' F_IDENT '(' arg_list ')'
    | MINUS expr %prec UMINUS
    | PLUS expr %prec UPLUS
    | UNOT expr %prec UNOT
    | expr STAR expr
    | expr SLASH expr
    | expr PLUS expr
    | expr MINUS expr
    | expr LT expr
    | expr GT expr
    | expr LE expr
    | expr GE expr
    | expr EQ expr
    | expr NEQ expr
    | expr LAND expr
    | expr LOR expr
    | expr ASSIGN expr
    | expr PLUSEQ expr
    | expr MINUSEQ expr
    | expr STAREQ expr
    | expr SLASHEQ expr
    | expr TILDEQ expr
    ;

arg_list
    : expr
    | arg_list ',' expr
    ;

init_decl
    : IDENT
    | IDENT '=' initializer
    ;

init_decl_list
    : init_decl
    | init_decl_list ',' init_decl
    ;

decl
    : type init_decl_list ';'
    ;

param
    : type IDENT
    | REF base_type IDENT
    | type IDENT '=' expr
    | REF base_type IDENT '=' expr
    ;

param_list
    : param
    | param_list ',' param
    ;

stmt
    : e_expr ';'
    | decl
    | compound_stmt
    | if_stmt
    | while_stmt
    | do_while_stmt
    | for_stmt
    | foreach_stmt
    | switch_stmt
    | RETURN e_expr ';'
    | BREAK ';'
    | CONTINUE ';'
    ;

stmt_list
    : stmt
    | stmt_list stmt
    ;

compound_stmt
    : '{' '}'
    | '{' stmt_list '}'
    ;

if_stmt
    : IF '(' expr ')' stmt %prec LOWER_THAN_ELSE
    | IF '(' expr ')' stmt ELSE stmt
    ;

while_stmt
    : WHILE '(' expr ')' stmt
    ;

do_while_stmt
    : DO stmt WHILE '(' expr ')' ';'
    ;

for_stmt
    : FOR '(' e_expr ';' e_expr ';' e_expr ')' stmt
    | FOR '(' decl e_expr ';' e_expr ')' stmt
    ;

foreach_stmt
    : FOREACH '(' IDENT ';' expr ')' stmt
    | FOREACH '(' type IDENT ';' expr ')' stmt
    ;

switch_stmt
    : SWITCH '(' expr ')' '{' case_list '}'
    | SWITCH '(' expr ')' '{' '}'
    ;

case_list
    : case_item
    | default_item
    | case_list case_item
    | case_list default_item
    ;

case_item
    : CASE expr ':'
    | CASE expr ':' stmt_list
    ;

default_item
    : DEFAULT ':'
    | DEFAULT ':' stmt_list
    ;

func_def
    : type F_IDENT '(' ')' func_body
    | type F_IDENT '(' param_list ')' func_body
    | VOID F_IDENT '(' ')' func_body
    | VOID F_IDENT '(' param_list ')' func_body
    ;

func_body
    : compound_stmt
    | ';'
    ;

class_def
    : CLASS CLASSNAME '{' class_members '}'
    | CLASS CLASSNAME '{' '}'
    | CLASS CLASSNAME ':' CLASSNAME '{' class_members '}'
    | CLASS CLASSNAME ':' CLASSNAME '{' '}'
    ;

class_members
    : class_member
    | class_members class_member
    ;

class_member
    : decl
    | method_def
    | access_spec decl
    | access_spec method_def
    | ctor_def
    | dtor_def
    | enum_def
    | access_spec enum_def
    ;

access_spec
    : PUBLIC 
	| PRIVATE 
	| PROTECTED
    ;

method_def
    : type F_IDENT '(' ')' compound_stmt
    | type F_IDENT '(' param_list ')' compound_stmt
    | VOID F_IDENT '(' ')' compound_stmt
    | VOID F_IDENT '(' param_list ')' compound_stmt
    | OVERRIDE type F_IDENT '(' ')' compound_stmt
    | OVERRIDE type F_IDENT '(' param_list ')' compound_stmt
    | OVERRIDE VOID F_IDENT '(' ')' compound_stmt
    | OVERRIDE VOID F_IDENT '(' param_list ')' compound_stmt
    ;

ctor_def
    : THIS '(' ')' compound_stmt
    | THIS '(' param_list ')' compound_stmt
    ;

dtor_def
    : '~' THIS '(' ')' compound_stmt
    ;

enum_def
    : ENUM IDENT '{' enum_body '}'
    | ENUM '{' enum_body '}'
    ;

enum_body
    : IDENT
    | IDENT '=' INTEGER
    | enum_body ',' IDENT
    | enum_body ',' IDENT '=' INTEGER
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Parse error at line %d: %s\n", yylineno, s);
}