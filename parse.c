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
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         _mos_yyparse
#define yylex           _mos_yylex
#define yyerror         _mos_yyerror
#define yydebug         _mos_yydebug
#define yynerrs         _mos_yynerrs

/* First part of user prologue.  */
// #line 5 "parse.y"


// clang-format off
#include "mos/mos.h"
#include "mos/expr.h"
#include "mos/lex.h"
#include "parse.h"
#include <stdlib.h> /* malloc, free */
#include <string.h> /* strcat */
// clang-format on

#define nil mos_vector_make(0,0)
#define undef mos_undef
#define append(L,X) mos_send((L), mos_s(append_), (X))
#define cons(X,L) mos_send((L), mos_s(insert_), (X))
#define selExpr(S,L) mos_vector_make_(2, (S), (L))
#define vector1(X) mos_vector_make_(1, (X))
#define vector2(X,Y) mos_vector_make_(2, (X), (Y))
#define sel(X) mos_vector_V(X)[0]
#define args(X) mos_vector_V(X)[1]

#define PARSE_DEBUG _mos_yydebug

static
mos_value exprMsg(mos_value CNTX, mos_value RCVR, mos_value SEL, mos_value ARGS)
{
  if ( mos_EQ(ARGS,mos_undef) ) {
    static mos_value x = mos_undef;

    if ( mos_EQ(x,mos_undef) ) {
      x = mos_vector_make(0,0);
    }

    ARGS = x;
  }

  SEL = mos_send(SEL, mos_s(asSelector));

  /* SEL = mos_exprConstant(SEL); */
  CNTX = mos_exprSend_V(CNTX, RCVR, SEL, ARGS);

  if ( PARSE_DEBUG ) {
    mos_printf(mos_FILE(err), "parse: got %MW\n", mos_s(printExprOn_), CNTX);
  }

  return CNTX;
}

#define YYSTYPE mos_value

#ifndef YYBISON
#error Must use bison
#endif

#define YYDEBUG 1

#ifndef YYDEBUG
#define YYDEBUG 1
#endif
#define YYERROR_VERBOSE 1

static int _mos_yyerror(YYLTYPE *_yylloc, mos_parse_cntx *cntx, const char *msg)
{
  mos_error(mos_s(parseError), msg);
  return 0;
}


// #line 145 "parse.c"

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

