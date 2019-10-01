/* A Bison parser, made by GNU Bison 3.3.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
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
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.3.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "imp.y" /* yacc.c:337  */

#include <stdio.h>
#include "tree.h"

extern char *yytext;
extern AST_NODE* AST;

void yyerror(char* ss) {
  printf("syntax error before %s\n", yytext);
}

int yylex(); //to prevent implicit declaration warnings.

PROPERTY current_property;


#line 87 "imp.tab.cpp" /* yacc.c:337  */
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

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "imp.tab.h".  */
#ifndef YY_YY_IMP_TAB_H_INCLUDED
# define YY_YY_IMP_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    tINTCONST = 258,
    tIDENTIFIER = 259,
    tIDENTIFIER_DECL = 260,
    tSTRING = 261,
    tCOMMAND_LOAD = 262,
    tCOMMAND_CONTAIN = 263,
    tCOMMAND_SHADER = 264,
    tCOMMAND_BUILD = 265,
    tEQUALITY = 266,
    tLESSER_EQUAL = 267,
    tGREATER_EQUAL = 268,
    tNON_EQUALITY = 269,
    tAND = 270,
    tOR = 271,
    tRETURN = 272,
    tWRITE = 273,
    tIF = 274,
    tELSE = 275,
    tTHEN = 276,
    tALLOCATE = 277,
    tWHILE = 278,
    tDO = 279,
    tTYPE = 280,
    tFUNC = 281,
    tEND = 282,
    tVAR = 283,
    tOF_LENGTH = 284,
    tBYTE = 285,
    tINT = 286,
    tSHORT = 287,
    tLONG = 288,
    tFLOAT = 289,
    tDOUBLE = 290,
    tu8 = 291,
    tu16 = 292,
    tu32 = 293,
    tu64 = 294,
    tu128 = 295,
    tu256 = 296,
    tu512 = 297,
    tSTRING_TOKEN = 298,
    tBOOL = 299,
    tTRUE = 300,
    tFALSE = 301,
    tNULL = 302,
    tBITSET = 303,
    tBOX = 304,
    tSTRUCT = 305,
    tUNION = 306,
    tVOID = 307,
    tPLUS_EQUAL = 308,
    tMINUS_EQUAL = 309,
    tTIMES_EQUAL = 310,
    tDIVIDE_EQUAL = 311,
    tAND_EQUAL = 312,
    tOR_EQUAL = 313,
    tFOR = 314,
    tDOUBLE_SEMICOLON = 315,
    tRIGHT_ARROW = 316,
    tLEFT_ARROW = 317,
    tPRINT = 318,
    tSOA = 319,
    tAOS = 320
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 18 "imp.y" /* yacc.c:352  */

   int intconst;
   char *stringconst;
   char* dynamic_string;	//for holding actual strings in our language like "hello world" (without quotes).
   struct AST_NODE* ast;
   struct DIRECTIVE_LIST* directive_list;
   struct DIRECTIVE* directive;
   struct STRING* string;
   struct FUNCTION* function;
   struct HEAD* head;
   struct TAIL* tail;
   struct TYPE* type;
   struct PAR_DECL_LIST* par_decl_list;
   struct VAR_DECL_LIST* var_decl_list;
   struct VAR_TYPE* var_type;
   struct BODY* body;
   struct DECL_LIST* decl_list;
   struct DECL* declaration;
   struct STM_LIST* statement_list;
   struct STM* statement;
   struct VARIABLE* variable;
   struct EXP *exp;
   struct TERM* term;
   struct ACT_LIST* act_list;
   struct EXP_LIST* exp_list;

#line 223 "imp.tab.cpp" /* yacc.c:352  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_IMP_TAB_H_INCLUDED  */



#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
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


#if ! defined yyoverflow || YYERROR_VERBOSE

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
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
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
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   497

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  85
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  25
/* YYNRULES -- Number of rules.  */
#define YYNRULES  96
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  176

#define YYUNDEFTOK  2
#define YYMAXUTOK   320

