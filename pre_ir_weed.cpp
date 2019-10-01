#include "pre_ir_weed.h"
#include "type_checker.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "memory.h"
#include <unordered_map>
#include "logger.h"
#include "codegen.h"
#include "emit_util.h"

extern char FALCON_DEBUG_IR_WEED;

extern std::unordered_map<std::string, call_node*> CALL_TABLE;
extern std::unordered_map<std::string, FUNCTION*> FUNCTION_LIST;
extern std::unordered_map<std::string, VAR_TYPE*> GLOBAL_VARIABLE_LIST;
extern std::unordered_map<std::string, FUNCTION*> GLOBAL_FUNCTION_LIST;
std::unordered_map<void*, array*> GLOBAL_USED_REGISTER_MAP;

extern array* calling_list;

static FUNCTION* weeder_global_function; //ptr to the currently scoped function. to keep track of it.


enum { collect, verify } state;

static bool IN_GLOBAL_SCOPE = true;
static bool GLOBAL_IS_PARAMETER = false;

void pre_ir_weed(AST_NODE* AST)
{
	weeder_global_function = 0;
	if (AST->body->stm_list->kind != STM_LIST::stm_list_empty)
	{
		fprintf(stderr, "\x1b[37;1m");
		fprintf(stderr, "%d: ", AST->body->stm_list->lineno); //print  lineno
		fprintf(stderr, "\x1b[0m"); //reset color.
		fprintf(stderr, "\x1b[31;1m"); //start print color magenta.
		fprintf(stderr, "Error: "); //start print color magenta.
		fprintf(stderr, "\x1b[0m"); //reset color.
		fprintf(stderr, "Statements are not allowed to be declared in the outer body of a source file.");
		fprintf(stderr, "\x1b[0m\n");
		exit(1);
	}

	state = collect;
	ir_weed_AST(AST);

	state = verify;
	ir_weed_AST(AST);


}

void ir_weed_AST(AST_NODE* ast)
{
	ir_weed_DIRECTIVE_LIST(ast->directive_list);
	ir_weed_BODY(ast->body);
}


void ir_weed_DIRECTIVE_LIST(DIRECTIVE_LIST* directive_list)
{
	if (FALCON_DEBUG_IR_WEED)
		printf("|||||------> <directive_list> %d -- %d\n", directive_list->kind, directive_list->lineno);
	switch (directive_list->kind)
	{
		case DIRECTIVE_LIST::pair:
		{
			ir_weed_DIRECTIVE_LIST(directive_list->val.pair.directive_list);
			ir_weed_DIRECTIVE(directive_list->val.pair.directive);
		}break;
		case DIRECTIVE_LIST::empty:
		{
		}break;
	}

}


void ir_weed_DIRECTIVE(DIRECTIVE* directive)
{
	if (FALCON_DEBUG_IR_WEED)
		printf("|||||------> <directive> %d -- %d\n", directive->kind, directive->lineno);
	switch (directive->kind)
	{
		case DIRECTIVE::load:
		{
		}break;
	}
}

void ir_weed_STRING(STRING* s)
{
	if (s)
	{
		if (FALCON_DEBUG_IR_WEED)
			printf("|||||------> <tail> %s %d -- %d\n", s->str, s->kind, s->lineno);
			//no-op. to prevent gcc compilation warnings.
	}
}

void ir_weed_FUNCTION(FUNCTION* f)
{
	if (FALCON_DEBUG_IR_WEED)
		printf("|||||------> <function> %d -- %d\n", 0, f->lineno);
	weeder_global_function = f;
	IN_GLOBAL_SCOPE = false;
	if (state == collect)
	{
		f->call_info = static_cast<call_info*>(Calloc(1, sizeof(call_info)));
		f->call_info->call_node = static_cast<call_node*>(Calloc(1, sizeof(call_node)));
		f->call_info->callees = array_new();
		f->call_info->callers = array_new();
		f->call_info->is_recursive = false;
		f->local_variables = array_new();
		FUNCTION_LIST.insert_or_assign(std::string(f->func_head->function_id), f);
	}

	ir_weed_HEAD(f->func_head, f);
	ir_weed_BODY(f->func_body);

	log_infoc("NUMBER OF LOCAL VARIABLES in function: ");
	printf("%s, %zu\n", f->func_head->function_id, f->local_variables->length);

	IN_GLOBAL_SCOPE = true;
}

void ir_weed_HEAD(HEAD* head, FUNCTION* f)
{
	if (FALCON_DEBUG_IR_WEED)
		printf("|||||------> <head> %s %d -- %d\n", head->function_id, 0, head->lineno);
	ir_weed_PAR_DECL_LIST(head->params);
	ir_weed_TYPE(head->return_type);

	//if (head->visibility == VISIBILITY::GLOBAL)
	//	{
	GLOBAL_FUNCTION_LIST.insert_or_assign(head->function_id, f);
//}
}