#include "parse.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_ERROR = 3,                      /* ERROR  */
  YYSYMBOL_NAME = 4,                       /* NAME  */
  YYSYMBOL_KEYWORD = 5,                    /* KEYWORD  */
  YYSYMBOL_CAPWORD = 6,                    /* CAPWORD  */
  YYSYMBOL_OPERATOR = 7,                   /* OPERATOR  */
  YYSYMBOL_STRING = 8,                     /* STRING  */
  YYSYMBOL_CONSTANT = 9,                   /* CONSTANT  */
  YYSYMBOL_MEMO = 10,                      /* MEMO  */
  YYSYMBOL_EXPR = 11,                      /* EXPR  */
  YYSYMBOL_12_ = 12,                       /* '['  */
  YYSYMBOL_13_ = 13,                       /* ';'  */
  YYSYMBOL_14_ = 14,                       /* ']'  */
  YYSYMBOL_15_ = 15,                       /* ':'  */
  YYSYMBOL_16_ = 16,                       /* '('  */
  YYSYMBOL_17_ = 17,                       /* ')'  */
  YYSYMBOL_18_ = 18,                       /* '{'  */
  YYSYMBOL_19_ = 19,                       /* '}'  */
  YYSYMBOL_20_ = 20,                       /* '@'  */
  YYSYMBOL_YYACCEPT = 21,                  /* $accept  */
  YYSYMBOL_start = 22,                     /* start  */
  YYSYMBOL_expr = 23,                      /* expr  */
  YYSYMBOL_msg = 24,                       /* msg  */
  YYSYMBOL_keyword_expr = 25,              /* keyword_expr  */
  YYSYMBOL_sel_expr_list = 26,             /* sel_expr_list  */
  YYSYMBOL_array_expr = 27,                /* array_expr  */
  YYSYMBOL_op_expr = 28,                   /* op_expr  */
  YYSYMBOL_name_expr = 29,                 /* name_expr  */
  YYSYMBOL_rootexpr = 30,                  /* rootexpr  */
  YYSYMBOL_object = 31,                    /* object  */
  YYSYMBOL_object_body = 32,               /* object_body  */
  YYSYMBOL_slots = 33,                     /* slots  */
  YYSYMBOL_regular_slot = 34,              /* regular_slot  */
  YYSYMBOL_method_slot = 35,               /* method_slot  */
  YYSYMBOL_sel_name_list = 36,             /* sel_name_list  */
  YYSYMBOL_capword_name_list = 37,         /* capword_name_list  */
  YYSYMBOL_locals = 38,                    /* locals  */
  YYSYMBOL_local_list = 39,                /* local_list  */
  YYSYMBOL_local = 40,                     /* local  */
  YYSYMBOL_stmts = 41,                     /* stmts  */
  YYSYMBOL_block_arg_list = 42,            /* block_arg_list  */
  YYSYMBOL_block_args = 43,                /* block_args  */
  YYSYMBOL_block = 44                      /* block  */
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
typedef yytype_int8 yy_state_t;

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
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

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
#define YYFINAL  37
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   94

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  21
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  24
/* YYNRULES -- Number of rules.  */
#define YYNRULES  57
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  89

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   266


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
      16,    17,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    15,    13,
       2,     2,     2,     2,    20,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    12,     2,    14,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    18,     2,    19,     2,     2,     2,     2,
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
       5,     6,     7,     8,     9,    10,    11
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    85,    85,    86,    88,    93,    98,   102,   104,   106,
     111,   113,   118,   120,   122,   127,   129,   131,   135,   139,
     143,   155,   156,   157,   158,   159,   160,   161,   165,   171,
     172,   178,   179,   181,   183,   185,   190,   203,   208,   213,
     215,   217,   223,   224,   230,   231,   237,   238,   240,   245,
     251,   252,   254,   260,   261,   267,   268,   272
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "ERROR", "NAME",
  "KEYWORD", "CAPWORD", "OPERATOR", "STRING", "CONSTANT", "MEMO", "EXPR",
  "'['", "';'", "']'", "':'", "'('", "')'", "'{'", "'}'", "'@'", "$accept",
  "start", "expr", "msg", "keyword_expr", "sel_expr_list", "array_expr",
  "op_expr", "name_expr", "rootexpr", "object", "object_body", "slots",
  "regular_slot", "method_slot", "sel_name_list", "capword_name_list",
  "locals", "local_list", "local", "stmts", "block_arg_list", "block_args",
  "block", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-48)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-40)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      11,   -48,    44,    57,   -48,   -48,    44,   -48,    -8,    11,
      44,    29,     9,     4,   -48,     8,    25,     2,    30,    34,
     -48,   -48,   -48,   -48,   -48,   -48,    39,    24,   -48,    28,
      23,    42,    53,    38,    46,    72,    45,   -48,   -48,    25,
      44,    44,    57,   -48,    39,    54,    66,    44,   -48,    44,
     -48,   -48,    44,    55,    29,    29,   -48,    24,     8,    58,
     -48,   -48,   -48,    71,   -48,    61,    69,    70,    73,   -48,
      78,   -48,   -48,   -48,   -48,    44,    74,   -48,    66,    44,
     -48,    82,    75,    57,   -48,   -48,   -48,   -48,    30
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,    18,     0,     0,    22,    21,     0,    24,    55,     2,
       0,    31,     0,     0,     5,     6,     8,     9,    14,    17,
      20,    25,    26,    10,    16,    23,    53,    44,     4,     0,
      37,     0,     0,    29,    32,    31,     0,     1,     3,     7,
       0,     0,     0,    19,    53,     0,    46,    50,    27,     0,
      42,    40,    50,     0,    31,    31,    34,    44,    11,     0,
      15,    54,    56,    37,    49,     0,    47,    51,     0,    36,
      41,    30,    28,    33,    35,    50,    12,    45,    46,    50,
      57,     0,     0,     0,    48,    52,    43,    38,    13
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -48,    79,     1,   -48,     0,   -11,   -48,    -3,   -48,   -48,
     -48,   -48,   -29,   -43,   -48,   -48,   -48,    33,    13,   -48,
     -47,    48,   -48,   -48
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,    12,    67,    14,    15,    16,    17,    18,    19,    20,
      21,    53,    33,    34,    35,    36,    70,    47,    65,    66,
      68,    45,    27,    22
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      24,    13,    23,    64,    39,    71,    56,    25,    26,    37,
      13,    29,    39,     2,    41,     1,     2,    38,     3,     4,
       5,     6,     7,     8,     9,    73,    74,    10,    82,    11,
      49,    40,    85,    30,    31,    64,    32,    42,    43,    60,
      58,   -39,    59,    44,    46,    48,    50,    39,     1,     2,
      69,     3,     4,     5,     6,     7,     8,    51,    52,    54,
      10,     1,    11,    57,     3,     4,     5,     6,     7,     8,
      63,    62,    76,    10,    72,    11,    30,    31,    49,    32,
      88,    77,    78,    79,    81,    55,    86,    80,    28,    83,
      75,    84,    61,     0,    87
};

