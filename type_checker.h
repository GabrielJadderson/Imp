#pragma once
#include "tree.h"
#include "symbol.h"

typedef struct type_info_imp
{
	enum
	{
		t_idk,
		t_intK,
		t_boolK,
		t_stringK,
		t_recordK,
		t_arrayK,
		t_nullK,
		t_typeK, //proper type.
	} kind;

	char* func_id;
	char* type_id;
	char is_record;
	char has_return;
	char is_a_type;
	char is_a_function;
	char is_a_variable;
	char is_a_record;
	char is_verified;
	char is_record_element;
	char visited;
	struct type_info_imp* array_type_info;
	struct type_info_imp* type_info_child;
	struct type_info_imp* return_type;
	VAR_DECL_LIST* record_var_decl_list;
	array* function_var_decl_list_types;
	array* record_var_decl_list_types;
	array* act_list_types;

	SymbolTable* record_scope;
	SymbolTable* sym_table;
	SYMBOL* symbol;

	size_t act_list_count;
	FUNCTION* function;

	bool parameterized;
	int parameter_offset;
	int variable_parameter_offset;
	int pushed_at_index;
	int variable_declaration_offset;
	bool is_global;

	bool local_variable;

	bool is_used = false;
} type_info_imp;

SymbolTable* type_checker(AST_NODE* AST);
type_info_imp* type_check_AST(AST_NODE* ast, SymbolTable* scope);
type_info_imp* type_check_DIRECTIVE_LIST(DIRECTIVE_LIST* directive_list, SymbolTable* scope);
type_info_imp* type_check_DIRECTIVE(DIRECTIVE* directive, SymbolTable* scope);



type_info_imp* type_check_STRING(STRING* s, SymbolTable* scope);
type_info_imp* type_check_FUNCTION(FUNCTION* f, SymbolTable* scope);
type_info_imp* type_check_HEAD(HEAD* head, SymbolTable* scope, FUNCTION* f);
type_info_imp* type_check_TYPE(TYPE* type, SymbolTable* scope);
type_info_imp* type_check_PAR_DECL_LIST(PAR_DECL_LIST* par_decl_list, SymbolTable* scope);
type_info_imp* type_check_VAR_DECL_LIST(VAR_DECL_LIST* var_decl_list, SymbolTable* scope, array* arr);
type_info_imp* type_check_VAR_TYPE(VAR_TYPE* var_type, SymbolTable* scope);
type_info_imp* type_check_BODY(BODY* body, SymbolTable* scope);
//type_info_imp* type_check_BODY_DECL_LIST(BODY_DECL_LIST* body_decl_list, SymbolTable* scope);
type_info_imp* type_check_DECL_LIST(DECL_LIST* decl_list, SymbolTable* scope);
type_info_imp* type_check_DECL(DECL* decl, SymbolTable* scope);
type_info_imp* type_check_STM_LIST(STM_LIST* stm_list, SymbolTable* scope);
type_info_imp* type_check_STM(STM* stm, SymbolTable* scope);
type_info_imp* type_check_VARIABLE(VARIABLE* var, SymbolTable* scope);
type_info_imp* type_check_EXP(EXP* exp, SymbolTable* scope);
type_info_imp* type_check_TERM(TERM* term, SymbolTable* scope);
type_info_imp* type_check_ACT_LIST(ACT_LIST* act_list, SymbolTable* scope, array* arr);
type_info_imp* type_check_EXP_LIST(EXP_LIST * exp_list, SymbolTable* scope, array* arr);

void type_checker_print_undefined_variable(int lineno, char* id);

