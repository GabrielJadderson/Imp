#include "codegen.h"
#include <stdio.h>
#include "falcon_string.h"
#include "symbol.h"
#include "linked_list.h"
#include "logger.h"
#include "array.h"
#include "memory.h"
#include "file_io.h"
#include <unordered_map>
#include "pre_ir_weed.h"
#include "ir.h"
#include "emit_util.h"
//https://en.wikibooks.org/wiki/X86_Assembly/NASM_Syntax

std::unordered_map<std::string, bool> LOADED_FUNCTIONS;

extern std::unordered_map<std::string, call_node*> CALL_TABLE;
extern std::unordered_map<std::string, FUNCTION*> FUNCTION_LIST;
extern std::unordered_map<std::string, VAR_TYPE*> GLOBAL_VARIABLE_LIST;
extern std::unordered_map<std::string, FUNCTION*> GLOBAL_FUNCTION_LIST;
extern std::unordered_map<void*, array*> GLOBAL_USED_REGISTER_MAP;
extern array* calling_list;

static string_builder* SB;

int REGISTER_COUNTER;

extern char FALCON_DEBUG_CODEGEN;
int label_counter = 0;

//The memery offset 4 = 32bit, 8 = 64bit
static int GLOBAL_VARIABLE_DECLARATIONS = 0;
int GLOBAL_OFFSET_MULTIPLIER = 8;
int CODEGEN_STACK_OFFSET = 0;
int CODEGEN_STACK_COUNT = 0;

array* array_BSS_SECTION;
array* array_DATA_SECTION;

linked_list* GLOBAL_CODE_LIST;
linked_list* GLOBAL_VARIABLE_DECLARATION_LIST;
linked_list* READ_ONLY_DATA_SECTION;
linked_list* GLOBAL_LATE_DECLARATION_LIST;

FUNCTION* GLOBAL_CURRENT_FUNCTION;
bool GLOBAL_CURRENT_FUNCTION_IS_MAIN;

static int NUMBER_OF_DECLARED_VARIABLES_IN_CURRENT_SCOPE = 0;
static int DECLARED_VARIABLES_IN_CURRENT_SCOPE_OFFSET = 0;

static bool IN_GLOBAL_SCOPE = true;


char* codegen_gen_temp_register()
{
	string_builder* s = string_builder_new();
	string_builder_append_chars(s, "%rax");
	//string_builder_append_int(s, &REGISTER_COUNTER);
	char*result = string_builder_copy_to_char_array(s);
	string_builder_destroy(s);
	REGISTER_COUNTER++;
	return result;
}

char* codegen_create_unique_label_with_seed(char* seed)
{
	string_builder* sb = string_builder_new();
	string_builder_append_chars(sb, seed);
	string_builder_append_int(sb, &label_counter);
	char* str_final = string_builder_copy_to_char_array(sb);
	string_builder_destroy(sb);
	label_counter++;
	return str_final;
}

char* codegen_create_unique_label()
{
	return codegen_create_unique_label_with_seed("label_");
}


char* codegen_build_and_get_DATA_section()
{
	string_builder* sb = string_builder_new();
	string_builder_append_chars(sb, ".data\n");
	string_builder_append_chars(sb, ".align 8\n");
	string_builder_append_chars(sb, "heap_pointer: \n.space 16394\n");
	string_builder_append_chars(sb, "heap_next: \n.quad 0\n");
	size_t arr_length = array_DATA_SECTION->length;
	for (size_t i = 0; i < arr_length; ++i) //parse rest of data.
	{
		string_builder_append_chars(sb, (char*)array_at(array_DATA_SECTION, i));
		string_builder_append_chars(sb, "\n");
	}
	char* str = string_builder_copy_to_char_array(sb);
	string_builder_destroy(sb);
	return str;
}

char* codegen_build_and_get_BSS_section()
{
	string_builder* sb = string_builder_new();
	string_builder_append_chars(sb, "\n.bss\n"); //TODO: MAKE THE ENTIRE CODE_GEN WORK BY WITHOUT THIS NEWLINE IN FRONT OF THE SECTION.
	size_t arr_length = array_BSS_SECTION->length;
	for (size_t i = 0; i < arr_length; ++i) //parse rest of data.
	{
		string_builder_append_chars(sb, (char*)array_at(array_BSS_SECTION, i));
	}
	char* str = string_builder_copy_to_char_array(sb);
	string_builder_destroy(sb);
	return str;
}

linked_list* codegen(AST_NODE* AST, SymbolTable* root)
{
	label_counter = 0;
	array_BSS_SECTION = array_new();
	array_DATA_SECTION = array_new();
	GLOBAL_CODE_LIST = linked_list_new();
	GLOBAL_VARIABLE_DECLARATION_LIST = linked_list_new();
	GLOBAL_LATE_DECLARATION_LIST = linked_list_new();
	READ_ONLY_DATA_SECTION = linked_list_new();
	SB = string_builder_new();
	REGISTER_COUNTER = 0;

	codegen_AST(AST);


	linked_list_add_first(&GLOBAL_CODE_LIST, "\n.text\n");
	linked_list_add_first(&GLOBAL_CODE_LIST, "\n.globl  main\n");


	linked_list_add_first(&READ_ONLY_DATA_SECTION, "printf_format_nl: .asciz \"\\n\"\n");
	linked_list_add_first(&READ_ONLY_DATA_SECTION, "printf_format_int: .asciz \"%d\\n\"\n");
	linked_list_add_first(&READ_ONLY_DATA_SECTION, "printf_format_string: .asciz \"%s\\n\"\n");
	linked_list_add_first(&READ_ONLY_DATA_SECTION, ".section\t.rodata\n");
	char* read_only_data = linked_list_serialize_to_char_array(&READ_ONLY_DATA_SECTION);
	linked_list_add_first(&GLOBAL_CODE_LIST, read_only_data);

	linked_list_add_first(&GLOBAL_CODE_LIST, codegen_build_and_get_BSS_section());
	linked_list_add_first(&GLOBAL_CODE_LIST, codegen_build_and_get_DATA_section());


	linked_list_add_last(&GLOBAL_CODE_LIST, "\n");

	char* data = string_builder_copy_to_char_array(SB);
	string_builder_destroy(SB);
	linked_list_add_last(&GLOBAL_CODE_LIST, data);



	linked_list_print(&GLOBAL_CODE_LIST);
	file_write_linked_list("output.s", &GLOBAL_CODE_LIST);
	file_write_linked_list("output.asm", &GLOBAL_CODE_LIST);

	log_warning("emit done");

	return GLOBAL_CODE_LIST;
}

code_info* codegen_AST(AST_NODE* ast)
{
	codegen_DIRECTIVE_LIST(ast->directive_list);
	//only generate for decl list, stm's not allowed in outer source code.
	array* array_of_global_declarations = array_new();//TODO: REMOVE THIS INTO AST.
	codegen_DECL_LIST(ast->body->decl_list, array_of_global_declarations);


	//find main
	FUNCTION* f = static_cast<FUNCTION*>(FUNCTION_LIST["main"]);
	if (f)
	{
		GLOBAL_CURRENT_FUNCTION = f;
		GLOBAL_CURRENT_FUNCTION_IS_MAIN = true;
		IN_GLOBAL_SCOPE = false;
		string_builder_append_chars(SB, "main:\n");
		string_builder_append_chars(SB, push_reg(RBP));
		string_builder_append_chars(SB, "movq\t%rsp, %rbp\n");
		//string_builder_append_chars(SB, "subq\t$8, %rsp\t\n");
		string_builder_append_chars(SB, push_reg(RBX));
		string_builder_append_chars(SB, push_reg(R12));
		string_builder_append_chars(SB, push_reg(R13));
		string_builder_append_chars(SB, push_reg(R14));
		string_builder_append_chars(SB, push_reg(R15));

		f->STACK_ALLIGNMENT_COUNT = 5;
		f->STACK_ALLIGNMENT_VALUE = 5 * 8;



		//reserve space for parameters
		//int decl_space = (GLOBAL_VARIABLE_DECLARATIONS + 1) * GLOBAL_OFFSET_MULTIPLIER;
		//string_builder_append_chars(SB, "subq\t$");
		//string_builder_append_int(SB, &decl_space);
		//string_builder_append_chars(SB, ", %rsp\n");
		log_info("102");


		codegen_HEAD(f->func_head);
		log_info("103");
		codegen_BODY(f->func_body);
		log_info("104");


		if (GLOBAL_LATE_DECLARATION_LIST->Length > 0)
		{
			char* late_declarations = linked_list_serialize_to_char_array(&GLOBAL_LATE_DECLARATION_LIST);
			string_builder_append_chars(SB, late_declarations);
		}


		//string_builder_append_chars(SB, "addq\t$");
		//string_builder_append_int(SB, &decl_space);
		//string_builder_append_chars(SB, ", %rsp\n");
		string_builder_append_chars(SB, pop_reg(R15));
		string_builder_append_chars(SB, pop_reg(R14));
		string_builder_append_chars(SB, pop_reg(R13));
		string_builder_append_chars(SB, pop_reg(R12));
		string_builder_append_chars(SB, pop_reg(RBX));

		//string_builder_append_chars(SB, "addq\t$8, %rsp\t\n");
		string_builder_append_chars(SB, "movq\t%rbp, %rsp\n");
		string_builder_append_chars(SB, pop_reg(RBP));
		string_builder_append_chars(SB, "retq\n");
	}
	else
	{
		fprintf(stderr, "\x1b[37;1m");
		fprintf(stderr, "%d: ", ast->body->lineno); //print  lineno
		fprintf(stderr, "\x1b[0m"); //reset color.
		fprintf(stderr, "\x1b[31;1m"); //start print color magenta.
		fprintf(stderr, "Error: "); //start print color magenta.
		fprintf(stderr, "\x1b[0m"); //reset color.
		fprintf(stderr, "main function was not found.");
		fprintf(stderr, "\x1b[0m\n");
		exit(1);
	}

	return nullptr;
}


