#pragma once
#include "tree.h"


typedef struct call_node
{
	FUNCTION* self;
	FUNCTION* target; //target function that is called inside self function.
	call_node* next;
}call_node;

typedef struct call_info
{
	bool is_recursive;
	call_node* call_node;
	array* callees; //the functions we call.
	array* callers; //the functions that call us.


}call_info;

void pre_ir_weed(AST_NODE* body);
void ir_weed_AST(AST_NODE* ast);
void ir_weed_DIRECTIVE_LIST(DIRECTIVE_LIST* directive_list);
void ir_weed_DIRECTIVE(DIRECTIVE* directive);
void ir_weed_FUNCTION(FUNCTION* f);
void ir_weed_HEAD(HEAD* head, FUNCTION* f);
void ir_weed_TYPE(TYPE* type);
void ir_weed_PAR_DECL_LIST(PAR_DECL_LIST* par_decl_list);
void ir_weed_VAR_DECL_LIST(VAR_DECL_LIST* var_decl_list);
void ir_weed_VAR_TYPE(VAR_TYPE* var_type);
void ir_weed_BODY(BODY* body);
//void ir_weed_BODY_DECL_LIST(BODY_DECL_LIST* body_decl_list);
void ir_weed_DECL_LIST(DECL_LIST* decl_list, array* array_of_declarations);
void ir_weed_DECL(DECL* decl);
void ir_weed_STM_LIST(STM_LIST* stm_list);
void ir_weed_STM(STM* stm);
void ir_weed_VARIABLE(VARIABLE* var);
void ir_weed_EXP(EXP* exp);
void ir_weed_TERM(TERM* term);
void ir_weed_ACT_LIST(ACT_LIST* act_list);
void ir_weed_EXP_LIST(EXP_LIST * exp_list);
