%{
#include "imp.tab.h"
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

extern int GLOBAL_ERROR_COUNT;
extern int lineno;
extern int fileno();


unsigned int str_length = 0;
unsigned int str_capacity = 32; //initial capacity of 16 characters use a power of two!
char* string_buf_ptr = 0;

unsigned int multi_line_inception = 0; //incremented each time we enter a nested multi-line-comment.
unsigned int error_count_comments = 0; //used to count errors during the scanning phase such that we can report them out.
unsigned int error_count = 0; //used to count errors during the scanning phase such that we can report them out.

%}

%option noyywrap noinput

%x slash_state
%x multi_line_comment
%x comment
%x command
%x command_load
%x command_load_string
%x identifier_state
%x string



%%
 /* ================= command BEGIN ==================== */
"#"		{ BEGIN(command); }
<command>"load"         { BEGIN(INITIAL);  return tCOMMAND_LOAD; }
<command>"shader"         { BEGIN(INITIAL);  return tCOMMAND_SHADER; }
<command>"contain"         { BEGIN(INITIAL);  return tCOMMAND_CONTAIN; }
<command>"build"         { BEGIN(INITIAL);  return tCOMMAND_BUILD; }
<command>\n             { lineno++; BEGIN(INITIAL); }
<command>.             { BEGIN(INITIAL); yyless(0); }

 /* ================= command END ==================== */

 /* ================= COMMENTS BEGIN ==================== */
"/"		{ BEGIN(slash_state); }
<slash_state>"/"             { lineno++; BEGIN(comment); }
<slash_state>"*"             { lineno++; BEGIN(multi_line_comment); multi_line_inception++;}
<slash_state>.             { lineno++; BEGIN(INITIAL); return '/';}

<comment>\n             { lineno++; BEGIN(INITIAL);}
<comment>.				/* ignore everything else */

<multi_line_comment>"/*"	{ multi_line_inception++; }
<multi_line_comment>\n      {lineno++;}
<multi_line_comment>.		{/* eat everything else. */}
<multi_line_comment>"*"+"/"	{
					multi_line_inception--;
					if (multi_line_inception == 0)
					{
						BEGIN(INITIAL);
					}
					error_count_comments = multi_line_inception;
				}
 /* ================= COMMENTS END ==================== */

 /* ================= STRING HANDLING BEGIN ==================== */
 /* this initial snippet was taken from
  *	https://www.imada.sdu.dk/~kslarsen/dm546/Material/flex.pdf - pages 24-25
  * however, it has been heavily modified and expanded upon to suit our needs.
  */

\"	{ string_buf_ptr = (char*)calloc(str_capacity, sizeof(char)); BEGIN(string);}

<string>\"	{ /* saw closing quote - all done */
			if (str_length > str_capacity)
			{
				str_capacity = str_capacity << 1; //increase by next power of two.
				char* ptr = (char*)realloc(string_buf_ptr, str_capacity);
				if (ptr)
				{
					string_buf_ptr = ptr;
				} else { printf("ERROR FAILED TO SCAN STRING, REALLOC FAILURE.\n"); exit(1);}
			}
			string_buf_ptr[str_length] = '\0'; //terminate the string.

			BEGIN(INITIAL);


			/* return to bison. (NOTE: it's important to begin intial before returning the token.) */
			yylval.dynamic_string = (char *)calloc(str_length, sizeof(char));
			strcpy(yylval.dynamic_string, string_buf_ptr);
			//printf("|scanned| --> string: %s\n", yylval.dynamic_string);

			//clean-up
			free(string_buf_ptr);
			string_buf_ptr = 0;
			str_length = 0;
			str_capacity = 16;

            return tSTRING;

        }

<string>\n	{
			/* error - unterminated string constant */
			/* generate error message */
			error_count++;
			printf("error string unterminated? at line %d\n", lineno);
		}

<string>\\[0-7]{1,3}	{
			/* octal escape sequence so we can have colored strings!!! xD */
			unsigned int result;

		    (void) sscanf( yytext + 1, "%o", &result );

	        if ( result > 0xff )
					error_count++;
		            /* error, constant is out-of-bounds */

			//printf("eating string %d at line %d\n", result, lineno);

			if (str_length > str_capacity)
			{
				str_capacity = str_capacity << 1; //increase by next power of two.
				char* ptr = (char*)realloc(string_buf_ptr, str_capacity);
				if (ptr)
				{
					string_buf_ptr = ptr;
				} else { printf("ERROR FAILED TO SCAN STRING, REALLOC FAILURE.\n"); exit(1);}
			}
			string_buf_ptr[str_length] = result;
			str_length++;
       }