void ir_weed_TYPE(TYPE * type)
{
	if (FALCON_DEBUG_IR_WEED)
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
		ir_weed_TYPE(type->val.type);
		break;
		case TYPE::type_recordK:
		ir_weed_VAR_DECL_LIST(type->val.var_decl_list);
		break;
		case TYPE::type_stringK:
		break;
		default:  break;
	}
}


void ir_weed_PAR_DECL_LIST(PAR_DECL_LIST * par_decl_list)
{
	if (FALCON_DEBUG_IR_WEED)
		printf("|||||------> <par_decl_list> %d -- %d\n", par_decl_list->kind, par_decl_list->lineno);
	switch (par_decl_list->kind)
	{
		case PAR_DECL_LIST::par_decl_list_var_decl_listK:
		{
			GLOBAL_IS_PARAMETER = true;
			ir_weed_VAR_DECL_LIST(par_decl_list->val.var_decl_list);
			GLOBAL_IS_PARAMETER = false;
		}
		break;
		case PAR_DECL_LIST::par_decl_list_emptyK:
		break;
		default:  break;;
	}
}
void ir_weed_VAR_DECL_LIST(VAR_DECL_LIST * var_decl_list)
{
	if (FALCON_DEBUG_IR_WEED)
		printf("|||||------> <var_decl_list> %d -- %d\n", var_decl_list->kind, var_decl_list->lineno);
	switch (var_decl_list->kind)
	{
		case VAR_DECL_LIST::var_decl_list_pairK:
		ir_weed_VAR_TYPE(var_decl_list->val.pair.head);
		ir_weed_VAR_DECL_LIST(var_decl_list->val.pair.tail);
		break;
		case VAR_DECL_LIST::var_decl_var_typeK:
		ir_weed_VAR_TYPE(var_decl_list->val.var_type);
		break;
		default:  break;
	}
}

void ir_weed_VAR_TYPE(VAR_TYPE * var_type)
{
	if (FALCON_DEBUG_IR_WEED)
		printf("|||||------> <var_type> %d -- %d\n", 0, var_type->lineno);
	ir_weed_TYPE(var_type->type);

	if (state == collect)
	{
		type_info_imp* ti_var = lookup_once(var_type->ti->sym_table, var_type->id, var_type->lineno);
		if (GLOBAL_IS_PARAMETER)
		{
			ti_var->parameterized = true;

		}
		else if (!IN_GLOBAL_SCOPE && !GLOBAL_IS_PARAMETER) //local variable
		{
			ti_var->parameterized = false;
			ti_var->local_variable = true;
			array_push(weeder_global_function->local_variables, var_type); //push self onto the current function's local variables.
		}

		if (IN_GLOBAL_SCOPE)
		{
			GLOBAL_VARIABLE_LIST.insert_or_assign(var_type->id, var_type);
		}
	}
}


void ir_weed_BODY(BODY * body)
{
	if (FALCON_DEBUG_IR_WEED)
		printf("|||||------> <body> %d -- %d\n", 0, body->lineno);

	body->array_of_declarations = array_new();
	ir_weed_DECL_LIST(body->decl_list, body->array_of_declarations);
	ir_weed_STM_LIST(body->stm_list);


}

/*
void ir_weed_BODY_DECL_LIST(BODY_DECL_LIST* body_decl_list)
{
	if (FALCON_DEBUG_IR_WEED)
		printf("|||||------> <body_decl_list> %d -- %d\n", body_decl_list->kind, body_decl_list->lineno);
	switch (body_decl_list->kind)
	{
		case BODY_DECL_LIST::body_decl_list_list:
		{
			ir_weed_DECL_LIST(body_decl_list->decl_list);
		}break;
		case BODY_DECL_LIST::body_decl_list_empty:
		{

		} break;
	}

}
*/

void ir_weed_DECL_LIST(DECL_LIST * decl_list, array* array_of_declarations)
{
	if (FALCON_DEBUG_IR_WEED)
		printf("|||||------> <decl_list> %d -- %d\n", decl_list->kind, decl_list->lineno);
	switch (decl_list->kind)
	{
		case DECL_LIST::decl_list_pair:
		{
			ir_weed_DECL_LIST(decl_list->val.pair.next, array_of_declarations);
			ir_weed_DECL(decl_list->val.pair.head);
		}break;
		case DECL_LIST::decl_list_empty:
		{
			//ir_weed_DECL(decl_list->val.decl);
		}
		break;
		default:  break;
	}
}