/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    83,     2,     2,     2,     2,     2,     2,
      72,    73,    70,    68,    79,    69,    82,    71,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    76,    80,
      66,    81,    67,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    77,     2,    78,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    74,    84,    75,     2,     2,     2,     2,
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
      65
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   165,   165,   171,   178,   182,   188,   192,   198,   202,
     206,   210,   218,   225,   232,   236,   241,   244,   252,   256,
     260,   264,   268,   272,   277,   281,   285,   289,   293,   297,
     301,   305,   309,   313,   317,   321,   325,   329,   333,   339,
     343,   350,   354,   360,   366,   372,   376,   396,   400,   404,
     426,   430,   449,   453,   457,   461,   465,   469,   473,   477,
     481,   485,   489,   493,   497,   504,   508,   512,   519,   523,
     527,   531,   535,   539,   543,   547,   551,   555,   559,   563,
     567,   574,   578,   582,   586,   590,   594,   598,   602,   606,
     610,   617,   621,   628,   632,   638,   642
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "tINTCONST", "tIDENTIFIER",
  "tIDENTIFIER_DECL", "tSTRING", "tCOMMAND_LOAD", "tCOMMAND_CONTAIN",
  "tCOMMAND_SHADER", "tCOMMAND_BUILD", "tEQUALITY", "tLESSER_EQUAL",
  "tGREATER_EQUAL", "tNON_EQUALITY", "tAND", "tOR", "tRETURN", "tWRITE",
  "tIF", "tELSE", "tTHEN", "tALLOCATE", "tWHILE", "tDO", "tTYPE", "tFUNC",
  "tEND", "tVAR", "tOF_LENGTH", "tBYTE", "tINT", "tSHORT", "tLONG",
  "tFLOAT", "tDOUBLE", "tu8", "tu16", "tu32", "tu64", "tu128", "tu256",
  "tu512", "tSTRING_TOKEN", "tBOOL", "tTRUE", "tFALSE", "tNULL", "tBITSET",
  "tBOX", "tSTRUCT", "tUNION", "tVOID", "tPLUS_EQUAL", "tMINUS_EQUAL",
  "tTIMES_EQUAL", "tDIVIDE_EQUAL", "tAND_EQUAL", "tOR_EQUAL", "tFOR",
  "tDOUBLE_SEMICOLON", "tRIGHT_ARROW", "tLEFT_ARROW", "tPRINT", "tSOA",
  "tAOS", "'<'", "'>'", "'+'", "'-'", "'*'", "'/'", "'('", "')'", "'{'",
  "'}'", "':'", "'['", "']'", "','", "';'", "'='", "'.'", "'!'", "'|'",
  "$accept", "pre_program", "program", "string", "directive_list",
  "directive", "function", "head", "array_length", "type_omittable",
  "type", "par_decl_list", "var_decl_list", "var_type", "body",
  "decl_list", "declaration", "statement_list", "statement", "variable",
  "exp", "term", "act_list", "exp_list", "property", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,    60,    62,    43,    45,
      42,    47,    40,    41,   123,   125,    58,    91,    93,    44,
      59,    61,    46,    33,   124
};
# endif

#define YYPACT_NINF -84

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-84)))