<string>\\[0-9]+ {
        /* generate error - bad escape sequence; something
         * like '\48' or '\0777777'
         */
		 error_count++;
			//printf("bad string escape sequence; at line %d\n", lineno);
		}

<string>\\n	{
			if (str_length > str_capacity)
			{
				str_capacity = str_capacity << 1; //increase by next power of two.
				char* ptr = (char*)realloc(string_buf_ptr, str_capacity);
				if (ptr)
				{
					string_buf_ptr = ptr;
				} else { printf("ERROR FAILED TO SCAN STRING, REALLOC FAILURE.\n"); exit(1);}
			}
			string_buf_ptr[str_length] = '\n';
			str_length++;
		}
<string>\\t	{
			if (str_length > str_capacity)
			{
				str_capacity = str_capacity << 1; //increase by next power of two.
				char* ptr = (char*)realloc(string_buf_ptr, str_capacity);
				if (ptr)
				{
					string_buf_ptr = ptr;
				} else { printf("ERROR FAILED TO SCAN STRING, REALLOC FAILURE.\n"); exit(1);}
			}
			string_buf_ptr[str_length] = '\t';
			str_length++;
		}
<string>\\r	{
			if (str_length > str_capacity)
			{
				str_capacity = str_capacity << 1; //increase by next power of two.
				char* ptr = (char*)realloc(string_buf_ptr, str_capacity);
				if (ptr)
				{
					string_buf_ptr = ptr;
				} else { printf("ERROR FAILED TO SCAN STRING, REALLOC FAILURE.\n"); exit(1);}
			}
			string_buf_ptr[str_length] = '\r';
			str_length++;
		}
<string>\\b	{
			if (str_length > str_capacity)
			{
				str_capacity = str_capacity << 1; //increase by next power of two.
				char* ptr = (char*)realloc(string_buf_ptr, str_capacity);
				if (ptr)
				{
					string_buf_ptr = ptr;
				} else { printf("ERROR FAILED TO SCAN STRING, REALLOC FAILURE.\n"); exit(1);}
			}
			string_buf_ptr[str_length] = '\b';
			str_length++;
		}
<string>\\f	{
			if (str_length > str_capacity)
			{
				str_capacity = str_capacity << 1; //increase by next power of two.
				char* ptr = (char*)realloc(string_buf_ptr, str_capacity);
				if (ptr)
				{
					string_buf_ptr = ptr;
				 } else { printf("ERROR FAILED TO SCAN STRING, REALLOC FAILURE.\n"); exit(1);}
			}
			string_buf_ptr[str_length] = '\f';
			str_length++;
		}

<string>\\(.|\n)	{
			if (str_length > str_capacity)
			{
				str_capacity = str_capacity << 1; //increase by next power of two.
				char* ptr = (char*)realloc(string_buf_ptr, str_capacity);
				if (ptr)
				{
					string_buf_ptr = ptr;
				} else { printf("ERROR FAILED TO SCAN STRING, REALLOC FAILURE.\n"); exit(1);}
			}
			string_buf_ptr[str_length] = yytext[1];
			str_length++;
		}

<string>[^\\\n\"]+	{
        char *yptr = yytext;

        while (*yptr)
			{
				if (str_length == 0) //handle special initial case.
				{
					string_buf_ptr[str_length] = *yptr++;
					str_length++;
				} else {
					if (str_length > str_capacity)
					{
						str_capacity = str_capacity << 1; //increase by next power of two.
						char* ptr = (char*)realloc(string_buf_ptr, str_capacity);
						if (ptr)
						{
							string_buf_ptr = ptr;
						} else { printf("ERROR FAILED TO SCAN STRING, REALLOC FAILURE.\n"); exit(1);}
					}
					string_buf_ptr[str_length] = *yptr++;
					str_length++;
				}
			}
        }


 /* ================= STRING HANDLING END ==================== */

