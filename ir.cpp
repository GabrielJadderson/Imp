#include "ir.h"
#include "logger.h"
#include "memory.h"
#include "type_checker.h"
#include "falcon_string.h"
#include <cstdio>
#include <cstring>
#include <unordered_map>
#include "pre_ir_weed.h"
#include "file_io.h"

extern char FALCON_DEBUG_IR;

extern std::unordered_map<std::string, call_node*> CALL_TABLE;
extern std::unordered_map<std::string, FUNCTION*> FUNCTION_LIST;
extern std::unordered_map<std::string, VAR_TYPE*> GLOBAL_VARIABLE_LIST;
extern std::unordered_map<std::string, FUNCTION*> GLOBAL_FUNCTION_LIST;

extern array* calling_list;

static linked_list* GLOBAL_IR_LIST;
static string_builder* SB;
static int REGISTER_COUNTER;
static array* offset_stack;

char* ir_type_to_string(type_info_imp* type)
{
	switch (type->kind)
	{
		case type_info_imp::t_idk: { log_internal_error(__FILE__, __func__, __LINE__, "type is id!?"); } break;
		case type_info_imp::t_intK: { return "i32"; } break;
		case type_info_imp::t_boolK: { return "bool"; } break;
		case type_info_imp::t_stringK: { return "string"; } break;
		case type_info_imp::t_recordK: { return "record"; } break;
		case type_info_imp::t_arrayK: { return "array"; } break;
		case type_info_imp::t_nullK: { return "null"; }	break;
		default: { log_internal_error(__FILE__, __func__, __LINE__, "default case."); } break;
	}
	return "";
}

char* ir_gen_temp_register()
{
	string_builder* s = string_builder_new();
	string_builder_append_chars(s, "%");
	string_builder_append_int(s, &REGISTER_COUNTER);
	char*result = string_builder_copy_to_char_array(s);
	string_builder_destroy(s);
	REGISTER_COUNTER++;
	return result;
}

char* ir_gen_load()
{
	return nullptr;
}


linked_list* intermediate_representation(AST_NODE* AST)
{
	REGISTER_COUNTER = 0;
	GLOBAL_IR_LIST = linked_list_new();
	SB = string_builder_new();
	offset_stack = array_new();

	//find main
	FUNCTION* f = static_cast<FUNCTION*>(FUNCTION_LIST["main"]);
	if (f)
	{
		string_builder_append_chars(SB, "define ");
		ir_HEAD(f->func_head);
		string_builder_append_chars(SB, "{\n");
		string_builder_append_chars(SB, "entry:\n");
		ir_BODY(f->func_body);
		string_builder_append_chars(SB, "\n}\n");
	} else
	{
		fprintf(stderr, "\x1b[37;1m");
		fprintf(stderr, "%d: ", AST->body->lineno); //print  lineno
		fprintf(stderr, "\x1b[0m"); //reset color.
		fprintf(stderr, "\x1b[31;1m"); //start print color magenta.
		fprintf(stderr, "Error: "); //start print color magenta.
		fprintf(stderr, "\x1b[0m"); //reset color.
		fprintf(stderr, "main function was not found.");
		fprintf(stderr, "\x1b[0m\n");
		exit(1);
	}

	ir_AST(AST);
	//ir_DECL_LIST(AST->body->decl_list);

	//no statements are allowed in outer body only in functions.
	//ir_STM_LIST(AST->stm_list);
	//ir_BODY(AST);


	char* data = string_builder_copy_to_char_array(SB);
	linked_list_add_last(&GLOBAL_IR_LIST, data);

	linked_list_print(&GLOBAL_IR_LIST);
	file_write_linked_list("out.ll", &GLOBAL_IR_LIST);

	string_builder_destroy(SB);
	return GLOBAL_IR_LIST;
}

ir_info* ir_AST(AST_NODE* ast)
{
	ir_DIRECTIVE_LIST(ast->directive_list);

	ir_DECL_LIST(ast->body->decl_list);

	//ir_BODY(ast->body);
	return nullptr;
}