code_info* codegen_DIRECTIVE_LIST(DIRECTIVE_LIST* directive_list)
{
	if (FALCON_DEBUG_CODEGEN)
		printf("|||||------> <directive_list> %d -- %d\n", directive_list->kind, directive_list->lineno);
	switch (directive_list->kind)
	{
		case DIRECTIVE_LIST::pair:
		{
			code_info* ti_directive_list = codegen_DIRECTIVE_LIST(directive_list->val.pair.directive_list);
			code_info* ti_directive = codegen_DIRECTIVE(directive_list->val.pair.directive);
		}break;
		case DIRECTIVE_LIST::empty:
		{
			return nullptr;
		}break;
	}

	return nullptr;
}

code_info* codegen_DIRECTIVE(DIRECTIVE* directive)
{
	if (FALCON_DEBUG_CODEGEN)
		printf("|||||------> <directive_list> %d -- %d\n", directive->kind, directive->lineno);
	switch (directive->kind)
	{
		case DIRECTIVE::load:
		{}
		break;
	}
	return nullptr;
}




code_info* codegen_STRING(STRING* str)
{
	if (FALCON_DEBUG_CODEGEN)
		printf("|-- <string> %d \"%s\"\n", str->lineno, str->str);

	switch (str->kind)
	{
		case STRING::stringK:
		{
			code_info* cg = (code_info*)Calloc(1, sizeof(code_info));
			register_info* reg = (register_info*)Calloc(1, sizeof(struct register_info));
			cg->register_info = reg;
			cg->node_type = ntSTRING;
			str->cg = cg;



			cg->ptr_to_static_string_label = (char**)Calloc(1, sizeof(char**));
			char* label_str = codegen_create_unique_label();
			string_builder* temp = string_builder_new();
			string_builder_append_chars(temp, label_str);
			string_builder_append_chars(temp, ": .asciz \"");
			string_builder_append_chars(temp, str->str);
			string_builder_append_chars(temp, "\"\n");
			char* str = string_builder_copy_to_char_array(temp);
			string_builder_clear(temp);
			*cg->ptr_to_static_string_label = label_str;

			//add it to the static data instead.
			linked_list_add_last(&READ_ONLY_DATA_SECTION, str);
			//array_push(array_DATA_SECTION, str);

			//string_builder_append_chars(sb_push_label, "pushq \%rbx\n"); //save rbx first?

			string_builder_append_chars(temp, "$");
			string_builder_append_chars(temp, label_str);
			//string_builder_append_chars(temp, "(\%rip)");
			char* ret = string_builder_copy_to_char_array(temp);
			string_builder_destroy(temp);
			cg->returned_string = ret;
			//char* code = string_builder_copy_to_char_array(sb_push_label);
			//string_builder_destroy(sb_push_label);
			//linked_list_add_last(&code_list, code);





		}
		break;
		case STRING::string_concatenationK:
		{
		}
		break;
	}

	return str->cg;
}


code_info* codegen_FUNCTION(FUNCTION* f)
{
	if (FALCON_DEBUG_CODEGEN)
		printf("|||||------> <function> \"%s\"  %d -- %d\n", f->func_head->function_id, 0, f->lineno);


	if (strcmp(f->func_head->function_id, "main") == 0) //this shouldn't happen.
	{
		return nullptr;
	}


	f->STACK_ALLIGNMENT_COUNT = 0;
	f->STACK_ALLIGNMENT_VALUE = 0;

	DECLARED_VARIABLES_IN_CURRENT_SCOPE_OFFSET = 0;
	NUMBER_OF_DECLARED_VARIABLES_IN_CURRENT_SCOPE = 0;
	GLOBAL_CURRENT_FUNCTION = f;
	GLOBAL_CURRENT_FUNCTION_IS_MAIN = false;
	GLOBAL_VARIABLE_DECLARATIONS = 0;
	IN_GLOBAL_SCOPE = false;

	//reset  counter before begining.
	code_info* code_info_func = (code_info*)Calloc(1, sizeof(struct code_info));
	register_info* reg = (register_info*)Calloc(1, sizeof(struct register_info));
	code_info_func->register_info = reg;
	code_info_func->node_type = ntFUNCTION;

	string_builder_append_chars(SB, f->func_head->function_id);
	string_builder_append_chars(SB, ":\n");
	string_builder_append_chars(SB, push_reg(RBP));
	string_builder_append_chars(SB, "movq\t%rsp, %rbp\n");
	codegen_HEAD(f->func_head); //TODO: handle allignment inside head.
	codegen_BODY(f->func_body);
	string_builder_append_chars(SB, "movq\t%rbp, %rsp\n");
	string_builder_append_chars(SB, pop_reg(RBP));
	string_builder_append_chars(SB, "retq\n\n");


	IN_GLOBAL_SCOPE = true;
	return code_info_func;
}

code_info* codegen_HEAD(HEAD* head)
{
	if (FALCON_DEBUG_CODEGEN)
		printf("|||||------> <head> \"%s\" %d -- %d\n", head->function_id, 0, head->lineno);
	code_info* cg = (code_info*)Calloc(1, sizeof(struct code_info));
	register_info* reg = (register_info*)Calloc(1, sizeof(struct register_info));
	cg->register_info = reg;
	cg->node_type = ntHEAD;

	code_info* cg_type = codegen_TYPE(head->return_type);
	log_info("99");
	code_info* cg_par_decl_list = codegen_PAR_DECL_LIST(head->params);
	log_info("100");

	return cg;
}


code_info* codegen_TYPE(TYPE * type)
{
	if (FALCON_DEBUG_CODEGEN)
		printf("|||||------> <type> %d -- %d\n", type->kind, type->lineno);
	code_info* cg = static_cast<code_info*>(Calloc(1, sizeof(struct code_info)));
	register_info* reg = static_cast<register_info*>(Calloc(1, sizeof(struct register_info)));
	cg->register_info = reg;
	cg->node_type = ntTYPE;
	switch (type->kind)
	{
		case TYPE::type_idK:
		{
		}
		break;
		case TYPE::type_intK:
		{
		}
		break;
		case TYPE::type_boolK:
		{
		}
		break;
		case TYPE::type_arrayK:
		{
			code_info* cg_type = codegen_TYPE(type->val.type);
		}
		break;
		case TYPE::type_recordK:
		{
			array* arr = array_new();
			code_info* cg_var_decl_list = codegen_VAR_DECL_LIST(type->val.var_decl_list, arr, 0, false);
		}
		break;
		case TYPE::type_stringK:
		{}
		break;
		default: { log_internal_error(__FILE__, __func__, __LINE__, "DEFAULT CASE."); } break;
	}
	return cg;
}


code_info* codegen_PAR_DECL_LIST(PAR_DECL_LIST * par_decl_list)
{
	if (FALCON_DEBUG_CODEGEN)
		printf("|||||------> <par_decl_list> %d -- %d\n", par_decl_list->kind, par_decl_list->lineno);
	code_info* cg = (code_info*)Calloc(1, sizeof(struct code_info));
	register_info* reg = (register_info*)Calloc(1, sizeof(struct register_info));
	cg->register_info = reg;
	cg->node_type = ntPAR_DECL_LIST;

	log_info("wee");
	switch (par_decl_list->kind)
	{
		case PAR_DECL_LIST::par_decl_list_var_decl_listK:
		{
			log_info("22");
			par_decl_list->list_of_var_types = array_new();
			log_info("44");
			code_info* cg_var_decl_list = codegen_VAR_DECL_LIST(par_decl_list->val.var_decl_list, par_decl_list->list_of_var_types, 0, true);
			log_info("55");
		}
		break;
		case PAR_DECL_LIST::par_decl_list_emptyK:
		{}
		break;
		default:  break;;
	}
	return cg;
}
code_info* codegen_VAR_DECL_LIST(VAR_DECL_LIST * var_decl_list, array* arr, int offset_count, bool is_parameter)
{
	if (FALCON_DEBUG_CODEGEN)
		printf("|||||------> <var_decl_list> %d -- %d\n", var_decl_list->kind, var_decl_list->lineno);
	code_info* cg = (code_info*)Calloc(1, sizeof(struct code_info));
	register_info* reg = (register_info*)Calloc(1, sizeof(struct register_info));
	cg->register_info = reg;
	cg->node_type = ntVAR_DECL_LIST;
	switch (var_decl_list->kind)
	{
		case VAR_DECL_LIST::var_decl_list_pairK:
		{
			offset_count++;
			NUMBER_OF_DECLARED_VARIABLES_IN_CURRENT_SCOPE++;
			code_info* cg_var_type = codegen_VAR_TYPE(var_decl_list->val.pair.head, offset_count, false, is_parameter);
			array_push(arr, cg_var_type);
			code_info* cg_var_decl_list = codegen_VAR_DECL_LIST(var_decl_list->val.pair.tail, arr, offset_count, is_parameter);
		}
		break;
		case VAR_DECL_LIST::var_decl_var_typeK:
		{
			offset_count++;
			NUMBER_OF_DECLARED_VARIABLES_IN_CURRENT_SCOPE++;
			code_info* cg_var_type = codegen_VAR_TYPE(var_decl_list->val.var_type, offset_count, false, is_parameter);
			array_push(arr, cg_var_type);
		}
		break;
		default:  break;
	}
	return cg;
}

