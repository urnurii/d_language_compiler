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



#line 90 "d_parser.tab.c"

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
  YYSYMBOL_57_ = 57,                       /* ';'  */
  YYSYMBOL_58_ = 58,                       /* '('  */
  YYSYMBOL_59_ = 59,                       /* ')'  */
  YYSYMBOL_60_ = 60,                       /* '.'  */
  YYSYMBOL_61_ = 61,                       /* ','  */
  YYSYMBOL_62_ = 62,                       /* '['  */
  YYSYMBOL_63_ = 63,                       /* ']'  */
  YYSYMBOL_64_ = 64,                       /* '{'  */
  YYSYMBOL_65_ = 65,                       /* '}'  */
  YYSYMBOL_66_ = 66,                       /* '='  */
  YYSYMBOL_67_ = 67,                       /* '<'  */
  YYSYMBOL_68_ = 68,                       /* '>'  */
  YYSYMBOL_69_ = 69,                       /* '+'  */
  YYSYMBOL_70_ = 70,                       /* '-'  */
  YYSYMBOL_71_ = 71,                       /* '*'  */
  YYSYMBOL_72_ = 72,                       /* '/'  */
  YYSYMBOL_73_ = 73,                       /* '!'  */
  YYSYMBOL_74_ = 74,                       /* ':'  */
  YYSYMBOL_YYACCEPT = 75,                  /* $accept  */
  YYSYMBOL_program = 76,                   /* program  */
  YYSYMBOL_module_clause = 77,             /* module_clause  */
  YYSYMBOL_e_import_decl_list = 78,        /* e_import_decl_list  */
  YYSYMBOL_import_decl_list = 79,          /* import_decl_list  */
  YYSYMBOL_import_decl = 80,               /* import_decl  */
  YYSYMBOL_e_import_spec_list = 81,        /* e_import_spec_list  */
  YYSYMBOL_import_spec_list = 82,          /* import_spec_list  */
  YYSYMBOL_import_spec = 83,               /* import_spec  */
  YYSYMBOL_e_top_level_decl_list = 84,     /* e_top_level_decl_list  */
  YYSYMBOL_top_level_decl_list = 85,       /* top_level_decl_list  */
  YYSYMBOL_top_level_decl = 86,            /* top_level_decl  */
  YYSYMBOL_decl = 87,                      /* decl  */
  YYSYMBOL_func_decl = 88,                 /* func_decl  */
  YYSYMBOL_param_list = 89,                /* param_list  */
  YYSYMBOL_param_decl = 90,                /* param_decl  */
  YYSYMBOL_type_or_builtin = 91,           /* type_or_builtin  */
  YYSYMBOL_block = 92,                     /* block  */
  YYSYMBOL_stmt_list = 93,                 /* stmt_list  */
  YYSYMBOL_stmt = 94,                      /* stmt  */
  YYSYMBOL_simple_stmt = 95,               /* simple_stmt  */
  YYSYMBOL_assignment_stmt = 96,           /* assignment_stmt  */
  YYSYMBOL_lvalue = 97,                    /* lvalue  */
  YYSYMBOL_return_stmt = 98,               /* return_stmt  */
  YYSYMBOL_const_decl = 99,                /* const_decl  */
  YYSYMBOL_const_spec_list = 100,          /* const_spec_list  */
  YYSYMBOL_const_spec = 101,               /* const_spec  */
  YYSYMBOL_var_decl = 102,                 /* var_decl  */
  YYSYMBOL_var_spec_list = 103,            /* var_spec_list  */
  YYSYMBOL_var_spec = 104,                 /* var_spec  */
  YYSYMBOL_id_list = 105,                  /* id_list  */
  YYSYMBOL_type_name = 106,                /* type_name  */
  YYSYMBOL_type = 107,                     /* type  */
  YYSYMBOL_type_list = 108,                /* type_list  */
  YYSYMBOL_expr_list = 109,                /* expr_list  */
  YYSYMBOL_expr = 110,                     /* expr  */
  YYSYMBOL_or_expr = 111,                  /* or_expr  */
  YYSYMBOL_and_expr = 112,                 /* and_expr  */
  YYSYMBOL_eq_expr = 113,                  /* eq_expr  */
  YYSYMBOL_rel_expr = 114,                 /* rel_expr  */
  YYSYMBOL_add_expr = 115,                 /* add_expr  */
  YYSYMBOL_mul_expr = 116,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 117,               /* unary_expr  */
  YYSYMBOL_postfix_expr = 118,             /* postfix_expr  */
  YYSYMBOL_primary_expr = 119,             /* primary_expr  */
  YYSYMBOL_primary_name = 120,             /* primary_name  */
  YYSYMBOL_operand = 121,                  /* operand  */
  YYSYMBOL_if_stmt = 122,                  /* if_stmt  */
  YYSYMBOL_switch_stmt = 123,              /* switch_stmt  */
  YYSYMBOL_case_list = 124,                /* case_list  */
  YYSYMBOL_case_item = 125,                /* case_item  */
  YYSYMBOL_for_stmt = 126,                 /* for_stmt  */
  YYSYMBOL_while_stmt = 127,               /* while_stmt  */
  YYSYMBOL_do_while_stmt = 128,            /* do_while_stmt  */
  YYSYMBOL_foreach_stmt = 129,             /* foreach_stmt  */
  YYSYMBOL_class_decl = 130,               /* class_decl  */
  YYSYMBOL_class_inherit_opt = 131,        /* class_inherit_opt  */
  YYSYMBOL_class_body_opt = 132,           /* class_body_opt  */
  YYSYMBOL_class_body = 133,               /* class_body  */
  YYSYMBOL_class_member = 134,             /* class_member  */
  YYSYMBOL_enum_decl = 135,                /* enum_decl  */
  YYSYMBOL_enum_member_list = 136,         /* enum_member_list  */
  YYSYMBOL_enum_member = 137               /* enum_member  */
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
#define YYLAST   689

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  75
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  63
/* YYNRULES -- Number of rules.  */
#define YYNRULES  180
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  347

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   311


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
       2,     2,     2,    73,     2,     2,     2,     2,     2,     2,
      58,    59,    71,    69,    61,    70,    60,    72,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    74,    57,
      67,    66,    68,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    62,     2,    63,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    64,     2,    65,     2,     2,     2,     2,
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
      55,    56
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   109,   109,   114,   119,   120,   125,   126,   131,   132,
     137,   138,   143,   144,   149,   150,   151,   156,   157,   162,
     163,   168,   169,   170,   171,   176,   177,   182,   183,   184,
     185,   190,   191,   196,   197,   202,   203,   204,   205,   206,
     207,   212,   213,   218,   219,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   240,   241,   242,
     243,   248,   249,   250,   251,   252,   253,   258,   259,   264,
     265,   270,   271,   276,   277,   282,   283,   284,   289,   290,
     295,   296,   301,   302,   303,   308,   309,   314,   315,   316,
     317,   318,   319,   320,   321,   326,   327,   328,   329,   330,
     331,   336,   337,   342,   343,   348,   353,   354,   359,   360,
     365,   366,   367,   372,   373,   374,   375,   376,   381,   382,
     383,   388,   389,   390,   395,   396,   397,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   415,   416,   417,   422,
     423,   424,   429,   430,   431,   432,   433,   434,   439,   440,
     441,   446,   447,   452,   453,   458,   459,   464,   465,   466,
     467,   472,   477,   482,   483,   488,   493,   494,   499,   500,
     505,   506,   511,   512,   513,   518,   519,   524,   525,   530,
     531
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
  "';'", "'('", "')'", "'.'", "','", "'['", "']'", "'{'", "'}'", "'='",
  "'<'", "'>'", "'+'", "'-'", "'*'", "'/'", "'!'", "':'", "$accept",
  "program", "module_clause", "e_import_decl_list", "import_decl_list",
  "import_decl", "e_import_spec_list", "import_spec_list", "import_spec",
  "e_top_level_decl_list", "top_level_decl_list", "top_level_decl", "decl",
  "func_decl", "param_list", "param_decl", "type_or_builtin", "block",
  "stmt_list", "stmt", "simple_stmt", "assignment_stmt", "lvalue",
  "return_stmt", "const_decl", "const_spec_list", "const_spec", "var_decl",
  "var_spec_list", "var_spec", "id_list", "type_name", "type", "type_list",
  "expr_list", "expr", "or_expr", "and_expr", "eq_expr", "rel_expr",
  "add_expr", "mul_expr", "unary_expr", "postfix_expr", "primary_expr",
  "primary_name", "operand", "if_stmt", "switch_stmt", "case_list",
  "case_item", "for_stmt", "while_stmt", "do_while_stmt", "foreach_stmt",
  "class_decl", "class_inherit_opt", "class_body_opt", "class_body",
  "class_member", "enum_decl", "enum_member_list", "enum_member", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-263)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-68)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      23,    19,    39,     1,  -263,  -263,    47,    62,    97,    47,
     100,   124,    -6,   138,  -263,   132,   120,  -263,  -263,    43,
      -6,   143,  -263,   -18,   152,   -12,   167,   194,  -263,   132,
     189,  -263,  -263,  -263,  -263,  -263,  -263,  -263,  -263,   190,
    -263,  -263,   211,  -263,    37,   196,   211,  -263,   406,   115,
     191,   199,  -263,  -263,   -21,   200,   202,  -263,  -263,  -263,
    -263,  -263,  -263,  -263,  -263,   216,    93,   579,   203,   195,
     137,    38,   207,   579,   209,   231,   212,   239,  -263,   211,
    -263,  -263,   423,  -263,  -263,  -263,  -263,  -263,  -263,  -263,
    -263,   579,   501,   579,   579,   219,   229,   233,    32,    42,
      45,    59,  -263,   234,  -263,  -263,   176,   236,  -263,   501,
     579,   237,   415,   517,   101,  -263,  -263,   468,   243,   211,
    -263,  -263,   236,   579,  -263,    16,   240,    63,  -263,  -263,
    -263,   131,   248,  -263,  -263,  -263,   501,   579,   579,   579,
     579,   579,   579,   579,   579,   579,   579,   579,   579,   361,
    -263,  -263,   579,  -263,   134,   236,   456,   261,  -263,   235,
     501,   245,   415,   256,  -263,   501,  -263,   236,   310,   253,
    -263,   259,    16,  -263,   579,   -23,  -263,  -263,  -263,  -263,
     233,    32,    42,    42,    45,    45,    45,    45,    59,    59,
    -263,  -263,   525,   147,    66,  -263,   501,  -263,  -263,   160,
     579,  -263,  -263,   271,    26,   235,   272,   616,   274,  -263,
    -263,  -263,   304,   276,  -263,  -263,   182,  -263,   183,   617,
    -263,  -263,  -263,  -263,  -263,  -263,  -263,  -263,   501,  -263,
     235,  -263,   161,  -263,  -263,  -263,  -263,  -263,  -263,  -263,
    -263,    81,  -263,  -263,   531,  -263,  -263,   236,   579,   579,
     227,   318,   579,   568,   235,   489,  -263,   279,  -263,   579,
     579,   579,   579,   579,    21,   579,  -263,  -263,  -263,  -263,
    -263,  -263,   579,  -263,    84,   278,   280,   579,   264,    -3,
    -263,   282,   283,   573,   284,   129,  -263,   275,   306,  -263,
     236,   236,   236,   236,   236,  -263,   236,   287,  -263,   579,
     235,   289,   277,   347,  -263,  -263,   579,   235,   300,   315,
     579,   579,   299,  -263,   298,   363,   227,   347,   347,   317,
    -263,   235,   319,   322,   321,   579,  -263,    -4,    14,   347,
     324,  -263,   235,   579,   235,   323,  -263,  -263,  -263,  -263,
    -263,   332,  -263,   235,   235,  -263,  -263
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
      13,    86,     0,    71,    75,     0,     0,    78,     0,   167,
       0,     0,    20,    12,     0,     0,     0,    87,    88,    89,
      90,    91,    92,    93,    94,     0,     0,     0,    95,     0,
       0,     0,     0,     0,    82,     0,     0,     0,    19,     0,
      72,    74,     0,    85,   140,   141,   144,   145,   147,   139,
     146,     0,     0,     0,     0,     0,   105,   107,   109,   112,
     117,   120,   123,   126,   135,   142,   136,    76,   104,     0,
       0,     0,     0,     0,     0,    32,    34,     0,    35,     0,
      79,    81,    84,     0,   166,   169,   179,     0,   178,    73,
     100,     0,     0,    98,   125,   124,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     137,   138,     0,   102,     0,    77,     0,     0,    28,     0,
       0,     0,     0,     0,    33,     0,    80,    83,     0,     0,
     173,     0,   168,   171,     0,     0,   175,    99,   143,    97,
     106,   108,   110,   111,   115,   116,   113,   114,   118,   119,
     121,   122,     0,     0,     0,   103,     0,    96,    38,     0,
      69,    48,    49,     0,     0,     0,     0,     0,     0,    42,
      45,    50,     0,     0,    46,    60,     0,    47,    57,   142,
      51,    52,    53,    54,    55,    56,    30,    40,     0,    27,
       0,    31,     0,   174,   172,   165,   170,   180,   176,   177,
     128,     0,   134,   127,     0,   101,    37,    70,     0,     0,
       0,     0,     0,     0,     0,     0,    41,     0,    44,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    39,    29,
      36,   130,     0,   129,     0,     0,     0,     0,     0,     0,
     154,     0,     0,     0,     0,    57,   160,     0,     0,    43,
      62,    63,    64,    65,    66,    68,    61,     0,   131,     0,
       0,     0,     0,     0,   152,   153,     0,     0,     0,     0,
       0,     0,     0,   132,     0,   150,     0,     0,   156,     0,
     161,     0,     0,     0,     0,     0,   133,     0,     0,   155,
       0,   159,     0,     0,     0,     0,   149,   148,   151,   162,
     158,     0,   163,     0,     0,   164,   157
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -263,  -263,  -263,  -263,  -263,   374,  -263,  -263,    36,  -263,
    -263,   377,   -13,   -49,   -76,   244,   291,  -109,  -225,  -211,
    -248,  -263,  -263,  -263,  -263,  -263,   -24,  -263,  -263,   -32,
     -15,   -50,   -35,  -116,   -58,   -66,  -263,   273,   286,     9,
      72,   104,   -59,  -263,  -263,  -153,  -263,    82,  -263,    98,
    -262,  -263,  -263,  -263,  -263,  -263,  -263,  -263,  -263,   241,
    -263,  -263,   247
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     2,     3,     8,     9,    10,    19,    20,    14,    28,
      29,    30,   210,    32,   114,   115,   116,   211,   212,   213,
     214,   215,   216,   217,    33,    54,    43,    34,    71,    47,
     117,    68,   153,   154,   107,   108,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   220,   221,   279,
     280,   222,   223,   224,   225,    35,    76,   171,   172,   173,
      36,   127,   128
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      95,   257,    31,   158,   219,   284,   131,   203,    44,    69,
      48,   277,   278,    74,    72,   122,    31,   305,    55,   126,
     118,    23,    24,    25,    41,   132,     1,    44,   277,   278,
      41,    48,   118,   193,   134,   135,    79,    11,    80,     5,
      42,   168,   238,    56,    84,    85,    46,   161,    21,   232,
     226,     7,   155,   229,    13,   129,    39,   133,     6,   219,
     157,     4,   304,    89,    44,   167,   305,   118,    57,    58,
      59,    60,    61,    62,    63,    64,   170,   159,   318,   338,
     199,   139,   140,   194,   249,   341,   195,   166,   190,   191,
     250,   218,   329,   141,   142,   119,   251,   120,    65,    66,
     219,   179,    38,    67,    48,    11,   118,   257,   237,   143,
     144,   295,   169,   118,   145,   146,    84,    85,   257,   233,
      12,   269,    13,   170,   175,   227,   241,   230,   176,   243,
     147,   148,    86,    87,    88,    89,    90,    23,    24,    25,
     244,   254,   247,   111,   271,   286,   218,   298,   182,   183,
     219,    91,    26,    27,    15,   272,    92,    17,   299,   169,
     162,   245,   163,    93,   219,   219,    94,    18,    57,    58,
      59,    60,    61,    62,    63,    64,   219,    37,   274,    41,
     219,    22,   275,   276,   266,   267,   282,   285,   178,    75,
     177,   315,   163,   268,    45,   196,   112,   197,   320,   113,
      40,   290,   291,   292,   293,   294,   297,   296,   196,    49,
     242,   302,   331,   184,   185,   186,   187,   309,   336,   246,
     288,   163,   196,   340,   270,   342,   259,   260,   261,   262,
     263,   150,   151,   314,   345,   346,    50,   218,   266,   267,
     319,   277,   278,   264,   323,   324,    52,    53,   265,   188,
     189,   218,   218,    41,    70,    77,    78,    81,    83,   335,
      82,   110,   111,   218,   121,   109,    23,   218,    25,   200,
     201,   202,   203,   124,   204,   123,   125,   205,   206,   207,
     208,   126,   136,   137,    84,    85,   138,    57,    58,    59,
      60,    61,    62,    63,    64,   156,   149,   152,    41,   157,
      86,    87,    88,    89,    90,   165,   174,   178,   228,    23,
     234,    25,   200,   201,   202,   203,    24,   204,   113,    91,
     205,   206,   207,   208,   235,   157,   209,    84,    85,   248,
     252,    93,   255,   258,    94,   281,   289,   300,   303,   301,
     306,   310,   307,    86,    87,    88,    89,    90,   312,   311,
     313,   317,    23,   316,    25,   200,   201,   202,   203,   321,
     204,   326,    91,   205,   206,   207,   208,    56,   157,   256,
      84,    85,   322,   325,    93,   327,   330,    94,   332,   333,
     334,   339,   343,    16,    84,    85,    86,    87,    88,    89,
      90,   344,    57,    58,    59,    60,    61,    62,    63,    64,
      86,    87,    88,    89,    90,    91,    51,   231,   164,   337,
     180,   157,    56,   236,   328,     0,     0,    93,     0,    91,
      94,    56,   239,    66,   181,     0,     0,     0,     0,   111,
       0,    93,     0,     0,    94,   192,     0,    57,    58,    59,
      60,    61,    62,    63,    64,     0,    57,    58,    59,    60,
      61,    62,    63,    64,    57,    58,    59,    60,    61,    62,
      63,    64,   111,     0,     0,    41,     0,    65,    66,     0,
       0,     0,    73,     0,   111,     0,     0,    66,     0,   157,
       0,     0,   130,     0,     0,   113,     0,    57,    58,    59,
      60,    61,    62,    63,    64,    56,     0,     0,    41,    57,
      58,    59,    60,    61,    62,    63,    64,    56,     0,     0,
       0,     0,     0,     0,     0,   198,     0,     0,   113,     0,
      57,    58,    59,    60,    61,    62,    63,    64,     0,    65,
     113,   287,    57,    58,    59,    60,    61,    62,    63,    64,
      84,    85,     0,     0,     0,     0,     0,     0,    84,    85,
       0,    66,     0,     0,    84,    85,    86,    87,    88,    89,
      90,     0,     0,    66,    86,    87,    88,    89,    90,     0,
      86,    87,    88,    89,    90,    91,     0,     0,     0,     0,
     160,     0,     0,    91,     0,     0,     0,    93,   240,    91,
      94,    84,    85,     0,   273,    93,    84,    85,    94,     0,
       0,    93,    84,    85,    94,     0,     0,    86,    87,    88,
      89,    90,    86,    87,    88,    89,    90,     0,    86,    87,
      88,    89,    90,     0,     0,   283,    91,     0,     0,     0,
     308,    91,     0,     0,     0,     0,     0,    91,    93,    84,
      85,    94,     0,    93,     0,     0,    94,     0,     0,    93,
       0,     0,    94,     0,     0,    86,    87,    88,    89,    90,
       0,   -67,   -67,   -67,   -67,   -67,     0,     0,     0,     0,
       0,     0,     0,     0,   253,     0,     0,     0,   -67,     0,
       0,     0,     0,   -67,     0,     0,    93,     0,     0,    94
};

