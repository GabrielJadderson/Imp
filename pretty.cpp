#include <stdio.h>
#include "pretty.h"
#include "falcon_string.h"
#include "file_io.h"
#include <cstdlib>
#include "logger.h"

extern char FALCON_DEBUG_PARSER;
extern char FALCON_DEBUG_AST;

static string_builder* SB;

void pretty_print(AST_NODE* AST)
{

	if (FALCON_DEBUG_AST)
	{
		SB = string_builder_new();

		string_builder_append_chars(SB, "\\documentclass{article} \n");
		string_builder_append_chars(SB, "\\newcommand{\\IS}{:} \n");
		string_builder_append_chars(SB, "\\newcommand{\\OR}{$|$} \n");
		string_builder_append_chars(SB, "\\newcommand{\\T}[1]{{\\bf #1}} \n");
		string_builder_append_chars(SB, "\\newcommand{\\NT}[1]{$\\langle$#1$\\rangle$} \n");
		string_builder_append_chars(SB, "\\newcommand{\\bigfont}{\\usefont{OT1}{cmbig}{m}{n}} \n");
		string_builder_append_chars(SB, "\\newcommand{\\MyHuge}[1]{{\\bigfont\\fontsize{2.2cm}{2.42cm}\\selectfont #1}} \n");
		string_builder_append_chars(SB, "\\usepackage[utf8]{inputenc} \n");
		string_builder_append_chars(SB, "\\usepackage{times} \n");
		string_builder_append_chars(SB, "\\usepackage{float} \n");
		string_builder_append_chars(SB, "\\usepackage[super]{nth} \n");
		string_builder_append_chars(SB, "\\usepackage{ textcomp } \n");
		string_builder_append_chars(SB, "\\usepackage{tikz-qtree} \n");
		string_builder_append_chars(SB, "\\usepackage{lmodern} \n");
		string_builder_append_chars(SB, "\\usepackage{mathtools}\n");
		string_builder_append_chars(SB, "\\usepackage{amsmath}\n");
		string_builder_append_chars(SB, "\\usepackage{tikz}\n");
		string_builder_append_chars(SB, "\\newcommand{\\dittotikz}{\n");
		string_builder_append_chars(SB, "\\tikz{\n");
		string_builder_append_chars(SB, "\\draw [line width=0.12ex] (-0.2ex,0) -- +(0,0.8ex)\n");
		string_builder_append_chars(SB, "(0.2ex,0) -- +(0,0.8ex);\n");
		string_builder_append_chars(SB, "\\draw [line width=0.08ex] (-0.6ex,0.4ex) -- +(-1.5em,0)\n");
		string_builder_append_chars(SB, "(0.6ex,0.4ex) -- +(1.5em,0);\n");
		string_builder_append_chars(SB, "}\n");
		string_builder_append_chars(SB, "}\n");
		string_builder_append_chars(SB, "\\setlength{\\parindent}{0em}\n");
		string_builder_append_chars(SB, "\\setlength{\\parskip}{1ex plus .1ex minus .1ex}\n");
		string_builder_append_chars(SB, "\\setlength{\\itemsep}{0ex}\n");
		string_builder_append_chars(SB, "\\DeclareFontFamily{OT1}{cmbig}{}\n");
		string_builder_append_chars(SB, "\\DeclareFontShape{OT1}{cmbig}{m}{n}{ <-> cmssbx10 }{}\n");
		string_builder_append_chars(SB, "\\usepackage{xcolor}\n");
		string_builder_append_chars(SB, "\\usepackage{listings}\n");
		string_builder_append_chars(SB, "\\usepackage{color}\n");
		string_builder_append_chars(SB, "\\begin{document} \n");

		string_builder_append_chars(SB, "\\begin{figure}[H] \n");
		string_builder_append_chars(SB, "\\resizebox{0.9\\textwidth}{!}{ \n");
		string_builder_append_chars(SB, "\\Tree \n");

	}

	if (FALCON_DEBUG_PARSER || FALCON_DEBUG_AST)
		prettyAST(AST);

	if (FALCON_DEBUG_AST)
	{
		string_builder_append_chars(SB, " } \n");
		string_builder_append_chars(SB, "\\end{figure}\n");
		string_builder_append_chars(SB, "\\end{document}\n");

		char* str = string_builder_copy_to_char_array(SB);
		string_builder_destroy(SB);

		//create file
		file_write_string("AST.tex", str);
		system("wsl.exe pdflatex AST.tex");

		if (remove("AST.tex") == 0)
		{
			remove("AST.log");
			remove("AST.aux");
		}
		else
		{
			log_internal_error(__FILE__, __func__, __LINE__, "FAILED TO DELETE AST.TEX");
			exit(1);
		}

	}
}