code_info* codegen_VAR_TYPE(VAR_TYPE * var_type, int offset_counter, bool is_var_type_declaration, bool is_parameter)
{
	if (FALCON_DEBUG_CODEGEN)
		printf("|||||------> <var_type> %d -- %d\n", 0, var_type->lineno);
	code_info* cg = (code_info*)Calloc(1, sizeof(struct code_info));
	register_info* reg = (register_info*)Calloc(1, sizeof(struct register_info));
	cg->register_info = reg;
	cg->node_type = ntVAR_TYPE;
	GLOBAL_VARIABLE_DECLARATIONS++;
	code_info* cg_type = codegen_TYPE(var_type->type);
	type_info_imp* type = lookup_once(var_type->ti->sym_table, var_type->id, var_type->lineno);
	type->parameter_offset = (offset_counter + 0) * GLOBAL_OFFSET_MULTIPLIER;
	type->variable_declaration_offset = GLOBAL_VARIABLE_DECLARATIONS * GLOBAL_OFFSET_MULTIPLIER;
	type->is_global = IN_GLOBAL_SCOPE;

	if (IN_GLOBAL_SCOPE)
	{

		string_builder* temp = string_builder_new();
		string_builder_append_chars(temp, var_type->id);
		string_builder_append_chars(temp, ":\n");
		switch (type->kind)
		{
			case type_info_imp::t_intK:
			{
				//string_builder_append_chars(temp, "\t.long\t1\n");
				string_builder_append_chars(temp, "\t.quad 0\n");
			}
			default: break;;
		}
		/*
		if (var_type->visibility == VISIBILITY::GLOBAL)
		{
			string_builder_append_chars(temp, "\t.globl ");
			string_builder_append_chars(temp, var_type->id);
			string_builder_append_chars(temp, "\n");
		}
		else if (var_type->visibility == VISIBILITY::STATIC)
		{
			string_builder_append_chars(temp, "\t.local ");
			string_builder_append_chars(temp, var_type->id);
			string_builder_append_chars(temp, "\n");
		}
		*/

		char* str = string_builder_copy_to_char_array(temp);
		array_push(array_DATA_SECTION, str);

		string_builder_clear(temp);
		if (is_var_type_declaration)
		{
			string_builder_append_chars(temp, var_type->id);
			string_builder_append_chars(temp, "(%rip)");
		}
		else
		{
			string_builder_append_chars(temp, "$");
			string_builder_append_chars(temp, var_type->id);
		}
		char* return_str = string_builder_copy_to_char_array(temp);
		cg->returned_string = return_str;
		string_builder_destroy(temp);

		//linked_list_add_last(&GLOBAL_VARIABLE_DECLARATION_LIST, str);
	}
	else
	{
		if (!is_parameter)
		{
			DECLARED_VARIABLES_IN_CURRENT_SCOPE_OFFSET += GLOBAL_OFFSET_MULTIPLIER;
		}


		string_builder* temp = string_builder_new();
		string_builder_append_chars(temp, "-");
		string_builder_append_int(temp, &type->variable_declaration_offset);
		string_builder_append_chars(temp, "(%rbp)");


		char* str = string_builder_copy_to_char_array(temp);
		cg->returned_string = str;
		string_builder_destroy(temp);




	}
	return cg;
}


bool codegen_is_even(int stack_alignment_count)
{
	return stack_alignment_count % 2 == 0;
}

code_info* codegen_BODY(BODY * body)
{
	if (FALCON_DEBUG_CODEGEN)
		printf("|||||------> <body> %d -- %d\n", 0, body->lineno);
	code_info* cg = (code_info*)Calloc(1, sizeof(struct code_info));
	register_info* reg = (register_info*)Calloc(1, sizeof(struct register_info));
	cg->register_info = reg;
	cg->node_type = ntBODY;

	array* array_of_local_declarations = array_new();
	code_info* cg_decl_list = codegen_DECL_LIST(body->decl_list, array_of_local_declarations);

	if (!IN_GLOBAL_SCOPE)
	{
		if (NUMBER_OF_DECLARED_VARIABLES_IN_CURRENT_SCOPE > 0)
		{
			string_builder_append_chars(SB, "subq\t$");
			if (NUMBER_OF_DECLARED_VARIABLES_IN_CURRENT_SCOPE % 2 != 0)
			{
				int offset = DECLARED_VARIABLES_IN_CURRENT_SCOPE_OFFSET += GLOBAL_OFFSET_MULTIPLIER;
				string_builder_append_int(SB, &offset);
			}
			else
				string_builder_append_int(SB, &DECLARED_VARIABLES_IN_CURRENT_SCOPE_OFFSET);
			string_builder_append_chars(SB, ", %rsp\t#make space for variables\n");
		}
		else
		{
			printf("%d \n", GLOBAL_CURRENT_FUNCTION->STACK_ALLIGNMENT_COUNT);
			//check if even
			if (!codegen_is_even(GLOBAL_CURRENT_FUNCTION->STACK_ALLIGNMENT_COUNT))
			{
				//allign the stack.
				GLOBAL_OFFSET_MULTIPLIER += 8;
				string_builder_append_chars(SB, "subq\t$8, %rsp\t\n");
			}
		}

	}




	code_info* cg_stm_list = codegen_STM_LIST(body->stm_list);

	return cg;
}


code_info* codegen_DECL_LIST(DECL_LIST * decl_list, array* array_of_declarations)
{
	if (FALCON_DEBUG_CODEGEN)
		printf("|||||------> <decl_list> %d -- %d\n", decl_list->kind, decl_list->lineno);
	code_info* cg = (code_info*)Calloc(1, sizeof(struct code_info));
	register_info* reg = (register_info*)Calloc(1, sizeof(struct register_info));
	cg->register_info = reg;
	cg->node_type = ntDECL_LIST;
	switch (decl_list->kind)
	{
		case DECL_LIST::decl_list_pair:
		{
			code_info* cg_decl_list = codegen_DECL_LIST(decl_list->val.pair.next, array_of_declarations);
			code_info* cg_decl = codegen_DECL(decl_list->val.pair.head);
			array_push(array_of_declarations, cg_decl);
		}
		break;
		case DECL_LIST::decl_list_empty:
		{
		}
		break;
		default:  break;
	}
	return cg;
}


code_info* codegen_DECL(DECL * decl)
{
	if (FALCON_DEBUG_CODEGEN)
		printf("|||||------> <decl> %d -- %d\n", decl->kind, decl->lineno);
	code_info* cg = (code_info*)Calloc(1, sizeof(struct code_info));
	register_info* reg = (register_info*)Calloc(1, sizeof(struct register_info));
	cg->register_info = reg;
	cg->node_type = ntDECL;
	switch (decl->kind)
	{
		case DECL::decl_assignK:
		{
			code_info* cg_type = codegen_TYPE(decl->val.type);
		}
		break;
		case DECL::decl_functionK:
		{
			code_info* cg_function = codegen_FUNCTION(decl->val.function);
		}
		break;
		case DECL::decl_varsK:
		{
			array* arr = array_new();
			code_info* cg_var_decl_list = codegen_VAR_DECL_LIST(decl->val.var_decl_list, arr, 0, false);

		}
		break;
		case DECL::decl_var_typ_exp:
		{
			code_info* cg_var_type = codegen_VAR_TYPE(decl->var_type, 0, true, false);
			code_info* cg_exp = codegen_EXP(decl->exp, 1);

			if (IN_GLOBAL_SCOPE)
			{
				string_builder* temp = string_builder_new();
				string_builder_append_chars(temp, "movq\t");
				string_builder_append_chars(temp, cg_exp->returned_string);
				string_builder_append_chars(temp, ", ");
				string_builder_append_chars(temp, cg_var_type->returned_string);
				string_builder_append_chars(temp, "\t#global declaration var_type\n");
				char* str = string_builder_copy_to_char_array(temp);
				linked_list_add_last(&GLOBAL_LATE_DECLARATION_LIST, str);
			}
			else
			{
				string_builder_append_chars(SB, "movq\t");
				string_builder_append_chars(SB, cg_exp->returned_string);
				string_builder_append_chars(SB, ", ");
				string_builder_append_chars(SB, cg_var_type->returned_string);
				string_builder_append_chars(SB, "\t#declaration var_type\n");
			}
		}break;
		case DECL::decl_struct:
		{
			code_info* cg_body = codegen_BODY(decl->body);
		}break;
		default:  break;
	}
	return cg;
}


code_info* codegen_STM_LIST(STM_LIST * stm_list)
{
	if (FALCON_DEBUG_CODEGEN)
		printf("|||||------> <stm_list> %d -- %d\n", stm_list->kind, stm_list->lineno);
	code_info* cg = (code_info*)Calloc(1, sizeof(struct code_info));
	register_info* reg = (register_info*)Calloc(1, sizeof(struct register_info));
	cg->register_info = reg;
	cg->node_type = ntSTM_LIST;
	switch (stm_list->kind)
	{
		case STM_LIST::stm_list_pairK:
		{
			code_info* cg_stm_list = codegen_STM_LIST(stm_list->val.pair.tail);
			code_info* cg_stm = codegen_STM(stm_list->val.pair.head);
		}
		break;
		case STM_LIST::stm_list_empty:
		{
		}break;
		default:  break;
	}
	return cg;
}