#define YYTABLE_NINF -18

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -84,    32,   -84,    17,   -84,     1,    29,    33,    44,   -84,
     -84,    31,   -84,   -84,    45,   -18,   115,   -84,   -24,   -25,
     -17,   -84,   197,   -84,    -1,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,     0,    -8,   -84,    -4,    76,     5,     6,   -84,
     -84,    79,   -84,    -1,    -1,    84,    -1,    -1,   -84,   -84,
     -39,   -84,    18,    24,   -84,   -84,   -84,    -1,    -1,    -1,
     -84,   -73,   405,   -84,    21,   -84,   -84,    27,    79,    79,
     -84,    25,    -1,    46,   138,   -84,   196,   183,   -19,   183,
     211,    30,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,
     100,   424,   -84,     7,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   -84,    79,    47,
      50,   -84,   138,   217,   -84,   -84,   -84,   104,    -1,   -84,
     -84,   -84,   -84,   287,   293,   308,   314,   411,   329,   -84,
      52,   -84,   -84,   -84,   171,   -40,   -40,   171,   399,   378,
     -40,   -40,   -59,   -59,   -84,   -84,   -84,    51,   -84,    59,
     -84,   -84,   197,   335,   -84,   -84,   -84,   -84,   -84,   -84,
     -84,   -84,   138,   -84,   -84,   -84
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       7,     0,     2,    45,     1,     0,     0,     0,     0,     6,
       3,    50,     8,     9,     0,     0,    16,    47,     0,     0,
      42,    46,    44,    11,    92,    18,    24,    19,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    21,
      20,    36,    96,     0,    38,     0,    15,     0,    43,    45,
      48,     0,    65,     0,     0,     0,     0,     0,    50,    51,
       0,    86,    65,     4,    87,    88,    89,     0,     0,     0,
      90,    81,    93,    80,     0,    91,    95,     0,     0,    40,
      14,     0,     0,     0,     0,    41,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    10,     0,     0,
       0,    39,     0,     0,    12,    43,    52,    61,     0,    54,
      63,    53,    64,     0,     0,     0,     0,     0,     0,    67,
       0,     5,    83,    85,    72,    77,    76,    75,    74,    73,
      79,    78,    70,    71,    68,    69,    94,     0,    37,     0,
      22,    49,     0,     0,    57,    58,    59,    60,    66,    56,
      82,    23,     0,    62,    55,    13
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
     -83,   -84,   -38,   -84,    78,   -84,   -84,    70,   -81,   -22,
       3,    61,    34,    14,   -84
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,     2,    70,     3,     9,    17,    18,    81,    47,
      48,   120,    19,    20,    10,    11,    21,    22,    59,    71,
      72,    73,    74,    75,    77
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      60,   125,    61,    62,    96,    63,   127,    12,   130,    98,
     128,   114,   115,    85,    92,    93,    94,    95,   104,   105,
     106,   107,   108,   109,     5,     6,     7,     8,   112,   113,
     114,   115,     4,    88,    52,    13,    16,    14,    96,   160,
     119,   121,    97,    98,    64,    65,    66,    53,    15,    54,
      49,    23,    55,    56,    24,    50,    86,    87,    96,    89,
      90,   129,    51,    98,    76,    60,    78,    60,    79,    60,
     101,    67,   103,   110,   111,   112,   113,   114,   115,    80,
     157,   173,    68,    69,    84,   123,    82,   -17,    52,   175,
      99,   143,   100,    57,   117,   133,   134,   135,   136,   137,
     138,   118,   139,   122,    58,   132,   141,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,    25,
     172,   124,   158,   159,   162,   170,   171,    83,    91,   102,
     156,   163,     0,   140,     0,     0,     0,     0,     0,     0,
      60,     0,    25,     0,     0,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
       0,     0,     0,    41,     0,    42,    43,    44,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,   105,   106,     0,    41,    52,    42,    43,
      44,    45,    46,     0,   104,   105,   106,   107,   108,   109,
      53,    52,    54,     0,     0,    55,    56,   104,   105,   106,
     107,   108,   109,     0,    53,    46,    54,     0,     0,    55,
      56,     0,   104,   105,   106,   107,   108,   109,   104,   105,
     106,   107,   108,   109,     0,     0,     0,   110,   111,   112,
     113,   114,   115,     0,     0,     0,    57,     0,     0,   110,
     111,   112,   113,   114,   115,     0,     0,    58,     0,     0,
      57,     0,   110,   111,   112,   113,   114,   115,     0,     0,
       0,    58,     0,     0,     0,     0,   126,   110,   111,   112,
     113,   114,   115,   110,   111,   112,   113,   114,   115,     0,
       0,   131,     0,     0,     0,     0,     0,   161,   104,   105,
     106,   107,   108,   109,   104,   105,   106,   107,   108,   109,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   104,
     105,   106,   107,   108,   109,   104,   105,   106,   107,   108,
     109,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     104,   105,   106,   107,   108,   109,   104,   105,   106,   107,
     108,   109,     0,   110,   111,   112,   113,   114,   115,   110,
     111,   112,   113,   114,   115,     0,     0,   164,     0,     0,
       0,     0,     0,   165,   110,   111,   112,   113,   114,   115,
     110,   111,   112,   113,   114,   115,     0,     0,   166,   104,
     105,   106,   107,   108,   167,   110,   111,   112,   113,   114,
     115,   110,   111,   112,   113,   114,   115,     0,     0,   169,
     104,   105,   106,   107,     0,   174,   104,   105,   106,   107,
     108,   109,   104,   105,   106,   107,   108,   109,     0,     0,
       0,     0,     0,     0,     0,   104,   105,   106,   107,   108,
     109,     0,     0,     0,   110,   111,   112,   113,   114,   115,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   110,   111,   112,   113,   114,
     115,   110,   111,   112,   113,   114,   115,   110,   111,   112,
     113,   114,   115,     0,   116,     0,     0,     0,     0,   168,
     110,   111,   112,   113,   114,   115,     0,   142
};

