#include "tree.h"
#include "weeder.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern char FALCON_DEBUG_WEEDER;

static FUNCTION* weeder_global_function; //ptr to the currently scoped function. to keep track of it.
static unsigned int weeder_global_return_statements_encountered; //unsigned integer incremented for each function we're in.
static unsigned int weeder_global_error_count;

void weed(AST_NODE* AST)
{
	weeder_global_function = 0;
	weeder_global_return_statements_encountered = 0;
	weeder_global_error_count = 0;

	weed_AST(AST);


	//stop immediately.
	//GHJ: The reason why we wait til the weeder is done,
	//is to give the user all the warnings and errors instead
	//of just stopping at the first error encountered.
	if (weeder_global_error_count > 0)
	{
		exit(1);
	}
}


void weed_DIRECTIVE_LIST(DIRECTIVE_LIST* directive_list)
{
	if (FALCON_DEBUG_WEEDER)
		printf("|||||------> <directive_list> %d -- %d\n", directive_list->kind, directive_list->lineno);
	switch (directive_list->kind)
	{
		case DIRECTIVE_LIST::pair:
		{
			weed_DIRECTIVE_LIST(directive_list->val.pair.directive_list);
			weed_DIRECTIVE(directive_list->val.pair.directive);
		}break;
		case DIRECTIVE_LIST::empty:
		{
		}break;
	}


}


void weed_DIRECTIVE(DIRECTIVE* directive)
{
	if (FALCON_DEBUG_WEEDER)
		printf("|||||------> <directive> %d -- %d\n", directive->kind, directive->lineno);
	switch (directive->kind)
	{
		case DIRECTIVE::load:
		{}
		break;
	}
}

void weed_AST(AST_NODE* ast)
{


	weeder_global_function = 0;
	weeder_global_return_statements_encountered = 0;
	weeder_global_error_count = 0;


	weed_DIRECTIVE_LIST(ast->directive_list);
	weed_BODY(ast->body);


	//stop immediately.
	//GHJ: The reason why we wait til the weeder is done,
	//is to give the user all the warnings and errors instead
	//of just stopping at the first error encountered.
	if (weeder_global_error_count > 0)
	{
		exit(1);
	}
}

void weed_STRING(STRING* s)
{
	if (FALCON_DEBUG_WEEDER)
		printf("|||||------> <string> %d -- %d\n", 0, s->lineno);
	if (s)
	{
		//no-op. to prevent gcc compilation warnings.
	}
}

void weed_FUNCTION(FUNCTION* f)
{
	if (FALCON_DEBUG_WEEDER)
		printf("|||||------> <function> %s -- %d -- %d\n", f->func_head->function_id, 0, f->lineno);
	weeder_global_function = f;


	weed_HEAD(f->func_head);
	weed_BODY(f->func_body);
}

void weed_HEAD(HEAD* head)
{
	if (FALCON_DEBUG_WEEDER)
		printf("|||||------> <head> %d -- %d\n", 0, head->lineno);
	weed_PAR_DECL_LIST(head->params);
	weed_TYPE(head->return_type);
}


void weed_TYPE(TYPE * type)
{
	if (FALCON_DEBUG_WEEDER)
		printf("|||||------> <type> %d -- %d\n", type->kind, type->lineno);

	switch (type->kind)
	{
		case TYPE::type_idK:
		break;
		case TYPE::type_intK:
		break;
		case TYPE::type_boolK:
		break;
		case TYPE::type_arrayK:
		weed_TYPE(type->val.type);
		break;
		case TYPE::type_recordK:
		weed_VAR_DECL_LIST(type->val.var_decl_list);
		break;
		case TYPE::type_stringK:
		break;
		default:  break;
	}
}


void weed_PAR_DECL_LIST(PAR_DECL_LIST * par_decl_list)
{
	if (FALCON_DEBUG_WEEDER)
		printf("|||||------> <par_decl_list> %d -- %d\n", 0, par_decl_list->lineno);
	switch (par_decl_list->kind)
	{
		case PAR_DECL_LIST::par_decl_list_var_decl_listK:
		weed_VAR_DECL_LIST(par_decl_list->val.var_decl_list);
		break;
		case PAR_DECL_LIST::par_decl_list_emptyK:
		break;
		default:  break;;
	}
}
void weed_VAR_DECL_LIST(VAR_DECL_LIST * var_decl_list)
{
	if (FALCON_DEBUG_WEEDER)
		printf("|||||------> <var_decl_list> %d -- %d\n", var_decl_list->kind, var_decl_list->lineno);
	switch (var_decl_list->kind)
	{
		case VAR_DECL_LIST::var_decl_list_pairK:
		weed_VAR_TYPE(var_decl_list->val.pair.head);
		weed_VAR_DECL_LIST(var_decl_list->val.pair.tail);
		break;
		case VAR_DECL_LIST::var_decl_var_typeK:
		weed_VAR_TYPE(var_decl_list->val.var_type);
		break;
		default:  break;
	}
}