code_info* codegen_STM(STM * stm)
{
	if (FALCON_DEBUG_CODEGEN)
		printf("|||||------> <stm> %d -- %d\n", stm->kind, stm->lineno);
	code_info* cg = (code_info*)Calloc(1, sizeof(struct code_info));
	register_info* reg = (register_info*)Calloc(1, sizeof(struct register_info));
	cg->register_info = reg;
	cg->node_type = ntSTM;
	switch (stm->kind)
	{
		case STM::stm_returnK:
		{
			code_info* cg_exp = codegen_EXP(stm->val.exp, 1);

			if (GLOBAL_CURRENT_FUNCTION_IS_MAIN)
			{
				if (cg_exp->register_info->reg != RAX)
				{
					string_builder_append_chars(SB, "movq\t");
					string_builder_append_chars(SB, cg_exp->returned_string);
					string_builder_append_chars(SB, ", %rax\t#return statement.\n");
				}
				string_builder_append_chars(SB, "movl\t%eax, %ecx\t#main return statement.\n");
				string_builder_append_chars(SB, "movl\t%ecx, %eax\t#main return statement.\n");
			}
			else
			{
				if (cg_exp->returned_string)
				{
					printf("\nkind: %d\n", stm->val.exp->kind);
					string_builder_append_chars(SB, "movq\t");
					string_builder_append_chars(SB, cg_exp->returned_string);
					string_builder_append_chars(SB, ", %rax\t#return statement.\n");
				}
			}

		}	break;
		case STM::stm_printf:
		{
			if (!LOADED_FUNCTIONS["printf"])
			{
				LOADED_FUNCTIONS["printf"] = true;

				//array_push(array_DATA_SECTION, );
			}


			/*
			array* used_registers = GLOBAL_USED_REGISTER_MAP.at(stm);
			if (used_registers)
			{
				string_builder_append_chars(SB, "\n");
				string_builder_append_chars(SB, function_prologue("printf", used_registers));
			}
			else
			{
			}
			*/


			code_info* cg_exp = codegen_EXP(stm->val.exp, 1);
			type_info_imp* ti_exp = stm->val.exp->type;

//			string_builder_append_chars(SB, "push\t%rbx\n");
//			string_builder_append_chars(SB, "push\t%rbx\n");
//			string_builder_append_chars(SB, "push\t%rbx\n");
			if (ti_exp->kind == type_info_imp::t_intK)
			{
					//string_builder_append_chars(SB, "leaq\tprintf_format_int(\%rip), \%rdi\n"); //# passing 1. argument in %rdi
				string_builder_append_chars(SB, "movq\t$printf_format_int, %rdi\n"); //# passing 1. argument in %rdi
				//string_builder_append_chars(SB, "movabsq\t$printf_format_int, \%rdi\n"); //# passing 1. argument in %rdi
			}
			else if (ti_exp->kind == type_info_imp::t_stringK)
			{
				//string_builder_append_chars(SB, "leaq\tprintf_format_string(\%rip), \%rdi\n"); //# passing 1. argument in %rdi
				string_builder_append_chars(SB, "movq\t$printf_format_string, %rdi\n"); //# passing 1. argument in %rdi
				//string_builder_append_chars(SB, "movabsq\t$printf_format_string, \%rdi\n"); //# passing 1. argument in %rdi
			}

			string_builder_append_chars(SB, "movq\t");
			string_builder_append_chars(SB, cg_exp->returned_string);
			string_builder_append_chars(SB, ", %rsi\n"); //# passing 2. argument in %rsi
			string_builder_append_chars(SB, "movq\t$0, %rax\n"); //# no floating point registers used
			string_builder_append_chars(SB, "xor\t%eax, %eax\t#Zeroing EAX is efficient way to clear AL and set it to 0.\n");
			string_builder_append_chars(SB, "call\tprintf\n");

			//string_builder_append_chars(SB, "pop\t%rbx\n");
			//string_builder_append_chars(SB, "pop\t%rbx\n");
			//string_builder_append_chars(SB, "pop\t%rbx\n");

			/*
			if (used_registers)
			{
				string_builder_append_chars(SB, function_epilogue("printf", used_registers));
				string_builder_append_chars(SB, "\n");
			}
			*/

			//string_builder_append_chars(SB, "popq\t\%rax\n"); //# passing 1. argument in %rdi
			//string_builder_append_chars(SB, "popq\t\%rsi\n"); //# passing 1. argument in %rdi
			//string_builder_append_chars(SB, "popq\t\%rdi\n"); //# passing 1. argument in %rdi
				//char* string = string_builder_copy_to_char_array(SB);
			//string_builder_destroy(SB);
			//linked_list_add_last(&code_list, string);



		}break;
		case STM::stm_allocateK:
		{
			code_info* cg_variable = codegen_VARIABLE(stm->val.var, 1);
		}break;
		case STM::stm_allocate_lengthK:
		{
			code_info* cg_variable = codegen_VARIABLE(stm->val.allocate_length.var, 1);
			code_info* cg_exp = codegen_EXP(stm->val.allocate_length.exp, 1);
		}
		break;
		case STM::stm_assignmentK:
		{
			code_info* cg_variable = codegen_VARIABLE(stm->val.assignment.var, 1);
			code_info* cg_exp = codegen_EXP(stm->val.assignment.exp, 1);

			string_builder_append_chars(SB, "movq\t");
			string_builder_append_chars(SB, cg_exp->returned_string);
			string_builder_append_chars(SB, ", ");
			string_builder_append_chars(SB, cg_variable->returned_string);
			string_builder_append_chars(SB, "\t#Assignment statement.\n");
		}
		break;
		case STM::stm_ifK:
		{
			/*
			* code for <expression>
			* cmp "<expression>-result", "true"
			* jne end_if #assuming equality
			* code for <stm1>
			* jmp end_if
			* end_if:
			*/
			char* label_end_if = codegen_create_unique_label_with_seed("end_if_");


			code_info* cg_exp = codegen_EXP(stm->val.if_stm.exp, 0);

			string_builder_append_chars(SB, "movq\t$1, %rdx\t#if-statement\n");
			string_builder_append_chars(SB, "cmpq\t%rax, %rdx\t#check exp for if-statement\n");
			string_builder_append_chars(SB, "jne\t");
			string_builder_append_chars(SB, label_end_if);
			string_builder_append_chars(SB, "\n");

			code_info* cg_stm = codegen_STM(stm->val.if_stm.stm);

			string_builder_append_chars(SB, label_end_if);
			string_builder_append_chars(SB, ":\n");
		}
		break;
		case STM::stm_if_elseK:
		{
			/*
			 * code for <expression>
			 * cmp "<expression>-result", "true"
			 * jne else_part
			 * code for <stm1>
			 * jmp end_if
			 * else_part:
			 * code for <stm_2>
			 * end_if:
			 */
			char* label_else_if = codegen_create_unique_label_with_seed("else_if_");
			char* label_end_if = codegen_create_unique_label_with_seed("end_if_else_");


			code_info* cg_exp = codegen_EXP(stm->val.if_else_stm.exp, 1);

			string_builder_append_chars(SB, "movq\t$1, %rdx\t#if-statement\n");
			string_builder_append_chars(SB, "cmpq\t%rax, %rdx\t#check exp for if-statement\n");
			string_builder_append_chars(SB, "jne\t");
			string_builder_append_chars(SB, label_else_if);
			string_builder_append_chars(SB, "\n");

			code_info* cg_stm_then = codegen_STM(stm->val.if_else_stm.stm_then);

			string_builder_append_chars(SB, "jmp\t");
			string_builder_append_chars(SB, label_end_if);
			string_builder_append_chars(SB, "\n");

			string_builder_append_chars(SB, label_else_if);
			string_builder_append_chars(SB, ":\n");

			code_info* cg_stm_else = codegen_STM(stm->val.if_else_stm.stm_else);

			string_builder_append_chars(SB, label_end_if);
			string_builder_append_chars(SB, ":\n");
		}
		break;

		case STM::stm_whileK:
		{
			/* while_start:
			 * code for <expression>
			 * cmp "<expression>-result", "true"
			 * jne while_end
			 * code for <stm>
			 * while_end:
			 */
			char* while_start = codegen_create_unique_label_with_seed("while_start_");
			char* while_end = codegen_create_unique_label_with_seed("while_end_");

			string_builder_append_chars(SB, while_start);
			string_builder_append_chars(SB, ":\n");


			code_info* cg_exp = codegen_EXP(stm->val.while_stm.exp, 1);

			string_builder_append_chars(SB, "movq\t$1, %rdx\t#if-statement\n");
			string_builder_append_chars(SB, "cmpq\t%rax, %rdx\t#check exp for if-statement\n");
			string_builder_append_chars(SB, "jne\t");
			string_builder_append_chars(SB, while_end);
			string_builder_append_chars(SB, "\n");

			code_info* cg_stm = codegen_STM(stm->val.while_stm.stm);

			string_builder_append_chars(SB, while_end);
			string_builder_append_chars(SB, ":\n");

		}break;

		case STM::stm_for:
		{
			codegen_VAR_TYPE(stm->val.for_stm.var_type, 1, false, false);
			codegen_STM(stm->val.for_stm.stm);
			codegen_EXP(stm->val.for_stm.exp, 1);
			codegen_BODY(stm->val.for_stm.body);
		}
		break;

		case STM::stm_listK:
		{
			code_info* cg_stm_list = codegen_STM_LIST(stm->val.list);
		}
		break;
		default:  break;
	}
	return cg;
}