ir_info* ir_DIRECTIVE_LIST(DIRECTIVE_LIST* directive_list)
{
	if (FALCON_DEBUG_IR)
		printf("|||||------> <directive_list> %d -- %d\n", directive_list->kind, directive_list->lineno);
	switch (directive_list->kind)
	{
		case DIRECTIVE_LIST::pair:
			{
				ir_info* ti_directive_list = ir_DIRECTIVE_LIST(directive_list->val.pair.directive_list);
				ir_info* ti_directive = ir_DIRECTIVE(directive_list->val.pair.directive);
			}break;
		case DIRECTIVE_LIST::empty:
			{
				return nullptr;
			}break;
	}

	return nullptr;
}


ir_info* ir_DIRECTIVE(DIRECTIVE* directive)
{
	if (FALCON_DEBUG_IR)
		printf("|||||------> <directive> %d -- %d\n", directive->kind, directive->lineno);
	switch (directive->kind)
	{
		case DIRECTIVE::load:
			{

			}break;
	}
	return nullptr;
}


ir_info* ir_STRING(STRING* str)
{
	if (FALCON_DEBUG_IR)
		printf("|||||------> <string> %d -- %d\n", 0, str->lineno);
	ir_info* ir = (ir_info*)Calloc(1, sizeof(struct ir_info));
	ir->node_type = ntSTRING;
	//TODO: handle this differently.
	string_builder_append_chars(SB, str->str);
	return ir;
}

ir_info* ir_FUNCTION(FUNCTION* f)
{
	if (FALCON_DEBUG_IR)
		printf("|||||------> <var_decl_list> %d -- %d\n", 0, f->lineno);

	//reset register counter before begining.
	ir_info* ir = (ir_info*)Calloc(1, sizeof(struct ir_info));
	ir->node_type = ntFUNCTION;

	if (strcmp(f->func_head->function_id, "main") == 0)
	{
		return ir;
	}

	string_builder_append_chars(SB, "define ");
	ir_HEAD(f->func_head);
	string_builder_append_chars(SB, "{\n");
	ir_BODY(f->func_body);
	string_builder_append_chars(SB, "\n}\n");

	//reset register counter once done.
	//REGISTER_COUNTER = 0;
	return ir;
}

ir_info* ir_HEAD(HEAD* head)
{
	if (FALCON_DEBUG_IR)
		printf("|||||------> <head> %d -- %d\n", 0, head->lineno);
	ir_info* ir = (ir_info*)Calloc(1, sizeof(struct ir_info));
	ir->node_type = ntHEAD;
	ir_info* ir_type = ir_TYPE(head->return_type);
	string_builder_append_chars(SB, " @");
	string_builder_append_chars(SB, head->function_id);
	string_builder_append_chars(SB, "(");
	ir_info* ir_par_decl_list = ir_PAR_DECL_LIST(head->params);
	string_builder_append_chars(SB, ")\n");

	return ir;
}

ir_info* ir_TYPE(TYPE * type)
{
	if (FALCON_DEBUG_IR)
		printf("|||||------> <type> %d -- %d\n", type->kind, type->lineno);
	ir_info* ir = (ir_info*)Calloc(1, sizeof(struct ir_info));
	ir->node_type = ntTYPE;
	switch (type->kind)
	{
		case TYPE::type_idK:
			{
				//string_builder_append_chars(SB, type->val.type_id);
			}
			break;
		case TYPE::type_intK:
			{
				string_builder_append_chars(SB, "i32");
			}
			break;
		case TYPE::type_boolK:
			{
				string_builder_append_chars(SB, "i8");
			}
			break;
		case TYPE::type_arrayK:
			{
				ir_info* ir_type = ir_TYPE(type->val.type);
				string_builder_append_chars(SB, "v64");
			}
			break;
		case TYPE::type_recordK:
			{
				ir_info* ir_var_decl_list = ir_VAR_DECL_LIST(type->val.var_decl_list);
			}
			break;
		case TYPE::type_stringK:
			{}
			break;
		default: { log_internal_error(__FILE__, __func__, __LINE__, "DEFAULT CASE."); } break;
	}
	return ir;
}


