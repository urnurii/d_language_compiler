%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree_nodes.h"
#include "ast_builder.h"

int yylex(void);
int yyparse(void);
void yyerror(const char *s);
extern int yylineno;
extern char *yytext;
NExpr* CreateNewArrayExpr(NType *type, NExpr *expr);
void AppendSourceItemToRoot(NSourceItem *item);

NProgram *root = NULL;
%}
%union {
    // Типы
    NType *type;
    NArrayDecl *array_decl;
    NInitializer *initializer;
    BaseType base_type;
    TypeKind type_kind;
    
    // Выражения
    NExpr *expr;
    NExprList *expr_list;
    OpType op_type;
    ExprType expr_type;
    
    // Параметры и объявления
    NParam *param;
    NParamList *param_list;
    NInitDecl *init_decl;
    NInitDeclList *init_decl_list;
    NDeclStmt decl_stmt;
    
    // Операторы
    NStmt *stmt;
    NStmtList *stmt_list;
    StmtType stmt_type;
    NCaseItem *case_item;
    NCaseList *case_list;
    
    // Функции и методы
    NFuncDef *func_def;
    NMethodDef *method_def;
    NCtorDef *ctor_def;
    NDtorDef *dtor_def;
    
    // Классы
    NClassDef *class_def;
    NClassMember *class_member;
    AccessSpec access_spec;
    ClassMemberType class_member_type;
    
    // Enum
    NEnumDef *enum_def;
    NEnumItemList *enum_item_list;
    
    // Элементы верхнего уровня
    NSourceItem *source_item;
    NProgram *program;
    
    // Терминалы
    char *string_val;
    int int_val;
    double float_val;
    char char_val;
}

%token INT CHAR STRING BOOL FLOAT_TYPE DOUBLE REAL VOID
%token IF ELSE SWITCH CASE DEFAULT
%token WHILE DO FOR FOREACH
%token RETURN BREAK CONTINUE
%token CLASS OVERRIDE SUPER PUBLIC PRIVATE PROTECTED
%token THIS REF NEW ENUM
%token TRUE_VAL FALSE_VAL NULL_CONST NAN_CONST
%token DOTDOT

%token <int_val> INTEGER
%token <float_val> FLOAT_VAL
%token <string_val> STR IDENT CLASSNAME ENUMNAME
%token <char_val> CHARVAL

%type <type> type base_type
%type <array_decl> array_decl
%type <expr> expr e_expr
%type <expr_list> arg_list
%type <initializer> initializer array_init
%type <stmt> stmt compound_stmt if_stmt while_stmt do_while_stmt
%type <stmt> for_stmt foreach_stmt switch_stmt
%type <stmt_list> stmt_list
%type <case_item> case_item default_item
%type <case_list> case_list
%type <param> param
%type <param_list> param_list
%type <init_decl> init_decl
%type <init_decl_list> init_decl_list
%type <decl_stmt> decl
%type <func_def> func_def
%type <stmt> func_body
%type <method_def> method_def
%type <ctor_def> ctor_def
%type <dtor_def> dtor_def
%type <class_def> class_def
%type <class_member> class_member class_members
%type <access_spec> access_spec
%type <enum_def> enum_def
%type <string_val> enum_item_name
%type <enum_item_list> enum_body
%type <source_item> source_item translation_unit
%type <program> program

%right '=' PLUSEQ MINUSEQ STAREQ SLASHEQ TILDEQ
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
    : translation_unit { $$ = root; }
    | %empty { $$ = CreateProgram(NULL); root = $$; }
    ;

translation_unit
    : source_item { AppendSourceItemToRoot($1); $$ = $1; }
    | translation_unit source_item { AppendSourceItemToRoot($2); $$ = $1; }
    ;

source_item
    : func_def { $$ = CreateFuncSourceItem($1); }
    | class_def { $$ = CreateClassSourceItem($1); }
    | decl { $$ = CreateDeclSourceItem($1.type, $1.init_decls); }
    | enum_def { $$ = CreateEnumSourceItem($1); }
    ;

