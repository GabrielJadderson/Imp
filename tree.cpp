#include "memory.h"
#include "tree.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "logger.h"
#include <string>

extern char FALCON_DEBUG_PARSER;

extern int lineno;

/* =============== AST_NODE START =============== */
AST_NODE* make_AST(struct DIRECTIVE_LIST* directive_list, struct BODY* body)
{
	if (FALCON_DEBUG_PARSER)
		printf("Made <ast_node> at line:%d\n", lineno);
	AST_NODE * e = NEW(AST_NODE);
	e->directive_list = directive_list;
	e->body = body;
	return e;
}
/* =============== AST_NODE END =============== */
/* =============== DIRECTIVE_LIST START =============== */
DIRECTIVE_LIST* make_DIRECTIVE_LIST_pair(struct DIRECTIVE_LIST* directive_list, struct DIRECTIVE* directive)
{
	if (FALCON_DEBUG_PARSER)
		printf("Made <directive> load at line:%d\n", lineno);
	DIRECTIVE_LIST * e = NEW(DIRECTIVE_LIST);
	e->lineno = lineno;
	e->kind = DIRECTIVE_LIST::pair;
	e->val.pair.directive_list = directive_list;
	e->val.pair.directive = directive;
	return e;
}
DIRECTIVE_LIST* make_DIRECTIVE_LIST_empty()
{
	if (FALCON_DEBUG_PARSER)
		printf("Made <directive> load at line:%d\n", lineno);
	DIRECTIVE_LIST * e = NEW(DIRECTIVE_LIST);
	e->lineno = lineno;
	e->kind = DIRECTIVE_LIST::empty;
	return e;

}
/* =============== DIRECTIVE_LIST END =============== */
/* =============== DIRECTIVE START =============== */
DIRECTIVE* make_DIRECTIVE_load(char* string)
{
	if (FALCON_DEBUG_PARSER)
		printf("Made <directive> load at line:%d\n", lineno);
	DIRECTIVE * e = NEW(DIRECTIVE);
	e->lineno = lineno;
	e->kind = DIRECTIVE::load;
	e->val.load.load_string = string;
	e->val.load.string_length = strlen(string);
	return e;
}
DIRECTIVE* make_DIRECTIVE_contain(char* string)
{
	if (FALCON_DEBUG_PARSER)
		printf("Made <directive> load at line:%d\n", lineno);
	DIRECTIVE * e = NEW(DIRECTIVE);
	e->lineno = lineno;
	e->kind = DIRECTIVE::contain;
	e->val.contain.contain_string = string;
	e->val.contain.string_length = strlen(string);
	return e;
}
DIRECTIVE* make_DIRECTIVE_build(char* id, struct ACT_LIST* act_list)
{
	if (FALCON_DEBUG_PARSER)
		printf("Made <directive> load at line:%d\n", lineno);
	DIRECTIVE * e = NEW(DIRECTIVE);
	e->lineno = lineno;
	e->kind = DIRECTIVE::build;
	e->val.build.build_id = id;
	e->val.build.string_length = strlen(id);
	e->val.build.act_list = act_list;
	return e;
}
DIRECTIVE* make_DIRECTIVE_shader(char* id, char* filepath)
{
	if (FALCON_DEBUG_PARSER)
		printf("Made <directive> load at line:%d\n", lineno);
	DIRECTIVE * e = NEW(DIRECTIVE);
	e->lineno = lineno;
	e->kind = DIRECTIVE::shader;
	e->val.shader.shader_id = id;
	e->val.shader.shader_id_length = strlen(id);
	e->val.shader.shader_filepath = filepath;
	e->val.shader.shader_filepath_length = strlen(filepath);
	return e;
}
/* =============== DIRECTIVE END =============== */

/* =============== STRING START =============== */
STRING* make_STRING(char* string)
{
	if (FALCON_DEBUG_PARSER)
		printf("Made <string> \"%s\" of length: %zu at line:%d\n", string, strlen(string), lineno);
	STRING * e = NEW(STRING);
	e->lineno = lineno;
	e->kind = STRING::stringK;
	e->str = string;
	e->length = strlen(string);
	return e;
}
STRING* make_STRING_concatenation(char* a, char* b)
{
	//quick and dirty:
	std::string string_a(a);
	std::string string_b(b);
	std::string string_c = string_a + string_b;

	size_t length = strlen(string_c.c_str());

	char* ptr = (char*)Calloc(string_c.length() + 1, sizeof(char)); // +1 for the null character.
	strcpy_s(ptr, length, string_c.c_str());


	STRING * e = NEW(STRING);
	e->lineno = lineno;
	e->str = ptr;
	e->length = length;
	e->kind = STRING::stringK;
	if (FALCON_DEBUG_PARSER)
		printf("%s\n", ptr);
	return e;


	/*
	size_t length_a = strlen(a);
	size_t length_b = strlen(b);
	size_t total_length = length_a + length_b;
	char* ptr = (char*)Calloc(total_length + 1, sizeof(char)); // +1 for the null character.
	if (!ptr) { printf("FAILED TO ALLOCATE SPACE FOR STRING CONCATENATION\n"); exit(1); }
	strcpy(ptr, a); //first we copy the contents of a into the new space
	strcat(ptr, b); //then we use strcat to append b after we've added a.

	if (FALCON_DEBUG_PARSER)
		printf("Made <string> concatenation of \"%s\" + \"%s\" of length: %zu, %zu at line:%d into -> ", a, b, length_a, length_b, lineno);
	//free both a and b, since we allocate them in flex during the scanning phase.
	free(a);
	free(b);

	STRING * e = NEW(STRING);
	e->lineno = lineno;
	e->str = ptr;
	e->length = total_length;
	e->kind = STRING::string_concatenationK;
	if (FALCON_DEBUG_PARSER)
		printf("%s\n", ptr);
	return e;
	*/
}
/* =============== STRING END ================ */