static const yytype_int8 yycheck[] =
{
       3,     0,     2,    46,    15,    52,    35,     6,    16,     0,
       9,    10,    23,     5,    12,     4,     5,    13,     7,     8,
       9,    10,    11,    12,    13,    54,    55,    16,    75,    18,
       7,     6,    79,     4,     5,    78,     7,     7,     4,    42,
      40,    18,    41,     4,    20,    17,     4,    58,     4,     5,
      49,     7,     8,     9,    10,    11,    12,     4,    20,    13,
      16,     4,    18,    18,     7,     8,     9,    10,    11,    12,
       4,    17,    14,    16,    19,    18,     4,     5,     7,     7,
      83,    20,    13,    13,     6,    13,     4,    14,     9,    15,
      57,    78,    44,    -1,    19
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     4,     5,     7,     8,     9,    10,    11,    12,    13,
      16,    18,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    44,    25,    28,    23,    16,    43,    22,    23,
       4,     5,     7,    33,    34,    35,    36,     0,    13,    26,
       6,    12,     7,     4,     4,    42,    20,    38,    17,     7,
       4,     4,    20,    32,    13,    13,    33,    18,    25,    23,
      28,    42,    17,     4,    34,    39,    40,    23,    41,    23,
      37,    41,    19,    33,    33,    38,    14,    20,    13,    13,
      14,     6,    41,    15,    39,    41,     4,    19,    28
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    21,    22,    22,    22,    23,    24,    25,    25,    25,
      26,    26,    27,    27,    27,    28,    28,    28,    29,    29,
      29,    30,    30,    30,    30,    30,    30,    30,    31,    32,
      32,    33,    33,    33,    33,    33,    34,    34,    35,    36,
      36,    36,    37,    37,    38,    38,    39,    39,    39,    40,
      41,    41,    41,    42,    42,    43,    43,    44
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     1,     1,     2,     1,     1,
       2,     3,     4,     6,     1,     3,     2,     1,     1,     2,
       1,     1,     1,     2,     1,     1,     1,     3,     4,     0,
       2,     0,     1,     3,     2,     3,     3,     1,     5,     1,
       2,     3,     0,     3,     0,     3,     0,     1,     3,     1,
       0,     1,     3,     0,     2,     0,     3,     5
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
        yyerror (&yylloc, cntx, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


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


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location, cntx); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, mos_parse_cntx* cntx)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  YY_USE (cntx);
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
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, mos_parse_cntx* cntx)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp, cntx);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule, mos_parse_cntx* cntx)
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
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]), cntx);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, cntx); \
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
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, mos_parse_cntx* cntx)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  YY_USE (cntx);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

int
yyparse (mos_parse_cntx* cntx)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs = 0;

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

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
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
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
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
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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
      yychar = yylex (&yylval, &yylloc, cntx);
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
      yyerror_range[1] = yylloc;
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
  *++yylsp = yylloc;

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

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* start: %empty  */
// #line 85 "parse.y"
        { cntx->expr = mos_eos; return(0); }
// #line 1356 "parse.c"
    break;

  case 3: /* start: expr ';'  */