base_type
    : INT { $$ = CreateBaseType(TYPE_INT); }
    | CHAR { $$ = CreateBaseType(TYPE_CHAR); }
    | STRING { $$ = CreateBaseType(TYPE_STRING); }
    | BOOL { $$ = CreateBaseType(TYPE_BOOL); }
    | FLOAT_TYPE { $$ = CreateBaseType(TYPE_FLOAT); }
    | DOUBLE { $$ = CreateBaseType(TYPE_DOUBLE); }
    | REAL { $$ = CreateBaseType(TYPE_REAL); }
    ;

array_decl
    : '[' ']' { $$ = CreateArrayDecl(0, 0); }
    | '[' INTEGER ']' { $$ = CreateArrayDecl(1, $2); }
    ;

type
    : base_type { $$ = $1; }
    | base_type array_decl { $$ = AddArrayToType($1, $2); }
    | CLASSNAME { $$ = CreateClassType($1); }
    | CLASSNAME array_decl { $$ = AddArrayToType(CreateClassType($1), $2); }
    | ENUMNAME { $$ = CreateEnumType($1); }
    | ENUMNAME array_decl { $$ = AddArrayToType(CreateEnumType($1), $2); }
    ;

array_init
    : '[' ']' { $$ = CreateArrayInitializer(NULL, 0); }
    | '[' arg_list ']' { $$ = CreateArrayInitializer($2->elements, $2->count); }
    ;

initializer
    : expr { $$ = CreateExprInitializer($1); }
    | array_init { $$ = $1; }
    ;

e_expr
    : expr { $$ = $1; }
    | %empty { $$ = NULL; }
    ;

expr
    : IDENT { $$ = CreateIdentExpr($1); }
    | INTEGER { $$ = CreateIntExpr($1); }
    | FLOAT_VAL { $$ = CreateFloatExpr($1); }
    | CHARVAL { $$ = CreateCharExpr($1); }
    | STR { $$ = CreateStringExpr($1); }
    | TRUE_VAL { $$ = CreateBoolExpr(1); }
    | FALSE_VAL { $$ = CreateBoolExpr(0); }
    | NULL_CONST { $$ = CreateNullExpr(); }
    | NAN_CONST { $$ = CreateNanExpr(); }
    | THIS { $$ = CreateThisExpr(); }
    | '(' expr ')' { $$ = CreateParenExpr($2); }
    | NEW CLASSNAME '[' expr ']' { $$ = CreateNewArrayExpr(CreateClassType($2), $4); }
    | NEW CLASSNAME '(' ')' { $$ = CreateNewExpr(CreateClassType($2), NULL, 0); }
    | NEW CLASSNAME '(' arg_list ')' { $$ = CreateNewExpr(CreateClassType($2), $4->elements, $4->count); }
    | NEW ENUMNAME '[' expr ']' { $$ = CreateNewArrayExpr(CreateEnumType($2), $4); }
    | NEW base_type '[' expr ']' { $$ = CreateNewArrayExpr($2, $4); }
    | expr '[' expr ']' { $$ = CreateArrayAccessExpr($1, $3, NULL); }
    | expr '[' expr DOTDOT expr ']' { $$ = CreateArrayAccessExpr($1, $3, $5); }
    | ENUMNAME '.' IDENT { $$ = CreateMemberAccessExpr(CreateIdentExpr($1), $3); }
    | expr '.' IDENT { $$ = CreateMemberAccessExpr($1, $3); }
    | expr '.' IDENT '(' ')' { $$ = CreateMethodCallExpr($1, $3, NULL, 0); }
    | expr '.' IDENT '(' arg_list ')' { $$ = CreateMethodCallExpr($1, $3, $5->elements, $5->count); }
    | IDENT '(' ')' { $$ = CreateFuncCallExpr($1, NULL, 0); }
    | IDENT '(' arg_list ')' { $$ = CreateFuncCallExpr($1, $3->elements, $3->count); }
    | SUPER '.' IDENT { $$ = CreateSuperExpr($3); }
    | SUPER '.' IDENT '(' ')' { $$ = CreateSuperMethodCallExpr($3, NULL, 0); }
    | SUPER '.' IDENT '(' arg_list ')' { $$ = CreateSuperMethodCallExpr($3, $5->elements, $5->count); }
    | MINUS expr %prec UMINUS { $$ = CreateUnaryOpExpr(OP_MINUS, $2); }
    | PLUS expr %prec UPLUS { $$ = CreateUnaryOpExpr(OP_PLUS, $2); }
    | UNOT expr %prec UNOT { $$ = CreateUnaryOpExpr(OP_NOT, $2); }
    | expr STAR expr { $$ = CreateBinaryOpExpr(OP_MUL, $1, $3); }
    | expr SLASH expr { $$ = CreateBinaryOpExpr(OP_DIV, $1, $3); }
    | expr PLUS expr { $$ = CreateBinaryOpExpr(OP_PLUS, $1, $3); }
    | expr MINUS expr { $$ = CreateBinaryOpExpr(OP_MINUS, $1, $3); }
    | expr LT expr { $$ = CreateBinaryOpExpr(OP_LT, $1, $3); }
    | expr GT expr { $$ = CreateBinaryOpExpr(OP_GT, $1, $3); }
    | expr LE expr { $$ = CreateBinaryOpExpr(OP_LE, $1, $3); }
    | expr GE expr { $$ = CreateBinaryOpExpr(OP_GE, $1, $3); }
    | expr EQ expr { $$ = CreateBinaryOpExpr(OP_EQ, $1, $3); }
    | expr NEQ expr { $$ = CreateBinaryOpExpr(OP_NEQ, $1, $3); }
    | expr LAND expr { $$ = CreateBinaryOpExpr(OP_AND, $1, $3); }
    | expr LOR expr { $$ = CreateBinaryOpExpr(OP_OR, $1, $3); }
    | expr '=' expr { $$ = CreateAssignExpr(OP_ASSIGN, $1, $3); }
    | expr PLUSEQ expr { $$ = CreateAssignExpr(OP_PLUS_ASSIGN, $1, $3); }
    | expr MINUSEQ expr { $$ = CreateAssignExpr(OP_MINUS_ASSIGN, $1, $3); }
    | expr STAREQ expr { $$ = CreateAssignExpr(OP_MUL_ASSIGN, $1, $3); }
    | expr SLASHEQ expr { $$ = CreateAssignExpr(OP_DIV_ASSIGN, $1, $3); }
    | expr TILDEQ expr { $$ = CreateAssignExpr(OP_BITWISE_NOT_ASSIGN, $1, $3); }
    ;

