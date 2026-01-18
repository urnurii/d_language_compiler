/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_D_PARSER_TAB_H_INCLUDED
# define YY_YY_D_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    INT = 258,                     /* INT  */
    CHAR = 259,                    /* CHAR  */
    STRING = 260,                  /* STRING  */
    BOOL = 261,                    /* BOOL  */
    FLOAT_TYPE = 262,              /* FLOAT_TYPE  */
    DOUBLE = 263,                  /* DOUBLE  */
    REAL = 264,                    /* REAL  */
    VOID = 265,                    /* VOID  */
    IF = 266,                      /* IF  */
    ELSE = 267,                    /* ELSE  */
    SWITCH = 268,                  /* SWITCH  */
    CASE = 269,                    /* CASE  */
    DEFAULT = 270,                 /* DEFAULT  */
    WHILE = 271,                   /* WHILE  */
    DO = 272,                      /* DO  */
    FOR = 273,                     /* FOR  */
    FOREACH = 274,                 /* FOREACH  */
    RETURN = 275,                  /* RETURN  */
    BREAK = 276,                   /* BREAK  */
    CONTINUE = 277,                /* CONTINUE  */
    CLASS = 278,                   /* CLASS  */
    OVERRIDE = 279,                /* OVERRIDE  */
    SUPER = 280,                   /* SUPER  */
    PUBLIC = 281,                  /* PUBLIC  */
    PRIVATE = 282,                 /* PRIVATE  */
    PROTECTED = 283,               /* PROTECTED  */
    THIS = 284,                    /* THIS  */
    REF = 285,                     /* REF  */
    NEW = 286,                     /* NEW  */
    ENUM = 287,                    /* ENUM  */
    TRUE_VAL = 288,                /* TRUE_VAL  */
    FALSE_VAL = 289,               /* FALSE_VAL  */
    NULL_CONST = 290,              /* NULL_CONST  */
    NAN_CONST = 291,               /* NAN_CONST  */
    DOTDOT = 292,                  /* DOTDOT  */
    INTEGER = 293,                 /* INTEGER  */
    FLOAT_VAL = 294,               /* FLOAT_VAL  */
    STR = 295,                     /* STR  */
    IDENT = 296,                   /* IDENT  */
    CLASSNAME = 297,               /* CLASSNAME  */
    CHARVAL = 298,                 /* CHARVAL  */
    PLUSEQ = 299,                  /* PLUSEQ  */
    MINUSEQ = 300,                 /* MINUSEQ  */
    STAREQ = 301,                  /* STAREQ  */
    SLASHEQ = 302,                 /* SLASHEQ  */
    TILDEQ = 303,                  /* TILDEQ  */
    LOR = 304,                     /* LOR  */
    LAND = 305,                    /* LAND  */
    EQ = 306,                      /* EQ  */
    NEQ = 307,                     /* NEQ  */
    LT = 308,                      /* LT  */
    GT = 309,                      /* GT  */
    LE = 310,                      /* LE  */
    GE = 311,                      /* GE  */
    PLUS = 312,                    /* PLUS  */
    MINUS = 313,                   /* MINUS  */
    STAR = 314,                    /* STAR  */
    SLASH = 315,                   /* SLASH  */
    UNOT = 316,                    /* UNOT  */
    UMINUS = 317,                  /* UMINUS  */
    UPLUS = 318,                   /* UPLUS  */
    LOWER_THAN_ELSE = 319          /* LOWER_THAN_ELSE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 18 "d_parser.y"

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

#line 183 "d_parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_D_PARSER_TAB_H_INCLUDED  */