// #line 87 "parse.y"
        { cntx->expr = yyvsp[-1]; return(0); }
// #line 1362 "parse.c"
    break;

  case 4: /* start: ';' start  */
// #line 89 "parse.y"
        { (void) yynerrs; yyval = yyvsp[0]; }
// #line 1368 "parse.c"
    break;

  case 7: /* keyword_expr: keyword_expr sel_expr_list  */
// #line 103 "parse.y"
        { yyval = exprMsg(undef, yyvsp[-1], sel(yyvsp[0]), args(yyvsp[0])); }
// #line 1374 "parse.c"
    break;

  case 8: /* keyword_expr: sel_expr_list  */
// #line 105 "parse.y"
        { yyval = exprMsg(undef, undef, sel(yyvsp[0]), args(yyvsp[0])); }
// #line 1380 "parse.c"
    break;

  case 10: /* sel_expr_list: KEYWORD keyword_expr  */
// #line 112 "parse.y"
        { yyval = selExpr(yyvsp[-1],vector1(yyvsp[0])); }
// #line 1386 "parse.c"
    break;

  case 11: /* sel_expr_list: sel_expr_list CAPWORD keyword_expr  */
// #line 114 "parse.y"
        { yyval = yyvsp[-2]; append(sel(yyvsp[-2]), yyvsp[-1]); append(args(yyvsp[-2]), yyvsp[0]); }
// #line 1392 "parse.c"
    break;

  case 12: /* array_expr: array_expr '[' expr ']'  */
// #line 119 "parse.y"
        { yyval = exprMsg(undef, yyvsp[-3], mos_s(get_), vector1(yyvsp[-1])); }
// #line 1398 "parse.c"
    break;

  case 13: /* array_expr: array_expr '[' expr ']' ':' op_expr  */
// #line 121 "parse.y"
        { yyval = exprMsg(undef, yyvsp[-5], mos_s(set_Value_), vector2(yyvsp[-3], yyvsp[0])); }
// #line 1404 "parse.c"
    break;

  case 15: /* op_expr: op_expr OPERATOR op_expr  */
// #line 128 "parse.y"
        { yyval = exprMsg(undef, yyvsp[-2], yyvsp[-1], vector1(yyvsp[0])); }
// #line 1410 "parse.c"
    break;

  case 16: /* op_expr: OPERATOR op_expr  */
// #line 130 "parse.y"
        { yyval = exprMsg(undef, undef, yyvsp[-1], vector1(yyvsp[0])); }
// #line 1416 "parse.c"
    break;

  case 18: /* name_expr: NAME  */
// #line 136 "parse.y"
        {
	  yyval = exprMsg(undef, undef, yyvsp[0], undef);
	}
// #line 1424 "parse.c"
    break;

  case 19: /* name_expr: name_expr NAME  */
// #line 140 "parse.y"
        {
	  yyval = exprMsg(undef, yyvsp[-1], yyvsp[0], undef);
	}
// #line 1432 "parse.c"
    break;

  case 21: /* rootexpr: CONSTANT  */
// #line 155 "parse.y"
             { yyval = mos_exprConstant(yyvsp[0]); }
// #line 1438 "parse.c"
    break;

  case 22: /* rootexpr: STRING  */
// #line 156 "parse.y"
           { yyval = mos_exprConstant(yyvsp[0]); }
// #line 1444 "parse.c"
    break;

  case 23: /* rootexpr: MEMO expr  */
// #line 157 "parse.y"
              { yyval = mos_exprMemo(yyvsp[0]); }
// #line 1450 "parse.c"
    break;

  case 27: /* rootexpr: '(' expr ')'  */
// #line 161 "parse.y"
                 { yyval = mos_exprGroup(yyvsp[-1]); }
// #line 1456 "parse.c"
    break;

  case 28: /* object: '{' slots object_body '}'  */
// #line 166 "parse.y"
        { yyval = mos_exprObject(yyvsp[-2], yyvsp[-1]); }
// #line 1462 "parse.c"
    break;

  case 29: /* object_body: %empty  */
// #line 171 "parse.y"
        { yyval = nil; }