arg_list
    : expr { $$ = CreateExprList(); AddExprToList($$, $1); }
    | arg_list ',' expr { $$ = $1; AddExprToList($$, $3); }
    ;

init_decl
    : IDENT { $$ = CreateInitDecl($1, NULL); }
    | IDENT '=' initializer { $$ = CreateInitDecl($1, $3); }
    ;

init_decl_list
    : init_decl { $$ = CreateInitDeclList(); AddInitDeclToList($$, $1); }
    | init_decl_list ',' init_decl { $$ = $1; AddInitDeclToList($$, $3); }
    ;

decl
    : type init_decl_list ';' { $$.type = $1; $$.init_decls = $2; }
    ;

param
    : type IDENT { $$ = CreateParam($1, $2, 0, NULL); }
    | REF type IDENT { $$ = CreateParam($2, $3, 1, NULL); }
    | type IDENT '=' expr { $$ = CreateParam($1, $2, 0, $4); }
    | REF type IDENT '=' expr { $$ = CreateParam($2, $3, 1, $5); }
    ;

param_list
    : param { $$ = CreateParamList(); AddParamToList($$, $1); }
    | param_list ',' param { $$ = $1; AddParamToList($$, $3); }
    ;

stmt
    : e_expr ';' { $$ = CreateExprStmt($1); }
    | decl { $$ = CreateDeclStmt($1.type, $1.init_decls); }
    | compound_stmt { $$ = $1; }
    | if_stmt { $$ = $1; }
    | while_stmt { $$ = $1; }
    | do_while_stmt { $$ = $1; }
    | for_stmt { $$ = $1; }
    | foreach_stmt { $$ = $1; }
    | switch_stmt { $$ = $1; }
    | RETURN e_expr ';' { $$ = CreateReturnStmt($2); }
    | BREAK ';' { $$ = CreateBreakStmt(); }
    | CONTINUE ';' { $$ = CreateContinueStmt(); }
    ;