code_info* codegen_VARIABLE(VARIABLE * var, int offset_index)
{
	if (FALCON_DEBUG_CODEGEN)
		printf("|||||------> <var> %d -- %d\n", var->kind, var->lineno);

	code_info* cg = (code_info*)Calloc(1, sizeof(struct code_info));
	register_info* reg = (register_info*)Calloc(1, sizeof(struct register_info));
	cg->register_info = reg;
	cg->node_type = ntVARIABLE;
	switch (var->kind)
	{
		case VARIABLE::variable_idK:
		{

			type_info_imp* type = lookup_once(var->type->sym_table, var->val.id, var->lineno);
			string_builder* temp = string_builder_new();

			if (type->is_global)
			{
				string_builder_append_chars(temp, var->val.id);
				string_builder_append_chars(temp, "(%rip)");
			}
			else
			{

				if (type->parameterized)
				{
					//string_builder_append_chars(temp, "-");
					int offset = (offset_index + 1) * GLOBAL_OFFSET_MULTIPLIER;
					type->variable_parameter_offset = offset;
					string_builder_append_int(temp, &offset);
				}
				else
				{
					GLOBAL_CURRENT_FUNCTION->STACK_ALLIGNMENT_COUNT++;
					//if (type->kind == type_info_imp::t_intK)
					GLOBAL_CURRENT_FUNCTION->STACK_ALLIGNMENT_VALUE += 8;

					string_builder_append_chars(temp, "-");
					string_builder_append_int(temp, &type->variable_declaration_offset);
				}

				string_builder_append_chars(temp, "(%rbp)");
			}
			cg->returned_string = string_builder_copy_to_char_array(temp);
			string_builder_destroy(temp);
		}
		break;
		case VARIABLE::variable_arrayAccessK:
		{
			code_info* cg_variable = codegen_VARIABLE(var->val.array_access.var, offset_index);
			code_info* cg_exp = codegen_EXP(var->val.array_access.exp, offset_index);
		}
		break;
		case VARIABLE::variable_access_idK:
		{
			code_info* cg_variable = codegen_VARIABLE(var->val.access.var, offset_index);

		}
		break;
		default: break;
	}
	return cg;
}