// #line 1468 "parse.c"
    break;

  case 30: /* object_body: '@' stmts  */
// #line 173 "parse.y"
        { yyval = yyvsp[0]; }
// #line 1474 "parse.c"
    break;

  case 31: /* slots: %empty  */
// #line 178 "parse.y"
        { yyval = nil; }
// #line 1480 "parse.c"
    break;

  case 32: /* slots: regular_slot  */
// #line 180 "parse.y"
        { yyval = vector1(yyvsp[0]); }
// #line 1486 "parse.c"
    break;

  case 33: /* slots: regular_slot ';' slots  */
// #line 182 "parse.y"
        { yyval = cons(yyvsp[-2], yyvsp[0]); }
// #line 1492 "parse.c"
    break;

  case 34: /* slots: method_slot slots  */
// #line 184 "parse.y"
        { yyval = cons(yyvsp[-1], yyvsp[0]); }
// #line 1498 "parse.c"
    break;

  case 35: /* slots: method_slot ';' slots  */
// #line 186 "parse.y"
        { yyval = cons(yyvsp[-2], yyvsp[0]); }
// #line 1504 "parse.c"
    break;

  case 36: /* regular_slot: NAME OPERATOR expr  */
// #line 191 "parse.y"
        {
	  if ( mos_EQ(yyvsp[-1], mos_s(__ASN__)) ) {
	    yyval = mos_exprReadOnlySlot(yyvsp[-2], yyvsp[0]);
	  } else
	  if ( mos_EQ(yyvsp[-1], mos_s(__RAR__)) ) {
	    yyval = mos_exprSlot(yyvsp[-2], yyvsp[0]);
	  } else {
	    mos_value n = mos_send(yyvsp[-2], mos_s(asString));
	    mos_error(mos_s(parseError), "Slot definition should be \"%W -> ...\" or \"%W = ...\"", n, n);
	    return -1;
	  }
	}
// #line 1521 "parse.c"
    break;

  case 37: /* regular_slot: NAME  */
// #line 204 "parse.y"
        { yyval = mos_exprSlot(yyvsp[0], undef); }
// #line 1527 "parse.c"
    break;

  case 38: /* method_slot: sel_name_list '{' locals stmts '}'  */
// #line 209 "parse.y"
        { yyval = mos_exprMethodSlot(sel(yyvsp[-4]), mos_exprMethod(args(yyvsp[-4]), yyvsp[-2], yyvsp[-1])); }
// #line 1533 "parse.c"
    break;

  case 39: /* sel_name_list: NAME  */
// #line 214 "parse.y"
        { yyval = selExpr(yyvsp[0], nil); }
// #line 1539 "parse.c"
    break;

  case 40: /* sel_name_list: OPERATOR NAME  */
// #line 216 "parse.y"
        { yyval = selExpr(yyvsp[-1], vector1(yyvsp[0])); }
// #line 1545 "parse.c"
    break;

  case 41: /* sel_name_list: KEYWORD NAME capword_name_list  */
// #line 218 "parse.y"
        { yyval = yyvsp[0]; cons(yyvsp[-2], sel(yyvsp[0])); cons(yyvsp[-1], args(yyvsp[0])); }
// #line 1551 "parse.c"
    break;

  case 42: /* capword_name_list: %empty  */
// #line 223 "parse.y"
        { yyval = selExpr(mos_string_make(0,0),nil); }
// #line 1557 "parse.c"
    break;

  case 43: /* capword_name_list: capword_name_list CAPWORD NAME  */
// #line 225 "parse.y"
        { yyval = yyvsp[-2]; append(sel(yyvsp[-2]),yyvsp[-1]); append(args(yyvsp[-2]),yyvsp[0]); }
// #line 1563 "parse.c"
    break;

  case 44: /* locals: %empty  */
// #line 230 "parse.y"
        { yyval = nil; }
// #line 1569 "parse.c"
    break;

  case 45: /* locals: '@' local_list '@'  */
// #line 232 "parse.y"
        { yyval = yyvsp[-1]; }
// #line 1575 "parse.c"
    break;

  case 46: /* local_list: %empty  */
// #line 237 "parse.y"
        { yyval = nil; }
