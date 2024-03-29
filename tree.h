#ifndef __tree_h
#define __tree_h
#include "symbol.h"
#include <stddef.h>
#include "array.h"

enum class PROPERTY
{
	NONE,
	SOA,
	AOS,
	ALWAYS_GLOBAL,
	ATTACHED,
};

/* =============== AST_NODE START =============== */
typedef struct AST_NODE
{
	struct DIRECTIVE_LIST* directive_list;
	struct BODY* body;
}AST_NODE;
AST_NODE* make_AST(struct DIRECTIVE_LIST* directive_list, struct BODY* body);
/* =============== AST_NODE END =============== */
/* =============== DIRECTIVE START =============== */
typedef struct DIRECTIVE_LIST
{
	enum
	{
		pair, empty,
	} kind;

	int lineno;

	union
	{
		struct { struct DIRECTIVE_LIST* directive_list; struct DIRECTIVE* directive; } pair;
	} val;

	struct type_info_imp* type;
	struct code_info* cg;
} DIRECTIVE_LIST;
DIRECTIVE_LIST* make_DIRECTIVE_LIST_pair(struct DIRECTIVE_LIST* directive_list, struct DIRECTIVE* directive);
DIRECTIVE_LIST* make_DIRECTIVE_LIST_empty();
/* =============== DIRECTIVE END =============== */
/* =============== DIRECTIVE START =============== */
typedef struct DIRECTIVE
{
	enum
	{
		load, contain, build, shader,
	} kind;

	int lineno;

	union
	{
		struct { char* load_string; size_t string_length; } load;
		struct { char* contain_string; size_t string_length; } contain;
		struct { char* build_id; size_t string_length; struct ACT_LIST* act_list; } build;
		struct
		{
			char* shader_id; size_t shader_id_length; char* shader_filepath; size_t shader_filepath_length;
		} shader;
	} val;

	struct type_info_imp* type;
	struct code_info* cg;
} DIRECTIVE;
DIRECTIVE* make_DIRECTIVE_load(char* string);
DIRECTIVE* make_DIRECTIVE_contain(char* string);
DIRECTIVE* make_DIRECTIVE_build(char* id, struct ACT_LIST* act_list);
DIRECTIVE* make_DIRECTIVE_shader(char* id, char* filepath);
/* =============== DIRECTIVE END =============== */
/* =============== STRING START =============== */
typedef struct STRING
{
	enum
	{
		stringK, string_concatenationK,
	} kind;

	int lineno;
	char* str;
	size_t length;
	//TODO: unsigned int hash_code; we can store the hash of a string, then use it to compare two strings?
	//or use it to for object orientation?
	struct type_info_imp* type;
	struct code_info* cg;
} STRING;
STRING* make_STRING(char* string);
STRING* make_STRING_concatenation(char* a, char* b);
/* =============== FUNCTION END ================ */

/* =============== FUNCTION START =============== */
typedef struct FUNCTION
{
	int lineno;
	struct HEAD *func_head;
	struct BODY *func_body;

	SYMBOL* symbol_entry;
	struct type_info_imp* type;
	SymbolTable* scope; //the scope that the function creates is stored here, for easy and fast retrieval.
	struct code_info* cg;
	struct call_info* call_info;


	array* local_variables;
	int STACK_ALLIGNMENT_COUNT = 0; //the count is how many times we push onto the stack and pop from the stack.
	int STACK_ALLIGNMENT_VALUE = 0; //the value contains the offset of the stack. 8, 16, 24 etc.. this should support 32-bit and 64-bit in the future.

} FUNCTION;

FUNCTION* make_FUNC(struct HEAD* head, struct BODY* body);
/* =============== FUNCTION END ================ */

/* =============== HEAD START =============== */
typedef struct HEAD
{
	int lineno;
	char* function_id;
	struct PAR_DECL_LIST* params;
	struct TYPE* return_type;
	struct type_info_imp* type;
	struct code_info* cg;
} HEAD;

HEAD* make_HEAD(char* func_id, struct PAR_DECL_LIST* params, struct TYPE* type);
/* =============== HEAD END ================ */


/* =============== TYPE START =============== */
typedef struct TYPE
{
	int lineno;
	enum
	{
		type_idK, type_intK, type_boolK, type_arrayK, type_recordK, type_stringK, type_empty,
		type_byte,
		type_short,
		type_long,
		type_float,
		type_double,
		type_u8,
		type_u16,
		type_u32,
		type_u64,
		type_u128,
		type_u256,
		type_u512,
		type_bitset,
		type_union,
		type_void,
	} kind;

	union
	{
		char* type_id; // id
		struct STRING* string;
		struct TYPE* type; //array of type
		struct VAR_DECL_LIST* var_decl_list; //record of
	} val;

	int array_length;

	PROPERTY property;
	struct type_info_imp* ti;
	struct code_info* cg;
} TYPE;

