%{
#include <stdio.h>
#include "tree.h"

extern char *yytext;
extern AST_NODE* AST;

void yyerror(char* ss) {
  printf("syntax error before %s\n", yytext);
}

int yylex(); //to prevent implicit declaration warnings.

PROPERTY current_property;

%}

%union {
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
}


 /* ====================================== TOKEN START ========================================== */

%token <intconst> tINTCONST
%token <stringconst> tIDENTIFIER
%token <stringconst> tIDENTIFIER_DECL
%token <dynamic_string> tSTRING
%token tCOMMAND_LOAD
%token tCOMMAND_CONTAIN
%token tCOMMAND_SHADER
%token tCOMMAND_BUILD
%token tEQUALITY
%token tLESSER_EQUAL
%token tGREATER_EQUAL
%token tNON_EQUALITY
%token tAND
%token tOR
%token tRETURN
%token tWRITE
%token tIF
%token tELSE
%token tTHEN
%token tALLOCATE
%token tWHILE
%token tDO
%token tTYPE
%token tFUNC
%token tEND
%token tVAR
%token tOF_LENGTH


 /* types */
%token tBYTE			/* "byte"		*/
%token tINT				/* "int"		*/
%token tSHORT			/* "short"		*/
%token tLONG			/* "long"		*/
%token tFLOAT			/* "float"		*/
%token tDOUBLE			/* "double"		*/
%token tu8				/* "u8"			*/
%token tu16				/* "u16"		*/
%token tu32				/* "u32"		*/
%token tu64				/* "u64"		*/
%token tu128			/* "u128"		*/
%token tu256			/* "u256"		*/
%token tu512			/* "u512"		*/
%token tSTRING_TOKEN;	/* "string"		*/
%token tBOOL			/* "bool"		*/
%token tTRUE			/* "true"		*/
%token tFALSE			/* "false"		*/
%token tNULL			/* "null"		*/
%token tBITSET			/* "bitset"		*/
%token tBOX				/* "[]" array	*/
%token tSTRUCT			/* "struct"		*/
%token tUNION			/* "union"		*/
%token tVOID			/* "void"		*/

%token tPLUS_EQUAL
%token tMINUS_EQUAL
%token tTIMES_EQUAL
%token tDIVIDE_EQUAL
%token tAND_EQUAL
%token tOR_EQUAL


%token tFOR
%token tDOUBLE_SEMICOLON
%token tRIGHT_ARROW
%token tLEFT_ARROW
%token tPRINT

 /* property */
%token tSOA
%token tAOS



 /* ====================================== TOKEN END ========================================== */

%type <directive_list> directive_list
%type <directive> directive
%type <string> string
%type <intconst> array_length

%type <function> function
%type <head> head
%type <tail> tail
%type <type> type
%type <type> type_omittable
%type <par_decl_list> par_decl_list
%type <var_decl_list> var_decl_list
%type <var_type> var_type

%type <body> body
%type <ast> program pre_program

%type <decl_list> decl_list
%type <declaration> declaration
%type <statement_list> statement_list
%type <statement> statement
%type <variable> variable
%type <exp> exp
%type <term> term
%type <act_list> act_list
%type <exp_list> exp_list

%expect 2

%start pre_program

%left tOR
%left tAND
%left tEQUALITY tNON_EQUALITY tPLUS_EQUAL tMINUS_EQUAL tTIMES_EQUAL tDIVIDE_EQUAL tAND_EQUAL tOR_EQUAL
%left '<' '>' tLESSER_EQUAL tGREATER_EQUAL
%left '+' '-'
%left '*' '/'

%%


pre_program : program
	{
		AST = $1;
	}
;

program: directive_list body
	{
		$$ = make_AST($1, $2);
	}
;


string : tSTRING
	{
		$$ = make_STRING($1);
	}
	| tSTRING '+' tSTRING
	{
		$$ = make_STRING_concatenation($1, $3);
	}
;

directive_list : directive_list directive
	{
		$$ = make_DIRECTIVE_LIST_pair($1, $2);
	}
	| %empty
	{
		$$ = make_DIRECTIVE_LIST_empty();
	}
;

