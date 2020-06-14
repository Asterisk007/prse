%{
	#include <stdio.h>
	#include <iostream>
	#include <string>
   #include <vector>
   #include "parser.h"
	//#include "Constant.h"
	using namespace std;

	int yylex(void);
	void yyerror(const char*);
	extern int line_count;
%}

%union {
	bool                  union_bool;
	int                   union_int;
	char*                 union_char;
	double                union_double;
	//float               union_float;
	std::string*          union_string;
   Arg_list*             union_arglist; 
}

%destructor { delete $$; } <union_string>

%error-verbose

// Tokens
// Data constants
%token <union_bool>		BOOL_CONST		"boolean constant"
%token <union_int> 		INT_CONST		"int constant"
//%token <union_float> 	FLOAT 			"float"
%token <union_double> 	DOUBLE_CONST	"double constant"
%token <union_char> 	   CHAR_CONST		"char constant"
%token <union_string> 	STRING_CONST	"string constant"
// Variable types
%token BOOL				   	"boolean"
%token INT				   	"int"
%token DOUBLE			   	"double"
%token CHAR				   	"char"
%token STRING			   	"string"
%token VOID				   	"void"
// Key tokens
%token USE 				   	"use"
%token VAR 				   	"var"
%token FUNCTION 		   	"func"
%token MAIN				   	"main"
%token RETURN 			   	"return"
%token CLASS			   	"class"
%token PUBLIC			   	"public"
%token PRIVATE			   	"private"
// Flow control
%token IF 				   	"if"
%token ELSE 			   	"else"
%token FOR				   	"for"
// Arithmetic
%token ASSIGN 			   	"="
%token PLUS 			   	"+"
%token MINUS 			   	"-"
%token STAR 			   	"*"
%token SLASH 			   	"/"
%token CARET 			   	"^"
%token INCREMENT 		   	"++"
%token DECREMENT 		   	"--"
%token PLUS_ASSIGN 	   	"+="
%token MINUS_ASSIGN 	   	"-="
// Logic
%token LOGIC_TRUE		   	"true"
%token LOGIC_FALSE	   	"false"
%token LOGIC_EQ		   	"=="
%token LOGIC_NE		   	"!="
%token LOGIC_NOT		   	"!"
%token LOGIC_AND		   	"&&"
%token LOGIC_OR		   	"||"
%token LOGIC_GREATER	   	">"
%token LOGIC_LESS		   	"<"
%token LOGIC_GREATER_EQUAL	">="
%token LOGIC_LESS_EQUAL		"<="
// Miscellaneous
%token PERIOD 			   	"."
%token COMMA 			   	","
%token COLON 			   	":"
%token SEMICOLON 		   	";"
%token DOUBLE_QUOTE 	   	"\""
%token SINGLE_QUOTE 	   	"'"
%token L_PAREN			   	"("
%token R_PAREN			   	")"
%token L_SQUARE_BRACKET 	"["
%token R_SQUARE_BRACKET 	"]"
%token L_CURLY_BRACKET 		"{"
%token R_CURLY_BRACKET 		"}"
%token TAB                 "  "
%token SPACE               " "
%token ERROR
// Variable identification
%token <union_string> ID 	"identifier"

// Production types
%type <union_string> libraries_list
%type <union_string> variable_type
%type <union_string> basic_type
%type <union_string> constant
%type <union_bool> optional_array
%type <union_arglist> parameter_list_or_empty 
%type <union_arglist> parameter_list
%type <union_string> parameter
%type <union_arglist> argument_list_or_empty
%type <union_arglist> argument_list
%type <union_string> argument

%%

program:
	import_list declaration_list function_list
	;
	
import_list:
	import_list import_statement
	| empty
	;

import_statement:
	USE libraries_list {
      if ($2 != nullptr){
		   cout << "Using libraries: " << *$2 << endl;
         delete $2;
      }
	}
	;

libraries_list:
	STRING_CONST COMMA libraries_list {
      $$ = new string("");
      *$$ += *$1;
      *$$ += ", ";
      *$$ += *$3;
      delete $1;
      delete $3;
	}
	| STRING_CONST {
     $$ = new string("");
     *$$ = *$1;
     delete $1;
	}
	;

declaration_list:
	declaration_list variable_declaration_list_or_empty
   | declaration_list class_declaration_list_or_empty
	| empty
	;

variable_declaration_list_or_empty:
   variable_declaration variable_declaration_list_or_empty
   | empty
   ;

variable_declaration:
	VAR ID COLON variable_type ASSIGN constant SEMICOLON {
		cout << "Variable " << *$2 << " with type " << *$4 << " set to " << *$6 << endl;
      delete $2;
      delete $4;
      delete $6;
	}
   ;

class_declaration_list_or_empty:
   class_declaration class_declaration_list_or_empty
   | empty
   ;

class_declaration:
   CLASS ID L_CURLY_BRACKET public_or_private_block_list_or_empty class_function_list R_CURLY_BRACKET
   ;

public_or_private_block_list_or_empty:
   public_or_private_block public_or_private_block_list_or_empty
   | empty
   ;

public_or_private_block:
   PUBLIC L_CURLY_BRACKET variable_declaration_list_or_empty {

   }
   PRIVATE L_CURLY_BRACKET variable_declaration_list_or_empty {

   }
   ;

variable_assignment:
	ID ASSIGN constant SEMICOLON {
      // TODO this needs to ensure that $1 exists before assigning it.
		cout << "Variable " << *$1 << " set to " << *$3 << endl;
      delete $1;
      delete $3;
	}
	;

function_list:
	function_list function_definition
	| empty
	;