/* =============== FUNCTION START =============== */
FUNCTION * make_FUNC(struct HEAD * head, struct BODY * body)
{
	if (FALCON_DEBUG_PARSER)
		printf("Made <function> at line:%d\n", lineno);
	FUNCTION * e = NEW(FUNCTION);
	e->lineno = lineno;
	e->func_head = head;
	e->func_body = body;
	return e;
}
/* =============== FUNCTION END ================ */


/* =============== HEAD START =============== */
HEAD* make_HEAD(char* func_id, struct PAR_DECL_LIST * params, struct TYPE * type)
{
	if (FALCON_DEBUG_PARSER)
		printf("Made <head> of id: %s -> at line:%d\n", func_id, lineno);
	HEAD * e = NEW(HEAD);
	e->lineno = lineno;
	e->function_id = func_id;
	e->params = params;
	e->return_type = type;
	return e;
}
/* =============== HEAD END ================ */


/* =============== TYPE START =============== */
TYPE* make_TYPE_id(char* type_id)
{
	if (FALCON_DEBUG_PARSER)
		printf("Made <type> of id: %s -> at line:%d\n", type_id, lineno);
	TYPE * e = NEW(TYPE);
	e->lineno = lineno;
	e->kind = TYPE::type_idK;
	e->val.type_id = type_id;
	return e;
}
TYPE* make_TYPE_int()
{
	if (FALCON_DEBUG_PARSER)
		printf("Made <type> of int at line: %d\n", lineno);
	TYPE * e = NEW(TYPE);
	e->lineno = lineno;
	e->kind = TYPE::type_intK;
	return e;
}
TYPE* make_TYPE_bool()
{
	if (FALCON_DEBUG_PARSER)
		printf("Made <type> of bool at line: %d\n", lineno);
	TYPE * e = NEW(TYPE);
	e->lineno = lineno;
	e->kind = TYPE::type_boolK;
	return e;
}
TYPE* make_TYPE_string()
{
	if (FALCON_DEBUG_PARSER)
		printf("Made <type> of string at line: %d\n", lineno);
	TYPE * e = NEW(TYPE);
	e->lineno = lineno;
	e->kind = TYPE::type_stringK;
	return e;
}

TYPE* make_TYPE_array(int length, struct TYPE * type)
{
	if (FALCON_DEBUG_PARSER)
		printf("Made <type> of array of <type->kind = %d> at line: %d\n", type->kind, lineno);
	TYPE * e = NEW(TYPE);
	e->lineno = lineno;
	e->kind = TYPE::type_arrayK;
	e->val.type = type;
	e->array_length = length;
	return e;
}
TYPE* make_TYPE_record(struct VAR_DECL_LIST * var_decl_list, PROPERTY property)
{
	if (FALCON_DEBUG_PARSER)
		printf("Made <type> record of ... at line: %d\n", lineno);
	TYPE * e = NEW(TYPE);
	e->lineno = lineno;
	e->kind = TYPE::type_recordK;
	e->val.var_decl_list = var_decl_list;
	e->property = property;
	return e;
}

TYPE* make_TYPE_empty()
{
	if (FALCON_DEBUG_PARSER)
		printf("Made empty <type> at line: %d\n", lineno);
	TYPE * e = NEW(TYPE);
	e->lineno = lineno;
	e->kind = TYPE::type_empty;
	return e;
}