stmt_list
    : stmt { $$ = CreateStmtList(); AddStmtToList($$, $1); }
    | stmt_list stmt { $$ = $1; AddStmtToList($$, $2); }
    ;

compound_stmt
    : '{' '}' { $$ = CreateCompoundStmt(NULL); }
    | '{' stmt_list '}' { $$ = CreateCompoundStmt($2); }
    ;

if_stmt
    : IF '(' expr ')' stmt %prec LOWER_THAN_ELSE { $$ = CreateIfStmt($3, $5, NULL); }
    | IF '(' expr ')' stmt ELSE stmt { $$ = CreateIfStmt($3, $5, $7); }
    ;

while_stmt
    : WHILE '(' expr ')' stmt { $$ = CreateWhileStmt($3, $5); }
    ;

do_while_stmt
    : DO stmt WHILE '(' expr ')' ';' { $$ = CreateDoWhileStmt($2, $5); }
    ;

for_stmt
    : FOR '(' e_expr ';' e_expr ';' e_expr ')' stmt { $$ = CreateForStmt($3, NULL, NULL, $5, $7, $9); }
    | FOR '(' decl e_expr ';' e_expr ')' stmt { $$ = CreateForStmt(NULL, $3.type, $3.init_decls, $4, $6, $8); }
    ;

foreach_stmt
    : FOREACH '(' IDENT ';' expr ')' stmt { $$ = CreateForeachStmt(0, NULL, $3, $5, $7); }
    | FOREACH '(' type IDENT ';' expr ')' stmt { $$ = CreateForeachStmt(1, $3, $4, $6, $8); }
    ;

switch_stmt
    : SWITCH '(' expr ')' '{' case_list '}' { $$ = CreateSwitchStmt($3, $6->items, $6->count); }
    | SWITCH '(' expr ')' '{' '}' { $$ = CreateSwitchStmt($3, NULL, 0); }
    ;

case_list
    : case_item { $$ = CreateCaseList(); AddCaseItemToList($$, $1); }
    | default_item { $$ = CreateCaseList(); AddCaseItemToList($$, $1); }
    | case_list case_item { $$ = $1; AddCaseItemToList($$, $2); }
    | case_list default_item { $$ = $1; AddCaseItemToList($$, $2); }
    ;

case_item
    : CASE expr ':' { $$ = CreateCaseItem($2, NULL); }
    | CASE expr ':' stmt_list { $$ = CreateCaseItem($2, $4); }
    ;

default_item
    : DEFAULT ':' { $$ = CreateDefaultItem(NULL); }
    | DEFAULT ':' stmt_list { $$ = CreateDefaultItem($3); }
    ;

func_def
    : type IDENT '(' ')' func_body { $$ = CreateFuncDef($1, $2, NULL, $5); }
    | type IDENT '(' param_list ')' func_body { $$ = CreateFuncDef($1, $2, $4, $6); }
    | VOID IDENT '(' ')' func_body { $$ = CreateVoidFuncDef($2, NULL, $5); }
    | VOID IDENT '(' param_list ')' func_body { $$ = CreateVoidFuncDef($2, $4, $6); }
    ;

func_body
    : compound_stmt { $$ = $1; }
    | ';' { $$ = NULL; }
    ;

class_def
    : CLASS CLASSNAME '{' class_members '}' { $$ = CreateClassDef($2, NULL, $4); }
    | CLASS CLASSNAME '{' '}' { $$ = CreateClassDef($2, NULL, NULL); }
    | CLASS CLASSNAME ':' CLASSNAME '{' class_members '}' { $$ = CreateClassDef($2, $4, $6); }
    | CLASS CLASSNAME ':' CLASSNAME '{' '}' { $$ = CreateClassDef($2, $4, NULL); }
    ;

class_members
    : class_member { $$ = $1; }
    | class_members class_member { $$ = AppendClassMember($1, $2); }
    ;