void prettyAST(AST_NODE* AST)
{
	if (FALCON_DEBUG_AST)
	{
		string_builder_append_chars(SB, "[.AST \n");
		prettyDIRECTIVE_LIST(AST->directive_list);
		prettyBODY(AST->body);
		string_builder_append_chars(SB, " ] \n");

	}
}

void prettyDIRECTIVE_LIST(DIRECTIVE_LIST* directive_list)
{
	if (FALCON_DEBUG_AST)
	{
		string_builder_append_chars(SB, "[.directive\\_list \n");
		switch (directive_list->kind)
		{
			case DIRECTIVE_LIST::pair:
			{
				prettyDIRECTIVE_LIST(directive_list->val.pair.directive_list);
				prettyDIRECTIVE(directive_list->val.pair.directive);
			}
			break;
			case DIRECTIVE_LIST::empty:
			{
				string_builder_append_chars(SB, " (empty) \n");
			}
			break;

		}
		string_builder_append_chars(SB, " ] \n");

	}
}
void prettyDIRECTIVE(DIRECTIVE* directive)
{
	if (FALCON_DEBUG_AST)
	{
		string_builder_append_chars(SB, "[.directive \n");
		switch (directive->kind)
		{
			case DIRECTIVE::load:
			{
				string_builder_append_chars(SB, " load \n");
			}
			break;
		}
		string_builder_append_chars(SB, " ] \n");

	}
}

void prettySTRING(STRING* s)
{

	if (FALCON_DEBUG_AST)
	{
		string_builder_append_chars(SB, "[.string \n");
		string_builder_append_chars(SB, "\"");
		string_builder_append_chars(SB, s->str);
		string_builder_append_chars(SB, "\"");
		string_builder_append_chars(SB, " ] \n");
	}
}

void prettyFUNCTION(FUNCTION* f)
{
	if (FALCON_DEBUG_AST)
	{
		string_builder_append_chars(SB, "[.function \n");
		prettyHEAD(f->func_head);
		prettyBODY(f->func_body);
		string_builder_append_chars(SB, " ] \n");
	}

}

void prettyHEAD(HEAD* head)
{
	if (FALCON_DEBUG_AST)
	{
		string_builder_append_chars(SB, "[.head \n");
		string_builder_append_chars(SB, " func ");
		string_builder_append_chars(SB, head->function_id);
		string_builder_append_chars(SB, " ");
		prettyPAR_DECL_LIST(head->params);
		string_builder_append_chars(SB, " : ");
		prettyTYPE(head->return_type);
		string_builder_append_chars(SB, " ] \n");
	}

}


void prettyTYPE(TYPE* type)
{
	if (FALCON_DEBUG_AST)
	{
		string_builder_append_chars(SB, "[.type \n");
		switch (type->kind)
		{
			case TYPE::type_idK:
			string_builder_append_chars(SB, type->val.type_id);
			break;
			case TYPE::type_intK:
			string_builder_append_chars(SB, " int ");
			break;
			case TYPE::type_boolK:
			string_builder_append_chars(SB, " bool ");
			break;
			case TYPE::type_arrayK:
			string_builder_append_chars(SB, " array of ");
			prettyTYPE(type->val.type);
			break;
			case TYPE::type_recordK:
			string_builder_append_chars(SB, " record of ");
			prettyVAR_DECL_LIST(type->val.var_decl_list);
			break;
			case TYPE::type_stringK:
			string_builder_append_chars(SB, " string ");
			break;
			default:  break;
		}
		string_builder_append_chars(SB, " ] \n");
	}

}


void prettyPAR_DECL_LIST(PAR_DECL_LIST* par_decl_list)
{
	if (FALCON_DEBUG_AST)
	{
		string_builder_append_chars(SB, "[.par\\_decl\\_list \n");
		switch (par_decl_list->kind)
		{
			case PAR_DECL_LIST::par_decl_list_var_decl_listK:
			prettyVAR_DECL_LIST(par_decl_list->val.var_decl_list);
			break;
			case PAR_DECL_LIST::par_decl_list_emptyK:
			string_builder_append_chars(SB, " empty ");
			break;
			default: break;;
		}
		string_builder_append_chars(SB, " ] \n");
	}
}
void prettyVAR_DECL_LIST(VAR_DECL_LIST* var_decl_list)
{
	if (FALCON_DEBUG_AST)
	{
		string_builder_append_chars(SB, "[.var\\_decl\\_list \n");
		switch (var_decl_list->kind)
		{
			case VAR_DECL_LIST::var_decl_list_pairK:
			prettyVAR_TYPE(var_decl_list->val.pair.head);
			string_builder_append_chars(SB, " , ");
			prettyVAR_DECL_LIST(var_decl_list->val.pair.tail);
			break;
			case VAR_DECL_LIST::var_decl_var_typeK:
			prettyVAR_TYPE(var_decl_list->val.var_type);
			break;
			default:  break;
		}
		string_builder_append_chars(SB, " ] \n");
	}

}