TYPE* make_TYPE_byte()
{
	if (FALCON_DEBUG_PARSER)
		printf("Made byte <type> at line: %d\n", lineno);
	TYPE * e = NEW(TYPE);
	e->lineno = lineno;
	e->kind = TYPE::type_byte;
	return e;
}
TYPE* make_TYPE_short()
{
	if (FALCON_DEBUG_PARSER)
		printf("Made short <type> at line: %d\n", lineno);
	TYPE * e = NEW(TYPE);
	e->lineno = lineno;
	e->kind = TYPE::type_short;
	return e;
}
TYPE* make_TYPE_long()
{
	if (FALCON_DEBUG_PARSER)
		printf("Made long <type> at line: %d\n", lineno);
	TYPE * e = NEW(TYPE);
	e->lineno = lineno;
	e->kind = TYPE::type_long;
	return e;
}
TYPE* make_TYPE_float()
{
	if (FALCON_DEBUG_PARSER)
		printf("Made float <type> at line: %d\n", lineno);
	TYPE * e = NEW(TYPE);
	e->lineno = lineno;
	e->kind = TYPE::type_float;
	return e;
}
TYPE* make_TYPE_double()
{
	if (FALCON_DEBUG_PARSER)
		printf("Made double <type> at line: %d\n", lineno);
	TYPE * e = NEW(TYPE);
	e->lineno = lineno;
	e->kind = TYPE::type_double;
	return e;
}
TYPE* make_TYPE_u8()
{
	if (FALCON_DEBUG_PARSER)
		printf("Made u8 <type> at line: %d\n", lineno);
	TYPE * e = NEW(TYPE);
	e->lineno = lineno;
	e->kind = TYPE::type_u8;
	return e;
}
TYPE* make_TYPE_u16()
{
	if (FALCON_DEBUG_PARSER)
		printf("Made u16 <type> at line: %d\n", lineno);
	TYPE * e = NEW(TYPE);
	e->lineno = lineno;
	e->kind = TYPE::type_u16;
	return e;
}
TYPE* make_TYPE_u32()
{
	if (FALCON_DEBUG_PARSER)
		printf("Made u32 <type> at line: %d\n", lineno);
	TYPE * e = NEW(TYPE);
	e->lineno = lineno;
	e->kind = TYPE::type_u32;
	return e;
}
TYPE* make_TYPE_u64()
{
	if (FALCON_DEBUG_PARSER)
		printf("Made u64 <type> at line: %d\n", lineno);
	TYPE * e = NEW(TYPE);
	e->lineno = lineno;
	e->kind = TYPE::type_u64;
	return e;
}
TYPE* make_TYPE_u128()
{
	if (FALCON_DEBUG_PARSER)
		printf("Made u128 <type> at line: %d\n", lineno);
	TYPE * e = NEW(TYPE);
	e->lineno = lineno;
	e->kind = TYPE::type_u128;
	return e;
}
TYPE* make_TYPE_u256()
{
	if (FALCON_DEBUG_PARSER)
		printf("Made u256 <type> at line: %d\n", lineno);
	TYPE * e = NEW(TYPE);
	e->lineno = lineno;
	e->kind = TYPE::type_u256;
	return e;
}
TYPE* make_TYPE_u512()
{
	if (FALCON_DEBUG_PARSER)
		printf("Made u512 <type> at line: %d\n", lineno);
	TYPE * e = NEW(TYPE);
	e->lineno = lineno;
	e->kind = TYPE::type_u512;
	return e;
}
TYPE* make_TYPE_bitset()
{
	if (FALCON_DEBUG_PARSER)
		printf("Made bitset <type> at line: %d\n", lineno);
	TYPE * e = NEW(TYPE);
	e->lineno = lineno;
	e->kind = TYPE::type_bitset;
	return e;
}
TYPE* make_TYPE_union(struct VAR_DECL_LIST* var_decl_list)
{
	if (FALCON_DEBUG_PARSER)
		printf("Made union <type> at line: %d\n", lineno);
	TYPE * e = NEW(TYPE);
	e->lineno = lineno;
	e->kind = TYPE::type_union;
	e->val.var_decl_list = var_decl_list;
	return e;
}
TYPE* make_TYPE_void()
{
	if (FALCON_DEBUG_PARSER)
		printf("Made void <type> at line: %d\n", lineno);
	TYPE * e = NEW(TYPE);
	e->lineno = lineno;
	e->kind = TYPE::type_void;
	return e;
}
/* =============== TYPE END ================ */


/* =============== PAR_DECL_LIST START =============== */
PAR_DECL_LIST* make_PAR_DECL_LIST_empty()
{
	if (FALCON_DEBUG_PARSER)
		printf("Made <par_decl_list> empty at line: %d\n", lineno);
	PAR_DECL_LIST * e = NEW(PAR_DECL_LIST);
	e->lineno = lineno;
	e->kind = PAR_DECL_LIST::par_decl_list_emptyK;
	return e;
}
PAR_DECL_LIST* make_PAR_DECL_LIST(struct VAR_DECL_LIST * var_decl_list)
{
	if (FALCON_DEBUG_PARSER)
		printf("Made <par_decl_list> non-empty at line: %d\n", lineno);
	PAR_DECL_LIST * e = NEW(PAR_DECL_LIST);
	e->lineno = lineno;
	e->kind = PAR_DECL_LIST::par_decl_list_var_decl_listK;
	e->val.var_decl_list = var_decl_list;
	return e;
}
/* =============== PAR_DECL_LIST END ================ */


