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

extern int yylex(void);
extern int yylineno;
extern char *yytext;

void yyerror(const char *s);

#line 79 "d_parser.tab.c"

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
  YYSYMBOL_TRUE = 33,                      /* TRUE  */
  YYSYMBOL_FALSE = 34,                     /* FALSE  */
  YYSYMBOL_NULL_CONST = 35,                /* NULL_CONST  */
  YYSYMBOL_NAN_CONST = 36,                 /* NAN_CONST  */
  YYSYMBOL_DOTDOT = 37,                    /* DOTDOT  */
  YYSYMBOL_INTEGER = 38,                   /* INTEGER  */
  YYSYMBOL_FLOATVAL = 39,                  /* FLOATVAL  */
  YYSYMBOL_STR = 40,                       /* STR  */
  YYSYMBOL_CHARVAL = 41,                   /* CHARVAL  */
  YYSYMBOL_IDENT = 42,                     /* IDENT  */
  YYSYMBOL_F_IDENT = 43,                   /* F_IDENT  */
  YYSYMBOL_CLASSNAME = 44,                 /* CLASSNAME  */
  YYSYMBOL_ASSIGN = 45,                    /* ASSIGN  */
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
  YYSYMBOL_73_ = 73,                       /* '='  */
  YYSYMBOL_74_ = 74,                       /* ';'  */
  YYSYMBOL_75_ = 75,                       /* '{'  */
  YYSYMBOL_76_ = 76,                       /* '}'  */
  YYSYMBOL_77_ = 77,                       /* ':'  */
  YYSYMBOL_78_ = 78,                       /* '~'  */
  YYSYMBOL_YYACCEPT = 79,                  /* $accept  */
  YYSYMBOL_program = 80,                   /* program  */
  YYSYMBOL_translation_unit = 81,          /* translation_unit  */
  YYSYMBOL_source_item = 82,               /* source_item  */
  YYSYMBOL_base_type = 83,                 /* base_type  */
  YYSYMBOL_array_decl = 84,                /* array_decl  */
  YYSYMBOL_type = 85,                      /* type  */
  YYSYMBOL_array_init = 86,                /* array_init  */
  YYSYMBOL_initializer = 87,               /* initializer  */
  YYSYMBOL_e_expr = 88,                    /* e_expr  */
  YYSYMBOL_expr = 89,                      /* expr  */
  YYSYMBOL_arg_list = 90,                  /* arg_list  */
  YYSYMBOL_init_decl = 91,                 /* init_decl  */
  YYSYMBOL_init_decl_list = 92,            /* init_decl_list  */
  YYSYMBOL_decl = 93,                      /* decl  */
  YYSYMBOL_param = 94,                     /* param  */
  YYSYMBOL_param_list = 95,                /* param_list  */
  YYSYMBOL_stmt = 96,                      /* stmt  */
  YYSYMBOL_stmt_list = 97,                 /* stmt_list  */
  YYSYMBOL_compound_stmt = 98,             /* compound_stmt  */
  YYSYMBOL_if_stmt = 99,                   /* if_stmt  */
  YYSYMBOL_while_stmt = 100,               /* while_stmt  */
  YYSYMBOL_do_while_stmt = 101,            /* do_while_stmt  */
  YYSYMBOL_for_stmt = 102,                 /* for_stmt  */
  YYSYMBOL_foreach_stmt = 103,             /* foreach_stmt  */
  YYSYMBOL_switch_stmt = 104,              /* switch_stmt  */
  YYSYMBOL_case_list = 105,                /* case_list  */
  YYSYMBOL_case_item = 106,                /* case_item  */
  YYSYMBOL_default_item = 107,             /* default_item  */
  YYSYMBOL_func_def = 108,                 /* func_def  */
  YYSYMBOL_func_body = 109,                /* func_body  */
  YYSYMBOL_class_def = 110,                /* class_def  */
  YYSYMBOL_class_members = 111,            /* class_members  */
  YYSYMBOL_class_member = 112,             /* class_member  */
  YYSYMBOL_access_spec = 113,              /* access_spec  */
  YYSYMBOL_method_def = 114,               /* method_def  */
  YYSYMBOL_ctor_def = 115,                 /* ctor_def  */
  YYSYMBOL_dtor_def = 116,                 /* dtor_def  */
  YYSYMBOL_enum_def = 117,                 /* enum_def  */
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
#define YYFINAL  27
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1503

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  79
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  40
/* YYNRULES -- Number of rules.  */
#define YYNRULES  161
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  343

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   321


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
       2,     2,     2,     2,     2,     2,     2,     2,    77,    74,
       2,    73,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    67,     2,    68,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    75,     2,    76,    78,     2,     2,     2,
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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    71
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    50,    50,    51,    55,    56,    60,    61,    62,    63,
      67,    68,    69,    70,    71,    72,    73,    77,    78,    82,
      83,    84,    85,    89,    90,    94,    95,    99,   100,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   153,   154,   158,   159,   163,
     164,   168,   172,   173,   174,   175,   179,   180,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     199,   200,   204,   205,   209,   210,   214,   218,   222,   223,
     227,   228,   232,   233,   237,   238,   239,   240,   244,   245,
     249,   250,   254,   255,   256,   257,   261,   262,   266,   267,
     268,   269,   273,   274,   278,   279,   280,   281,   282,   283,
     284,   285,   289,   290,   291,   295,   296,   297,   298,   299,
     300,   301,   302,   306,   307,   311,   315,   316,   320,   321,
     322,   323
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
  "PROTECTED", "THIS", "REF", "NEW", "ENUM", "TRUE", "FALSE", "NULL_CONST",
  "NAN_CONST", "DOTDOT", "INTEGER", "FLOATVAL", "STR", "CHARVAL", "IDENT",
  "F_IDENT", "CLASSNAME", "ASSIGN", "PLUSEQ", "MINUSEQ", "STAREQ",
  "SLASHEQ", "TILDEQ", "LOR", "LAND", "EQ", "NEQ", "LT", "GT", "LE", "GE",
  "PLUS", "MINUS", "STAR", "SLASH", "UNOT", "UMINUS", "UPLUS", "'.'",
  "'['", "']'", "'('", "')'", "LOWER_THAN_ELSE", "','", "'='", "';'",
  "'{'", "'}'", "':'", "'~'", "$accept", "program", "translation_unit",
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

