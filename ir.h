#pragma once
#include "tree.h"

enum e_node_type
{
	//t_ROOT_AST, //not really needed.
	ntSTRING,
	ntFUNCTION,
	ntHEAD,
	ntTAIL,
	ntTYPE,
	ntPAR_DECL_LIST,
	ntVAR_DECL_LIST,
	ntVAR_TYPE,
	ntBODY,
	ntBODY_DECL_LIST,
	ntDECL_LIST,
	ntDECL,
	ntSTM_LIST,
	ntSTM,
	ntVARIABLE,
	ntEXP,
	ntTERM,
	ntACT_LIST,
	ntEXP_LIST,
	TERM_FUNCTION_CALL,
};



typedef struct ir_info
{
	enum e_node_type node_type;

	size_t registers_in;
	size_t registers_out;

	char* returned_string;
	char* returned_type;
	char* variable_identifier;
	SymbolTable* variable_sym_table;

} ir_info;

enum identifier_distinction
{
	GLOBAL_IDENTIFIER,
	LOCAL_IDENTIFIER,
};




linked_list* intermediate_representation(AST_NODE* AST);
ir_info* ir_AST(AST_NODE* ast);
ir_info* ir_DIRECTIVE_LIST(DIRECTIVE_LIST* directive_list);
ir_info* ir_DIRECTIVE(DIRECTIVE* directive);
ir_info* ir_STRING(STRING* str);
ir_info* ir_FUNCTION(FUNCTION* f);
ir_info* ir_HEAD(HEAD* head);
ir_info* ir_TYPE(TYPE* type);
ir_info* ir_PAR_DECL_LIST(PAR_DECL_LIST* par_decl_list);
ir_info* ir_VAR_DECL_LIST(VAR_DECL_LIST* var_decl_list);
ir_info* ir_VAR_TYPE(VAR_TYPE* var_type);
ir_info* ir_BODY(BODY* body);
//ir_info* ir_BODY_DECL_LIST(BODY_DECL_LIST* body_decl_list);
ir_info* ir_DECL_LIST(DECL_LIST* decl_list);
ir_info* ir_DECL(DECL* decl);
ir_info* ir_STM_LIST(STM_LIST* stm_list);
ir_info* ir_STM(STM* stm);
ir_info* ir_VARIABLE(VARIABLE* var);
ir_info* ir_EXP(EXP* exp);
ir_info* ir_TERM(TERM* term);
ir_info* ir_ACT_LIST(ACT_LIST* act_list);
ir_info* ir_EXP_LIST(EXP_LIST * exp_list);