/* =============== VAR_DECL_LIST START =============== */
VAR_DECL_LIST* make_VAR_DECL_LIST_pair(struct VAR_TYPE * head, VAR_DECL_LIST * tail)
{
	if (FALCON_DEBUG_PARSER)
		printf("Made <var_decl_list> pair at line: %d\n", lineno);
	VAR_DECL_LIST * e = NEW(VAR_DECL_LIST);
	e->lineno = lineno;
	e->kind = VAR_DECL_LIST::var_decl_list_pairK;
	e->val.pair.head = head;
	e->val.pair.tail = tail;
	return e;
}
VAR_DECL_LIST* make_VAR_DECL_LIST_last(struct VAR_TYPE * var_type)
{
	if (FALCON_DEBUG_PARSER)
		printf("Made <var_decl_list> var at line: %d\n", lineno);
	VAR_DECL_LIST * e = NEW(VAR_DECL_LIST);
	e->lineno = lineno;
	e->kind = VAR_DECL_LIST::var_decl_var_typeK;
	e->val.var_type = var_type;
	return e;
}
/* =============== VAR_DECL_LIST END ================ */


/* =============== VAR_TYPE START =============== */
VAR_TYPE* make_VAR_TYPE(char* var_id, struct TYPE * type)
{
	if (FALCON_DEBUG_PARSER)
		printf("Made <var_type> at line: %d\n", lineno);
	VAR_TYPE * e = NEW(VAR_TYPE);
	e->lineno = lineno;
	e->id = var_id;
	e->type = type;
	return e;
}
/* =============== VAR_TYPE END ================ */


/* =============== BODY START =============== */
BODY* make_BODY(DECL_LIST * decl_list, STM_LIST * stm_list)
{
	if (FALCON_DEBUG_PARSER)
		printf("Made <body> at line: %d\n", lineno);
	BODY * e = NEW(BODY);
	e->lineno = lineno;
	e->decl_list = decl_list;
	e->stm_list = stm_list;
	return e;
}
/* =============== BODY END ================ */

/* =============== BODY_DECL_LIST START =============== */
BODY_DECL_LIST* make_BODY_DECL_LIST_list(struct DECL_LIST* list)
{
	if (FALCON_DEBUG_PARSER)
		printf("made <body_decl_list> at line: %d\n", lineno);
	BODY_DECL_LIST * e = NEW(BODY_DECL_LIST);
	e->lineno = lineno;
	e->kind = BODY_DECL_LIST::body_decl_list_list;
	e->decl_list = list;
	return e;
}
BODY_DECL_LIST* make_BODY_DECL_LIST_empty()
{
	if (FALCON_DEBUG_PARSER)
		printf("made <body_decl_list> at line: %d\n", lineno);
	BODY_DECL_LIST * e = NEW(BODY_DECL_LIST);
	e->lineno = lineno;
	e->kind = BODY_DECL_LIST::body_decl_list_empty;
	return e;
}
/* =============== BODY_DECL_LIST END =============== */
/* =============== DECL_LIST START =============== */
DECL_LIST* make_DECL_LIST_pair(DECL * head, DECL_LIST * next)
{
	if (FALCON_DEBUG_PARSER)
		printf("made <decl_list> at line: %d\n", lineno);
	DECL_LIST * e = NEW(DECL_LIST);
	e->lineno = lineno;
	e->kind = DECL_LIST::decl_list_pair;
	e->val.pair.head = head;
	e->val.pair.next = next;
	return e;
}
/*
DECL_LIST* make_DECL_LIST_decl(struct DECL* decl)
{
	if (FALCON_DEBUG_PARSER)
		printf("made <decl_list> decl at line: %d\n", lineno);
	DECL_LIST * e = NEW(DECL_LIST);
	e->lineno = lineno;
	e->kind = DECL_LIST::decl_list_decl;
	e->val.decl = decl;
	return e;
}
*/

DECL_LIST* make_DECL_LIST_empty()
{
	if (FALCON_DEBUG_PARSER)
		printf("made <decl_list> empty at line: %d\n", lineno);
	DECL_LIST * e = NEW(DECL_LIST);
	e->lineno = lineno;
	e->kind = DECL_LIST::decl_list_empty;
	return e;
}

/* =============== DECL_LIST END ================ */


/* =============== DECL START =============== */
DECL* make_DECL_assignment(char* id, TYPE * type)
{
	if (FALCON_DEBUG_PARSER)
		printf("made assignment <decl> at line: %d\n", lineno);
	DECL * e = NEW(DECL);
	e->lineno = lineno;
	e->kind = DECL::decl_assignK;
	e->decl_id = id;
	e->val.type = type;
	return e;
}
DECL* make_DECL_func(FUNCTION * func)
{
	if (FALCON_DEBUG_PARSER)
		printf("made func <decl> at line: %d\n", lineno);
	DECL * e = NEW(DECL);
	e->lineno = lineno;
	e->kind = DECL::decl_functionK;
	e->val.function = func;
	return e;
}
DECL* make_DECL_vars(VAR_DECL_LIST * var_list)
{
	if (FALCON_DEBUG_PARSER)
		printf("made var_decl_list <decl> at line: %d\n", lineno);
	DECL * e = NEW(DECL);
	e->lineno = lineno;
	e->kind = DECL::decl_varsK;
	e->val.var_decl_list = var_list;
	return e;
}

