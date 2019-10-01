/* A Bison parser, made by GNU Bison 3.3.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

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
#line 18 "imp.y" /* yacc.c:1921  */

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

#line 151 "imp.tab.h" /* yacc.c:1921  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_IMP_TAB_H_INCLUDED  */