directive : tCOMMAND_LOAD tSTRING
	{
		$$ = make_DIRECTIVE_load($2);
	}
	| tCOMMAND_CONTAIN tSTRING
	{
		$$ = make_DIRECTIVE_contain($2);
	}
	| tCOMMAND_BUILD tIDENTIFIER '(' act_list ')'
	{
		$$ = make_DIRECTIVE_build($2, $4);
	}
	| tCOMMAND_SHADER tIDENTIFIER tSTRING
	{
		$$ = make_DIRECTIVE_shader($2, $3);
	}

;


function : head '{' body '}'
	{
		$$ = make_FUNC($1, $3);
	}
;


head : tIDENTIFIER_DECL ':' '(' par_decl_list ')' tRIGHT_ARROW type
	{
		$$ = make_HEAD($1, $4, $7);
	}
;


array_length : tINTCONST
	{
		$$ = $1;
	}
	| %empty
	{
	}
;

type_omittable : %empty
	{
	}
	| type
	{
		$$ = $1;
	}
;


type
	: tIDENTIFIER
	{
		$$ = make_TYPE_id($1);
	}
	| tINT
	{
		$$ = make_TYPE_int();
	}
	| tBOOL
	{
		$$= make_TYPE_bool();
	}
	| tSTRING_TOKEN
	{
		$$ = make_TYPE_string();
	}
	| '[' array_length ']' type
	{
		$$ = make_TYPE_array($2, $4);
	}
	| tSTRUCT property '{' var_decl_list '}'
	{
		$$ = make_TYPE_record($4, current_property);
		current_property = PROPERTY::NONE;
	}
	| tBYTE
	{
		$$ = make_TYPE_byte();
	}
	| tSHORT
	{
		$$ = make_TYPE_short();
	}
	| tLONG
	{
		$$ = make_TYPE_long();
	}
	| tFLOAT
	{
		$$ = make_TYPE_float();
	}
	| tDOUBLE
	{
		$$ = make_TYPE_double();
	}
	| tu8
	{
		$$ = make_TYPE_u8();
	}
	| tu16
	{
		$$ = make_TYPE_u16();
	}
	| tu32
	{
		$$ = make_TYPE_u32();
	}
	| tu64
	{
		$$ = make_TYPE_u64();
	}
	| tu128
	{
		$$ = make_TYPE_u128();
	}
	| tu256
	{
		$$ = make_TYPE_u256();
	}
	| tu512
	{
		$$ = make_TYPE_u512();
	}
	| tBITSET
	{
		$$ = make_TYPE_bitset();
	}
	| tUNION '{' var_decl_list '}'
	{
		$$ = make_TYPE_union($3);
	}
	| tVOID
	{
		$$ = make_TYPE_void();
	}
;

par_decl_list : var_decl_list
	{
		$$ = make_PAR_DECL_LIST($1);
	}
	| %empty
	{
		$$ = make_PAR_DECL_LIST_empty();
	}
;


var_decl_list : var_type ',' var_decl_list
	{
		$$ = make_VAR_DECL_LIST_pair($1, $3);
	}
	| var_type
	{
		$$ = make_VAR_DECL_LIST_last($1);
	}
;

var_type : tIDENTIFIER_DECL type
	{
		$$ = make_VAR_TYPE($1, $2);
	}
;

body : decl_list statement_list
	{
		$$ = make_BODY($1,$2);
	}
;

decl_list : %empty
	{
		$$ = make_DECL_LIST_empty();
	}
	| decl_list declaration
	{
		$$ = make_DECL_LIST_pair($2, $1);
	}
;

/*

decl_list : declaration decl_list
	{
		$$ = make_DECL_LIST_pair($1, $2);
	}
	| %empty
	{
		$$ = make_DECL_LIST_empty();
	}
;

*/

declaration : function
	{
		$$ = make_DECL_func($1);
	}
	| var_decl_list ';'
	{
		$$ = make_DECL_vars($1);
	}
	| tIDENTIFIER_DECL type_omittable '=' exp ';'
	{
		//
	}
;

/*
declaration : tTYPE tIDENTIFIER '=' type ';'
	{
		$$ = make_DECL_assignment($2, $4);
	}
	| function
	{
		$$ = make_DECL_func($1);
	}
	| tVAR var_decl_list ';'
	{
		$$ = make_DECL_vars($2);
	}
;
*/

statement_list : %empty
	{
		$$ = make_STM_LIST_empty();
	}
	| statement_list statement
	{
		$$ = make_STM_LIST_pair($2, $1);
	}
;

/*
statement_list : statement statement_list
	{
		$$ = make_STM_LIST_pair($1, $2);
	}
	| %empty
	{
		$$ = make_STM_LIST_empty();
	}
;
*/