// Function definitions can consist of none, one, or many arguments, and can have a declared return type (default void)
function_definition:
	FUNCTION ID L_PAREN parameter_list_or_empty R_PAREN L_CURLY_BRACKET expression_list R_CURLY_BRACKET {
		cout << "Function " << *$2 << " returns void" << endl;
      delete $2;
      if ($4->args.size() > 0){
         cout << "Parameter(s):" << endl;
         for (auto arg : $4->args) {
            cout << arg << endl;
         }
      }
      delete $4;
      cout << endl;
	}
	| FUNCTION ID L_PAREN parameter_list_or_empty R_PAREN COLON variable_type L_CURLY_BRACKET expression_list R_CURLY_BRACKET {
		cout << "Function " << *$2 << " returns " << *$7 << ":" << endl;
      delete $2;
      delete $7;
		cout << "Parameter(s):" << endl;
      if ($4->args.size() > 0){
         for (auto arg : $4->args) {
            cout << arg << endl;
         }
      }
      delete $4;
      cout << endl;
	}
	;

class_function_list:
   class_function function_definition
   | empty
   ;

class_function:
   optional_public_or_private FUNCTION ID L_PAREN parameter_list_or_empty R_PAREN L_CURLY_BRACKET expression_list R_CURLY_BRACKET {

   }
   optional_public_or_private FUNCTION ID L_PAREN parameter_list_or_empty R_PAREN COLON variable_type L_CURLY_BRACKET expression_list R_CURLY_BRACKET {

   }
   ;

optional_public_or_private:
   PUBLIC
   | PRIVATE
   | empty
   ;

expression_list:
	expression_list expression
	| empty
	;

expression:
   variable_declaration
   | variable_assignment
   | function_call
	| empty
	;

function_call:
   ID L_PAREN argument_list_or_empty R_PAREN SEMICOLON {
      cout << "Function \"" << *$1 << "\" called";
      delete $1;
      if ($3->args.size() > 0){
         cout << " with " << $3->args.size() << " argument(s):" << endl;
         for (auto arg : $3->args){
            cout << arg << endl;
         }
         delete $3;
         cout << endl;
      }
      else cout << ".";
   }
   ;

argument_list_or_empty:
   argument_list
   | empty {
      $$ = new Arg_list();
   }
   ;

argument_list:
   argument COMMA argument_list {
      $$ = new Arg_list();
      $$->args.push_back(*$1);
      delete $1;
      $$->args.insert($$->args.end(), $3->args.begin(), $3->args.end());
      delete $3;
   }
   | argument {
      $$ = new Arg_list();
      $$->args.push_back(*$1);
      delete $1;
   }
   ;

argument:
   constant {
      $$ = $1;
   }
   ;

parameter_list_or_empty:
   parameter_list
   | empty {
      $$ = new Arg_list();
   }
   ;

parameter_list:
	parameter COMMA parameter_list {
      $$ = new Arg_list();
      $$->args.push_back(*$1);
      delete $1;
      $$->args.insert($$->args.end(), $3->args.begin(), $3->args.end());
      delete $3;
   }
	| parameter {
      $$ = new Arg_list();
      $$->args.push_back(*$1);
      delete $1;
   }
   ;

parameter:
	ID COLON variable_type {
      $$ = new string("");
      string str = " " + *$1 + ": " + *$3;
      *$$ += str;
      delete $1;
      delete $3;
	}
	;

constant:
	BOOL_CONST 		{ $$ = ($1 == true) ? new std::string("true") : new std::string("false"); }
	| INT_CONST 	{ $$ = new std::string(std::to_string($1)); }
	| CHAR_CONST 	{ $$ = new std::string($1); }
	| DOUBLE_CONST 	{ $$ = new std::string(std::to_string($1)); }
	| STRING_CONST 	{ $$ = new std::string(*$1); delete $1; }
   | ID           { $$ = new std::string(*$1); delete $1; }
	;

variable_type:
   basic_type optional_array {
      $$ = new string("");
      if ($2 == true){
         *$1 += " array";
      }
      *$$ = *$1; 
      delete $1;
   }
   ;

basic_type:
	BOOL { $$ = new std::string("bool"); }
	| INT { $$ = new std::string("int"); }
	| CHAR { $$ = new std::string("char"); }
	| DOUBLE { $$ = new std::string("double"); }
	| STRING { $$ = new std::string("string"); }
   | VOID { $$ = new std::string("void"); }
	;

optional_array:
   L_SQUARE_BRACKET R_SQUARE_BRACKET {
      $$ = true;
   }
   | empty {
      $$ = false;
   }
   ;

empty:
	;

%%
void yyerror(const char* error){
	fprintf(stderr, "%s on line %d\n", error, line_count);
}

void help_text(){
	cout << "Usage: prse_c [OPTIONS] [FILE]" << endl;
	cout << "OPTIONS: " << endl;
	string options_list[] = {
		"	-cpp	Output directly to C++ instead of binary file",
		"	-o		Name of output file",
      "  -g    Include debugging symbols in binary, for use with gdb"
	};
	for (int i = 0; i < (sizeof(options_list)/sizeof(options_list[0])); i++){
		cout << options_list[i] << endl;	
	}
}

int main(int argc, char** argv){
	// Raise an error if the user only types 'prse_c'
	if ( !(argc > 1) ){
		help_text();
		return 1;
	}
	FILE* in_file = fopen(argv[1], "r");
	if (!in_file){
		cerr << "Could not open file \"" << argv[1] << "\"" << endl;
		return 1;
	}
	extern FILE* yyin;
	yyin = in_file;
	yyparse();
}