DECL* make_DECL_var_type_exp(struct VAR_TYPE* var_type, struct EXP* exp)
{
	if (FALCON_DEBUG_PARSER)
		printf("made var_decl_list <decl> at line: %d\n", lineno);
	DECL * e = NEW(DECL);
	e->lineno = lineno;
	e->kind = DECL::decl_var_typ_exp;
	e->var_type = var_type;
	e->exp = exp;
	return e;
}
DECL* make_DECL_struct(char* id, struct BODY* body)
{
	if (FALCON_DEBUG_PARSER)
		printf("made var_decl_list <decl> at line: %d\n", lineno);
	DECL * e = NEW(DECL);
	e->lineno = lineno;
	e->kind = DECL::decl_struct;
	e->decl_id = id;
	e->body = body;
	return e;
}
/* =============== DECL END ================ */


/* =============== STM_LIST START =============== */
STM_LIST* make_STM_LIST_statement(STM * stm)
{
	if (FALCON_DEBUG_PARSER)
		printf("Made a <stm_list> statement at line: %d\n", lineno);
	STM_LIST * e = NEW(STM_LIST);
	e->lineno = lineno;
	e->kind = STM_LIST::stm_list_statementK;
	e->val.stm = stm;
	return e;
}

STM_LIST* make_STM_LIST_pair(STM * stm, STM_LIST * stmList)
{
	if (FALCON_DEBUG_PARSER)
		printf("Made a <stm_list> pair at line: %d\n", lineno);
	STM_LIST * e = NEW(STM_LIST);
	e->lineno = lineno;
	e->kind = STM_LIST::stm_list_pairK;
	e->val.pair.head = stm;
	e->val.pair.tail = stmList;
	return e;
}
STM_LIST* make_STM_LIST_empty()
{
	if (FALCON_DEBUG_PARSER)
		printf("Made a <stm_list> pair at line: %d\n", lineno);
	STM_LIST * e = NEW(STM_LIST);
	e->lineno = lineno;
	e->kind = STM_LIST::stm_list_empty;
	return e;
}
/* =============== STM_LIST END ================ */


