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
    TRUE = 288,                    /* TRUE  */
    FALSE = 289,                   /* FALSE  */
    NULL_CONST = 290,              /* NULL_CONST  */
    NAN_CONST = 291,               /* NAN_CONST  */
    DOTDOT = 292,                  /* DOTDOT  */
    INTEGER = 293,                 /* INTEGER  */
    FLOATVAL = 294,                /* FLOATVAL  */
    STR = 295,                     /* STR  */
    CHARVAL = 296,                 /* CHARVAL  */
    IDENT = 297,                   /* IDENT  */
    CLASSNAME = 298,               /* CLASSNAME  */
    ASSIGN = 299,                  /* ASSIGN  */
    PLUSEQ = 300,                  /* PLUSEQ  */
    MINUSEQ = 301,                 /* MINUSEQ  */
    STAREQ = 302,                  /* STAREQ  */
    SLASHEQ = 303,                 /* SLASHEQ  */
    TILDEQ = 304,                  /* TILDEQ  */
    LOR = 305,                     /* LOR  */
    LAND = 306,                    /* LAND  */
    EQ = 307,                      /* EQ  */
    NEQ = 308,                     /* NEQ  */
    LT = 309,                      /* LT  */
    GT = 310,                      /* GT  */
    LE = 311,                      /* LE  */
    GE = 312,                      /* GE  */
    PLUS = 313,                    /* PLUS  */
    MINUS = 314,                   /* MINUS  */
    STAR = 315,                    /* STAR  */
    SLASH = 316,                   /* SLASH  */
    UNOT = 317,                    /* UNOT  */
    UMINUS = 318,                  /* UMINUS  */
    UPLUS = 319,                   /* UPLUS  */
    LOWER_THAN_ELSE = 320          /* LOWER_THAN_ELSE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_D_PARSER_TAB_H_INCLUDED  */