ir_info* ir_PAR_DECL_LIST(PAR_DECL_LIST * par_decl_list)
{
	if (FALCON_DEBUG_IR)
		printf("|||||------> <par_decl_list> %d -- %d\n", par_decl_list->kind, par_decl_list->lineno);
	ir_info* ir = (ir_info*)Calloc(1, sizeof(struct ir_info));
	ir->node_type = ntPAR_DECL_LIST;
	switch (par_decl_list->kind)
	{
		case PAR_DECL_LIST::par_decl_list_var_decl_listK:
			{
				ir_info* ir_var_decl_list = ir_VAR_DECL_LIST(par_decl_list->val.var_decl_list);
			}
			break;
		case PAR_DECL_LIST::par_decl_list_emptyK:
			{}
			break;
		default:  break;;
	}
	return ir;
}
ir_info* ir_VAR_DECL_LIST(VAR_DECL_LIST * var_decl_list)
{
	if (FALCON_DEBUG_IR)
		printf("|||||------> <var_decl_list> %d -- %d\n", var_decl_list->kind, var_decl_list->lineno);
	ir_info* ir = (ir_info*)Calloc(1, sizeof(struct ir_info));
	ir->node_type = ntVAR_DECL_LIST;
	switch (var_decl_list->kind)
	{
		case VAR_DECL_LIST::var_decl_list_pairK:
			{
				ir_info* ir_var_type = ir_VAR_TYPE(var_decl_list->val.pair.head);
				string_builder_append_chars(SB, ", ");
				ir_info* ir_var_decl_list = ir_VAR_DECL_LIST(var_decl_list->val.pair.tail);
			}
			break;
		case VAR_DECL_LIST::var_decl_var_typeK:
			{
				ir_info* ir_var_type = ir_VAR_TYPE(var_decl_list->val.var_type);
			}
			break;
		default:  break;
	}
	return ir;
}

ir_info* ir_VAR_TYPE(VAR_TYPE * var_type)
{
	if (FALCON_DEBUG_IR)
		printf("|||||------> <var_type> %d -- %d\n", 0, var_type->lineno);
	ir_info* ir = (ir_info*)Calloc(1, sizeof(struct ir_info));
	ir->node_type = ntVAR_TYPE;
	ir_info* ir_type = ir_TYPE(var_type->type);

	//if (distinction == GLOBAL_IDENTIFIER)
	//{
	string_builder_append_chars(SB, " @");
	//}
	//else if (distinction == LOCAL_IDENTIFIER)
	//{
	//	string_builder_append_chars(SB, " %");
	//}
	string_builder_append_chars(SB, var_type->id);
	return ir;
}


ir_info* ir_BODY(BODY * body)
{
	if (FALCON_DEBUG_IR)
		printf("|||||------> <body> %d -- %d\n", 0, body->lineno);
	ir_info* ir = (ir_info*)Calloc(1, sizeof(struct ir_info));
	ir->node_type = ntBODY;
	ir_info* ir_decl_list = ir_DECL_LIST(body->decl_list);

	ir_info* ir_stm_list = ir_STM_LIST(body->stm_list);

	return ir;
}

/*
ir_info* ir_BODY_DECL_LIST(BODY_DECL_LIST* body_decl_list)
{
	if (FALCON_DEBUG_IR)
		printf("|||||------> <body_decl_list> %d -- %d\n", body_decl_list->kind, body_decl_list->lineno);
	ir_info* ir = (ir_info*)Calloc(1, sizeof(struct ir_info));
	ir->node_type = ntBODY_DECL_LIST;
	switch (body_decl_list->kind)
	{
		case BODY_DECL_LIST::body_decl_list_list:
		{
			ir_info* ir_decl_list = ir_DECL_LIST(body_decl_list->decl_list);

		}break;
		case BODY_DECL_LIST::body_decl_list_empty:
		{}
		break;
	}
	return ir;
}
*/