/* =============== STM START =============== */
STM* make_STM_return(EXP * exp)
{
	if (FALCON_DEBUG_PARSER)
		printf("Made a return <stm> at line: %d\n", lineno);
	STM * e = NEW(STM);
	e->lineno = lineno;
	e->kind = STM::stm_returnK;
	e->val.exp = exp;
	return e;
}
STM* make_STM_var_type_exp(struct VAR_TYPE* var_type, struct EXP* exp)
{
	if (FALCON_DEBUG_PARSER)
		printf("Made a var_type exp <stm> at line: %d\n", lineno);
	STM * e = NEW(STM);
	e->lineno = lineno;
	e->kind = STM::stm_var_type_exp;
	e->val.var_type = var_type;
	e->val.exp = exp;
	return e;
}
STM* make_STM_printf(EXP * exp)
{
	if (FALCON_DEBUG_PARSER)
		printf("Made a write <stm> at line: %d\n", lineno);
	STM * e = NEW(STM);
	e->lineno = lineno;
	e->kind = STM::stm_printf;
	e->val.exp = exp;
	return e;
}
STM* make_STM_allocate(struct VARIABLE * var)
{
	if (FALCON_DEBUG_PARSER)
		printf("Made an allocate <stm> at line: %d\n", lineno);
	STM * e = NEW(STM);
	e->lineno = lineno;
	e->kind = STM::stm_allocateK;
	e->val.var = var;
	return e;
}
STM* make_STM_allocate_of_length(struct VARIABLE * var, struct EXP * exp)
{
	if (FALCON_DEBUG_PARSER)
		printf("Made an allocate of length <stm> at line: %d\n", lineno);
	STM * e = NEW(STM);
	e->lineno = lineno;
	e->kind = STM::stm_allocate_lengthK;
	e->val.allocate_length.var = var;
	e->val.allocate_length.exp = exp;
	return e;
}
STM* make_STM_assign(struct VARIABLE * var, struct EXP * exp, int variant)
{
	if (FALCON_DEBUG_PARSER)
		printf("Made an assign <stm> at line: %d\n", lineno);
	STM * e = NEW(STM);
	e->lineno = lineno;
	e->kind = STM::stm_assignmentK;

	switch (variant)
	{
		case 0: { e->assignment_variant = STM::assignment_variant_NONE; } break;
		case 1: { e->assignment_variant = STM::assignment_variant_plus_equal; } break;
		case 2: { e->assignment_variant = STM::assignment_variant_minus_equal; }break;
		case 3: { e->assignment_variant = STM::assignment_variant_times_equal; }break;
		case 4: { e->assignment_variant = STM::assignment_variant_divide_equal; }break;
		default: { log_internal_error(__FILE__, __func__, __LINE__, "ERROR UNHANDLED VARIANT CASE FROM PARSER."); } break;
	}

	e->val.assignment.var = var;
	e->val.assignment.exp = exp;
	return e;
}
STM* make_STM_assign_self(struct VARIABLE * var, int variant)
{
	if (FALCON_DEBUG_PARSER)
		printf("Made an assign <stm> at line: %d\n", lineno);
	STM * e = NEW(STM);
	e->lineno = lineno;
	e->kind = STM::stm_assignment_selfK;
	switch (variant)
	{
		case 5: { e->assignment_variant = STM::assignment_variant_pre_plusplus; } break;
		case 6: { e->assignment_variant = STM::assignment_variant_post_plusplus; } break;
		case 7: { e->assignment_variant = STM::assignment_variant_pre_minusminus; }break;
		case 8: { e->assignment_variant = STM::assignment_variant_post_minusminus; }break;
		default: { log_internal_error(__FILE__, __func__, __LINE__, "ERROR UNHANDLED VARIANT CASE FROM PARSER."); } break;
	}
	e->val.var = var;
	return e;
}
STM* make_STM_if(EXP * exp, STM * stm)
{
	if (FALCON_DEBUG_PARSER)
		printf("Made a if <stm> at line: %d\n", lineno);
	STM * e = NEW(STM);
	e->lineno = lineno;
	e->kind = STM::stm_ifK;
	e->val.if_stm.exp = exp;
	e->val.if_stm.stm = stm;
	return e;
}
STM* make_STM_if_else(EXP * exp, STM * stm_then, STM * stm_else)
{
	if (FALCON_DEBUG_PARSER)
		printf("Made a if-else <stm> at line: %d\n", lineno);
	STM * e = NEW(STM);
	e->lineno = lineno;
	e->kind = STM::stm_if_elseK;
	e->val.if_else_stm.exp = exp;
	e->val.if_else_stm.stm_then = stm_then;
	e->val.if_else_stm.stm_else = stm_else;
	return e;
}
STM* make_STM_list(STM_LIST * list)
{
	if (FALCON_DEBUG_PARSER)
		printf("Made a <stm> list at line: %d\n", lineno);
	STM * e = NEW(STM);
	e->lineno = lineno;
	e->kind = STM::stm_listK;
	e->val.list = list;
	return e;
}
STM* make_STM_while(EXP * exp, STM * stm)
{
	if (FALCON_DEBUG_PARSER)
		printf("Made a <stm> while at line: %d\n", lineno);
	STM * e = NEW(STM);
	e->lineno = lineno;
	e->kind = STM::stm_whileK;
	e->val.while_stm.exp = exp;
	e->val.while_stm.stm = stm;
	return e;
}

STM* make_STM_FOR_LOOP(struct VAR_TYPE* var_type, struct EXP* exp, struct STM* statement, struct BODY* body)
{
	if (FALCON_DEBUG_PARSER)
		printf("Made a <stm> while at line: %d\n", lineno);
	STM * e = NEW(STM);
	e->lineno = lineno;
	e->kind = STM::stm_for;
	e->val.for_stm.var_type = var_type;
	e->val.for_stm.exp = exp;
	e->val.for_stm.stm = statement;
	e->val.for_stm.body = body;
	return e;
}
/* =============== STM END ================ */


/* =============== VARIABLE START =============== */
VARIABLE* make_VARIABLE(char* id)
{
	if (FALCON_DEBUG_PARSER)
		printf("Made a <variable> with id: %s at line: %d\n", id, lineno);
	VARIABLE * e = NEW(VARIABLE);
	e->lineno = lineno;
	e->kind = VARIABLE::variable_idK;
	e->val.id = id;
	return e;
}
VARIABLE* make_VARIABLE_array_access(struct VARIABLE * var, struct EXP * exp)
{
	if (FALCON_DEBUG_PARSER)
		printf("Made a <variable> with array access at line: %d\n", lineno);
	VARIABLE * e = NEW(VARIABLE);
	e->lineno = lineno;
	e->kind = VARIABLE::variable_arrayAccessK;
	e->val.array_access.var = var;
	e->val.array_access.exp = exp;
	return e;
}
VARIABLE* make_VARIABLE_access(struct VARIABLE * var, char* id)
{
	if (FALCON_DEBUG_PARSER)
		printf("=============== a Made a <variable> access with id: %s at line: %d\n", id, lineno);
	VARIABLE * e = NEW(VARIABLE);
	e->lineno = lineno;
	e->kind = VARIABLE::variable_access_idK;
	e->val.access.id = id;
	e->val.access.var = var;
	return e;
}
/* =============== VARIABLE END ================ */