void prettyVAR_TYPE(VAR_TYPE* var_type)
{
	if (FALCON_DEBUG_AST)
	{
		string_builder_append_chars(SB, "[.var\\_type ");
		string_builder_append_chars(SB, var_type->id);
		string_builder_append_chars(SB, " : ");
		prettyTYPE(var_type->type);
		string_builder_append_chars(SB, " ] \n");
	}

}


void prettyBODY(BODY* body)
{
	if (FALCON_DEBUG_AST)
	{
		string_builder_append_chars(SB, "[.body \n");
		prettyDECL_LIST(body->decl_list);
		prettySTM_LIST(body->stm_list);
		string_builder_append_chars(SB, " ] \n");
	}
}
/*
void prettyBODY_DECL_LIST(BODY_DECL_LIST* body_decl_list)
{
	if (FALCON_DEBUG_AST)
	{
		string_builder_append_chars(SB, "[.body\\_decl\\_list \n");

		switch (body_decl_list->kind)
		{
			case BODY_DECL_LIST::body_decl_list_list:
			{
				prettyDECL_LIST(body_decl_list->decl_list);
			}
			break;
			case BODY_DECL_LIST::body_decl_list_empty:
			{
				string_builder_append_chars(SB, " (empty) ");
			}break;
		}

		string_builder_append_chars(SB, " ] \n");
	}
}
*/

void prettyDECL_LIST(DECL_LIST* decl_list)
{
	if (FALCON_DEBUG_AST)
	{
		string_builder_append_chars(SB, "[.decl\\_list \n");

		switch (decl_list->kind)
		{
			case DECL_LIST::decl_list_pair:
			{
				prettyDECL(decl_list->val.pair.head);
				prettyDECL_LIST(decl_list->val.pair.next);
			}
			break;
			case DECL_LIST::decl_list_empty:
			{
				//prettyDECL(decl_list->val.decl);
			}break;
			default:  break;
		}

		string_builder_append_chars(SB, " ] \n");
	}

}

void prettyDECL(DECL* decl)
{
	if (FALCON_DEBUG_AST)
	{
		string_builder_append_chars(SB, "[.declaration \n");
		switch (decl->kind)
		{
			case DECL::decl_assignK:
			string_builder_append_chars(SB, " type ");
			string_builder_append_chars(SB, decl->decl_id);
			string_builder_append_chars(SB, " = ");
			prettyTYPE(decl->val.type);
			break;
			case DECL::decl_functionK:
			prettyFUNCTION(decl->val.function);
			break;
			case DECL::decl_varsK:
			prettyVAR_DECL_LIST(decl->val.var_decl_list);
			break;
			case DECL::decl_var_typ_exp:
			{
				prettyVAR_TYPE(decl->var_type);
				string_builder_append_chars(SB, " = ");
				prettyEXP(decl->exp);
				string_builder_append_chars(SB, " ; ");
			}break;
			case DECL::decl_struct:
			{
				string_builder_append_chars(SB, " ");
				string_builder_append_chars(SB, decl->decl_id);
				string_builder_append_chars(SB, " :: struct  { ");
				prettyBODY(decl->body);
				string_builder_append_chars(SB, " } ");
			}break;
			default: break;
		}
		string_builder_append_chars(SB, " ] \n");
	}

}


void prettySTM_LIST(STM_LIST* stm_list)
{
	if (FALCON_DEBUG_AST)
	{
		string_builder_append_chars(SB, "[.stm\\_list \n");
		switch (stm_list->kind)
		{
			case STM_LIST::stm_list_pairK:
			prettySTM(stm_list->val.pair.head);
			prettySTM_LIST(stm_list->val.pair.tail);
			break;
			case STM_LIST::stm_list_empty:
			break;
			default: break;
		}
		string_builder_append_chars(SB, " ] \n");
	}

}


