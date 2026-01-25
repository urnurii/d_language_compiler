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
extern char *yytext;
NExpr* CreateNewArrayExpr(NType *type, NExpr *expr);
void AppendSourceItemToRoot(NSourceItem *item);

NProgram *root = NULL;

#line 89 "d_parser.tab.c"

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
  YYSYMBOL_ENUMNAME = 43,                  /* ENUMNAME  */
  YYSYMBOL_CHARVAL = 44,                   /* CHARVAL  */
  YYSYMBOL_45_ = 45,                       /* '='  */
  YYSYMBOL_PLUSEQ = 46,                    /* PLUSEQ  */
  YYSYMBOL_MINUSEQ = 47,                   /* MINUSEQ  */
  YYSYMBOL_STAREQ = 48,                    /* STAREQ  */
  YYSYMBOL_SLASHEQ = 49,                   /* SLASHEQ  */
  YYSYMBOL_TILDEQ = 50,                    /* TILDEQ  */
  YYSYMBOL_LOR = 51,                       /* LOR  */
  YYSYMBOL_LAND = 52,                      /* LAND  */
  YYSYMBOL_EQ = 53,                        /* EQ  */
  YYSYMBOL_NEQ = 54,                       /* NEQ  */
  YYSYMBOL_LT = 55,                        /* LT  */
  YYSYMBOL_GT = 56,                        /* GT  */
  YYSYMBOL_LE = 57,                        /* LE  */
  YYSYMBOL_GE = 58,                        /* GE  */
  YYSYMBOL_PLUS = 59,                      /* PLUS  */
  YYSYMBOL_MINUS = 60,                     /* MINUS  */
  YYSYMBOL_STAR = 61,                      /* STAR  */
  YYSYMBOL_SLASH = 62,                     /* SLASH  */
  YYSYMBOL_UNOT = 63,                      /* UNOT  */
  YYSYMBOL_UMINUS = 64,                    /* UMINUS  */
  YYSYMBOL_UPLUS = 65,                     /* UPLUS  */
  YYSYMBOL_66_ = 66,                       /* '.'  */
  YYSYMBOL_67_ = 67,                       /* '['  */
  YYSYMBOL_68_ = 68,                       /* ']'  */
  YYSYMBOL_69_ = 69,                       /* '('  */
  YYSYMBOL_70_ = 70,                       /* ')'  */
  YYSYMBOL_LOWER_THAN_ELSE = 71,           /* LOWER_THAN_ELSE  */
  YYSYMBOL_72_ = 72,                       /* ','  */
  YYSYMBOL_73_ = 73,                       /* ';'  */
  YYSYMBOL_74_ = 74,                       /* '{'  */
  YYSYMBOL_75_ = 75,                       /* '}'  */
  YYSYMBOL_76_ = 76,                       /* ':'  */
  YYSYMBOL_77_ = 77,                       /* '~'  */
  YYSYMBOL_YYACCEPT = 78,                  /* $accept  */
  YYSYMBOL_program = 79,                   /* program  */
  YYSYMBOL_translation_unit = 80,          /* translation_unit  */
  YYSYMBOL_source_item = 81,               /* source_item  */
  YYSYMBOL_base_type = 82,                 /* base_type  */
  YYSYMBOL_array_decl = 83,                /* array_decl  */
  YYSYMBOL_type = 84,                      /* type  */
  YYSYMBOL_array_init = 85,                /* array_init  */
  YYSYMBOL_initializer = 86,               /* initializer  */
  YYSYMBOL_e_expr = 87,                    /* e_expr  */
  YYSYMBOL_expr = 88,                      /* expr  */
  YYSYMBOL_arg_list = 89,                  /* arg_list  */
  YYSYMBOL_init_decl = 90,                 /* init_decl  */
  YYSYMBOL_init_decl_list = 91,            /* init_decl_list  */
  YYSYMBOL_decl = 92,                      /* decl  */
  YYSYMBOL_param = 93,                     /* param  */
  YYSYMBOL_param_list = 94,                /* param_list  */
  YYSYMBOL_stmt = 95,                      /* stmt  */
  YYSYMBOL_stmt_list = 96,                 /* stmt_list  */
  YYSYMBOL_compound_stmt = 97,             /* compound_stmt  */
  YYSYMBOL_if_stmt = 98,                   /* if_stmt  */
  YYSYMBOL_while_stmt = 99,                /* while_stmt  */
  YYSYMBOL_do_while_stmt = 100,            /* do_while_stmt  */
  YYSYMBOL_for_stmt = 101,                 /* for_stmt  */
  YYSYMBOL_foreach_stmt = 102,             /* foreach_stmt  */
  YYSYMBOL_switch_stmt = 103,              /* switch_stmt  */
  YYSYMBOL_case_list = 104,                /* case_list  */
  YYSYMBOL_case_item = 105,                /* case_item  */
  YYSYMBOL_default_item = 106,             /* default_item  */
  YYSYMBOL_func_def = 107,                 /* func_def  */
  YYSYMBOL_func_body = 108,                /* func_body  */
  YYSYMBOL_class_def = 109,                /* class_def  */
  YYSYMBOL_class_members = 110,            /* class_members  */
  YYSYMBOL_class_member = 111,             /* class_member  */
  YYSYMBOL_access_spec = 112,              /* access_spec  */
  YYSYMBOL_method_def = 113,               /* method_def  */
  YYSYMBOL_ctor_def = 114,                 /* ctor_def  */
  YYSYMBOL_dtor_def = 115,                 /* dtor_def  */
  YYSYMBOL_enum_def = 116,                 /* enum_def  */
  YYSYMBOL_enum_item_name = 117,           /* enum_item_name  */
  YYSYMBOL_enum_body = 118                 /* enum_body  */
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
#define YYFINAL  30
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1611

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  78
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  41
/* YYNRULES -- Number of rules.  */
#define YYNRULES  173
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  371

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   320


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
      69,    70,     2,     2,    72,     2,    66,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    76,    73,
       2,    45,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    67,     2,    68,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    74,     2,    75,    77,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      71
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   135,   135,   136,   140,   141,   145,   146,   147,   148,
     152,   153,   154,   155,   156,   157,   158,   162,   163,   167,
     168,   169,   170,   171,   172,   176,   177,   181,   182,   186,
     187,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   242,
     243,   247,   248,   252,   253,   257,   261,   262,   263,   264,
     268,   269,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   292,   293,
     297,   298,   302,   303,   307,   311,   315,   316,   320,   321,
     325,   326,   330,   331,   332,   333,   337,   338,   342,   343,
     347,   348,   349,   350,   354,   355,   359,   360,   361,   362,
     366,   367,   371,   372,   373,   374,   375,   376,   377,   378,
     382,   383,   384,   388,   389,   390,   391,   392,   393,   394,
     395,   399,   400,   404,   408,   409,   410,   414,   415,   416,
     420,   421,   422,   423
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
  "IDENT", "CLASSNAME", "ENUMNAME", "CHARVAL", "'='", "PLUSEQ", "MINUSEQ",
  "STAREQ", "SLASHEQ", "TILDEQ", "LOR", "LAND", "EQ", "NEQ", "LT", "GT",
  "LE", "GE", "PLUS", "MINUS", "STAR", "SLASH", "UNOT", "UMINUS", "UPLUS",
  "'.'", "'['", "']'", "'('", "')'", "LOWER_THAN_ELSE", "','", "';'",
  "'{'", "'}'", "':'", "'~'", "$accept", "program", "translation_unit",
  "source_item", "base_type", "array_decl", "type", "array_init",
  "initializer", "e_expr", "expr", "arg_list", "init_decl",
  "init_decl_list", "decl", "param", "param_list", "stmt", "stmt_list",
  "compound_stmt", "if_stmt", "while_stmt", "do_while_stmt", "for_stmt",
  "foreach_stmt", "switch_stmt", "case_list", "case_item", "default_item",
  "func_def", "func_body", "class_def", "class_members", "class_member",
  "access_spec", "method_def", "ctor_def", "dtor_def", "enum_def",
  "enum_item_name", "enum_body", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-321)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-31)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     331,  -321,  -321,  -321,  -321,  -321,  -321,  -321,    -5,   -11,
     -16,     0,     0,    73,   331,  -321,     0,    53,  -321,  -321,
    -321,  -321,    59,    41,    45,    64,    50,   -25,  -321,  -321,
    -321,  -321,  -321,   -37,  -321,   -10,   214,    13,   109,    50,
      50,  -321,  -321,  -321,   118,   -42,    86,  -321,  1107,   405,
     132,  -321,   465,    10,   143,  -321,    17,   156,   172,  -321,
    -321,  -321,   129,  -321,   176,   171,  -321,   271,  -321,   831,
    -321,  -321,  -321,  -321,   139,     2,     3,   168,    50,  -321,
    -321,   161,  -321,   538,  -321,  -321,  -321,  -321,  -321,  -321,
    -321,   169,   179,  -321,  1185,  1185,  1185,  1144,  1185,  -321,
    -321,  1544,    10,    52,   186,  -321,   208,  -321,   420,  -321,
    -321,   207,    10,   786,   184,   217,   218,   680,   191,   -17,
    -321,  -321,  -321,  -321,  -321,   323,  -321,  -321,  -321,   210,
     220,    56,   195,   197,   855,   224,    65,    65,    65,  -321,
    1544,   -15,  1266,  1185,  1185,  1185,  1185,  1185,  1185,  1185,
    1185,  1185,  1185,  1185,  1185,  1185,  1185,  1185,  1185,  1185,
    1185,   226,  1185,  -321,    10,   223,   200,   201,   203,   638,
     213,   216,  1185,   219,   228,    16,   221,   121,  -321,   132,
     229,  1544,  -321,  -321,   493,  -321,  -321,  -321,  -321,  -321,
    -321,  -321,  1185,  -321,  -321,   687,   222,   227,   209,    57,
     242,   728,  -321,   375,   248,   246,  1185,   897,  1185,  1185,
    -321,    76,  -321,  -321,  1185,  -321,  1544,  1544,  1544,  1544,
    1544,  1544,   561,   850,   809,   809,    74,    74,    74,    74,
      -2,    -2,    65,    65,   247,  1210,  -321,  1185,  1185,  1185,
    1185,   273,   818,   301,   245,  -321,  -321,   939,   981,  -321,
    -321,  -321,  1544,   209,    85,   735,   776,  -321,   209,   209,
     209,    92,  -321,  -321,  1023,  1448,  -321,    97,  1472,  1496,
    -321,  1544,  1065,  1185,  -321,  1544,  1292,  1318,  1344,   251,
     249,  1185,   250,   252,  -321,   272,   100,   280,   124,  -321,
     209,   209,   131,   209,   158,  -321,  -321,  -321,   209,  -321,
     162,  -321,  -321,  -321,  -321,  -321,   165,  1520,   638,   282,
     638,  1185,  1185,   284,  1185,   285,  -321,   287,  -321,   288,
    -321,  -321,   209,  -321,   209,  -321,  -321,  -321,  -321,   275,
      -9,  -321,  1370,   289,  1185,  1396,  1185,  -321,  -321,  -321,
    -321,   638,  1185,   291,  -321,    -4,  -321,  -321,   295,  1185,
     254,   638,  1422,  -321,  1234,   566,  -321,  -321,  -321,  -321,
     255,   638,  -321,   638,   566,   566,   638,  -321,  -321,   566,
    -321
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,    10,    11,    12,    13,    14,    15,    16,     0,     0,
       0,    21,    23,     0,     2,     4,    19,     0,     8,     6,
       7,     9,     0,     0,     0,     0,     0,     0,    22,    24,
       1,     5,    20,    81,    83,     0,     0,     0,     0,     0,
       0,   167,   168,   169,   170,     0,     0,    17,     0,     0,
       0,    85,     0,     0,     0,    90,     0,     0,     0,   150,
     151,   152,     0,   137,     0,     0,   142,     0,   140,     0,
     143,   146,   147,   148,     0,     0,     0,     0,     0,   166,
      18,     0,    40,     0,    36,    37,    38,    39,    32,    33,
      35,    31,     0,    34,     0,     0,     0,     0,     0,    28,
      82,    27,     0,     0,    81,    84,     0,   135,    30,   134,
     132,    86,     0,     0,     0,     0,     0,     0,     0,    81,
     136,   141,   144,   145,   149,     0,   164,   165,   171,   172,
       0,     0,     0,     0,     0,     0,    59,    58,    60,    25,
      79,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   130,     0,    87,     0,     0,     0,    30,
       0,     0,    30,     0,     0,     0,    40,    23,   110,     0,
       0,    29,    93,   108,    30,    94,    95,    96,    97,    98,
      99,   100,     0,   133,    91,     0,     0,     0,     0,     0,
       0,     0,   139,     0,     0,    55,     0,     0,     0,     0,
      53,     0,    49,    26,     0,    41,    73,    74,    75,    76,
      77,    78,    72,    71,    69,    70,    65,    66,    67,    68,
      63,    64,    61,    62,    50,     0,   131,     0,     0,     0,
       0,     0,    30,     0,     0,   102,   103,     0,     0,    92,
     111,   109,    88,     0,     0,     0,     0,   161,     0,     0,
       0,     0,   138,   173,     0,     0,    43,     0,     0,     0,
      54,    80,     0,     0,    47,    89,     0,     0,     0,     0,
       0,    30,     0,     0,   101,     0,     0,     0,     0,   155,
       0,     0,     0,     0,     0,   162,   163,   153,     0,    56,
       0,    42,    44,    45,    46,    51,     0,     0,    30,     0,
      30,     0,    30,     0,     0,     0,   105,     0,   107,     0,
     156,   159,     0,   157,     0,   154,    57,    52,    48,   112,
       0,   114,     0,     0,    30,     0,     0,   104,   106,   160,
     158,    30,     0,     0,   121,     0,   122,   123,     0,    30,
       0,    30,     0,   113,     0,   128,   120,   124,   125,   115,
       0,    30,   118,    30,   126,   129,    30,   117,   119,   127,
     116
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -321,  -321,  -321,   307,   281,    18,   116,  -321,  -321,  -163,
     -48,  -127,   319,  -321,     1,   257,   -45,  -115,  -320,   -51,
    -321,  -321,  -321,  -321,  -321,  -321,  -321,    30,    31,  -321,
     -88,  -321,   261,   -64,  -321,   308,  -321,  -321,    12,   309,
     103
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,    13,    14,    15,    16,    29,   179,    99,   100,   180,
     181,   141,    34,    35,   182,    55,    56,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   345,   346,   347,    19,
     110,    20,    67,    68,    69,    70,    71,    72,    73,    44,
      45
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     101,    18,   109,   121,   103,   342,   343,   211,    48,   244,
     342,   343,    21,    46,   163,    18,     1,     2,     3,     4,
       5,     6,     7,    57,   193,    24,    21,    25,    48,    28,
      78,    23,    49,    79,    32,   365,    22,    58,    66,    59,
      60,    61,    62,    47,   369,    10,   136,   137,   138,   140,
     142,   109,   201,   213,   241,    11,    12,   214,    26,   159,
     160,   109,    50,    51,   161,   162,   344,    27,    66,   251,
     122,   356,   199,    30,    78,    78,   236,   126,   127,   280,
     267,   124,   130,   107,   108,   247,   140,   112,    63,   113,
      64,    41,    42,    43,    33,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   109,   235,    37,    17,    38,   313,    39,
     286,   288,   164,   206,   113,   207,    66,   258,    36,   113,
      17,   161,   162,   157,   158,   159,   160,   300,    40,   121,
     161,   162,    75,    76,   252,   306,   270,   257,   214,   333,
     254,    74,    54,    65,    80,   290,   261,   113,   265,   140,
     268,   269,   298,    77,   113,    54,   271,   302,   106,   214,
     317,   350,   214,   104,   116,     1,     2,     3,     4,     5,
       6,     7,   115,    65,   111,    65,   360,   135,    27,   275,
     276,   277,   278,   329,   319,   331,   214,   114,   117,   140,
     140,   322,   289,   113,    66,   118,   128,   295,   296,   297,
     292,   294,   119,   125,    11,    12,   140,     1,     2,     3,
       4,     5,     6,     7,   140,   307,   353,   130,   324,    54,
     113,    48,   326,    54,   214,   327,   362,   214,   134,   320,
     321,    65,   323,   281,    52,   135,   367,   325,   368,   165,
     251,   370,   192,   195,   251,   204,    11,    12,   196,   197,
     200,   205,   208,   332,   209,   212,   335,   234,   237,   238,
     239,   339,   240,   340,     1,     2,     3,     4,     5,     6,
       7,    57,   242,   108,    53,   243,   263,   341,   352,   279,
     248,   255,   245,   315,   354,    58,   256,    59,    60,    61,
      62,   246,   249,    10,     1,     2,     3,     4,     5,     6,
       7,    54,   259,    11,    12,   264,   272,    54,   284,    65,
     311,    31,   312,   314,   361,   366,     1,     2,     3,     4,
       5,     6,     7,    57,     1,     2,     3,     4,     5,     6,
       7,     8,   282,    11,    12,   316,   120,    58,    64,    59,
      60,    61,    62,   318,     9,    10,   330,   334,   336,   283,
     337,   338,   349,    10,   133,    11,    12,   355,   359,   105,
     194,    54,    54,    11,    12,   357,   358,   123,     1,     2,
       3,     4,     5,     6,     7,    57,   203,   129,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   202,    58,
      64,    59,    60,    61,    62,     0,     0,    10,     1,     2,
       3,     4,     5,     6,     7,     0,     0,    11,    12,     0,
       0,     0,     0,     1,     2,     3,     4,     5,     6,     7,
       0,   166,     0,   167,     0,    52,   168,   169,   170,   171,
     172,   173,   174,     0,     0,   175,     0,    11,    12,   176,
     262,    83,    64,    84,    85,    86,    87,     0,    88,    89,
      90,    91,    11,   177,    93,     0,     0,     0,     1,     2,
       3,     4,     5,     6,     7,   102,     0,     0,     0,    94,
      95,     0,     0,    96,     0,     0,     0,     0,     0,    98,
       0,     0,     0,     0,   108,   178,     1,     2,     3,     4,
       5,     6,     7,     0,   166,     0,   167,    11,    12,   168,
     169,   170,   171,   172,   173,   174,     0,     0,   175,     0,
       0,     0,   176,     0,    83,     0,    84,    85,    86,    87,
       0,    88,    89,    90,    91,    11,   177,    93,     0,     0,
       0,     1,     2,     3,     4,     5,     6,     7,     0,     0,
       0,     0,    94,    95,     0,     0,    96,     0,     0,     0,
       0,     0,    98,     0,     0,     0,     0,   108,   250,     1,
       2,     3,     4,     5,     6,     7,     0,   166,     0,   167,
     131,   132,   168,   169,   170,   171,   172,   173,   174,     0,
       0,   175,     0,     0,     0,   176,     0,    83,     0,    84,
      85,    86,    87,     0,    88,    89,    90,    91,    11,   177,
      93,     0,     0,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,     0,    94,    95,   161,   162,    96,
       0,     0,     0,     0,     0,    98,     0,     0,     0,   -30,
     108,     1,     2,     3,     4,     5,     6,     7,     0,   166,
       0,   167,     0,     0,   168,   169,   170,   171,   172,   173,
     174,     0,     0,   175,     0,     0,     0,   176,     0,    83,
       0,    84,    85,    86,    87,     0,    88,    89,    90,    91,
      11,   177,    93,     1,     2,     3,     4,     5,     6,     7,
       1,     2,     3,     4,     5,     6,     7,    94,    95,     0,
       0,    96,     0,     0,     0,     0,     0,    98,     0,     0,
      52,     0,   108,     0,     0,     0,     0,    52,     0,     0,
       0,     0,    11,    12,     0,     0,     0,     0,     0,    11,
      12,     1,     2,     3,     4,     5,     6,     7,     1,     2,
       3,     4,     5,     6,     7,     0,     0,     0,     0,     0,
     198,     0,     0,     0,     0,     0,     0,   253,    52,     0,
       0,     0,     0,     0,     0,    52,     0,     0,     0,     0,
      11,    12,     0,     0,     0,     0,     0,    11,    12,     1,
       2,     3,     4,     5,     6,     7,     0,     0,     0,     1,
       2,     3,     4,     5,     6,     7,     0,     0,   260,     0,
       0,     0,     0,     0,     0,   291,    52,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    52,     0,    11,    12,
       0,     1,     2,     3,     4,     5,     6,     7,    11,    12,
       0,     0,     0,     0,     1,     2,     3,     4,     5,     6,
       7,    57,     0,    81,     0,     0,   293,    82,     0,    83,
       0,    84,    85,    86,    87,    58,    88,    89,    90,    91,
      11,   177,    93,    10,   153,   154,   155,   156,   157,   158,
     159,   160,     0,    11,    12,   161,   162,    94,    95,     0,
      81,    96,     0,     0,    82,     0,    83,    98,    84,    85,
      86,    87,     0,    88,    89,    90,    91,     0,    92,    93,
       0,     0,     0,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,     0,    94,    95,   161,   162,    96,     0,
       0,     0,    81,     0,    98,   210,    82,     0,    83,     0,
      84,    85,    86,    87,     0,    88,    89,    90,    91,     0,
      92,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,    95,     0,     0,
      96,     0,     0,     0,    81,     0,    98,   266,    82,     0,
      83,     0,    84,    85,    86,    87,     0,    88,    89,    90,
      91,     0,    92,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    95,
       0,     0,    96,     0,     0,     0,    81,     0,    98,   285,
      82,     0,    83,     0,    84,    85,    86,    87,     0,    88,
      89,    90,    91,     0,    92,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,    95,     0,     0,    96,     0,     0,     0,    81,     0,
      98,   287,    82,     0,    83,     0,    84,    85,    86,    87,
       0,    88,    89,    90,    91,     0,    92,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,    95,     0,     0,    96,     0,     0,     0,
      81,     0,    98,   299,    82,     0,    83,     0,    84,    85,
      86,    87,     0,    88,    89,    90,    91,     0,    92,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,    95,     0,     0,    96,     0,
       0,     0,    81,     0,    98,   305,    82,     0,    83,     0,
      84,    85,    86,    87,     0,    88,    89,    90,    91,     0,
      92,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,    95,     0,    81,
      96,     0,     0,    82,    97,    83,    98,    84,    85,    86,
      87,     0,    88,    89,    90,    91,     0,    92,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,    95,     0,     0,    96,     0,     0,
      81,     0,   139,    98,    82,     0,    83,     0,    84,    85,
      86,    87,     0,    88,    89,    90,    91,     0,    92,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,    95,     0,   273,    96,     0,
       0,     0,     0,     0,    98,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,     0,     0,     0,   161,   162,   274,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,     0,     0,     0,
     161,   162,     0,     0,     0,     0,     0,     0,     0,     0,
     364,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,     0,
       0,     0,   161,   162,     0,     0,   215,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,     0,     0,     0,   161,   162,
       0,     0,   308,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,     0,     0,     0,   161,   162,     0,     0,   309,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,     0,     0,     0,
     161,   162,     0,     0,   310,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,     0,     0,     0,   161,   162,     0,     0,
     348,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,     0,
       0,     0,   161,   162,     0,     0,   351,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,     0,     0,     0,   161,   162,
       0,     0,   363,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,     0,     0,     0,   161,   162,   301,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,     0,     0,     0,   161,   162,
     303,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,     0,
       0,     0,   161,   162,   304,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,     0,     0,     0,   161,   162,   328,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,     0,     0,     0,
     161,   162
};

