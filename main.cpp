#include "tree.h"
#include "weeder.h"
#include "pretty.h"
#include "type_checker.h"
#include <stdio.h>
#include "falcon_arguments.h"
#include <stdlib.h>
#include "file_io.h"
#include "ir.h"
#include "imp.tab.h"
#include "pre_ir_weed.h"
#include <unordered_map>
#include "codegen.h"
#include "logger.h"


#define internal static
#define local_persist static
#define global_variable static

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef uint8_t ui8;
typedef uint16_t ui16;
typedef uint32_t ui32;
typedef uint64_t ui64;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef int32 bool32;



char FALCON_DEBUG_PARSER;
char FALCON_DEBUG_AST;
char FALCON_DEBUG_CODEGEN;
char FALCON_DEBUG_IR;
char FALCON_DEBUG_WEEDER;
char FALCON_DEBUG_TYPECHECK;
char FALCON_DEBUG_TYPECHECK_SYMBOLTABLE;
char FALCON_DEBUG_IR_WEED;
std::unordered_map<std::string, call_node*> CALL_TABLE;
std::unordered_map<std::string, FUNCTION*> FUNCTION_LIST;
std::unordered_map<std::string, VAR_TYPE*> GLOBAL_VARIABLE_LIST;
std::unordered_map<std::string, FUNCTION*> GLOBAL_FUNCTION_LIST;

array* calling_list;
int lineno;

int GLOBAL_ERROR_COUNT;


AST_NODE* AST;

int main(int argc, char** argv)
{


	std::unordered_map<std::string, call_node*> CALL_TABLE;
	std::unordered_map<std::string, FUNCTION*> FUNCTION_LIST;
	std::unordered_map<std::string, VAR_TYPE*> GLOBAL_VARIABLE_LIST;
	std::unordered_map<std::string, FUNCTION*> GLOBAL_FUNCTION_LIST;

	GLOBAL_ERROR_COUNT = 0;

	log_warning("here");

	//handles input arguments, just (somewhat) like gcc.
	falcon_config* config = falcon_argumenter(argc, argv);
	FALCON_DEBUG_PARSER = config->FALCON_DEBUG_PARSER;
	FALCON_DEBUG_AST = config->FALCON_DEBUG_AST;
	FALCON_DEBUG_IR = config->FALCON_DEBUG_IR;
	FALCON_DEBUG_WEEDER = config->FALCON_DEBUG_WEEDER;
	FALCON_DEBUG_IR_WEED = config->FALCON_DEBUG_IR_WEED;
	FALCON_DEBUG_TYPECHECK = config->FALCON_DEBUG_TYPECHECK;
	FALCON_DEBUG_TYPECHECK_SYMBOLTABLE = config->FALCON_DEBUG_TYPECHECK_SYMBOLTABLE;
	FALCON_DEBUG_CODEGEN = config->FALCON_DEBUG_CODEGEN;


	lineno = 1;
	GLOBAL_ERROR_COUNT += yyparse();

	if (GLOBAL_ERROR_COUNT > 0)
	{
		fprintf(stderr, "\x1b[37;1m");
		fprintf(stderr, "%d: ", lineno); //print  lineno
		fprintf(stderr, "\x1b[0m"); //reset color.
		fprintf(stderr, "\x1b[31;1m"); //start print color magenta.
		fprintf(stderr, "%d Errors ", GLOBAL_ERROR_COUNT); //start print color magenta.
		fprintf(stderr, "\x1b[0m"); //reset color.
		fprintf(stderr, "Detected during scanning and parsing phase. Exiting..");
		fprintf(stderr, "\x1b[0m.\n");
		exit(1);
	}


	pretty_print(AST);
	log_warning("here 2");
	weed(AST);
	log_warning("here 3");

	SymbolTable* root = type_checker(AST);
	log_warning("here 4");
	if (GLOBAL_ERROR_COUNT > 0)
	{
		fprintf(stderr, "\x1b[37;1m");
		fprintf(stderr, "%d: ", lineno); //print  lineno
		fprintf(stderr, "\x1b[0m"); //reset color.
		fprintf(stderr, "\x1b[31;1m"); //start print color magenta.
		fprintf(stderr, "%d Errors ", GLOBAL_ERROR_COUNT); //start print color magenta.
		fprintf(stderr, "\x1b[0m"); //reset color.
		fprintf(stderr, "Detected during type checking phase. Exiting..");
		fprintf(stderr, "\x1b[0m.\n");
		exit(1);
	}


	calling_list = array_new();
	log_warning("pre_ir weed");
	pre_ir_weed(AST);
	log_warning("here 5");
	intermediate_representation(AST);
	log_warning("here 6");
	//codegen(AST, root);
	log_warning("here 7");



	//inked_list* IR_linked_list = code_gen(AST, root);

	//linked_list_print(&IR_linked_list);

	//GHJ: disabled for now, since it's quite bugged at the moment.
	//there's a bug with multiple expressions, 5+4 works fine but 5+4+3 is not handled correctly.
	//peephole (IR_linked_list);




	return 0;
}

