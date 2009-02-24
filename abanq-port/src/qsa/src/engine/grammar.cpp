/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 1

/* Substitute the variable and function names.  */
#define yyparse qsyyparse
#define yylex   qsyylex
#define yyerror qsyyerror
#define yylval  qsyylval
#define yychar  qsyychar
#define yydebug qsyydebug
#define yynerrs qsyynerrs
#define yylloc qsyylloc

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NULLTOKEN = 258,
     TRUETOKEN = 259,
     FALSETOKEN = 260,
     BREAK = 261,
     CASE = 262,
     DEFAULT = 263,
     FOR = 264,
     NEW = 265,
     VAR = 266,
     CONTINUE = 267,
     FUNCTION = 268,
     RETURN = 269,
     QS_VOID = 270,
     QS_DELETE = 271,
     IF = 272,
     THIS = 273,
     DO = 274,
     WHILE = 275,
     ELSE = 276,
     QS_IN = 277,
     INSTANCEOF = 278,
     TYPEOF = 279,
     IS = 280,
     SWITCH = 281,
     WITH = 282,
     RESERVED = 283,
     THROW = 284,
     TRY = 285,
     CATCH = 286,
     FINALLY = 287,
     CLASS = 288,
     CONSTRUCTOR = 289,
     EXTENDS = 290,
     ABSTRACT = 291,
     FINAL = 292,
     PRIVATE = 293,
     PUBLIC = 294,
     STATIC = 295,
     QS_CONST = 296,
     PACKAGE = 297,
     IMPORT = 298,
     EQEQ = 299,
     NE = 300,
     STREQ = 301,
     STRNEQ = 302,
     LE = 303,
     GE = 304,
     OR = 305,
     AND = 306,
     PLUSPLUS = 307,
     MINUSMINUS = 308,
     LSHIFT = 309,
     RSHIFT = 310,
     URSHIFT = 311,
     PLUSEQUAL = 312,
     MINUSEQUAL = 313,
     MULTEQUAL = 314,
     DIVEQUAL = 315,
     LSHIFTEQUAL = 316,
     RSHIFTEQUAL = 317,
     URSHIFTEQUAL = 318,
     ANDEQUAL = 319,
     MODEQUAL = 320,
     XOREQUAL = 321,
     OREQUAL = 322,
     NUMBER = 323,
     STRING = 324,
     IDENT = 325
   };
#endif
/* Tokens.  */
#define NULLTOKEN 258
#define TRUETOKEN 259
#define FALSETOKEN 260
#define BREAK 261
#define CASE 262
#define DEFAULT 263
#define FOR 264
#define NEW 265
#define VAR 266
#define CONTINUE 267
#define FUNCTION 268
#define RETURN 269
#define QS_VOID 270
#define QS_DELETE 271
#define IF 272
#define THIS 273
#define DO 274
#define WHILE 275
#define ELSE 276
#define QS_IN 277
#define INSTANCEOF 278
#define TYPEOF 279
#define IS 280
#define SWITCH 281
#define WITH 282
#define RESERVED 283
#define THROW 284
#define TRY 285
#define CATCH 286
#define FINALLY 287
#define CLASS 288
#define CONSTRUCTOR 289
#define EXTENDS 290
#define ABSTRACT 291
#define FINAL 292
#define PRIVATE 293
#define PUBLIC 294
#define STATIC 295
#define QS_CONST 296
#define PACKAGE 297
#define IMPORT 298
#define EQEQ 299
#define NE 300
#define STREQ 301
#define STRNEQ 302
#define LE 303
#define GE 304
#define OR 305
#define AND 306
#define PLUSPLUS 307
#define MINUSMINUS 308
#define LSHIFT 309
#define RSHIFT 310
#define URSHIFT 311
#define PLUSEQUAL 312
#define MINUSEQUAL 313
#define MULTEQUAL 314
#define DIVEQUAL 315
#define LSHIFTEQUAL 316
#define RSHIFTEQUAL 317
#define URSHIFTEQUAL 318
#define ANDEQUAL 319
#define MODEQUAL 320
#define XOREQUAL 321
#define OREQUAL 322
#define NUMBER 323
#define STRING 324
#define IDENT 325




/* Copy the first part of user declarations.  */
#line 1 "grammar.y"

/****************************************************************************
** $Id$
**
** Copyright (C) 2001-2003 Trolltech AS.  All rights reserved.
**
** This file is part of the Qt Script for Applications framework (QSA).
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding a valid Qt Script for Applications license may use
** this file in accordance with the Qt Script for Applications License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.trolltech.com/pricing.html or email sales@trolltech.com for
**   information about QSA Commercial License Agreements.
** See http://www.trolltech.com/gpl/ for GPL licensing information.
**
** Contact info@trolltech.com if any conditions of this licensing are
** not clear to you.
**
*****************************************************************************/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <string.h>
#include "qsengine.h"
#include "qsnodes.h"
#include "qslexer.h"

/* default values for bison */
#define YYDEBUG 0
//#define YYMAXDEPTH 0
#ifdef QSDEBUGGER
#define YYERROR_VERBOSE
#define DBG(l, s, e) { l->setLoc(s.first_line, e.last_line); } // location
#else
#undef YYLSP_NEEDED
#define DBG(l, s, e)
#endif

