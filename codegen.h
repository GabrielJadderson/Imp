#pragma once
#include "tree.h"
#include "linked_list.h"
#include "type_checker.h"

enum register_type
{
	//SSE2 Registers XMM0–XMM7 are 128 bits wide but only accessible through SSE instructions
	XMM0,
	XMM1,
	XMM2,
	XMM3,
	XMM4,
	XMM5,
	XMM6,
	XMM7,

	//256-bit registers YMM0–YMM7
	YMM0,
	YMM1,
	YMM2,
	YMM3,
	YMM4,
	YMM5,
	YMM6,
	YMM7,

	//General-Purpose Registers
	//R8–R15 are the new 64-bit registers.
	R8,
	R9,
	R10,
	R11,
	R12,
	R13,
	R14,
	R15,
	//R8D–R15D are the lowermost 32 bits of each register.
	R8D,
	R9D,
	R10D,
	R11D,
	R12D,
	R13D,
	R14D,
	R15D,
	//R8W–R15W are the lowermost 16 bits of each register.
	R8W,
	R9W,
	R10W,
	R11W,
	R12W,
	R13W,
	R14W,
	R15W,
	//R8B–R15B are the lowermost 8 bits of each register.
	R8B,
	R9B,
	R10B,
	R11B,
	R12B,
	R13B,
	R14B,
	R15B,

	//Instruction Pointer
	RIP,
	EIP,

	//64-bit
	RAX, //accumulator, Used in arithmetic operations
	RCX, //counter, Used in shift/rotate instructions and loops.
	RDX, //data, Used in arithmetic operations and I/O operations.
	RBX, //base, Used as a pointer to data (located in segment register DS, when in segmented mode).
	RSP, //stack pointer, Pointer to the top of the stack.
	RBP, //stack base pointer, Used to point to the base of the stack.
	RSI, //source, Used as a pointer to a source in stream operations.
	RDI, //destination, Used as a pointer to a destination in stream operations.

	//32-bit
	EAX, //accumulator
	ECX, //counter
	EBX, //data
	ESP, //base,
	EBP, //stack
	ESI, //source
	EDI, //destination

	//16-bit
	AX, //accumulator
	CX,	//counter
	DX,	//data
	BX,	//base,
	SP,	//stack pointer
	BP,	//stack base pointer
	SI,	//source
	DI,  //destination

	//8-bit
	AH, //accumulator
	AL, //accumulator
	//----
	CH, //counter
	CL, //counter
	//----
	DH, //data
	DL, //data
	//----
	BH, //base
	BL, //base
	//----
	SPL, //Stack Pointer
	BPL, //stack base pointer
	SIL, //source
	DIL, //destination

	//Segment Registers
	SS, //stack segment, pointer to stack
	CS, //code segment, pointer to code
	DS, //Data Segment, Pointer to the data.
	ES, //Extra Segment, Pointer to extra data ('E' stands for 'Extra').
	FS, //F Segment, Pointer to more extra data ('F' comes after 'E').
	GS, //G segment, Pointer to still more extra data ('G' comes after 'F').

	NONE,
};

typedef struct register_info
{
	enum register_type reg;
	bool alive;
} register_info;


//structure to convey data in the recurtion
typedef struct code_info
{
	enum e_node_type node_type;

	array* array_of_directives;


	int num;
	SymbolTable* current_scope;

	char** ptr_to_static_string_label;

	type_info* type_information;
	char* returned_string;
	register_info* register_info;


} code_info;



//char* code_gen_resolve_register_to_string(cg_data* cg);
char* code_gen_create_unique_label();
char* code_gen_create_unique_label_with_seed(char* seed);

linked_list* codegen(AST_NODE* AST, SymbolTable* root);
code_info* codegen_AST(AST_NODE* ast);
code_info* codegen_DIRECTIVE_LIST(DIRECTIVE_LIST* directive_list);
code_info* codegen_DIRECTIVE(DIRECTIVE* directive);

code_info* codegen_STRING(STRING* str);
code_info* codegen_FUNCTION(FUNCTION* f);
code_info* codegen_HEAD(HEAD* head);
code_info* codegen_TYPE(TYPE* type);
code_info* codegen_PAR_DECL_LIST(PAR_DECL_LIST* par_decl_list);
code_info* codegen_VAR_DECL_LIST(VAR_DECL_LIST* var_decl_list, array* arr, int offset_count, bool is_parameter);
code_info* codegen_VAR_TYPE(VAR_TYPE* var_type, int offset, bool is_var_type_declaration, bool is_parameter);
code_info* codegen_BODY(BODY* body);
//code_info* codegen_BODY_DECL_LIST(BODY_DECL_LIST* body_decl_list);
code_info* codegen_DECL_LIST(DECL_LIST* decl_list, array* array_of_declarations);
code_info* codegen_DECL(DECL* decl);
code_info* codegen_STM_LIST(STM_LIST* stm_list);
code_info* codegen_STM(STM* stm);
code_info* codegen_VARIABLE(VARIABLE* var, int offset_index);
code_info* codegen_EXP(EXP* exp, int offset_index);
code_info* codegen_TERM(TERM* term, int offset_index);
code_info* codegen_ACT_LIST(ACT_LIST* act_list);
code_info* codegen_EXP_LIST(EXP_LIST * exp_list, array* arr, int offset_count);