void ir_weed_DECL(DECL * decl)
{
	if (FALCON_DEBUG_IR_WEED)
		printf("|||||------> <decl> %d -- %d\n", decl->kind, decl->lineno);
	switch (decl->kind)
	{
		case DECL::decl_assignK:
		ir_weed_TYPE(decl->val.type);
		break;
		case DECL::decl_functionK:
		ir_weed_FUNCTION(decl->val.function);
		break;
		case DECL::decl_varsK:
		ir_weed_VAR_DECL_LIST(decl->val.var_decl_list);
		break;
		case DECL::decl_var_typ_exp:
		{
			ir_weed_VAR_TYPE(decl->var_type);
			ir_weed_EXP(decl->exp);
		}break;
		case DECL::decl_struct:
		{
			ir_weed_BODY(decl->body);
		}break;
		default:  break;
	}
}

void ir_weed_STM_LIST(STM_LIST * stm_list)
{
	if (FALCON_DEBUG_IR_WEED)
		printf("|||||------> <stm_list> %d -- %d\n", stm_list->kind, stm_list->lineno);
	switch (stm_list->kind)
	{
		case STM_LIST::stm_list_pairK:
		ir_weed_STM(stm_list->val.pair.head);
		ir_weed_STM_LIST(stm_list->val.pair.tail);
		break;
		case STM_LIST::stm_list_empty:
		break;
		default:  break;
	}
}


void ir_weed_STM(STM * stm)
{
	if (FALCON_DEBUG_IR_WEED)
		printf("|||||------> <stm> %d -- %d\n", stm->kind, stm->lineno);
	switch (stm->kind)
	{
		case STM::stm_returnK:
		ir_weed_EXP(stm->val.exp);
		break;
		case STM::stm_printf:
		{
			ir_weed_EXP(stm->val.exp);
			array* used_registers = array_new();

			array_push(used_registers, register_new_of_type(register_type::RCX));
			array_push(used_registers, register_new_of_type(register_type::RDX));
			array_push(used_registers, register_new_of_type(register_type::RSI));
			array_push(used_registers, register_new_of_type(register_type::RDI));
			array_push(used_registers, register_new_of_type(register_type::R8));
			array_push(used_registers, register_new_of_type(register_type::R9));
			array_push(used_registers, register_new_of_type(register_type::R10));
			array_push(used_registers, register_new_of_type(register_type::R11));

			GLOBAL_USED_REGISTER_MAP.insert_or_assign(static_cast<void*>(stm), used_registers);
		}
		break;
		case STM::stm_allocateK:
		ir_weed_VARIABLE(stm->val.var);
		break;
		case STM::stm_allocate_lengthK:
		ir_weed_VARIABLE(stm->val.allocate_length.var);
		ir_weed_EXP(stm->val.allocate_length.exp);
		break;
		case STM::stm_assignmentK:
		ir_weed_VARIABLE(stm->val.assignment.var);
		ir_weed_EXP(stm->val.assignment.exp);
		break;
		case STM::stm_ifK:
		ir_weed_EXP(stm->val.if_stm.exp);
		ir_weed_STM(stm->val.if_stm.stm);
		break;
		case STM::stm_if_elseK:
		ir_weed_EXP(stm->val.if_else_stm.exp);
		ir_weed_STM(stm->val.if_else_stm.stm_then);
		ir_weed_STM(stm->val.if_else_stm.stm_else);
		break;

		case STM::stm_whileK:
		ir_weed_EXP(stm->val.while_stm.exp);
		ir_weed_STM(stm->val.while_stm.stm);
		break;
		case STM::stm_for:
		{
			ir_weed_VAR_TYPE(stm->val.for_stm.var_type);
			ir_weed_EXP(stm->val.for_stm.exp);
			ir_weed_STM(stm->val.for_stm.stm);
			ir_weed_BODY(stm->val.for_stm.body);
		}
		break;


		//case STM::stm_listK:
		//ir_weed_STM_LIST(stm->val.list);
		//break;
		default:  break;
	}
}


void ir_weed_VARIABLE(VARIABLE * var)
{
	if (FALCON_DEBUG_IR_WEED)
		printf("|||||------> <var> %d -- %d\n", var->kind, var->lineno);
	switch (var->kind)
	{
		case VARIABLE::variable_idK:
		{
			if (state == collect)
			{
				type_info_imp* ti_var = static_cast<type_info_imp*>(lookup_once(var->type->sym_table, var->val.id, var->lineno));
				ti_var->is_used = true;
			}
		}
		break;
		case VARIABLE::variable_arrayAccessK:
		{
			ir_weed_VARIABLE(var->val.array_access.var);
			ir_weed_EXP(var->val.array_access.exp);
		}break;
		case VARIABLE::variable_access_idK:
		{
			ir_weed_VARIABLE(var->val.access.var);
		}break;
		default: break;
	}
}