extern int yylex();
static int yyerror (const char *);
static bool automatic();



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 55 "grammar.y"
{
  int                 ival;
  double              dval;
  QString             *ustr;
  QSNode                *node;
  QSStatementNode       *stat;
  QSParameterNode       *param;
  QSFunctionBodyNode    *body;
  QSFuncDeclNode        *func;
  QSClassDefNode        *clss;
  QSProgramNode         *prog;
  QSAssignExprNode      *init;
  QSSourceElementNode   *src;
  QSSourceElementsNode  *srcs;
  QSStatListNode        *slist;
  QSArgumentsNode       *args;
  QSArgumentListNode    *alist;
  QSCaseBlockNode       *cblk;
  QSClauseListNode      *clist;
  QSCaseClauseNode      *ccl;
  QSElementNode         *elm;
  QSElisionNode         *eli;
  QSTypeNode            *type;
  QSTypedVarNode        *tvar;
  QSVarBindingNode      *vbin;
  QSVarBindingListNode  *blist;
  QSAttributeNode       *attrs;
  QSAttribute            attr;
  Operator            op;
}
/* Line 187 of yacc.c.  */
#line 329 "grammar.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 354 "grammar.tab.c"

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
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
    YYLTYPE yyls;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  200
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1305

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  95
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  74
/* YYNRULES -- Number of rules.  */
#define YYNRULES  224
/* YYNRULES -- Number of states.  */
#define YYNSTATES  407

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   325

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    84,     2,     2,     2,    86,    89,     2,
      72,    73,    85,    81,    78,    82,    80,    71,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    79,    94,
      87,    93,    88,    92,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    76,     2,    77,    90,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    74,    91,    75,    83,     2,     2,     2,
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
      65,    66,    67,    68,    69,    70
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     7,     9,    11,    13,    15,    17,
      19,    21,    23,    27,    30,    34,    38,    42,    48,    51,
      56,    57,    59,    61,    64,    68,    74,    76,    78,    80,
      82,    84,    89,    93,    97,    99,   102,   105,   108,   113,
     117,   120,   124,   126,   130,   132,   134,   136,   139,   142,
     144,   147,   150,   153,   156,   159,   162,   165,   168,   171,
     173,   177,   181,   185,   187,   191,   195,   197,   201,   205,
     209,   211,   215,   219,   223,   227,   231,   235,   239,   241,
     245,   249,   253,   257,   259,   263,   265,   269,   271,   275,
     277,   281,   283,   287,   289,   295,   297,   301,   303,   305,
     307,   309,   311,   313,   315,   317,   319,   321,   323,   325,
     327,   331,   333,   335,   337,   340,   342,   344,   346,   348,
     350,   352,   354,   356,   358,   360,   362,   364,   366,   368,
     371,   375,   377,   380,   382,   385,   388,   394,   402,   409,
     415,   425,   436,   444,   453,   454,   456,   459,   462,   466,
     470,   473,   476,   480,   484,   487,   490,   494,   498,   504,
     510,   514,   520,   521,   523,   525,   528,   532,   537,   540,
     544,   548,   552,   556,   560,   565,   571,   574,   578,   582,
     584,   588,   595,   603,   611,   620,   621,   624,   629,   635,
     637,   641,   645,   651,   654,   658,   662,   666,   672,   673,
     675,   677,   680,   682,   684,   686,   689,   691,   693,   695,
     697,   699,   701,   703,   705,   708,   712,   714,   716,   718,
     722,   724,   728,   730,   734
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     158,     0,    -1,     3,    -1,     4,    -1,     5,    -1,    68,
      -1,    69,    -1,    71,    -1,    18,    -1,    70,    -1,    96,
      -1,    98,    -1,    72,   125,    73,    -1,    74,    75,    -1,
      74,   102,    75,    -1,    76,   100,    77,    -1,    76,    99,
      77,    -1,    76,    99,    78,   100,    77,    -1,   100,   123,
      -1,    99,    78,   100,   123,    -1,    -1,   101,    -1,    78,
      -1,   101,    78,    -1,   103,    79,   123,    -1,   102,    78,
     103,    79,   123,    -1,    70,    -1,    69,    -1,    68,    -1,
      97,    -1,   154,    -1,   104,    76,   125,    77,    -1,   104,
      80,    70,    -1,    10,   104,   107,    -1,   104,    -1,    10,
     105,    -1,   104,   107,    -1,   106,   107,    -1,   106,    76,
     125,    77,    -1,   106,    80,    70,    -1,    72,    73,    -1,
      72,   108,    73,    -1,   123,    -1,   108,    78,   123,    -1,
     105,    -1,   106,    -1,   109,    -1,   109,    52,    -1,   109,
      53,    -1,   110,    -1,    16,   111,    -1,    15,   111,    -1,
      24,   111,    -1,    52,   111,    -1,    53,   111,    -1,    81,
     111,    -1,    82,   111,    -1,    83,   111,    -1,    84,   111,
      -1,   111,    -1,   112,    85,   111,    -1,   112,    71,   111,
      -1,   112,    86,   111,    -1,   112,    -1,   113,    81,   112,
      -1,   113,    82,   112,    -1,   113,    -1,   114,    54,   113,
      -1,   114,    55,   113,    -1,   114,    56,   113,    -1,   114,
      -1,   115,    87,   114,    -1,   115,    88,   114,    -1,   115,
      48,   114,    -1,   115,    49,   114,    -1,   115,    25,   114,
      -1,   115,    23,   114,    -1,   115,    22,   114,    -1,   115,
      -1,   116,    44,   115,    -1,   116,    45,   115,    -1,   116,
      46,   115,    -1,   116,    47,   115,    -1,   116,    -1,   117,
      89,   116,    -1,   117,    -1,   118,    90,   117,    -1,   118,
      -1,   119,    91,   118,    -1,   119,    -1,   120,    51,   119,
      -1,   120,    -1,   121,    50,   120,    -1,   121,    -1,   121,
      92,   123,    79,   123,    -1,   122,    -1,   109,   124,   123,
      -1,    93,    -1,    57,    -1,    58,    -1,    59,    -1,    60,
      -1,    61,    -1,    62,    -1,    63,    -1,    64,    -1,    66,
      -1,    67,    -1,    65,    -1,   123,    -1,   125,    78,   123,
      -1,   127,    -1,   163,    -1,   157,    -1,   161,   157,    -1,
     149,    -1,   129,    -1,   130,    -1,   131,    -1,   132,    -1,
     134,    -1,   135,    -1,   136,    -1,   137,    -1,   138,    -1,
     144,    -1,   145,    -1,   146,    -1,   150,    -1,    74,    75,
      -1,    74,   128,    75,    -1,   126,    -1,   128,   126,    -1,
      94,    -1,   125,    94,    -1,   125,     1,    -1,    17,    72,
     125,    73,   126,    -1,    17,    72,   125,    73,   126,    21,
     126,    -1,    19,   126,    20,    72,   125,    73,    -1,    20,
      72,   125,    73,   126,    -1,     9,    72,   133,    94,   133,
      94,   133,    73,   126,    -1,     9,    72,    11,   165,    94,
     133,    94,   133,    73,   126,    -1,     9,    72,   109,    22,
     125,    73,   126,    -1,     9,    72,    11,   166,    22,   125,
      73,   126,    -1,    -1,   125,    -1,    12,    94,    -1,    12,
       1,    -1,    12,    70,    94,    -1,    12,    70,     1,    -1,
       6,    94,    -1,     6,     1,    -1,     6,    70,    94,    -1,
       6,    70,     1,    -1,    14,    94,    -1,    14,     1,    -1,
      14,   125,    94,    -1,    14,   125,     1,    -1,    27,    72,
     125,    73,   126,    -1,    26,    72,   125,    73,   139,    -1,
      74,   140,    75,    -1,    74,   140,   143,   140,    75,    -1,
      -1,   141,    -1,   142,    -1,   141,   142,    -1,     7,   125,
      79,    -1,     7,   125,    79,   128,    -1,     8,    79,    -1,
       8,    79,   128,    -1,    70,    79,   126,    -1,    29,   125,
      94,    -1,    30,   127,   147,    -1,    30,   127,   148,    -1,
      30,   127,   147,   148,    -1,    31,    72,    70,    73,   127,
      -1,    32,   127,    -1,    42,   151,   156,    -1,    43,   151,
      94,    -1,    70,    -1,   151,    80,    70,    -1,    13,    70,
      72,    73,   153,   156,    -1,    13,    70,    72,   155,    73,
     153,   156,    -1,   161,    13,    70,    72,    73,   153,   156,
      -1,   161,    13,    70,    72,   155,    73,   153,   156,    -1,
      -1,    79,   168,    -1,    13,    72,    73,   156,    -1,    13,
      72,   155,    73,   156,    -1,    70,    -1,    70,    79,   168,
      -1,   155,    78,    70,    -1,   155,    78,    70,    79,   168,
      -1,    74,    75,    -1,    74,   159,    75,    -1,    33,    70,
      94,    -1,    33,    70,   156,    -1,    33,    70,    35,   168,
     156,    -1,    -1,   159,    -1,   160,    -1,   159,   160,    -1,
     126,    -1,   152,    -1,   162,    -1,   161,   162,    -1,    36,
      -1,    37,    -1,    38,    -1,    39,    -1,    40,    -1,     4,
      -1,     5,    -1,    34,    -1,   164,   165,    -1,   161,   164,
     165,    -1,    11,    -1,    41,    -1,   166,    -1,   165,    78,
     166,    -1,   167,    -1,   167,    93,   123,    -1,    70,    -1,
      70,    79,   168,    -1,    70,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   175,   175,   176,   177,   178,   179,   180,   186,   187,
     189,   190,   191,   192,   193,   197,   198,   199,   203,   204,
     209,   210,   214,   215,   219,   220,   225,   227,   228,   232,
     233,   234,   235,   237,   241,   242,   246,   247,   248,   249,
     254,   255,   259,   260,   264,   265,   269,   270,   271,   275,
     276,   277,   278,   279,   280,   281,   282,   283,   284,   288,
     289,   290,   291,   295,   296,   297,   301,   302,   303,   304,
     308,   309,   311,   313,   315,   317,   319,   321,   326,   327,
     328,   329,   330,   334,   335,   339,   340,   344,   345,   349,
     350,   355,   356,   361,   362,   367,   368,   373,   374,   375,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   388,
     389,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   414,
     415,   419,   420,   424,   428,   430,   438,   439,   444,   445,
     446,   449,   452,   455,   461,   462,   466,   467,   471,   473,
     481,   482,   486,   488,   497,   498,   502,   503,   513,   518,
     523,   524,   529,   530,   534,   535,   539,   540,   544,   545,
     549,   555,   559,   560,   561,   565,   569,   573,   579,   583,
     584,   588,   590,   592,   595,   601,   602,   606,   607,   613,
     615,   617,   618,   623,   624,   628,   630,   632,   638,   639,
     643,   644,   648,   649,   653,   654,   658,   659,   660,   661,
     662,   663,   664,   665,   671,   673,   679,   680,   684,   685,
     690,   691,   695,   697,   704
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NULLTOKEN", "TRUETOKEN", "FALSETOKEN",
  "BREAK", "CASE", "DEFAULT", "FOR", "NEW", "VAR", "CONTINUE", "FUNCTION",
  "RETURN", "QS_VOID", "QS_DELETE", "IF", "THIS", "DO", "WHILE", "ELSE",
  "QS_IN", "INSTANCEOF", "TYPEOF", "IS", "SWITCH", "WITH", "RESERVED",
  "THROW", "TRY", "CATCH", "FINALLY", "CLASS", "CONSTRUCTOR", "EXTENDS",
  "ABSTRACT", "FINAL", "PRIVATE", "PUBLIC", "STATIC", "QS_CONST",
  "PACKAGE", "IMPORT", "EQEQ", "NE", "STREQ", "STRNEQ", "LE", "GE", "OR",
  "AND", "PLUSPLUS", "MINUSMINUS", "LSHIFT", "RSHIFT", "URSHIFT",
  "PLUSEQUAL", "MINUSEQUAL", "MULTEQUAL", "DIVEQUAL", "LSHIFTEQUAL",
  "RSHIFTEQUAL", "URSHIFTEQUAL", "ANDEQUAL", "MODEQUAL", "XOREQUAL",
  "OREQUAL", "NUMBER", "STRING", "IDENT", "'/'", "'('", "')'", "'{'",
  "'}'", "'['", "']'", "','", "':'", "'.'", "'+'", "'-'", "'~'", "'!'",
  "'*'", "'%'", "'<'", "'>'", "'&'", "'^'", "'|'", "'?'", "'='", "';'",
  "$accept", "Literal", "PrimaryExpr", "ArrayLiteral", "ElementList",
  "ElisionOpt", "Elision", "PropertyNameAndValueList", "PropertyName",
  "MemberExpr", "NewExpr", "CallExpr", "Arguments", "ArgumentList",
  "LeftHandSideExpr", "PostfixExpr", "UnaryExpr", "MultiplicativeExpr",
  "AdditiveExpr", "ShiftExpr", "RelationalExpr", "EqualityExpr",
  "BitwiseANDExpr", "BitwiseXORExpr", "BitwiseORExpr", "LogicalANDExpr",
  "LogicalORExpr", "ConditionalExpr", "AssignmentExpr",
  "AssignmentOperator", "Expr", "Statement", "Block", "StatementList",
  "EmptyStatement", "ExprStatement", "IfStatement", "IterationStatement",
  "ExprOpt", "ContinueStatement", "BreakStatement", "ReturnStatement",
  "WithStatement", "SwitchStatement", "CaseBlock", "CaseClausesOpt",
  "CaseClauses", "CaseClause", "DefaultClause", "LabelledStatement",
  "ThrowStatement", "TryStatement", "Catch", "Finally",
  "PackageDefinition", "ImportStatement", "PackageName",
  "FunctionDeclaration", "ResultSignature", "FunctionExpr",
  "FormalParameterList", "FunctionBody", "ClassDefinition", "Program",
  "SourceElements", "SourceElement", "Attributes", "Attribute",
  "VariableDefinition", "VariableDefinitionKind", "VariableBindingList",
  "VariableBinding", "TypedVariable", "TypeExpression", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,    47,    40,    41,   123,   125,    91,    93,    44,    58,
      46,    43,    45,   126,    33,    42,    37,    60,    62,    38,
      94,   124,    63,    61,    59
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    95,    96,    96,    96,    96,    96,    96,    97,    97,
      97,    97,    97,    97,    97,    98,    98,    98,    99,    99,
     100,   100,   101,   101,   102,   102,   103,   103,   103,   104,
     104,   104,   104,   104,   105,   105,   106,   106,   106,   106,
     107,   107,   108,   108,   109,   109,   110,   110,   110,   111,
     111,   111,   111,   111,   111,   111,   111,   111,   111,   112,
     112,   112,   112,   113,   113,   113,   114,   114,   114,   114,
     115,   115,   115,   115,   115,   115,   115,   115,   116,   116,
     116,   116,   116,   117,   117,   118,   118,   119,   119,   120,
     120,   121,   121,   122,   122,   123,   123,   124,   124,   124,
     124,   124,   124,   124,   124,   124,   124,   124,   124,   125,
     125,   126,   126,   126,   126,   126,   126,   126,   126,   126,
     126,   126,   126,   126,   126,   126,   126,   126,   126,   127,
     127,   128,   128,   129,   130,   130,   131,   131,   132,   132,
     132,   132,   132,   132,   133,   133,   134,   134,   134,   134,
     135,   135,   135,   135,   136,   136,   136,   136,   137,   138,
     139,   139,   140,   140,   141,   141,   142,   142,   143,   143,
     144,   145,   146,   146,   146,   147,   148,   149,   150,   151,
     151,   152,   152,   152,   152,   153,   153,   154,   154,   155,
     155,   155,   155,   156,   156,   157,   157,   157,   158,   158,
     159,   159,   160,   160,   161,   161,   162,   162,   162,   162,
     162,   162,   162,   162,   163,   163,   164,   164,   165,   165,
     166,   166,   167,   167,   168
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     2,     3,     3,     3,     5,     2,     4,
       0,     1,     1,     2,     3,     5,     1,     1,     1,     1,
       1,     4,     3,     3,     1,     2,     2,     2,     4,     3,
       2,     3,     1,     3,     1,     1,     1,     2,     2,     1,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     1,
       3,     3,     3,     1,     3,     3,     1,     3,     3,     3,
       1,     3,     3,     3,     3,     3,     3,     3,     1,     3,
       3,     3,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     5,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     1,     1,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       3,     1,     2,     1,     2,     2,     5,     7,     6,     5,
       9,    10,     7,     8,     0,     1,     2,     2,     3,     3,
       2,     2,     3,     3,     2,     2,     3,     3,     5,     5,
       3,     5,     0,     1,     1,     2,     3,     4,     2,     3,
       3,     3,     3,     3,     4,     5,     2,     3,     3,     1,
       3,     6,     7,     7,     8,     0,     2,     4,     5,     1,
       3,     3,     5,     2,     3,     3,     3,     5,     0,     1,
       1,     2,     1,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     3,     1,     1,     1,     3,
       1,     3,     1,     3,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
     198,     2,     3,     4,     0,     0,     0,   216,     0,     0,
       0,     0,     0,     0,     8,     0,     0,     0,     0,     0,
       0,     0,     0,   213,   206,   207,   208,   209,   210,   217,
       0,     0,     0,     0,     5,     6,     9,     7,     0,     0,
      20,     0,     0,     0,     0,   133,    10,    29,    11,    34,
      44,    45,    46,    49,    59,    63,    66,    70,    78,    83,
      85,    87,    89,    91,    93,    95,   109,     0,   202,   111,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   115,   128,   203,    30,   113,     0,   199,   200,
       0,   204,   112,     0,   151,     0,   150,   144,     3,     4,
       0,     9,     0,    34,    35,   147,     0,   146,     0,     0,
     155,   154,     0,    46,    51,    50,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,   179,     0,     0,
      53,    54,     0,     0,     5,     6,     9,    13,     0,     0,
     131,     0,    22,     0,     0,    21,    55,    56,    57,    58,
       0,     0,     0,    36,     0,     0,    37,    47,    48,    98,
      99,   100,   101,   102,   103,   104,   105,   108,   106,   107,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   135,     0,   134,
       1,   201,   211,   212,     0,   114,   205,     0,   222,   214,
     218,   220,   153,   152,     0,    46,   145,     0,    28,    27,
      26,    13,    33,   149,   148,     0,   189,     0,     0,   157,
     156,     0,     0,     0,     0,     0,   171,   129,     0,     0,
     172,   173,     0,     0,   195,   196,     0,   177,   178,   170,
      12,    14,     0,     0,   130,   132,    16,    20,    15,    18,
      23,    40,     0,    42,     0,    32,     0,    39,    96,    61,
      60,    62,    64,    65,    67,    68,    69,    77,    76,    75,
      73,    74,    71,    72,    79,    80,    81,    82,    84,    86,
      88,    90,    92,     0,   110,     0,   215,     0,     0,     0,
       0,   218,     0,   144,   185,     0,     0,   187,     0,     0,
       0,     0,     0,     0,     0,     0,   176,   174,   224,     0,
     193,     0,   180,     0,    24,     0,    41,     0,    31,    38,
       0,     0,   223,   219,   221,   144,     0,     0,     0,     0,
       0,   185,   190,   188,   191,   136,     0,   139,   162,   159,
     158,     0,   197,   194,     0,    17,    19,    43,    94,   185,
       0,     0,     0,     0,   144,   186,   181,     0,     0,     0,
     138,     0,     0,   163,   164,     0,    25,     0,   185,   144,
       0,   142,     0,   182,   192,   137,     0,     0,   160,   162,
     165,   175,   183,     0,     0,   143,     0,   166,   168,     0,
     184,     0,   140,   167,   169,   161,   141
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    46,    47,    48,   143,   144,   145,   138,   139,    49,
      50,    51,   153,   262,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,   171,
      67,    68,    69,   141,    70,    71,    72,    73,   217,    74,
      75,    76,    77,    78,   349,   372,   373,   374,   389,    79,
      80,    81,   240,   241,    82,    83,   128,    84,   340,    85,
     228,   245,    86,    87,    88,    89,   118,    91,    92,    93,
     209,   210,   211,   319
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -305
static const yytype_int16 yypact[] =
{
     841,  -305,  1204,  1216,     5,   -39,   304,  -305,    17,    58,
      53,  1142,  1142,     7,  -305,   927,    36,  1142,    68,   111,
    1142,   127,   115,  -305,  -305,  -305,  -305,  -305,  -305,  -305,
     152,   152,  1142,  1142,  -305,  -305,   136,  -305,  1142,   411,
     147,  1142,  1142,  1142,  1142,  -305,  -305,  -305,  -305,    78,
    -305,    81,   924,  -305,  -305,   -33,     4,   151,   126,   187,
     163,   178,   183,   226,   -22,  -305,  -305,    24,  -305,  -305,
    -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,
    -305,  -305,  -305,  -305,  -305,  -305,  -305,   279,   841,  -305,
    1254,  -305,  -305,   215,  -305,    21,  -305,   128,  -305,  -305,
     214,  -305,    14,    78,  -305,  -305,    25,  -305,   216,   -25,
    -305,  -305,    26,   175,  -305,  -305,  1142,   267,  1264,  1142,
    -305,  1142,  1142,   -43,   497,   205,   -14,  -305,    -7,    -4,
    -305,  -305,   927,    19,   210,   211,   136,  1019,    23,   212,
    -305,   583,  -305,   167,   260,   220,  -305,  -305,  -305,  -305,
    1060,  1142,   222,  -305,  1142,   230,  -305,  -305,  -305,  -305,
    -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,
    -305,  1142,  1142,  1142,  1142,  1142,  1142,  1142,  1142,  1142,
    1142,  1142,  1142,  1142,  1142,  1142,  1142,  1142,  1142,  1142,
    1142,  1142,  1142,  1142,  1142,  1142,  1142,  -305,  1142,  -305,
    -305,  -305,  -305,  -305,   231,  -305,  -305,   215,   217,   225,
    -305,   213,  -305,  -305,   215,  1139,   227,   224,  -305,  -305,
    -305,  -305,  -305,  -305,  -305,    86,   232,   236,    22,  -305,
    -305,    82,   244,   100,   104,   113,  -305,  -305,   247,   127,
     272,  -305,   250,   669,  -305,  -305,   251,  -305,  -305,  -305,
    -305,  -305,   179,  1142,  -305,  -305,  -305,   147,  -305,  -305,
    -305,  -305,   114,  -305,   180,  -305,   189,  -305,  -305,  -305,
    -305,  -305,   -33,   -33,     4,     4,     4,   151,   151,   151,
     151,   151,   151,   151,   126,   126,   126,   126,   187,   163,
     178,   183,   226,   246,  -305,   261,   225,   250,   215,  1142,
     -32,   301,  1142,  1142,   256,   116,   250,  -305,   236,   268,
     927,  1142,   927,   265,   927,   270,  -305,  -305,  -305,   236,
    -305,   755,  -305,   266,  -305,  1101,  -305,  1142,  -305,  -305,
    1142,   106,  -305,  -305,  -305,  1142,  1142,   130,   252,   250,
     236,   256,  -305,  -305,   271,   326,   145,  -305,   342,  -305,
    -305,   278,  -305,  -305,  1142,  -305,  -305,  -305,  -305,   256,
     146,   258,   148,   927,  1142,  -305,  -305,   236,   250,   927,
    -305,  1142,     6,   342,  -305,   127,  -305,   236,   256,  1142,
     927,  -305,   280,  -305,  -305,  -305,   193,   281,  -305,   342,
    -305,  -305,  -305,   236,   284,  -305,   927,   927,   927,   283,
    -305,   927,  -305,   927,   927,  -305,  -305
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -305,  -305,  -305,  -305,  -305,   102,  -305,  -305,   109,   357,
     358,  -305,   -31,  -305,   218,  -305,    -2,   105,    77,   -17,
      52,   176,   174,   184,   177,   173,  -305,  -305,  -137,  -305,
      -9,   -15,   -19,  -115,  -305,  -305,  -305,  -305,  -286,  -305,
    -305,  -305,  -305,  -305,  -305,   -20,  -305,    11,  -305,  -305,
    -305,  -305,  -305,   139,  -305,  -305,   354,  -305,  -304,  -305,
    -217,  -124,   -74,  -305,   144,   -83,     3,   -71,  -305,   -54,
    -164,  -202,  -305,  -274
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -213
static const yytype_int16 yytable[] =
{
     117,   112,   125,    90,   247,   201,    94,   259,   305,   114,
     115,   123,   301,   263,   387,   120,   205,   338,   105,   206,
     156,   242,   212,   332,   140,   197,   223,   229,   195,   133,
     130,   131,   342,    97,   268,   198,   207,   367,   172,   146,
     147,   148,   149,   296,   205,   226,   298,   206,   227,   361,
     300,   236,   173,   174,   110,   377,     1,    98,    99,   293,
     243,   294,   335,     6,   207,   365,   100,   243,    11,    12,
     196,    14,   222,   246,   393,    95,   246,    17,   382,   116,
     244,   388,   218,   219,   220,   175,   176,   106,   216,   221,
     248,    90,   250,   394,   384,   308,   333,   198,   251,    96,
     309,   252,   198,   307,   198,    32,    33,   231,   119,   140,
     233,   107,   234,   235,   360,   213,   324,   249,   199,   224,
     230,    34,    35,   101,    37,    38,   255,   102,   108,    40,
     109,     1,    98,    99,    41,    42,    43,    44,     6,   214,
     121,   100,   264,    11,    12,   266,    14,   111,   180,   181,
     150,   182,    17,   150,   151,   310,   226,   154,   152,   304,
     198,   155,   334,   277,   278,   279,   280,   281,   282,   283,
     269,   270,   271,   312,   183,   184,   226,   313,   198,   359,
      32,    33,   198,   122,   343,   126,   314,   326,   356,   341,
     357,   198,   327,   358,   309,   352,    34,    35,   101,    37,
      38,   124,   102,   363,    40,   177,   178,   179,   198,    41,
      42,    43,    44,   185,   186,   132,   366,   376,   370,   378,
     316,   380,   127,   198,   309,   142,   198,   157,   158,   113,
     113,   187,   188,   189,   190,   113,   238,   239,   201,   284,
     285,   286,   287,   383,   256,   257,    90,   218,   219,   220,
     113,   113,   191,   392,   274,   275,   276,   328,   198,   113,
     113,   113,   113,     1,    98,    99,   329,   198,   192,   400,
       6,   198,   397,   100,   193,    11,    12,   194,    14,   200,
     272,   273,   403,   404,    17,   208,   109,   232,   225,   -28,
     -27,   253,   265,   337,   216,   345,   297,   347,   260,   350,
     267,   295,   346,   298,   239,   198,   299,     1,    98,    99,
     243,   306,    32,    33,     6,   215,   311,   100,   303,   315,
     318,   322,    14,   336,    90,   330,   216,   362,    34,    35,
     101,    37,    38,   331,   102,   339,    40,   258,   344,   348,
     351,    41,    42,    43,    44,   354,   364,   369,   381,   371,
     368,   375,   379,   396,   385,   216,   391,   401,   405,   325,
     398,   323,   386,   103,   104,   395,   289,   288,   292,   399,
     216,   291,    34,    35,   101,    37,    38,   290,   102,   317,
      40,   402,   140,   140,   390,   129,   406,   321,   255,   255,
     113,   113,   113,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   113,   113,     1,     2,     3,     4,     0,     0,
       5,     6,     7,     8,   100,    10,    11,    12,    13,    14,
      15,    16,     0,     0,     0,    17,     0,    18,    19,     0,
      20,    21,     0,     0,    22,    23,     0,    24,    25,    26,
      27,    28,    29,    30,    31,     0,     0,     0,     0,     0,
       0,     0,     0,    32,    33,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   134,
     135,   136,    37,    38,     0,    39,   137,    40,     0,     0,
       0,     0,    41,    42,    43,    44,     0,     0,     0,     0,
       1,     2,     3,     4,     0,    45,     5,     6,     7,     8,
     100,    10,    11,    12,    13,    14,    15,    16,     0,     0,
       0,    17,     0,    18,    19,     0,    20,    21,     0,     0,
      22,    23,     0,    24,    25,    26,    27,    28,    29,    30,
      31,     0,     0,     0,     0,     0,     0,     0,     0,    32,
      33,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    34,    35,    36,    37,    38,
       0,    39,   237,    40,     0,     0,     0,     0,    41,    42,
      43,    44,     0,     0,     0,     0,     1,     2,     3,     4,
       0,    45,     5,     6,     7,     8,   100,    10,    11,    12,
      13,    14,    15,    16,     0,     0,     0,    17,     0,    18,
      19,     0,    20,    21,     0,     0,    22,    23,     0,    24,
      25,    26,    27,    28,    29,    30,    31,     0,     0,     0,
       0,     0,     0,     0,     0,    32,    33,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    34,    35,    36,    37,    38,     0,    39,   254,    40,
       0,     0,     0,     0,    41,    42,    43,    44,     0,     0,
       0,     0,     1,     2,     3,     4,     0,    45,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
       0,     0,     0,    17,     0,    18,    19,     0,    20,    21,
       0,     0,    22,    23,     0,    24,    25,    26,    27,    28,
      29,    30,    31,     0,     0,     0,     0,     0,     0,     0,
       0,    32,    33,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    34,    35,    36,
      37,    38,     0,    39,   320,    40,     0,     0,     0,     0,
      41,    42,    43,    44,     0,     0,     0,     0,     1,     2,
       3,     4,     0,    45,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,     0,     0,     0,    17,
       0,    18,    19,     0,    20,    21,     0,     0,    22,    23,
       0,    24,    25,    26,    27,    28,    29,    30,    31,     0,
       0,     0,     0,     0,     0,     0,     0,    32,    33,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    34,    35,    36,    37,    38,     0,    39,
     353,    40,     0,     0,     0,     0,    41,    42,    43,    44,
       0,     0,     0,     0,     1,     2,     3,     4,     0,    45,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,     0,     0,     0,    17,     0,    18,    19,     0,
      20,    21,     0,     0,    22,    23,     0,    24,    25,    26,
      27,    28,    29,    30,    31,     0,     0,     0,     0,     0,
       0,     0,     0,    32,    33,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    34,
      35,    36,    37,    38,     0,    39,     0,    40,     0,     0,
       0,     0,    41,    42,    43,    44,     0,     0,     0,     0,
       1,     2,     3,     4,     0,    45,     5,     6,     7,     8,
     100,    10,    11,    12,    13,    14,    15,    16,     0,     0,
       0,    17,     0,    18,    19,     0,    20,    21,     0,     0,
      22,    23,     0,    24,    25,    26,    27,    28,    29,    30,
      31,     0,     0,     0,     0,     0,   157,   158,     0,    32,
      33,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,     0,     0,     0,    34,    35,    36,    37,    38,
       0,    39,     0,    40,     0,     0,     0,     0,    41,    42,
      43,    44,     0,     0,     0,     0,     0,   170,     0,  -129,
       0,    45,  -129,  -129,  -129,  -129,  -129,  -129,  -129,  -129,
    -129,  -129,  -129,  -129,  -129,  -129,  -129,  -129,  -129,  -129,
    -129,     0,     0,  -129,     0,  -129,  -129,     0,  -129,  -129,
       0,     0,  -129,  -129,     0,  -129,  -129,  -129,  -129,  -129,
    -129,  -129,  -129,     1,    98,    99,     0,     0,     0,     0,
       6,     0,     0,   100,     0,    11,    12,     0,    14,     0,
       0,     0,     0,     0,    17,     0,     0,  -129,  -129,  -129,
       0,     0,     0,  -129,  -129,     0,     0,     0,     0,     0,
       0,     0,  -129,  -129,     1,    98,    99,     0,     0,     0,
       0,     6,    32,    33,   100,     0,    11,    12,     0,    14,
       0,     0,     0,     0,     0,    17,     0,     0,    34,    35,
     101,    37,    38,   261,   102,     0,    40,     0,     0,     0,
       0,    41,    42,    43,    44,     1,    98,    99,     0,     0,
       0,     0,     6,    32,    33,   100,     0,    11,    12,     0,
      14,   302,     0,     0,     0,     0,    17,     0,     0,    34,
      35,   101,    37,    38,     0,   102,     0,    40,   355,     0,
       0,     0,    41,    42,    43,    44,     0,     0,     0,     0,
       0,   157,   158,     0,    32,    33,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,     0,  -211,  -211,
      34,    35,   101,    37,    38,  -211,   102,  -211,    40,     0,
    -212,  -212,     0,    41,    42,    43,    44,  -212,     0,  -212,
       0,     0,   170,     0,     0,     0,     0,  -211,  -211,     0,
    -211,  -211,  -211,  -211,  -211,  -211,     0,     0,     0,  -212,
    -212,     0,  -212,  -212,  -212,  -212,  -212,  -212,   202,   203,
       0,     0,     0,     0,     0,     7,     0,   204,   202,   203,
       0,     0,     0,     0,     0,     7,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    22,    23,     0,
      24,    25,    26,    27,    28,    29,     0,    22,    23,     0,
      24,    25,    26,    27,    28,    29
};

static const yytype_int16 yycheck[] =
{
      15,    10,    21,     0,   128,    88,     1,   144,   225,    11,
      12,    20,   214,   150,     8,    17,    90,   303,     1,    90,
      51,    35,     1,   297,    39,     1,     1,     1,    50,    38,
      32,    33,   306,    72,   171,    78,    90,   341,    71,    41,
      42,    43,    44,   207,   118,    70,    78,   118,    73,   335,
     214,    94,    85,    86,     1,   359,     3,     4,     5,   196,
      74,   198,    94,    10,   118,   339,    13,    74,    15,    16,
      92,    18,   103,    80,   378,    70,    80,    24,   364,    72,
      94,    75,    68,    69,    70,    81,    82,    70,    97,    75,
      94,    88,    73,   379,   368,    73,   298,    78,    75,    94,
      78,    78,    78,   227,    78,    52,    53,   116,    72,   124,
     119,    94,   121,   122,   331,    94,   253,   132,    94,    94,
      94,    68,    69,    70,    71,    72,   141,    74,    70,    76,
      72,     3,     4,     5,    81,    82,    83,    84,    10,    11,
      72,    13,   151,    15,    16,   154,    18,    94,    22,    23,
      72,    25,    24,    72,    76,    73,    70,    76,    80,    73,
      78,    80,   299,   180,   181,   182,   183,   184,   185,   186,
     172,   173,   174,    73,    48,    49,    70,    73,    78,    73,
      52,    53,    78,    72,   308,    70,    73,    73,   325,    73,
     327,    78,    78,   330,    78,   319,    68,    69,    70,    71,
      72,    74,    74,    73,    76,    54,    55,    56,    78,    81,
      82,    83,    84,    87,    88,    79,   340,   354,    73,    73,
     239,    73,    70,    78,    78,    78,    78,    52,    53,    11,
      12,    44,    45,    46,    47,    17,    31,    32,   321,   187,
     188,   189,   190,   367,    77,    78,   243,    68,    69,    70,
      32,    33,    89,   377,   177,   178,   179,    77,    78,    41,
      42,    43,    44,     3,     4,     5,    77,    78,    90,   393,
      10,    78,    79,    13,    91,    15,    16,    51,    18,     0,
     175,   176,   397,   398,    24,    70,    72,    20,    72,    79,
      79,    79,    70,   302,   303,   310,    79,   312,    78,   314,
      70,    70,   311,    78,    32,    78,    93,     3,     4,     5,
      74,    79,    52,    53,    10,    97,    72,    13,    94,    72,
      70,    70,    18,    22,   321,    79,   335,   336,    68,    69,
      70,    71,    72,    72,    74,    79,    76,    77,    70,    74,
      70,    81,    82,    83,    84,    79,    94,    21,   363,     7,
      79,    73,    94,    73,   369,   364,   375,    73,    75,   257,
      79,   252,   371,     6,     6,   380,   192,   191,   195,   389,
     379,   194,    68,    69,    70,    71,    72,   193,    74,   240,
      76,   396,   397,   398,   373,    31,   401,   243,   403,   404,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,     3,     4,     5,     6,    -1,    -1,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    24,    -1,    26,    27,    -1,
      29,    30,    -1,    -1,    33,    34,    -1,    36,    37,    38,
      39,    40,    41,    42,    43,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    52,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,
      69,    70,    71,    72,    -1,    74,    75,    76,    -1,    -1,
      -1,    -1,    81,    82,    83,    84,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    94,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    24,    -1,    26,    27,    -1,    29,    30,    -1,    -1,
      33,    34,    -1,    36,    37,    38,    39,    40,    41,    42,
      43,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    68,    69,    70,    71,    72,
      -1,    74,    75,    76,    -1,    -1,    -1,    -1,    81,    82,
      83,    84,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    94,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    24,    -1,    26,
      27,    -1,    29,    30,    -1,    -1,    33,    34,    -1,    36,
      37,    38,    39,    40,    41,    42,    43,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    52,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    68,    69,    70,    71,    72,    -1,    74,    75,    76,
      -1,    -1,    -1,    -1,    81,    82,    83,    84,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    94,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    24,    -1,    26,    27,    -1,    29,    30,
      -1,    -1,    33,    34,    -1,    36,    37,    38,    39,    40,
      41,    42,    43,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    52,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,    69,    70,
      71,    72,    -1,    74,    75,    76,    -1,    -1,    -1,    -1,
      81,    82,    83,    84,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    94,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    24,
      -1,    26,    27,    -1,    29,    30,    -1,    -1,    33,    34,
      -1,    36,    37,    38,    39,    40,    41,    42,    43,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    68,    69,    70,    71,    72,    -1,    74,
      75,    76,    -1,    -1,    -1,    -1,    81,    82,    83,    84,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    94,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    24,    -1,    26,    27,    -1,
      29,    30,    -1,    -1,    33,    34,    -1,    36,    37,    38,
      39,    40,    41,    42,    43,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    52,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,
      69,    70,    71,    72,    -1,    74,    -1,    76,    -1,    -1,
      -1,    -1,    81,    82,    83,    84,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    94,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    24,    -1,    26,    27,    -1,    29,    30,    -1,    -1,
      33,    34,    -1,    36,    37,    38,    39,    40,    41,    42,
      43,    -1,    -1,    -1,    -1,    -1,    52,    53,    -1,    52,
      53,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    -1,    -1,    -1,    68,    69,    70,    71,    72,
      -1,    74,    -1,    76,    -1,    -1,    -1,    -1,    81,    82,
      83,    84,    -1,    -1,    -1,    -1,    -1,    93,    -1,     0,
      -1,    94,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    -1,    24,    -1,    26,    27,    -1,    29,    30,
      -1,    -1,    33,    34,    -1,    36,    37,    38,    39,    40,
      41,    42,    43,     3,     4,     5,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    13,    -1,    15,    16,    -1,    18,    -1,
      -1,    -1,    -1,    -1,    24,    -1,    -1,    68,    69,    70,
      -1,    -1,    -1,    74,    75,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    84,     3,     4,     5,    -1,    -1,    -1,
      -1,    10,    52,    53,    13,    -1,    15,    16,    -1,    18,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    68,    69,
      70,    71,    72,    73,    74,    -1,    76,    -1,    -1,    -1,
      -1,    81,    82,    83,    84,     3,     4,     5,    -1,    -1,
      -1,    -1,    10,    52,    53,    13,    -1,    15,    16,    -1,
      18,    22,    -1,    -1,    -1,    -1,    24,    -1,    -1,    68,
      69,    70,    71,    72,    -1,    74,    -1,    76,    77,    -1,
      -1,    -1,    81,    82,    83,    84,    -1,    -1,    -1,    -1,
      -1,    52,    53,    -1,    52,    53,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    -1,     4,     5,
      68,    69,    70,    71,    72,    11,    74,    13,    76,    -1,
       4,     5,    -1,    81,    82,    83,    84,    11,    -1,    13,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    37,    38,    39,    40,    41,    -1,    -1,    -1,    33,
      34,    -1,    36,    37,    38,    39,    40,    41,     4,     5,
      -1,    -1,    -1,    -1,    -1,    11,    -1,    13,     4,     5,
      -1,    -1,    -1,    -1,    -1,    11,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    37,    38,    39,    40,    41,    -1,    33,    34,    -1,
      36,    37,    38,    39,    40,    41
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    24,    26,    27,
      29,    30,    33,    34,    36,    37,    38,    39,    40,    41,
      42,    43,    52,    53,    68,    69,    70,    71,    72,    74,
      76,    81,    82,    83,    84,    94,    96,    97,    98,   104,
     105,   106,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   125,   126,   127,
     129,   130,   131,   132,   134,   135,   136,   137,   138,   144,
     145,   146,   149,   150,   152,   154,   157,   158,   159,   160,
     161,   162,   163,   164,     1,    70,    94,    72,     4,     5,
      13,    70,    74,   104,   105,     1,    70,    94,    70,    72,
       1,    94,   125,   109,   111,   111,    72,   126,   161,    72,
     111,    72,    72,   125,    74,   127,    70,    70,   151,   151,
     111,   111,    79,   125,    68,    69,    70,    75,   102,   103,
     126,   128,    78,    99,   100,   101,   111,   111,   111,   111,
      72,    76,    80,   107,    76,    80,   107,    52,    53,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      93,   124,    71,    85,    86,    81,    82,    54,    55,    56,
      22,    23,    25,    48,    49,    87,    88,    44,    45,    46,
      47,    89,    90,    91,    51,    50,    92,     1,    78,    94,
       0,   160,     4,     5,    13,   157,   162,   164,    70,   165,
     166,   167,     1,    94,    11,   109,   125,   133,    68,    69,
      70,    75,   107,     1,    94,    72,    70,    73,   155,     1,
      94,   125,    20,   125,   125,   125,    94,    75,    31,    32,
     147,   148,    35,    74,    94,   156,    80,   156,    94,   126,
      73,    75,    78,    79,    75,   126,    77,    78,    77,   123,
      78,    73,   108,   123,   125,    70,   125,    70,   123,   111,
     111,   111,   112,   112,   113,   113,   113,   114,   114,   114,
     114,   114,   114,   114,   115,   115,   115,   115,   116,   117,
     118,   119,   120,   123,   123,    70,   165,    79,    78,    93,
     165,   166,    22,    94,    73,   155,    79,   156,    73,    78,
      73,    72,    73,    73,    73,    72,   127,   148,    70,   168,
      75,   159,    70,   103,   123,   100,    73,    78,    77,    77,
      79,    72,   168,   166,   123,    94,    22,   125,   133,    79,
     153,    73,   168,   156,    70,   126,   125,   126,    74,   139,
     126,    70,   156,    75,    79,    77,   123,   123,   123,    73,
     155,   133,   125,    73,    94,   168,   156,   153,    79,    21,
      73,     7,   140,   141,   142,    73,   123,   153,    73,    94,
      73,   126,   133,   156,   168,   126,   125,     8,    75,   143,
     142,   127,   156,   153,   133,   126,    73,    79,    79,   140,
     156,    73,   126,   128,   128,    75,   126
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule); \
} while (YYID (0))

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
#ifndef	YYINITDEPTH
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
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
	    /* Fall through.  */
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

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {
      case 69: /* "STRING" */
#line 125 "grammar.y"
	{ if ( (yyvaluep->ustr) ) delete (yyvaluep->ustr); };
#line 1839 "grammar.tab.c"
	break;
      case 70: /* "IDENT" */
#line 125 "grammar.y"
	{ if ( (yyvaluep->ustr) ) delete (yyvaluep->ustr); };
#line 1844 "grammar.tab.c"
	break;

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;
/* Location data for the look-ahead symbol.  */
YYLTYPE yylloc;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;

  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[2];

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;
  yylsp = yyls;
#if YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 0;
#endif

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);
	yyls = yyls1;
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
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
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);
	YYSTACK_RELOCATE (yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;
  *++yylsp = yylloc;
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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 175 "grammar.y"
    { (yyval.node) = new QSNullNode(); ;}
    break;

  case 3:
#line 176 "grammar.y"
    { (yyval.node) = new QSBooleanNode(true); ;}
    break;

  case 4:
#line 177 "grammar.y"
    { (yyval.node) = new QSBooleanNode(false); ;}
    break;

  case 5:
#line 178 "grammar.y"
    { (yyval.node) = new QSNumberNode((yyvsp[(1) - (1)].dval)); ;}
    break;

  case 6:
#line 179 "grammar.y"
    { (yyval.node) = new QSStringNode((yyvsp[(1) - (1)].ustr)); delete (yyvsp[(1) - (1)].ustr); ;}
    break;

  case 7:
#line 180 "grammar.y"
    { QSLexer *l = QSLexer::lexer();
                                     if (!l->scanRegExp()) YYABORT;
                                     (yyval.node) = new QSRegExpNode(l->pattern,l->flags);;}
    break;

  case 8:
#line 186 "grammar.y"
    { (yyval.node) = new QSThisNode(); ;}
    break;

  case 9:
#line 187 "grammar.y"
    { (yyval.node) = new QSResolveNode((yyvsp[(1) - (1)].ustr));
                                     delete (yyvsp[(1) - (1)].ustr); ;}
    break;

  case 12:
#line 191 "grammar.y"
    { (yyval.node) = new QSGroupNode((yyvsp[(2) - (3)].node)); ;}
    break;

  case 13:
#line 192 "grammar.y"
    { (yyval.node) = new QSObjectLiteralNode(0L); ;}
    break;

  case 14:
#line 193 "grammar.y"
    { (yyval.node) = new QSObjectLiteralNode((yyvsp[(2) - (3)].node)); ;}
    break;

  case 15:
#line 197 "grammar.y"
    { (yyval.node) = new QSArrayNode((yyvsp[(2) - (3)].eli)); ;}
    break;

  case 16:
#line 198 "grammar.y"
    { (yyval.node) = new QSArrayNode((yyvsp[(2) - (3)].elm)); ;}
    break;

  case 17:
#line 199 "grammar.y"
    { (yyval.node) = new QSArrayNode((yyvsp[(4) - (5)].eli), (yyvsp[(2) - (5)].elm)); ;}
    break;

  case 18:
#line 203 "grammar.y"
    { (yyval.elm) = new QSElementNode((yyvsp[(1) - (2)].eli), (yyvsp[(2) - (2)].node)); ;}
    break;

  case 19:
#line 205 "grammar.y"
    { (yyval.elm) = new QSElementNode((yyvsp[(1) - (4)].elm), (yyvsp[(3) - (4)].eli), (yyvsp[(4) - (4)].node)); ;}
    break;

  case 20:
#line 209 "grammar.y"
    { (yyval.eli) = 0L; ;}
    break;

  case 22:
#line 214 "grammar.y"
    { (yyval.eli) = new QSElisionNode(0L); ;}
    break;

  case 23:
#line 215 "grammar.y"
    { (yyval.eli) = new QSElisionNode((yyvsp[(1) - (2)].eli)); ;}
    break;

  case 24:
#line 219 "grammar.y"
    { (yyval.node) = new QSPropertyValueNode((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 25:
#line 221 "grammar.y"
    { (yyval.node) = new QSPropertyValueNode((yyvsp[(3) - (5)].node), (yyvsp[(5) - (5)].node), (yyvsp[(1) - (5)].node)); ;}
    break;

  case 26:
#line 225 "grammar.y"
    { (yyval.node) = new QSPropertyNode((yyvsp[(1) - (1)].ustr));
                                     delete (yyvsp[(1) - (1)].ustr); ;}
    break;

  case 27:
#line 227 "grammar.y"
    { (yyval.node) = new QSPropertyNode((yyvsp[(1) - (1)].ustr)); delete (yyvsp[(1) - (1)].ustr); ;}
    break;

  case 28:
#line 228 "grammar.y"
    { (yyval.node) = new QSPropertyNode((yyvsp[(1) - (1)].dval)); ;}
    break;

  case 31:
#line 234 "grammar.y"
    { (yyval.node) = new QSAccessorNode1((yyvsp[(1) - (4)].node), (yyvsp[(3) - (4)].node)); ;}
    break;

  case 32:
#line 235 "grammar.y"
    { (yyval.node) = new QSAccessorNode2((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].ustr));
                                     delete (yyvsp[(3) - (3)].ustr); ;}
    break;

  case 33:
#line 237 "grammar.y"
    { (yyval.node) = new QSNewExprNode((yyvsp[(2) - (3)].node), (yyvsp[(3) - (3)].args)); ;}
    break;

  case 35:
#line 242 "grammar.y"
    { (yyval.node) = new QSNewExprNode((yyvsp[(2) - (2)].node)); ;}
    break;

  case 36:
#line 246 "grammar.y"
    { (yyval.node) = new QSFunctionCallNode((yyvsp[(1) - (2)].node), (yyvsp[(2) - (2)].args)); ;}
    break;

  case 37:
#line 247 "grammar.y"
    { (yyval.node) = new QSFunctionCallNode((yyvsp[(1) - (2)].node), (yyvsp[(2) - (2)].args)); ;}
    break;

  case 38:
#line 248 "grammar.y"
    { (yyval.node) = new QSAccessorNode1((yyvsp[(1) - (4)].node), (yyvsp[(3) - (4)].node)); ;}
    break;

  case 39:
#line 249 "grammar.y"
    { (yyval.node) = new QSAccessorNode2((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].ustr));
                                     delete (yyvsp[(3) - (3)].ustr); ;}
    break;

  case 40:
#line 254 "grammar.y"
    { (yyval.args) = new QSArgumentsNode(0L); ;}
    break;

  case 41:
#line 255 "grammar.y"
    { (yyval.args) = new QSArgumentsNode((yyvsp[(2) - (3)].alist)); ;}
    break;

  case 42:
#line 259 "grammar.y"
    { (yyval.alist) = new QSArgumentListNode((yyvsp[(1) - (1)].node)); ;}
    break;

  case 43:
#line 260 "grammar.y"
    { (yyval.alist) = new QSArgumentListNode((yyvsp[(1) - (3)].alist), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 47:
#line 270 "grammar.y"
    { (yyval.node) = new QSPostfixNode((yyvsp[(1) - (2)].node), OpPlusPlus); ;}
    break;

  case 48:
#line 271 "grammar.y"
    { (yyval.node) = new QSPostfixNode((yyvsp[(1) - (2)].node), OpMinusMinus); ;}
    break;

  case 50:
#line 276 "grammar.y"
    { (yyval.node) = new QSDeleteNode((yyvsp[(2) - (2)].node)); ;}
    break;

  case 51:
#line 277 "grammar.y"
    { (yyval.node) = new QSVoidNode((yyvsp[(2) - (2)].node)); ;}
    break;

  case 52:
#line 278 "grammar.y"
    { (yyval.node) = new QSTypeOfNode((yyvsp[(2) - (2)].node)); ;}
    break;

  case 53:
#line 279 "grammar.y"
    { (yyval.node) = new QSPrefixNode(OpPlusPlus, (yyvsp[(2) - (2)].node)); ;}
    break;

  case 54:
#line 280 "grammar.y"
    { (yyval.node) = new QSPrefixNode(OpMinusMinus, (yyvsp[(2) - (2)].node)); ;}
    break;

  case 55:
#line 281 "grammar.y"
    { (yyval.node) = new QSUnaryPlusNode((yyvsp[(2) - (2)].node)); ;}
    break;

  case 56:
#line 282 "grammar.y"
    { (yyval.node) = new QSNegateNode((yyvsp[(2) - (2)].node)); ;}
    break;

  case 57:
#line 283 "grammar.y"
    { (yyval.node) = new QSBitwiseNotNode((yyvsp[(2) - (2)].node)); ;}
    break;

  case 58:
#line 284 "grammar.y"
    { (yyval.node) = new QSLogicalNotNode((yyvsp[(2) - (2)].node)); ;}
    break;

  case 60:
#line 289 "grammar.y"
    { (yyval.node) = new QSMultNode((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), '*'); ;}
    break;

  case 61:
#line 290 "grammar.y"
    { (yyval.node) = new QSMultNode((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), '/'); ;}
    break;

  case 62:
#line 291 "grammar.y"
    { (yyval.node) = new QSMultNode((yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node),'%'); ;}
    break;

  case 64:
#line 296 "grammar.y"
    { (yyval.node) = new QSAddNode((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), '+'); ;}
    break;

  case 65:
#line 297 "grammar.y"
    { (yyval.node) = new QSAddNode((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), '-'); ;}
    break;

  case 67:
#line 302 "grammar.y"
    { (yyval.node) = new QSShiftNode((yyvsp[(1) - (3)].node), OpLShift, (yyvsp[(3) - (3)].node)); ;}
    break;

  case 68:
#line 303 "grammar.y"
    { (yyval.node) = new QSShiftNode((yyvsp[(1) - (3)].node), OpRShift, (yyvsp[(3) - (3)].node)); ;}
    break;

  case 69:
#line 304 "grammar.y"
    { (yyval.node) = new QSShiftNode((yyvsp[(1) - (3)].node), OpURShift, (yyvsp[(3) - (3)].node)); ;}
    break;

  case 71:
#line 310 "grammar.y"
    { (yyval.node) = new QSRelationalNode((yyvsp[(1) - (3)].node), OpLess, (yyvsp[(3) - (3)].node)); ;}
    break;

  case 72:
#line 312 "grammar.y"
    { (yyval.node) = new QSRelationalNode((yyvsp[(1) - (3)].node), OpGreater, (yyvsp[(3) - (3)].node)); ;}
    break;

  case 73:
#line 314 "grammar.y"
    { (yyval.node) = new QSRelationalNode((yyvsp[(1) - (3)].node), OpLessEq, (yyvsp[(3) - (3)].node)); ;}
    break;

  case 74:
#line 316 "grammar.y"
    { (yyval.node) = new QSRelationalNode((yyvsp[(1) - (3)].node), OpGreaterEq, (yyvsp[(3) - (3)].node)); ;}
    break;

  case 75:
#line 318 "grammar.y"
    { (yyval.node) = new QSRelationalNode((yyvsp[(1) - (3)].node), OpIs, (yyvsp[(3) - (3)].node)); ;}
    break;

  case 76:
#line 320 "grammar.y"
    { (yyval.node) = new QSRelationalNode((yyvsp[(1) - (3)].node), OpInstanceOf, (yyvsp[(3) - (3)].node)); ;}
    break;

  case 77:
#line 322 "grammar.y"
    { (yyval.node) = new QSRelationalNode((yyvsp[(1) - (3)].node), OpIn, (yyvsp[(3) - (3)].node)); ;}
    break;

  case 79:
#line 327 "grammar.y"
    { (yyval.node) = new QSEqualNode((yyvsp[(1) - (3)].node), OpEqEq, (yyvsp[(3) - (3)].node)); ;}
    break;

  case 80:
#line 328 "grammar.y"
    { (yyval.node) = new QSEqualNode((yyvsp[(1) - (3)].node), OpNotEq, (yyvsp[(3) - (3)].node)); ;}
    break;

  case 81:
#line 329 "grammar.y"
    { (yyval.node) = new QSEqualNode((yyvsp[(1) - (3)].node), OpStrEq, (yyvsp[(3) - (3)].node)); ;}
    break;

  case 82:
#line 330 "grammar.y"
    { (yyval.node) = new QSEqualNode((yyvsp[(1) - (3)].node), OpStrNEq, (yyvsp[(3) - (3)].node));;}
    break;

  case 84:
#line 335 "grammar.y"
    { (yyval.node) = new QSBitOperNode((yyvsp[(1) - (3)].node), OpBitAnd, (yyvsp[(3) - (3)].node)); ;}
    break;

  case 86:
#line 340 "grammar.y"
    { (yyval.node) = new QSBitOperNode((yyvsp[(1) - (3)].node), OpBitXOr, (yyvsp[(3) - (3)].node)); ;}
    break;

  case 88:
#line 345 "grammar.y"
    { (yyval.node) = new QSBitOperNode((yyvsp[(1) - (3)].node), OpBitOr, (yyvsp[(3) - (3)].node)); ;}
    break;

  case 90:
#line 351 "grammar.y"
    { (yyval.node) = new QSBinaryLogicalNode((yyvsp[(1) - (3)].node), OpAnd, (yyvsp[(3) - (3)].node)); ;}
    break;

  case 92:
#line 357 "grammar.y"
    { (yyval.node) = new QSBinaryLogicalNode((yyvsp[(1) - (3)].node), OpOr, (yyvsp[(3) - (3)].node)); ;}
    break;

  case 94:
#line 363 "grammar.y"
    { (yyval.node) = new QSConditionalNode((yyvsp[(1) - (5)].node), (yyvsp[(3) - (5)].node), (yyvsp[(5) - (5)].node)); ;}
    break;

  case 96:
#line 369 "grammar.y"
    { (yyval.node) = new QSAssignNode((yyvsp[(1) - (3)].node), (yyvsp[(2) - (3)].op), (yyvsp[(3) - (3)].node));;}
    break;

  case 97:
#line 373 "grammar.y"
    { (yyval.op) = OpEqual; ;}
    break;

  case 98:
#line 374 "grammar.y"
    { (yyval.op) = OpPlusEq; ;}
    break;

  case 99:
#line 375 "grammar.y"
    { (yyval.op) = OpMinusEq; ;}
    break;

  case 100:
#line 376 "grammar.y"
    { (yyval.op) = OpMultEq; ;}
    break;

  case 101:
#line 377 "grammar.y"
    { (yyval.op) = OpDivEq; ;}
    break;

  case 102:
#line 378 "grammar.y"
    { (yyval.op) = OpLShift; ;}
    break;

  case 103:
#line 379 "grammar.y"
    { (yyval.op) = OpRShift; ;}
    break;

  case 104:
#line 380 "grammar.y"
    { (yyval.op) = OpURShift; ;}
    break;

  case 105:
#line 381 "grammar.y"
    { (yyval.op) = OpAndEq; ;}
    break;

  case 106:
#line 382 "grammar.y"
    { (yyval.op) = OpXOrEq; ;}
    break;

  case 107:
#line 383 "grammar.y"
    { (yyval.op) = OpOrEq; ;}
    break;

  case 108:
#line 384 "grammar.y"
    { (yyval.op) = OpModEq; ;}
    break;

  case 110:
#line 389 "grammar.y"
    { (yyval.node) = new QSCommaNode((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 113:
#line 395 "grammar.y"
    { (yyval.stat) = (yyvsp[(1) - (1)].clss); ;}
    break;

  case 114:
#line 396 "grammar.y"
    { (yyvsp[(2) - (2)].clss)->setAttributes( (yyvsp[(1) - (2)].attrs) ); (yyval.stat) = (yyvsp[(2) - (2)].clss); ;}
    break;

  case 129:
#line 414 "grammar.y"
    { (yyval.stat) = new QSBlockNode(0L); DBG((yyval.stat), (yylsp[(2) - (2)]), (yylsp[(2) - (2)])); ;}
    break;

  case 130:
#line 415 "grammar.y"
    { (yyval.stat) = new QSBlockNode((yyvsp[(2) - (3)].slist)); DBG((yyval.stat), (yylsp[(3) - (3)]), (yylsp[(3) - (3)])); ;}
    break;

  case 131:
#line 419 "grammar.y"
    { (yyval.slist) = new QSStatListNode((yyvsp[(1) - (1)].stat)); ;}
    break;

  case 132:
#line 420 "grammar.y"
    { (yyval.slist) = new QSStatListNode((yyvsp[(1) - (2)].slist), (yyvsp[(2) - (2)].stat)); ;}
    break;

  case 133:
#line 424 "grammar.y"
    { (yyval.stat) = new QSEmptyStatementNode(); ;}
    break;

  case 134:
#line 428 "grammar.y"
    { (yyval.stat) = new QSExprStatementNode((yyvsp[(1) - (2)].node));
                                     DBG((yyval.stat), (yylsp[(1) - (2)]), (yylsp[(2) - (2)])); ;}
    break;

  case 135:
#line 430 "grammar.y"
    { if (automatic()) {
                                       (yyval.stat) = new QSExprStatementNode((yyvsp[(1) - (2)].node));
				       DBG((yyval.stat), (yylsp[(1) - (2)]), (yylsp[(1) - (2)]));
                                     } else
				       YYABORT; ;}
    break;

  case 136:
#line 438 "grammar.y"
    { (yyval.stat) = new QSIfNode((yyvsp[(3) - (5)].node),(yyvsp[(5) - (5)].stat),0L);DBG((yyval.stat),(yylsp[(1) - (5)]),(yylsp[(4) - (5)])); ;}
    break;

  case 137:
#line 440 "grammar.y"
    { (yyval.stat) = new QSIfNode((yyvsp[(3) - (7)].node),(yyvsp[(5) - (7)].stat),(yyvsp[(7) - (7)].stat));DBG((yyval.stat),(yylsp[(1) - (7)]),(yylsp[(4) - (7)])); ;}
    break;

  case 138:
#line 444 "grammar.y"
    { (yyval.stat)=new QSDoWhileNode((yyvsp[(2) - (6)].stat),(yyvsp[(5) - (6)].node));DBG((yyval.stat),(yylsp[(1) - (6)]),(yylsp[(3) - (6)]));;}
    break;

  case 139:
#line 445 "grammar.y"
    { (yyval.stat) = new QSWhileNode((yyvsp[(3) - (5)].node),(yyvsp[(5) - (5)].stat));DBG((yyval.stat),(yylsp[(1) - (5)]),(yylsp[(4) - (5)])); ;}
    break;

  case 140:
#line 447 "grammar.y"
    { (yyval.stat) = new QSForNode((yyvsp[(3) - (9)].node),(yyvsp[(5) - (9)].node),(yyvsp[(7) - (9)].node),(yyvsp[(9) - (9)].stat));
	                             DBG((yyval.stat),(yylsp[(1) - (9)]),(yylsp[(8) - (9)])); ;}
    break;

  case 141:
#line 450 "grammar.y"
    { (yyval.stat) = new QSForNode((yyvsp[(4) - (10)].blist),(yyvsp[(6) - (10)].node),(yyvsp[(8) - (10)].node),(yyvsp[(10) - (10)].stat));
	                             DBG((yyval.stat),(yylsp[(1) - (10)]),(yylsp[(9) - (10)])); ;}
    break;

  case 142:
#line 453 "grammar.y"
    { (yyval.stat) = new QSForInNode((yyvsp[(3) - (7)].node), (yyvsp[(5) - (7)].node), (yyvsp[(7) - (7)].stat));
	                             DBG((yyval.stat),(yylsp[(1) - (7)]),(yylsp[(6) - (7)])); ;}
    break;

  case 143:
#line 456 "grammar.y"
    { (yyval.stat) = new QSForInNode((yyvsp[(4) - (8)].vbin),(yyvsp[(6) - (8)].node),(yyvsp[(8) - (8)].stat));
	                             DBG((yyval.stat),(yylsp[(1) - (8)]),(yylsp[(7) - (8)])); ;}
    break;

  case 144:
#line 461 "grammar.y"
    { (yyval.node) = 0L; ;}
    break;

  case 146:
#line 466 "grammar.y"
    { (yyval.stat) = new QSContinueNode(); DBG((yyval.stat),(yylsp[(1) - (2)]),(yylsp[(2) - (2)])); ;}
    break;

  case 147:
#line 467 "grammar.y"
    { if (automatic()) {
                                       (yyval.stat) = new QSContinueNode(); DBG((yyval.stat),(yylsp[(1) - (2)]),(yylsp[(2) - (2)]));
                                     } else
				       YYABORT; ;}
    break;

  case 148:
#line 471 "grammar.y"
    { (yyval.stat) = new QSContinueNode((yyvsp[(2) - (3)].ustr)); DBG((yyval.stat),(yylsp[(1) - (3)]),(yylsp[(3) - (3)]));
                                     delete (yyvsp[(2) - (3)].ustr); ;}
    break;

  case 149:
#line 473 "grammar.y"
    { if (automatic()) {
                                       (yyval.stat) = new QSContinueNode((yyvsp[(2) - (3)].ustr));DBG((yyval.stat),(yylsp[(1) - (3)]),(yylsp[(2) - (3)]));
				       delete (yyvsp[(2) - (3)].ustr);
                                     } else
				       YYABORT; ;}
    break;

  case 150:
#line 481 "grammar.y"
    { (yyval.stat) = new QSBreakNode();DBG((yyval.stat),(yylsp[(1) - (2)]),(yylsp[(2) - (2)])); ;}
    break;

  case 151:
#line 482 "grammar.y"
    { if (automatic()) {
                                       (yyval.stat) = new QSBreakNode(); DBG((yyval.stat),(yylsp[(1) - (2)]),(yylsp[(1) - (2)]));
                                     } else
				       YYABORT; ;}
    break;

  case 152:
#line 486 "grammar.y"
    { (yyval.stat) = new QSBreakNode((yyvsp[(2) - (3)].ustr)); DBG((yyval.stat),(yylsp[(1) - (3)]),(yylsp[(3) - (3)]));
                                     delete (yyvsp[(2) - (3)].ustr); ;}
    break;

  case 153:
#line 488 "grammar.y"
    { if (automatic()) {
                                       (yyval.stat) = new QSBreakNode((yyvsp[(2) - (3)].ustr)); DBG((yyval.stat),(yylsp[(1) - (3)]),(yylsp[(2) - (3)]));
				       delete (yyvsp[(2) - (3)].ustr);
                                     } else
				       YYABORT;
                                   ;}
    break;

  case 154:
#line 497 "grammar.y"
    { (yyval.stat) = new QSReturnNode(0L); DBG((yyval.stat),(yylsp[(1) - (2)]),(yylsp[(2) - (2)])); ;}
    break;

  case 155:
#line 498 "grammar.y"
    { if (automatic()) {
                                       (yyval.stat) = new QSReturnNode(0L); DBG((yyval.stat),(yylsp[(1) - (2)]),(yylsp[(1) - (2)]));
                                     } else
				       YYABORT; ;}
    break;

  case 156:
#line 502 "grammar.y"
    { (yyval.stat) = new QSReturnNode((yyvsp[(2) - (3)].node)); DBG((yyval.stat),(yylsp[(1) - (3)]),(yylsp[(2) - (3)])); ;}
    break;

  case 157:
#line 503 "grammar.y"
    { if (automatic()) {
                                       (yyval.stat) = new QSReturnNode((yyvsp[(2) - (3)].node));
                                       DBG((yyval.stat),(yylsp[(1) - (3)]),(yylsp[(2) - (3)]));
                                     } else {
				       YYABORT;
				     } 
                                   ;}
    break;

  case 158:
#line 513 "grammar.y"
    { (yyval.stat) = new QSWithNode((yyvsp[(3) - (5)].node),(yyvsp[(5) - (5)].stat));
                                     DBG((yyval.stat), (yylsp[(1) - (5)]), (yylsp[(4) - (5)])); ;}
    break;

  case 159:
#line 518 "grammar.y"
    { (yyval.stat) = new QSSwitchNode((yyvsp[(3) - (5)].node), (yyvsp[(5) - (5)].cblk));
                                     DBG((yyval.stat), (yylsp[(1) - (5)]), (yylsp[(4) - (5)])); ;}
    break;

  case 160:
#line 523 "grammar.y"
    { (yyval.cblk) = new QSCaseBlockNode((yyvsp[(2) - (3)].clist), 0L, 0L); ;}
    break;

  case 161:
#line 525 "grammar.y"
    { (yyval.cblk) = new QSCaseBlockNode((yyvsp[(2) - (5)].clist), (yyvsp[(3) - (5)].ccl), (yyvsp[(4) - (5)].clist)); ;}
    break;

  case 162:
#line 529 "grammar.y"
    { (yyval.clist) = 0L; ;}
    break;

  case 164:
#line 534 "grammar.y"
    { (yyval.clist) = new QSClauseListNode((yyvsp[(1) - (1)].ccl)); ;}
    break;

  case 165:
#line 535 "grammar.y"
    { (yyval.clist) = (yyvsp[(1) - (2)].clist)->append((yyvsp[(2) - (2)].ccl)); ;}
    break;

  case 166:
#line 539 "grammar.y"
    { (yyval.ccl) = new QSCaseClauseNode((yyvsp[(2) - (3)].node), 0L); ;}
    break;

  case 167:
#line 540 "grammar.y"
    { (yyval.ccl) = new QSCaseClauseNode((yyvsp[(2) - (4)].node), (yyvsp[(4) - (4)].slist)); ;}
    break;

  case 168:
#line 544 "grammar.y"
    { (yyval.ccl) = new QSCaseClauseNode(0L, 0L);; ;}
    break;

  case 169:
#line 545 "grammar.y"
    { (yyval.ccl) = new QSCaseClauseNode(0L, (yyvsp[(3) - (3)].slist)); ;}
    break;

  case 170:
#line 549 "grammar.y"
    { (yyvsp[(3) - (3)].stat)->pushLabel((yyvsp[(1) - (3)].ustr));
                                     (yyval.stat) = new QSLabelNode((yyvsp[(1) - (3)].ustr), (yyvsp[(3) - (3)].stat));
                                     delete (yyvsp[(1) - (3)].ustr); ;}
    break;

  case 171:
#line 555 "grammar.y"
    { (yyval.stat) = new QSThrowNode((yyvsp[(2) - (3)].node)); ;}
    break;

  case 172:
#line 559 "grammar.y"
    { (yyval.stat) = new QSTryNode((yyvsp[(2) - (3)].stat), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 173:
#line 560 "grammar.y"
    { (yyval.stat) = new QSTryNode((yyvsp[(2) - (3)].stat), 0L, (yyvsp[(3) - (3)].node)); ;}
    break;

  case 174:
#line 561 "grammar.y"
    { (yyval.stat) = new QSTryNode((yyvsp[(2) - (4)].stat), (yyvsp[(3) - (4)].node), (yyvsp[(4) - (4)].node)); ;}
    break;

  case 175:
#line 565 "grammar.y"
    { (yyval.node) = new QSCatchNode((yyvsp[(3) - (5)].ustr), (yyvsp[(5) - (5)].stat)); delete (yyvsp[(3) - (5)].ustr); ;}
    break;

  case 176:
#line 569 "grammar.y"
    { (yyval.node) = new QSFinallyNode((yyvsp[(2) - (2)].stat)); ;}
    break;

  case 177:
#line 574 "grammar.y"
    { (yyval.stat) = new QSPackageNode((yyvsp[(2) - (3)].ustr), (yyvsp[(3) - (3)].body));
                                     delete (yyvsp[(2) - (3)].ustr); ;}
    break;

  case 178:
#line 579 "grammar.y"
    { (yyval.stat) = new QSImportNode((yyvsp[(2) - (3)].ustr)); delete (yyvsp[(2) - (3)].ustr); ;}
    break;

  case 179:
#line 583 "grammar.y"
    { (yyval.ustr) = (yyvsp[(1) - (1)].ustr); delete (yyvsp[(1) - (1)].ustr); ;}
    break;

  case 180:
#line 584 "grammar.y"
    { *(yyval.ustr) += QString::fromLatin1(".") + *(yyvsp[(3) - (3)].ustr); delete (yyvsp[(3) - (3)].ustr); ;}
    break;

  case 181:
#line 589 "grammar.y"
    { (yyval.func) = new QSFuncDeclNode((yyvsp[(2) - (6)].ustr), 0L, (yyvsp[(5) - (6)].type), (yyvsp[(6) - (6)].body)); delete (yyvsp[(2) - (6)].ustr); ;}
    break;

  case 182:
#line 591 "grammar.y"
    { (yyval.func) = new QSFuncDeclNode((yyvsp[(2) - (7)].ustr), (yyvsp[(4) - (7)].param), (yyvsp[(6) - (7)].type), (yyvsp[(7) - (7)].body)); delete (yyvsp[(2) - (7)].ustr); ;}
    break;

  case 183:
#line 593 "grammar.y"
    { (yyval.func) = new QSFuncDeclNode((yyvsp[(3) - (7)].ustr), 0L, (yyvsp[(6) - (7)].type), (yyvsp[(7) - (7)].body));
			  (yyval.func)->setAttributes( (yyvsp[(1) - (7)].attrs) ); delete (yyvsp[(3) - (7)].ustr); ;}
    break;

  case 184:
#line 596 "grammar.y"
    { (yyval.func) = new QSFuncDeclNode((yyvsp[(3) - (8)].ustr), (yyvsp[(5) - (8)].param), (yyvsp[(7) - (8)].type), (yyvsp[(8) - (8)].body));
                          (yyval.func)->setAttributes( (yyvsp[(1) - (8)].attrs) ); delete (yyvsp[(3) - (8)].ustr); ;}
    break;

  case 185:
#line 601 "grammar.y"
    { (yyval.type) = 0L; ;}
    break;

  case 186:
#line 602 "grammar.y"
    { (yyval.type) = (yyvsp[(2) - (2)].type); ;}
    break;

  case 187:
#line 606 "grammar.y"
    { (yyval.node) = new QSFuncExprNode(0L, (yyvsp[(4) - (4)].body)); ;}
    break;

  case 188:
#line 608 "grammar.y"
    { (yyval.node) = new QSFuncExprNode((yyvsp[(3) - (5)].param), (yyvsp[(5) - (5)].body)); ;}
    break;

  case 189:
#line 613 "grammar.y"
    { (yyval.param) = new QSParameterNode((yyvsp[(1) - (1)].ustr), 0L);
                                     delete (yyvsp[(1) - (1)].ustr); ;}
    break;

  case 190:
#line 615 "grammar.y"
    { (yyval.param) = new QSParameterNode((yyvsp[(1) - (3)].ustr), (yyvsp[(3) - (3)].type));
                                     delete (yyvsp[(1) - (3)].ustr); ;}
    break;

  case 191:
#line 617 "grammar.y"
    { (yyval.param) = (yyvsp[(1) - (3)].param)->append((yyvsp[(3) - (3)].ustr), 0L); delete (yyvsp[(3) - (3)].ustr); ;}
    break;

  case 192:
#line 619 "grammar.y"
    { (yyval.param) = (yyvsp[(1) - (5)].param)->append((yyvsp[(3) - (5)].ustr), (yyvsp[(5) - (5)].type)); delete (yyvsp[(3) - (5)].ustr); ;}
    break;

  case 193:
#line 623 "grammar.y"
    { (yyval.body) = new QSFunctionBodyNode(0L); ;}
    break;

  case 194:
#line 624 "grammar.y"
    { (yyval.body) = new QSFunctionBodyNode((yyvsp[(2) - (3)].srcs)); ;}
    break;

  case 195:
#line 628 "grammar.y"
    { (yyval.clss) = new QSClassDefNode((yyvsp[(2) - (3)].ustr), 0L, 0L);
                                     delete (yyvsp[(2) - (3)].ustr); ;}
    break;

  case 196:
#line 630 "grammar.y"
    { (yyval.clss) = new QSClassDefNode((yyvsp[(2) - (3)].ustr), 0L, (yyvsp[(3) - (3)].body));
                                     delete (yyvsp[(2) - (3)].ustr); ;}
    break;

  case 197:
#line 633 "grammar.y"
    { (yyval.clss) = new QSClassDefNode((yyvsp[(2) - (5)].ustr), (yyvsp[(4) - (5)].type), (yyvsp[(5) - (5)].body));
                                     delete (yyvsp[(2) - (5)].ustr); ;}
    break;

  case 198:
#line 638 "grammar.y"
    { (yyval.prog) = new QSProgramNode(0L); ;}
    break;

  case 199:
#line 639 "grammar.y"
    { (yyval.prog) = new QSProgramNode((yyvsp[(1) - (1)].srcs)); ;}
    break;

  case 200:
#line 643 "grammar.y"
    { (yyval.srcs) = new QSSourceElementsNode((yyvsp[(1) - (1)].src)); ;}
    break;

  case 201:
#line 644 "grammar.y"
    { (yyval.srcs) = new QSSourceElementsNode((yyvsp[(1) - (2)].srcs), (yyvsp[(2) - (2)].src)); ;}
    break;

  case 202:
#line 648 "grammar.y"
    { (yyval.src) = new QSSourceElementNode((yyvsp[(1) - (1)].stat)); ;}
    break;

  case 203:
#line 649 "grammar.y"
    { (yyval.src) = new QSSourceElementNode((yyvsp[(1) - (1)].func)); ;}
    break;

  case 204:
#line 653 "grammar.y"
    { (yyval.attrs) = new QSAttributeNode((yyvsp[(1) - (1)].attr)); ;}
    break;

  case 205:
#line 654 "grammar.y"
    { (yyvsp[(1) - (2)].attrs)->add((yyvsp[(2) - (2)].attr)); (yyval.attrs) = (yyvsp[(1) - (2)].attrs); ;}
    break;

  case 206:
#line 658 "grammar.y"
    { (yyval.attr) = AttributeAbstract; ;}
    break;

  case 207:
#line 659 "grammar.y"
    { (yyval.attr) = AttributeFinal; ;}
    break;

  case 208:
#line 660 "grammar.y"
    { (yyval.attr) = AttributePrivate; ;}
    break;

  case 209:
#line 661 "grammar.y"
    { (yyval.attr) = AttributePublic; ;}
    break;

  case 210:
#line 662 "grammar.y"
    { (yyval.attr) = AttributeStatic; ;}
    break;

  case 211:
#line 663 "grammar.y"
    { (yyval.attr) = AttributeTrue; ;}
    break;

  case 212:
#line 664 "grammar.y"
    { (yyval.attr) = AttributeFalse; ;}
    break;

  case 213:
#line 665 "grammar.y"
    { (yyval.attr) = AttributeConstructor; ;}
    break;

  case 214:
#line 672 "grammar.y"
    { (yyval.stat) = new QSVarDefNode((yyvsp[(1) - (2)].ival), (yyvsp[(2) - (2)].blist)); DBG((yyval.stat), (yylsp[(1) - (2)]), (yylsp[(2) - (2)])); ;}
    break;

  case 215:
#line 674 "grammar.y"
    { (yyval.stat) = new QSVarDefNode((yyvsp[(2) - (3)].ival), (yyvsp[(3) - (3)].blist)); DBG((yyval.stat), (yylsp[(1) - (3)]), (yylsp[(3) - (3)]));
                               ((QSVarDefNode*)(yyval.stat))->setAttributes( (yyvsp[(1) - (3)].attrs) ); ;}
    break;

  case 216:
#line 679 "grammar.y"
    { (yyval.ival) = 0; ;}
    break;

  case 217:
#line 680 "grammar.y"
    { (yyval.ival) = 1; ;}
    break;

  case 218:
#line 684 "grammar.y"
    { (yyval.blist) = new QSVarBindingListNode(0L, (yyvsp[(1) - (1)].vbin)); ;}
    break;

  case 219:
#line 686 "grammar.y"
    { (yyval.blist) = new QSVarBindingListNode((yyvsp[(1) - (3)].blist), (yyvsp[(3) - (3)].vbin)); ;}
    break;

  case 220:
#line 690 "grammar.y"
    { (yyval.vbin) = new QSVarBindingNode((yyvsp[(1) - (1)].tvar), 0L); ;}
    break;

  case 221:
#line 691 "grammar.y"
    { (yyval.vbin) = new QSVarBindingNode((yyvsp[(1) - (3)].tvar), (yyvsp[(3) - (3)].node)); ;}
    break;

  case 222:
#line 695 "grammar.y"
    { (yyval.tvar) = new QSTypedVarNode((yyvsp[(1) - (1)].ustr), 0L);
                                     delete (yyvsp[(1) - (1)].ustr); ;}
    break;

  case 223:
#line 697 "grammar.y"
    { (yyval.tvar) = new QSTypedVarNode((yyvsp[(1) - (3)].ustr), (yyvsp[(3) - (3)].type));
                                     delete (yyvsp[(1) - (3)].ustr); ;}
    break;

  case 224:
#line 704 "grammar.y"
    { (yyval.type) = new QSTypeNode((yyvsp[(1) - (1)].ustr)); delete (yyvsp[(1) - (1)].ustr); ;}
    break;


/* Line 1267 of yacc.c.  */
#line 3134 "grammar.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }

  yyerror_range[0] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
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
		      yytoken, &yylval, &yylloc);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[0] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
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

      yyerror_range[0] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;

  yyerror_range[1] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the look-ahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
  *++yylsp = yyloc;

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

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, &yylloc);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp);
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
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 707 "grammar.y"


int yyerror ( const char *errstr )  /* Called by yyparse on error */
{
  QSLexer::lexer()->setErrorMessage( errstr );
  return 1;
}

/* may we automatically insert a semicolon ? */
bool automatic()
{
  if ( yychar == '}' || yychar == 0 )
    return true;
  else if ( QSLexer::lexer()->prevTerminator() )
    return true;

  return false;
}