static const yytype_int16 yycheck[] =
{
      22,    84,     3,     4,    77,     6,    87,     6,    89,    82,
      29,    70,    71,    51,    53,    54,    55,    56,    11,    12,
      13,    14,    15,    16,     7,     8,     9,    10,    68,    69,
      70,    71,     0,    55,     4,     6,     5,     4,    77,   122,
      78,    79,    81,    82,    45,    46,    47,    17,     4,    19,
      74,     6,    22,    23,    72,    80,    53,    54,    77,    56,
      57,    80,    79,    82,    64,    87,    74,    89,    72,    91,
      67,    72,    69,    66,    67,    68,    69,    70,    71,     3,
     118,   162,    83,    84,     5,    82,    81,    81,     4,   172,
      72,    84,    68,    63,    73,    92,    93,    94,    95,    96,
      97,    74,     4,    78,    74,    75,     6,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,     4,
      61,    75,    75,    73,    20,    73,    75,    49,    58,    68,
     116,   128,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,
     162,    -1,     4,    -1,    -1,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      -1,    -1,    -1,    48,    -1,    50,    51,    52,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    12,    13,    -1,    48,     4,    50,    51,
      52,    76,    77,    -1,    11,    12,    13,    14,    15,    16,
      17,     4,    19,    -1,    -1,    22,    23,    11,    12,    13,
      14,    15,    16,    -1,    17,    77,    19,    -1,    -1,    22,
      23,    -1,    11,    12,    13,    14,    15,    16,    11,    12,
      13,    14,    15,    16,    -1,    -1,    -1,    66,    67,    68,
      69,    70,    71,    -1,    -1,    -1,    63,    -1,    -1,    66,
      67,    68,    69,    70,    71,    -1,    -1,    74,    -1,    -1,
      63,    -1,    66,    67,    68,    69,    70,    71,    -1,    -1,
      -1,    74,    -1,    -1,    -1,    -1,    80,    66,    67,    68,
      69,    70,    71,    66,    67,    68,    69,    70,    71,    -1,
      -1,    80,    -1,    -1,    -1,    -1,    -1,    80,    11,    12,
      13,    14,    15,    16,    11,    12,    13,    14,    15,    16,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    11,
      12,    13,    14,    15,    16,    11,    12,    13,    14,    15,
      16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      11,    12,    13,    14,    15,    16,    11,    12,    13,    14,
      15,    16,    -1,    66,    67,    68,    69,    70,    71,    66,
      67,    68,    69,    70,    71,    -1,    -1,    80,    -1,    -1,
      -1,    -1,    -1,    80,    66,    67,    68,    69,    70,    71,
      66,    67,    68,    69,    70,    71,    -1,    -1,    80,    11,
      12,    13,    14,    15,    80,    66,    67,    68,    69,    70,
      71,    66,    67,    68,    69,    70,    71,    -1,    -1,    80,
      11,    12,    13,    14,    -1,    80,    11,    12,    13,    14,
      15,    16,    11,    12,    13,    14,    15,    16,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    11,    12,    13,    14,    15,
      16,    -1,    -1,    -1,    66,    67,    68,    69,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    67,    68,    69,    70,
      71,    66,    67,    68,    69,    70,    71,    66,    67,    68,
      69,    70,    71,    -1,    79,    -1,    -1,    -1,    -1,    78,
      66,    67,    68,    69,    70,    71,    -1,    73
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    86,    87,    89,     0,     7,     8,     9,    10,    90,
      99,   100,     6,     6,     4,     4,     5,    91,    92,    97,
      98,   101,   102,     6,    72,     4,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    48,    50,    51,    52,    76,    77,    94,    95,    74,
      80,    79,     4,    17,    19,    22,    23,    63,    74,   103,
     104,     3,     4,     6,    45,    46,    47,    72,    83,    84,
      88,   104,   105,   106,   107,   108,    64,   109,    74,    72,
       3,    93,    81,    99,     5,    97,   105,   105,   104,   105,
     105,   102,    53,    54,    55,    56,    77,    81,    82,    72,
      68,   105,   106,   105,    11,    12,    13,    14,    15,    16,
      66,    67,    68,    69,    70,    71,    79,    73,    74,    97,
      96,    97,    78,   105,    75,    95,    80,   103,    29,    80,
     103,    80,    75,   105,   105,   105,   105,   105,   105,     4,
     107,     6,    73,    84,   105,   105,   105,   105,   105,   105,
     105,   105,   105,   105,   105,   105,   108,    97,    75,    73,
      95,    80,    20,   105,    80,    80,    80,    80,    78,    80,
      73,    75,    61,   103,    80,    95
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    85,    86,    87,    88,    88,    89,    89,    90,    90,
      90,    90,    91,    92,    93,    93,    94,    94,    95,    95,
      95,    95,    95,    95,    95,    95,    95,    95,    95,    95,
      95,    95,    95,    95,    95,    95,    95,    95,    95,    96,
      96,    97,    97,    98,    99,   100,   100,   101,   101,   101,
     102,   102,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   104,   104,   104,   105,   105,
     105,   105,   105,   105,   105,   105,   105,   105,   105,   105,
     105,   106,   106,   106,   106,   106,   106,   106,   106,   106,
     106,   107,   107,   108,   108,   109,   109
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     3,     2,     0,     2,     2,
       5,     3,     4,     7,     1,     0,     0,     1,     1,     1,
       1,     1,     4,     5,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     1,     1,
       0,     3,     1,     2,     2,     0,     2,     1,     2,     5,
       0,     2,     3,     3,     3,     5,     4,     4,     4,     4,
       4,     3,     5,     3,     3,     1,     4,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     1,     4,     3,     2,     3,     1,     1,     1,     1,
       1,     1,     0,     1,     3,     1,     0
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


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

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



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

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
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
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return (YYSIZE_T) (yystpcpy (yyres, yystr) - yyres);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
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
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
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
| yynewstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  *yyssp = (yytype_int16) yystate;

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = (YYSIZE_T) (yyssp - yyss + 1);

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
        case 2:
#line 166 "imp.y" /* yacc.c:1652  */
    {
		AST = (yyvsp[0].ast);
	}
#line 1516 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 3:
#line 172 "imp.y" /* yacc.c:1652  */
    {
		(yyval.ast) = make_AST((yyvsp[-1].directive_list), (yyvsp[0].body));
	}
#line 1524 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 4:
#line 179 "imp.y" /* yacc.c:1652  */
    {
		(yyval.string) = make_STRING((yyvsp[0].dynamic_string));
	}
#line 1532 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 5:
#line 183 "imp.y" /* yacc.c:1652  */
    {
		(yyval.string) = make_STRING_concatenation((yyvsp[-2].dynamic_string), (yyvsp[0].dynamic_string));
	}
#line 1540 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 6:
#line 189 "imp.y" /* yacc.c:1652  */
    {
		(yyval.directive_list) = make_DIRECTIVE_LIST_pair((yyvsp[-1].directive_list), (yyvsp[0].directive));
	}
#line 1548 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 7:
#line 193 "imp.y" /* yacc.c:1652  */
    {
		(yyval.directive_list) = make_DIRECTIVE_LIST_empty();
	}
#line 1556 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 8:
#line 199 "imp.y" /* yacc.c:1652  */
    {
		(yyval.directive) = make_DIRECTIVE_load((yyvsp[0].dynamic_string));
	}
#line 1564 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 9:
#line 203 "imp.y" /* yacc.c:1652  */
    {
		(yyval.directive) = make_DIRECTIVE_contain((yyvsp[0].dynamic_string));
	}
#line 1572 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 10:
#line 207 "imp.y" /* yacc.c:1652  */
    {
		(yyval.directive) = make_DIRECTIVE_build((yyvsp[-3].stringconst), (yyvsp[-1].act_list));
	}
#line 1580 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 11:
#line 211 "imp.y" /* yacc.c:1652  */
    {
		(yyval.directive) = make_DIRECTIVE_shader((yyvsp[-1].stringconst), (yyvsp[0].dynamic_string));
	}
#line 1588 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 12:
#line 219 "imp.y" /* yacc.c:1652  */
    {
		(yyval.function) = make_FUNC((yyvsp[-3].head), (yyvsp[-1].body));
	}
#line 1596 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 13:
#line 226 "imp.y" /* yacc.c:1652  */
    {
		(yyval.head) = make_HEAD((yyvsp[-6].stringconst), (yyvsp[-3].par_decl_list), (yyvsp[0].type));
	}
#line 1604 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 14:
#line 233 "imp.y" /* yacc.c:1652  */
    {
		(yyval.intconst) = (yyvsp[0].intconst);
	}
#line 1612 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 15:
#line 237 "imp.y" /* yacc.c:1652  */
    {
	}
#line 1619 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 16:
#line 242 "imp.y" /* yacc.c:1652  */
    {
	}
#line 1626 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 17:
#line 245 "imp.y" /* yacc.c:1652  */
    {
		(yyval.type) = (yyvsp[0].type);
	}
#line 1634 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 18:
#line 253 "imp.y" /* yacc.c:1652  */
    {
		(yyval.type) = make_TYPE_id((yyvsp[0].stringconst));
	}
#line 1642 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 19:
#line 257 "imp.y" /* yacc.c:1652  */
    {
		(yyval.type) = make_TYPE_int();
	}
#line 1650 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 20:
#line 261 "imp.y" /* yacc.c:1652  */
    {
		(yyval.type)= make_TYPE_bool();
	}
#line 1658 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 21:
#line 265 "imp.y" /* yacc.c:1652  */
    {
		(yyval.type) = make_TYPE_string();
	}
#line 1666 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 22:
#line 269 "imp.y" /* yacc.c:1652  */
    {
		(yyval.type) = make_TYPE_array((yyvsp[-2].intconst), (yyvsp[0].type));
	}
#line 1674 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 23:
#line 273 "imp.y" /* yacc.c:1652  */
    {
		(yyval.type) = make_TYPE_record((yyvsp[-1].var_decl_list), current_property);
		current_property = PROPERTY::NONE;
	}
#line 1683 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 24:
#line 278 "imp.y" /* yacc.c:1652  */
    {
		(yyval.type) = make_TYPE_byte();
	}
#line 1691 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 25:
#line 282 "imp.y" /* yacc.c:1652  */
    {
		(yyval.type) = make_TYPE_short();
	}
#line 1699 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 26:
#line 286 "imp.y" /* yacc.c:1652  */
    {
		(yyval.type) = make_TYPE_long();
	}
#line 1707 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 27:
#line 290 "imp.y" /* yacc.c:1652  */
    {
		(yyval.type) = make_TYPE_float();
	}
#line 1715 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 28:
#line 294 "imp.y" /* yacc.c:1652  */
    {
		(yyval.type) = make_TYPE_double();
	}
#line 1723 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 29:
#line 298 "imp.y" /* yacc.c:1652  */
    {
		(yyval.type) = make_TYPE_u8();
	}
#line 1731 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 30:
#line 302 "imp.y" /* yacc.c:1652  */
    {
		(yyval.type) = make_TYPE_u16();
	}
#line 1739 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 31:
#line 306 "imp.y" /* yacc.c:1652  */
    {
		(yyval.type) = make_TYPE_u32();
	}
#line 1747 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 32:
#line 310 "imp.y" /* yacc.c:1652  */
    {
		(yyval.type) = make_TYPE_u64();
	}
#line 1755 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 33:
#line 314 "imp.y" /* yacc.c:1652  */
    {
		(yyval.type) = make_TYPE_u128();
	}
#line 1763 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 34:
#line 318 "imp.y" /* yacc.c:1652  */
    {
		(yyval.type) = make_TYPE_u256();
	}
#line 1771 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 35:
#line 322 "imp.y" /* yacc.c:1652  */
    {
		(yyval.type) = make_TYPE_u512();
	}
#line 1779 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 36:
#line 326 "imp.y" /* yacc.c:1652  */
    {
		(yyval.type) = make_TYPE_bitset();
	}
#line 1787 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 37:
#line 330 "imp.y" /* yacc.c:1652  */
    {
		(yyval.type) = make_TYPE_union((yyvsp[-1].var_decl_list));
	}
#line 1795 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 38:
#line 334 "imp.y" /* yacc.c:1652  */
    {
		(yyval.type) = make_TYPE_void();
	}
#line 1803 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 39:
#line 340 "imp.y" /* yacc.c:1652  */
    {
		(yyval.par_decl_list) = make_PAR_DECL_LIST((yyvsp[0].var_decl_list));
	}
#line 1811 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 40:
#line 344 "imp.y" /* yacc.c:1652  */
    {
		(yyval.par_decl_list) = make_PAR_DECL_LIST_empty();
	}
#line 1819 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 41:
#line 351 "imp.y" /* yacc.c:1652  */
    {
		(yyval.var_decl_list) = make_VAR_DECL_LIST_pair((yyvsp[-2].var_type), (yyvsp[0].var_decl_list));
	}
#line 1827 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 42:
#line 355 "imp.y" /* yacc.c:1652  */
    {
		(yyval.var_decl_list) = make_VAR_DECL_LIST_last((yyvsp[0].var_type));
	}
#line 1835 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 43:
#line 361 "imp.y" /* yacc.c:1652  */
    {
		(yyval.var_type) = make_VAR_TYPE((yyvsp[-1].stringconst), (yyvsp[0].type));
	}
#line 1843 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 44:
#line 367 "imp.y" /* yacc.c:1652  */
    {
		(yyval.body) = make_BODY((yyvsp[-1].decl_list),(yyvsp[0].statement_list));
	}
#line 1851 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 45:
#line 373 "imp.y" /* yacc.c:1652  */
    {
		(yyval.decl_list) = make_DECL_LIST_empty();
	}
#line 1859 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 46:
#line 377 "imp.y" /* yacc.c:1652  */
    {
		(yyval.decl_list) = make_DECL_LIST_pair((yyvsp[0].declaration), (yyvsp[-1].decl_list));
	}
#line 1867 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 47:
#line 397 "imp.y" /* yacc.c:1652  */
    {
		(yyval.declaration) = make_DECL_func((yyvsp[0].function));
	}
#line 1875 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 48:
#line 401 "imp.y" /* yacc.c:1652  */
    {
		(yyval.declaration) = make_DECL_vars((yyvsp[-1].var_decl_list));
	}
#line 1883 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 49:
#line 405 "imp.y" /* yacc.c:1652  */
    {
		//
	}
#line 1891 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 50:
#line 427 "imp.y" /* yacc.c:1652  */
    {
		(yyval.statement_list) = make_STM_LIST_empty();
	}
#line 1899 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 51:
#line 431 "imp.y" /* yacc.c:1652  */
    {
		(yyval.statement_list) = make_STM_LIST_pair((yyvsp[0].statement), (yyvsp[-1].statement_list));
	}
#line 1907 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 52:
#line 450 "imp.y" /* yacc.c:1652  */
    {
		(yyval.statement) = make_STM_return((yyvsp[-1].exp));
	}
#line 1915 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 53:
#line 454 "imp.y" /* yacc.c:1652  */
    {
		(yyval.statement) = make_STM_printf((yyvsp[-1].exp));
	}
#line 1923 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 54:
#line 458 "imp.y" /* yacc.c:1652  */
    {
		(yyval.statement) = make_STM_allocate((yyvsp[-1].variable));
	}
#line 1931 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 55:
#line 462 "imp.y" /* yacc.c:1652  */
    {
		(yyval.statement) = make_STM_allocate_of_length((yyvsp[-3].variable), (yyvsp[-1].exp));
	}
#line 1939 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 56:
#line 466 "imp.y" /* yacc.c:1652  */
    {
		(yyval.statement) = make_STM_assign((yyvsp[-3].variable), (yyvsp[-1].exp), 0);
	}
#line 1947 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 57:
#line 470 "imp.y" /* yacc.c:1652  */
    {
		(yyval.statement) = make_STM_assign((yyvsp[-3].variable), (yyvsp[-1].exp), 1); //variant 1
	}
#line 1955 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 58:
#line 474 "imp.y" /* yacc.c:1652  */
    {
		(yyval.statement) = make_STM_assign((yyvsp[-3].variable), (yyvsp[-1].exp), 2); //variant 2
	}
#line 1963 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 59:
#line 478 "imp.y" /* yacc.c:1652  */
    {
		(yyval.statement) = make_STM_assign((yyvsp[-3].variable), (yyvsp[-1].exp), 3); //variant 3
	}
#line 1971 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 60:
#line 482 "imp.y" /* yacc.c:1652  */
    {
		(yyval.statement) = make_STM_assign((yyvsp[-3].variable), (yyvsp[-1].exp), 4); //variant 4
	}
#line 1979 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 61:
#line 486 "imp.y" /* yacc.c:1652  */
    {
		(yyval.statement) =  make_STM_if((yyvsp[-1].exp), (yyvsp[0].statement));
	}
#line 1987 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 62:
#line 490 "imp.y" /* yacc.c:1652  */
    {
		(yyval.statement) = make_STM_if_else((yyvsp[-3].exp), (yyvsp[-2].statement), (yyvsp[0].statement));
	}
#line 1995 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 63:
#line 494 "imp.y" /* yacc.c:1652  */
    {
		(yyval.statement) = make_STM_while((yyvsp[-1].exp), (yyvsp[0].statement));
	}
#line 2003 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 64:
#line 498 "imp.y" /* yacc.c:1652  */
    {
		(yyval.statement) = make_STM_list((yyvsp[-1].statement_list));
	}
#line 2011 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 65:
#line 505 "imp.y" /* yacc.c:1652  */
    {
		(yyval.variable) = make_VARIABLE((yyvsp[0].stringconst));
	}
#line 2019 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 66:
#line 509 "imp.y" /* yacc.c:1652  */
    {
		(yyval.variable) =  make_VARIABLE_array_access((yyvsp[-3].variable), (yyvsp[-1].exp));
	}
#line 2027 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 67:
#line 513 "imp.y" /* yacc.c:1652  */
    {
		(yyval.variable) = make_VARIABLE_access((yyvsp[-2].variable), (yyvsp[0].stringconst));
	}
#line 2035 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 68:
#line 520 "imp.y" /* yacc.c:1652  */
    {
		(yyval.exp) = make_EXP_times((yyvsp[-2].exp),(yyvsp[0].exp));
	}
#line 2043 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 69:
#line 524 "imp.y" /* yacc.c:1652  */
    {
		(yyval.exp) = make_EXP_div((yyvsp[-2].exp),(yyvsp[0].exp));
	}
#line 2051 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 70:
#line 528 "imp.y" /* yacc.c:1652  */
    {
		(yyval.exp) = make_EXP_plus((yyvsp[-2].exp),(yyvsp[0].exp));
	}
#line 2059 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 71:
#line 532 "imp.y" /* yacc.c:1652  */
    {
		(yyval.exp) = make_EXP_minus((yyvsp[-2].exp),(yyvsp[0].exp));
	}
#line 2067 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 72:
#line 536 "imp.y" /* yacc.c:1652  */
    {
		(yyval.exp) = make_EXP_equality((yyvsp[-2].exp), (yyvsp[0].exp));
	}
#line 2075 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 73:
#line 540 "imp.y" /* yacc.c:1652  */
    {
		(yyval.exp) = make_EXP_or((yyvsp[-2].exp), (yyvsp[0].exp));
	}
#line 2083 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 74:
#line 544 "imp.y" /* yacc.c:1652  */
    {
		(yyval.exp) = make_EXP_and((yyvsp[-2].exp), (yyvsp[0].exp));
	}
#line 2091 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 75:
#line 548 "imp.y" /* yacc.c:1652  */
    {
		(yyval.exp) = make_EXP_non_equality((yyvsp[-2].exp), (yyvsp[0].exp));
	}
#line 2099 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 76:
#line 552 "imp.y" /* yacc.c:1652  */
    {
		(yyval.exp) = make_EXP_greater_equal((yyvsp[-2].exp), (yyvsp[0].exp));
	}
#line 2107 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 77:
#line 556 "imp.y" /* yacc.c:1652  */
    {
		(yyval.exp) = make_EXP_lesser_equal((yyvsp[-2].exp), (yyvsp[0].exp));
	}
#line 2115 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 78:
#line 560 "imp.y" /* yacc.c:1652  */
    {
		(yyval.exp) = make_EXP_greater((yyvsp[-2].exp), (yyvsp[0].exp));
	}
#line 2123 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 79:
#line 564 "imp.y" /* yacc.c:1652  */
    {
		(yyval.exp) = make_EXP_lesser((yyvsp[-2].exp), (yyvsp[0].exp));
	}
#line 2131 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 80:
#line 568 "imp.y" /* yacc.c:1652  */
    {
		(yyval.exp) = make_EXP_term((yyvsp[0].term));
	}
#line 2139 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 81:
#line 575 "imp.y" /* yacc.c:1652  */
    {
		(yyval.term) = make_TERM_variable((yyvsp[0].variable));
	}
#line 2147 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 82:
#line 579 "imp.y" /* yacc.c:1652  */
    {
		(yyval.term) = make_TERM_id_act_list((yyvsp[-3].stringconst), (yyvsp[-1].act_list));
	}
#line 2155 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 83:
#line 583 "imp.y" /* yacc.c:1652  */
    {
		(yyval.term) = make_TERM_expression((yyvsp[-1].exp));
	}
#line 2163 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 84:
#line 587 "imp.y" /* yacc.c:1652  */
    {
		(yyval.term) = make_TERM_negate((yyvsp[0].term));
	}
#line 2171 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 85:
#line 591 "imp.y" /* yacc.c:1652  */
    {
		(yyval.term) = make_TERM_absolute((yyvsp[-1].exp));
	}
#line 2179 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 86:
#line 595 "imp.y" /* yacc.c:1652  */
    {
		(yyval.term) = make_TERM_num((yyvsp[0].intconst));
	}
#line 2187 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 87:
#line 599 "imp.y" /* yacc.c:1652  */
    {
		(yyval.term) = make_TERM_true();
	}
#line 2195 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 88:
#line 603 "imp.y" /* yacc.c:1652  */
    {
		(yyval.term) = make_TERM_false();
	}
#line 2203 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 89:
#line 607 "imp.y" /* yacc.c:1652  */
    {
		(yyval.term) = make_TERM_null();
	}
#line 2211 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 90:
#line 611 "imp.y" /* yacc.c:1652  */
    {
		(yyval.term) = make_TERM_string((yyvsp[0].string));
	}
#line 2219 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 91:
#line 618 "imp.y" /* yacc.c:1652  */
    {
		(yyval.act_list) = make_ACT_LIST_exp_list((yyvsp[0].exp_list));
	}
#line 2227 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 92:
#line 622 "imp.y" /* yacc.c:1652  */
    {
		(yyval.act_list) = make_ACT_LIST_empty();
	}
#line 2235 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 93:
#line 629 "imp.y" /* yacc.c:1652  */
    {
		(yyval.exp_list) = make_EXP_LIST_expression((yyvsp[0].exp));
	}
#line 2243 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 94:
#line 633 "imp.y" /* yacc.c:1652  */
    {
		(yyval.exp_list) = make_EXP_LIST_pair((yyvsp[-2].exp), (yyvsp[0].exp_list));
	}
#line 2251 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 95:
#line 639 "imp.y" /* yacc.c:1652  */
    {
		current_property = PROPERTY::SOA;
	}
#line 2259 "imp.tab.cpp" /* yacc.c:1652  */
    break;

  case 96:
#line 643 "imp.y" /* yacc.c:1652  */
    {
	}
#line 2266 "imp.tab.cpp" /* yacc.c:1652  */
    break;


#line 2270 "imp.tab.cpp" /* yacc.c:1652  */
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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

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
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
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

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
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
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 648 "imp.y" /* yacc.c:1918  */