TYPE* make_TYPE_id(char* type_id);
TYPE* make_TYPE_int();
TYPE* make_TYPE_bool();
TYPE* make_TYPE_string();
TYPE* make_TYPE_array(int length, struct TYPE* type);
TYPE* make_TYPE_record(struct VAR_DECL_LIST* var_decl_list, PROPERTY property);
TYPE* make_TYPE_empty();
TYPE* make_TYPE_byte();
TYPE* make_TYPE_short();
TYPE* make_TYPE_long();
TYPE* make_TYPE_float();
TYPE* make_TYPE_double();
TYPE* make_TYPE_u8();
TYPE* make_TYPE_u16();
TYPE* make_TYPE_u32();
TYPE* make_TYPE_u64();
TYPE* make_TYPE_u128();
TYPE* make_TYPE_u256();
TYPE* make_TYPE_u512();
TYPE* make_TYPE_bitset();
TYPE* make_TYPE_union(struct VAR_DECL_LIST* var_decl_list);
TYPE* make_TYPE_void();



/* =============== TYPE END ================ */


/* =============== PAR_DECL_LIST START =============== */
typedef struct PAR_DECL_LIST
{
	int lineno;
	enum
	{
		par_decl_list_var_decl_listK, par_decl_list_emptyK,
	} kind;

	union
	{
		struct VAR_DECL_LIST* var_decl_list;
	} val;
	struct type_info_imp* type;
	struct code_info* cg;
	array* list_of_var_types;
} PAR_DECL_LIST;

PAR_DECL_LIST* make_PAR_DECL_LIST(struct VAR_DECL_LIST* var_decl_list);
PAR_DECL_LIST* make_PAR_DECL_LIST_empty();
/* =============== PAR_DECL_LIST END ================ */



/* =============== VAR_DECL_LIST START =============== */
typedef struct VAR_DECL_LIST
{
	int lineno;
	enum
	{
		var_decl_list_pairK, var_decl_var_typeK
	} kind;

	union
	{
		struct { struct VAR_TYPE* head; struct VAR_DECL_LIST* tail; } pair;
		struct VAR_TYPE* var_type;
	} val;
	//array containing type_info_imp*
	array* types;
	struct type_info_imp* type;
	struct code_info* cg;
} VAR_DECL_LIST;

VAR_DECL_LIST* make_VAR_DECL_LIST_pair(struct VAR_TYPE *head, VAR_DECL_LIST *tail);
VAR_DECL_LIST* make_VAR_DECL_LIST_last(struct VAR_TYPE *var_type);
/* =============== VAR_DECL_LIST END ================ */



/* =============== VAR_TYPE START =============== */
typedef struct VAR_TYPE
{
	int lineno;
	char* id;
	TYPE * type;
	struct type_info_imp* ti;
	struct code_info* cg;
} VAR_TYPE;


//VAR_TYPE* make_VAR_TYPE(char* var_id, struct TYPE* type);
VAR_TYPE* make_VAR_TYPE(char* var_id, struct TYPE* type);
/* =============== VAR_TYPE END ================ */


/* =============== BODY START =============== */
typedef struct BODY
{
	int lineno;
	struct DECL_LIST* decl_list;
	struct STM_LIST* stm_list;
	struct type_info_imp* ti;
	struct code_info* cg;

	array* array_of_declarations;
} BODY;

BODY* make_BODY(struct DECL_LIST* decl_list, struct STM_LIST* stm_list);
/* =============== BODY END ================ */



/* =============== BODY_DECL_LIST START =============== */
typedef struct BODY_DECL_LIST
{
	int lineno;
	enum
	{
		body_decl_list_list, body_decl_list_empty,
	}kind;

	struct DECL_LIST *decl_list;
	struct type_info_imp* type;
	struct code_info* cg;
} BODY_DECL_LIST;

BODY_DECL_LIST* make_BODY_DECL_LIST_list(struct DECL_LIST* list);
BODY_DECL_LIST* make_BODY_DECL_LIST_empty();
/* =============== DECL_LIST END ================ */

/* =============== DECL_LIST START =============== */
typedef struct DECL_LIST
{
	int lineno;
	enum
	{
		decl_list_pair, decl_list_empty, //decl_list_decl,
	}kind;

	union
	{
		struct { struct DECL* head; struct DECL_LIST *next; } pair;
		struct DECL* decl;
	} val;

	struct type_info_imp* type;
	struct code_info* cg;
} DECL_LIST;