void ir_weed_EXP(EXP * exp)
{
	if (FALCON_DEBUG_IR_WEED)
		printf("|||||------> <exp> %d -- %d\n", exp->kind, exp->lineno);

	array* used_registers = array_new();
	array_push(used_registers, register_new_of_type(register_type::RDX));
	GLOBAL_USED_REGISTER_MAP.insert_or_assign(static_cast<void*>(exp), used_registers);

	switch (exp->kind)
	{
		case EXP::exp_timesK:
		ir_weed_EXP(exp->val.op.left);
		ir_weed_EXP(exp->val.op.right);
		break;
		case EXP::exp_divK:
		ir_weed_EXP(exp->val.op.left);
		ir_weed_EXP(exp->val.op.right);
		break;
		case EXP::exp_plusK:
		ir_weed_EXP(exp->val.op.left);
		ir_weed_EXP(exp->val.op.right);
		break;
		case EXP::exp_minusK:
		ir_weed_EXP(exp->val.op.left);
		ir_weed_EXP(exp->val.op.right);
		break;
		case EXP::exp_equalityK:
		ir_weed_EXP(exp->val.op.left);
		ir_weed_EXP(exp->val.op.right);
		break;
		case EXP::exp_non_equalityK:
		ir_weed_EXP(exp->val.op.left);
		ir_weed_EXP(exp->val.op.right);
		break;
		case EXP::exp_greaterK:
		ir_weed_EXP(exp->val.op.left);
		ir_weed_EXP(exp->val.op.right);
		break;
		case EXP::exp_lesserK:
		ir_weed_EXP(exp->val.op.left);
		ir_weed_EXP(exp->val.op.right);
		break;
		case EXP::exp_greater_equalK:
		ir_weed_EXP(exp->val.op.left);
		ir_weed_EXP(exp->val.op.right);
		break;
		case EXP::exp_lesser_equalK:
		ir_weed_EXP(exp->val.op.left);
		ir_weed_EXP(exp->val.op.right);
		break;
		case EXP::exp_andK:
		ir_weed_EXP(exp->val.op.left);
		ir_weed_EXP(exp->val.op.right);
		break;
		case EXP::exp_orK:
		ir_weed_EXP(exp->val.op.left);
		ir_weed_EXP(exp->val.op.right);
		break;
		case EXP::exp_termK:
		ir_weed_TERM(exp->val.term);
		break;

		default:break;
	}
}

void ir_weed_TERM(TERM * term)
{
	if (FALCON_DEBUG_IR_WEED)
		printf("|||||------> <term> %d -- %d\n", term->kind, term->lineno);
	switch (term->kind)
	{
		case TERM::term_varK:
		ir_weed_VARIABLE(term->val.var);
		break;
		case TERM::term_id_act_listK:
		{
			if (state == verify)
			{
				FUNCTION* calling_function = lookup_once(term->type->sym_table, term->val.id_act_list.id, term->lineno)->function;

				weeder_global_function->call_info->call_node->self = weeder_global_function;
				weeder_global_function->call_info->call_node->target = calling_function;

				array_push(weeder_global_function->call_info->callees, calling_function);
				array_push(calling_function->call_info->callers, weeder_global_function);

				CALL_TABLE.insert_or_assign(std::string(term->val.id_act_list.id), weeder_global_function->call_info->call_node);

				array_push(calling_list, weeder_global_function->call_info->call_node);

				term->call_info = weeder_global_function->call_info;
			}


			ir_weed_ACT_LIST(term->val.id_act_list.act_list);



		} break;
		case TERM::term_parenthesis_expressionK:
		ir_weed_EXP(term->val.exp);
		break;
		case TERM::term_negationK:
		ir_weed_TERM(term->val.term);
		break;
		case TERM::term_absoluteK:
		ir_weed_EXP(term->val.exp);
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
		ir_weed_STRING(term->val.string);
		break;
		default:  break;
	}
}


void ir_weed_ACT_LIST(ACT_LIST * act_list)
{
	if (FALCON_DEBUG_IR_WEED)
		printf("|||||------> <act_list> %d -- %d\n", act_list->kind, act_list->lineno);
	switch (act_list->kind)
	{
		case ACT_LIST::act_list_exp_listK:
		ir_weed_EXP_LIST(act_list->exp_list);
		break;
		case ACT_LIST::act_list_emptyK:
		break;
		default:  break;
	}
}


void ir_weed_EXP_LIST(EXP_LIST * exp_list)
{
	if (FALCON_DEBUG_IR_WEED)
		printf("|||||------> <exp_list> %d -- %d\n", exp_list->kind, exp_list->lineno);
	switch (exp_list->kind)
	{
		case EXP_LIST::exp_list_pairK:
		ir_weed_EXP(exp_list->val.pair.head);
		ir_weed_EXP_LIST(exp_list->val.pair.tail);
		break;
		case EXP_LIST::exp_list_expressionK:
		ir_weed_EXP(exp_list->val.exp);
		break;
		default:  break;
	}
}

