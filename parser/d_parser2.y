%{
extern int yylex(void);
extern int yylineno;
extern char *yytext;

void yyerror(const char *s);
%}

%token INT CHAR STRING BOOL FLOAT_TYPE DOUBLE REAL VOID
%token IF ELSE SWITCH CASE DEFAULT
%token WHILE DO FOR FOREACH IN
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
%type expr prim_expr postfix_expr unary_expr mult_expr add_expr
%type rel_expr eq_expr land_expr lor_expr assign_expr
%type stmt compound_stmt stmt_list decl init_decl init_decl_list
%type param param_list arg arg_list func_def func_body class_def enum_def
%type class_members class_member access_spec if_stmt while_stmt do_while_stmt
%type for_stmt foreach_stmt switch_stmt case_list case_item default_item
%type method_def field_decl ctor_def dtor_def initializer array_init

%right ASSIGN PLUSEQ MINUSEQ STAREQ SLASHEQ TILDEQ
%left LOR
%left LAND
%left EQ NEQ
%left LT GT LE GE
%left PLUS MINUS
%left STAR SLASH
%right UNOT UMINUS UPLUS BITNOT
%left '.'
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
    | array_decl '[' ']'
    | array_decl '[' INTEGER ']'
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

prim_expr
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
    | NEW CLASSNAME array_decl
    | NEW F_IDENT '(' ')'
    | NEW F_IDENT '(' arg_list ')'
    | NEW base_type
    ;

postfix_expr
    : prim_expr
    | postfix_expr '[' expr ']'
    | postfix_expr '[' expr DOTDOT expr ']'
    | postfix_expr '.' IDENT
    | postfix_expr '.' F_IDENT '(' ')'
    | postfix_expr '.' F_IDENT '(' arg_list ')'
    | postfix_expr '(' ')'
    | postfix_expr '(' arg_list ')'
    | SUPER '.' IDENT
    | SUPER '.' F_IDENT '(' ')'
    | SUPER '.' F_IDENT '(' arg_list ')'
    ;

unary_expr
    : postfix_expr
    | MINUS unary_expr %prec UMINUS
    | PLUS unary_expr %prec UPLUS
    | UNOT unary_expr %prec UNOT
    | BITNOT unary_expr %prec BITNOT
    ;

mult_expr
    : unary_expr
    | mult_expr STAR unary_expr
    | mult_expr SLASH unary_expr
    ;

add_expr
    : mult_expr
    | add_expr PLUS mult_expr
    | add_expr MINUS mult_expr
    ;

rel_expr
    : add_expr
    | rel_expr LT add_expr
    | rel_expr GT add_expr
    | rel_expr LE add_expr
    | rel_expr GE add_expr
    ;

eq_expr
    : rel_expr
    | eq_expr EQ rel_expr
    | eq_expr NEQ rel_expr
    ;

land_expr
    : eq_expr
    | land_expr LAND eq_expr
    ;

lor_expr
    : land_expr
    | lor_expr LOR land_expr
    ;

assign_expr
    : lor_expr
    | unary_expr ASSIGN assign_expr
    | unary_expr PLUSEQ assign_expr
    | unary_expr MINUSEQ assign_expr
    | unary_expr STAREQ assign_expr
    | unary_expr SLASHEQ assign_expr
    | unary_expr TILDEQ assign_expr
    ;

expr
    : assign_expr
    ;

arg
    : expr
    ;

arg_list
    : arg
    | arg_list ',' arg
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
    : ';'
    | expr ';'
    | decl
    | compound_stmt
    | if_stmt
    | while_stmt
    | do_while_stmt
    | for_stmt
    | foreach_stmt
    | switch_stmt
    | RETURN ';'
    | RETURN expr ';'
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
    : FOR '(' ';' ';' ')' stmt
    | FOR '(' expr ';' ';' ')' stmt
    | FOR '(' ';' expr ';' ')' stmt
    | FOR '(' expr ';' expr ';' ')' stmt
    | FOR '(' ';' ';' expr ')' stmt
    | FOR '(' expr ';' ';' expr ')' stmt
    | FOR '(' ';' expr ';' expr ')' stmt
    | FOR '(' expr ';' expr ';' expr ')' stmt
    | FOR '(' decl ';' ')' stmt
    | FOR '(' decl expr ';' ')' stmt
    | FOR '(' decl ';' expr ')' stmt
    | FOR '(' decl expr ';' expr ')' stmt
    ;

foreach_stmt
    : FOREACH '(' IDENT IN expr ')' stmt
    | FOREACH '(' type IDENT IN expr ')' stmt
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
    : CASE INTEGER ':'
    | CASE INTEGER ':' stmt_list
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
    : field_decl
    | method_def
    | access_spec field_decl
    | access_spec method_def
    | ctor_def
    | dtor_def
    | enum_def
    | access_spec enum_def
    ;

access_spec
    : PUBLIC | PRIVATE | PROTECTED
    ;

field_decl
    : type IDENT ';'
    | type IDENT '=' initializer ';'
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

int main(int argc, char *argv[]) {
    if (argc > 1) {
        FILE *f = fopen(argv[1], "r");
        if (!f) {
            perror("fopen");
            return 1;
        }
        yyin = f;
    } else {
        yyin = stdin;
    }

    int result = yyparse();

    if (argc > 1 && yyin != stdin) {
        fclose(yyin);
    }

    return result;
}