/* =============== EXP START =============== */
EXP* make_EXP_times(EXP * left, EXP * right)
{
	if (FALCON_DEBUG_PARSER)
		printf("made * at line: %d\n", lineno);
	EXP * e = NEW(EXP);
	e->lineno = lineno;
	e->kind = EXP::exp_timesK;
	e->val.op.left = left;
	e->val.op.right = right;
	return e;
}
EXP* make_EXP_div(EXP * left, EXP * right)
{
	if (FALCON_DEBUG_PARSER)
		printf("made / at line: %d\n", lineno);
	EXP * e = NEW(EXP);
	e->lineno = lineno;
	e->kind = EXP::exp_divK;
	e->val.op.left = left;
	e->val.op.right = right;
	return e;
}
EXP* make_EXP_plus(EXP * left, EXP * right)
{
	if (FALCON_DEBUG_PARSER)
		printf("made + at line: %d\n", lineno);
	EXP * e = NEW(EXP);
	e->lineno = lineno;
	e->kind = EXP::exp_plusK;
	e->val.op.left = left;
	e->val.op.right = right;
	return e;
}
EXP* make_EXP_minus(EXP * left, EXP * right)
{
	if (FALCON_DEBUG_PARSER)
		printf("made - at line: %d\n", lineno);
	EXP * e = NEW(EXP);
	e->lineno = lineno;
	e->kind = EXP::exp_minusK;
	e->val.op.left = left;
	e->val.op.right = right;
	return e;
}
EXP* make_EXP_equality(EXP * left, EXP * right)
{
	if (FALCON_DEBUG_PARSER)
		printf("made == at line: %d\n", lineno);
	EXP * e = NEW(EXP);
	e->lineno = lineno;
	e->kind = EXP::exp_equalityK;
	e->val.op.left = left;
	e->val.op.right = right;
	return e;
}
EXP* make_EXP_non_equality(EXP * left, EXP * right)
{
	if (FALCON_DEBUG_PARSER)
		printf("made != at line: %d\n", lineno);
	EXP * e = NEW(EXP);
	e->lineno = lineno;
	e->kind = EXP::exp_non_equalityK;
	e->val.op.left = left;
	e->val.op.right = right;
	return e;
}
EXP* make_EXP_greater(EXP * left, EXP * right)
{
	if (FALCON_DEBUG_PARSER)
		printf("made > at line: %d\n", lineno);
	EXP * e = NEW(EXP);
	e->lineno = lineno;
	e->kind = EXP::exp_greaterK;
	e->val.op.left = left;
	e->val.op.right = right;
	return e;
}
EXP* make_EXP_lesser(EXP * left, EXP * right)
{
	if (FALCON_DEBUG_PARSER)
		printf("made < at line: %d\n", lineno);
	EXP * e = NEW(EXP);
	e->lineno = lineno;
	e->kind = EXP::exp_lesserK;
	e->val.op.left = left;
	e->val.op.right = right;
	return e;
}
EXP* make_EXP_greater_equal(EXP * left, EXP * right)
{
	if (FALCON_DEBUG_PARSER)
		printf("made >= at line: %d\n", lineno);
	EXP * e = NEW(EXP);
	e->lineno = lineno;
	e->kind = EXP::exp_greater_equalK;
	e->val.op.left = left;
	e->val.op.right = right;
	return e;
}
EXP* make_EXP_lesser_equal(EXP * left, EXP * right)
{
	if (FALCON_DEBUG_PARSER)
		printf("made <= at line: %d\n", lineno);
	EXP * e = NEW(EXP);
	e->lineno = lineno;
	e->kind = EXP::exp_lesser_equalK;
	e->val.op.left = left;
	e->val.op.right = right;
	return e;
}
EXP* make_EXP_and(EXP * left, EXP * right)
{
	if (FALCON_DEBUG_PARSER)
		printf("made && at line: %d\n", lineno);
	EXP * e = NEW(EXP);
	e->lineno = lineno;
	e->kind = EXP::exp_andK;
	e->val.op.left = left;
	e->val.op.right = right;
	return e;
}
EXP* make_EXP_or(EXP * left, EXP * right)
{
	if (FALCON_DEBUG_PARSER)
		printf("made || at line: %d\n", lineno);
	EXP * e = NEW(EXP);
	e->lineno = lineno;
	e->kind = EXP::exp_orK;
	e->val.op.left = left;
	e->val.op.right = right;
	return e;
}
EXP* make_EXP_term(TERM * term)
{
	if (FALCON_DEBUG_PARSER)
		printf("made <exp> term at line: %d\n", lineno);
	EXP * e = NEW(EXP);
	e->lineno = lineno;
	e->kind = EXP::exp_termK;
	e->val.term = term;
	return e;
}
/* =============== EXP END =============== */