statement : tRETURN exp ';'
	{
		$$ = make_STM_return($2);
	}
	| tPRINT exp ';'
	{
		$$ = make_STM_printf($2);
	}
	| tALLOCATE variable ';'
	{
		$$ = make_STM_allocate($2);
	}
	| tALLOCATE variable tOF_LENGTH exp ';'
	{
		$$ = make_STM_allocate_of_length($2, $4);
	}
	| variable '=' exp ';'
	{
		$$ = make_STM_assign($1, $3, 0);
	}
	| variable tPLUS_EQUAL exp ';'
	{
		$$ = make_STM_assign($1, $3, 1); //variant 1
	}
	| variable tMINUS_EQUAL exp ';'
	{
		$$ = make_STM_assign($1, $3, 2); //variant 2
	}
	| variable tTIMES_EQUAL exp ';'
	{
		$$ = make_STM_assign($1, $3, 3); //variant 3
	}
	| variable tDIVIDE_EQUAL exp ';'
	{
		$$ = make_STM_assign($1, $3, 4); //variant 4
	}
	| tIF exp statement
	{
		$$ =  make_STM_if($2, $3);
	}
	| tIF exp statement tELSE statement
	{
		$$ = make_STM_if_else($2, $3, $5);
	}
	| tWHILE exp statement
	{
		$$ = make_STM_while($2, $3);
	}
	|'{' statement_list '}'
	{
		$$ = make_STM_list($2);
	}
;


variable : tIDENTIFIER
	{
		$$ = make_VARIABLE($1);
	}
	| variable '[' exp ']'
	{
		$$ =  make_VARIABLE_array_access($1, $3);
	}
	| variable '.' tIDENTIFIER
	{
		$$ = make_VARIABLE_access($1, $3);
	}
;


exp : exp '*' exp
	{
		$$ = make_EXP_times($1,$3);
	}
	| exp '/' exp
	{
		$$ = make_EXP_div($1,$3);
	}
	| exp '+' exp
	{
		$$ = make_EXP_plus($1,$3);
	}
	| exp '-' exp
	{
		$$ = make_EXP_minus($1,$3);
	}
	| exp tEQUALITY exp
	{
		$$ = make_EXP_equality($1, $3);
	}
	| exp tOR exp
	{
		$$ = make_EXP_or($1, $3);
	}
	| exp tAND exp
	{
		$$ = make_EXP_and($1, $3);
	}
	| exp tNON_EQUALITY exp
	{
		$$ = make_EXP_non_equality($1, $3);
	}
	| exp tGREATER_EQUAL exp
	{
		$$ = make_EXP_greater_equal($1, $3);
	}
	| exp tLESSER_EQUAL exp
	{
		$$ = make_EXP_lesser_equal($1, $3);
	}
	| exp '>' exp
	{
		$$ = make_EXP_greater($1, $3);
	}
	| exp '<' exp
	{
		$$ = make_EXP_lesser($1, $3);
	}
	| term
	{
		$$ = make_EXP_term($1);
	}
;


term : variable
	{
		$$ = make_TERM_variable($1);
	}
	| tIDENTIFIER '(' act_list ')'
	{
		$$ = make_TERM_id_act_list($1, $3);
	}
	| '(' exp ')'
	{
		$$ = make_TERM_expression($2);
	}
	| '!' term
	{
		$$ = make_TERM_negate($2);
	}
	| '|' exp '|'
	{
		$$ = make_TERM_absolute($2);
	}
	| tINTCONST
	{
		$$ = make_TERM_num($1);
	}
	| tTRUE
	{
		$$ = make_TERM_true();
	}
	| tFALSE
	{
		$$ = make_TERM_false();
	}
	| tNULL
	{
		$$ = make_TERM_null();
	}
	| string
	{
		$$ = make_TERM_string($1);
	}
;


act_list : exp_list
	{
		$$ = make_ACT_LIST_exp_list($1);
	}
	| %empty
	{
		$$ = make_ACT_LIST_empty();
	}
;


exp_list : exp
	{
		$$ = make_EXP_LIST_expression($1);
	}
	| exp ',' exp_list
	{
		$$ = make_EXP_LIST_pair($1, $3);
	}
;

property : tSOA
	{
		current_property = PROPERTY::SOA;
	}
	| %empty
	{
	}
;


%%