code_info* codegen_EXP(EXP * exp, int offset_index)
{
	if (FALCON_DEBUG_CODEGEN)
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


	code_info* cg = (code_info*)Calloc(1, sizeof(struct code_info));
	register_info* reg = (register_info*)Calloc(1, sizeof(struct register_info));
	cg->register_info = reg;
	cg->node_type = ntEXP;

	switch (exp->kind)
	{
		case EXP::exp_timesK:
		{
			code_info* cg_exp_left = codegen_EXP(exp->val.op.left, offset_index);
			code_info* cg_exp_right = codegen_EXP(exp->val.op.right, offset_index);

			char* result = codegen_gen_temp_register();

			if (cg_exp_left->node_type == ntVARIABLE || cg_exp_left->node_type == ntTERM)
			{
				string_builder_append_chars(SB, "movq\t");
				string_builder_append_chars(SB, cg_exp_left->returned_string);
				string_builder_append_chars(SB, ", %rdx\t#expression left\n");

				if (cg_exp_right->node_type == ntVARIABLE || cg_exp_right->node_type == ntTERM)
				{
					string_builder_append_chars(SB, "movq\t");
					string_builder_append_chars(SB, cg_exp_right->returned_string);
					string_builder_append_chars(SB, ", %rax\t#expression right\n");
				}
				else
				{
				}
				string_builder_append_chars(SB, "imulq\t%rdx, %rax\t#expression calculation\n");
			}
			else
			{
				if (cg_exp_right->node_type == ntVARIABLE || cg_exp_right->node_type == ntTERM)
				{
					string_builder_append_chars(SB, "movq\t");
					string_builder_append_chars(SB, cg_exp_right->returned_string);
					string_builder_append_chars(SB, ", %rdx\t#expression right\n");
				}
				else
				{
				}
				string_builder_append_chars(SB, "imulq\t%rax, %rdx\t#expression calculation\n");
				string_builder_append_chars(SB, "movq\t%rdx, %rax\t#expression calculation\n");
			}

			char* line = string_builder_copy_to_char_array(SB);
			string_builder_clear(SB);
			linked_list_add_last(&GLOBAL_CODE_LIST, line);

			cg->register_info->reg = RAX;

			//return the result back up.
			cg->returned_string = result;

		}
		break;
		case EXP::exp_divK:
		{
			code_info* cg_exp_left = codegen_EXP(exp->val.op.left, offset_index);
			code_info* cg_exp_right = codegen_EXP(exp->val.op.right, offset_index);

			char* result = codegen_gen_temp_register();

			if (cg_exp_left->node_type == ntVARIABLE || cg_exp_left->node_type == ntTERM)
			{
				string_builder_append_chars(SB, "movq\t");
				string_builder_append_chars(SB, cg_exp_left->returned_string);
				string_builder_append_chars(SB, ", %rdx\t#expression left\n");


				if (cg_exp_right->node_type == ntVARIABLE || cg_exp_right->node_type == ntTERM)
				{
					string_builder_append_chars(SB, "movq\t");
					string_builder_append_chars(SB, cg_exp_right->returned_string);
					string_builder_append_chars(SB, ", %rax\t#expression right\n");
				}
				else
				{

				}


				string_builder_append_chars(SB, "divq\t%rdx, %rax\t#expression calculation\n");
			}
			else
			{

				if (cg_exp_right->node_type == ntVARIABLE || cg_exp_right->node_type == ntTERM)
				{
					string_builder_append_chars(SB, "movq\t");
					string_builder_append_chars(SB, cg_exp_right->returned_string);
					string_builder_append_chars(SB, ", %rdx\t#expression right\n");
				}
				else
				{

				}

				string_builder_append_chars(SB, "divq\t%rdx, %rax\t#expression calculation\n");
			}

			char* line = string_builder_copy_to_char_array(SB);
			string_builder_clear(SB);
			linked_list_add_last(&GLOBAL_CODE_LIST, line);

			cg->register_info->reg = RAX;

			//return the result back up.
			cg->returned_string = result;
		}
		break;
		case EXP::exp_plusK:
		{
			code_info* cg_exp_left = codegen_EXP(exp->val.op.left, offset_index);
			code_info* cg_exp_right = codegen_EXP(exp->val.op.right, offset_index);

			char* result = codegen_gen_temp_register();

			if (cg_exp_left->node_type == ntVARIABLE || cg_exp_left->node_type == ntTERM)
			{
				string_builder_append_chars(SB, "movq\t");
				string_builder_append_chars(SB, cg_exp_left->returned_string);
				string_builder_append_chars(SB, ", %rdx\t#expression left\n");


				if (cg_exp_right->node_type == ntVARIABLE || cg_exp_right->node_type == ntTERM)
				{
					string_builder_append_chars(SB, "movq\t");
					string_builder_append_chars(SB, cg_exp_right->returned_string);
					string_builder_append_chars(SB, ", %rax\t#expression right\n");
				}
				else
				{

				}


				string_builder_append_chars(SB, "addq\t%rdx, %rax\t#expression calculation\n");
			}
			else
			{

				if (cg_exp_right->node_type == ntVARIABLE || cg_exp_right->node_type == ntTERM)
				{
					string_builder_append_chars(SB, "movq\t");
					string_builder_append_chars(SB, cg_exp_right->returned_string);
					string_builder_append_chars(SB, ", %rdx\t#expression right\n");
				}
				else
				{

				}

				string_builder_append_chars(SB, "addq\t%rdx, %rax\t#expression calculation\n");
			}

			char* line = string_builder_copy_to_char_array(SB);
			string_builder_clear(SB);
			linked_list_add_last(&GLOBAL_CODE_LIST, line);

			cg->register_info->reg = RAX;

			//return the result back up.
			cg->returned_string = result;
		}
		break;
		case EXP::exp_minusK:
		{
			code_info* cg_exp_left = codegen_EXP(exp->val.op.left, offset_index);
			code_info* cg_exp_right = codegen_EXP(exp->val.op.right, offset_index);

			char* result = codegen_gen_temp_register();

			if (cg_exp_left->node_type == ntVARIABLE || cg_exp_left->node_type == ntTERM)
			{
				string_builder_append_chars(SB, "movq\t");
				string_builder_append_chars(SB, cg_exp_left->returned_string);
				string_builder_append_chars(SB, ", %rdx\t#expression left\n");


				if (cg_exp_right->node_type == ntVARIABLE || cg_exp_right->node_type == ntTERM)
				{
					string_builder_append_chars(SB, "movq\t");
					string_builder_append_chars(SB, cg_exp_right->returned_string);
					string_builder_append_chars(SB, ", %rax\t#expression right\n");
				}
				else
				{

				}


				string_builder_append_chars(SB, "subq\t%rdx, %rax\t#expression calculation\n");
			}
			else
			{

				if (cg_exp_right->node_type == ntVARIABLE || cg_exp_right->node_type == ntTERM)
				{
					string_builder_append_chars(SB, "movq\t");
					string_builder_append_chars(SB, cg_exp_right->returned_string);
					string_builder_append_chars(SB, ", %rdx\t#expression right\n");
				}
				else
				{

				}

				string_builder_append_chars(SB, "subq\t%rdx, %rax\t#expression calculation\n");
			}

			char* line = string_builder_copy_to_char_array(SB);
			string_builder_clear(SB);
			linked_list_add_last(&GLOBAL_CODE_LIST, line);

			cg->register_info->reg = RAX;

			//return the result back up.
			cg->returned_string = result;
		}
		break;
		case EXP::exp_equalityK:
		{

			/* @ = unique incremented integer.
			 *
			 * code for <exp-left>
			 * code for <exp-right>
			 * cmpq %rdx, %rax
			 * je exp_true_@
			 * xor %eax, %eax # set rax to 0.
			 * jmp exp_end_@
			 * exp_true_@:
			 * mov $1, %al # set rax to 1.
			 * exp_end_@:
			 *
			 */



			code_info* cg_exp_left = codegen_EXP(exp->val.op.left, offset_index);
			code_info* cg_exp_right = codegen_EXP(exp->val.op.right, offset_index);

			char* label_true = codegen_create_unique_label_with_seed("exp_true_");
			char* label_end = codegen_create_unique_label_with_seed("exp_end_");

			char* result = codegen_gen_temp_register();

			if (cg_exp_left->node_type == ntVARIABLE || cg_exp_left->node_type == ntTERM)
			{
				string_builder_append_chars(SB, "movq\t");
				string_builder_append_chars(SB, cg_exp_left->returned_string);
				string_builder_append_chars(SB, ", %rdx\t#expression left\n");


				if (cg_exp_right->node_type == ntVARIABLE || cg_exp_right->node_type == ntTERM)
				{
					string_builder_append_chars(SB, "movq\t");
					string_builder_append_chars(SB, cg_exp_right->returned_string);
					string_builder_append_chars(SB, ", %rax\t#expression right\n");
				}
				else
				{

				}


			}
			else
			{

				if (cg_exp_right->node_type == ntVARIABLE || cg_exp_right->node_type == ntTERM)
				{
					string_builder_append_chars(SB, "movq\t");
					string_builder_append_chars(SB, cg_exp_right->returned_string);
					string_builder_append_chars(SB, ", %rdx\t#expression right\n");
				}
				else
				{

				}

			}

			string_builder_append_chars(SB, "cmpq\t%rdx, %rax\t#expression calculation\n");

			string_builder_append_chars(SB, "je\t");
			string_builder_append_chars(SB, label_true);
			string_builder_append_chars(SB, "\t#jump on equality\n");
			string_builder_append_chars(SB, "movq\t$0, %rax\t#Zero RAX\n");

			string_builder_append_chars(SB, "jmp\t");
			string_builder_append_chars(SB, label_end);
			string_builder_append_chars(SB, "\n");

			string_builder_append_chars(SB, label_true);
			string_builder_append_chars(SB, ":\n");
			string_builder_append_chars(SB, "movq\t$1, %rax\t#set rax to 1\n");
			string_builder_append_chars(SB, label_end);
			string_builder_append_chars(SB, ":\n");

			//char* line = string_builder_copy_to_char_array(SB);
			//string_builder_clear(SB);
			//linked_list_add_last(&GLOBAL_CODE_LIST, line);

			cg->register_info->reg = RAX;

			//return the result back up.
			cg->returned_string = result;
		}
		break;
		case EXP::exp_non_equalityK:
		{

			/* @ = unique incremented integer.
			 *
			 * code for <exp-left>
			 * code for <exp-right>
			 * cmpq %rdx, %rax
			 * je exp_true_@
			 * xor %eax, %eax # set rax to 0.
			 * jmp exp_end_@
			 * exp_true_@:
			 * mov $1, %al # set rax to 1.
			 * exp_end_@:
			 *
			 */



			code_info* cg_exp_left = codegen_EXP(exp->val.op.left, offset_index);
			code_info* cg_exp_right = codegen_EXP(exp->val.op.right, offset_index);

			char* label_true = codegen_create_unique_label_with_seed("exp_true_");
			char* label_end = codegen_create_unique_label_with_seed("exp_end_");

			char* result = codegen_gen_temp_register();

			if (cg_exp_left->node_type == ntVARIABLE || cg_exp_left->node_type == ntTERM)
			{
				string_builder_append_chars(SB, "movq\t");
				string_builder_append_chars(SB, cg_exp_left->returned_string);
				string_builder_append_chars(SB, ", %rdx\t#expression left\n");


				if (cg_exp_right->node_type == ntVARIABLE || cg_exp_right->node_type == ntTERM)
				{
					string_builder_append_chars(SB, "movq\t");
					string_builder_append_chars(SB, cg_exp_right->returned_string);
					string_builder_append_chars(SB, ", %rax\t#expression right\n");
				}
				else
				{

				}


			}
			else
			{

				if (cg_exp_right->node_type == ntVARIABLE || cg_exp_right->node_type == ntTERM)
				{
					string_builder_append_chars(SB, "movq\t");
					string_builder_append_chars(SB, cg_exp_right->returned_string);
					string_builder_append_chars(SB, ", %rdx\t#expression right\n");
				}
				else
				{

				}

			}

			string_builder_append_chars(SB, "cmpq\t%rdx, %rax\t#expression calculation\n");

			string_builder_append_chars(SB, "jne\t");
			string_builder_append_chars(SB, label_true);
			string_builder_append_chars(SB, "\t#jump on non-equality\n");
			string_builder_append_chars(SB, "movq\t$0, %rax\t#Zero RAX\n");

			string_builder_append_chars(SB, "jmp\t");
			string_builder_append_chars(SB, label_end);
			string_builder_append_chars(SB, "\n");

			string_builder_append_chars(SB, label_true);
			string_builder_append_chars(SB, ":\n");
			string_builder_append_chars(SB, "movq\t$1, %rax\t#set rax to 1\n");
			string_builder_append_chars(SB, label_end);
			string_builder_append_chars(SB, ":\n");

			//char* line = string_builder_copy_to_char_array(SB);
			//string_builder_clear(SB);
			//linked_list_add_last(&GLOBAL_CODE_LIST, line);

			cg->register_info->reg = RAX;

			//return the result back up.
			cg->returned_string = result;
		}
		break;
		case EXP::exp_greaterK:
		{

			/* @ = unique incremented integer.
			 *
			 * code for <exp-left>
			 * code for <exp-right>
			 * cmpq %rdx, %rax
			 * jg exp_false_@
			 * xor %eax, %eax # set rax to 0.
			 * jmp exp_end_@
			 * exp_false_@:
			 * mov $1, %al # set rax to 1.
			 * exp_end_@:
			 *
			 */

			code_info* cg_exp_left = codegen_EXP(exp->val.op.left, offset_index);
			code_info* cg_exp_right = codegen_EXP(exp->val.op.right, offset_index);

			char* label_true = codegen_create_unique_label_with_seed("exp_true_");
			char* label_end = codegen_create_unique_label_with_seed("exp_end_");

			char* result = codegen_gen_temp_register();

			if (cg_exp_left->node_type == ntVARIABLE || cg_exp_left->node_type == ntTERM)
			{
				string_builder_append_chars(SB, "movq\t");
				string_builder_append_chars(SB, cg_exp_left->returned_string);
				string_builder_append_chars(SB, ", %rdx\t#expression left\n");

				if (cg_exp_right->node_type == ntVARIABLE || cg_exp_right->node_type == ntTERM)
				{
					string_builder_append_chars(SB, "movq\t");
					string_builder_append_chars(SB, cg_exp_right->returned_string);
					string_builder_append_chars(SB, ", %rax\t#expression right\n");
				}
				else
				{
				}
				string_builder_append_chars(SB, "cmpq\t%rax, %rdx\t#expression calculation\n");
			}
			else
			{
				if (cg_exp_right->node_type == ntVARIABLE || cg_exp_right->node_type == ntTERM)
				{
					string_builder_append_chars(SB, "movq\t");
					string_builder_append_chars(SB, cg_exp_right->returned_string);
					string_builder_append_chars(SB, ", %rdx\t#expression right\n");
				}
				else
				{
				}
				string_builder_append_chars(SB, "cmpq\t%rdx, %rax\t#expression calculation\n");
			}


			string_builder_append_chars(SB, "jle\t");
			string_builder_append_chars(SB, label_end);
			string_builder_append_chars(SB, "\t#jump to end if less or equal.\n");

			string_builder_append_chars(SB, "movq\t$1, %rax\t#set rax to 1\n");

			string_builder_append_chars(SB, "jmp\t");
			string_builder_append_chars(SB, label_end);
			string_builder_append_chars(SB, "\n");

			string_builder_append_chars(SB, label_true);
			string_builder_append_chars(SB, ":\n");
			string_builder_append_chars(SB, "movq\t$0, %rax\t#Zero RAX\n");

			string_builder_append_chars(SB, label_end);
			string_builder_append_chars(SB, ":\n");


			cg->register_info->reg = RAX;

			//return the result back up.
			cg->returned_string = result;
		}
		break;
		case EXP::exp_lesserK:
		{

			/* @ = unique incremented integer.
			 *
			 * code for <exp-left>
			 * code for <exp-right>
			 * cmpq %rdx, %rax
			 * je exp_true_@
			 * xor %eax, %eax # set rax to 0.
			 * jmp exp_end_@
			 * exp_true_@:
			 * mov $1, %al # set rax to 1.
			 * exp_end_@:
			 *
			 */



			code_info* cg_exp_left = codegen_EXP(exp->val.op.left, offset_index);
			code_info* cg_exp_right = codegen_EXP(exp->val.op.right, offset_index);

			char* label_true = codegen_create_unique_label_with_seed("exp_true_");
			char* label_end = codegen_create_unique_label_with_seed("exp_end_");

			char* result = codegen_gen_temp_register();

			if (cg_exp_left->node_type == ntVARIABLE || cg_exp_left->node_type == ntTERM)
			{
				string_builder_append_chars(SB, "movq\t");
				string_builder_append_chars(SB, cg_exp_left->returned_string);
				string_builder_append_chars(SB, ", %rdx\t#expression left\n");


				if (cg_exp_right->node_type == ntVARIABLE || cg_exp_right->node_type == ntTERM)
				{
					string_builder_append_chars(SB, "movq\t");
					string_builder_append_chars(SB, cg_exp_right->returned_string);
					string_builder_append_chars(SB, ", %rax\t#expression right\n");
				}
				else
				{

				}

				string_builder_append_chars(SB, "cmpq\t%rax, %rdx\t#expression calculation\n");
			}
			else
			{

				if (cg_exp_right->node_type == ntVARIABLE || cg_exp_right->node_type == ntTERM)
				{
					string_builder_append_chars(SB, "movq\t");
					string_builder_append_chars(SB, cg_exp_right->returned_string);
					string_builder_append_chars(SB, ", %rdx\t#expression right\n");
				}
				else
				{

				}
				string_builder_append_chars(SB, "cmpq\t%rdx, %rax\t#expression calculation\n");
			}


			string_builder_append_chars(SB, "jge\t");
			string_builder_append_chars(SB, label_true);
			string_builder_append_chars(SB, "\t#jump on jump if lesser\n");
			string_builder_append_chars(SB, "movq\t$1, %rax\t#Zero RAX\n");

			string_builder_append_chars(SB, "jmp\t");
			string_builder_append_chars(SB, label_end);
			string_builder_append_chars(SB, "\n");

			string_builder_append_chars(SB, label_true);
			string_builder_append_chars(SB, ":\n");
			string_builder_append_chars(SB, "movq\t$0, %rax\t#set rax to 0\n");
			string_builder_append_chars(SB, label_end);
			string_builder_append_chars(SB, ":\n");

			char* line = string_builder_copy_to_char_array(SB);
			string_builder_clear(SB);
			linked_list_add_last(&GLOBAL_CODE_LIST, line);

			cg->register_info->reg = RAX;

			//return the result back up.
			cg->returned_string = result;
		}
		break;
		case EXP::exp_greater_equalK:
		{
			/* @ = unique incremented integer.
			 *
			 * code for <exp-left>
			 * code for <exp-right>
			 * cmpq %rdx, %rax
			 * je exp_true_@
			 * xor %eax, %eax # set rax to 0.
			 * jmp exp_end_@
			 * exp_true_@:
			 * mov $1, %al # set rax to 1.
			 * exp_end_@:
			 *
			 */

			code_info* cg_exp_left = codegen_EXP(exp->val.op.left, offset_index);
			code_info* cg_exp_right = codegen_EXP(exp->val.op.right, offset_index);

			char* label_false = codegen_create_unique_label_with_seed("exp_false_");
			char* label_end = codegen_create_unique_label_with_seed("exp_end_");

			char* result = codegen_gen_temp_register();

			if (cg_exp_left->node_type == ntVARIABLE || cg_exp_left->node_type == ntTERM)
			{
				string_builder_append_chars(SB, "movq\t");
				string_builder_append_chars(SB, cg_exp_left->returned_string);
				string_builder_append_chars(SB, ", %rdx\t#expression left\n");

				if (cg_exp_right->node_type == ntVARIABLE || cg_exp_right->node_type == ntTERM)
				{
					string_builder_append_chars(SB, "movq\t");
					string_builder_append_chars(SB, cg_exp_right->returned_string);
					string_builder_append_chars(SB, ", %rax\t#expression right\n");
				}
				else
				{
				}
				string_builder_append_chars(SB, "cmpq\t%rax, %rdx\t#expression calculation\n");
			}
			else
			{
				if (cg_exp_right->node_type == ntVARIABLE || cg_exp_right->node_type == ntTERM)
				{
					string_builder_append_chars(SB, "movq\t");
					string_builder_append_chars(SB, cg_exp_right->returned_string);
					string_builder_append_chars(SB, ", %rdx\t#expression right\n");
				}
				else
				{
				}
				string_builder_append_chars(SB, "cmpq\t%rdx, %rax\t#expression calculation\n");
			}

			string_builder_append_chars(SB, "jl\t");
			string_builder_append_chars(SB, label_false);
			string_builder_append_chars(SB, "\t#jump on non-equality\n");
			string_builder_append_chars(SB, "movq\t$1, %rax\t#set rax to 1\n");

			string_builder_append_chars(SB, "jmp\t");
			string_builder_append_chars(SB, label_end);
			string_builder_append_chars(SB, "\n");

			string_builder_append_chars(SB, label_false);
			string_builder_append_chars(SB, ":\n");
			string_builder_append_chars(SB, "movq\t$0, %rax\t#Zero RAX\n");
			string_builder_append_chars(SB, label_end);
			string_builder_append_chars(SB, ":\n");

			char* line = string_builder_copy_to_char_array(SB);
			string_builder_clear(SB);
			linked_list_add_last(&GLOBAL_CODE_LIST, line);

			cg->register_info->reg = RAX;

			//return the result back up.
			cg->returned_string = result;
		}
		break;
		case EXP::exp_lesser_equalK:
		{

			code_info* cg_exp_left = codegen_EXP(exp->val.op.left, offset_index);
			code_info* cg_exp_right = codegen_EXP(exp->val.op.right, offset_index);

			char* label_false = codegen_create_unique_label_with_seed("exp_false_");
			char* label_end = codegen_create_unique_label_with_seed("exp_end_");

			char* result = codegen_gen_temp_register();

			if (cg_exp_left->node_type == ntVARIABLE || cg_exp_left->node_type == ntTERM)
			{
				string_builder_append_chars(SB, "movq\t");
				string_builder_append_chars(SB, cg_exp_left->returned_string);
				string_builder_append_chars(SB, ", %rdx\t#expression left\n");


				if (cg_exp_right->node_type == ntVARIABLE || cg_exp_right->node_type == ntTERM)
				{
					string_builder_append_chars(SB, "movq\t");
					string_builder_append_chars(SB, cg_exp_right->returned_string);
					string_builder_append_chars(SB, ", %rax\t#expression right\n");
				}
				else
				{
				}
				string_builder_append_chars(SB, "cmpq\t%rax, %rdx\t#expression calculation\n");
			}
			else
			{
				if (cg_exp_right->node_type == ntVARIABLE || cg_exp_right->node_type == ntTERM)
				{
					string_builder_append_chars(SB, "movq\t");
					string_builder_append_chars(SB, cg_exp_right->returned_string);
					string_builder_append_chars(SB, ", %rdx\t#expression right\n");
				}
				else
				{
				}
				string_builder_append_chars(SB, "cmpq\t%rdx, %rax\t#expression calculation\n");
			}

			string_builder_append_chars(SB, "jg\t");
			string_builder_append_chars(SB, label_false);
			string_builder_append_chars(SB, "\t#jump on non-equality\n");
			string_builder_append_chars(SB, "movq\t$1, %rax\t#set rax to 1\n");


			string_builder_append_chars(SB, "jmp\t");
			string_builder_append_chars(SB, label_end);
			string_builder_append_chars(SB, "\n");

			string_builder_append_chars(SB, label_false);
			string_builder_append_chars(SB, ":\n");
			string_builder_append_chars(SB, "movq\t$0, %rax\t#Zero RAX\n");


			string_builder_append_chars(SB, label_end);
			string_builder_append_chars(SB, ":\n");

			char* line = string_builder_copy_to_char_array(SB);
			string_builder_clear(SB);
			linked_list_add_last(&GLOBAL_CODE_LIST, line);

			cg->register_info->reg = RAX;

			//return the result back up.
			cg->returned_string = result;
		}
		break;
		case EXP::exp_andK:
		{
			/* @ = unique incremented integer.
			 * jz = "jump if the zero flag is set" instruction.
			 *
			 * code for <exp-left>
			 * code for <exp-right>
			 * andq %rdx, %rax
			 * jz exp_true_
			 * xor %eax, %eax
			 * jmp exp_end_@
			 * exp_false_@:
			 * movq $1, %rax
			 * exp_end_@:
			 *
			 */

			code_info* cg_exp_left = codegen_EXP(exp->val.op.left, offset_index);
			code_info* cg_exp_right = codegen_EXP(exp->val.op.right, offset_index);

			char* label_true = codegen_create_unique_label_with_seed("exp_true_");
			char* label_end = codegen_create_unique_label_with_seed("exp_end_");

			char* result = codegen_gen_temp_register();

			if (cg_exp_left->node_type == ntVARIABLE || cg_exp_left->node_type == ntTERM)
			{
				string_builder_append_chars(SB, "movq\t");
				string_builder_append_chars(SB, cg_exp_left->returned_string);
				string_builder_append_chars(SB, ", %rdx\t#expression left\n");

				if (cg_exp_right->node_type == ntVARIABLE || cg_exp_right->node_type == ntTERM)
				{
					string_builder_append_chars(SB, "movq\t");
					string_builder_append_chars(SB, cg_exp_right->returned_string);
					string_builder_append_chars(SB, ", %rax\t#expression right\n");
				}
				else
				{
				}
				string_builder_append_chars(SB, "andq\t%rdx, %rax\t#expression calculation\n");
			}
			else
			{
				if (cg_exp_right->node_type == ntVARIABLE || cg_exp_right->node_type == ntTERM)
				{
					string_builder_append_chars(SB, "movq\t");
					string_builder_append_chars(SB, cg_exp_right->returned_string);
					string_builder_append_chars(SB, ", %rdx\t#expression right\n");
				}
				else
				{
				}
				string_builder_append_chars(SB, "andq\t%rax, %rdx\t#expression calculation\n");
			}


			string_builder_append_chars(SB, "jz\t");
			string_builder_append_chars(SB, label_true);
			string_builder_append_chars(SB, "\t#jump on non-equality\n");
			string_builder_append_chars(SB, "movq\t$1, %rax\t#set rax to 1\n");

			string_builder_append_chars(SB, "jmp\t");
			string_builder_append_chars(SB, label_end);
			string_builder_append_chars(SB, "\n");

			string_builder_append_chars(SB, label_true);
			string_builder_append_chars(SB, ":\n");
			string_builder_append_chars(SB, "xor\t%eax, %eax\t#Zero EAX\n");
			string_builder_append_chars(SB, label_end);
			string_builder_append_chars(SB, ":\n");

			char* line = string_builder_copy_to_char_array(SB);
			string_builder_clear(SB);
			linked_list_add_last(&GLOBAL_CODE_LIST, line);

			cg->register_info->reg = RAX;

			//return the result back up.
			cg->returned_string = result;
		}
		break;
		case EXP::exp_orK:
		{
			/* @ = unique incremented integer.
			 * jz = "jump if the zero flag is set" instruction.
			 *
			 * code for <exp-left>
			 * code for <exp-right>
			 * andq %rdx, %rax
			 * jz exp_true_
			 * xor %eax, %eax
			 * jmp exp_end_@
			 * exp_false_@:
			 * movq $1, %rax
			 * exp_end_@:
			 *
			 */

			code_info* cg_exp_left = codegen_EXP(exp->val.op.left, offset_index);
			code_info* cg_exp_right = codegen_EXP(exp->val.op.right, offset_index);

			char* label_true = codegen_create_unique_label_with_seed("exp_true_");
			char* label_end = codegen_create_unique_label_with_seed("exp_end_");

			char* result = codegen_gen_temp_register();

			if (cg_exp_left->node_type == ntVARIABLE || cg_exp_left->node_type == ntTERM)
			{
				string_builder_append_chars(SB, "movq\t");
				string_builder_append_chars(SB, cg_exp_left->returned_string);
				string_builder_append_chars(SB, ", %rdx\t#expression left\n");

				if (cg_exp_right->node_type == ntVARIABLE || cg_exp_right->node_type == ntTERM)
				{
					string_builder_append_chars(SB, "movq\t");
					string_builder_append_chars(SB, cg_exp_right->returned_string);
					string_builder_append_chars(SB, ", %rax\t#expression right\n");
				}
				else
				{
				}
				string_builder_append_chars(SB, "orq\t%rdx, %rax\t#expression calculation\n");
			}
			else
			{
				if (cg_exp_right->node_type == ntVARIABLE || cg_exp_right->node_type == ntTERM)
				{
					string_builder_append_chars(SB, "movq\t");
					string_builder_append_chars(SB, cg_exp_right->returned_string);
					string_builder_append_chars(SB, ", %rdx\t#expression right\n");
				}
				else
				{
				}
				string_builder_append_chars(SB, "orq\t%rax, %rdx\t#expression calculation\n");
			}

			string_builder_append_chars(SB, "jz\t");
			string_builder_append_chars(SB, label_true);
			string_builder_append_chars(SB, "\t#jump on non-equality\n");
			string_builder_append_chars(SB, "movq\t$1, %rax\t#set rax to 1\n");

			string_builder_append_chars(SB, "jmp\t");
			string_builder_append_chars(SB, label_end);
			string_builder_append_chars(SB, "\n");

			string_builder_append_chars(SB, label_true);
			string_builder_append_chars(SB, ":\n");
			string_builder_append_chars(SB, "xor\t%eax, %eax\t#Zero EAX\n");
			string_builder_append_chars(SB, label_end);
			string_builder_append_chars(SB, ":\n");

			char* line = string_builder_copy_to_char_array(SB);
			string_builder_clear(SB);
			linked_list_add_last(&GLOBAL_CODE_LIST, line);

			cg->register_info->reg = RAX;

			//return the result back up.
			cg->returned_string = result;

		}
		break;
		case EXP::exp_termK:
		{
			code_info* cg_term = codegen_TERM(exp->val.term, offset_index);
			cg = cg_term;
			cg->returned_string = cg_term->returned_string;
			printf("exp->val.term->kind %d\n", exp->val.term->kind);
		}break;

		default:break;
	}
	return cg;
}