/* =============== TERM START =============== */
TERM* make_TERM_variable(VARIABLE * var)
{
	if (FALCON_DEBUG_PARSER)
		printf("made <term> variable at line: %d\n", lineno);
	TERM * e = NEW(TERM);
	e->lineno = lineno;
	e->kind = TERM::term_varK;
	e->val.var = var;
	return e;
}
TERM* make_TERM_id_act_list(char* id, struct ACT_LIST * act_list)
{
	if (FALCON_DEBUG_PARSER)
		printf("made <term> id_act_list at line: %d\n", lineno);
	TERM * e = NEW(TERM);
	e->lineno = lineno;
	e->kind = TERM::term_id_act_listK;
	e->val.id_act_list.id = id;
	e->val.id_act_list.act_list = act_list;
	return e;
}
TERM* make_TERM_expression(EXP * exp)
{
	if (FALCON_DEBUG_PARSER)
		printf("made <term> expression at line: %d\n", lineno);
	TERM * e = NEW(TERM);
	e->lineno = lineno;
	e->kind = TERM::term_parenthesis_expressionK;
	e->val.exp = exp;
	return e;
}
TERM* make_TERM_negate(TERM * term)
{
	if (FALCON_DEBUG_PARSER)
		printf("made <term> negation at line: %d\n", lineno);
	TERM * e = NEW(TERM);
	e->lineno = lineno;
	e->kind = TERM::term_negationK;
	e->val.term = term;
	return e;
}
TERM* make_TERM_absolute(EXP * exp)
{
	if (FALCON_DEBUG_PARSER)
		printf("made <term> absolute at line: %d\n", lineno);
	TERM * e = NEW(TERM);
	e->lineno = lineno;
	e->kind = TERM::term_absoluteK;
	e->val.exp = exp;
	return e;
}
TERM* make_TERM_num(int intconst)
{
	if (FALCON_DEBUG_PARSER)
		printf("made <term> num at line: %d\n", lineno);
	TERM * e = NEW(TERM);
	e->lineno = lineno;
	e->kind = TERM::term_numK;
	e->val.num = intconst;
	return e;
}
TERM* make_TERM_true()
{
	if (FALCON_DEBUG_PARSER)
		printf("made <term> true at line: %d\n", lineno);
	TERM * e = NEW(TERM);
	e->lineno = lineno;
	e->kind = TERM::term_trueK;
	e->val.boolean = 1;
	return e;
}
TERM* make_TERM_false()
{
	if (FALCON_DEBUG_PARSER)
		printf("made <term> false at line: %d\n", lineno);
	TERM * e = NEW(TERM);
	e->lineno = lineno;
	e->kind = TERM::term_falseK;
	e->val.boolean = 0;
	return e;
}
TERM* make_TERM_null()
{
	if (FALCON_DEBUG_PARSER)
		printf("made <term> null at line: %d\n", lineno);
	TERM * e = NEW(TERM);
	e->lineno = lineno;
	e->kind = TERM::term_nullK;
	return e;
}
TERM* make_TERM_string(STRING * string)
{
	if (FALCON_DEBUG_PARSER)
		printf("made <term> string at line: %d\n", lineno);
	TERM * e = NEW(TERM);
	e->lineno = lineno;
	e->kind = TERM::term_stringK;
	e->val.string = string;
	return e;
}

/* =============== TERM END =============== */


/* =============== ACT_LIST START =============== */
ACT_LIST* make_ACT_LIST_exp_list(struct EXP_LIST * exp_list)
{
	if (FALCON_DEBUG_PARSER)
		printf("made <act_list> exp_list at line: %d\n", lineno);
	ACT_LIST * e = NEW(ACT_LIST);
	e->lineno = lineno;
	e->kind = ACT_LIST::act_list_exp_listK;
	e->exp_list = exp_list;
	return e;
}
ACT_LIST* make_ACT_LIST_empty()
{
	if (FALCON_DEBUG_PARSER)
		printf("made <act_list> empty at line: %d\n", lineno);
	ACT_LIST * e = NEW(ACT_LIST);
	e->lineno = lineno;
	e->kind = ACT_LIST::act_list_emptyK;
	return e;
}
/* =============== ACT_LIST END =============== */


/* =============== EXP_LIST START =============== */
EXP_LIST* make_EXP_LIST_expression(EXP * exp)
{
	if (FALCON_DEBUG_PARSER)
		printf("made <exp_list> expression at line: %d\n", lineno);
	EXP_LIST * e = NEW(EXP_LIST);
	e->lineno = lineno;
	e->kind = EXP_LIST::exp_list_expressionK;
	e->val.exp = exp;
	return e;
}
EXP_LIST* make_EXP_LIST_pair(EXP * head, EXP_LIST * tail)
{
	if (FALCON_DEBUG_PARSER)
		printf("made <exp_list> pair at line: %d\n", lineno);
	EXP_LIST * e = NEW(EXP_LIST);
	e->lineno = lineno;
	e->kind = EXP_LIST::exp_list_pairK;
	e->val.pair.head = head;
	e->val.pair.tail = tail;
	return e;

}
/* =============== EXP_LIST END =============== */