void weed_VAR_TYPE(VAR_TYPE * var_type)
{
	if (FALCON_DEBUG_WEEDER)
		printf("|||||------> <var_type> %d -- %d\n", 0, var_type->lineno);
	weed_TYPE(var_type->type);
}


void weed_BODY(BODY * body)
{
	if (FALCON_DEBUG_WEEDER)
		printf("|||||------> <body> %d -- %d\n", 0, body->lineno);
	weed_DECL_LIST(body->decl_list);
	weed_STM_LIST(body->stm_list);
}
/*
void weed_BODY_DECL_LIST(BODY_DECL_LIST* body_decl_list)
{
	if (FALCON_DEBUG_WEEDER)
		printf("|||||------> <body_decl_list> %d -- %d\n", body_decl_list->kind, body_decl_list->lineno);
	switch (body_decl_list->kind)
	{
		case BODY_DECL_LIST::body_decl_list_list:
		{
			weed_DECL_LIST(body_decl_list->decl_list);
		}break;
		case BODY_DECL_LIST::body_decl_list_empty:
		{

		}
		break;
	}
}
*/

void weed_DECL_LIST(DECL_LIST * decl_list)
{
	if (FALCON_DEBUG_WEEDER)
		printf("|||||------> <decl_list> %d -- %d\n", decl_list->kind, decl_list->lineno);
	switch (decl_list->kind)
	{
		case DECL_LIST::decl_list_pair:
		{
			weed_DECL(decl_list->val.pair.head);
			weed_DECL_LIST(decl_list->val.pair.next);
		}break;
		case DECL_LIST::decl_list_empty:
		{
			//weed_DECL(decl_list->val.decl);
		} break;
		default:  break;
	}
}


void weed_DECL(DECL * decl)
{
	if (FALCON_DEBUG_WEEDER)
		printf("|||||------> <decl> %d -- %d\n", decl->kind, decl->lineno);
	switch (decl->kind)
	{
		case DECL::decl_assignK:
		weed_TYPE(decl->val.type);
		break;
		case DECL::decl_functionK:
		weed_FUNCTION(decl->val.function);
		break;
		case DECL::decl_varsK:
		weed_VAR_DECL_LIST(decl->val.var_decl_list);
		break;
		case DECL::decl_var_typ_exp:
		{
			weed_VAR_TYPE(decl->var_type);
			weed_EXP(decl->exp);
		}break;
		case DECL::decl_struct:
		{
			weed_BODY(decl->body);
		}break;
		default:  break;
	}
}

void weed_STM_LIST(STM_LIST * stm_list)
{
	if (FALCON_DEBUG_WEEDER)
		printf("|||||------> <stm_list> %d -- %d\n", stm_list->kind, stm_list->lineno);
	switch (stm_list->kind)
	{
		case STM_LIST::stm_list_pairK:
		{
			weed_STM(stm_list->val.pair.head);
			weed_STM_LIST(stm_list->val.pair.tail);
		}
		break;
		case STM_LIST::stm_list_empty:
		{

		}
		break;
		default:  break;
	}
}


void weed_STM(STM * stm)
{
	if (FALCON_DEBUG_WEEDER)
		printf("|||||------> <stm> %d -- %d\n", stm->kind, stm->lineno);
	switch (stm->kind)
	{
		case STM::stm_returnK:
		weeder_global_return_statements_encountered++;
		weed_EXP(stm->val.exp);
		break;
		case STM::stm_printf:
		{
			weed_EXP(stm->val.exp);
		}break;
		case STM::stm_allocateK:
		weed_VARIABLE(stm->val.var);
		break;
		case STM::stm_allocate_lengthK:
		weed_VARIABLE(stm->val.allocate_length.var);
		weed_EXP(stm->val.allocate_length.exp);
		break;
		case STM::stm_assignmentK:
		weed_VARIABLE(stm->val.assignment.var);
		weed_EXP(stm->val.assignment.exp);
		break;
		case STM::stm_ifK:
		weed_EXP(stm->val.if_stm.exp);
		weed_STM(stm->val.if_stm.stm);
		break;
		case STM::stm_if_elseK:
		weed_EXP(stm->val.if_else_stm.exp);
		weed_STM(stm->val.if_else_stm.stm_then);
		weed_STM(stm->val.if_else_stm.stm_else);
		break;

		case STM::stm_whileK:
		weed_EXP(stm->val.while_stm.exp);
		weed_STM(stm->val.while_stm.stm);
		break;
		case STM::stm_for:
		{
			weed_VAR_TYPE(stm->val.for_stm.var_type);
			weed_EXP(stm->val.for_stm.exp);
			weed_STM(stm->val.for_stm.stm);
			weed_BODY(stm->val.for_stm.body);
		}
		break;

		//case STM::stm_listK:
		//weed_STM_LIST(stm->val.list);
		break;
		default:  break;
	}
}


