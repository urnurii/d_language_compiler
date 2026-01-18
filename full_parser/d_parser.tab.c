/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "d_parser.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree_nodes.h"
#include "ast_builder.h"

int yylex(void);
int yyparse(void);
void yyerror(const char *s);
extern int yylineno;
NExpr* CreateNewArrayExpr(NType *type, NExpr *expr);
void AppendSourceItemToRoot(NSourceItem *item);

NProgram *root = NULL;

#line 88 "d_parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "d_parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_INT = 3,                        /* INT  */
  YYSYMBOL_CHAR = 4,                       /* CHAR  */
  YYSYMBOL_STRING = 5,                     /* STRING  */
  YYSYMBOL_BOOL = 6,                       /* BOOL  */
  YYSYMBOL_FLOAT_TYPE = 7,                 /* FLOAT_TYPE  */
  YYSYMBOL_DOUBLE = 8,                     /* DOUBLE  */
  YYSYMBOL_REAL = 9,                       /* REAL  */
  YYSYMBOL_VOID = 10,                      /* VOID  */
  YYSYMBOL_IF = 11,                        /* IF  */
  YYSYMBOL_ELSE = 12,                      /* ELSE  */
  YYSYMBOL_SWITCH = 13,                    /* SWITCH  */
  YYSYMBOL_CASE = 14,                      /* CASE  */
  YYSYMBOL_DEFAULT = 15,                   /* DEFAULT  */
  YYSYMBOL_WHILE = 16,                     /* WHILE  */
  YYSYMBOL_DO = 17,                        /* DO  */
  YYSYMBOL_FOR = 18,                       /* FOR  */
  YYSYMBOL_FOREACH = 19,                   /* FOREACH  */
  YYSYMBOL_RETURN = 20,                    /* RETURN  */
  YYSYMBOL_BREAK = 21,                     /* BREAK  */
  YYSYMBOL_CONTINUE = 22,                  /* CONTINUE  */
  YYSYMBOL_CLASS = 23,                     /* CLASS  */
  YYSYMBOL_OVERRIDE = 24,                  /* OVERRIDE  */
  YYSYMBOL_SUPER = 25,                     /* SUPER  */
  YYSYMBOL_PUBLIC = 26,                    /* PUBLIC  */
  YYSYMBOL_PRIVATE = 27,                   /* PRIVATE  */
  YYSYMBOL_PROTECTED = 28,                 /* PROTECTED  */
  YYSYMBOL_THIS = 29,                      /* THIS  */
  YYSYMBOL_REF = 30,                       /* REF  */
  YYSYMBOL_NEW = 31,                       /* NEW  */
  YYSYMBOL_ENUM = 32,                      /* ENUM  */
  YYSYMBOL_TRUE_VAL = 33,                  /* TRUE_VAL  */
  YYSYMBOL_FALSE_VAL = 34,                 /* FALSE_VAL  */
  YYSYMBOL_NULL_CONST = 35,                /* NULL_CONST  */
  YYSYMBOL_NAN_CONST = 36,                 /* NAN_CONST  */
  YYSYMBOL_DOTDOT = 37,                    /* DOTDOT  */
  YYSYMBOL_INTEGER = 38,                   /* INTEGER  */
  YYSYMBOL_FLOAT_VAL = 39,                 /* FLOAT_VAL  */
  YYSYMBOL_STR = 40,                       /* STR  */
  YYSYMBOL_IDENT = 41,                     /* IDENT  */
  YYSYMBOL_CLASSNAME = 42,                 /* CLASSNAME  */
  YYSYMBOL_CHARVAL = 43,                   /* CHARVAL  */
  YYSYMBOL_44_ = 44,                       /* '='  */
  YYSYMBOL_PLUSEQ = 45,                    /* PLUSEQ  */
  YYSYMBOL_MINUSEQ = 46,                   /* MINUSEQ  */
  YYSYMBOL_STAREQ = 47,                    /* STAREQ  */
  YYSYMBOL_SLASHEQ = 48,                   /* SLASHEQ  */
  YYSYMBOL_TILDEQ = 49,                    /* TILDEQ  */
  YYSYMBOL_LOR = 50,                       /* LOR  */
  YYSYMBOL_LAND = 51,                      /* LAND  */
  YYSYMBOL_EQ = 52,                        /* EQ  */
  YYSYMBOL_NEQ = 53,                       /* NEQ  */
  YYSYMBOL_LT = 54,                        /* LT  */
  YYSYMBOL_GT = 55,                        /* GT  */
  YYSYMBOL_LE = 56,                        /* LE  */
  YYSYMBOL_GE = 57,                        /* GE  */
  YYSYMBOL_PLUS = 58,                      /* PLUS  */
  YYSYMBOL_MINUS = 59,                     /* MINUS  */
  YYSYMBOL_STAR = 60,                      /* STAR  */
  YYSYMBOL_SLASH = 61,                     /* SLASH  */
  YYSYMBOL_UNOT = 62,                      /* UNOT  */
  YYSYMBOL_UMINUS = 63,                    /* UMINUS  */
  YYSYMBOL_UPLUS = 64,                     /* UPLUS  */
  YYSYMBOL_65_ = 65,                       /* '.'  */
  YYSYMBOL_66_ = 66,                       /* '['  */
  YYSYMBOL_67_ = 67,                       /* ']'  */
  YYSYMBOL_68_ = 68,                       /* '('  */
  YYSYMBOL_69_ = 69,                       /* ')'  */
  YYSYMBOL_LOWER_THAN_ELSE = 70,           /* LOWER_THAN_ELSE  */
  YYSYMBOL_71_ = 71,                       /* ','  */
  YYSYMBOL_72_ = 72,                       /* ';'  */
  YYSYMBOL_73_ = 73,                       /* '{'  */
  YYSYMBOL_74_ = 74,                       /* '}'  */
  YYSYMBOL_75_ = 75,                       /* ':'  */
  YYSYMBOL_76_ = 76,                       /* '~'  */
  YYSYMBOL_YYACCEPT = 77,                  /* $accept  */
  YYSYMBOL_program = 78,                   /* program  */
  YYSYMBOL_translation_unit = 79,          /* translation_unit  */
  YYSYMBOL_source_item = 80,               /* source_item  */
  YYSYMBOL_base_type = 81,                 /* base_type  */
  YYSYMBOL_array_decl = 82,                /* array_decl  */
  YYSYMBOL_type = 83,                      /* type  */
  YYSYMBOL_array_init = 84,                /* array_init  */
  YYSYMBOL_initializer = 85,               /* initializer  */
  YYSYMBOL_e_expr = 86,                    /* e_expr  */
  YYSYMBOL_expr = 87,                      /* expr  */
  YYSYMBOL_arg_list = 88,                  /* arg_list  */
  YYSYMBOL_init_decl = 89,                 /* init_decl  */
  YYSYMBOL_init_decl_list = 90,            /* init_decl_list  */
  YYSYMBOL_decl = 91,                      /* decl  */
  YYSYMBOL_param = 92,                     /* param  */
  YYSYMBOL_param_list = 93,                /* param_list  */
  YYSYMBOL_stmt = 94,                      /* stmt  */
  YYSYMBOL_stmt_list = 95,                 /* stmt_list  */
  YYSYMBOL_compound_stmt = 96,             /* compound_stmt  */
  YYSYMBOL_if_stmt = 97,                   /* if_stmt  */
  YYSYMBOL_while_stmt = 98,                /* while_stmt  */
  YYSYMBOL_do_while_stmt = 99,             /* do_while_stmt  */
  YYSYMBOL_for_stmt = 100,                 /* for_stmt  */
  YYSYMBOL_foreach_stmt = 101,             /* foreach_stmt  */
  YYSYMBOL_switch_stmt = 102,              /* switch_stmt  */
  YYSYMBOL_case_list = 103,                /* case_list  */
  YYSYMBOL_case_item = 104,                /* case_item  */
  YYSYMBOL_default_item = 105,             /* default_item  */
  YYSYMBOL_func_def = 106,                 /* func_def  */
  YYSYMBOL_func_body = 107,                /* func_body  */
  YYSYMBOL_class_def = 108,                /* class_def  */
  YYSYMBOL_class_members = 109,            /* class_members  */
  YYSYMBOL_class_member = 110,             /* class_member  */
  YYSYMBOL_access_spec = 111,              /* access_spec  */
  YYSYMBOL_method_def = 112,               /* method_def  */
  YYSYMBOL_ctor_def = 113,                 /* ctor_def  */
  YYSYMBOL_dtor_def = 114,                 /* dtor_def  */
  YYSYMBOL_enum_def = 115,                 /* enum_def  */
  YYSYMBOL_enum_body = 116                 /* enum_body  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  27
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1449

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  77
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  40
/* YYNRULES -- Number of rules.  */
#define YYNRULES  161
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  340

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   319


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      68,    69,     2,     2,    71,     2,    65,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    75,    72,
       2,    44,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    66,     2,    67,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    73,     2,    74,    76,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    70
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   133,   133,   134,   138,   139,   143,   144,   145,   146,
     150,   151,   152,   153,   154,   155,   156,   160,   161,   165,
     166,   167,   168,   172,   173,   177,   178,   182,   183,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   236,   237,   241,   242,   246,
     247,   251,   255,   256,   257,   258,   262,   263,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     282,   283,   287,   288,   292,   293,   297,   301,   305,   306,
     310,   311,   315,   316,   320,   321,   322,   323,   327,   328,
     332,   333,   337,   338,   339,   340,   344,   345,   349,   350,
     351,   352,   356,   357,   361,   362,   363,   364,   365,   366,
     367,   368,   372,   373,   374,   378,   379,   380,   381,   382,
     383,   384,   385,   389,   390,   394,   398,   399,   403,   404,
     405,   406
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "INT", "CHAR",
  "STRING", "BOOL", "FLOAT_TYPE", "DOUBLE", "REAL", "VOID", "IF", "ELSE",
  "SWITCH", "CASE", "DEFAULT", "WHILE", "DO", "FOR", "FOREACH", "RETURN",
  "BREAK", "CONTINUE", "CLASS", "OVERRIDE", "SUPER", "PUBLIC", "PRIVATE",
  "PROTECTED", "THIS", "REF", "NEW", "ENUM", "TRUE_VAL", "FALSE_VAL",
  "NULL_CONST", "NAN_CONST", "DOTDOT", "INTEGER", "FLOAT_VAL", "STR",
  "IDENT", "CLASSNAME", "CHARVAL", "'='", "PLUSEQ", "MINUSEQ", "STAREQ",
  "SLASHEQ", "TILDEQ", "LOR", "LAND", "EQ", "NEQ", "LT", "GT", "LE", "GE",
  "PLUS", "MINUS", "STAR", "SLASH", "UNOT", "UMINUS", "UPLUS", "'.'",
  "'['", "']'", "'('", "')'", "LOWER_THAN_ELSE", "','", "';'", "'{'",
  "'}'", "':'", "'~'", "$accept", "program", "translation_unit",
  "source_item", "base_type", "array_decl", "type", "array_init",
  "initializer", "e_expr", "expr", "arg_list", "init_decl",
  "init_decl_list", "decl", "param", "param_list", "stmt", "stmt_list",
  "compound_stmt", "if_stmt", "while_stmt", "do_while_stmt", "for_stmt",
  "foreach_stmt", "switch_stmt", "case_list", "case_item", "default_item",
  "func_def", "func_body", "class_def", "class_members", "class_member",
  "access_spec", "method_def", "ctor_def", "dtor_def", "enum_def",
  "enum_body", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-260)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-29)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     363,  -260,  -260,  -260,  -260,  -260,  -260,  -260,   -25,   -11,
     -30,   -13,    41,   363,  -260,   -13,    17,  -260,  -260,  -260,
    -260,    12,     0,    46,    47,   -16,  -260,  -260,  -260,  -260,
     -29,  -260,     5,    20,   268,    82,    47,    94,   -38,    -6,
    -260,   957,   322,    87,  -260,   169,    43,   100,  -260,   -34,
     113,   437,  -260,  -260,  -260,    89,  -260,   141,   143,  -260,
     277,  -260,  1074,  -260,  -260,  -260,  -260,   115,   -14,   153,
     161,  -260,  -260,   129,  -260,   256,  -260,  -260,  -260,  -260,
    -260,  -260,  -260,   142,  -260,  1033,  1033,  1033,   993,  1033,
    -260,  -260,  1383,    43,    15,   164,  -260,   173,  -260,   395,
    -260,  -260,   172,    43,   156,   151,   179,   183,   651,   158,
     -12,  -260,  -260,  -260,  -260,  -260,   308,  -260,  -260,   184,
     186,    19,   174,   793,   140,   140,   140,  -260,  1383,   -53,
    1129,  1033,  1033,  1033,  1033,  1033,  1033,  1033,  1033,  1033,
    1033,  1033,  1033,  1033,  1033,  1033,  1033,  1033,  1033,   198,
    1033,  -260,    43,   197,   176,   178,   182,   610,   185,   187,
    1033,   180,   195,  -260,    87,   196,  1383,  -260,  -260,   467,
    -260,  -260,  -260,  -260,  -260,  -260,  -260,  1033,  -260,  -260,
     658,   188,   201,   181,    63,   210,   698,  -260,   351,   250,
     221,  1033,   834,  1033,  -260,    64,  -260,  1033,  -260,  1383,
    1383,  1383,  1383,  1383,  1383,   534,   788,   177,   177,    52,
      52,    52,    52,    61,    61,   140,   140,   222,  1105,  -260,
    1033,  1033,  1033,  1033,   275,   757,   512,   227,  -260,  -260,
    -260,  -260,  -260,  1383,   181,    73,   705,   745,  -260,   181,
     181,   181,    76,  -260,  -260,   875,  1311,  -260,    77,  1335,
    -260,  1383,   916,  1033,  -260,  1383,  1155,  1181,  1207,   225,
     230,  1033,   235,   267,  -260,  -260,   181,   181,    84,   181,
      98,  -260,  -260,  -260,   181,  -260,   114,  -260,  -260,  -260,
    -260,   118,  1359,   610,   247,   610,  1033,  1033,   249,  1033,
     251,  -260,  -260,   181,  -260,   181,  -260,  -260,  -260,  -260,
     310,    -7,  -260,  1233,   252,  1033,  1259,  1033,  -260,  -260,
     610,  1033,   258,  -260,    -5,  -260,  -260,   266,  1033,   270,
     610,  1285,  -260,  1073,   539,  -260,  -260,  -260,  -260,   272,
     610,  -260,   610,   539,   539,   610,  -260,  -260,   539,  -260
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,    10,    11,    12,    13,    14,    15,    16,     0,     0,
       0,    21,     0,     2,     4,    19,     0,     8,     6,     7,
       9,     0,     0,     0,     0,     0,    22,     1,     5,    20,
      77,    79,     0,     0,     0,     0,     0,   158,     0,     0,
      17,     0,     0,     0,    81,     0,     0,     0,    86,     0,
       0,     0,   142,   143,   144,     0,   129,     0,     0,   134,
       0,   132,     0,   135,   138,   139,   140,     0,     0,     0,
       0,   157,    18,     0,    38,     0,    34,    35,    36,    37,
      30,    31,    33,    29,    32,     0,     0,     0,     0,     0,
      26,    78,    25,     0,     0,    77,    80,     0,   127,    28,
     126,   124,    82,     0,     0,     0,     0,     0,     0,     0,
      77,   128,   133,   136,   137,   141,     0,   156,   159,   160,
       0,     0,     0,     0,    55,    54,    56,    23,    75,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   122,     0,    83,     0,     0,     0,    28,     0,     0,
      28,     0,     0,   102,     0,     0,    27,    89,   100,    28,
      90,    91,    92,    93,    94,    95,    96,     0,   125,    87,
       0,     0,     0,     0,     0,     0,     0,   131,     0,     0,
      51,     0,     0,     0,    49,     0,    24,     0,    39,    69,
      70,    71,    72,    73,    74,    68,    67,    65,    66,    61,
      62,    63,    64,    59,    60,    57,    58,    46,     0,   123,
       0,     0,     0,     0,     0,    28,     0,     0,    98,    99,
      88,   103,   101,    84,     0,     0,     0,     0,   153,     0,
       0,     0,     0,   130,   161,     0,     0,    41,     0,     0,
      50,    76,     0,     0,    44,    85,     0,     0,     0,     0,
       0,    28,     0,     0,    97,   147,     0,     0,     0,     0,
       0,   154,   155,   145,     0,    52,     0,    40,    42,    43,
      47,     0,     0,    28,     0,    28,     0,    28,     0,     0,
       0,   148,   151,     0,   149,     0,   146,    53,    48,    45,
     104,     0,   106,     0,     0,    28,     0,     0,   152,   150,
      28,     0,     0,   113,     0,   114,   115,     0,    28,     0,
      28,     0,   105,     0,   120,   112,   116,   117,   107,     0,
      28,   110,    28,   118,   121,    28,   109,   111,   119,   108
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -260,  -260,  -260,   330,   -33,   331,    21,  -260,  -260,  -147,
     -41,  -122,   302,  -260,     4,   243,   -37,  -117,  -259,   -44,
    -260,  -260,  -260,  -260,  -260,  -260,  -260,    34,    35,  -260,
     -73,  -260,   246,   -57,  -260,   301,  -260,  -260,     6,   329
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,    12,    13,    14,    15,    26,   164,    90,    91,   165,
     166,   129,    31,    32,   167,    48,    49,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   314,   315,   316,    18,
     101,    19,    60,    61,    62,    63,    64,    65,    66,    38
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      92,   195,   100,   112,    17,    94,    20,   311,   312,   311,
     312,    23,    97,   227,   196,    41,    21,    17,   197,    20,
     151,    16,    39,     1,     2,     3,     4,     5,     6,     7,
     178,    22,    41,    70,    16,   103,    71,   104,    59,    42,
     224,    27,   122,    24,   124,   125,   126,   128,   130,   100,
      45,    40,   232,    25,    47,    58,   186,    70,    30,   100,
     117,    72,    11,    47,    59,   334,   113,   313,   115,   325,
     248,   184,   107,    34,   338,    35,    43,    44,   260,   219,
      33,    58,   128,    58,   152,   191,   104,   192,    37,    46,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   216,   100,   218,
     145,   146,   147,   148,   288,    98,    99,   149,   150,    36,
      59,   147,   148,   276,    67,    47,   149,   150,    95,    47,
     281,   112,   239,   250,   104,   197,   233,    58,    69,   238,
     304,   102,   266,   235,   104,   274,   278,   104,   197,   242,
     246,   128,   249,   293,   105,   104,   251,   108,   319,     1,
       2,     3,     4,     5,     6,     7,   300,   295,   302,   104,
     109,   329,     1,     2,     3,     4,     5,     6,     7,   255,
     256,   257,   258,   297,   110,   197,    45,   298,   116,   197,
     265,   118,    59,   322,   120,   271,   272,   273,    11,   268,
     270,    47,   119,   331,   128,   149,   150,    47,    41,    58,
     123,   128,   282,   336,   153,   337,   177,   232,   339,   180,
     181,   232,   291,   292,   182,   294,   185,   190,   189,   261,
     296,   141,   142,   143,   144,   145,   146,   147,   148,   217,
     193,   220,   149,   150,   221,   303,   222,   263,   306,   308,
     223,   309,   228,   225,    99,   226,   236,    47,    47,     1,
       2,     3,     4,     5,     6,     7,   321,   229,   230,   237,
     323,     1,     2,     3,     4,     5,     6,     7,    50,   240,
       1,     2,     3,     4,     5,     6,     7,    50,   244,   245,
     252,   259,    51,   286,    52,    53,    54,    55,   121,   264,
      10,    51,   287,    52,    53,    54,    55,   289,   290,    10,
      11,     1,     2,     3,     4,     5,     6,     7,    50,    11,
     301,   305,   310,   307,   318,     1,     2,     3,     4,     5,
       6,     7,    51,   324,    52,    53,    54,    55,   328,   330,
      10,   335,    56,    28,    57,    96,    29,   179,   326,   327,
      11,   111,    45,    57,     1,     2,     3,     4,     5,     6,
       7,    50,   188,   114,    11,    68,     1,     2,     3,     4,
       5,     6,     7,     8,     0,    51,     0,    52,    53,    54,
      55,     0,   187,    10,    57,     0,     9,     0,     0,     0,
       0,    93,     0,    11,     0,    10,     0,     0,     1,     2,
       3,     4,     5,     6,     7,    11,   154,     0,   155,     0,
       0,   156,   157,   158,   159,   160,   161,   162,     0,     0,
      73,     0,     0,     0,    74,   243,    75,    57,    76,    77,
      78,    79,     0,    80,    81,    82,    83,    11,    84,     0,
       1,     2,     3,     4,     5,     6,     7,   106,     0,     0,
       0,     0,     0,    85,    86,     0,     0,    87,     0,     0,
       0,     0,     0,    89,     0,     0,     0,     0,    99,   163,
       1,     2,     3,     4,     5,     6,     7,     0,   154,    11,
     155,     0,     0,   156,   157,   158,   159,   160,   161,   162,
       0,     0,    73,     0,     0,     0,    74,     0,    75,     0,
      76,    77,    78,    79,     0,    80,    81,    82,    83,    11,
      84,     0,     0,     0,     0,     1,     2,     3,     4,     5,
       6,     7,     0,     0,     0,    85,    86,     0,     0,    87,
       0,     0,     0,     0,     0,    89,     0,     0,     0,     0,
      99,   231,     1,     2,     3,     4,     5,     6,     7,     0,
     154,     0,   155,   262,    11,   156,   157,   158,   159,   160,
     161,   162,     0,     0,    73,     0,     0,     0,    74,     0,
      75,     0,    76,    77,    78,    79,     0,    80,    81,    82,
      83,    11,    84,     0,     0,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,     0,    85,    86,   149,
     150,    87,     0,     0,     0,     0,     0,    89,     0,     0,
       0,   -28,    99,     1,     2,     3,     4,     5,     6,     7,
       0,   154,     0,   155,     0,     0,   156,   157,   158,   159,
     160,   161,   162,     0,     0,    73,     0,     0,     0,    74,
       0,    75,     0,    76,    77,    78,    79,     0,    80,    81,
      82,    83,    11,    84,     1,     2,     3,     4,     5,     6,
       7,     1,     2,     3,     4,     5,     6,     7,    85,    86,
       0,     0,    87,     0,     0,     0,     0,     0,    89,     0,
       0,    45,     0,    99,     0,     0,     0,     0,    45,     0,
       0,     0,     0,    11,     0,     0,     0,     0,     0,     0,
      11,     1,     2,     3,     4,     5,     6,     7,     1,     2,
       3,     4,     5,     6,     7,     0,     0,     0,     0,     0,
     183,     0,     0,     0,     0,     0,     0,   234,    45,     0,
       0,     0,     0,     0,     0,    45,     0,     0,     0,     0,
      11,     0,     0,     0,     0,     0,     0,    11,     1,     2,
       3,     4,     5,     6,     7,     0,     0,     0,     0,     0,
       1,     2,     3,     4,     5,     6,     7,   241,     0,     0,
       0,     0,     0,     0,   267,    45,     0,     0,     0,     0,
       0,     0,    73,     0,     0,     0,    74,    11,    75,     0,
      76,    77,    78,    79,     0,    80,    81,    82,    83,    11,
      84,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   269,    85,    86,     0,    73,    87,
       0,     0,    74,     0,    75,    89,    76,    77,    78,    79,
       0,    80,    81,    82,    83,     0,    84,     0,     0,     0,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
       0,    85,    86,   149,   150,    87,     0,     0,     0,    73,
       0,    89,   194,    74,     0,    75,     0,    76,    77,    78,
      79,     0,    80,    81,    82,    83,     0,    84,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    85,    86,     0,     0,    87,     0,     0,     0,
      73,     0,    89,   247,    74,     0,    75,     0,    76,    77,
      78,    79,     0,    80,    81,    82,    83,     0,    84,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    85,    86,     0,     0,    87,     0,     0,
       0,    73,     0,    89,   275,    74,     0,    75,     0,    76,
      77,    78,    79,     0,    80,    81,    82,    83,     0,    84,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    85,    86,     0,     0,    87,     0,
       0,     0,    73,     0,    89,   280,    74,     0,    75,     0,
      76,    77,    78,    79,     0,    80,    81,    82,    83,     0,
      84,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    85,    86,     0,    73,    87,
       0,     0,    74,    88,    75,    89,    76,    77,    78,    79,
       0,    80,    81,    82,    83,     0,    84,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    85,    86,     0,     0,    87,     0,     0,    73,     0,
     127,    89,    74,     0,    75,     0,    76,    77,    78,    79,
       0,    80,    81,    82,    83,     0,    84,     1,     2,     3,
       4,     5,     6,     7,    50,     0,     0,     0,     0,     0,
       0,    85,    86,     0,     0,    87,     0,     0,    51,     0,
       0,    89,     0,     0,     0,     0,    10,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    11,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,     0,     0,     0,   149,   150,
       0,     0,   253,     0,     0,     0,     0,     0,   333,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,     0,     0,     0,
     149,   150,   254,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,     0,     0,     0,   149,   150,     0,     0,   198,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,     0,     0,     0,
     149,   150,     0,     0,   283,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,     0,     0,     0,   149,   150,     0,     0,
     284,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,     0,
       0,     0,   149,   150,     0,     0,   285,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,     0,     0,     0,   149,   150,
       0,     0,   317,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,     0,     0,     0,   149,   150,     0,     0,   320,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,     0,     0,     0,
     149,   150,     0,     0,   332,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,     0,     0,     0,   149,   150,   277,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,     0,     0,     0,
     149,   150,   279,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,     0,     0,     0,   149,   150,   299,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,     0,     0,     0,   149,   150
};