static const yytype_int16 yycheck[] =
{
      48,     0,    53,    67,    49,    14,    15,   134,    45,   172,
      14,    15,     0,    38,   102,    14,     3,     4,     5,     6,
       7,     8,     9,    10,   112,    41,    14,    43,    45,    11,
      72,    42,    69,    75,    16,   355,    41,    24,    37,    26,
      27,    28,    29,    68,   364,    32,    94,    95,    96,    97,
      98,   102,    69,    68,   169,    42,    43,    72,    74,    61,
      62,   112,    72,    73,    66,    67,    75,    67,    67,   184,
      69,    75,   117,     0,    72,    72,   164,    75,    75,   242,
     207,    69,    66,    73,    74,    69,   134,    70,    75,    72,
      77,    41,    42,    43,    41,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   164,   162,    74,     0,    76,   281,    74,
     247,   248,    70,    67,    72,    69,   125,    70,    69,    72,
      14,    66,    67,    59,    60,    61,    62,   264,    74,   203,
      66,    67,    39,    40,   192,   272,    70,   198,    72,   312,
     195,    42,    36,    37,    68,    70,   201,    72,   206,   207,
     208,   209,    70,    45,    72,    49,   214,    70,    52,    72,
      70,   334,    72,    41,    58,     3,     4,     5,     6,     7,
       8,     9,    10,    67,    41,    69,   349,    66,    67,   237,
     238,   239,   240,   308,    70,   310,    72,    41,    69,   247,
     248,    70,   253,    72,   203,    29,    38,   258,   259,   260,
     255,   256,    41,    74,    42,    43,   264,     3,     4,     5,
       6,     7,     8,     9,   272,   273,   341,    66,    70,   113,
      72,    45,    70,   117,    72,    70,   351,    72,    69,   290,
     291,   125,   293,   242,    30,    66,   361,   298,   363,    41,
     365,   366,    45,    69,   369,    45,    42,    43,    41,    41,
      69,    41,    67,   311,    67,    41,   314,    41,    45,    69,
      69,   322,    69,   324,     3,     4,     5,     6,     7,     8,
       9,    10,    69,    74,    70,    69,    38,    12,   336,    16,
      69,    69,    73,    41,   342,    24,    69,    26,    27,    28,
      29,    73,    73,    32,     3,     4,     5,     6,     7,     8,
       9,   195,    70,    42,    43,    69,    69,   201,    73,   203,
      69,    14,    73,    73,    70,    70,     3,     4,     5,     6,
       7,     8,     9,    10,     3,     4,     5,     6,     7,     8,
       9,    10,    41,    42,    43,    73,    75,    24,    77,    26,
      27,    28,    29,    73,    23,    32,    74,    73,    73,   243,
      73,    73,    73,    32,    83,    42,    43,    76,    73,    50,
     113,   255,   256,    42,    43,   345,   345,    69,     3,     4,
       5,     6,     7,     8,     9,    10,   125,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    24,
      77,    26,    27,    28,    29,    -1,    -1,    32,     3,     4,
       5,     6,     7,     8,     9,    -1,    -1,    42,    43,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,     9,
      -1,    11,    -1,    13,    -1,    30,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    25,    -1,    42,    43,    29,
      75,    31,    77,    33,    34,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,     9,    70,    -1,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    69,
      -1,    -1,    -1,    -1,    74,    75,     3,     4,     5,     6,
       7,     8,     9,    -1,    11,    -1,    13,    42,    43,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    25,    -1,
      -1,    -1,    29,    -1,    31,    -1,    33,    34,    35,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    -1,    -1,
      -1,     3,     4,     5,     6,     7,     8,     9,    -1,    -1,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    69,    -1,    -1,    -1,    -1,    74,    75,     3,
       4,     5,     6,     7,     8,     9,    -1,    11,    -1,    13,
      42,    43,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    25,    -1,    -1,    -1,    29,    -1,    31,    -1,    33,
      34,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    -1,    -1,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    -1,    59,    60,    66,    67,    63,
      -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,    73,
      74,     3,     4,     5,     6,     7,     8,     9,    -1,    11,
      -1,    13,    -1,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    25,    -1,    -1,    -1,    29,    -1,    31,
      -1,    33,    34,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,     3,     4,     5,     6,     7,     8,     9,
       3,     4,     5,     6,     7,     8,     9,    59,    60,    -1,
      -1,    63,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,
      30,    -1,    74,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    42,    43,    -1,    -1,    -1,    -1,    -1,    42,
      43,     3,     4,     5,     6,     7,     8,     9,     3,     4,
       5,     6,     7,     8,     9,    -1,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    70,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      42,    43,    -1,    -1,    -1,    -1,    -1,    42,    43,     3,
       4,     5,     6,     7,     8,     9,    -1,    -1,    -1,     3,
       4,     5,     6,     7,     8,     9,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    42,    43,
      -1,     3,     4,     5,     6,     7,     8,     9,    42,    43,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    25,    -1,    -1,    70,    29,    -1,    31,
      -1,    33,    34,    35,    36,    24,    38,    39,    40,    41,
      42,    43,    44,    32,    55,    56,    57,    58,    59,    60,
      61,    62,    -1,    42,    43,    66,    67,    59,    60,    -1,
      25,    63,    -1,    -1,    29,    -1,    31,    69,    33,    34,
      35,    36,    -1,    38,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    -1,    59,    60,    66,    67,    63,    -1,
      -1,    -1,    25,    -1,    69,    70,    29,    -1,    31,    -1,
      33,    34,    35,    36,    -1,    38,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    -1,    -1,    25,    -1,    69,    70,    29,    -1,
      31,    -1,    33,    34,    35,    36,    -1,    38,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    -1,    -1,    25,    -1,    69,    70,
      29,    -1,    31,    -1,    33,    34,    35,    36,    -1,    38,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    -1,    -1,    25,    -1,
      69,    70,    29,    -1,    31,    -1,    33,    34,    35,    36,
      -1,    38,    39,    40,    41,    -1,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    -1,    -1,
      25,    -1,    69,    70,    29,    -1,    31,    -1,    33,    34,
      35,    36,    -1,    38,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      -1,    -1,    25,    -1,    69,    70,    29,    -1,    31,    -1,
      33,    34,    35,    36,    -1,    38,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    59,    60,    -1,    25,
      63,    -1,    -1,    29,    67,    31,    69,    33,    34,    35,
      36,    -1,    38,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    -1,
      25,    -1,    68,    69,    29,    -1,    31,    -1,    33,    34,
      35,    36,    -1,    38,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    59,    60,    -1,    37,    63,    -1,
      -1,    -1,    -1,    -1,    69,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    -1,    -1,    -1,    66,    67,    68,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    -1,    -1,    -1,
      66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      76,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    -1,
      -1,    -1,    66,    67,    -1,    -1,    70,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    -1,    -1,    -1,    66,    67,
      -1,    -1,    70,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    -1,    -1,    -1,    66,    67,    -1,    -1,    70,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    -1,    -1,    -1,
      66,    67,    -1,    -1,    70,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    -1,    -1,    -1,    66,    67,    -1,    -1,
      70,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    -1,
      -1,    -1,    66,    67,    -1,    -1,    70,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    -1,    -1,    -1,    66,    67,
      -1,    -1,    70,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    -1,    -1,    -1,    66,    67,    68,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    -1,    -1,    -1,    66,    67,
      68,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    -1,
      -1,    -1,    66,    67,    68,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    -1,    -1,    -1,    66,    67,    68,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    -1,    -1,    -1,
      66,    67
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    23,
      32,    42,    43,    79,    80,    81,    82,    84,    92,   107,
     109,   116,    41,    42,    41,    43,    74,    67,    83,    83,
       0,    81,    83,    41,    90,    91,    69,    74,    76,    74,
      74,    41,    42,    43,   117,   118,    38,    68,    45,    69,
      72,    73,    30,    70,    84,    93,    94,    10,    24,    26,
      27,    28,    29,    75,    77,    84,    92,   110,   111,   112,
     113,   114,   115,   116,    42,   118,   118,    45,    72,    75,
      68,    25,    29,    31,    33,    34,    35,    36,    38,    39,
      40,    41,    43,    44,    59,    60,    63,    67,    69,    85,
      86,    88,    70,    94,    41,    90,    84,    73,    74,    97,
     108,    41,    70,    72,    41,    10,    84,    69,    29,    41,
      75,   111,    92,   113,   116,    74,    75,    75,    38,   117,
      66,    42,    43,    82,    69,    66,    88,    88,    88,    68,
      88,    89,    88,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    66,    67,   108,    70,    41,    11,    13,    16,    17,
      18,    19,    20,    21,    22,    25,    29,    43,    75,    84,
      87,    88,    92,    95,    96,    97,    98,    99,   100,   101,
     102,   103,    45,   108,    93,    69,    41,    41,    70,    94,
      69,    69,    75,   110,    45,    41,    67,    69,    67,    67,
      70,    89,    41,    68,    72,    70,    88,    88,    88,    88,
      88,    88,    88,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    88,    88,    41,    88,   108,    45,    69,    69,
      69,    95,    69,    69,    87,    73,    73,    69,    69,    73,
      75,    95,    88,    70,    94,    69,    69,    97,    70,    70,
      70,    94,    75,    38,    69,    88,    70,    89,    88,    88,
      70,    88,    69,    37,    68,    88,    88,    88,    88,    16,
      87,    92,    41,    84,    73,    70,    89,    70,    89,    97,
      70,    70,    94,    70,    94,    97,    97,    97,    70,    70,
      89,    68,    70,    68,    68,    70,    89,    88,    70,    70,
      70,    69,    73,    87,    73,    41,    73,    70,    73,    70,
      97,    97,    70,    97,    70,    97,    70,    70,    68,    95,
      74,    95,    88,    87,    73,    88,    73,    73,    73,    97,
      97,    12,    14,    15,    75,   104,   105,   106,    70,    73,
      87,    70,    88,    95,    88,    76,    75,   105,   106,    73,
      87,    70,    95,    70,    76,    96,    70,    95,    95,    96,
      95
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    78,    79,    79,    80,    80,    81,    81,    81,    81,
      82,    82,    82,    82,    82,    82,    82,    83,    83,    84,
      84,    84,    84,    84,    84,    85,    85,    86,    86,    87,
      87,    88,    88,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    88,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    88,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    88,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    88,    88,    88,    88,    88,    88,    88,    89,
      89,    90,    90,    91,    91,    92,    93,    93,    93,    93,
      94,    94,    95,    95,    95,    95,    95,    95,    95,    95,
      95,    95,    95,    95,    95,    95,    95,    95,    96,    96,
      97,    97,    98,    98,    99,   100,   101,   101,   102,   102,
     103,   103,   104,   104,   104,   104,   105,   105,   106,   106,
     107,   107,   107,   107,   108,   108,   109,   109,   109,   109,
     110,   110,   111,   111,   111,   111,   111,   111,   111,   111,
     112,   112,   112,   113,   113,   113,   113,   113,   113,   113,
     113,   114,   114,   115,   116,   116,   116,   117,   117,   117,
     118,   118,   118,   118
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     3,     1,
       2,     1,     2,     1,     2,     2,     3,     1,     1,     1,
       0,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     5,     4,     5,     5,     5,     4,     6,     3,
       3,     5,     6,     3,     4,     3,     5,     6,     2,     2,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       3,     1,     3,     1,     3,     3,     2,     3,     4,     5,
       1,     3,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     2,     2,     5,     4,     5,     4,     1,     2,
       2,     3,     5,     7,     5,     7,     9,     8,     7,     8,
       7,     6,     1,     1,     2,     2,     3,     4,     2,     3,
       5,     6,     5,     6,     1,     1,     5,     4,     7,     6,
       1,     2,     1,     1,     2,     2,     1,     1,     1,     2,
       1,     1,     1,     5,     6,     5,     6,     6,     7,     6,
       7,     4,     5,     5,     5,     5,     4,     1,     1,     1,
       1,     3,     3,     5
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
#line 135 "d_parser.y"
                       { (yyval.program) = root; }
#line 1696 "d_parser.tab.c"
    break;

  case 3: /* program: %empty  */
#line 136 "d_parser.y"
             { (yyval.program) = CreateProgram(NULL); root = (yyval.program); }
#line 1702 "d_parser.tab.c"
    break;

  case 4: /* translation_unit: source_item  */
#line 140 "d_parser.y"
                  { AppendSourceItemToRoot((yyvsp[0].source_item)); (yyval.source_item) = (yyvsp[0].source_item); }
#line 1708 "d_parser.tab.c"
    break;

  case 5: /* translation_unit: translation_unit source_item  */
#line 141 "d_parser.y"
                                   { AppendSourceItemToRoot((yyvsp[0].source_item)); (yyval.source_item) = (yyvsp[-1].source_item); }
#line 1714 "d_parser.tab.c"
    break;

  case 6: /* source_item: func_def  */
#line 145 "d_parser.y"
               { (yyval.source_item) = CreateFuncSourceItem((yyvsp[0].func_def)); }
#line 1720 "d_parser.tab.c"
    break;

  case 7: /* source_item: class_def  */
#line 146 "d_parser.y"
                { (yyval.source_item) = CreateClassSourceItem((yyvsp[0].class_def)); }
#line 1726 "d_parser.tab.c"
    break;

  case 8: /* source_item: decl  */
#line 147 "d_parser.y"
           { (yyval.source_item) = CreateDeclSourceItem((yyvsp[0].decl_stmt).type, (yyvsp[0].decl_stmt).init_decls); }
#line 1732 "d_parser.tab.c"
    break;

  case 9: /* source_item: enum_def  */
#line 148 "d_parser.y"
               { (yyval.source_item) = CreateEnumSourceItem((yyvsp[0].enum_def)); }
#line 1738 "d_parser.tab.c"
    break;

  case 10: /* base_type: INT  */
#line 152 "d_parser.y"
          { (yyval.type) = CreateBaseType(TYPE_INT); }
#line 1744 "d_parser.tab.c"
    break;

  case 11: /* base_type: CHAR  */
#line 153 "d_parser.y"
           { (yyval.type) = CreateBaseType(TYPE_CHAR); }
#line 1750 "d_parser.tab.c"
    break;

  case 12: /* base_type: STRING  */
#line 154 "d_parser.y"
             { (yyval.type) = CreateBaseType(TYPE_STRING); }
#line 1756 "d_parser.tab.c"
    break;

  case 13: /* base_type: BOOL  */
#line 155 "d_parser.y"
           { (yyval.type) = CreateBaseType(TYPE_BOOL); }
#line 1762 "d_parser.tab.c"
    break;

  case 14: /* base_type: FLOAT_TYPE  */
#line 156 "d_parser.y"
                 { (yyval.type) = CreateBaseType(TYPE_FLOAT); }
#line 1768 "d_parser.tab.c"
    break;

  case 15: /* base_type: DOUBLE  */
#line 157 "d_parser.y"
             { (yyval.type) = CreateBaseType(TYPE_DOUBLE); }
#line 1774 "d_parser.tab.c"
    break;

  case 16: /* base_type: REAL  */
#line 158 "d_parser.y"
           { (yyval.type) = CreateBaseType(TYPE_REAL); }
#line 1780 "d_parser.tab.c"
    break;

  case 17: /* array_decl: '[' ']'  */
#line 162 "d_parser.y"
              { (yyval.array_decl) = CreateArrayDecl(0, 0); }
#line 1786 "d_parser.tab.c"
    break;

  case 18: /* array_decl: '[' INTEGER ']'  */
#line 163 "d_parser.y"
                      { (yyval.array_decl) = CreateArrayDecl(1, (yyvsp[-1].int_val)); }
#line 1792 "d_parser.tab.c"
    break;

  case 19: /* type: base_type  */
#line 167 "d_parser.y"
                { (yyval.type) = (yyvsp[0].type); }
#line 1798 "d_parser.tab.c"
    break;

  case 20: /* type: base_type array_decl  */
#line 168 "d_parser.y"
                           { (yyval.type) = AddArrayToType((yyvsp[-1].type), (yyvsp[0].array_decl)); }
#line 1804 "d_parser.tab.c"
    break;

  case 21: /* type: CLASSNAME  */
#line 169 "d_parser.y"
                { (yyval.type) = CreateClassType((yyvsp[0].string_val)); }
#line 1810 "d_parser.tab.c"
    break;

  case 22: /* type: CLASSNAME array_decl  */
#line 170 "d_parser.y"
                           { (yyval.type) = AddArrayToType(CreateClassType((yyvsp[-1].string_val)), (yyvsp[0].array_decl)); }
#line 1816 "d_parser.tab.c"
    break;

  case 23: /* type: ENUMNAME  */
#line 171 "d_parser.y"
               { (yyval.type) = CreateEnumType((yyvsp[0].string_val)); }
#line 1822 "d_parser.tab.c"
    break;

  case 24: /* type: ENUMNAME array_decl  */
#line 172 "d_parser.y"
                          { (yyval.type) = AddArrayToType(CreateEnumType((yyvsp[-1].string_val)), (yyvsp[0].array_decl)); }
#line 1828 "d_parser.tab.c"
    break;

  case 25: /* array_init: '[' ']'  */
#line 176 "d_parser.y"
              { (yyval.initializer) = CreateArrayInitializer(NULL, 0); }
#line 1834 "d_parser.tab.c"
    break;

  case 26: /* array_init: '[' arg_list ']'  */
#line 177 "d_parser.y"
                       { (yyval.initializer) = CreateArrayInitializer((yyvsp[-1].expr_list)->elements, (yyvsp[-1].expr_list)->count); }
#line 1840 "d_parser.tab.c"
    break;

  case 27: /* initializer: expr  */
#line 181 "d_parser.y"
           { (yyval.initializer) = CreateExprInitializer((yyvsp[0].expr)); }
#line 1846 "d_parser.tab.c"
    break;

  case 28: /* initializer: array_init  */
#line 182 "d_parser.y"
                 { (yyval.initializer) = (yyvsp[0].initializer); }
#line 1852 "d_parser.tab.c"
    break;

  case 29: /* e_expr: expr  */
#line 186 "d_parser.y"
           { (yyval.expr) = (yyvsp[0].expr); }
#line 1858 "d_parser.tab.c"
    break;

  case 30: /* e_expr: %empty  */
#line 187 "d_parser.y"
             { (yyval.expr) = NULL; }
#line 1864 "d_parser.tab.c"
    break;

  case 31: /* expr: IDENT  */
#line 191 "d_parser.y"
            { (yyval.expr) = CreateIdentExpr((yyvsp[0].string_val)); }
#line 1870 "d_parser.tab.c"
    break;

  case 32: /* expr: INTEGER  */
#line 192 "d_parser.y"
              { (yyval.expr) = CreateIntExpr((yyvsp[0].int_val)); }
#line 1876 "d_parser.tab.c"
    break;

  case 33: /* expr: FLOAT_VAL  */
#line 193 "d_parser.y"
                { (yyval.expr) = CreateFloatExpr((yyvsp[0].float_val)); }
#line 1882 "d_parser.tab.c"
    break;

  case 34: /* expr: CHARVAL  */
#line 194 "d_parser.y"
              { (yyval.expr) = CreateCharExpr((yyvsp[0].char_val)); }
#line 1888 "d_parser.tab.c"
    break;

  case 35: /* expr: STR  */
#line 195 "d_parser.y"
          { (yyval.expr) = CreateStringExpr((yyvsp[0].string_val)); }
#line 1894 "d_parser.tab.c"
    break;

  case 36: /* expr: TRUE_VAL  */
#line 196 "d_parser.y"
               { (yyval.expr) = CreateBoolExpr(1); }
#line 1900 "d_parser.tab.c"
    break;

  case 37: /* expr: FALSE_VAL  */
#line 197 "d_parser.y"
                { (yyval.expr) = CreateBoolExpr(0); }
#line 1906 "d_parser.tab.c"
    break;

  case 38: /* expr: NULL_CONST  */
#line 198 "d_parser.y"
                 { (yyval.expr) = CreateNullExpr(); }
#line 1912 "d_parser.tab.c"
    break;

  case 39: /* expr: NAN_CONST  */
#line 199 "d_parser.y"
                { (yyval.expr) = CreateNanExpr(); }
#line 1918 "d_parser.tab.c"
    break;

  case 40: /* expr: THIS  */
#line 200 "d_parser.y"
           { (yyval.expr) = CreateThisExpr(); }
#line 1924 "d_parser.tab.c"
    break;

  case 41: /* expr: '(' expr ')'  */
#line 201 "d_parser.y"
                   { (yyval.expr) = CreateParenExpr((yyvsp[-1].expr)); }
#line 1930 "d_parser.tab.c"
    break;

  case 42: /* expr: NEW CLASSNAME '[' expr ']'  */
#line 202 "d_parser.y"
                                 { (yyval.expr) = CreateNewArrayExpr(CreateClassType((yyvsp[-3].string_val)), (yyvsp[-1].expr)); }
#line 1936 "d_parser.tab.c"
    break;

  case 43: /* expr: NEW CLASSNAME '(' ')'  */
#line 203 "d_parser.y"
                            { (yyval.expr) = CreateNewExpr(CreateClassType((yyvsp[-2].string_val)), NULL, 0); }
#line 1942 "d_parser.tab.c"
    break;

  case 44: /* expr: NEW CLASSNAME '(' arg_list ')'  */
#line 204 "d_parser.y"
                                     { (yyval.expr) = CreateNewExpr(CreateClassType((yyvsp[-3].string_val)), (yyvsp[-1].expr_list)->elements, (yyvsp[-1].expr_list)->count); }
#line 1948 "d_parser.tab.c"
    break;

  case 45: /* expr: NEW ENUMNAME '[' expr ']'  */
#line 205 "d_parser.y"
                                { (yyval.expr) = CreateNewArrayExpr(CreateEnumType((yyvsp[-3].string_val)), (yyvsp[-1].expr)); }
#line 1954 "d_parser.tab.c"
    break;

  case 46: /* expr: NEW base_type '[' expr ']'  */
#line 206 "d_parser.y"
                                 { (yyval.expr) = CreateNewArrayExpr((yyvsp[-3].type), (yyvsp[-1].expr)); }
#line 1960 "d_parser.tab.c"
    break;

  case 47: /* expr: expr '[' expr ']'  */
#line 207 "d_parser.y"
                        { (yyval.expr) = CreateArrayAccessExpr((yyvsp[-3].expr), (yyvsp[-1].expr), NULL); }
#line 1966 "d_parser.tab.c"
    break;

  case 48: /* expr: expr '[' expr DOTDOT expr ']'  */
#line 208 "d_parser.y"
                                    { (yyval.expr) = CreateArrayAccessExpr((yyvsp[-5].expr), (yyvsp[-3].expr), (yyvsp[-1].expr)); }
#line 1972 "d_parser.tab.c"
    break;

  case 49: /* expr: ENUMNAME '.' IDENT  */
#line 209 "d_parser.y"
                         { (yyval.expr) = CreateMemberAccessExpr(CreateIdentExpr((yyvsp[-2].string_val)), (yyvsp[0].string_val)); }
#line 1978 "d_parser.tab.c"
    break;

  case 50: /* expr: expr '.' IDENT  */
#line 210 "d_parser.y"
                     { (yyval.expr) = CreateMemberAccessExpr((yyvsp[-2].expr), (yyvsp[0].string_val)); }
#line 1984 "d_parser.tab.c"
    break;

  case 51: /* expr: expr '.' IDENT '(' ')'  */
#line 211 "d_parser.y"
                             { (yyval.expr) = CreateMethodCallExpr((yyvsp[-4].expr), (yyvsp[-2].string_val), NULL, 0); }
#line 1990 "d_parser.tab.c"
    break;

  case 52: /* expr: expr '.' IDENT '(' arg_list ')'  */
#line 212 "d_parser.y"
                                      { (yyval.expr) = CreateMethodCallExpr((yyvsp[-5].expr), (yyvsp[-3].string_val), (yyvsp[-1].expr_list)->elements, (yyvsp[-1].expr_list)->count); }
#line 1996 "d_parser.tab.c"
    break;

  case 53: /* expr: IDENT '(' ')'  */
#line 213 "d_parser.y"
                    { (yyval.expr) = CreateFuncCallExpr((yyvsp[-2].string_val), NULL, 0); }
#line 2002 "d_parser.tab.c"
    break;

  case 54: /* expr: IDENT '(' arg_list ')'  */
#line 214 "d_parser.y"
                             { (yyval.expr) = CreateFuncCallExpr((yyvsp[-3].string_val), (yyvsp[-1].expr_list)->elements, (yyvsp[-1].expr_list)->count); }
#line 2008 "d_parser.tab.c"
    break;

  case 55: /* expr: SUPER '.' IDENT  */
#line 215 "d_parser.y"
                      { (yyval.expr) = CreateSuperExpr((yyvsp[0].string_val)); }
#line 2014 "d_parser.tab.c"
    break;

  case 56: /* expr: SUPER '.' IDENT '(' ')'  */
#line 216 "d_parser.y"
                              { (yyval.expr) = CreateSuperMethodCallExpr((yyvsp[-2].string_val), NULL, 0); }
#line 2020 "d_parser.tab.c"
    break;

  case 57: /* expr: SUPER '.' IDENT '(' arg_list ')'  */
#line 217 "d_parser.y"
                                       { (yyval.expr) = CreateSuperMethodCallExpr((yyvsp[-3].string_val), (yyvsp[-1].expr_list)->elements, (yyvsp[-1].expr_list)->count); }
#line 2026 "d_parser.tab.c"
    break;

  case 58: /* expr: MINUS expr  */
#line 218 "d_parser.y"
                              { (yyval.expr) = CreateUnaryOpExpr(OP_MINUS, (yyvsp[0].expr)); }
#line 2032 "d_parser.tab.c"
    break;

  case 59: /* expr: PLUS expr  */
#line 219 "d_parser.y"
                            { (yyval.expr) = CreateUnaryOpExpr(OP_PLUS, (yyvsp[0].expr)); }
#line 2038 "d_parser.tab.c"
    break;

  case 60: /* expr: UNOT expr  */
#line 220 "d_parser.y"
                           { (yyval.expr) = CreateUnaryOpExpr(OP_NOT, (yyvsp[0].expr)); }
#line 2044 "d_parser.tab.c"
    break;

  case 61: /* expr: expr STAR expr  */
#line 221 "d_parser.y"
                     { (yyval.expr) = CreateBinaryOpExpr(OP_MUL, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2050 "d_parser.tab.c"
    break;

  case 62: /* expr: expr SLASH expr  */
#line 222 "d_parser.y"
                      { (yyval.expr) = CreateBinaryOpExpr(OP_DIV, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2056 "d_parser.tab.c"
    break;

  case 63: /* expr: expr PLUS expr  */
#line 223 "d_parser.y"
                     { (yyval.expr) = CreateBinaryOpExpr(OP_PLUS, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2062 "d_parser.tab.c"
    break;

  case 64: /* expr: expr MINUS expr  */
#line 224 "d_parser.y"
                      { (yyval.expr) = CreateBinaryOpExpr(OP_MINUS, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2068 "d_parser.tab.c"
    break;

  case 65: /* expr: expr LT expr  */
#line 225 "d_parser.y"
                   { (yyval.expr) = CreateBinaryOpExpr(OP_LT, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2074 "d_parser.tab.c"
    break;

  case 66: /* expr: expr GT expr  */
#line 226 "d_parser.y"
                   { (yyval.expr) = CreateBinaryOpExpr(OP_GT, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2080 "d_parser.tab.c"
    break;

  case 67: /* expr: expr LE expr  */
#line 227 "d_parser.y"
                   { (yyval.expr) = CreateBinaryOpExpr(OP_LE, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2086 "d_parser.tab.c"
    break;

  case 68: /* expr: expr GE expr  */
#line 228 "d_parser.y"
                   { (yyval.expr) = CreateBinaryOpExpr(OP_GE, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2092 "d_parser.tab.c"
    break;

  case 69: /* expr: expr EQ expr  */
#line 229 "d_parser.y"
                   { (yyval.expr) = CreateBinaryOpExpr(OP_EQ, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2098 "d_parser.tab.c"
    break;

  case 70: /* expr: expr NEQ expr  */
#line 230 "d_parser.y"
                    { (yyval.expr) = CreateBinaryOpExpr(OP_NEQ, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2104 "d_parser.tab.c"
    break;

  case 71: /* expr: expr LAND expr  */
#line 231 "d_parser.y"
                     { (yyval.expr) = CreateBinaryOpExpr(OP_AND, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2110 "d_parser.tab.c"
    break;

  case 72: /* expr: expr LOR expr  */
#line 232 "d_parser.y"
                    { (yyval.expr) = CreateBinaryOpExpr(OP_OR, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2116 "d_parser.tab.c"
    break;

  case 73: /* expr: expr '=' expr  */
#line 233 "d_parser.y"
                    { (yyval.expr) = CreateAssignExpr(OP_ASSIGN, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2122 "d_parser.tab.c"
    break;

  case 74: /* expr: expr PLUSEQ expr  */
#line 234 "d_parser.y"
                       { (yyval.expr) = CreateAssignExpr(OP_PLUS_ASSIGN, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2128 "d_parser.tab.c"
    break;

  case 75: /* expr: expr MINUSEQ expr  */
#line 235 "d_parser.y"
                        { (yyval.expr) = CreateAssignExpr(OP_MINUS_ASSIGN, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2134 "d_parser.tab.c"
    break;

  case 76: /* expr: expr STAREQ expr  */
#line 236 "d_parser.y"
                       { (yyval.expr) = CreateAssignExpr(OP_MUL_ASSIGN, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2140 "d_parser.tab.c"
    break;

  case 77: /* expr: expr SLASHEQ expr  */
#line 237 "d_parser.y"
                        { (yyval.expr) = CreateAssignExpr(OP_DIV_ASSIGN, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2146 "d_parser.tab.c"
    break;

  case 78: /* expr: expr TILDEQ expr  */
#line 238 "d_parser.y"
                       { (yyval.expr) = CreateAssignExpr(OP_BITWISE_NOT_ASSIGN, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2152 "d_parser.tab.c"
    break;

  case 79: /* arg_list: expr  */
#line 242 "d_parser.y"
           { (yyval.expr_list) = CreateExprList(); AddExprToList((yyval.expr_list), (yyvsp[0].expr)); }
#line 2158 "d_parser.tab.c"
    break;

  case 80: /* arg_list: arg_list ',' expr  */
#line 243 "d_parser.y"
                        { (yyval.expr_list) = (yyvsp[-2].expr_list); AddExprToList((yyval.expr_list), (yyvsp[0].expr)); }
#line 2164 "d_parser.tab.c"
    break;

  case 81: /* init_decl: IDENT  */
#line 247 "d_parser.y"
            { (yyval.init_decl) = CreateInitDecl((yyvsp[0].string_val), NULL); }
#line 2170 "d_parser.tab.c"
    break;

  case 82: /* init_decl: IDENT '=' initializer  */
#line 248 "d_parser.y"
                            { (yyval.init_decl) = CreateInitDecl((yyvsp[-2].string_val), (yyvsp[0].initializer)); }
#line 2176 "d_parser.tab.c"
    break;

  case 83: /* init_decl_list: init_decl  */
#line 252 "d_parser.y"
                { (yyval.init_decl_list) = CreateInitDeclList(); AddInitDeclToList((yyval.init_decl_list), (yyvsp[0].init_decl)); }
#line 2182 "d_parser.tab.c"
    break;

  case 84: /* init_decl_list: init_decl_list ',' init_decl  */
#line 253 "d_parser.y"
                                   { (yyval.init_decl_list) = (yyvsp[-2].init_decl_list); AddInitDeclToList((yyval.init_decl_list), (yyvsp[0].init_decl)); }
#line 2188 "d_parser.tab.c"
    break;

  case 85: /* decl: type init_decl_list ';'  */
#line 257 "d_parser.y"
                              { (yyval.decl_stmt).type = (yyvsp[-2].type); (yyval.decl_stmt).init_decls = (yyvsp[-1].init_decl_list); }
#line 2194 "d_parser.tab.c"
    break;

  case 86: /* param: type IDENT  */
#line 261 "d_parser.y"
                 { (yyval.param) = CreateParam((yyvsp[-1].type), (yyvsp[0].string_val), 0, NULL); }
#line 2200 "d_parser.tab.c"
    break;

  case 87: /* param: REF type IDENT  */
#line 262 "d_parser.y"
                     { (yyval.param) = CreateParam((yyvsp[-1].type), (yyvsp[0].string_val), 1, NULL); }
#line 2206 "d_parser.tab.c"
    break;

  case 88: /* param: type IDENT '=' expr  */
#line 263 "d_parser.y"
                          { (yyval.param) = CreateParam((yyvsp[-3].type), (yyvsp[-2].string_val), 0, (yyvsp[0].expr)); }
#line 2212 "d_parser.tab.c"
    break;

  case 89: /* param: REF type IDENT '=' expr  */
#line 264 "d_parser.y"
                              { (yyval.param) = CreateParam((yyvsp[-3].type), (yyvsp[-2].string_val), 1, (yyvsp[0].expr)); }
#line 2218 "d_parser.tab.c"
    break;

  case 90: /* param_list: param  */
#line 268 "d_parser.y"
            { (yyval.param_list) = CreateParamList(); AddParamToList((yyval.param_list), (yyvsp[0].param)); }
#line 2224 "d_parser.tab.c"
    break;

  case 91: /* param_list: param_list ',' param  */
#line 269 "d_parser.y"
                           { (yyval.param_list) = (yyvsp[-2].param_list); AddParamToList((yyval.param_list), (yyvsp[0].param)); }
#line 2230 "d_parser.tab.c"
    break;

  case 92: /* stmt: e_expr ';'  */
#line 273 "d_parser.y"
                 { (yyval.stmt) = CreateExprStmt((yyvsp[-1].expr)); }
#line 2236 "d_parser.tab.c"
    break;

  case 93: /* stmt: decl  */
#line 274 "d_parser.y"
           { (yyval.stmt) = CreateDeclStmt((yyvsp[0].decl_stmt).type, (yyvsp[0].decl_stmt).init_decls); }
#line 2242 "d_parser.tab.c"
    break;

  case 94: /* stmt: compound_stmt  */
#line 275 "d_parser.y"
                    { (yyval.stmt) = (yyvsp[0].stmt); }
#line 2248 "d_parser.tab.c"
    break;

  case 95: /* stmt: if_stmt  */
#line 276 "d_parser.y"
              { (yyval.stmt) = (yyvsp[0].stmt); }
#line 2254 "d_parser.tab.c"
    break;

  case 96: /* stmt: while_stmt  */
#line 277 "d_parser.y"
                 { (yyval.stmt) = (yyvsp[0].stmt); }
#line 2260 "d_parser.tab.c"
    break;

  case 97: /* stmt: do_while_stmt  */
#line 278 "d_parser.y"
                    { (yyval.stmt) = (yyvsp[0].stmt); }
#line 2266 "d_parser.tab.c"
    break;

  case 98: /* stmt: for_stmt  */
#line 279 "d_parser.y"
               { (yyval.stmt) = (yyvsp[0].stmt); }
#line 2272 "d_parser.tab.c"
    break;

  case 99: /* stmt: foreach_stmt  */
#line 280 "d_parser.y"
                   { (yyval.stmt) = (yyvsp[0].stmt); }
#line 2278 "d_parser.tab.c"
    break;

  case 100: /* stmt: switch_stmt  */
#line 281 "d_parser.y"
                  { (yyval.stmt) = (yyvsp[0].stmt); }
#line 2284 "d_parser.tab.c"
    break;

  case 101: /* stmt: RETURN e_expr ';'  */
#line 282 "d_parser.y"
                        { (yyval.stmt) = CreateReturnStmt((yyvsp[-1].expr)); }
#line 2290 "d_parser.tab.c"
    break;

  case 102: /* stmt: BREAK ';'  */
#line 283 "d_parser.y"
                { (yyval.stmt) = CreateBreakStmt(); }
#line 2296 "d_parser.tab.c"
    break;

  case 103: /* stmt: CONTINUE ';'  */
#line 284 "d_parser.y"
                   { (yyval.stmt) = CreateContinueStmt(); }
#line 2302 "d_parser.tab.c"
    break;

  case 104: /* stmt: SUPER '(' arg_list ')' ';'  */
#line 285 "d_parser.y"
                                 { (yyval.stmt) = CreateSuperCtorCallStmt((yyvsp[-2].expr_list)); }
#line 2308 "d_parser.tab.c"
    break;

  case 105: /* stmt: SUPER '(' ')' ';'  */
#line 286 "d_parser.y"
                        { (yyval.stmt) = CreateSuperCtorCallStmt(NULL); }
#line 2314 "d_parser.tab.c"
    break;

  case 106: /* stmt: THIS '(' arg_list ')' ';'  */
#line 287 "d_parser.y"
                                { (yyval.stmt) = CreateThisCtorCallStmt((yyvsp[-2].expr_list)); }
#line 2320 "d_parser.tab.c"
    break;

  case 107: /* stmt: THIS '(' ')' ';'  */
#line 288 "d_parser.y"
                       { (yyval.stmt) = CreateThisCtorCallStmt(NULL); }
#line 2326 "d_parser.tab.c"
    break;

  case 108: /* stmt_list: stmt  */
#line 292 "d_parser.y"
           { (yyval.stmt_list) = CreateStmtList(); AddStmtToList((yyval.stmt_list), (yyvsp[0].stmt)); }
#line 2332 "d_parser.tab.c"
    break;

  case 109: /* stmt_list: stmt_list stmt  */
#line 293 "d_parser.y"
                     { (yyval.stmt_list) = (yyvsp[-1].stmt_list); AddStmtToList((yyval.stmt_list), (yyvsp[0].stmt)); }
#line 2338 "d_parser.tab.c"
    break;

  case 110: /* compound_stmt: '{' '}'  */
#line 297 "d_parser.y"
              { (yyval.stmt) = CreateCompoundStmt(NULL); }
#line 2344 "d_parser.tab.c"
    break;

  case 111: /* compound_stmt: '{' stmt_list '}'  */
#line 298 "d_parser.y"
                        { (yyval.stmt) = CreateCompoundStmt((yyvsp[-1].stmt_list)); }
#line 2350 "d_parser.tab.c"
    break;

  case 112: /* if_stmt: IF '(' expr ')' stmt  */
#line 302 "d_parser.y"
                                                 { (yyval.stmt) = CreateIfStmt((yyvsp[-2].expr), (yyvsp[0].stmt), NULL); }
#line 2356 "d_parser.tab.c"
    break;

  case 113: /* if_stmt: IF '(' expr ')' stmt ELSE stmt  */
#line 303 "d_parser.y"
                                     { (yyval.stmt) = CreateIfStmt((yyvsp[-4].expr), (yyvsp[-2].stmt), (yyvsp[0].stmt)); }
#line 2362 "d_parser.tab.c"
    break;

  case 114: /* while_stmt: WHILE '(' expr ')' stmt  */
#line 307 "d_parser.y"
                              { (yyval.stmt) = CreateWhileStmt((yyvsp[-2].expr), (yyvsp[0].stmt)); }
#line 2368 "d_parser.tab.c"
    break;

  case 115: /* do_while_stmt: DO stmt WHILE '(' expr ')' ';'  */
#line 311 "d_parser.y"
                                     { (yyval.stmt) = CreateDoWhileStmt((yyvsp[-5].stmt), (yyvsp[-2].expr)); }
#line 2374 "d_parser.tab.c"
    break;

  case 116: /* for_stmt: FOR '(' e_expr ';' e_expr ';' e_expr ')' stmt  */
#line 315 "d_parser.y"
                                                    { (yyval.stmt) = CreateForStmt((yyvsp[-6].expr), NULL, NULL, (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].stmt)); }
#line 2380 "d_parser.tab.c"
    break;

  case 117: /* for_stmt: FOR '(' decl e_expr ';' e_expr ')' stmt  */
#line 316 "d_parser.y"
                                              { (yyval.stmt) = CreateForStmt(NULL, (yyvsp[-5].decl_stmt).type, (yyvsp[-5].decl_stmt).init_decls, (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].stmt)); }
#line 2386 "d_parser.tab.c"
    break;

  case 118: /* foreach_stmt: FOREACH '(' IDENT ';' expr ')' stmt  */
#line 320 "d_parser.y"
                                          { (yyval.stmt) = CreateForeachStmt(0, NULL, (yyvsp[-4].string_val), (yyvsp[-2].expr), (yyvsp[0].stmt)); }
#line 2392 "d_parser.tab.c"
    break;

  case 119: /* foreach_stmt: FOREACH '(' type IDENT ';' expr ')' stmt  */
#line 321 "d_parser.y"
                                               { (yyval.stmt) = CreateForeachStmt(1, (yyvsp[-5].type), (yyvsp[-4].string_val), (yyvsp[-2].expr), (yyvsp[0].stmt)); }
#line 2398 "d_parser.tab.c"
    break;

  case 120: /* switch_stmt: SWITCH '(' expr ')' '{' case_list '}'  */
#line 325 "d_parser.y"
                                            { (yyval.stmt) = CreateSwitchStmt((yyvsp[-4].expr), (yyvsp[-1].case_list)->items, (yyvsp[-1].case_list)->count); }
#line 2404 "d_parser.tab.c"
    break;

  case 121: /* switch_stmt: SWITCH '(' expr ')' '{' '}'  */
#line 326 "d_parser.y"
                                  { (yyval.stmt) = CreateSwitchStmt((yyvsp[-3].expr), NULL, 0); }
#line 2410 "d_parser.tab.c"
    break;

  case 122: /* case_list: case_item  */
#line 330 "d_parser.y"
                { (yyval.case_list) = CreateCaseList(); AddCaseItemToList((yyval.case_list), (yyvsp[0].case_item)); }
#line 2416 "d_parser.tab.c"
    break;

  case 123: /* case_list: default_item  */
#line 331 "d_parser.y"
                   { (yyval.case_list) = CreateCaseList(); AddCaseItemToList((yyval.case_list), (yyvsp[0].case_item)); }
#line 2422 "d_parser.tab.c"
    break;

  case 124: /* case_list: case_list case_item  */
#line 332 "d_parser.y"
                          { (yyval.case_list) = (yyvsp[-1].case_list); AddCaseItemToList((yyval.case_list), (yyvsp[0].case_item)); }
#line 2428 "d_parser.tab.c"
    break;

  case 125: /* case_list: case_list default_item  */
#line 333 "d_parser.y"
                             { (yyval.case_list) = (yyvsp[-1].case_list); AddCaseItemToList((yyval.case_list), (yyvsp[0].case_item)); }
#line 2434 "d_parser.tab.c"
    break;

  case 126: /* case_item: CASE expr ':'  */
#line 337 "d_parser.y"
                    { (yyval.case_item) = CreateCaseItem((yyvsp[-1].expr), NULL); }
#line 2440 "d_parser.tab.c"
    break;

  case 127: /* case_item: CASE expr ':' stmt_list  */
#line 338 "d_parser.y"
                              { (yyval.case_item) = CreateCaseItem((yyvsp[-2].expr), (yyvsp[0].stmt_list)); }
#line 2446 "d_parser.tab.c"
    break;

  case 128: /* default_item: DEFAULT ':'  */
#line 342 "d_parser.y"
                  { (yyval.case_item) = CreateDefaultItem(NULL); }
#line 2452 "d_parser.tab.c"
    break;

  case 129: /* default_item: DEFAULT ':' stmt_list  */
#line 343 "d_parser.y"
                            { (yyval.case_item) = CreateDefaultItem((yyvsp[0].stmt_list)); }
#line 2458 "d_parser.tab.c"
    break;

  case 130: /* func_def: type IDENT '(' ')' func_body  */
#line 347 "d_parser.y"
                                   { (yyval.func_def) = CreateFuncDef((yyvsp[-4].type), (yyvsp[-3].string_val), NULL, (yyvsp[0].stmt)); }
#line 2464 "d_parser.tab.c"
    break;

  case 131: /* func_def: type IDENT '(' param_list ')' func_body  */
#line 348 "d_parser.y"
                                              { (yyval.func_def) = CreateFuncDef((yyvsp[-5].type), (yyvsp[-4].string_val), (yyvsp[-2].param_list), (yyvsp[0].stmt)); }
#line 2470 "d_parser.tab.c"
    break;

  case 132: /* func_def: VOID IDENT '(' ')' func_body  */
#line 349 "d_parser.y"
                                   { (yyval.func_def) = CreateVoidFuncDef((yyvsp[-3].string_val), NULL, (yyvsp[0].stmt)); }
#line 2476 "d_parser.tab.c"
    break;

  case 133: /* func_def: VOID IDENT '(' param_list ')' func_body  */
#line 350 "d_parser.y"
                                              { (yyval.func_def) = CreateVoidFuncDef((yyvsp[-4].string_val), (yyvsp[-2].param_list), (yyvsp[0].stmt)); }
#line 2482 "d_parser.tab.c"
    break;

  case 134: /* func_body: compound_stmt  */
#line 354 "d_parser.y"
                    { (yyval.stmt) = (yyvsp[0].stmt); }
#line 2488 "d_parser.tab.c"
    break;

  case 135: /* func_body: ';'  */
#line 355 "d_parser.y"
          { (yyval.stmt) = NULL; }
#line 2494 "d_parser.tab.c"
    break;

  case 136: /* class_def: CLASS CLASSNAME '{' class_members '}'  */
#line 359 "d_parser.y"
                                            { (yyval.class_def) = CreateClassDef((yyvsp[-3].string_val), NULL, (yyvsp[-1].class_member)); }
#line 2500 "d_parser.tab.c"
    break;

  case 137: /* class_def: CLASS CLASSNAME '{' '}'  */
#line 360 "d_parser.y"
                              { (yyval.class_def) = CreateClassDef((yyvsp[-2].string_val), NULL, NULL); }
#line 2506 "d_parser.tab.c"
    break;

  case 138: /* class_def: CLASS CLASSNAME ':' CLASSNAME '{' class_members '}'  */
#line 361 "d_parser.y"
                                                          { (yyval.class_def) = CreateClassDef((yyvsp[-5].string_val), (yyvsp[-3].string_val), (yyvsp[-1].class_member)); }
#line 2512 "d_parser.tab.c"
    break;

  case 139: /* class_def: CLASS CLASSNAME ':' CLASSNAME '{' '}'  */
#line 362 "d_parser.y"
                                            { (yyval.class_def) = CreateClassDef((yyvsp[-4].string_val), (yyvsp[-2].string_val), NULL); }
#line 2518 "d_parser.tab.c"
    break;

  case 140: /* class_members: class_member  */
#line 366 "d_parser.y"
                   { (yyval.class_member) = (yyvsp[0].class_member); }
#line 2524 "d_parser.tab.c"
    break;

  case 141: /* class_members: class_members class_member  */
#line 367 "d_parser.y"
                                 { (yyval.class_member) = AppendClassMember((yyvsp[-1].class_member), (yyvsp[0].class_member)); }
#line 2530 "d_parser.tab.c"
    break;

  case 142: /* class_member: decl  */
#line 371 "d_parser.y"
           { (yyval.class_member) = CreateFieldMember(ACCESS_PUBLIC, (yyvsp[0].decl_stmt).type, (yyvsp[0].decl_stmt).init_decls); }
#line 2536 "d_parser.tab.c"
    break;

  case 143: /* class_member: method_def  */
#line 372 "d_parser.y"
                 { (yyval.class_member) = CreateMethodMember(ACCESS_PUBLIC, (yyvsp[0].method_def)); }
#line 2542 "d_parser.tab.c"
    break;

  case 144: /* class_member: access_spec decl  */
#line 373 "d_parser.y"
                       { (yyval.class_member) = CreateFieldMember((yyvsp[-1].access_spec), (yyvsp[0].decl_stmt).type, (yyvsp[0].decl_stmt).init_decls); }
#line 2548 "d_parser.tab.c"
    break;

  case 145: /* class_member: access_spec method_def  */
#line 374 "d_parser.y"
                             { (yyval.class_member) = CreateMethodMember((yyvsp[-1].access_spec), (yyvsp[0].method_def)); }
#line 2554 "d_parser.tab.c"
    break;

  case 146: /* class_member: ctor_def  */
#line 375 "d_parser.y"
               { (yyval.class_member) = CreateCtorMember(ACCESS_PUBLIC, (yyvsp[0].ctor_def)); }
#line 2560 "d_parser.tab.c"
    break;

  case 147: /* class_member: dtor_def  */
#line 376 "d_parser.y"
               { (yyval.class_member) = CreateDtorMember(ACCESS_PUBLIC, (yyvsp[0].dtor_def)); }
#line 2566 "d_parser.tab.c"
    break;

  case 148: /* class_member: enum_def  */
#line 377 "d_parser.y"
               { (yyval.class_member) = CreateEnumMember(ACCESS_PUBLIC, (yyvsp[0].enum_def)); }
#line 2572 "d_parser.tab.c"
    break;

  case 149: /* class_member: access_spec enum_def  */
#line 378 "d_parser.y"
                           { (yyval.class_member) = CreateEnumMember((yyvsp[-1].access_spec), (yyvsp[0].enum_def)); }
#line 2578 "d_parser.tab.c"
    break;

  case 150: /* access_spec: PUBLIC  */
#line 382 "d_parser.y"
             { (yyval.access_spec) = ACCESS_PUBLIC; }
#line 2584 "d_parser.tab.c"
    break;

  case 151: /* access_spec: PRIVATE  */
#line 383 "d_parser.y"
              { (yyval.access_spec) = ACCESS_PRIVATE; }
#line 2590 "d_parser.tab.c"
    break;

  case 152: /* access_spec: PROTECTED  */
#line 384 "d_parser.y"
                { (yyval.access_spec) = ACCESS_PROTECTED; }
#line 2596 "d_parser.tab.c"
    break;

  case 153: /* method_def: type IDENT '(' ')' compound_stmt  */
#line 388 "d_parser.y"
                                       { (yyval.method_def) = CreateMethodDef(0, (yyvsp[-4].type), (yyvsp[-3].string_val), NULL, (yyvsp[0].stmt)); }
#line 2602 "d_parser.tab.c"
    break;

  case 154: /* method_def: type IDENT '(' param_list ')' compound_stmt  */
#line 389 "d_parser.y"
                                                  { (yyval.method_def) = CreateMethodDef(0, (yyvsp[-5].type), (yyvsp[-4].string_val), (yyvsp[-2].param_list), (yyvsp[0].stmt)); }
#line 2608 "d_parser.tab.c"
    break;

  case 155: /* method_def: VOID IDENT '(' ')' compound_stmt  */
#line 390 "d_parser.y"
                                       { (yyval.method_def) = CreateVoidMethodDef(0, (yyvsp[-3].string_val), NULL, (yyvsp[0].stmt)); }
#line 2614 "d_parser.tab.c"
    break;

  case 156: /* method_def: VOID IDENT '(' param_list ')' compound_stmt  */
#line 391 "d_parser.y"
                                                  { (yyval.method_def) = CreateVoidMethodDef(0, (yyvsp[-4].string_val), (yyvsp[-2].param_list), (yyvsp[0].stmt)); }
#line 2620 "d_parser.tab.c"
    break;

  case 157: /* method_def: OVERRIDE type IDENT '(' ')' compound_stmt  */
#line 392 "d_parser.y"
                                                { (yyval.method_def) = CreateMethodDef(1, (yyvsp[-4].type), (yyvsp[-3].string_val), NULL, (yyvsp[0].stmt)); }
#line 2626 "d_parser.tab.c"
    break;

  case 158: /* method_def: OVERRIDE type IDENT '(' param_list ')' compound_stmt  */
#line 393 "d_parser.y"
                                                           { (yyval.method_def) = CreateMethodDef(1, (yyvsp[-5].type), (yyvsp[-4].string_val), (yyvsp[-2].param_list), (yyvsp[0].stmt)); }
#line 2632 "d_parser.tab.c"
    break;

  case 159: /* method_def: OVERRIDE VOID IDENT '(' ')' compound_stmt  */
#line 394 "d_parser.y"
                                                { (yyval.method_def) = CreateVoidMethodDef(1, (yyvsp[-3].string_val), NULL, (yyvsp[0].stmt)); }
#line 2638 "d_parser.tab.c"
    break;

  case 160: /* method_def: OVERRIDE VOID IDENT '(' param_list ')' compound_stmt  */
#line 395 "d_parser.y"
                                                           { (yyval.method_def) = CreateVoidMethodDef(1, (yyvsp[-4].string_val), (yyvsp[-2].param_list), (yyvsp[0].stmt)); }
#line 2644 "d_parser.tab.c"
    break;

  case 161: /* ctor_def: THIS '(' ')' compound_stmt  */
#line 399 "d_parser.y"
                                 { (yyval.ctor_def) = CreateCtorDef(NULL, (yyvsp[0].stmt)); }
#line 2650 "d_parser.tab.c"
    break;

  case 162: /* ctor_def: THIS '(' param_list ')' compound_stmt  */
#line 400 "d_parser.y"
                                            { (yyval.ctor_def) = CreateCtorDef((yyvsp[-2].param_list), (yyvsp[0].stmt)); }
#line 2656 "d_parser.tab.c"
    break;

  case 163: /* dtor_def: '~' THIS '(' ')' compound_stmt  */
#line 404 "d_parser.y"
                                     { (yyval.dtor_def) = CreateDtorDef((yyvsp[0].stmt)); }
#line 2662 "d_parser.tab.c"
    break;

  case 164: /* enum_def: ENUM IDENT '{' enum_body '}'  */
#line 408 "d_parser.y"
                                   { (yyval.enum_def) = CreateEnumDef((yyvsp[-3].string_val), (yyvsp[-1].enum_item_list)->items, (yyvsp[-1].enum_item_list)->count); }
#line 2668 "d_parser.tab.c"
    break;

  case 165: /* enum_def: ENUM ENUMNAME '{' enum_body '}'  */
#line 409 "d_parser.y"
                                      { (yyval.enum_def) = CreateEnumDef((yyvsp[-3].string_val), (yyvsp[-1].enum_item_list)->items, (yyvsp[-1].enum_item_list)->count); }
#line 2674 "d_parser.tab.c"
    break;

  case 166: /* enum_def: ENUM '{' enum_body '}'  */
#line 410 "d_parser.y"
                             { (yyval.enum_def) = CreateEnumDef(NULL, (yyvsp[-1].enum_item_list)->items, (yyvsp[-1].enum_item_list)->count); }
#line 2680 "d_parser.tab.c"
    break;

  case 167: /* enum_item_name: IDENT  */
#line 414 "d_parser.y"
            { (yyval.string_val) = (yyvsp[0].string_val); }
#line 2686 "d_parser.tab.c"
    break;

  case 168: /* enum_item_name: CLASSNAME  */
#line 415 "d_parser.y"
                { (yyval.string_val) = (yyvsp[0].string_val); }
#line 2692 "d_parser.tab.c"
    break;

  case 169: /* enum_item_name: ENUMNAME  */
#line 416 "d_parser.y"
               { (yyval.string_val) = (yyvsp[0].string_val); }
#line 2698 "d_parser.tab.c"
    break;

  case 170: /* enum_body: enum_item_name  */
#line 420 "d_parser.y"
                     { (yyval.enum_item_list) = CreateEnumItemList(); AddEnumItemToList((yyval.enum_item_list), CreateEnumItem((yyvsp[0].string_val), 0, 0)); }
#line 2704 "d_parser.tab.c"
    break;

  case 171: /* enum_body: enum_item_name '=' INTEGER  */
#line 421 "d_parser.y"
                                 { (yyval.enum_item_list) = CreateEnumItemList(); AddEnumItemToList((yyval.enum_item_list), CreateEnumItem((yyvsp[-2].string_val), 1, (yyvsp[0].int_val))); }
#line 2710 "d_parser.tab.c"
    break;

  case 172: /* enum_body: enum_body ',' enum_item_name  */
#line 422 "d_parser.y"
                                   { (yyval.enum_item_list) = (yyvsp[-2].enum_item_list); AddEnumItemToList((yyval.enum_item_list), CreateEnumItem((yyvsp[0].string_val), 0, 0)); }
#line 2716 "d_parser.tab.c"
    break;

  case 173: /* enum_body: enum_body ',' enum_item_name '=' INTEGER  */
#line 423 "d_parser.y"
                                               { (yyval.enum_item_list) = (yyvsp[-4].enum_item_list); AddEnumItemToList((yyval.enum_item_list), CreateEnumItem((yyvsp[-2].string_val), 1, (yyvsp[0].int_val))); }
#line 2722 "d_parser.tab.c"
    break;


#line 2726 "d_parser.tab.c"

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

#line 426 "d_parser.y"


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