code_info* codegen_TERM(TERM * term, int offset_index)
{
	if (FALCON_DEBUG_CODEGEN)
		printf("|||||------> <term> %d -- %d\n", term->kind, term->lineno);
	code_info* cg = (code_info*)Calloc(1, sizeof(struct code_info));
	register_info* reg = (register_info*)Calloc(1, sizeof(struct register_info));
	cg->register_info = reg;
	cg->node_type = ntTERM;
	switch (term->kind)
	{
		case TERM::term_varK:
		{
			code_info* cg_variable = codegen_VARIABLE(term->val.var, offset_index);
			cg = cg_variable;
		}
		break;
		case TERM::term_id_act_listK:
		{

			code_info* cg_id_act_list = codegen_ACT_LIST(term->val.id_act_list.act_list);

			size_t argument_length = term->val.id_act_list.act_list->type->act_list_count;


			int argument_offset = ((int)argument_length) * GLOBAL_OFFSET_MULTIPLIER;


			string_builder_append_chars(SB, "subq\t$");
			string_builder_append_int(SB, &argument_offset);
			string_builder_append_chars(SB, ",%rsp\n");

			for (size_t i = 0; i < argument_length; ++i)
			{
				type_info_imp* type = (type_info_imp*)array_at(term->val.id_act_list.act_list->type->act_list_types, i);
				type->pushed_at_index = i;
				code_info* codes = (code_info*)array_at(term->val.id_act_list.act_list->list_of_code_infos, i);

				//string_builder_append_chars(SB, push_reg(register_type::RDX));

				string_builder_append_chars(SB, "pushq\t");
				string_builder_append_chars(SB, codes->returned_string);
				string_builder_append_chars(SB, "\t#pushed parameter ");
				int index = (int)i + 1;
				string_builder_append_int(SB, &index);
				string_builder_append_chars(SB, " on the stack\n");

				/*
				string_builder_append_chars(SB, "movq\t");
				string_builder_append_chars(SB, codes->returned_string);
				string_builder_append_chars(SB, ", -");
				int offset = (i + 1) * GLOBAL_OFFSET_MULTIPLIER;
				string_builder_append_int(SB, &offset);
				string_builder_append_chars(SB, "(%rbp)\n");
				*/
			}

			string_builder_append_chars(SB, "callq\t");
			string_builder_append_chars(SB, term->val.id_act_list.id);
			string_builder_append_chars(SB, "\n");


			string_builder_append_chars(SB, "subq\t$");
			string_builder_append_int(SB, &argument_offset);
			string_builder_append_chars(SB, ", %rsp\t#clean the pushed parameters from the stack\n");


			//string_builder_append_chars(SB, pop_reg(register_type::RDX));



			cg->returned_string = "%rax";

		}
		break;
		case TERM::term_parenthesis_expressionK:
		{
			code_info* cg_exp = codegen_EXP(term->val.exp, offset_index);
			cg = cg_exp;
			cg->returned_string = cg_exp->returned_string;
			printf("term->val.exp->kind %d\n", term->val.exp->kind);
			return cg_exp;
		}
		break;
		case TERM::term_negationK:
		{
			code_info* cg_term = codegen_TERM(term->val.term, offset_index);
		}
		break;
		case TERM::term_absoluteK:
		{
			code_info* cg_exp = codegen_EXP(term->val.exp, offset_index);
		}
		break;
		case TERM::term_numK:
		{
			string_builder* temp = string_builder_new();
			string_builder_append_chars(temp, "$");
			string_builder_append_int(temp, &term->val.num);
			cg->returned_string = string_builder_copy_to_char_array(temp);
			string_builder_destroy(temp);
		}
		break;
		case TERM::term_trueK:
		{
			string_builder* temp = string_builder_new();
			int result = 1;
			string_builder_append_chars(temp, "$");
			string_builder_append_int(temp, &result);
			cg->returned_string = string_builder_copy_to_char_array(temp);
			string_builder_destroy(temp);
		}
		break;
		case TERM::term_falseK:
		{
			string_builder* temp = string_builder_new();
			int result = 0;
			string_builder_append_chars(temp, "$");
			string_builder_append_int(temp, &result);
			cg->returned_string = string_builder_copy_to_char_array(temp);
			string_builder_destroy(temp);
		}
		break;
		case TERM::term_nullK:
		{
			string_builder* temp = string_builder_new();
			int result = 0;
			string_builder_append_chars(temp, "$");
			string_builder_append_int(temp, &result);
			cg->returned_string = string_builder_copy_to_char_array(temp);
			string_builder_destroy(temp);
		}
		break;
		case TERM::term_stringK:
		{

			log_info("string pre");
			code_info* cg_string = codegen_STRING(term->val.string);
			cg = cg_string;
			log_info("string post");
		}
		break;
		default:  break;
	}
	return cg;
}