#define YYPACT_NINF (-313)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-29)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     170,  -313,  -313,  -313,  -313,  -313,  -313,  -313,   -41,   -27,
     -36,   -58,    25,   170,  -313,   -58,    38,  -313,  -313,  -313,
    -313,   -26,   -14,   -13,    -8,   -18,  -313,  -313,  -313,  -313,
     -15,   -20,  -313,    -2,   322,   270,    29,    -8,     9,   -35,
      19,  -313,  1016,   664,    82,  -313,   139,    37,    92,  -313,
      16,    88,    23,  -313,  -313,  -313,    99,  -313,   140,    78,
    -313,   279,  -313,   443,  -313,  -313,  -313,  -313,   120,   -17,
     152,   166,  -313,  -313,   144,  -313,   520,  -313,  -313,  -313,
    -313,  -313,  -313,  -313,  -313,  -313,  1094,  1094,  1094,  1053,
    1094,  -313,  -313,  1434,    37,    67,  -313,   176,  -313,   401,
    -313,  -313,   147,    37,   237,   154,   181,   185,   671,   161,
     163,  -313,  -313,  -313,  -313,  -313,   312,  -313,  -313,   160,
     107,   167,   168,   171,    10,    10,    10,  -313,  1434,   -32,
    1177,  1094,  1094,  1094,  1094,  1094,  1094,  1094,  1094,  1094,
    1094,  1094,  1094,  1094,  1094,  1094,  1094,  1094,  1094,   162,
    1094,   848,  -313,    37,   164,   179,   180,   183,   622,   187,
     188,  1094,   165,   184,  -313,    82,   186,  1434,  -313,  -313,
     475,  -313,  -313,  -313,  -313,  -313,  -313,  -313,  1094,  -313,
    -313,   713,   190,   194,   189,    84,   195,   720,  -313,   355,
     216,  -313,   197,   890,  1094,  1094,  -313,  1094,  -313,  1434,
    1434,  1434,  1434,  1434,  1434,   544,   768,   836,   836,    56,
      56,    56,    56,    66,    66,    10,    10,  -313,   200,  1119,
    -313,    89,  -313,  1094,  1094,  1094,  1094,   254,   811,   369,
     217,  -313,  -313,  -313,  -313,  -313,  1434,   189,    90,   762,
     769,  -313,   189,   189,   189,    94,  -313,  -313,   932,  -313,
      95,  1359,  1384,  1434,   974,  1094,  -313,  -313,  1434,  1203,
    1229,  1255,   202,   218,  1094,   219,   248,  -313,  -313,   189,
     189,   115,   189,   116,  -313,  -313,  -313,   189,  -313,   119,
    -313,  -313,  -313,  -313,   126,  1409,   622,   220,   622,  1094,
    1094,   226,  1094,   227,  -313,  -313,   189,  -313,   189,  -313,
    -313,  -313,  -313,   292,    -7,  -313,  1281,   235,  1094,  1307,
    1094,  -313,  -313,   622,  1094,   233,  -313,    -1,  -313,  -313,
     238,  1094,   243,   622,  1333,  -313,  1144,   549,  -313,  -313,
    -313,  -313,   262,   622,  -313,   622,   549,   549,   622,  -313,
    -313,   549,  -313
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,    10,    11,    12,    13,    14,    15,    16,     0,     0,
       0,    21,     0,     2,     4,    19,     0,     8,     6,     7,
       9,     0,     0,     0,     0,     0,    22,     1,     5,    20,
      77,     0,    79,     0,     0,     0,     0,     0,   158,     0,
       0,    17,     0,     0,     0,    81,     0,     0,     0,    86,
       0,     0,     0,   142,   143,   144,     0,   129,     0,     0,
     134,     0,   132,     0,   135,   138,   139,   140,     0,     0,
       0,     0,   157,    18,     0,    38,     0,    34,    35,    36,
      37,    30,    31,    33,    32,    29,     0,     0,     0,     0,
       0,    26,    78,    25,     0,     0,    80,     0,   127,    28,
     126,   124,    82,     0,     0,     0,     0,     0,     0,     0,
       0,   128,   133,   136,   137,   141,     0,   156,   159,   160,
       0,     0,     0,     0,    55,    54,    56,    23,    75,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   122,     0,    83,     0,     0,     0,    28,     0,
       0,    28,     0,     0,   102,     0,     0,    27,    89,   100,
      28,    90,    91,    92,    93,    94,    95,    96,     0,   125,
      87,     0,     0,     0,     0,     0,     0,     0,   131,     0,
       0,    51,     0,     0,     0,     0,    24,     0,    39,    69,
      70,    71,    72,    73,    74,    68,    67,    65,    66,    61,
      62,    63,    64,    59,    60,    57,    58,    46,     0,     0,
      49,     0,   123,     0,     0,     0,     0,     0,    28,     0,
       0,    98,    99,    88,   103,   101,    84,     0,     0,     0,
       0,   153,     0,     0,     0,     0,   130,   161,     0,    41,
       0,     0,     0,    76,     0,     0,    44,    50,    85,     0,
       0,     0,     0,     0,    28,     0,     0,    97,   147,     0,
       0,     0,     0,     0,   154,   155,   145,     0,    52,     0,
      42,    40,    43,    47,     0,     0,    28,     0,    28,     0,
      28,     0,     0,     0,   148,   151,     0,   149,     0,   146,
      53,    48,    45,   104,     0,   106,     0,     0,    28,     0,
       0,   152,   150,    28,     0,     0,   113,     0,   114,   115,
       0,    28,     0,    28,     0,   105,     0,   120,   112,   116,
     117,   107,     0,    28,   110,    28,   118,   121,    28,   109,
     111,   119,   108
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -313,  -313,  -313,   311,   -23,   318,    22,  -313,  -313,  -150,
     -42,  -141,   290,  -313,     3,   231,   -24,  -116,  -312,   -43,
    -313,  -313,  -313,  -313,  -313,  -313,  -313,    20,    26,  -313,
     -82,  -313,   229,   -60,  -313,   284,  -313,  -313,     5,   305
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,    12,    13,    14,    15,    26,   165,    91,    92,   166,
     167,   129,    32,    33,   168,    49,    50,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   317,   318,   319,    18,
     101,    19,    61,    62,    63,    64,    65,    66,    67,    39
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      93,   112,    21,    17,   100,    20,    23,   314,   315,    25,
     221,   230,   152,   314,   315,   337,    17,    22,    20,    95,
      40,   179,    16,    97,   341,    27,     1,     2,     3,     4,
       5,     6,     7,   106,    38,    16,   196,    71,    60,    24,
     197,    72,   227,    34,   124,   125,   126,   128,   130,    43,
      41,   100,   250,   123,   235,    71,    48,    59,    42,   117,
     100,    35,    37,    36,    60,    48,   113,    11,   115,   316,
      44,   222,    45,    68,   107,   328,   149,   150,   263,   151,
      30,    31,    70,    59,   185,    59,   103,    73,   104,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   279,   219,   128,
     100,    98,    99,   284,   291,   145,   146,   147,   148,    60,
      30,   110,   149,   150,    30,   151,    48,   147,   148,   112,
      48,   105,   149,   150,   102,   151,   236,   153,    59,   104,
     307,   241,     1,     2,     3,     4,     5,     6,     7,   191,
     192,   128,   251,   252,   242,   253,   104,   238,   322,   257,
     269,   197,   104,   245,   277,   280,   104,   197,   108,   109,
     303,   332,   305,     1,     2,     3,     4,     5,     6,     7,
       8,   258,   259,   260,   261,   296,   298,   104,   104,   300,
     118,   197,    60,     9,   268,   116,   301,   325,   197,   274,
     275,   276,    10,    48,   217,   218,   128,   334,   119,    48,
     120,    59,   128,   285,    11,   271,   273,   339,   154,   340,
     178,   235,   342,   181,   182,   235,   294,   295,   183,   297,
     186,   264,   187,   190,   299,   194,   193,   223,   195,   231,
       1,     2,     3,     4,     5,     6,     7,   306,   224,   225,
     309,   266,   226,   311,   247,   312,   228,   229,   232,   239,
     233,    48,    48,   240,    99,   243,   248,    46,   324,   254,
     262,   289,   326,     1,     2,     3,     4,     5,     6,     7,
      51,    11,     1,     2,     3,     4,     5,     6,     7,    51,
     293,   267,   290,   292,    52,   304,    53,    54,    55,    56,
     308,   310,    10,    52,   313,    53,    54,    55,    56,   321,
     327,    10,   331,   333,    11,     1,     2,     3,     4,     5,
       6,     7,    51,    11,    28,     1,     2,     3,     4,     5,
       6,     7,   338,    29,    96,   180,    52,   329,    53,    54,
      55,    56,    69,   330,    10,   189,    57,   114,    58,     0,
       0,     0,    46,     0,     0,   111,    11,    58,     1,     2,
       3,     4,     5,     6,     7,    51,    11,     0,     0,     0,
       0,     0,     1,     2,     3,     4,     5,     6,     7,    52,
       0,    53,    54,    55,    56,     0,     0,    10,   188,     0,
      58,     0,    47,     0,     0,     0,     0,     0,     0,    11,
       0,     0,     0,     0,     1,     2,     3,     4,     5,     6,
       7,   265,   155,    11,   156,     0,     0,   157,   158,   159,
     160,   161,   162,   163,     0,     0,    74,     0,     0,     0,
      75,   246,    76,    58,    77,    78,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    11,     1,     2,     3,     4,
       5,     6,     7,    51,     0,     0,     0,     0,     0,     0,
      86,    87,     0,     0,    88,     0,     0,    52,     0,     0,
      90,     0,     0,     0,     0,    10,    99,   164,     1,     2,
       3,     4,     5,     6,     7,     0,   155,    11,   156,     0,
       0,   157,   158,   159,   160,   161,   162,   163,     0,     0,
      74,     0,     0,     0,    75,     0,    76,     0,    77,    78,
      79,    80,     0,    81,    82,    83,    84,    85,     0,    11,
       0,     0,     0,     1,     2,     3,     4,     5,     6,     7,
       0,     0,     0,     0,    86,    87,     0,     0,    88,     0,
       0,     0,     0,     0,    90,     0,     0,     0,     0,     0,
      99,   234,     1,     2,     3,     4,     5,     6,     7,     0,
     155,     0,   156,   121,   122,   157,   158,   159,   160,   161,
     162,   163,     0,     0,    74,     0,     0,     0,    75,     0,
      76,     0,    77,    78,    79,    80,     0,    81,    82,    83,
      84,    85,     0,    11,     0,     0,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,     0,    86,    87,
     149,   150,    88,   151,     0,     0,     0,     0,    90,     0,
       0,     0,     0,   -28,    99,     1,     2,     3,     4,     5,
       6,     7,     0,   155,     0,   156,     0,     0,   157,   158,
     159,   160,   161,   162,   163,     0,     0,    74,     0,     0,
       0,    75,     0,    76,     0,    77,    78,    79,    80,     0,
      81,    82,    83,    84,    85,     0,    11,     1,     2,     3,
       4,     5,     6,     7,     1,     2,     3,     4,     5,     6,
       7,    86,    87,     0,     0,    88,     0,     0,     0,     0,
       0,    90,     0,     0,    46,     0,     0,    99,     0,     0,
       0,    46,     0,     0,     0,     0,     0,     0,    11,     0,
       0,     0,     0,     0,     0,    11,     1,     2,     3,     4,
       5,     6,     7,     1,     2,     3,     4,     5,     6,     7,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,   184,     0,    46,     0,     0,     0,     0,     0,     0,
      46,     0,     0,     0,     0,     0,     0,    11,     0,     0,
       0,     0,     0,     0,    11,     1,     2,     3,     4,     5,
       6,     7,     1,     2,     3,     4,     5,     6,     7,     0,
       0,     0,     0,   237,     0,     0,     0,     0,     0,     0,
     244,     0,    46,     0,     0,     0,     0,     0,     0,    46,
       0,     0,     0,     0,     0,     0,    11,     0,     0,     0,
       0,     0,     0,    11,     1,     2,     3,     4,     5,     6,
       7,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,     0,   270,     0,   149,   150,    74,   151,     0,   272,
      75,     0,    76,     0,    77,    78,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    11,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,    87,     0,    74,    88,     0,     0,    75,     0,    76,
      90,    77,    78,    79,    80,     0,    81,    82,    83,    84,
      85,   141,   142,   143,   144,   145,   146,   147,   148,     0,
       0,     0,   149,   150,     0,   151,     0,    86,    87,     0,
       0,    88,     0,     0,     0,    74,     0,    90,   220,    75,
       0,    76,     0,    77,    78,    79,    80,     0,    81,    82,
      83,    84,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
      87,     0,     0,    88,     0,     0,     0,    74,     0,    90,
     249,    75,     0,    76,     0,    77,    78,    79,    80,     0,
      81,    82,    83,    84,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,    87,     0,     0,    88,     0,     0,     0,    74,
       0,    90,   278,    75,     0,    76,     0,    77,    78,    79,
      80,     0,    81,    82,    83,    84,    85,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,    87,     0,     0,    88,     0,     0,
       0,    74,     0,    90,   283,    75,     0,    76,     0,    77,
      78,    79,    80,     0,    81,    82,    83,    84,    85,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,    87,     0,    74,    88,
       0,     0,    75,    89,    76,    90,    77,    78,    79,    80,
       0,    81,    82,    83,    84,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    86,    87,     0,     0,    88,     0,     0,    74,
       0,   127,    90,    75,     0,    76,     0,    77,    78,    79,
      80,     0,    81,    82,    83,    84,    85,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,    87,     0,   255,    88,     0,     0,
       0,     0,     0,    90,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,     0,     0,     0,   149,   150,   256,   151,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,     0,     0,     0,
     149,   150,     0,   151,     0,     0,     0,     0,     0,     0,
       0,   336,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
       0,     0,     0,   149,   150,     0,   151,   198,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,     0,     0,     0,   149,
     150,     0,   151,   286,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,     0,     0,     0,   149,   150,     0,   151,   287,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,     0,     0,
       0,   149,   150,     0,   151,   288,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,     0,     0,     0,   149,   150,     0,
     151,   320,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
       0,     0,     0,   149,   150,     0,   151,   323,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,     0,     0,     0,   149,
     150,     0,   151,   335,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,     0,     0,     0,   149,   150,   281,   151,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,     0,     0,     0,
     149,   150,   282,   151,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,     0,     0,     0,   149,   150,   302,   151,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,     0,     0,     0,
     149,   150,     0,   151
};