void prettySTM(STM* stm)
{
	if (FALCON_DEBUG_AST)
	{
		string_builder_append_chars(SB, "[.stm \n");
		switch (stm->kind)
		{
			case STM::stm_returnK:
			string_builder_append_chars(SB, " return ");
			prettyEXP(stm->val.exp);
			string_builder_append_chars(SB, " ; ");
			break;
			case STM::stm_printf:
			string_builder_append_chars(SB, " printf ");
			prettyEXP(stm->val.exp);
			string_builder_append_chars(SB, " ; ");
			break;
			case STM::stm_allocateK:
			string_builder_append_chars(SB, " allocate ");
			prettyVARIABLE(stm->val.var);
			string_builder_append_chars(SB, " ; ");
			break;
			case STM::stm_allocate_lengthK:
			string_builder_append_chars(SB, " allocate ");
			prettyVARIABLE(stm->val.allocate_length.var);
			string_builder_append_chars(SB, " of length ");
			prettyEXP(stm->val.allocate_length.exp);
			string_builder_append_chars(SB, " ; ");
			break;
			case STM::stm_assignmentK:
			prettyVARIABLE(stm->val.assignment.var);
			string_builder_append_chars(SB, " $=$ ");
			prettyEXP(stm->val.assignment.exp);
			string_builder_append_chars(SB, " ; ");
			break;
			case STM::stm_ifK:
			string_builder_append_chars(SB, " if ");
			prettyEXP(stm->val.if_stm.exp);
			string_builder_append_chars(SB, " then ");
			prettySTM(stm->val.if_stm.stm);
			break;
			case STM::stm_if_elseK:
			string_builder_append_chars(SB, " if ");
			prettyEXP(stm->val.if_else_stm.exp);
			string_builder_append_chars(SB, " then ");
			prettySTM(stm->val.if_else_stm.stm_then);
			string_builder_append_chars(SB, " else ");
			prettySTM(stm->val.if_else_stm.stm_else);
			break;
			case STM::stm_whileK:
			string_builder_append_chars(SB, " while ");
			prettyEXP(stm->val.while_stm.exp);
			string_builder_append_chars(SB, " do ");
			prettySTM(stm->val.while_stm.stm);
			break;
			/*
			 case STM::stm_listK:
			string_builder_append_chars(SB, " \\{ ");
			prettySTM_LIST(stm->val.list);
			string_builder_append_chars(SB, " \\} ");
			break;
			*/
			case STM::stm_for:
			{
				prettyVAR_TYPE(stm->val.for_stm.var_type);
				prettyEXP(stm->val.for_stm.exp);
				prettySTM(stm->val.for_stm.stm);
				prettyBODY(stm->val.for_stm.body);
			}
			break;
			default:  break;
		}
		string_builder_append_chars(SB, " ] \n");
	}

}


void prettyVARIABLE(VARIABLE* var)
{
	if (FALCON_DEBUG_AST)
	{
		string_builder_append_chars(SB, "[.variable \n");
		switch (var->kind)
		{
			case VARIABLE::variable_idK:
			string_builder_append_chars(SB, var->val.id);
			break;
			case VARIABLE::variable_arrayAccessK:
			prettyVARIABLE(var->val.array_access.var);
			string_builder_append_chars(SB, " \\text{[} ");
			prettyEXP(var->val.array_access.exp);
			string_builder_append_chars(SB, " \\text{]} ");
			break;
			case VARIABLE::variable_access_idK:
			prettyVARIABLE(var->val.access.var);
			string_builder_append_chars(SB, " .");
			string_builder_append_chars(SB, var->val.access.id);
			string_builder_append_chars(SB, " ");
			break;
			default: break;
		}
		string_builder_append_chars(SB, " ] \n");
	}

}

