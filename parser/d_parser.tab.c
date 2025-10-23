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
 // Пролог
#include <iostream>
#include <cstdlib>
#include <cstdio>

using namespace std;

extern int yylex(void);

void yyerror(char const* s) {
    cout << s << endl;
}


#line 86 "d_parser.tab.c"

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
  YYSYMBOL_MODULE = 3,                     /* MODULE  */
  YYSYMBOL_IMPORT = 4,                     /* IMPORT  */
  YYSYMBOL_CONST = 5,                      /* CONST  */
  YYSYMBOL_FUNC = 6,                       /* FUNC  */
  YYSYMBOL_VAR = 7,                        /* VAR  */
  YYSYMBOL_RETURN = 8,                     /* RETURN  */
  YYSYMBOL_BREAK = 9,                      /* BREAK  */
  YYSYMBOL_CONTINUE = 10,                  /* CONTINUE  */
  YYSYMBOL_IF = 11,                        /* IF  */
  YYSYMBOL_ELSE = 12,                      /* ELSE  */
  YYSYMBOL_SWITCH = 13,                    /* SWITCH  */
  YYSYMBOL_CASE = 14,                      /* CASE  */
  YYSYMBOL_DEFAULT = 15,                   /* DEFAULT  */
  YYSYMBOL_DO = 16,                        /* DO  */
  YYSYMBOL_WHILE = 17,                     /* WHILE  */
  YYSYMBOL_FOR = 18,                       /* FOR  */
  YYSYMBOL_FOREACH = 19,                   /* FOREACH  */
  YYSYMBOL_CLASS = 20,                     /* CLASS  */
  YYSYMBOL_ENUM = 21,                      /* ENUM  */
  YYSYMBOL_NEW = 22,                       /* NEW  */
  YYSYMBOL_THIS = 23,                      /* THIS  */
  YYSYMBOL_SUPER = 24,                     /* SUPER  */
  YYSYMBOL_OVERRIDE = 25,                  /* OVERRIDE  */
  YYSYMBOL_PUBLIC = 26,                    /* PUBLIC  */
  YYSYMBOL_PRIVATE = 27,                   /* PRIVATE  */
  YYSYMBOL_PROTECTED = 28,                 /* PROTECTED  */
  YYSYMBOL_AUTO = 29,                      /* AUTO  */
  YYSYMBOL_REF = 30,                       /* REF  */
  YYSYMBOL_INT = 31,                       /* INT  */
  YYSYMBOL_FLOAT = 32,                     /* FLOAT  */
  YYSYMBOL_DOUBLE = 33,                    /* DOUBLE  */
  YYSYMBOL_REAL = 34,                      /* REAL  */
  YYSYMBOL_BOOL = 35,                      /* BOOL  */
  YYSYMBOL_CHAR = 36,                      /* CHAR  */
  YYSYMBOL_STRING = 37,                    /* STRING  */
  YYSYMBOL_VOID = 38,                      /* VOID  */
  YYSYMBOL_INT_LIT = 39,                   /* INT_LIT  */
  YYSYMBOL_FLOAT_LIT = 40,                 /* FLOAT_LIT  */
  YYSYMBOL_BOOL_LIT = 41,                  /* BOOL_LIT  */
  YYSYMBOL_ID = 42,                        /* ID  */
  YYSYMBOL_STRING_LIT = 43,                /* STRING_LIT  */
  YYSYMBOL_PLUS_ASSIGN = 44,               /* PLUS_ASSIGN  */
  YYSYMBOL_MINUS_ASSIGN = 45,              /* MINUS_ASSIGN  */
  YYSYMBOL_MUL_ASSIGN = 46,                /* MUL_ASSIGN  */
  YYSYMBOL_DIV_ASSIGN = 47,                /* DIV_ASSIGN  */
  YYSYMBOL_TILDE_ASSIGN = 48,              /* TILDE_ASSIGN  */
  YYSYMBOL_EQ = 49,                        /* EQ  */
  YYSYMBOL_NE = 50,                        /* NE  */
  YYSYMBOL_LE = 51,                        /* LE  */
  YYSYMBOL_GE = 52,                        /* GE  */
  YYSYMBOL_AND_OP = 53,                    /* AND_OP  */
  YYSYMBOL_OR_OP = 54,                     /* OR_OP  */
  YYSYMBOL_INC = 55,                       /* INC  */
  YYSYMBOL_DEC = 56,                       /* DEC  */
  YYSYMBOL_57_ = 57,                       /* '='  */
  YYSYMBOL_58_ = 58,                       /* '<'  */
  YYSYMBOL_59_ = 59,                       /* '>'  */
  YYSYMBOL_60_ = 60,                       /* '+'  */
  YYSYMBOL_61_ = 61,                       /* '-'  */
  YYSYMBOL_62_ = 62,                       /* '*'  */
  YYSYMBOL_63_ = 63,                       /* '/'  */
  YYSYMBOL_64_ = 64,                       /* '!'  */
  YYSYMBOL_UMINUS = 65,                    /* UMINUS  */
  YYSYMBOL_66_ = 66,                       /* ';'  */
  YYSYMBOL_67_ = 67,                       /* '('  */
  YYSYMBOL_68_ = 68,                       /* ')'  */
  YYSYMBOL_69_ = 69,                       /* '.'  */
  YYSYMBOL_70_ = 70,                       /* ','  */
  YYSYMBOL_71_ = 71,                       /* '{'  */
  YYSYMBOL_72_ = 72,                       /* '}'  */
  YYSYMBOL_73_ = 73,                       /* '['  */
  YYSYMBOL_74_ = 74,                       /* ']'  */
  YYSYMBOL_75_ = 75,                       /* ':'  */
  YYSYMBOL_YYACCEPT = 76,                  /* $accept  */
  YYSYMBOL_program = 77,                   /* program  */
  YYSYMBOL_module_clause = 78,             /* module_clause  */
  YYSYMBOL_e_import_decl_list = 79,        /* e_import_decl_list  */
  YYSYMBOL_import_decl_list = 80,          /* import_decl_list  */
  YYSYMBOL_import_decl = 81,               /* import_decl  */
  YYSYMBOL_e_import_spec_list = 82,        /* e_import_spec_list  */
  YYSYMBOL_import_spec_list = 83,          /* import_spec_list  */
  YYSYMBOL_import_spec = 84,               /* import_spec  */
  YYSYMBOL_e_top_level_decl_list = 85,     /* e_top_level_decl_list  */
  YYSYMBOL_top_level_decl_list = 86,       /* top_level_decl_list  */
  YYSYMBOL_top_level_decl = 87,            /* top_level_decl  */
  YYSYMBOL_decl = 88,                      /* decl  */
  YYSYMBOL_func_decl = 89,                 /* func_decl  */
  YYSYMBOL_signature = 90,                 /* signature  */
  YYSYMBOL_params = 91,                    /* params  */
  YYSYMBOL_param_list = 92,                /* param_list  */
  YYSYMBOL_param_decl = 93,                /* param_decl  */
  YYSYMBOL_results = 94,                   /* results  */
  YYSYMBOL_block = 95,                     /* block  */
  YYSYMBOL_stmt_list = 96,                 /* stmt_list  */
  YYSYMBOL_stmt = 97,                      /* stmt  */
  YYSYMBOL_simple_stmt = 98,               /* simple_stmt  */
  YYSYMBOL_return_stmt = 99,               /* return_stmt  */
  YYSYMBOL_const_decl = 100,               /* const_decl  */
  YYSYMBOL_const_spec_list = 101,          /* const_spec_list  */
  YYSYMBOL_const_spec = 102,               /* const_spec  */
  YYSYMBOL_var_decl = 103,                 /* var_decl  */
  YYSYMBOL_var_spec_list = 104,            /* var_spec_list  */
  YYSYMBOL_var_spec = 105,                 /* var_spec  */
  YYSYMBOL_id_list = 106,                  /* id_list  */
  YYSYMBOL_type = 107,                     /* type  */
  YYSYMBOL_type_name = 108,                /* type_name  */
  YYSYMBOL_type_list = 109,                /* type_list  */
  YYSYMBOL_type_lit = 110,                 /* type_lit  */
  YYSYMBOL_array_type = 111,               /* array_type  */
  YYSYMBOL_func_type = 112,                /* func_type  */
  YYSYMBOL_slice_type = 113,               /* slice_type  */
  YYSYMBOL_expr_list = 114,                /* expr_list  */
  YYSYMBOL_expr = 115,                     /* expr  */
  YYSYMBOL_primary_expr = 116,             /* primary_expr  */
  YYSYMBOL_operand = 117,                  /* operand  */
  YYSYMBOL_operand_name = 118,             /* operand_name  */
  YYSYMBOL_if_stmt = 119,                  /* if_stmt  */
  YYSYMBOL_switch_stmt = 120,              /* switch_stmt  */
  YYSYMBOL_case_list = 121,                /* case_list  */
  YYSYMBOL_case_item = 122,                /* case_item  */
  YYSYMBOL_for_stmt = 123,                 /* for_stmt  */
  YYSYMBOL_while_stmt = 124,               /* while_stmt  */
  YYSYMBOL_do_while_stmt = 125,            /* do_while_stmt  */
  YYSYMBOL_foreach_stmt = 126,             /* foreach_stmt  */
  YYSYMBOL_class_decl = 127,               /* class_decl  */
  YYSYMBOL_class_inherit_opt = 128,        /* class_inherit_opt  */
  YYSYMBOL_class_body_opt = 129,           /* class_body_opt  */
  YYSYMBOL_class_body = 130,               /* class_body  */
  YYSYMBOL_class_member = 131,             /* class_member  */
  YYSYMBOL_enum_decl = 132,                /* enum_decl  */
  YYSYMBOL_enum_member_list = 133,         /* enum_member_list  */
  YYSYMBOL_enum_member = 134               /* enum_member  */
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
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1013

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  76
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  59
/* YYNRULES -- Number of rules.  */
#define YYNRULES  169
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  324

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   312


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
       2,     2,     2,    64,     2,     2,     2,     2,     2,     2,
      67,    68,    62,    60,    70,    61,    69,    63,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    75,    66,
      58,    57,    59,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    73,     2,    74,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    71,     2,    72,     2,     2,     2,     2,
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
      55,    56,    65
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   104,   104,   108,   112,   113,   117,   118,   122,   123,
     127,   128,   132,   133,   137,   138,   139,   143,   144,   148,
     149,   153,   154,   155,   156,   160,   161,   165,   166,   170,
     171,   175,   176,   180,   181,   185,   186,   190,   191,   195,
     196,   200,   201,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   232,   233,   237,   238,   242,   243,
     247,   248,   249,   253,   254,   258,   259,   263,   264,   265,
     269,   270,   274,   275,   276,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   292,   293,   297,   298,   299,   303,
     307,   311,   315,   316,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,   338,   342,   343,   344,   345,   346,   347,   348,
     349,   353,   354,   355,   359,   360,   361,   365,   366,   367,
     371,   372,   376,   377,   381,   382,   386,   387,   388,   389,
     393,   397,   401,   402,   406,   410,   411,   415,   416,   420,
     421,   425,   426,   427,   431,   432,   436,   437,   441,   442
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
  "\"end of file\"", "error", "\"invalid token\"", "MODULE", "IMPORT",
  "CONST", "FUNC", "VAR", "RETURN", "BREAK", "CONTINUE", "IF", "ELSE",
  "SWITCH", "CASE", "DEFAULT", "DO", "WHILE", "FOR", "FOREACH", "CLASS",
  "ENUM", "NEW", "THIS", "SUPER", "OVERRIDE", "PUBLIC", "PRIVATE",
  "PROTECTED", "AUTO", "REF", "INT", "FLOAT", "DOUBLE", "REAL", "BOOL",
  "CHAR", "STRING", "VOID", "INT_LIT", "FLOAT_LIT", "BOOL_LIT", "ID",
  "STRING_LIT", "PLUS_ASSIGN", "MINUS_ASSIGN", "MUL_ASSIGN", "DIV_ASSIGN",
  "TILDE_ASSIGN", "EQ", "NE", "LE", "GE", "AND_OP", "OR_OP", "INC", "DEC",
  "'='", "'<'", "'>'", "'+'", "'-'", "'*'", "'/'", "'!'", "UMINUS", "';'",
  "'('", "')'", "'.'", "','", "'{'", "'}'", "'['", "']'", "':'", "$accept",
  "program", "module_clause", "e_import_decl_list", "import_decl_list",
  "import_decl", "e_import_spec_list", "import_spec_list", "import_spec",
  "e_top_level_decl_list", "top_level_decl_list", "top_level_decl", "decl",
  "func_decl", "signature", "params", "param_list", "param_decl",
  "results", "block", "stmt_list", "stmt", "simple_stmt", "return_stmt",
  "const_decl", "const_spec_list", "const_spec", "var_decl",
  "var_spec_list", "var_spec", "id_list", "type", "type_name", "type_list",
  "type_lit", "array_type", "func_type", "slice_type", "expr_list", "expr",
  "primary_expr", "operand", "operand_name", "if_stmt", "switch_stmt",
  "case_list", "case_item", "for_stmt", "while_stmt", "do_while_stmt",
  "foreach_stmt", "class_decl", "class_inherit_opt", "class_body_opt",
  "class_body", "class_member", "enum_decl", "enum_member_list",
  "enum_member", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-246)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-104)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      12,    10,    55,    -1,  -246,  -246,    80,   -19,    23,    80,
      28,    54,   -30,    59,  -246,   209,    49,  -246,  -246,    56,
     -30,    63,  -246,   -32,    98,    -9,   101,   107,  -246,   209,
      93,  -246,  -246,  -246,  -246,  -246,  -246,  -246,  -246,   112,
    -246,  -246,   141,  -246,   237,   119,   141,  -246,   300,   114,
     116,   125,  -246,  -246,   -54,   129,   119,  -246,  -246,  -246,
    -246,  -246,  -246,  -246,  -246,  -246,   528,   151,   133,   142,
     128,  -246,  -246,  -246,  -246,    85,   131,   343,    69,   140,
     528,   152,   169,   137,   170,  -246,   141,  -246,  -246,  -246,
    -246,  -246,  -246,  -246,  -246,  -246,  -246,   528,   528,   528,
     149,   935,   148,  -246,   150,  -246,   398,   643,   528,   398,
     156,  -246,    71,  -246,   355,  -246,   241,  -246,  -246,  -246,
    -246,   141,  -246,  -246,   149,   528,  -246,   127,   171,    76,
    -246,  -246,  -246,  -246,   759,   528,   528,   528,   528,   528,
     528,   528,   528,   528,   528,   528,   528,   528,   121,   398,
    -246,   398,   149,  -246,   -10,  -246,   411,  -246,   528,  -246,
    -246,   164,    -4,   131,   168,   535,   172,   -26,  -246,  -246,
    -246,   280,   174,  -246,  -246,    61,   -23,   717,  -246,  -246,
    -246,  -246,  -246,  -246,  -246,   149,   230,   175,  -246,   165,
     127,  -246,   528,   -31,  -246,  -246,   935,    50,    50,    50,
      50,   266,   950,    50,    50,   -20,   -20,  -246,  -246,   474,
     562,    29,  -246,   398,  -246,  -246,   149,   528,   528,   166,
     236,   528,   360,   694,   431,  -246,   181,  -246,   528,   528,
     528,   528,   528,   528,  -246,  -246,  -246,  -246,  -246,  -246,
     935,  -246,  -246,  -246,   589,  -246,   482,  -246,  -246,   779,
     799,   528,   180,    11,  -246,   189,   819,   521,   195,   739,
    -246,   187,   221,  -246,   149,   149,   149,   149,   149,   149,
    -246,   528,  -246,   607,   131,   196,   625,   469,  -246,  -246,
     528,   131,   198,   899,   528,   528,   201,   660,  -246,   528,
     265,   166,   469,   469,   839,  -246,   131,   210,   917,   859,
     528,  -246,   677,    -5,    14,   469,   220,  -246,   131,   567,
     131,   879,  -246,  -246,  -246,  -246,  -246,  -246,   224,  -246,
     131,   131,  -246,  -246
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     3,     1,     5,     0,     0,     4,
       0,    14,    11,     0,     8,    18,     0,     7,    16,     0,
      10,     0,    15,     0,     0,     0,     0,     0,     2,    17,
       0,    21,    22,    25,    26,    23,    24,     6,     9,     0,
      13,    81,     0,    66,    70,     0,     0,    73,     0,   156,
       0,     0,    20,    12,     0,     0,     0,    85,    86,    87,
      88,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      82,    84,    96,    97,    98,     0,    27,    30,     0,     0,
       0,    77,     0,     0,     0,    19,     0,    67,    69,   100,
     135,   136,   105,   106,   108,   134,   107,     0,     0,     0,
      71,   103,   104,   123,   131,    80,     0,     0,     0,     0,
      81,    32,     0,    34,     0,    36,     0,    28,    37,    29,
      38,     0,    74,    76,    79,     0,   155,   158,   168,     0,
     167,    68,   122,   121,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,    72,    95,     0,    31,     0,    35,    64,    46,
      47,     0,     0,     0,     0,     0,     0,   134,    40,    43,
      48,     0,     0,    44,    45,     0,     0,    55,    49,    50,
      51,    52,    53,    54,    75,    78,     0,     0,   162,     0,
     157,   160,     0,     0,   164,   133,   102,   113,   114,   117,
     118,   119,   120,   115,   116,   109,   110,   111,   112,     0,
       0,     0,    99,     0,    83,    33,    65,     0,     0,     0,
       0,     0,     0,     0,     0,    39,     0,    42,     0,     0,
       0,     0,     0,     0,    56,    57,   163,   161,   154,   159,
     169,   165,   166,   125,     0,   124,     0,   132,    94,     0,
       0,     0,     0,     0,   143,     0,     0,     0,     0,   103,
     149,    93,     0,    41,    59,    60,    61,    62,    63,    58,
     127,     0,   126,     0,     0,     0,     0,     0,   141,   142,
       0,     0,     0,     0,     0,     0,     0,     0,   128,     0,
     137,     0,     0,   145,     0,   150,     0,     0,     0,     0,
       0,   129,     0,     0,     0,   144,     0,   148,     0,     0,
       0,     0,   130,   138,   139,   140,   151,   147,     0,   152,
       0,     0,   153,   146
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -246,  -246,  -246,  -246,  -246,   286,  -246,  -246,    34,  -246,
    -246,   271,   -13,   -90,   245,   232,  -246,   155,  -246,   -76,
    -241,  -167,  -217,  -246,  -246,  -246,   -33,  -246,  -246,   -39,
      15,   -21,  -246,   190,  -246,  -246,  -246,  -246,   -63,   -67,
    -246,  -246,  -246,    27,  -246,    52,  -245,  -246,  -246,  -246,
    -246,  -246,  -246,  -246,  -246,   124,  -246,  -246,   147
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     2,     3,     8,     9,    10,    19,    20,    14,    28,
      29,    30,   169,    32,    76,    77,   112,   113,   119,   170,
     171,   172,   173,   174,    33,    54,    43,    34,    78,    47,
     175,   115,    70,   154,    71,    72,    73,    74,   176,   101,
     102,   103,   104,   178,   179,   253,   254,   180,   181,   182,
     183,    35,    83,   189,   190,   191,    36,   129,   130
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     117,   107,    31,   100,   226,   258,   161,    79,   279,    55,
      41,   128,    86,    11,    87,     1,    31,   124,   -81,   -81,
     -81,   -81,   -81,    69,    11,   251,   252,    81,   251,   252,
     132,   133,   134,    41,   233,    42,   293,   188,    44,    13,
      48,   241,   146,   147,   -81,   152,    21,   135,    12,   177,
      13,   305,     4,   131,    39,     5,   120,    44,    46,   279,
     213,    48,   185,   218,   214,     6,   116,   219,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     208,   210,   184,   278,     7,   150,   315,   220,   153,    15,
     114,    56,   318,   157,    17,   216,   236,    18,   223,   213,
     188,    44,    22,   247,   177,   228,   229,   230,   231,   232,
     144,   145,   146,   147,   187,    37,    57,    58,    59,    60,
      61,    62,    63,    64,    38,   240,   226,   110,   153,    40,
     212,    67,    23,    24,    25,   121,    48,   122,   226,   155,
      45,   156,   244,    49,    90,    91,   193,   260,   194,    50,
     249,   250,   186,   111,   256,   259,    90,    91,    68,    52,
      92,    93,    94,    95,    96,   264,   265,   266,   267,   268,
     269,   114,    92,    93,    94,    95,    96,   187,    53,   273,
     251,   252,    97,    41,   276,    98,    75,    84,    99,    82,
     283,    85,   248,   105,    97,    88,   209,    98,   290,   108,
      99,   109,   116,   262,   287,   295,   123,   106,   127,   125,
     177,   126,   128,   294,    23,    24,    25,   298,   299,   135,
     307,   148,   302,   149,   -93,   177,   177,   313,   192,    26,
      27,   217,   317,   311,   319,   221,    24,   238,   177,   224,
     227,   237,   177,    56,   322,   323,    23,   263,    25,   158,
     159,   160,   161,   255,   162,   277,   280,   163,   164,   165,
     166,   284,   285,   286,    90,    91,   296,   291,    57,    58,
      59,    60,    61,    62,    63,    64,   300,   303,   308,    65,
      92,    93,    94,   167,    96,    23,   316,    25,   158,   159,
     160,   161,   321,   162,    66,    16,   163,   164,   165,   166,
      51,    89,    97,    90,    91,    98,    56,    67,    99,   118,
      68,   215,   116,   168,   239,   136,   137,   138,   139,    92,
      93,    94,   167,    96,   142,   143,   144,   145,   146,   147,
     314,    57,    58,    59,    60,    61,    62,    63,    64,   211,
     242,    97,    65,   304,    98,     0,     0,    99,     0,    56,
       0,   116,   225,     0,     0,     0,     0,    80,     0,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
      67,     0,     0,    68,    57,    58,    59,    60,    61,    62,
      63,    64,     0,    90,    91,    65,    57,    58,    59,    60,
      61,    62,    63,    64,     0,     0,     0,    65,     0,    92,
      93,    94,   167,    96,    56,     0,     0,     0,     0,     0,
      75,     0,     0,     0,     0,     0,    68,    56,     0,     0,
       0,    97,     0,     0,    98,    67,   257,    99,    68,    57,
      58,    59,    60,    61,    62,    63,    64,    56,     0,     0,
      65,     0,    57,    58,    59,    60,    61,    62,    63,    64,
       0,     0,     0,   110,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,    59,    60,    61,    62,    63,    64,
       0,    68,     0,   261,    23,     0,    25,   158,   159,   160,
     161,     0,   162,     0,    68,   163,   164,   165,   166,     0,
       0,     0,    90,    91,     0,     0,     0,    90,    91,     0,
       0,     0,     0,     0,    68,    90,    91,     0,    92,    93,
      94,   167,    96,    92,    93,    94,    95,    96,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,    98,     0,    97,    99,     0,    98,     0,
     116,    99,     0,    97,    90,    91,    98,     0,   243,    99,
       0,    90,    91,     0,     0,     0,   272,     0,    90,    91,
      92,    93,    94,    95,    96,     0,     0,    92,    93,    94,
      95,    96,     0,     0,    92,    93,    94,    95,    96,     0,
       0,     0,    97,     0,     0,    98,     0,   282,    99,    97,
      90,    91,    98,     0,     0,    99,    97,     0,     0,    98,
       0,     0,   222,     0,     0,     0,    92,    93,    94,   167,
      96,   136,   137,   138,   139,   140,   141,     0,     0,     0,
     142,   143,   144,   145,   146,   147,     0,     0,    97,     0,
       0,    98,     0,     0,    99,     0,   245,   246,   136,   137,
     138,   139,   140,   141,     0,     0,     0,   142,   143,   144,
     145,   146,   147,     0,     0,     0,   136,   137,   138,   139,
     140,   141,     0,   270,   271,   142,   143,   144,   145,   146,
     147,     0,     0,     0,   136,   137,   138,   139,   140,   141,
       0,   288,   289,   142,   143,   144,   145,   146,   147,     0,
       0,     0,   136,   137,   138,   139,   140,   141,     0,     0,
     292,   142,   143,   144,   145,   146,   147,     0,     0,   136,
     137,   138,   139,   140,   141,     0,     0,   151,   142,   143,
     144,   145,   146,   147,     0,     0,   136,   137,   138,   139,
     140,   141,     0,     0,   301,   142,   143,   144,   145,   146,
     147,     0,     0,   136,   137,   138,   139,   140,   141,     0,
       0,   312,   142,   143,   144,   145,   146,   147,     0,     0,
       0,     0,     0,     0,     0,   116,   136,   137,   138,   139,
     140,   141,   234,   235,  -103,   142,   143,   144,   145,   146,
     147,     0,     0,     0,     0,     0,     0,  -103,   136,   137,
     138,   139,   140,   141,   234,   235,     0,   142,   143,   144,
     145,   146,   147,     0,     0,   -55,     0,   195,   136,   137,
     138,   139,   140,   141,     0,     0,     0,   142,   143,   144,
     145,   146,   147,     0,     0,     0,     0,   195,   136,   137,
     138,   139,   140,   141,     0,     0,     0,   142,   143,   144,
     145,   146,   147,     0,     0,     0,     0,   274,   136,   137,
     138,   139,   140,   141,     0,     0,     0,   142,   143,   144,
     145,   146,   147,     0,     0,     0,     0,   275,   136,   137,
     138,   139,   140,   141,     0,     0,     0,   142,   143,   144,
     145,   146,   147,     0,     0,     0,     0,   281,   136,   137,
     138,   139,   140,   141,     0,     0,     0,   142,   143,   144,
     145,   146,   147,     0,     0,     0,     0,   306,   136,   137,
     138,   139,   140,   141,     0,     0,     0,   142,   143,   144,
     145,   146,   147,     0,     0,     0,     0,   310,   136,   137,
     138,   139,   140,   141,     0,     0,     0,   142,   143,   144,
     145,   146,   147,     0,     0,     0,     0,   320,   136,   137,
     138,   139,   140,   141,     0,     0,     0,   142,   143,   144,
     145,   146,   147,     0,     0,   297,   136,   137,   138,   139,
     140,   141,     0,     0,     0,   142,   143,   144,   145,   146,
     147,     0,     0,   309,   136,   137,   138,   139,   140,   141,
       0,     0,     0,   142,   143,   144,   145,   146,   147,   136,
     137,   138,   139,   140,     0,     0,     0,     0,   142,   143,
     144,   145,   146,   147
};