code_info* codegen_ACT_LIST(ACT_LIST * act_list)
{
	if (FALCON_DEBUG_CODEGEN)
		printf("|||||------> <act_list> %d -- %d\n", act_list->kind, act_list->lineno);
	code_info* cg = (code_info*)Calloc(1, sizeof(struct code_info));
	register_info* reg = (register_info*)Calloc(1, sizeof(struct register_info));
	cg->register_info = reg;
	cg->node_type = ntACT_LIST;
	switch (act_list->kind)
	{
		case ACT_LIST::act_list_exp_listK:
		{
			act_list->list_of_code_infos = array_new();
			code_info* cg_exp_list = codegen_EXP_LIST(act_list->exp_list, act_list->list_of_code_infos, 2);
			cg = cg_exp_list;
		}
		break;
		case ACT_LIST::act_list_emptyK:
		{}
		break;
		{}
		default: { log_internal_error(__FILE__, __func__, __LINE__, "default case."); } break;
	}
	return cg;
}


code_info* codegen_EXP_LIST(EXP_LIST * exp_list, array* arr, int offset_count)
{
	if (FALCON_DEBUG_CODEGEN)
		printf("|||||------> <exp_list> %d -- %d\n", exp_list->kind, exp_list->lineno);
	code_info* cg = (code_info*)Calloc(1, sizeof(struct code_info));
	register_info* reg = (register_info*)Calloc(1, sizeof(struct register_info));
	cg->register_info = reg;
	cg->node_type = ntEXP_LIST;
	switch (exp_list->kind)
	{
		case EXP_LIST::exp_list_pairK:
		{
			offset_count++;
			code_info* cg_exp = codegen_EXP(exp_list->val.pair.head, offset_count);
			array_push(arr, cg_exp);
			code_info* cg_exp_list = codegen_EXP_LIST(exp_list->val.pair.tail, arr, offset_count);
		}
		break;
		case EXP_LIST::exp_list_expressionK:
		{
			offset_count++;
			code_info* cg_exp = codegen_EXP(exp_list->val.exp, offset_count);
			array_push(arr, cg_exp);
		}
		break;
		default:  break;
	}
	return cg;
}