ir_info* ir_DECL_LIST(DECL_LIST * decl_list)
{
	if (FALCON_DEBUG_IR)
		printf("|||||------> <decl_list> %d -- %d\n", decl_list->kind, decl_list->lineno);
	ir_info* ir = (ir_info*)Calloc(1, sizeof(struct ir_info));
	ir->node_type = ntDECL_LIST;
	switch (decl_list->kind)
	{
		case DECL_LIST::decl_list_pair:
			{
				ir_info* ir_decl = ir_DECL(decl_list->val.pair.head);
				ir_info* ir_decl_list = ir_DECL_LIST(decl_list->val.pair.next);
			}
			break;
		case DECL_LIST::decl_list_empty:
			{
				//ir_info* ir_decl = ir_DECL(decl_list->val.decl);
			}
			break;
		default:  break;
	}
	return ir;
}


ir_info* ir_DECL(DECL * decl)
{
	if (FALCON_DEBUG_IR)
		printf("|||||------> <decl> %d -- %d\n", decl->kind, decl->lineno);
	ir_info* ir = (ir_info*)Calloc(1, sizeof(struct ir_info));
	ir->node_type = ntDECL;
	switch (decl->kind)
	{
		case DECL::decl_assignK:
			{
				ir_info* ir_type = ir_TYPE(decl->val.type);
			}
			break;
		case DECL::decl_functionK:
			{
				ir_info* ir_function = ir_FUNCTION(decl->val.function);
			}
			break;
		case DECL::decl_varsK:
			{
				ir_info* ir_var_decl_list = ir_VAR_DECL_LIST(decl->val.var_decl_list);
			}
			break;
		case DECL::decl_var_typ_exp:
			{
				ir_VAR_TYPE(decl->var_type);
				ir_EXP(decl->exp);
			}break;
		case DECL::decl_struct:
			{
				ir_BODY(decl->body);
			}break;
		default:  break;
	}
	return ir;
}

ir_info* ir_STM_LIST(STM_LIST * stm_list)
{
	if (FALCON_DEBUG_IR)
		printf("|||||------> <stm_list> %d -- %d\n", stm_list->kind, stm_list->lineno);
	ir_info* ir = (ir_info*)Calloc(1, sizeof(struct ir_info));
	ir->node_type = ntSTM_LIST;
	switch (stm_list->kind)
	{
		case STM_LIST::stm_list_pairK:
			{
				ir_info* ir_stm = ir_STM(stm_list->val.pair.head);
				ir_info* ir_stm_list = ir_STM_LIST(stm_list->val.pair.tail);
			}
			break;
		case STM_LIST::stm_list_empty:
			{

			}break;
		default:  break;
	}
	return ir;
}


ir_info* ir_STM(STM * stm)
{
	if (FALCON_DEBUG_IR)
		printf("|||||------> <stm> %d -- %d\n", stm->kind, stm->lineno);
	ir_info* ir = (ir_info*)Calloc(1, sizeof(struct ir_info));
	ir->node_type = ntSTM;
	switch (stm->kind)
	{
		case STM::stm_returnK:
			{
				ir_info* ir_exp = ir_EXP(stm->val.exp);
				string_builder_append_chars(SB, "ret ");
				char* type_ir = ir_type_to_string(stm->type->return_type);
				string_builder_append_chars(SB, type_ir);
				string_builder_append_chars(SB, " ");
				string_builder_append_chars(SB, ir_exp->returned_string);
				string_builder_append_chars(SB, "\n");
			}	break;
		case STM::stm_printf:
			{
				ir_info* ir_exp = ir_EXP(stm->val.exp);
			}break;
		case STM::stm_allocateK:
			{
				ir_info* ir_variable = ir_VARIABLE(stm->val.var);
			}break;
		case STM::stm_allocate_lengthK:
			{
				ir_info* ir_variable = ir_VARIABLE(stm->val.allocate_length.var);
				ir_info* ir_exp = ir_EXP(stm->val.allocate_length.exp);
			}
			break;
		case STM::stm_assignmentK:
			{
				ir_info* ir_variable = ir_VARIABLE(stm->val.assignment.var);
				ir_info* ir_exp = ir_EXP(stm->val.assignment.exp);
			}
			break;
		case STM::stm_ifK:
			{
				ir_info* ir_exp = ir_EXP(stm->val.if_stm.exp);
				ir_info* ir_stm = ir_STM(stm->val.if_stm.stm);
			}
			break;
		case STM::stm_if_elseK:
			{
				ir_info* ir_exp = ir_EXP(stm->val.if_else_stm.exp);
				ir_info* ir_stm_then = ir_STM(stm->val.if_else_stm.stm_then);
				ir_info* ir_stm_else = ir_STM(stm->val.if_else_stm.stm_else);
			}
			break;

		case STM::stm_whileK:
			{
				ir_info* ir_exp = ir_EXP(stm->val.while_stm.exp);
				ir_info* ir_stm = ir_STM(stm->val.while_stm.stm);
			}break;
		case STM::stm_for:
			{
				ir_VAR_TYPE(stm->val.for_stm.var_type);
				ir_EXP(stm->val.for_stm.exp);
				ir_STM(stm->val.for_stm.stm);
				ir_BODY(stm->val.for_stm.body);
			}
			break;

			/*
			case STM::stm_listK:
			{
				ir_info* ir_stm_list = ir_STM_LIST(stm->val.list);
			}
			break;
			*/
		default:  break;
	}
	return ir;
}