static const yytype_int16 yycheck[] =
{
      66,   212,    15,   112,   157,   253,    82,    11,    23,    44,
      25,    14,    15,    48,    46,    73,    29,   279,    42,    42,
      70,     5,     6,     7,    42,    91,     3,    42,    14,    15,
      42,    46,    82,   149,    93,    94,    57,    43,    59,     0,
      58,    25,    65,     6,    23,    24,    58,   113,    12,   165,
     159,     4,   110,   162,    60,    79,    20,    92,    57,   212,
      64,    42,    65,    42,    79,   123,   328,   117,    31,    32,
      33,    34,    35,    36,    37,    38,   125,   112,   303,    65,
     156,    49,    50,   149,    58,   333,   152,   119,   147,   148,
      64,   157,   317,    51,    52,    57,   205,    59,    61,    62,
     253,   136,    59,    66,   119,    43,   156,   318,   174,    67,
      68,   264,   125,   163,    69,    70,    23,    24,   329,   168,
      58,   230,    60,   172,    61,   160,   192,   162,    65,    63,
      71,    72,    39,    40,    41,    42,    43,     5,     6,     7,
      74,   207,   200,     6,    63,   254,   212,    63,   139,   140,
     303,    58,    20,    21,    57,    74,    63,    57,    74,   172,
      59,   196,    61,    70,   317,   318,    73,    43,    31,    32,
      33,    34,    35,    36,    37,    38,   329,    57,   244,    42,
     333,    43,   248,   249,    55,    56,   252,   253,    59,    74,
      59,   300,    61,   228,    42,    61,    59,    63,   307,    62,
      57,   259,   260,   261,   262,   263,   272,   265,    61,    42,
      63,   277,   321,   141,   142,   143,   144,   283,   327,    59,
     255,    61,    61,   332,    63,   334,    44,    45,    46,    47,
      48,    55,    56,   299,   343,   344,    42,   303,    55,    56,
     306,    14,    15,    61,   310,   311,    57,    57,    66,   145,
     146,   317,   318,    42,    58,    64,    57,    57,    42,   325,
      58,    66,     6,   329,    57,    62,     5,   333,     7,     8,
       9,    10,    11,    42,    13,    66,    64,    16,    17,    18,
      19,    42,    63,    54,    23,    24,    53,    31,    32,    33,
      34,    35,    36,    37,    38,    58,    62,    61,    42,    64,
      39,    40,    41,    42,    43,    62,    66,    59,    63,     5,
      57,     7,     8,     9,    10,    11,     6,    13,    62,    58,
      16,    17,    18,    19,    65,    64,    65,    23,    24,    58,
      58,    70,    58,    57,    73,    17,    57,    59,    74,    59,
      58,    57,    59,    39,    40,    41,    42,    43,    42,    74,
      63,    74,     5,    64,     7,     8,     9,    10,    11,    59,
      13,    63,    58,    16,    17,    18,    19,     6,    64,    65,
      23,    24,    57,    74,    70,    12,    59,    73,    59,    57,
      59,    57,    59,     9,    23,    24,    39,    40,    41,    42,
      43,    59,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    58,    29,   163,   117,   327,
     137,    64,     6,   172,   316,    -1,    -1,    70,    -1,    58,
      73,     6,   175,    62,   138,    -1,    -1,    -1,    -1,     6,
      -1,    70,    -1,    -1,    73,    74,    -1,    31,    32,    33,
      34,    35,    36,    37,    38,    -1,    31,    32,    33,    34,
      35,    36,    37,    38,    31,    32,    33,    34,    35,    36,
      37,    38,     6,    -1,    -1,    42,    -1,    61,    62,    -1,
      -1,    -1,    66,    -1,     6,    -1,    -1,    62,    -1,    64,
      -1,    -1,    59,    -1,    -1,    62,    -1,    31,    32,    33,
      34,    35,    36,    37,    38,     6,    -1,    -1,    42,    31,
      32,    33,    34,    35,    36,    37,    38,     6,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    59,    -1,    -1,    62,    -1,
      31,    32,    33,    34,    35,    36,    37,    38,    -1,    61,
      62,    42,    31,    32,    33,    34,    35,    36,    37,    38,
      23,    24,    -1,    -1,    -1,    -1,    -1,    -1,    23,    24,
      -1,    62,    -1,    -1,    23,    24,    39,    40,    41,    42,
      43,    -1,    -1,    62,    39,    40,    41,    42,    43,    -1,
      39,    40,    41,    42,    43,    58,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    58,    -1,    -1,    -1,    70,    63,    58,
      73,    23,    24,    -1,    63,    70,    23,    24,    73,    -1,
      -1,    70,    23,    24,    73,    -1,    -1,    39,    40,    41,
      42,    43,    39,    40,    41,    42,    43,    -1,    39,    40,
      41,    42,    43,    -1,    -1,    57,    58,    -1,    -1,    -1,
      57,    58,    -1,    -1,    -1,    -1,    -1,    58,    70,    23,
      24,    73,    -1,    70,    -1,    -1,    73,    -1,    -1,    70,
      -1,    -1,    73,    -1,    -1,    39,    40,    41,    42,    43,
      -1,    44,    45,    46,    47,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,    61,    -1,
      -1,    -1,    -1,    66,    -1,    -1,    70,    -1,    -1,    73
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    76,    77,    42,     0,    57,     4,    78,    79,
      80,    43,    58,    60,    83,    57,    80,    57,    43,    81,
      82,    83,    43,     5,     6,     7,    20,    21,    84,    85,
      86,    87,    88,    99,   102,   130,   135,    57,    59,    83,
      57,    42,    58,   101,   105,    42,    58,   104,   105,    42,
      42,    86,    57,    57,   100,   101,     6,    31,    32,    33,
      34,    35,    36,    37,    38,    61,    62,    66,   106,   107,
      58,   103,   104,    66,   107,    74,   131,    64,    57,    57,
      59,    57,    58,    42,    23,    24,    39,    40,    41,    42,
      43,    58,    63,    70,    73,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   109,   110,    62,
      66,     6,    59,    62,    89,    90,    91,   105,   106,    57,
      59,    57,   109,    66,    42,    64,    42,   136,   137,   101,
      59,    89,   110,   107,   117,   117,    63,    54,    53,    49,
      50,    51,    52,    67,    68,    69,    70,    71,    72,    62,
      55,    56,    61,   107,   108,   109,    58,    64,    92,   107,
      63,   110,    59,    61,    91,    62,   104,   109,    25,    87,
      88,   132,   133,   134,    66,    61,    65,    59,    59,   107,
     112,   113,   114,   114,   115,   115,   115,   115,   116,   116,
     117,   117,    74,   108,   110,   110,    61,    63,    59,    89,
       8,     9,    10,    11,    13,    16,    17,    18,    19,    65,
      87,    92,    93,    94,    95,    96,    97,    98,   110,   120,
     122,   123,   126,   127,   128,   129,    92,   107,    63,    92,
     107,    90,   108,    88,    57,    65,   134,   110,    65,   137,
      63,   110,    63,    63,    74,   107,    59,   109,    58,    58,
      64,    92,    58,    58,   110,    58,    65,    94,    57,    44,
      45,    46,    47,    48,    61,    66,    55,    56,   107,    92,
      63,    63,    74,    63,   110,   110,   110,    14,    15,   124,
     125,    17,   110,    57,    95,   110,    92,    42,   107,    57,
     109,   109,   109,   109,   109,   120,   109,   110,    63,    74,
      59,    59,   110,    74,    65,   125,    58,    59,    57,   110,
      57,    74,    42,    63,   110,    92,    64,    74,    93,   110,
      92,    59,    57,   110,   110,    74,    63,    12,   124,    93,
      59,    92,    59,    57,    59,   110,    92,   122,    65,    57,
      92,    95,    92,    59,    59,    92,    92
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    75,    76,    77,    78,    78,    79,    79,    80,    80,
      81,    81,    82,    82,    83,    83,    83,    84,    84,    85,
      85,    86,    86,    86,    86,    87,    87,    88,    88,    88,
      88,    89,    89,    90,    90,    91,    91,    91,    91,    91,
      91,    92,    92,    93,    93,    94,    94,    94,    94,    94,
      94,    94,    94,    94,    94,    94,    94,    95,    95,    95,
      95,    96,    96,    96,    96,    96,    96,    97,    97,    98,
      98,    99,    99,   100,   100,   101,   101,   101,   102,   102,
     103,   103,   104,   104,   104,   105,   105,   106,   106,   106,
     106,   106,   106,   106,   106,   107,   107,   107,   107,   107,
     107,   108,   108,   109,   109,   110,   111,   111,   112,   112,
     113,   113,   113,   114,   114,   114,   114,   114,   115,   115,
     115,   116,   116,   116,   117,   117,   117,   118,   118,   118,
     118,   118,   118,   118,   118,   118,   119,   119,   119,   120,
     120,   120,   121,   121,   121,   121,   121,   121,   122,   122,
     122,   123,   123,   124,   124,   125,   125,   126,   126,   126,
     126,   127,   128,   129,   129,   130,   131,   131,   132,   132,
     133,   133,   134,   134,   134,   135,   135,   136,   136,   137,
     137
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     5,     2,     1,     0,     3,     2,     2,     4,
       1,     0,     3,     2,     1,     2,     2,     1,     0,     3,
       2,     1,     1,     1,     1,     1,     1,     6,     5,     7,
       6,     3,     1,     2,     1,     1,     4,     4,     3,     4,
       3,     3,     2,     3,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       1,     3,     3,     3,     3,     3,     3,     1,     3,     1,
       2,     2,     4,     3,     2,     1,     3,     4,     2,     4,
       3,     2,     2,     4,     3,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     4,     3,     4,
       3,     3,     1,     3,     1,     1,     3,     1,     3,     1,
       3,     3,     1,     3,     3,     3,     3,     1,     3,     3,
       1,     3,     3,     1,     2,     2,     1,     4,     4,     5,
       5,     6,     7,     8,     4,     1,     1,     2,     2,     1,
       1,     1,     1,     3,     1,     1,     1,     1,     7,     7,
       5,     7,     4,     2,     1,     4,     3,     9,     7,     6,
       3,     5,     7,     7,     8,     6,     2,     0,     1,     0,
       2,     1,     2,     1,     2,     5,     6,     3,     1,     1,
       3
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

#line 1529 "d_parser.tab.c"

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

#line 535 "d_parser.y"



// Секция пользовательского кода