DECL_LIST* make_DECL_LIST_pair(struct DECL* head, struct DECL_LIST* next);
//DECL_LIST* make_DECL_LIST_decl(struct DECL* decl);
DECL_LIST* make_DECL_LIST_empty();
/* =============== DECL_LIST END ================ */


/* =============== DECL START =============== */
typedef struct DECL
{
	int lineno;
	enum
	{
		decl_assignK, decl_functionK, decl_varsK, decl_var_typ_exp, decl_struct,
	} kind;

	char* decl_id;
	struct VAR_TYPE* var_type;
	struct EXP* exp;
	struct BODY* body;
	union
	{
		struct TYPE* type;
		struct FUNCTION* function;
		struct VAR_DECL_LIST* var_decl_list;
	} val;

	struct type_info_imp* type;
	struct code_info* cg;
} DECL;

DECL* make_DECL_assignment(char* id, TYPE* type);
DECL* make_DECL_func(FUNCTION* func);
DECL* make_DECL_vars(struct VAR_DECL_LIST* var_list);
DECL* make_DECL_var_type_exp(struct VAR_TYPE* var_type, struct EXP* exp);
DECL* make_DECL_struct(char* id, struct BODY* body);
/* =============== DECL END ================ */

/* =============== STM_LIST START =============== */
typedef struct STM_LIST
{
	int lineno;
	enum
	{
		stm_list_pairK, stm_list_statementK, stm_list_empty,
	} kind;
	union
	{
		struct { struct STM* head;	struct STM_LIST* tail; } pair;
		struct STM* stm;
	} val;

	struct type_info_imp* type;
	struct code_info* cg;
} STM_LIST;

STM_LIST* make_STM_LIST_statement(struct STM* stm);
STM_LIST* make_STM_LIST_empty();
STM_LIST* make_STM_LIST_pair(struct STM* stm, struct STM_LIST* stmList);
/* =============== STM_LIST END ================ */


/* =============== STM START =============== */
typedef struct STM
{
	int lineno;
	enum
	{
		stm_returnK, stm_printf, stm_allocateK, stm_allocate_lengthK,
		stm_assignmentK, stm_assignment_selfK, stm_ifK, stm_if_elseK, stm_whileK, stm_listK,
		stm_var_type_exp, stm_for,
	} kind;

	//syntactic sugar
	enum
	{
		//THESE ARE UNDER stm_assignmentK and is implemented in make_STM_assign
		assignment_variant_NONE = 0,
		assignment_variant_plus_equal = 1,
		assignment_variant_minus_equal = 2,
		assignment_variant_times_equal = 3,
		assignment_variant_divide_equal = 4,

		//THESE ARE UNDER stm_assignment_selfK and is implemented in make_STM_assign_self
		assignment_variant_pre_plusplus = 5,
		assignment_variant_post_plusplus = 6,
		assignment_variant_pre_minusminus = 7,
		assignment_variant_post_minusminus = 8,
	} assignment_variant;
	union
	{
		struct VAR_TYPE* var_type;
		struct EXP *exp;
		struct VARIABLE* var;
		struct { struct VARIABLE* var; struct EXP* exp; } allocate_length;
		struct { struct VARIABLE* var; struct EXP* exp; } assignment;
		struct { struct EXP *exp; struct STM *stm; } if_stm;
		struct { struct EXP *exp; struct STM *stm_then; struct STM *stm_else; } if_else_stm;
		struct { struct EXP *exp; struct STM *stm; } while_stm;
		struct { struct VAR_TYPE* var_type;  struct EXP* exp; struct STM* stm; struct BODY* body; } for_stm;
		struct STM_LIST *list;
	} val;
	struct type_info_imp* type;
	struct code_info* cg;
} STM;


STM* make_STM_return(struct EXP* exp);
STM* make_STM_printf(struct EXP* exp);
STM* make_STM_allocate(struct VARIABLE* var);
STM* make_STM_allocate_of_length(struct VARIABLE* var, struct EXP* exp);
STM* make_STM_assign(struct VARIABLE* var, struct EXP* exp, int variant);
STM* make_STM_assign_self(struct VARIABLE* var, int variant);
STM* make_STM_if(struct EXP* exp, STM* stm);
STM* make_STM_if_else(struct EXP* exp, STM* stm_then, STM* stm_else);
STM* make_STM_list(STM_LIST* list);
STM* make_STM_while(struct EXP* exp, struct STM* stm);
STM* make_STM_ENHANCED_FOR_LOOP();
STM* make_STM_var_type_exp(struct VAR_TYPE* var_type, struct EXP* exp);
STM* make_STM_FOR_LOOP(struct VAR_TYPE* var_type, struct EXP* exp, struct STM* statement, struct BODY* body);

/* =============== STM END ================ */