ir_info* ir_VARIABLE(VARIABLE * var)
{
	if (FALCON_DEBUG_IR)
		printf("|||||------> <var> %d -- %d\n", var->kind, var->lineno);
	ir_info* ir = (ir_info*)Calloc(1, sizeof(struct ir_info));
	ir->node_type = ntVARIABLE;
	switch (var->kind)
	{
		case VARIABLE::variable_idK:
			{
				ir->returned_type = ir_type_to_string(var->type);
				ir->variable_identifier = var->val.id;
				ir->variable_sym_table = var->type->sym_table;
				string_builder* temp = string_builder_new();
				string_builder_append_chars(temp, "%");
				string_builder_append_chars(temp, var->val.id);
				ir->returned_string = string_builder_copy_to_char_array(temp);
				string_builder_destroy(temp);
			}
			break;
		case VARIABLE::variable_arrayAccessK:
			{
				ir_info* ir_variable = ir_VARIABLE(var->val.array_access.var);
				ir_info* ir_exp = ir_EXP(var->val.array_access.exp);
			}
			break;
		case VARIABLE::variable_access_idK:
			{
				ir_info* ir_variable = ir_VARIABLE(var->val.access.var);

			}
			break;
		default: break;
	}
	return ir;
}

ir_info* ir_EXP(EXP * exp)
{
	if (FALCON_DEBUG_IR)
		printf("|||||------> <exp> %d -- %d\n", exp->kind, exp->lineno);
	/* LLVM https://llvm.org/docs/LangRef.html#id92
	 * <result> = add float 4.0, %var          ; yields float:result = 4.0 + %var
	 *
	 * ‘sub’ Instruction:
	 * <result> = sub <ty> <op1>:, <op2>         ; yields ty:result
	 * <result> = sub nuw <ty> <op1>, <op2>      ; yields ty:result
	 * <result> = sub nsw <ty> <op1>, <op2>      ; yields ty:result
	 * <result> = sub nuw nsw <ty> <op1>, <op2>  ; yields ty:result
	 */

	 /*
	  * pop both exp, do calc push on the stack.
	  */

	ir_info* ir = (ir_info*)Calloc(1, sizeof(struct ir_info));
	ir->node_type = ntEXP;
	ir->registers_in = 2;
	ir->registers_out = 1;

	switch (exp->kind)
	{
		case EXP::exp_timesK:
			{
				ir_info* ir_exp_left = ir_EXP(exp->val.op.left);
				ir_info* ir_exp_right = ir_EXP(exp->val.op.right);

				//generate a temp register that is used to store the result.
				char* result = ir_gen_temp_register();
				string_builder_append_chars(SB, result);
				string_builder_append_chars(SB, " = ");
				string_builder_append_chars(SB, "mul ");

				if (ir_exp_left->node_type == e_node_type::ntVARIABLE)
				{
					string_builder_append_chars(SB, ir_exp_left->returned_type);
				} else if (ir_exp_right->node_type == e_node_type::ntVARIABLE)
				{
					string_builder_append_chars(SB, ir_exp_right->returned_type);
				} else
				{
					string_builder_append_chars(SB, ir_type_to_string(exp->type));
				}

				string_builder_append_chars(SB, " ");
				string_builder_append_chars(SB, ir_exp_left->returned_string);
				string_builder_append_chars(SB, ", ");
				string_builder_append_chars(SB, ir_exp_right->returned_string);
				string_builder_append_chars(SB, "\n");

				char* line = string_builder_copy_to_char_array(SB);
				string_builder_clear(SB);
				linked_list_add_last(&GLOBAL_IR_LIST, line);


				//return the result back up.
				ir->returned_string = result;
			}
			break;
		case EXP::exp_divK:
			{
				ir_info* ir_exp_left = ir_EXP(exp->val.op.left);
				ir_info* ir_exp_right = ir_EXP(exp->val.op.right);

				//generate a temp register that is used to store the result.
				char* result = ir_gen_temp_register();
				string_builder_append_chars(SB, result);
				string_builder_append_chars(SB, " = ");
				string_builder_append_chars(SB, "div ");

				if (ir_exp_left->node_type == e_node_type::ntVARIABLE)
				{
					string_builder_append_chars(SB, ir_exp_left->returned_type);
				} else if (ir_exp_right->node_type == e_node_type::ntVARIABLE)
				{
					string_builder_append_chars(SB, ir_exp_right->returned_type);
				} else
				{
					string_builder_append_chars(SB, ir_type_to_string(exp->type));
				}

				string_builder_append_chars(SB, " ");
				string_builder_append_chars(SB, ir_exp_left->returned_string);
				string_builder_append_chars(SB, ", ");
				string_builder_append_chars(SB, ir_exp_right->returned_string);
				string_builder_append_chars(SB, "\n");

				char* line = string_builder_copy_to_char_array(SB);
				string_builder_clear(SB);
				linked_list_add_last(&GLOBAL_IR_LIST, line);


				//return the result back up.
				ir->returned_string = result;


			}
			break;
		case EXP::exp_plusK:
			{
				ir_info* ir_exp_left = ir_EXP(exp->val.op.left);
				ir_info* ir_exp_right = ir_EXP(exp->val.op.right);

				//generate a temp register that is used to store the result.
				char* result = ir_gen_temp_register();
				string_builder_append_chars(SB, result);
				string_builder_append_chars(SB, " = ");
				string_builder_append_chars(SB, "add ");

				if (ir_exp_left->node_type == e_node_type::ntVARIABLE)
				{
					string_builder_append_chars(SB, ir_exp_left->returned_type);
				} else if (ir_exp_right->node_type == e_node_type::ntVARIABLE)
				{
					string_builder_append_chars(SB, ir_exp_right->returned_type);
				} else
				{
					string_builder_append_chars(SB, ir_type_to_string(exp->type));
				}

				string_builder_append_chars(SB, " ");
				string_builder_append_chars(SB, ir_exp_left->returned_string);
				string_builder_append_chars(SB, ", ");
				string_builder_append_chars(SB, ir_exp_right->returned_string);
				string_builder_append_chars(SB, "\n");

				char* line = string_builder_copy_to_char_array(SB);
				string_builder_clear(SB);
				linked_list_add_last(&GLOBAL_IR_LIST, line);


				//return the result back up.
				ir->returned_string = result;
			}
			break;
		case EXP::exp_minusK:
			{
				ir_info* ir_exp_left = ir_EXP(exp->val.op.left);
				ir_info* ir_exp_right = ir_EXP(exp->val.op.right);

				//generate a temp register that is used to store the result.
				char* result = ir_gen_temp_register();
				string_builder_append_chars(SB, result);
				string_builder_append_chars(SB, " = ");
				string_builder_append_chars(SB, "sub ");

				if (ir_exp_left->node_type == e_node_type::ntVARIABLE)
				{
					string_builder_append_chars(SB, ir_exp_left->returned_type);
				} else if (ir_exp_right->node_type == e_node_type::ntVARIABLE)
				{
					string_builder_append_chars(SB, ir_exp_right->returned_type);
				} else
				{
					string_builder_append_chars(SB, ir_type_to_string(exp->type));
				}

				string_builder_append_chars(SB, " ");
				string_builder_append_chars(SB, ir_exp_left->returned_string);
				string_builder_append_chars(SB, ", ");
				string_builder_append_chars(SB, ir_exp_right->returned_string);
				string_builder_append_chars(SB, "\n");

				char* line = string_builder_copy_to_char_array(SB);
				string_builder_clear(SB);
				linked_list_add_last(&GLOBAL_IR_LIST, line);


				//return the result back up.
				ir->returned_string = result;

			}
			break;
		case EXP::exp_equalityK:
			{
				ir_info* ir_exp_left = ir_EXP(exp->val.op.left);
				ir_info* ir_exp_right = ir_EXP(exp->val.op.right);
			}
			break;
		case EXP::exp_non_equalityK:
			{
				ir_info* ir_exp_left = ir_EXP(exp->val.op.left);
				ir_info* ir_exp_right = ir_EXP(exp->val.op.right);
			}
			break;
		case EXP::exp_greaterK:
			{
				ir_info* ir_exp_left = ir_EXP(exp->val.op.left);
				ir_info* ir_exp_right = ir_EXP(exp->val.op.right);
			}
			break;
		case EXP::exp_lesserK:
			{
				ir_info* ir_exp_left = ir_EXP(exp->val.op.left);
				ir_info* ir_exp_right = ir_EXP(exp->val.op.right);
			}
			break;
		case EXP::exp_greater_equalK:
			{
				ir_info* ir_exp_left = ir_EXP(exp->val.op.left);
				ir_info* ir_exp_right = ir_EXP(exp->val.op.right);
			}
			break;
		case EXP::exp_lesser_equalK:
			{
				ir_info* ir_exp_left = ir_EXP(exp->val.op.left);
				ir_info* ir_exp_right = ir_EXP(exp->val.op.right);
			}
			break;
		case EXP::exp_andK:
			{
				ir_info* ir_exp_left = ir_EXP(exp->val.op.left);
				ir_info* ir_exp_right = ir_EXP(exp->val.op.right);

			}
			break;
		case EXP::exp_orK:
			{

				ir_info* ir_exp_left = ir_EXP(exp->val.op.left);
				ir_info* ir_exp_right = ir_EXP(exp->val.op.right);


			}
			break;
		case EXP::exp_termK:
			{
				ir_info* ir_term = ir_TERM(exp->val.term);
				ir = ir_term;
			}break;

		default:break;
	}
	return ir;
}

