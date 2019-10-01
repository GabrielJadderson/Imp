#pragma once

#include "falcon_string.h"
#include "codegen.h"

char* push(char* string);
char* pop(char* string);
char* push_reg(register_type type);
char* pop_reg(register_type type);

char* register_to_string(register_type type);

char* function_prologue(char* function_id, array* used_registers);
char* function_epilogue(char* function_id, array* used_registers);


char* generate_int_to_string(char* integer_code, char* buffer_code);


register_info* register_new_of_type(register_type type);