class_member
    : decl { $$ = CreateFieldMember(ACCESS_PUBLIC, $1.type, $1.init_decls); }
    | method_def { $$ = CreateMethodMember(ACCESS_PUBLIC, $1); }
    | access_spec decl { $$ = CreateFieldMember($1, $2.type, $2.init_decls); }
    | access_spec method_def { $$ = CreateMethodMember($1, $2); }
    | ctor_def { $$ = CreateCtorMember(ACCESS_PUBLIC, $1); }
    | dtor_def { $$ = CreateDtorMember(ACCESS_PUBLIC, $1); }
    | enum_def { $$ = CreateEnumMember(ACCESS_PUBLIC, $1); }
    | access_spec enum_def { $$ = CreateEnumMember($1, $2); }
    ;

access_spec
    : PUBLIC { $$ = ACCESS_PUBLIC; }
    | PRIVATE { $$ = ACCESS_PRIVATE; }
    | PROTECTED { $$ = ACCESS_PROTECTED; }
    ;

method_def
    : type IDENT '(' ')' compound_stmt { $$ = CreateMethodDef(0, $1, $2, NULL, $5); }
    | type IDENT '(' param_list ')' compound_stmt { $$ = CreateMethodDef(0, $1, $2, $4, $6); }
    | VOID IDENT '(' ')' compound_stmt { $$ = CreateVoidMethodDef(0, $2, NULL, $5); }
    | VOID IDENT '(' param_list ')' compound_stmt { $$ = CreateVoidMethodDef(0, $2, $4, $6); }
    | OVERRIDE type IDENT '(' ')' compound_stmt { $$ = CreateMethodDef(1, $2, $3, NULL, $6); }
    | OVERRIDE type IDENT '(' param_list ')' compound_stmt { $$ = CreateMethodDef(1, $2, $3, $5, $7); }
    | OVERRIDE VOID IDENT '(' ')' compound_stmt { $$ = CreateVoidMethodDef(1, $3, NULL, $6); }
    | OVERRIDE VOID IDENT '(' param_list ')' compound_stmt { $$ = CreateVoidMethodDef(1, $3, $5, $7); }
    ;

ctor_def
    : THIS '(' ')' compound_stmt { $$ = CreateCtorDef(NULL, $4); }
    | THIS '(' param_list ')' compound_stmt { $$ = CreateCtorDef($3, $5); }
    ;

dtor_def
    : '~' THIS '(' ')' compound_stmt { $$ = CreateDtorDef($5); }
    ;

enum_def
    : ENUM IDENT '{' enum_body '}' { $$ = CreateEnumDef($2, $4->items, $4->count); }
    | ENUM ENUMNAME '{' enum_body '}' { $$ = CreateEnumDef($2, $4->items, $4->count); }
    | ENUM '{' enum_body '}' { $$ = CreateEnumDef(NULL, $3->items, $3->count); }
    ;

enum_item_name
    : IDENT { $$ = $1; }
    | CLASSNAME { $$ = $1; }
    | ENUMNAME { $$ = $1; }
    ;

enum_body
    : enum_item_name { $$ = CreateEnumItemList(); AddEnumItemToList($$, CreateEnumItem($1, 0, 0)); }
    | enum_item_name '=' INTEGER { $$ = CreateEnumItemList(); AddEnumItemToList($$, CreateEnumItem($1, 1, $3)); }
    | enum_body ',' enum_item_name { $$ = $1; AddEnumItemToList($$, CreateEnumItem($3, 0, 0)); }
    | enum_body ',' enum_item_name '=' INTEGER { $$ = $1; AddEnumItemToList($$, CreateEnumItem($3, 1, $5)); }
    ;

%%

void yyerror(const char *s) {
    if (yytext && yytext[0] != '\0') {
        fprintf(stderr, "PARSER ERROR at line %d near '%s': %s\n", yylineno, yytext, s);
    } else {
        fprintf(stderr, "PARSER ERROR at line %d: %s\n", yylineno, s);
    }
}

void AppendSourceItemToRoot(NSourceItem *item) {
    if (!item) return;
    if (!root) {
        root = CreateProgram(item);
    } else {
        AddSourceItemToProgram(root, item);
    }
}

NExpr* CreateNewArrayExpr(NType *type, NExpr *expr)
{
    NExprList *list = CreateExprList();
    AddExprToList(list, expr);
    if (type != NULL) {
        type = AddArrayToType(type, CreateArrayDecl(0, 0));
    }
    NExpr *result = CreateNewExpr(type, list->elements, list->count);
    return result;
}