ir_info* ir_TERM(TERM * term)
{
	if (FALCON_DEBUG_IR)
		printf("|||||------> <term> %d -- %d\n", term->kind, term->lineno);
	ir_info* ir = (ir_info*)Calloc(1, sizeof(struct ir_info));
	ir->node_type = ntTERM;
	ir->registers_in = 0;
	ir->registers_out = 1;
	switch (term->kind)
	{
		case TERM::term_varK:
			{
				ir_info* ir_variable = ir_VARIABLE(term->val.var);
				ir = ir_variable;
			}
			break;
		case TERM::term_id_act_listK:
			{
				ir->node_type = TERM_FUNCTION_CALL;
				char* result = ir_gen_temp_register();
				string_builder_append_chars(SB, result);
				string_builder_append_chars(SB, " = ");
				string_builder_append_chars(SB, "call ");
				type_info_imp* type = lookup_once(term->type->sym_table, term->val.id_act_list.id, term->lineno);
				char* type_ir = ir_type_to_string(type);
				string_builder_append_chars(SB, type_ir);
				string_builder_append_chars(SB, " @");
				string_builder_append_chars(SB, term->val.id_act_list.id);
				string_builder_append_chars(SB, "(");
				ir_info* ir_act_list = ir_ACT_LIST(term->val.id_act_list.act_list);
				string_builder_append_chars(SB, ")\n");

				ir->returned_string = result;
			}
			break;
		case TERM::term_parenthesis_expressionK:
			{
				ir_info* ir_exp = ir_EXP(term->val.exp);
				ir = ir_exp;
			}
			break;
		case TERM::term_negationK:
			{
				string_builder_append_chars(SB, "neg bool ");
				ir_info* ir_term = ir_TERM(term->val.term);
			}
			break;
		case TERM::term_absoluteK:
			{
				ir_info* ir_exp = ir_EXP(term->val.exp);
			}
			break;
		case TERM::term_numK:
			{
				ir->returned_type = ir_type_to_string(term->type);
				string_builder* temp = string_builder_new();
				string_builder_append_int(temp, &term->val.num);
				ir->returned_string = string_builder_copy_to_char_array(temp);
				string_builder_destroy(temp);
			}
			break;
		case TERM::term_trueK:
			{
				ir->returned_type = ir_type_to_string(term->type);
				string_builder* temp = string_builder_new();
				string_builder_append_chars(temp, "true");
				ir->returned_string = string_builder_copy_to_char_array(temp);
				string_builder_destroy(temp);
			}
			break;
		case TERM::term_falseK:
			{
				ir->returned_type = ir_type_to_string(term->type);
				string_builder* temp = string_builder_new();
				string_builder_append_chars(temp, "false");
				ir->returned_string = string_builder_copy_to_char_array(temp);
				string_builder_destroy(temp);
			}
			break;
		case TERM::term_nullK:
			{
				ir->returned_type = ir_type_to_string(term->type);
				string_builder* temp = string_builder_new();
				string_builder_append_chars(temp, "null");
				ir->returned_string = string_builder_copy_to_char_array(temp);
				string_builder_destroy(temp);
			}
			break;
		case TERM::term_stringK:
			{
				ir_info* ir_string = ir_STRING(term->val.string);
				ir = ir_string;
				ir->returned_type = ir_type_to_string(term->type);
				string_builder* temp = string_builder_new();
				string_builder_append_chars(temp, ir_string->returned_string);
				ir->returned_string = string_builder_copy_to_char_array(temp);
				string_builder_destroy(temp);
			}
			break;
		default:  break;
	}
	return ir;
}


