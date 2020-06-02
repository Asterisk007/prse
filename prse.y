%{
	extern int yylex(void);
	extern int yyerror(const char*);
	extern int line_count;

	#include <iostream>
	#include <string>
	//#include "Constant.h"
	using namespace std;
%}

%union {
	bool			union_bool;
	int 			union_int;
	char*			union_char;
	double 			union_double;
	//float 			union_float;
	std::string* 	union_string;
}

%destructor { delete $$; } <union_string>

%error-verbose

// Tokens
// Data constants
%token <union_bool>		BOOL_CONST		"boolean constant"
%token <union_int> 		INT_CONST		"int constant"
//%token <union_float> 	FLOAT 			"float"
%token <union_double> 	DOUBLE_CONST	"double constant"
%token <union_char> 	CHAR_CONST		"char constant"
%token <union_string> 	STRING_CONST	"string constant"
// Variable types
%token BOOL					"boolean"
%token INT					"int"
%token DOUBLE				"double"
%token CHAR					"char"
%token STRING				"string"
%token VOID					"void"
// Key tokens
%token USE 					"use"
%token VAR 					"var"
%token FUNCTION 			"func"
%token MAIN					"main"
%token RETURN 				"return"
%token CLASS				"class"
// Flow control
%token IF 					"if"
%token ELSE 				"else"
%token FOR					"for"
// Arithmetic
%token ASSIGN 				"="
%token PLUS 				"+"
%token MINUS 				"-"
%token STAR 				"*"
%token SLASH 				"/"
%token CARET 				"^"
%token INCREMENT 			"++"
%token DECREMENT 			"--"
%token PLUS_ASSIGN 		"+="
%token MINUS_ASSIGN 		"-="
// Logic
%token LOGIC_TRUE			"true"
%token LOGIC_FALSE			"false"
%token LOGIC_EQ				"=="
%token LOGIC_NE				"!="
%token LOGIC_NOT			"!"
%token LOGIC_AND			"&&"
%token LOGIC_OR				"||"
%token LOGIC_GREATER		">"
%token LOGIC_LESS			"<"
%token LOGIC_GREATER_EQUAL	">="
%token LOGIC_LESS_EQUAL		"<="
// Miscellaneous
%token PERIOD 				"."
%token COMMA 				","
%token COLON 				":"
%token SEMICOLON 			";"
%token DOUBLE_QUOTE 		"\""
%token SINGLE_QUOTE 		"'"
%token L_PAREN				"("
%token R_PAREN				")"
%token L_SQUARE_BRACKET 	"["
%token R_SQUARE_BRACKET 	"]"
%token L_CURLY_BRACKET 		"{"
%token R_CURLY_BRACKET 		"}"
%token ERROR
// Variable identification
%token <union_string> ID 	"identifier"

// %type <union_string> import_list
// %type <union_string> import_statement
// %type <union_string> declaration_list
// %type <union_string> function_list
// %type <union_string> import_multiple
%type <union_string> variable_type
%type <union_string> constant

%%

program:
	import_list declaration_list function_list
	;
	
import_list:
	import_list import_statement
	| empty
	;

import_statement:
	USE import_multiple {
		cout << "Using libraries: ";
	}
	;

import_multiple:
	STRING_CONST COMMA import_multiple {
		cout << $1 << ", ";
	}
	| STRING_CONST {
		cout << $1 << endl;
	}
	;

declaration_list:
	declaration_list variable_declaration 
	| empty
	;

variable_declaration:
	VAR ID COLON variable_type ASSIGN constant {
		cout << "Variable " << $2 << " with type " << $4 << " set to " << $6 << endl;
	}
	| ID ASSIGN constant {
		cout << "Variable " << $1 << " set to " << $3 << endl;
	}
	;

function_list:
	function_list function_definition
	| empty
	;

// Function definitions can consist of none, one, or many arguments, and can have a declared return type (default void)
function_definition:
	FUNCTION ID L_PAREN argument_list R_PAREN L_CURLY_BRACKET expression_list R_CURLY_BRACKET {
		cout << "Function " << $2 << ":" << endl;
		cout << "Arguments:" << endl;
	}
	| FUNCTION ID L_PAREN argument_list R_PAREN COLON variable_type L_CURLY_BRACKET expression_list R_CURLY_BRACKET {
		cout << "Function " << $2 << " returns " << $7 << ":" << endl;
		cout << "Arguments:" << endl;
	}
	| FUNCTION ID L_PAREN R_PAREN L_CURLY_BRACKET expression_list R_CURLY_BRACKET {
		cout << "Function " << $2 << endl;
	}
	| FUNCTION ID L_PAREN R_PAREN COLON variable_type L_CURLY_BRACKET expression_list R_CURLY_BRACKET {
		cout << "Function " << $2 << " returns " << $6 << ":" << endl;
	}
	;

expression_list:
	expression_list expression
	| empty
	;

expression:
	empty
	;

argument_list:
	argument COMMA argument_list
	| argument
	;

argument:
	ID COLON variable_type {
		cout << $1 << " " << $3 << endl;
	}
	;

constant:
	BOOL_CONST 		{ $$ = ($1 == true) ? new std::string("true") : new std::string("false"); }
	| INT_CONST 	{ $$ = new std::string(std::to_string($1)); }
	| CHAR_CONST 	{ $$ = new std::string($1); }
	| DOUBLE_CONST 	{ $$ = new std::string(std::to_string($1)); }
	| STRING_CONST 	{ $$ = new std::string(*$1); delete $1; }
	;

variable_type:
	BOOL { $$ = new std::string("bool"); }
	| INT { $$ = new std::string("int"); }
	| CHAR { $$ = new std::string("char"); }
	| DOUBLE { $$ = new std::string("double"); }
	| STRING { $$ = new std::string("string"); }
	;

empty:
	;

%%

int main(void){
	return yyparse();
}