// #line 1581 "parse.c"
    break;

  case 47: /* local_list: local  */
// #line 239 "parse.y"
        { yyval = vector1(yyvsp[0]); }
// #line 1587 "parse.c"
    break;

  case 48: /* local_list: local ';' local_list  */
// #line 241 "parse.y"
        { yyval = cons(yyvsp[-2], yyvsp[0]); }
// #line 1593 "parse.c"
    break;

  case 49: /* local: regular_slot  */
// #line 246 "parse.y"
        { yyval = yyvsp[0]; }
// #line 1599 "parse.c"
    break;

  case 50: /* stmts: %empty  */
// #line 251 "parse.y"
        { yyval = nil; }
// #line 1605 "parse.c"
    break;

  case 51: /* stmts: expr  */
// #line 253 "parse.y"
        { yyval = vector1(yyvsp[0]); }
// #line 1611 "parse.c"
    break;

  case 52: /* stmts: expr ';' stmts  */
// #line 255 "parse.y"
        { yyval = cons(yyvsp[-2], yyvsp[0]); }
// #line 1617 "parse.c"
    break;

  case 53: /* block_arg_list: %empty  */
// #line 260 "parse.y"
        { yyval = nil; }
// #line 1623 "parse.c"
    break;

  case 54: /* block_arg_list: NAME block_arg_list  */
// #line 262 "parse.y"
        { yyval = cons(yyvsp[-1], yyvsp[0]); }
// #line 1629 "parse.c"
    break;

  case 55: /* block_args: %empty  */
// #line 267 "parse.y"
        { yyval = nil; }
// #line 1635 "parse.c"
    break;

  case 56: /* block_args: '(' block_arg_list ')'  */
// #line 269 "parse.y"
        { yyval = yyvsp[-1]; }
// #line 1641 "parse.c"
    break;

  case 57: /* block: '[' block_args locals stmts ']'  */
// #line 273 "parse.y"
        { yyval = mos_exprBlock(yyvsp[-3], yyvsp[-2], yyvsp[-1]); }
// #line 1647 "parse.c"
    break;


// #line 1651 "parse.c"

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
  *++yylsp = yyloc;

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
      yyerror (&yylloc, cntx, YY_("syntax error"));
    }

  yyerror_range[1] = yylloc;
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
                      yytoken, &yylval, &yylloc, cntx);
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp, cntx);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

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
  yyerror (&yylloc, cntx, YY_("memory exhausted"));
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
                  yytoken, &yylval, &yylloc, cntx);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp, cntx);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

// #line 275 "parse.y"


#include "mos/lex.h"

/******************************************************************/


mos_ANNOT("Module: parser")
mos_ANNOT("Doc: Parse MOS language expressions.")


/******************************************************************/


  mos_ANNOT("Category: Parse")


mos_METHOD(parser,parseExprFrom_)
{
  extern int yydebug;
  mos_parse_cntx cntx;
  int yydebug_save;

  /* Initialize context. */
  cntx.stream = mos_ARGV[0];
  cntx.expr = mos_undef;
  cntx.error = mos_undef;

  /* Save yydebug. */
  yydebug_save = yydebug;

  /* Set yydebug. */
  yydebug = mos_NE(mos_send(mos_RCVR, mos_s(parseDebug)), mos_false);
  // yydebug = 1;

  /* Parse top-level expression. */
  if ( _mos_yyparse(&cntx) ) {
    /* Cannot parse. */
    cntx.expr = mos_undef;
  }

  /* Restore yydebug. */
  yydebug = yydebug_save;

  /* Return the expression. */
  mos_return(cntx.expr);
}
mos_METHOD_END

  mos_ANNOT_END


/******************************************************************/


mos_OBJECT(parser)
mos_OBJECT_M(parser,parseExprFrom_)
mos_ANNOT("Doc: Controls parse debug output.")
mos_OBJECT_A(parseDebug,0)
mos_ANNOT_END
mos_OBJECT_SLOTS(parser)
mos_OBJECT_S(mos_false)
mos_OBJECT_END(compiler,parser,mos_object,basicMeta)


/******************************************************************/


mos_ANNOT_END
mos_ANNOT_END


/******************************************************************/