ir_info* ir_ACT_LIST(ACT_LIST * act_list)
{
	if (FALCON_DEBUG_IR)
		printf("|||||------> <act_list> %d -- %d\n", act_list->kind, act_list->lineno);
	ir_info* ir = (ir_info*)Calloc(1, sizeof(struct ir_info));
	ir->node_type = ntACT_LIST;
	switch (act_list->kind)
	{
		case ACT_LIST::act_list_exp_listK:
			{
				ir_info* ir_exp_list = ir_EXP_LIST(act_list->exp_list);
			}
			break;
		case ACT_LIST::act_list_emptyK:
			{}
			break;
			{}
		default: { log_internal_error(__FILE__, __func__, __LINE__, "default case."); } break;
	}
	return ir;
}


ir_info* ir_EXP_LIST(EXP_LIST * exp_list)
{
	if (FALCON_DEBUG_IR)
		printf("|||||------> <exp_list> %d -- %d\n", exp_list->kind, exp_list->lineno);
	ir_info* ir = (ir_info*)Calloc(1, sizeof(struct ir_info));
	ir->node_type = ntEXP_LIST;
	switch (exp_list->kind)
	{
		case EXP_LIST::exp_list_pairK:
			{
				ir_info* ir_exp = ir_EXP(exp_list->val.pair.head);
				string_builder_append_chars(SB, ir_exp->returned_type);
				string_builder_append_chars(SB, " ");
				string_builder_append_chars(SB, ir_exp->returned_string);
				string_builder_append_chars(SB, ", ");
				ir_info* ir_exp_list = ir_EXP_LIST(exp_list->val.pair.tail);
			}
			break;
		case EXP_LIST::exp_list_expressionK:
			{
				ir_info* ir_exp = ir_EXP(exp_list->val.exp);
				string_builder_append_chars(SB, ir_exp->returned_type);
				string_builder_append_chars(SB, " ");
				string_builder_append_chars(SB, ir_exp->returned_string);
			}
			break;
		default:  break;
	}
	return ir;
}

