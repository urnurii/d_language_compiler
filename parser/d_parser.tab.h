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
    MODULE = 258,                  /* MODULE  */
    IMPORT = 259,                  /* IMPORT  */
    CONST = 260,                   /* CONST  */
    FUNC = 261,                    /* FUNC  */
    VAR = 262,                     /* VAR  */
    RETURN = 263,                  /* RETURN  */
    BREAK = 264,                   /* BREAK  */
    CONTINUE = 265,                /* CONTINUE  */
    IF = 266,                      /* IF  */
    ELSE = 267,                    /* ELSE  */
    SWITCH = 268,                  /* SWITCH  */
    CASE = 269,                    /* CASE  */
    DEFAULT = 270,                 /* DEFAULT  */
    DO = 271,                      /* DO  */
    WHILE = 272,                   /* WHILE  */
    FOR = 273,                     /* FOR  */
    FOREACH = 274,                 /* FOREACH  */
    CLASS = 275,                   /* CLASS  */
    ENUM = 276,                    /* ENUM  */
    NEW = 277,                     /* NEW  */
    THIS = 278,                    /* THIS  */
    SUPER = 279,                   /* SUPER  */
    OVERRIDE = 280,                /* OVERRIDE  */
    PUBLIC = 281,                  /* PUBLIC  */
    PRIVATE = 282,                 /* PRIVATE  */
    PROTECTED = 283,               /* PROTECTED  */
    AUTO = 284,                    /* AUTO  */
    REF = 285,                     /* REF  */
    INT = 286,                     /* INT  */
    FLOAT = 287,                   /* FLOAT  */
    DOUBLE = 288,                  /* DOUBLE  */
    REAL = 289,                    /* REAL  */
    BOOL = 290,                    /* BOOL  */
    CHAR = 291,                    /* CHAR  */
    STRING = 292,                  /* STRING  */
    VOID = 293,                    /* VOID  */
    INT_LIT = 294,                 /* INT_LIT  */
    FLOAT_LIT = 295,               /* FLOAT_LIT  */
    BOOL_LIT = 296,                /* BOOL_LIT  */
    ID = 297,                      /* ID  */
    STRING_LIT = 298,              /* STRING_LIT  */
    PLUS_ASSIGN = 299,             /* PLUS_ASSIGN  */
    MINUS_ASSIGN = 300,            /* MINUS_ASSIGN  */
    MUL_ASSIGN = 301,              /* MUL_ASSIGN  */
    DIV_ASSIGN = 302,              /* DIV_ASSIGN  */
    TILDE_ASSIGN = 303,            /* TILDE_ASSIGN  */
    EQ = 304,                      /* EQ  */
    NE = 305,                      /* NE  */
    LE = 306,                      /* LE  */
    GE = 307,                      /* GE  */
    AND_OP = 308,                  /* AND_OP  */
    OR_OP = 309,                   /* OR_OP  */
    INC = 310,                     /* INC  */
    DEC = 311                      /* DEC  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 24 "d_parser.y"

    int int_lit;
    char *identifier;
    char *str_lit;
    float float_lit;
    bool bool_lit;

#line 128 "d_parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_D_PARSER_TAB_H_INCLUDED  */