void prettyEXP(EXP* exp)
{
	if (FALCON_DEBUG_AST)
	{
		string_builder_append_chars(SB, "[.exp \n");
		switch (exp->kind)
		{
			case EXP::exp_timesK:
			prettyEXP(exp->val.op.left);
			string_builder_append_chars(SB, " $*$ ");
			prettyEXP(exp->val.op.right);
			break;
			case EXP::exp_divK:
			prettyEXP(exp->val.op.left);
			string_builder_append_chars(SB, " $/$ ");
			prettyEXP(exp->val.op.right);
			break;
			case EXP::exp_plusK:
			prettyEXP(exp->val.op.left);
			string_builder_append_chars(SB, " $+$ ");
			prettyEXP(exp->val.op.right);
			break;
			case EXP::exp_minusK:
			prettyEXP(exp->val.op.left);
			string_builder_append_chars(SB, " $-$ ");
			prettyEXP(exp->val.op.right);
			break;
			case EXP::exp_equalityK:
			prettyEXP(exp->val.op.left);
			string_builder_append_chars(SB, " $==$ ");
			prettyEXP(exp->val.op.right);
			break;
			case EXP::exp_non_equalityK:
			prettyEXP(exp->val.op.left);
			string_builder_append_chars(SB, " $\\neq$ ");
			prettyEXP(exp->val.op.right);
			break;
			case EXP::exp_greaterK:
			prettyEXP(exp->val.op.left);
			string_builder_append_chars(SB, " $>$ ");
			prettyEXP(exp->val.op.right);
			break;
			case EXP::exp_lesserK:
			prettyEXP(exp->val.op.left);
			string_builder_append_chars(SB, " $<$ ");
			prettyEXP(exp->val.op.right);
			break;
			case EXP::exp_greater_equalK:
			prettyEXP(exp->val.op.left);
			string_builder_append_chars(SB, " $\\geq$ ");
			prettyEXP(exp->val.op.right);
			break;
			case EXP::exp_lesser_equalK:
			prettyEXP(exp->val.op.left);
			string_builder_append_chars(SB, " $\\leq$ ");
			prettyEXP(exp->val.op.right);
			break;
			case EXP::exp_andK:
			prettyEXP(exp->val.op.left);
			string_builder_append_chars(SB, " \\&\\& ");
			prettyEXP(exp->val.op.right);
			break;
			case EXP::exp_orK:
			prettyEXP(exp->val.op.left);
			string_builder_append_chars(SB, " \\|\\| ");
			prettyEXP(exp->val.op.right);
			break;
			case EXP::exp_termK:
			prettyTERM(exp->val.term);
			break;

			default:  break;
		}
		string_builder_append_chars(SB, " ] \n");
	}
}

void prettyTERM(TERM* term)
{
	if (FALCON_DEBUG_AST)
	{
		string_builder_append_chars(SB, "[.term \n");
		switch (term->kind)
		{
			case TERM::term_varK:
			prettyVARIABLE(term->val.var);
			break;
			case TERM::term_id_act_listK:
			string_builder_append_chars(SB, term->val.id_act_list.id);
			prettyACT_LIST(term->val.id_act_list.act_list);
			break;
			case TERM::term_parenthesis_expressionK:
			string_builder_append_chars(SB, " ( ");
			prettyEXP(term->val.exp);
			string_builder_append_chars(SB, " ) ");
			break;
			case TERM::term_negationK:
			string_builder_append_chars(SB, " ! ");
			prettyTERM(term->val.term);
			break;
			case TERM::term_absoluteK:
			string_builder_append_chars(SB, " | ");
			prettyEXP(term->val.exp);
			string_builder_append_chars(SB, " | ");
			break;
			case TERM::term_numK:
			string_builder_append_int(SB, &term->val.num);
			break;
			case TERM::term_trueK:
			string_builder_append_chars(SB, " true ");
			break;
			case TERM::term_falseK:
			string_builder_append_chars(SB, " false ");
			break;
			case TERM::term_nullK:
			string_builder_append_chars(SB, " null ");
			break;
			case TERM::term_stringK:
			prettySTRING(term->val.string);
			break;
			default: break;
		}
		string_builder_append_chars(SB, " ] \n");
	}

}


void prettyACT_LIST(ACT_LIST* act_list)
{
	if (FALCON_DEBUG_AST)
	{
		string_builder_append_chars(SB, "[.act\\_list ");
		switch (act_list->kind)
		{
			case ACT_LIST::act_list_exp_listK:
			prettyEXP_LIST(act_list->exp_list);
			break;
			case ACT_LIST::act_list_emptyK:
			string_builder_append_chars(SB, " empty ");
			break;
			default:  break;
		}
		string_builder_append_chars(SB, " ] \n");
	}

}


void prettyEXP_LIST(EXP_LIST * exp_list)
{
	if (FALCON_DEBUG_AST)
	{
		string_builder_append_chars(SB, "[.exp\\_list ");
		switch (exp_list->kind)
		{
			case EXP_LIST::exp_list_pairK:
			prettyEXP(exp_list->val.pair.head);
			string_builder_append_chars(SB, " , ");
			prettyEXP_LIST(exp_list->val.pair.tail);
			break;
			case EXP_LIST::exp_list_expressionK:
			prettyEXP(exp_list->val.exp);
			break;
			default: break;
		}
		string_builder_append_chars(SB, " ] \n");
	}

}