[ \t]+        /* ignore */;
[ \r]		/* for windows :) */
\n              lineno++;
"*"             return '*';
"/"             return '/';
"+"             return '+';
"-"             return '-';
"("             return '(';
")"             return ')';
";"				return ';';
":"				return ':';
","				return ',';
"{"				return '{';
"}"				return '}';
"["				return '[';
"]"				return ']';
"="				return '=';
"<"             return '<';
">"             return '>';
"!"				return '!';
"|"				return '|';
"."				return '.';



"+="			return tPLUS_EQUAL;
"-="			return tMINUS_EQUAL;
"*="			return tTIMES_EQUAL;
"/="			return tDIVIDE_EQUAL;
"&="			return tAND_EQUAL;
"|="			return tOR_EQUAL;


"=="           	return tEQUALITY;
"<="            return tLESSER_EQUAL;
">="            return tGREATER_EQUAL;
"!="            return tNON_EQUALITY;
"&&"            return tAND;
"||"            return tOR;

"print"			return tPRINT;
"return" 		return tRETURN;
"if"			return tIF;
"else" 			return tELSE;
"then" 			return tTHEN;
"while"			return tWHILE;
"do"			return tDO;
"type"			return tTYPE;
"end"			return tEND;
"of length"		return tOF_LENGTH;
"var"			return tVAR;
"allocate"		return tALLOCATE;
"func"			return tFUNC;


 /* types */
"byte"			return tBYTE;
"int"			return tINT;
"short"			return tSHORT;
"long"			return tLONG;
"float"			return tFLOAT;
"double"		return tDOUBLE;
"u8"			return tu8;
"u16"			return tu16;
"u32"			return tu32;
"u64"			return tu64;
"u128"			return tu128;
"u256"			return tu256;
"u512"			return tu512;

"string"		return tSTRING_TOKEN;
"bool"			return tBOOL;
"true"			return tTRUE;
"false"			return tFALSE;
"null"			return tNULL;

"bitset"		return tBITSET;
"[]"			return tBOX;				/* array */
"struct"		return tSTRUCT;
"union"			return tUNION;
"void"			return tVOID;
"SOA"			return tSOA;


"for"			return tFOR;
"::"			return tDOUBLE_SEMICOLON;
"->"			return tRIGHT_ARROW;
"<-"			return tLEFT_ARROW;



0|([1-9][0-9]*)        { yylval.intconst = atoi(yytext);
                       return tINTCONST; }
[a-zA-Z_][a-zA-Z0-9_]* {
		yylval.stringconst = (char *)calloc(strlen(yytext)+1, 1);
		sprintf(yylval.stringconst,"%s",yytext);
		printf("found %s\n", yylval.stringconst);


		BEGIN(identifier_state);

		}

<identifier_state>[ \t]+ /* ignore */
<identifier_state>":" { /*printf("found : %d\n", yyleng);*/ BEGIN(INITIAL); return tIDENTIFIER_DECL;  }
<identifier_state>. { /*printf("found . %d\n", yyleng);*/

	yyless(0);
/* yyless(2); unput(2);*/
	/*
	int i;
	*/
    /* Copy yytext because unput() trashes yytext */
    /*
	char *yycopy = strdup( yytext );
    for ( i = yyleng - 1; i >= 0; --i )
        unput( yycopy[i] );
    free( yycopy );
	*/
	BEGIN(INITIAL);
	return tIDENTIFIER;
	}

.	{
		fprintf(stderr, "\x1b[37;1m");
		fprintf(stderr, "%d: ", lineno);
		fprintf(stderr, "\x1b[0m"); //reset color.
		fprintf(stderr, "\x1b[31;1m"); //start print color magenta.
		fprintf(stderr, "Error: "); //start print color magenta.
		fprintf(stderr, "\x1b[0m"); //reset color.
		fprintf(stderr, "Invalid Token ");
		fprintf(stderr, "\x1b[37;1m'");
		fprintf(stderr, "%s", yytext);
		fprintf(stderr, "'\x1b[0m.\n");
		GLOBAL_ERROR_COUNT++;
	}

<<EOF>>	{
		GLOBAL_ERROR_COUNT += error_count_comments;
		GLOBAL_ERROR_COUNT += error_count;
		yyterminate();
		return 0;
		}

%%
