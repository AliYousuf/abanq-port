/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

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
/* Line 1489 of yacc.c.  */
#line 220 "grammar.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE qsyylval;

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

extern YYLTYPE qsyylloc;