/* =============== VARIABLE START =============== */
typedef struct VARIABLE
{
	int lineno;
	enum
	{
		variable_idK, variable_arrayAccessK, variable_access_idK
	} kind;
	union
	{
		char *id;
		struct { struct VARIABLE* var; struct EXP* exp; } array_access;
		struct { struct VARIABLE* var; char* id; } access;
	} val;
	struct type_info_imp* type;
	struct code_info* cg;
} VARIABLE;

VARIABLE* make_VARIABLE(char* id);
VARIABLE* make_VARIABLE_array_access(struct VARIABLE* var, struct EXP* exp);
VARIABLE* make_VARIABLE_access(struct VARIABLE* var, char* id);
/* =============== VARIABLE END ================ */


/* =============== EXP START =============== */
typedef struct EXP
{
	int lineno;
	enum
	{
		exp_timesK, exp_divK, exp_plusK, exp_minusK,
		exp_equalityK, exp_non_equalityK, exp_greaterK, exp_lesserK, exp_greater_equalK,
		exp_lesser_equalK, exp_andK, exp_orK, exp_termK
	} kind;
	union
	{
		struct { struct EXP *left; struct EXP *right; } op;
		struct TERM* term;
	} val;
	struct type_info_imp* type;
	struct code_info* cg;
} EXP;

/*EXP arithmetic functions */
EXP *make_EXP_times(EXP *left, EXP *right);
EXP *make_EXP_div(EXP *left, EXP *right);
EXP *make_EXP_plus(EXP *left, EXP *right);
EXP *make_EXP_minus(EXP *left, EXP *right);

/*EXP logic functions*/
EXP *make_EXP_equality(EXP *left, EXP *right);
EXP *make_EXP_non_equality(EXP *left, EXP *right);
EXP *make_EXP_greater(EXP *left, EXP *right);
EXP *make_EXP_lesser(EXP *left, EXP *right);
EXP *make_EXP_greater_equal(EXP *left, EXP *right);
EXP *make_EXP_lesser_equal(EXP *left, EXP *right);
EXP *make_EXP_and(EXP *left, EXP *right);
EXP *make_EXP_or(EXP *left, EXP *right);

EXP *make_EXP_term(struct TERM *term);
/* =============== EXP END =============== */



/* =============== TERM START =============== */
typedef struct TERM
{
	int lineno;
	enum
	{
		term_varK, term_id_act_listK, term_parenthesis_expressionK, term_negationK,
		term_absoluteK, term_numK, term_trueK, term_falseK, term_nullK, term_stringK,
	} kind;

	union
	{
		//varK
		struct VARIABLE* var;

		//identK
		char* id;
		struct { char* id; struct ACT_LIST* act_list; } id_act_list;

		//USING exp for absolute and for parenthesis.
		struct EXP* exp;
		int boolean;
		struct STRING* string;

		struct TERM* term;
		int num;
	} val;

	struct type_info_imp* type;
	struct code_info* cg;
	struct call_info* call_info;
} TERM;

TERM* make_TERM_variable(VARIABLE* var);
TERM* make_TERM_id_act_list(char* id, struct ACT_LIST* act_list);
TERM* make_TERM_expression(EXP* exp);
TERM* make_TERM_negate(TERM* term);
TERM* make_TERM_absolute(EXP* exp);
TERM* make_TERM_num(int intconst);
TERM* make_TERM_true();
TERM* make_TERM_false();
TERM* make_TERM_null();
TERM* make_TERM_string(struct STRING* string);
/* =============== TERM END =============== */


/* =============== ACT_LIST START =============== */
typedef struct ACT_LIST
{
	int lineno;
	enum
	{
		act_list_exp_listK, act_list_emptyK
	} kind;

	struct EXP_LIST *exp_list;
	struct type_info_imp* type;
	struct code_info* cg;
	array* list_of_code_infos;
} ACT_LIST;

ACT_LIST* make_ACT_LIST_exp_list(struct EXP_LIST* exp_list);
ACT_LIST* make_ACT_LIST_empty();
/* =============== ACT_LIST END =============== */


/* =============== EXP_LIST START =============== */
typedef struct EXP_LIST
{
	int lineno;

	enum
	{
		exp_list_pairK, exp_list_expressionK
	} kind;

	union
	{
		struct { EXP* head; 	struct EXP_LIST *tail; } pair;
		EXP* exp;
	} val;

	struct type_info_imp* type;
	struct code_info* cg;
}EXP_LIST;

EXP_LIST* make_EXP_LIST_expression(EXP* exp);
EXP_LIST* make_EXP_LIST_pair(EXP* head, EXP_LIST* tail);
/* =============== EXP_LIST END =============== */


#endif