static const yytype_int16 yycheck[] =
{
      41,   123,    46,    60,     0,    42,     0,    14,    15,    14,
      15,    41,    45,   160,    67,    44,    41,    13,    71,    13,
      93,     0,    38,     3,     4,     5,     6,     7,     8,     9,
     103,    42,    44,    71,    13,    69,    74,    71,    34,    68,
     157,     0,    75,    73,    85,    86,    87,    88,    89,    93,
      30,    67,   169,    66,    33,    34,    68,    71,    41,   103,
      74,    67,    42,    42,    60,   324,    62,    74,    62,    74,
     192,   108,    51,    73,   333,    75,    71,    72,   225,   152,
      68,    60,   123,    62,    69,    66,    71,    68,    41,    69,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   152,   150,
      58,    59,    60,    61,   261,    72,    73,    65,    66,    73,
     116,    60,    61,   245,    42,   104,    65,    66,    41,   108,
     252,   188,    69,    69,    71,    71,   177,   116,    44,   183,
     287,    41,    69,   180,    71,    69,    69,    71,    71,   186,
     191,   192,   193,    69,    41,    71,   197,    68,   305,     3,
       4,     5,     6,     7,     8,     9,   283,    69,   285,    71,
      29,   318,     3,     4,     5,     6,     7,     8,     9,   220,
     221,   222,   223,    69,    41,    71,    30,    69,    73,    71,
     234,    38,   188,   310,    65,   239,   240,   241,    42,   236,
     237,   180,    41,   320,   245,    65,    66,   186,    44,   188,
      68,   252,   253,   330,    41,   332,    44,   334,   335,    68,
      41,   338,   266,   267,    41,   269,    68,    41,    44,   225,
     274,    54,    55,    56,    57,    58,    59,    60,    61,    41,
      66,    44,    65,    66,    68,   286,    68,   226,   289,   293,
      68,   295,    72,    68,    73,    68,    68,   236,   237,     3,
       4,     5,     6,     7,     8,     9,   307,    72,    72,    68,
     311,     3,     4,     5,     6,     7,     8,     9,    10,    69,
       3,     4,     5,     6,     7,     8,     9,    10,    38,    68,
      68,    16,    24,    68,    26,    27,    28,    29,    42,    72,
      32,    24,    72,    26,    27,    28,    29,    72,    41,    32,
      42,     3,     4,     5,     6,     7,     8,     9,    10,    42,
      73,    72,    12,    72,    72,     3,     4,     5,     6,     7,
       8,     9,    24,    75,    26,    27,    28,    29,    72,    69,
      32,    69,    74,    13,    76,    43,    15,   104,   314,   314,
      42,    74,    30,    76,     3,     4,     5,     6,     7,     8,
       9,    10,   116,    62,    42,    36,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    24,    -1,    26,    27,    28,
      29,    -1,    74,    32,    76,    -1,    23,    -1,    -1,    -1,
      -1,    69,    -1,    42,    -1,    32,    -1,    -1,     3,     4,
       5,     6,     7,     8,     9,    42,    11,    -1,    13,    -1,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      25,    -1,    -1,    -1,    29,    74,    31,    76,    33,    34,
      35,    36,    -1,    38,    39,    40,    41,    42,    43,    -1,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    -1,    -1,    58,    59,    -1,    -1,    62,    -1,    -1,
      -1,    -1,    -1,    68,    -1,    -1,    -1,    -1,    73,    74,
       3,     4,     5,     6,     7,     8,     9,    -1,    11,    42,
      13,    -1,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    25,    -1,    -1,    -1,    29,    -1,    31,    -1,
      33,    34,    35,    36,    -1,    38,    39,    40,    41,    42,
      43,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
       8,     9,    -1,    -1,    -1,    58,    59,    -1,    -1,    62,
      -1,    -1,    -1,    -1,    -1,    68,    -1,    -1,    -1,    -1,
      73,    74,     3,     4,     5,     6,     7,     8,     9,    -1,
      11,    -1,    13,    41,    42,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    25,    -1,    -1,    -1,    29,    -1,
      31,    -1,    33,    34,    35,    36,    -1,    38,    39,    40,
      41,    42,    43,    -1,    -1,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    -1,    58,    59,    65,
      66,    62,    -1,    -1,    -1,    -1,    -1,    68,    -1,    -1,
      -1,    72,    73,     3,     4,     5,     6,     7,     8,     9,
      -1,    11,    -1,    13,    -1,    -1,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    25,    -1,    -1,    -1,    29,
      -1,    31,    -1,    33,    34,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,     3,     4,     5,     6,     7,     8,
       9,     3,     4,     5,     6,     7,     8,     9,    58,    59,
      -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,    68,    -1,
      -1,    30,    -1,    73,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,    -1,
      42,     3,     4,     5,     6,     7,     8,     9,     3,     4,
       5,     6,     7,     8,     9,    -1,    -1,    -1,    -1,    -1,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    69,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      42,    -1,    -1,    -1,    -1,    -1,    -1,    42,     3,     4,
       5,     6,     7,     8,     9,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,     9,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    29,    42,    31,    -1,
      33,    34,    35,    36,    -1,    38,    39,    40,    41,    42,
      43,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    58,    59,    -1,    25,    62,
      -1,    -1,    29,    -1,    31,    68,    33,    34,    35,    36,
      -1,    38,    39,    40,    41,    -1,    43,    -1,    -1,    -1,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      -1,    58,    59,    65,    66,    62,    -1,    -1,    -1,    25,
      -1,    68,    69,    29,    -1,    31,    -1,    33,    34,    35,
      36,    -1,    38,    39,    40,    41,    -1,    43,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    58,    59,    -1,    -1,    62,    -1,    -1,    -1,
      25,    -1,    68,    69,    29,    -1,    31,    -1,    33,    34,
      35,    36,    -1,    38,    39,    40,    41,    -1,    43,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    58,    59,    -1,    -1,    62,    -1,    -1,
      -1,    25,    -1,    68,    69,    29,    -1,    31,    -1,    33,
      34,    35,    36,    -1,    38,    39,    40,    41,    -1,    43,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    58,    59,    -1,    -1,    62,    -1,
      -1,    -1,    25,    -1,    68,    69,    29,    -1,    31,    -1,
      33,    34,    35,    36,    -1,    38,    39,    40,    41,    -1,
      43,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    58,    59,    -1,    25,    62,
      -1,    -1,    29,    66,    31,    68,    33,    34,    35,    36,
      -1,    38,    39,    40,    41,    -1,    43,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    58,    59,    -1,    -1,    62,    -1,    -1,    25,    -1,
      67,    68,    29,    -1,    31,    -1,    33,    34,    35,    36,
      -1,    38,    39,    40,    41,    -1,    43,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    58,    59,    -1,    -1,    62,    -1,    -1,    24,    -1,
      -1,    68,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    42,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    -1,    -1,    -1,    65,    66,
      -1,    -1,    37,    -1,    -1,    -1,    -1,    -1,    75,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    -1,    -1,    -1,
      65,    66,    67,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    -1,    -1,    -1,    65,    66,    -1,    -1,    69,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    -1,    -1,    -1,
      65,    66,    -1,    -1,    69,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    -1,    -1,    -1,    65,    66,    -1,    -1,
      69,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    -1,
      -1,    -1,    65,    66,    -1,    -1,    69,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    -1,    -1,    -1,    65,    66,
      -1,    -1,    69,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    -1,    -1,    -1,    65,    66,    -1,    -1,    69,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    -1,    -1,    -1,
      65,    66,    -1,    -1,    69,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    -1,    -1,    -1,    65,    66,    67,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    -1,    -1,    -1,
      65,    66,    67,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    -1,    -1,    -1,    65,    66,    67,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    -1,    -1,    -1,    65,    66
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    23,
      32,    42,    78,    79,    80,    81,    83,    91,   106,   108,
     115,    41,    42,    41,    73,    66,    82,     0,    80,    82,
      41,    89,    90,    68,    73,    75,    73,    41,   116,    38,
      67,    44,    68,    71,    72,    30,    69,    83,    92,    93,
      10,    24,    26,    27,    28,    29,    74,    76,    83,    91,
     109,   110,   111,   112,   113,   114,   115,    42,   116,    44,
      71,    74,    67,    25,    29,    31,    33,    34,    35,    36,
      38,    39,    40,    41,    43,    58,    59,    62,    66,    68,
      84,    85,    87,    69,    93,    41,    89,    81,    72,    73,
      96,   107,    41,    69,    71,    41,    10,    83,    68,    29,
      41,    74,   110,    91,   112,   115,    73,    74,    38,    41,
      65,    42,    81,    68,    87,    87,    87,    67,    87,    88,
      87,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    65,
      66,   107,    69,    41,    11,    13,    16,    17,    18,    19,
      20,    21,    22,    74,    83,    86,    87,    91,    94,    95,
      96,    97,    98,    99,   100,   101,   102,    44,   107,    92,
      68,    41,    41,    69,    93,    68,    68,    74,   109,    44,
      41,    66,    68,    66,    69,    88,    67,    71,    69,    87,
      87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
      87,    87,    87,    87,    87,    87,    87,    41,    87,   107,
      44,    68,    68,    68,    94,    68,    68,    86,    72,    72,
      72,    74,    94,    87,    69,    93,    68,    68,    96,    69,
      69,    69,    93,    74,    38,    68,    87,    69,    88,    87,
      69,    87,    68,    37,    67,    87,    87,    87,    87,    16,
      86,    91,    41,    83,    72,    96,    69,    69,    93,    69,
      93,    96,    96,    96,    69,    69,    88,    67,    69,    67,
      69,    88,    87,    69,    69,    69,    68,    72,    86,    72,
      41,    96,    96,    69,    96,    69,    96,    69,    69,    67,
      94,    73,    94,    87,    86,    72,    87,    72,    96,    96,
      12,    14,    15,    74,   103,   104,   105,    69,    72,    86,
      69,    87,    94,    87,    75,    74,   104,   105,    72,    86,
      69,    94,    69,    75,    95,    69,    94,    94,    95,    94
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    77,    78,    78,    79,    79,    80,    80,    80,    80,
      81,    81,    81,    81,    81,    81,    81,    82,    82,    83,
      83,    83,    83,    84,    84,    85,    85,    86,    86,    87,
      87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
      87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
      87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
      87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
      87,    87,    87,    87,    87,    88,    88,    89,    89,    90,
      90,    91,    92,    92,    92,    92,    93,    93,    94,    94,
      94,    94,    94,    94,    94,    94,    94,    94,    94,    94,
      95,    95,    96,    96,    97,    97,    98,    99,   100,   100,
     101,   101,   102,   102,   103,   103,   103,   103,   104,   104,
     105,   105,   106,   106,   106,   106,   107,   107,   108,   108,
     108,   108,   109,   109,   110,   110,   110,   110,   110,   110,
     110,   110,   111,   111,   111,   112,   112,   112,   112,   112,
     112,   112,   112,   113,   113,   114,   115,   115,   116,   116,
     116,   116
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     3,     1,
       2,     1,     2,     2,     3,     1,     1,     1,     0,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       5,     4,     5,     5,     4,     6,     3,     5,     6,     3,
       4,     3,     5,     6,     2,     2,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     3,     1,     3,     1,
       3,     3,     2,     3,     4,     5,     1,     3,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     2,     2,
       1,     2,     2,     3,     5,     7,     5,     7,     9,     8,
       7,     8,     7,     6,     1,     1,     2,     2,     3,     4,
       2,     3,     5,     6,     5,     6,     1,     1,     5,     4,
       7,     6,     1,     2,     1,     1,     2,     2,     1,     1,
       1,     2,     1,     1,     1,     5,     6,     5,     6,     6,
       7,     6,     7,     4,     5,     5,     5,     4,     1,     3,
       3,     5
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: translation_unit  */
#line 133 "d_parser.y"
                       { (yyval.program) = root; }
#line 1641 "d_parser.tab.c"
    break;

  case 3: /* program: %empty  */
#line 134 "d_parser.y"
             { (yyval.program) = CreateProgram(NULL); root = (yyval.program); }
#line 1647 "d_parser.tab.c"
    break;

  case 4: /* translation_unit: source_item  */
#line 138 "d_parser.y"
                  { AppendSourceItemToRoot((yyvsp[0].source_item)); (yyval.source_item) = (yyvsp[0].source_item); }
#line 1653 "d_parser.tab.c"
    break;

  case 5: /* translation_unit: translation_unit source_item  */
#line 139 "d_parser.y"
                                   { AppendSourceItemToRoot((yyvsp[0].source_item)); (yyval.source_item) = (yyvsp[-1].source_item); }
#line 1659 "d_parser.tab.c"
    break;

  case 6: /* source_item: func_def  */
#line 143 "d_parser.y"
               { (yyval.source_item) = CreateFuncSourceItem((yyvsp[0].func_def)); }
#line 1665 "d_parser.tab.c"
    break;

  case 7: /* source_item: class_def  */
#line 144 "d_parser.y"
                { (yyval.source_item) = CreateClassSourceItem((yyvsp[0].class_def)); }
#line 1671 "d_parser.tab.c"
    break;

  case 8: /* source_item: decl  */
#line 145 "d_parser.y"
           { (yyval.source_item) = CreateDeclSourceItem((yyvsp[0].decl_stmt).type, (yyvsp[0].decl_stmt).init_decls); }
#line 1677 "d_parser.tab.c"
    break;

  case 9: /* source_item: enum_def  */
#line 146 "d_parser.y"
               { (yyval.source_item) = CreateEnumSourceItem((yyvsp[0].enum_def)); }
#line 1683 "d_parser.tab.c"
    break;

  case 10: /* base_type: INT  */
#line 150 "d_parser.y"
          { (yyval.type) = CreateBaseType(TYPE_INT); }
#line 1689 "d_parser.tab.c"
    break;

  case 11: /* base_type: CHAR  */
#line 151 "d_parser.y"
           { (yyval.type) = CreateBaseType(TYPE_CHAR); }
#line 1695 "d_parser.tab.c"
    break;

  case 12: /* base_type: STRING  */
#line 152 "d_parser.y"
             { (yyval.type) = CreateBaseType(TYPE_STRING); }
#line 1701 "d_parser.tab.c"
    break;

  case 13: /* base_type: BOOL  */
#line 153 "d_parser.y"
           { (yyval.type) = CreateBaseType(TYPE_BOOL); }
#line 1707 "d_parser.tab.c"
    break;

  case 14: /* base_type: FLOAT_TYPE  */
#line 154 "d_parser.y"
                 { (yyval.type) = CreateBaseType(TYPE_FLOAT); }
#line 1713 "d_parser.tab.c"
    break;

  case 15: /* base_type: DOUBLE  */
#line 155 "d_parser.y"
             { (yyval.type) = CreateBaseType(TYPE_DOUBLE); }
#line 1719 "d_parser.tab.c"
    break;

  case 16: /* base_type: REAL  */
#line 156 "d_parser.y"
           { (yyval.type) = CreateBaseType(TYPE_REAL); }
#line 1725 "d_parser.tab.c"
    break;

  case 17: /* array_decl: '[' ']'  */
#line 160 "d_parser.y"
              { (yyval.array_decl) = CreateArrayDecl(0, 0); }
#line 1731 "d_parser.tab.c"
    break;

  case 18: /* array_decl: '[' INTEGER ']'  */
#line 161 "d_parser.y"
                      { (yyval.array_decl) = CreateArrayDecl(1, (yyvsp[-1].int_val)); }
#line 1737 "d_parser.tab.c"
    break;

  case 19: /* type: base_type  */
#line 165 "d_parser.y"
                { (yyval.type) = (yyvsp[0].type); }
#line 1743 "d_parser.tab.c"
    break;

  case 20: /* type: base_type array_decl  */
#line 166 "d_parser.y"
                           { (yyval.type) = AddArrayToType((yyvsp[-1].type), (yyvsp[0].array_decl)); }
#line 1749 "d_parser.tab.c"
    break;

  case 21: /* type: CLASSNAME  */
#line 167 "d_parser.y"
                { (yyval.type) = CreateClassType((yyvsp[0].string_val)); }
#line 1755 "d_parser.tab.c"
    break;

  case 22: /* type: CLASSNAME array_decl  */
#line 168 "d_parser.y"
                           { (yyval.type) = AddArrayToType(CreateClassType((yyvsp[-1].string_val)), (yyvsp[0].array_decl)); }
#line 1761 "d_parser.tab.c"
    break;

  case 23: /* array_init: '[' ']'  */
#line 172 "d_parser.y"
              { (yyval.initializer) = CreateArrayInitializer(NULL, 0); }
#line 1767 "d_parser.tab.c"
    break;

  case 24: /* array_init: '[' arg_list ']'  */
#line 173 "d_parser.y"
                       { (yyval.initializer) = CreateArrayInitializer((yyvsp[-1].expr_list)->elements, (yyvsp[-1].expr_list)->count); }
#line 1773 "d_parser.tab.c"
    break;

  case 25: /* initializer: expr  */
#line 177 "d_parser.y"
           { (yyval.initializer) = CreateExprInitializer((yyvsp[0].expr)); }
#line 1779 "d_parser.tab.c"
    break;

  case 26: /* initializer: array_init  */
#line 178 "d_parser.y"
                 { (yyval.initializer) = (yyvsp[0].initializer); }
#line 1785 "d_parser.tab.c"
    break;

  case 27: /* e_expr: expr  */
#line 182 "d_parser.y"
           { (yyval.expr) = (yyvsp[0].expr); }
#line 1791 "d_parser.tab.c"
    break;

  case 28: /* e_expr: %empty  */
#line 183 "d_parser.y"
             { (yyval.expr) = NULL; }
#line 1797 "d_parser.tab.c"
    break;

  case 29: /* expr: IDENT  */
#line 187 "d_parser.y"
            { (yyval.expr) = CreateIdentExpr((yyvsp[0].string_val)); }
#line 1803 "d_parser.tab.c"
    break;

  case 30: /* expr: INTEGER  */
#line 188 "d_parser.y"
              { (yyval.expr) = CreateIntExpr((yyvsp[0].int_val)); }
#line 1809 "d_parser.tab.c"
    break;

  case 31: /* expr: FLOAT_VAL  */
#line 189 "d_parser.y"
                { (yyval.expr) = CreateFloatExpr((yyvsp[0].float_val)); }
#line 1815 "d_parser.tab.c"
    break;

  case 32: /* expr: CHARVAL  */
#line 190 "d_parser.y"
              { (yyval.expr) = CreateCharExpr((yyvsp[0].char_val)); }
#line 1821 "d_parser.tab.c"
    break;

  case 33: /* expr: STR  */
#line 191 "d_parser.y"
          { (yyval.expr) = CreateStringExpr((yyvsp[0].string_val)); }
#line 1827 "d_parser.tab.c"
    break;

  case 34: /* expr: TRUE_VAL  */
#line 192 "d_parser.y"
               { (yyval.expr) = CreateBoolExpr(1); }
#line 1833 "d_parser.tab.c"
    break;

  case 35: /* expr: FALSE_VAL  */
#line 193 "d_parser.y"
                { (yyval.expr) = CreateBoolExpr(0); }
#line 1839 "d_parser.tab.c"
    break;

  case 36: /* expr: NULL_CONST  */
#line 194 "d_parser.y"
                 { (yyval.expr) = CreateNullExpr(); }
#line 1845 "d_parser.tab.c"
    break;

  case 37: /* expr: NAN_CONST  */
#line 195 "d_parser.y"
                { (yyval.expr) = CreateNanExpr(); }
#line 1851 "d_parser.tab.c"
    break;

  case 38: /* expr: THIS  */
#line 196 "d_parser.y"
           { (yyval.expr) = CreateThisExpr(); }
#line 1857 "d_parser.tab.c"
    break;

  case 39: /* expr: '(' expr ')'  */
#line 197 "d_parser.y"
                   { (yyval.expr) = CreateParenExpr((yyvsp[-1].expr)); }
#line 1863 "d_parser.tab.c"
    break;

  case 40: /* expr: NEW CLASSNAME '[' expr ']'  */
#line 198 "d_parser.y"
                                 { (yyval.expr) = CreateNewArrayExpr(CreateClassType((yyvsp[-3].string_val)), (yyvsp[-1].expr)); }
#line 1869 "d_parser.tab.c"
    break;

  case 41: /* expr: NEW CLASSNAME '(' ')'  */
#line 199 "d_parser.y"
                            { (yyval.expr) = CreateNewExpr(CreateClassType((yyvsp[-2].string_val)), NULL, 0); }
#line 1875 "d_parser.tab.c"
    break;

  case 42: /* expr: NEW CLASSNAME '(' arg_list ')'  */
#line 200 "d_parser.y"
                                     { (yyval.expr) = CreateNewExpr(CreateClassType((yyvsp[-3].string_val)), (yyvsp[-1].expr_list)->elements, (yyvsp[-1].expr_list)->count); }
#line 1881 "d_parser.tab.c"
    break;

  case 43: /* expr: NEW base_type '[' expr ']'  */
#line 201 "d_parser.y"
                                 { (yyval.expr) = CreateNewArrayExpr((yyvsp[-3].type), (yyvsp[-1].expr)); }
#line 1887 "d_parser.tab.c"
    break;

  case 44: /* expr: expr '[' expr ']'  */
#line 202 "d_parser.y"
                        { (yyval.expr) = CreateArrayAccessExpr((yyvsp[-3].expr), (yyvsp[-1].expr), NULL); }
#line 1893 "d_parser.tab.c"
    break;

  case 45: /* expr: expr '[' expr DOTDOT expr ']'  */
#line 203 "d_parser.y"
                                    { (yyval.expr) = CreateArrayAccessExpr((yyvsp[-5].expr), (yyvsp[-3].expr), (yyvsp[-1].expr)); }
#line 1899 "d_parser.tab.c"
    break;

  case 46: /* expr: expr '.' IDENT  */
#line 204 "d_parser.y"
                     { (yyval.expr) = CreateMemberAccessExpr((yyvsp[-2].expr), (yyvsp[0].string_val)); }
#line 1905 "d_parser.tab.c"
    break;

  case 47: /* expr: expr '.' IDENT '(' ')'  */
#line 205 "d_parser.y"
                             { (yyval.expr) = CreateMethodCallExpr((yyvsp[-4].expr), (yyvsp[-2].string_val), NULL, 0); }
#line 1911 "d_parser.tab.c"
    break;

  case 48: /* expr: expr '.' IDENT '(' arg_list ')'  */
#line 206 "d_parser.y"
                                      { (yyval.expr) = CreateMethodCallExpr((yyvsp[-5].expr), (yyvsp[-3].string_val), (yyvsp[-1].expr_list)->elements, (yyvsp[-1].expr_list)->count); }
#line 1917 "d_parser.tab.c"
    break;

  case 49: /* expr: IDENT '(' ')'  */
#line 207 "d_parser.y"
                    { (yyval.expr) = CreateFuncCallExpr((yyvsp[-2].string_val), NULL, 0); }
#line 1923 "d_parser.tab.c"
    break;

  case 50: /* expr: IDENT '(' arg_list ')'  */
#line 208 "d_parser.y"
                             { (yyval.expr) = CreateFuncCallExpr((yyvsp[-3].string_val), (yyvsp[-1].expr_list)->elements, (yyvsp[-1].expr_list)->count); }
#line 1929 "d_parser.tab.c"
    break;

  case 51: /* expr: SUPER '.' IDENT  */
#line 209 "d_parser.y"
                      { (yyval.expr) = CreateSuperExpr((yyvsp[0].string_val)); }
#line 1935 "d_parser.tab.c"
    break;

  case 52: /* expr: SUPER '.' IDENT '(' ')'  */
#line 210 "d_parser.y"
                              { (yyval.expr) = CreateSuperMethodCallExpr((yyvsp[-2].string_val), NULL, 0); }
#line 1941 "d_parser.tab.c"
    break;

  case 53: /* expr: SUPER '.' IDENT '(' arg_list ')'  */
#line 211 "d_parser.y"
                                       { (yyval.expr) = CreateSuperMethodCallExpr((yyvsp[-3].string_val), (yyvsp[-1].expr_list)->elements, (yyvsp[-1].expr_list)->count); }
#line 1947 "d_parser.tab.c"
    break;

  case 54: /* expr: MINUS expr  */
#line 212 "d_parser.y"
                              { (yyval.expr) = CreateUnaryOpExpr(OP_MINUS, (yyvsp[0].expr)); }
#line 1953 "d_parser.tab.c"
    break;

  case 55: /* expr: PLUS expr  */
#line 213 "d_parser.y"
                            { (yyval.expr) = CreateUnaryOpExpr(OP_PLUS, (yyvsp[0].expr)); }
#line 1959 "d_parser.tab.c"
    break;

  case 56: /* expr: UNOT expr  */
#line 214 "d_parser.y"
                           { (yyval.expr) = CreateUnaryOpExpr(OP_NOT, (yyvsp[0].expr)); }
#line 1965 "d_parser.tab.c"
    break;

  case 57: /* expr: expr STAR expr  */
#line 215 "d_parser.y"
                     { (yyval.expr) = CreateBinaryOpExpr(OP_MUL, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1971 "d_parser.tab.c"
    break;

  case 58: /* expr: expr SLASH expr  */
#line 216 "d_parser.y"
                      { (yyval.expr) = CreateBinaryOpExpr(OP_DIV, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1977 "d_parser.tab.c"
    break;

  case 59: /* expr: expr PLUS expr  */
#line 217 "d_parser.y"
                     { (yyval.expr) = CreateBinaryOpExpr(OP_PLUS, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1983 "d_parser.tab.c"
    break;

  case 60: /* expr: expr MINUS expr  */
#line 218 "d_parser.y"
                      { (yyval.expr) = CreateBinaryOpExpr(OP_MINUS, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1989 "d_parser.tab.c"
    break;

  case 61: /* expr: expr LT expr  */
#line 219 "d_parser.y"
                   { (yyval.expr) = CreateBinaryOpExpr(OP_LT, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1995 "d_parser.tab.c"
    break;

  case 62: /* expr: expr GT expr  */
#line 220 "d_parser.y"
                   { (yyval.expr) = CreateBinaryOpExpr(OP_GT, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2001 "d_parser.tab.c"
    break;

  case 63: /* expr: expr LE expr  */
#line 221 "d_parser.y"
                   { (yyval.expr) = CreateBinaryOpExpr(OP_LE, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2007 "d_parser.tab.c"
    break;

  case 64: /* expr: expr GE expr  */
#line 222 "d_parser.y"
                   { (yyval.expr) = CreateBinaryOpExpr(OP_GE, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2013 "d_parser.tab.c"
    break;

  case 65: /* expr: expr EQ expr  */
#line 223 "d_parser.y"
                   { (yyval.expr) = CreateBinaryOpExpr(OP_EQ, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2019 "d_parser.tab.c"
    break;

  case 66: /* expr: expr NEQ expr  */
#line 224 "d_parser.y"
                    { (yyval.expr) = CreateBinaryOpExpr(OP_NEQ, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2025 "d_parser.tab.c"
    break;

  case 67: /* expr: expr LAND expr  */
#line 225 "d_parser.y"
                     { (yyval.expr) = CreateBinaryOpExpr(OP_AND, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2031 "d_parser.tab.c"
    break;

  case 68: /* expr: expr LOR expr  */
#line 226 "d_parser.y"
                    { (yyval.expr) = CreateBinaryOpExpr(OP_OR, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2037 "d_parser.tab.c"
    break;

  case 69: /* expr: expr '=' expr  */
#line 227 "d_parser.y"
                    { (yyval.expr) = CreateAssignExpr(OP_ASSIGN, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2043 "d_parser.tab.c"
    break;

  case 70: /* expr: expr PLUSEQ expr  */
#line 228 "d_parser.y"
                       { (yyval.expr) = CreateAssignExpr(OP_PLUS_ASSIGN, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2049 "d_parser.tab.c"
    break;

  case 71: /* expr: expr MINUSEQ expr  */
#line 229 "d_parser.y"
                        { (yyval.expr) = CreateAssignExpr(OP_MINUS_ASSIGN, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2055 "d_parser.tab.c"
    break;

  case 72: /* expr: expr STAREQ expr  */
#line 230 "d_parser.y"
                       { (yyval.expr) = CreateAssignExpr(OP_MUL_ASSIGN, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2061 "d_parser.tab.c"
    break;

  case 73: /* expr: expr SLASHEQ expr  */
#line 231 "d_parser.y"
                        { (yyval.expr) = CreateAssignExpr(OP_DIV_ASSIGN, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2067 "d_parser.tab.c"
    break;

  case 74: /* expr: expr TILDEQ expr  */
#line 232 "d_parser.y"
                       { (yyval.expr) = CreateAssignExpr(OP_BITWISE_NOT_ASSIGN, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2073 "d_parser.tab.c"
    break;

  case 75: /* arg_list: expr  */
#line 236 "d_parser.y"
           { (yyval.expr_list) = CreateExprList(); AddExprToList((yyval.expr_list), (yyvsp[0].expr)); }
#line 2079 "d_parser.tab.c"
    break;

  case 76: /* arg_list: arg_list ',' expr  */
#line 237 "d_parser.y"
                        { (yyval.expr_list) = (yyvsp[-2].expr_list); AddExprToList((yyval.expr_list), (yyvsp[0].expr)); }
#line 2085 "d_parser.tab.c"
    break;

  case 77: /* init_decl: IDENT  */
#line 241 "d_parser.y"
            { (yyval.init_decl) = CreateInitDecl((yyvsp[0].string_val), NULL); }
#line 2091 "d_parser.tab.c"
    break;

  case 78: /* init_decl: IDENT '=' initializer  */
#line 242 "d_parser.y"
                            { (yyval.init_decl) = CreateInitDecl((yyvsp[-2].string_val), (yyvsp[0].initializer)); }
#line 2097 "d_parser.tab.c"
    break;

  case 79: /* init_decl_list: init_decl  */
#line 246 "d_parser.y"
                { (yyval.init_decl_list) = CreateInitDeclList(); AddInitDeclToList((yyval.init_decl_list), (yyvsp[0].init_decl)); }
#line 2103 "d_parser.tab.c"
    break;

  case 80: /* init_decl_list: init_decl_list ',' init_decl  */
#line 247 "d_parser.y"
                                   { (yyval.init_decl_list) = (yyvsp[-2].init_decl_list); AddInitDeclToList((yyval.init_decl_list), (yyvsp[0].init_decl)); }
#line 2109 "d_parser.tab.c"
    break;

  case 81: /* decl: type init_decl_list ';'  */
#line 251 "d_parser.y"
                              { (yyval.decl_stmt).type = (yyvsp[-2].type); (yyval.decl_stmt).init_decls = (yyvsp[-1].init_decl_list); }
#line 2115 "d_parser.tab.c"
    break;

  case 82: /* param: type IDENT  */
#line 255 "d_parser.y"
                 { (yyval.param) = CreateParam((yyvsp[-1].type), (yyvsp[0].string_val), 0, NULL); }
#line 2121 "d_parser.tab.c"
    break;

  case 83: /* param: REF base_type IDENT  */
#line 256 "d_parser.y"
                          { (yyval.param) = CreateParam((yyvsp[-1].type), (yyvsp[0].string_val), 1, NULL); }
#line 2127 "d_parser.tab.c"
    break;

  case 84: /* param: type IDENT '=' expr  */
#line 257 "d_parser.y"
                          { (yyval.param) = CreateParam((yyvsp[-3].type), (yyvsp[-2].string_val), 0, (yyvsp[0].expr)); }
#line 2133 "d_parser.tab.c"
    break;

  case 85: /* param: REF base_type IDENT '=' expr  */
#line 258 "d_parser.y"
                                   { (yyval.param) = CreateParam((yyvsp[-3].type), (yyvsp[-2].string_val), 1, (yyvsp[0].expr)); }
#line 2139 "d_parser.tab.c"
    break;

  case 86: /* param_list: param  */
#line 262 "d_parser.y"
            { (yyval.param_list) = CreateParamList(); AddParamToList((yyval.param_list), (yyvsp[0].param)); }
#line 2145 "d_parser.tab.c"
    break;

  case 87: /* param_list: param_list ',' param  */
#line 263 "d_parser.y"
                           { (yyval.param_list) = (yyvsp[-2].param_list); AddParamToList((yyval.param_list), (yyvsp[0].param)); }
#line 2151 "d_parser.tab.c"
    break;

  case 88: /* stmt: e_expr ';'  */
#line 267 "d_parser.y"
                 { (yyval.stmt) = CreateExprStmt((yyvsp[-1].expr)); }
#line 2157 "d_parser.tab.c"
    break;

  case 89: /* stmt: decl  */
#line 268 "d_parser.y"
           { (yyval.stmt) = CreateDeclStmt((yyvsp[0].decl_stmt).type, (yyvsp[0].decl_stmt).init_decls); }
#line 2163 "d_parser.tab.c"
    break;

  case 90: /* stmt: compound_stmt  */
#line 269 "d_parser.y"
                    { (yyval.stmt) = (yyvsp[0].stmt); }
#line 2169 "d_parser.tab.c"
    break;

  case 91: /* stmt: if_stmt  */
#line 270 "d_parser.y"
              { (yyval.stmt) = (yyvsp[0].stmt); }
#line 2175 "d_parser.tab.c"
    break;

  case 92: /* stmt: while_stmt  */
#line 271 "d_parser.y"
                 { (yyval.stmt) = (yyvsp[0].stmt); }
#line 2181 "d_parser.tab.c"
    break;

  case 93: /* stmt: do_while_stmt  */
#line 272 "d_parser.y"
                    { (yyval.stmt) = (yyvsp[0].stmt); }
#line 2187 "d_parser.tab.c"
    break;

  case 94: /* stmt: for_stmt  */
#line 273 "d_parser.y"
               { (yyval.stmt) = (yyvsp[0].stmt); }
#line 2193 "d_parser.tab.c"
    break;

  case 95: /* stmt: foreach_stmt  */
#line 274 "d_parser.y"
                   { (yyval.stmt) = (yyvsp[0].stmt); }
#line 2199 "d_parser.tab.c"
    break;

  case 96: /* stmt: switch_stmt  */
#line 275 "d_parser.y"
                  { (yyval.stmt) = (yyvsp[0].stmt); }
#line 2205 "d_parser.tab.c"
    break;

  case 97: /* stmt: RETURN e_expr ';'  */
#line 276 "d_parser.y"
                        { (yyval.stmt) = CreateReturnStmt((yyvsp[-1].expr)); }
#line 2211 "d_parser.tab.c"
    break;

  case 98: /* stmt: BREAK ';'  */
#line 277 "d_parser.y"
                { (yyval.stmt) = CreateBreakStmt(); }
#line 2217 "d_parser.tab.c"
    break;

  case 99: /* stmt: CONTINUE ';'  */
#line 278 "d_parser.y"
                   { (yyval.stmt) = CreateContinueStmt(); }
#line 2223 "d_parser.tab.c"
    break;

  case 100: /* stmt_list: stmt  */
#line 282 "d_parser.y"
           { (yyval.stmt_list) = CreateStmtList(); AddStmtToList((yyval.stmt_list), (yyvsp[0].stmt)); }
#line 2229 "d_parser.tab.c"
    break;

  case 101: /* stmt_list: stmt_list stmt  */
#line 283 "d_parser.y"
                     { (yyval.stmt_list) = (yyvsp[-1].stmt_list); AddStmtToList((yyval.stmt_list), (yyvsp[0].stmt)); }
#line 2235 "d_parser.tab.c"
    break;

  case 102: /* compound_stmt: '{' '}'  */
#line 287 "d_parser.y"
              { (yyval.stmt) = CreateCompoundStmt(NULL); }
#line 2241 "d_parser.tab.c"
    break;

  case 103: /* compound_stmt: '{' stmt_list '}'  */
#line 288 "d_parser.y"
                        { (yyval.stmt) = CreateCompoundStmt((yyvsp[-1].stmt_list)); }
#line 2247 "d_parser.tab.c"
    break;

  case 104: /* if_stmt: IF '(' expr ')' stmt  */
#line 292 "d_parser.y"
                                                 { (yyval.stmt) = CreateIfStmt((yyvsp[-2].expr), (yyvsp[0].stmt), NULL); }
#line 2253 "d_parser.tab.c"
    break;

  case 105: /* if_stmt: IF '(' expr ')' stmt ELSE stmt  */
#line 293 "d_parser.y"
                                     { (yyval.stmt) = CreateIfStmt((yyvsp[-4].expr), (yyvsp[-2].stmt), (yyvsp[0].stmt)); }
#line 2259 "d_parser.tab.c"
    break;

  case 106: /* while_stmt: WHILE '(' expr ')' stmt  */
#line 297 "d_parser.y"
                              { (yyval.stmt) = CreateWhileStmt((yyvsp[-2].expr), (yyvsp[0].stmt)); }
#line 2265 "d_parser.tab.c"
    break;

  case 107: /* do_while_stmt: DO stmt WHILE '(' expr ')' ';'  */
#line 301 "d_parser.y"
                                     { (yyval.stmt) = CreateDoWhileStmt((yyvsp[-5].stmt), (yyvsp[-2].expr)); }
#line 2271 "d_parser.tab.c"
    break;

  case 108: /* for_stmt: FOR '(' e_expr ';' e_expr ';' e_expr ')' stmt  */
#line 305 "d_parser.y"
                                                    { (yyval.stmt) = CreateForStmt((yyvsp[-6].expr), NULL, NULL, (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].stmt)); }
#line 2277 "d_parser.tab.c"
    break;

  case 109: /* for_stmt: FOR '(' decl e_expr ';' e_expr ')' stmt  */
#line 306 "d_parser.y"
                                              { (yyval.stmt) = CreateForStmt(NULL, (yyvsp[-5].decl_stmt).type, (yyvsp[-5].decl_stmt).init_decls, (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].stmt)); }
#line 2283 "d_parser.tab.c"
    break;

  case 110: /* foreach_stmt: FOREACH '(' IDENT ';' expr ')' stmt  */
#line 310 "d_parser.y"
                                          { (yyval.stmt) = CreateForeachStmt(0, NULL, (yyvsp[-4].string_val), (yyvsp[-2].expr), (yyvsp[0].stmt)); }
#line 2289 "d_parser.tab.c"
    break;

  case 111: /* foreach_stmt: FOREACH '(' type IDENT ';' expr ')' stmt  */
#line 311 "d_parser.y"
                                               { (yyval.stmt) = CreateForeachStmt(1, (yyvsp[-5].type), (yyvsp[-4].string_val), (yyvsp[-2].expr), (yyvsp[0].stmt)); }
#line 2295 "d_parser.tab.c"
    break;

  case 112: /* switch_stmt: SWITCH '(' expr ')' '{' case_list '}'  */
#line 315 "d_parser.y"
                                            { (yyval.stmt) = CreateSwitchStmt((yyvsp[-4].expr), (yyvsp[-1].case_list)->items, (yyvsp[-1].case_list)->count); }
#line 2301 "d_parser.tab.c"
    break;

  case 113: /* switch_stmt: SWITCH '(' expr ')' '{' '}'  */
#line 316 "d_parser.y"
                                  { (yyval.stmt) = CreateSwitchStmt((yyvsp[-3].expr), NULL, 0); }
#line 2307 "d_parser.tab.c"
    break;

  case 114: /* case_list: case_item  */
#line 320 "d_parser.y"
                { (yyval.case_list) = CreateCaseList(); AddCaseItemToList((yyval.case_list), (yyvsp[0].case_item)); }
#line 2313 "d_parser.tab.c"
    break;

  case 115: /* case_list: default_item  */
#line 321 "d_parser.y"
                   { (yyval.case_list) = CreateCaseList(); AddCaseItemToList((yyval.case_list), (yyvsp[0].case_item)); }
#line 2319 "d_parser.tab.c"
    break;

  case 116: /* case_list: case_list case_item  */
#line 322 "d_parser.y"
                          { (yyval.case_list) = (yyvsp[-1].case_list); AddCaseItemToList((yyval.case_list), (yyvsp[0].case_item)); }
#line 2325 "d_parser.tab.c"
    break;

  case 117: /* case_list: case_list default_item  */
#line 323 "d_parser.y"
                             { (yyval.case_list) = (yyvsp[-1].case_list); AddCaseItemToList((yyval.case_list), (yyvsp[0].case_item)); }
#line 2331 "d_parser.tab.c"
    break;

  case 118: /* case_item: CASE expr ':'  */
#line 327 "d_parser.y"
                    { (yyval.case_item) = CreateCaseItem((yyvsp[-1].expr), NULL); }
#line 2337 "d_parser.tab.c"
    break;

  case 119: /* case_item: CASE expr ':' stmt_list  */
#line 328 "d_parser.y"
                              { (yyval.case_item) = CreateCaseItem((yyvsp[-2].expr), (yyvsp[0].stmt_list)); }
#line 2343 "d_parser.tab.c"
    break;

  case 120: /* default_item: DEFAULT ':'  */
#line 332 "d_parser.y"
                  { (yyval.case_item) = CreateDefaultItem(NULL); }
#line 2349 "d_parser.tab.c"
    break;

  case 121: /* default_item: DEFAULT ':' stmt_list  */
#line 333 "d_parser.y"
                            { (yyval.case_item) = CreateDefaultItem((yyvsp[0].stmt_list)); }
#line 2355 "d_parser.tab.c"
    break;

  case 122: /* func_def: type IDENT '(' ')' func_body  */
#line 337 "d_parser.y"
                                   { (yyval.func_def) = CreateFuncDef((yyvsp[-4].type), (yyvsp[-3].string_val), NULL, (yyvsp[0].stmt)); }
#line 2361 "d_parser.tab.c"
    break;

  case 123: /* func_def: type IDENT '(' param_list ')' func_body  */
#line 338 "d_parser.y"
                                              { (yyval.func_def) = CreateFuncDef((yyvsp[-5].type), (yyvsp[-4].string_val), (yyvsp[-2].param_list), (yyvsp[0].stmt)); }
#line 2367 "d_parser.tab.c"
    break;

  case 124: /* func_def: VOID IDENT '(' ')' func_body  */
#line 339 "d_parser.y"
                                   { (yyval.func_def) = CreateVoidFuncDef((yyvsp[-3].string_val), NULL, (yyvsp[0].stmt)); }
#line 2373 "d_parser.tab.c"
    break;

  case 125: /* func_def: VOID IDENT '(' param_list ')' func_body  */
#line 340 "d_parser.y"
                                              { (yyval.func_def) = CreateVoidFuncDef((yyvsp[-4].string_val), (yyvsp[-2].param_list), (yyvsp[0].stmt)); }
#line 2379 "d_parser.tab.c"
    break;

  case 126: /* func_body: compound_stmt  */
#line 344 "d_parser.y"
                    { (yyval.stmt) = (yyvsp[0].stmt); }
#line 2385 "d_parser.tab.c"
    break;

  case 127: /* func_body: ';'  */
#line 345 "d_parser.y"
          { (yyval.stmt) = NULL; }
#line 2391 "d_parser.tab.c"
    break;

  case 128: /* class_def: CLASS CLASSNAME '{' class_members '}'  */
#line 349 "d_parser.y"
                                            { (yyval.class_def) = CreateClassDef((yyvsp[-3].string_val), NULL, (yyvsp[-1].class_member)); }
#line 2397 "d_parser.tab.c"
    break;

  case 129: /* class_def: CLASS CLASSNAME '{' '}'  */
#line 350 "d_parser.y"
                              { (yyval.class_def) = CreateClassDef((yyvsp[-2].string_val), NULL, NULL); }
#line 2403 "d_parser.tab.c"
    break;

  case 130: /* class_def: CLASS CLASSNAME ':' CLASSNAME '{' class_members '}'  */
#line 351 "d_parser.y"
                                                          { (yyval.class_def) = CreateClassDef((yyvsp[-5].string_val), (yyvsp[-3].string_val), (yyvsp[-1].class_member)); }
#line 2409 "d_parser.tab.c"
    break;

  case 131: /* class_def: CLASS CLASSNAME ':' CLASSNAME '{' '}'  */
#line 352 "d_parser.y"
                                            { (yyval.class_def) = CreateClassDef((yyvsp[-4].string_val), (yyvsp[-2].string_val), NULL); }
#line 2415 "d_parser.tab.c"
    break;

  case 132: /* class_members: class_member  */
#line 356 "d_parser.y"
                   { (yyval.class_member) = (yyvsp[0].class_member); }
#line 2421 "d_parser.tab.c"
    break;

  case 133: /* class_members: class_members class_member  */
#line 357 "d_parser.y"
                                 { (yyval.class_member) = AppendClassMember((yyvsp[-1].class_member), (yyvsp[0].class_member)); }
#line 2427 "d_parser.tab.c"
    break;

  case 134: /* class_member: decl  */
#line 361 "d_parser.y"
           { (yyval.class_member) = CreateFieldMember(ACCESS_PUBLIC, (yyvsp[0].decl_stmt).type, (yyvsp[0].decl_stmt).init_decls); }
#line 2433 "d_parser.tab.c"
    break;

  case 135: /* class_member: method_def  */
#line 362 "d_parser.y"
                 { (yyval.class_member) = CreateMethodMember(ACCESS_PUBLIC, (yyvsp[0].method_def)); }
#line 2439 "d_parser.tab.c"
    break;

  case 136: /* class_member: access_spec decl  */
#line 363 "d_parser.y"
                       { (yyval.class_member) = CreateFieldMember((yyvsp[-1].access_spec), (yyvsp[0].decl_stmt).type, (yyvsp[0].decl_stmt).init_decls); }
#line 2445 "d_parser.tab.c"
    break;

  case 137: /* class_member: access_spec method_def  */
#line 364 "d_parser.y"
                             { (yyval.class_member) = CreateMethodMember((yyvsp[-1].access_spec), (yyvsp[0].method_def)); }
#line 2451 "d_parser.tab.c"
    break;

  case 138: /* class_member: ctor_def  */
#line 365 "d_parser.y"
               { (yyval.class_member) = CreateCtorMember(ACCESS_PUBLIC, (yyvsp[0].ctor_def)); }
#line 2457 "d_parser.tab.c"
    break;

  case 139: /* class_member: dtor_def  */
#line 366 "d_parser.y"
               { (yyval.class_member) = CreateDtorMember(ACCESS_PUBLIC, (yyvsp[0].dtor_def)); }
#line 2463 "d_parser.tab.c"
    break;

  case 140: /* class_member: enum_def  */
#line 367 "d_parser.y"
               { (yyval.class_member) = CreateEnumMember(ACCESS_PUBLIC, (yyvsp[0].enum_def)); }
#line 2469 "d_parser.tab.c"
    break;

  case 141: /* class_member: access_spec enum_def  */
#line 368 "d_parser.y"
                           { (yyval.class_member) = CreateEnumMember((yyvsp[-1].access_spec), (yyvsp[0].enum_def)); }
#line 2475 "d_parser.tab.c"
    break;

  case 142: /* access_spec: PUBLIC  */
#line 372 "d_parser.y"
             { (yyval.access_spec) = ACCESS_PUBLIC; }
#line 2481 "d_parser.tab.c"
    break;

  case 143: /* access_spec: PRIVATE  */
#line 373 "d_parser.y"
              { (yyval.access_spec) = ACCESS_PRIVATE; }
#line 2487 "d_parser.tab.c"
    break;

  case 144: /* access_spec: PROTECTED  */
#line 374 "d_parser.y"
                { (yyval.access_spec) = ACCESS_PROTECTED; }
#line 2493 "d_parser.tab.c"
    break;

  case 145: /* method_def: type IDENT '(' ')' compound_stmt  */
#line 378 "d_parser.y"
                                       { (yyval.method_def) = CreateMethodDef(0, (yyvsp[-4].type), (yyvsp[-3].string_val), NULL, (yyvsp[0].stmt)); }
#line 2499 "d_parser.tab.c"
    break;

  case 146: /* method_def: type IDENT '(' param_list ')' compound_stmt  */
#line 379 "d_parser.y"
                                                  { (yyval.method_def) = CreateMethodDef(0, (yyvsp[-5].type), (yyvsp[-4].string_val), (yyvsp[-2].param_list), (yyvsp[0].stmt)); }
#line 2505 "d_parser.tab.c"
    break;

  case 147: /* method_def: VOID IDENT '(' ')' compound_stmt  */
#line 380 "d_parser.y"
                                       { (yyval.method_def) = CreateVoidMethodDef(0, (yyvsp[-3].string_val), NULL, (yyvsp[0].stmt)); }
#line 2511 "d_parser.tab.c"
    break;

  case 148: /* method_def: VOID IDENT '(' param_list ')' compound_stmt  */
#line 381 "d_parser.y"
                                                  { (yyval.method_def) = CreateVoidMethodDef(0, (yyvsp[-4].string_val), (yyvsp[-2].param_list), (yyvsp[0].stmt)); }
#line 2517 "d_parser.tab.c"
    break;

  case 149: /* method_def: OVERRIDE type IDENT '(' ')' compound_stmt  */
#line 382 "d_parser.y"
                                                { (yyval.method_def) = CreateMethodDef(1, (yyvsp[-4].type), (yyvsp[-3].string_val), NULL, (yyvsp[0].stmt)); }
#line 2523 "d_parser.tab.c"
    break;

  case 150: /* method_def: OVERRIDE type IDENT '(' param_list ')' compound_stmt  */
#line 383 "d_parser.y"
                                                           { (yyval.method_def) = CreateMethodDef(1, (yyvsp[-5].type), (yyvsp[-4].string_val), (yyvsp[-2].param_list), (yyvsp[0].stmt)); }
#line 2529 "d_parser.tab.c"
    break;

  case 151: /* method_def: OVERRIDE VOID IDENT '(' ')' compound_stmt  */
#line 384 "d_parser.y"
                                                { (yyval.method_def) = CreateVoidMethodDef(1, (yyvsp[-3].string_val), NULL, (yyvsp[0].stmt)); }
#line 2535 "d_parser.tab.c"
    break;

  case 152: /* method_def: OVERRIDE VOID IDENT '(' param_list ')' compound_stmt  */
#line 385 "d_parser.y"
                                                           { (yyval.method_def) = CreateVoidMethodDef(1, (yyvsp[-4].string_val), (yyvsp[-2].param_list), (yyvsp[0].stmt)); }
#line 2541 "d_parser.tab.c"
    break;

  case 153: /* ctor_def: THIS '(' ')' compound_stmt  */
#line 389 "d_parser.y"
                                 { (yyval.ctor_def) = CreateCtorDef(NULL, (yyvsp[0].stmt)); }
#line 2547 "d_parser.tab.c"
    break;

  case 154: /* ctor_def: THIS '(' param_list ')' compound_stmt  */
#line 390 "d_parser.y"
                                            { (yyval.ctor_def) = CreateCtorDef((yyvsp[-2].param_list), (yyvsp[0].stmt)); }
#line 2553 "d_parser.tab.c"
    break;

  case 155: /* dtor_def: '~' THIS '(' ')' compound_stmt  */
#line 394 "d_parser.y"
                                     { (yyval.dtor_def) = CreateDtorDef((yyvsp[0].stmt)); }
#line 2559 "d_parser.tab.c"
    break;

  case 156: /* enum_def: ENUM IDENT '{' enum_body '}'  */
#line 398 "d_parser.y"
                                   { (yyval.enum_def) = CreateEnumDef((yyvsp[-3].string_val), (yyvsp[-1].enum_item_list)->items, (yyvsp[-1].enum_item_list)->count); }
#line 2565 "d_parser.tab.c"
    break;

  case 157: /* enum_def: ENUM '{' enum_body '}'  */
#line 399 "d_parser.y"
                             { (yyval.enum_def) = CreateEnumDef(NULL, (yyvsp[-1].enum_item_list)->items, (yyvsp[-1].enum_item_list)->count); }
#line 2571 "d_parser.tab.c"
    break;

  case 158: /* enum_body: IDENT  */
#line 403 "d_parser.y"
            { (yyval.enum_item_list) = CreateEnumItemList(); AddEnumItemToList((yyval.enum_item_list), CreateEnumItem((yyvsp[0].string_val), 0, 0)); }
#line 2577 "d_parser.tab.c"
    break;

  case 159: /* enum_body: IDENT '=' INTEGER  */
#line 404 "d_parser.y"
                        { (yyval.enum_item_list) = CreateEnumItemList(); AddEnumItemToList((yyval.enum_item_list), CreateEnumItem((yyvsp[-2].string_val), 1, (yyvsp[0].int_val))); }
#line 2583 "d_parser.tab.c"
    break;

  case 160: /* enum_body: enum_body ',' IDENT  */
#line 405 "d_parser.y"
                          { (yyval.enum_item_list) = (yyvsp[-2].enum_item_list); AddEnumItemToList((yyval.enum_item_list), CreateEnumItem((yyvsp[0].string_val), 0, 0)); }
#line 2589 "d_parser.tab.c"
    break;

  case 161: /* enum_body: enum_body ',' IDENT '=' INTEGER  */
#line 406 "d_parser.y"
                                      { (yyval.enum_item_list) = (yyvsp[-4].enum_item_list); AddEnumItemToList((yyval.enum_item_list), CreateEnumItem((yyvsp[-2].string_val), 1, (yyvsp[0].int_val))); }
#line 2595 "d_parser.tab.c"
    break;


#line 2599 "d_parser.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 409 "d_parser.y"


void yyerror(const char *s) {
    fprintf(stderr, "Parse error at line %d: %s\n", yylineno, s);
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
    NExpr *result = CreateNewExpr(type, list->elements, list->count);
    return result;
}