static const yytype_int16 yycheck[] =
{
      42,    61,    43,     0,    47,     0,    42,    14,    15,    67,
     151,   161,    94,    14,    15,   327,    13,    44,    13,    43,
      38,   103,     0,    46,   336,     0,     3,     4,     5,     6,
       7,     8,     9,    10,    42,    13,    68,    72,    35,    75,
      72,    76,   158,    69,    86,    87,    88,    89,    90,    69,
      68,    94,   193,    76,   170,    72,    34,    35,    73,    76,
     103,    75,    75,    77,    61,    43,    63,    44,    63,    76,
      72,   153,    74,    44,    52,    76,    66,    67,   228,    69,
      42,    43,    73,    61,   108,    63,    70,    68,    72,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   248,   150,   151,
     153,    74,    75,   254,   264,    59,    60,    61,    62,   116,
      42,    43,    66,    67,    42,    69,   104,    61,    62,   189,
     108,    43,    66,    67,    42,    69,   178,    70,   116,    72,
     290,   184,     3,     4,     5,     6,     7,     8,     9,    42,
      43,   193,   194,   195,    70,   197,    72,   181,   308,    70,
      70,    72,    72,   187,    70,    70,    72,    72,    69,    29,
     286,   321,   288,     3,     4,     5,     6,     7,     8,     9,
      10,   223,   224,   225,   226,    70,    70,    72,    72,    70,
      38,    72,   189,    23,   237,    75,    70,   313,    72,   242,
     243,   244,    32,   181,    42,    43,   248,   323,    42,   187,
      66,   189,   254,   255,    44,   239,   240,   333,    42,   335,
      73,   337,   338,    69,    43,   341,   269,   270,    43,   272,
      69,   228,    69,    73,   277,    67,    69,    73,    67,    74,
       3,     4,     5,     6,     7,     8,     9,   289,    69,    69,
     292,   229,    69,   296,    38,   298,    69,    69,    74,    69,
      74,   239,   240,    69,    75,    70,    69,    30,   310,    69,
      16,    69,   314,     3,     4,     5,     6,     7,     8,     9,
      10,    44,     3,     4,     5,     6,     7,     8,     9,    10,
      42,    74,    74,    74,    24,    75,    26,    27,    28,    29,
      74,    74,    32,    24,    12,    26,    27,    28,    29,    74,
      77,    32,    74,    70,    44,     3,     4,     5,     6,     7,
       8,     9,    10,    44,    13,     3,     4,     5,     6,     7,
       8,     9,    70,    15,    44,   104,    24,   317,    26,    27,
      28,    29,    37,   317,    32,   116,    76,    63,    78,    -1,
      -1,    -1,    30,    -1,    -1,    76,    44,    78,     3,     4,
       5,     6,     7,     8,     9,    10,    44,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,     9,    24,
      -1,    26,    27,    28,    29,    -1,    -1,    32,    76,    -1,
      78,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
       9,    42,    11,    44,    13,    -1,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    25,    -1,    -1,    -1,
      29,    76,    31,    78,    33,    34,    35,    36,    -1,    38,
      39,    40,    41,    42,    -1,    44,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    -1,    24,    -1,    -1,
      69,    -1,    -1,    -1,    -1,    32,    75,    76,     3,     4,
       5,     6,     7,     8,     9,    -1,    11,    44,    13,    -1,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      25,    -1,    -1,    -1,    29,    -1,    31,    -1,    33,    34,
      35,    36,    -1,    38,    39,    40,    41,    42,    -1,    44,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,     9,
      -1,    -1,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,
      75,    76,     3,     4,     5,     6,     7,     8,     9,    -1,
      11,    -1,    13,    43,    44,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    25,    -1,    -1,    -1,    29,    -1,
      31,    -1,    33,    34,    35,    36,    -1,    38,    39,    40,
      41,    42,    -1,    44,    -1,    -1,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    -1,    59,    60,
      66,    67,    63,    69,    -1,    -1,    -1,    -1,    69,    -1,
      -1,    -1,    -1,    74,    75,     3,     4,     5,     6,     7,
       8,     9,    -1,    11,    -1,    13,    -1,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    25,    -1,    -1,
      -1,    29,    -1,    31,    -1,    33,    34,    35,    36,    -1,
      38,    39,    40,    41,    42,    -1,    44,     3,     4,     5,
       6,     7,     8,     9,     3,     4,     5,     6,     7,     8,
       9,    59,    60,    -1,    -1,    63,    -1,    -1,    -1,    -1,
      -1,    69,    -1,    -1,    30,    -1,    -1,    75,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    44,     3,     4,     5,     6,
       7,     8,     9,     3,     4,     5,     6,     7,     8,     9,
      -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    44,     3,     4,     5,     6,     7,
       8,     9,     3,     4,     5,     6,     7,     8,     9,    -1,
      -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    44,     3,     4,     5,     6,     7,     8,
       9,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    -1,    70,    -1,    66,    67,    25,    69,    -1,    70,
      29,    -1,    31,    -1,    33,    34,    35,    36,    -1,    38,
      39,    40,    41,    42,    -1,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      59,    60,    -1,    25,    63,    -1,    -1,    29,    -1,    31,
      69,    33,    34,    35,    36,    -1,    38,    39,    40,    41,
      42,    55,    56,    57,    58,    59,    60,    61,    62,    -1,
      -1,    -1,    66,    67,    -1,    69,    -1,    59,    60,    -1,
      -1,    63,    -1,    -1,    -1,    25,    -1,    69,    70,    29,
      -1,    31,    -1,    33,    34,    35,    36,    -1,    38,    39,
      40,    41,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    -1,    -1,    25,    -1,    69,
      70,    29,    -1,    31,    -1,    33,    34,    35,    36,    -1,
      38,    39,    40,    41,    42,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    -1,    -1,    25,
      -1,    69,    70,    29,    -1,    31,    -1,    33,    34,    35,
      36,    -1,    38,    39,    40,    41,    42,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    -1,
      -1,    25,    -1,    69,    70,    29,    -1,    31,    -1,    33,
      34,    35,    36,    -1,    38,    39,    40,    41,    42,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    59,    60,    -1,    25,    63,
      -1,    -1,    29,    67,    31,    69,    33,    34,    35,    36,
      -1,    38,    39,    40,    41,    42,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    -1,    25,
      -1,    68,    69,    29,    -1,    31,    -1,    33,    34,    35,
      36,    -1,    38,    39,    40,    41,    42,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    59,    60,    -1,    37,    63,    -1,    -1,
      -1,    -1,    -1,    69,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    -1,    -1,    -1,    66,    67,    68,    69,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    -1,    -1,    -1,
      66,    67,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      -1,    -1,    -1,    66,    67,    -1,    69,    70,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    -1,    -1,    -1,    66,
      67,    -1,    69,    70,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    -1,    -1,    -1,    66,    67,    -1,    69,    70,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    -1,    -1,
      -1,    66,    67,    -1,    69,    70,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    -1,    -1,    -1,    66,    67,    -1,
      69,    70,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      -1,    -1,    -1,    66,    67,    -1,    69,    70,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    -1,    -1,    -1,    66,
      67,    -1,    69,    70,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    -1,    -1,    -1,    66,    67,    68,    69,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    -1,    -1,    -1,
      66,    67,    68,    69,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    -1,    -1,    -1,    66,    67,    68,    69,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    -1,    -1,    -1,
      66,    67,    -1,    69
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    23,
      32,    44,    80,    81,    82,    83,    85,    93,   108,   110,
     117,    43,    44,    42,    75,    67,    84,     0,    82,    84,
      42,    43,    91,    92,    69,    75,    77,    75,    42,   118,
      38,    68,    73,    69,    72,    74,    30,    70,    85,    94,
      95,    10,    24,    26,    27,    28,    29,    76,    78,    85,
      93,   111,   112,   113,   114,   115,   116,   117,    44,   118,
      73,    72,    76,    68,    25,    29,    31,    33,    34,    35,
      36,    38,    39,    40,    41,    42,    59,    60,    63,    67,
      69,    86,    87,    89,    70,    95,    91,    83,    74,    75,
      98,   109,    42,    70,    72,    43,    10,    85,    69,    29,
      43,    76,   112,    93,   114,   117,    75,    76,    38,    42,
      66,    43,    44,    83,    89,    89,    89,    68,    89,    90,
      89,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    66,
      67,    69,   109,    70,    42,    11,    13,    16,    17,    18,
      19,    20,    21,    22,    76,    85,    88,    89,    93,    96,
      97,    98,    99,   100,   101,   102,   103,   104,    73,   109,
      94,    69,    43,    43,    70,    95,    69,    69,    76,   111,
      73,    42,    43,    69,    67,    67,    68,    72,    70,    89,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    89,    89,    42,    43,    89,
      70,    90,   109,    73,    69,    69,    69,    96,    69,    69,
      88,    74,    74,    74,    76,    96,    89,    70,    95,    69,
      69,    98,    70,    70,    70,    95,    76,    38,    69,    70,
      90,    89,    89,    89,    69,    37,    68,    70,    89,    89,
      89,    89,    16,    88,    93,    42,    85,    74,    98,    70,
      70,    95,    70,    95,    98,    98,    98,    70,    70,    90,
      70,    68,    68,    70,    90,    89,    70,    70,    70,    69,
      74,    88,    74,    42,    98,    98,    70,    98,    70,    98,
      70,    70,    68,    96,    75,    96,    89,    88,    74,    89,
      74,    98,    98,    12,    14,    15,    76,   105,   106,   107,
      70,    74,    88,    70,    89,    96,    89,    77,    76,   106,
     107,    74,    88,    70,    96,    70,    77,    97,    70,    96,
      96,    97,    96
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    79,    80,    80,    81,    81,    82,    82,    82,    82,
      83,    83,    83,    83,    83,    83,    83,    84,    84,    85,
      85,    85,    85,    86,    86,    87,    87,    88,    88,    89,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    90,    90,    91,    91,    92,
      92,    93,    94,    94,    94,    94,    95,    95,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      97,    97,    98,    98,    99,    99,   100,   101,   102,   102,
     103,   103,   104,   104,   105,   105,   105,   105,   106,   106,
     107,   107,   108,   108,   108,   108,   109,   109,   110,   110,
     110,   110,   111,   111,   112,   112,   112,   112,   112,   112,
     112,   112,   113,   113,   113,   114,   114,   114,   114,   114,
     114,   114,   114,   115,   115,   116,   117,   117,   118,   118,
     118,   118
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

#line 1648 "d_parser.tab.c"

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

#line 326 "d_parser.y"


void yyerror(const char *s) {
    fprintf(stderr, "Parse error at line %d: %s\n", yylineno, s);
}