void weed_VARIABLE(VARIABLE * var)
{
	if (FALCON_DEBUG_WEEDER)
		printf("|||||------> <var> %d -- %d\n", var->kind, var->lineno);
	switch (var->kind)
	{
		case VARIABLE::variable_idK:
		break;
		case VARIABLE::variable_arrayAccessK:
		weed_VARIABLE(var->val.array_access.var);
		weed_EXP(var->val.array_access.exp);
		break;
		case VARIABLE::variable_access_idK:
		weed_VARIABLE(var->val.access.var);
		break;
		default: break;
	}
}

void weed_EXP(EXP * exp)
{
	if (FALCON_DEBUG_WEEDER)
		printf("|||||------> <exp> %d -- %d\n", exp->kind, exp->lineno);
	switch (exp->kind)
	{
		case EXP::exp_timesK:
		weed_EXP(exp->val.op.left);
		weed_EXP(exp->val.op.right);
		break;
		case EXP::exp_divK:
		weed_EXP(exp->val.op.left);
		weed_EXP(exp->val.op.right);
		break;
		case EXP::exp_plusK:
		weed_EXP(exp->val.op.left);
		weed_EXP(exp->val.op.right);
		break;
		case EXP::exp_minusK:
		weed_EXP(exp->val.op.left);
		weed_EXP(exp->val.op.right);
		break;
		case EXP::exp_equalityK:
		weed_EXP(exp->val.op.left);
		weed_EXP(exp->val.op.right);
		break;
		case EXP::exp_non_equalityK:
		weed_EXP(exp->val.op.left);
		weed_EXP(exp->val.op.right);
		break;
		case EXP::exp_greaterK:
		weed_EXP(exp->val.op.left);
		weed_EXP(exp->val.op.right);
		break;
		case EXP::exp_lesserK:
		weed_EXP(exp->val.op.left);
		weed_EXP(exp->val.op.right);
		break;
		case EXP::exp_greater_equalK:
		weed_EXP(exp->val.op.left);
		weed_EXP(exp->val.op.right);
		break;
		case EXP::exp_lesser_equalK:
		weed_EXP(exp->val.op.left);
		weed_EXP(exp->val.op.right);
		break;
		case EXP::exp_andK:
		weed_EXP(exp->val.op.left);
		weed_EXP(exp->val.op.right);
		break;
		case EXP::exp_orK:
		weed_EXP(exp->val.op.left);
		weed_EXP(exp->val.op.right);
		break;
		case EXP::exp_termK:
		weed_TERM(exp->val.term);
		break;

		default:break;
	}
}

void weed_TERM(TERM * term)
{
	if (FALCON_DEBUG_WEEDER)
		printf("|||||------> <term> %d -- %d\n", term->kind, term->lineno);
	switch (term->kind)
	{
		case TERM::term_varK:
		weed_VARIABLE(term->val.var);
		break;
		case TERM::term_id_act_listK:
		{
			weed_ACT_LIST(term->val.id_act_list.act_list);
		} break;
		case TERM::term_parenthesis_expressionK:
		weed_EXP(term->val.exp);
		break;
		case TERM::term_negationK:
		weed_TERM(term->val.term);
		break;
		case TERM::term_absoluteK:
		weed_EXP(term->val.exp);
		break;
		case TERM::term_numK:
		break;
		case TERM::term_trueK:
		break;
		case TERM::term_falseK:
		break;
		case TERM::term_nullK:
		break;
		case TERM::term_stringK:
		weed_STRING(term->val.string);
		break;
		default:  break;
	}
}




void weed_ACT_LIST(ACT_LIST * act_list)
{

	if (FALCON_DEBUG_WEEDER)
		printf("|||||------> <act_list> %d -- %d\n", act_list->kind, act_list->lineno);
	switch (act_list->kind)
	{
		case ACT_LIST::act_list_exp_listK:
		weed_EXP_LIST(act_list->exp_list);
		break;
		case ACT_LIST::act_list_emptyK:
		break;
		default:  break;
	}
}


void weed_EXP_LIST(EXP_LIST * exp_list)
{
	if (FALCON_DEBUG_WEEDER)
		printf("|||||------> <exp_list> %d -- %d\n", exp_list->kind, exp_list->lineno);
	switch (exp_list->kind)
	{
		case EXP_LIST::exp_list_pairK:
		weed_EXP(exp_list->val.pair.head);
		weed_EXP_LIST(exp_list->val.pair.tail);
		break;
		case EXP_LIST::exp_list_expressionK:
		weed_EXP(exp_list->val.exp);
		break;
		default:  break;
	}
}