static const yytype_int16 yycheck[] =
{
      76,    68,    15,    66,   171,   222,    11,    46,   253,    42,
      42,    42,    66,    43,    68,     3,    29,    80,    44,    45,
      46,    47,    48,    44,    43,    14,    15,    48,    14,    15,
      97,    98,    99,    42,    57,    67,   277,   127,    23,    69,
      25,    72,    62,    63,    70,   108,    12,    70,    67,   116,
      69,   292,    42,    86,    20,     0,    77,    42,    67,   304,
      70,    46,   125,    67,    74,    66,    71,    71,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   121,    72,     4,   106,    72,   163,   109,    66,
      75,     6,   309,   114,    66,   158,   186,    43,   165,    70,
     190,    86,    43,    74,   171,    44,    45,    46,    47,    48,
      60,    61,    62,    63,   127,    66,    31,    32,    33,    34,
      35,    36,    37,    38,    68,   192,   293,    42,   149,    66,
     151,    70,     5,     6,     7,    66,   121,    68,   305,    68,
      42,    70,   209,    42,    23,    24,    70,   223,    72,    42,
     217,   218,    25,    68,   221,   222,    23,    24,    73,    66,
      39,    40,    41,    42,    43,   228,   229,   230,   231,   232,
     233,   156,    39,    40,    41,    42,    43,   190,    66,   246,
      14,    15,    61,    42,   251,    64,    67,    71,    67,    75,
     257,    66,   213,    42,    61,    66,    75,    64,   274,    57,
      67,    73,    71,   224,   271,   281,    66,    74,    71,    57,
     277,    42,    42,   280,     5,     6,     7,   284,   285,    70,
     296,    73,   289,    73,    68,   292,   293,   303,    57,    20,
      21,    67,   308,   300,   310,    67,     6,    72,   305,    67,
      66,    66,   309,     6,   320,   321,     5,    66,     7,     8,
       9,    10,    11,    17,    13,    75,    67,    16,    17,    18,
      19,    66,    75,    42,    23,    24,    68,    71,    31,    32,
      33,    34,    35,    36,    37,    38,    75,    12,    68,    42,
      39,    40,    41,    42,    43,     5,    66,     7,     8,     9,
      10,    11,    68,    13,    57,     9,    16,    17,    18,    19,
      29,    56,    61,    23,    24,    64,     6,    70,    67,    77,
      73,   156,    71,    72,   190,    49,    50,    51,    52,    39,
      40,    41,    42,    43,    58,    59,    60,    61,    62,    63,
     303,    31,    32,    33,    34,    35,    36,    37,    38,   149,
     193,    61,    42,   291,    64,    -1,    -1,    67,    -1,     6,
      -1,    71,    72,    -1,    -1,    -1,    -1,    57,    -1,    -1,
      -1,     6,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    73,    31,    32,    33,    34,    35,    36,
      37,    38,    -1,    23,    24,    42,    31,    32,    33,    34,
      35,    36,    37,    38,    -1,    -1,    -1,    42,    -1,    39,
      40,    41,    42,    43,     6,    -1,    -1,    -1,    -1,    -1,
      67,    -1,    -1,    -1,    -1,    -1,    73,     6,    -1,    -1,
      -1,    61,    -1,    -1,    64,    70,    66,    67,    73,    31,
      32,    33,    34,    35,    36,    37,    38,     6,    -1,    -1,
      42,    -1,    31,    32,    33,    34,    35,    36,    37,    38,
      -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    33,    34,    35,    36,    37,    38,
      -1,    73,    -1,    42,     5,    -1,     7,     8,     9,    10,
      11,    -1,    13,    -1,    73,    16,    17,    18,    19,    -1,
      -1,    -1,    23,    24,    -1,    -1,    -1,    23,    24,    -1,
      -1,    -1,    -1,    -1,    73,    23,    24,    -1,    39,    40,
      41,    42,    43,    39,    40,    41,    42,    43,    -1,    -1,
      -1,    39,    40,    41,    42,    43,    -1,    -1,    -1,    -1,
      61,    -1,    -1,    64,    -1,    61,    67,    -1,    64,    -1,
      71,    67,    -1,    61,    23,    24,    64,    -1,    74,    67,
      -1,    23,    24,    -1,    -1,    -1,    74,    -1,    23,    24,
      39,    40,    41,    42,    43,    -1,    -1,    39,    40,    41,
      42,    43,    -1,    -1,    39,    40,    41,    42,    43,    -1,
      -1,    -1,    61,    -1,    -1,    64,    -1,    66,    67,    61,
      23,    24,    64,    -1,    -1,    67,    61,    -1,    -1,    64,
      -1,    -1,    67,    -1,    -1,    -1,    39,    40,    41,    42,
      43,    49,    50,    51,    52,    53,    54,    -1,    -1,    -1,
      58,    59,    60,    61,    62,    63,    -1,    -1,    61,    -1,
      -1,    64,    -1,    -1,    67,    -1,    74,    75,    49,    50,
      51,    52,    53,    54,    -1,    -1,    -1,    58,    59,    60,
      61,    62,    63,    -1,    -1,    -1,    49,    50,    51,    52,
      53,    54,    -1,    74,    75,    58,    59,    60,    61,    62,
      63,    -1,    -1,    -1,    49,    50,    51,    52,    53,    54,
      -1,    74,    75,    58,    59,    60,    61,    62,    63,    -1,
      -1,    -1,    49,    50,    51,    52,    53,    54,    -1,    -1,
      75,    58,    59,    60,    61,    62,    63,    -1,    -1,    49,
      50,    51,    52,    53,    54,    -1,    -1,    74,    58,    59,
      60,    61,    62,    63,    -1,    -1,    49,    50,    51,    52,
      53,    54,    -1,    -1,    74,    58,    59,    60,    61,    62,
      63,    -1,    -1,    49,    50,    51,    52,    53,    54,    -1,
      -1,    74,    58,    59,    60,    61,    62,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    49,    50,
      51,    52,    53,    54,    55,    56,    -1,    58,    59,    60,
      61,    62,    63,    -1,    -1,    66,    -1,    68,    49,    50,
      51,    52,    53,    54,    -1,    -1,    -1,    58,    59,    60,
      61,    62,    63,    -1,    -1,    -1,    -1,    68,    49,    50,
      51,    52,    53,    54,    -1,    -1,    -1,    58,    59,    60,
      61,    62,    63,    -1,    -1,    -1,    -1,    68,    49,    50,
      51,    52,    53,    54,    -1,    -1,    -1,    58,    59,    60,
      61,    62,    63,    -1,    -1,    -1,    -1,    68,    49,    50,
      51,    52,    53,    54,    -1,    -1,    -1,    58,    59,    60,
      61,    62,    63,    -1,    -1,    -1,    -1,    68,    49,    50,
      51,    52,    53,    54,    -1,    -1,    -1,    58,    59,    60,
      61,    62,    63,    -1,    -1,    -1,    -1,    68,    49,    50,
      51,    52,    53,    54,    -1,    -1,    -1,    58,    59,    60,
      61,    62,    63,    -1,    -1,    -1,    -1,    68,    49,    50,
      51,    52,    53,    54,    -1,    -1,    -1,    58,    59,    60,
      61,    62,    63,    -1,    -1,    -1,    -1,    68,    49,    50,
      51,    52,    53,    54,    -1,    -1,    -1,    58,    59,    60,
      61,    62,    63,    -1,    -1,    66,    49,    50,    51,    52,
      53,    54,    -1,    -1,    -1,    58,    59,    60,    61,    62,
      63,    -1,    -1,    66,    49,    50,    51,    52,    53,    54,
      -1,    -1,    -1,    58,    59,    60,    61,    62,    63,    49,
      50,    51,    52,    53,    -1,    -1,    -1,    -1,    58,    59,
      60,    61,    62,    63
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    77,    78,    42,     0,    66,     4,    79,    80,
      81,    43,    67,    69,    84,    66,    81,    66,    43,    82,
      83,    84,    43,     5,     6,     7,    20,    21,    85,    86,
      87,    88,    89,   100,   103,   127,   132,    66,    68,    84,
      66,    42,    67,   102,   106,    42,    67,   105,   106,    42,
      42,    87,    66,    66,   101,   102,     6,    31,    32,    33,
      34,    35,    36,    37,    38,    42,    57,    70,    73,   107,
     108,   110,   111,   112,   113,    67,    90,    91,   104,   105,
      57,   107,    75,   128,    71,    66,    66,    68,    66,    90,
      23,    24,    39,    40,    41,    42,    43,    61,    64,    67,
     114,   115,   116,   117,   118,    42,    74,   115,    57,    73,
      42,    68,    92,    93,   106,   107,    71,    95,    91,    94,
     107,    66,    68,    66,   114,    57,    42,    71,    42,   133,
     134,   102,   115,   115,   115,    70,    49,    50,    51,    52,
      53,    54,    58,    59,    60,    61,    62,    63,    73,    73,
     107,    74,   114,   107,   109,    68,    70,   107,     8,     9,
      10,    11,    13,    16,    17,    18,    19,    42,    72,    88,
      95,    96,    97,    98,    99,   106,   114,   115,   119,   120,
     123,   124,   125,   126,   105,   114,    25,    88,    89,   129,
     130,   131,    57,    70,    72,    68,   115,   115,   115,   115,
     115,   115,   115,   115,   115,   115,   115,   115,   115,    75,
     115,   109,   107,    70,    74,    93,   114,    67,    67,    71,
      95,    67,    67,   115,    67,    72,    97,    66,    44,    45,
      46,    47,    48,    57,    55,    56,    89,    66,    72,   131,
     115,    72,   134,    74,   115,    74,    75,    74,   107,   115,
     115,    14,    15,   121,   122,    17,   115,    66,    98,   115,
      95,    42,   107,    66,   114,   114,   114,   114,   114,   114,
      74,    75,    74,   115,    68,    68,   115,    75,    72,   122,
      67,    68,    66,   115,    66,    75,    42,   115,    74,    75,
      95,    71,    75,    96,   115,    95,    68,    66,   115,   115,
      75,    74,   115,    12,   121,    96,    68,    95,    68,    66,
      68,   115,    74,    95,   119,    72,    66,    95,    98,    95,
      68,    68,    95,    95
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    76,    77,    78,    79,    79,    80,    80,    81,    81,
      82,    82,    83,    83,    84,    84,    84,    85,    85,    86,
      86,    87,    87,    87,    87,    88,    88,    89,    89,    90,
      90,    91,    91,    92,    92,    93,    93,    94,    94,    95,
      95,    96,    96,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    98,    98,    98,    98,    98,
      98,    98,    98,    98,    99,    99,   100,   100,   101,   101,
     102,   102,   102,   103,   103,   104,   104,   105,   105,   105,
     106,   106,   107,   107,   107,   108,   108,   108,   108,   108,
     108,   108,   108,   108,   109,   109,   110,   110,   110,   111,
     112,   113,   114,   114,   115,   115,   115,   115,   115,   115,
     115,   115,   115,   115,   115,   115,   115,   115,   115,   115,
     115,   115,   115,   116,   116,   116,   116,   116,   116,   116,
     116,   117,   117,   117,   118,   118,   118,   119,   119,   119,
     120,   120,   121,   121,   122,   122,   123,   123,   123,   123,
     124,   125,   126,   126,   127,   128,   128,   129,   129,   130,
     130,   131,   131,   131,   132,   132,   133,   133,   134,   134
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     5,     2,     1,     0,     3,     2,     2,     4,
       1,     0,     3,     2,     1,     2,     2,     1,     0,     3,
       2,     1,     1,     1,     1,     1,     1,     3,     4,     2,
       1,     3,     2,     3,     1,     2,     1,     1,     1,     3,
       2,     3,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     3,     3,
       3,     3,     3,     3,     1,     2,     2,     4,     3,     2,
       1,     3,     4,     2,     4,     3,     2,     2,     4,     3,
       3,     1,     1,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     1,     1,     1,     1,     4,
       2,     3,     3,     1,     1,     1,     1,     1,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     1,     4,     4,     5,     5,     6,     7,
       8,     1,     4,     3,     1,     1,     1,     5,     7,     7,
       7,     4,     2,     1,     4,     3,     9,     7,     6,     3,
       5,     7,     7,     8,     6,     2,     0,     1,     0,     2,
       1,     2,     1,     2,     5,     6,     3,     1,     1,     3
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

#line 1574 "d_parser.tab.c"

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

#line 445 "d_parser.y"


// Секция пользовательского кода
