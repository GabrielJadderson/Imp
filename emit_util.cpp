#include "emit_util.h"
#include "memory.h"
#include <cstdio>

extern int CODEGEN_STACK_OFFSET;
extern int CODEGEN_STACK_COUNT;
extern int GLOBAL_OFFSET_MULTIPLIER;

#define IMP_ARCH_WINDOWS
//#define IMP_ARCH_LINUX


char* push(char* string)
{
	string_builder* temp = string_builder_new();
	string_builder_append_chars(temp, "pushq\t");
	string_builder_append_chars(temp, string);
	string_builder_append_chars(temp, "\n");
	char* str = string_builder_copy_to_char_array(temp);
	string_builder_destroy(temp);
	CODEGEN_STACK_COUNT++;
	CODEGEN_STACK_OFFSET += GLOBAL_OFFSET_MULTIPLIER;
	return str;
}

char* pop(char* string)
{
	string_builder* temp = string_builder_new();
	string_builder_append_chars(temp, "popq\t");
	string_builder_append_chars(temp, string);
	string_builder_append_chars(temp, "\n");
	char* str = string_builder_copy_to_char_array(temp);
	string_builder_destroy(temp);
	CODEGEN_STACK_COUNT--;
	CODEGEN_STACK_OFFSET -= GLOBAL_OFFSET_MULTIPLIER;
	return str;
}


char* push_reg(register_type type)
{
	string_builder* temp = string_builder_new();
	string_builder_append_chars(temp, "pushq\t");
	string_builder_append_chars(temp, register_to_string(type));
	string_builder_append_chars(temp, "\n");
	char* str = string_builder_copy_to_char_array(temp);
	string_builder_destroy(temp);
	CODEGEN_STACK_COUNT++;
	CODEGEN_STACK_OFFSET += GLOBAL_OFFSET_MULTIPLIER;
	return str;
}

char* pop_reg(register_type type)
{
	string_builder* temp = string_builder_new();
	string_builder_append_chars(temp, "popq\t");
	string_builder_append_chars(temp, register_to_string(type));
	string_builder_append_chars(temp, "\n");
	char* str = string_builder_copy_to_char_array(temp);
	string_builder_destroy(temp);
	CODEGEN_STACK_COUNT--;
	CODEGEN_STACK_OFFSET -= GLOBAL_OFFSET_MULTIPLIER;
	return str;
}


char* register_to_string(register_type type)
{
	switch (type)
	{
		case register_type::R8: { return "%r8"; }break;
		case register_type::R9: { return "%r9"; }break;
		case register_type::R10: { return "%r10"; }break;
		case register_type::R11: { return "%r11"; }break;
		case register_type::R12: { return "%r12"; }break;
		case register_type::R13: { return "%r13"; }break;
		case register_type::R14: { return "%r14"; }break;
		case register_type::R15: { return "%r15"; }break;
		case register_type::RAX: { return "%rax"; }break;
		case register_type::RCX: { return "%rcx"; }break;
		case register_type::RDX: { return "%rdx"; }break;
		case register_type::RBX: { return "%rbx"; }break;
		case register_type::RSP: { return "%rsp"; }break;
		case register_type::RBP: { return "%rbp"; }break;
		case register_type::RSI: { return "%rsi"; }break;
		case register_type::RDI: { return "%rdi"; }break;
		case register_type::EAX: { return "%eax"; }break;
		case register_type::ECX: { return "%ecx"; }break;
		case register_type::EBX: { return "%ebx"; }break;
		case register_type::ESP: { return "%esp"; }break;
		case register_type::EBP: { return "%ebp"; }break;
		case register_type::ESI: { return "%esi"; }break;
		case register_type::EDI: { return "%edi"; }break;
	}
}

char* function_epilogue(char* function_id, array* used_registers)
{
	string_builder* temp = string_builder_new();
	string_builder_append_chars(temp, "\t\t#CALLER EPILOGUE of \"");
	string_builder_append_chars(temp, function_id);
	string_builder_append_chars(temp, "\"\n");
	size_t used_registers_count = used_registers->length;
	for (int i = used_registers_count - 1; i >= 0; i--)
	{

		register_info* reg_info = static_cast<register_info*>(array_at(used_registers, i));
		if (reg_info)
		{
			string_builder_append_chars(temp, pop(register_to_string(reg_info->reg)));
		}
	}
	string_builder_append_chars(temp, "\n");
	char* str = string_builder_copy_to_char_array(temp);
	string_builder_destroy(temp);
	return str;
}

char* function_prologue(char* function_id, array* used_registers)
{
	string_builder* temp = string_builder_new();
	string_builder_append_chars(temp, "\t\t#CALLER PROLOGUE of \"");
	string_builder_append_chars(temp, function_id);
	string_builder_append_chars(temp, "\"\n");
	size_t used_registers_count = used_registers->length;
	for (size_t i = 0; i < used_registers_count; i++)
	{
		register_info* reg_info = static_cast<register_info*>(array_at(used_registers, i));
		if (reg_info)
		{
			string_builder_append_chars(temp, push(register_to_string(reg_info->reg)));
		}
	}
	string_builder_append_chars(temp, "\n");
	char* str = string_builder_copy_to_char_array(temp);
	string_builder_destroy(temp);
	return str;
}



char* generate_int_to_string(char* integer_code, char* buffer_code)
{
	string_builder* temp = string_builder_new();
	string_builder_append_chars(temp, "pushq\t%rax\n");
	string_builder_append_chars(temp, "pushq\t%rdi\n");
	string_builder_append_chars(temp, "pushq\t%rsi\n");
	string_builder_append_chars(temp, "pushq\t%rdx\n");
	string_builder_append_chars(temp, "pushq\t%rbp\n");
	string_builder_append_chars(temp, "movq\t%rsp, %rbp\n");

	string_builder_append_chars(temp, "movq\t");
	string_builder_append_chars(temp, integer_code);
	string_builder_append_chars(temp, ", %rdx\t# load integer.\n");
	string_builder_append_chars(temp, "movq\t");
	string_builder_append_chars(temp, buffer_code);
	string_builder_append_chars(temp, ", %rsi\t#load buffer.\n");
	string_builder_append_chars(temp, "movq\t$printf_format_int, %rdi\t#load format.\n");
	string_builder_append_chars(temp, "movq\t$0, %rax\t#load 0 into rax.\n");
	string_builder_append_chars(temp, "call\tsprintf\n");

	string_builder_append_chars(temp, "movq\t%rbp, %rsp\n");
	string_builder_append_chars(temp, "popq\t%rbp\n");
	string_builder_append_chars(temp, "popq\t%rdx\n");
	string_builder_append_chars(temp, "popq\t%rsi\n");
	string_builder_append_chars(temp, "popq\t%rdi\n");
	string_builder_append_chars(temp, "popq\t%rax\n");
	char* str = string_builder_copy_to_char_array(temp);
	string_builder_destroy(temp);
	return str;
}


register_info* register_new_of_type(register_type type)
{
	auto reg = static_cast<register_info*>(Calloc(1, sizeof(register_info)));
	reg->reg = type;
	return reg;
}




