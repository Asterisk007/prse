%{
	#include <stdio.h>
	#include <iostream>
	#include <string>
   #include <vector>
   #include <map>
   #include "parser.h"

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
      $$ = new string(*$1);
      *$$ += ", ";
      *$$ += *$3;
      delete $1;
      delete $3;
	}
	| STRING_CONST {
     $$ = new string(*$1);
     delete $1;
	}
	;

declaration_list:
	declaration_list variable_declaration_list
   | declaration_list class_declaration_list
   | empty
	;

variable_declaration_list:
   variable_declaration variable_declaration_list
   | variable_declaration
   ;

variable_declaration:
	VAR ID COLON variable_type ASSIGN constant SEMICOLON {
		cout << "Variable " << *$2 << " with type " << *$4 << " set to " << *$6 << endl;
      delete $2; delete $4; delete $6;
	}
   ;

class_declaration_list:
   class_declaration class_declaration_list
   | class_declaration
   ;

class_declaration:
   CLASS ID L_CURLY_BRACKET public_or_private_block_list_or_empty class_function_list_or_empty R_CURLY_BRACKET {
      delete $2;
   }
   ;

public_or_private_block_list_or_empty:
   public_or_private_block public_or_private_block_list_or_empty
   | empty
   ;

public_or_private_block:
   PUBLIC L_CURLY_BRACKET variable_declaration_list_or_empty {
      
   }
   | PRIVATE L_CURLY_BRACKET variable_declaration_list_or_empty {

   }
   ;

variable_declaration_list_or_empty:
   variable_declaration_list
   | empty
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

class_function_list_or_empty:
   class_function_list_or_empty class_function
   | empty
   ;

class_function:
   optional_public_or_private FUNCTION ID L_PAREN parameter_list_or_empty R_PAREN L_CURLY_BRACKET expression_list R_CURLY_BRACKET {
      delete $3;
   }
   | optional_public_or_private FUNCTION ID L_PAREN parameter_list_or_empty R_PAREN COLON variable_type L_CURLY_BRACKET expression_list R_CURLY_BRACKET {
      delete $3;
      delete $8;
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
	BOOL_CONST        { $$ = ($1 == true) ? new std::string("true") : new std::string("false"); }
	| INT_CONST       { $$ = new std::string(std::to_string($1)); }
	| CHAR_CONST      { $$ = new std::string($1); }
	| DOUBLE_CONST 	{ $$ = new std::string(std::to_string($1)); }
	| STRING_CONST 	{ $$ = new std::string(*$1); delete $1; }
   | ID              { $$ = new std::string(*$1); delete $1; }
	;

variable_type:
   basic_type optional_array {
      $$ = new string(*$1);
      delete $1;
      if ($2 == true){
         *$$ += " array";
      }
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
	cout << "Usage: prse_c [OPTIONS] [FILE(S)]" << endl;
	cout << "OPTIONS: " << endl;
	string options_list[] = {
		"  -cpp                    Output directly to C++ instead of binary file",
      "  -g                      Include debugging symbols in binary, for use with gdb"
      "\n  --help                  Display this help text",
		"  -o, --output=[FILE]     Specify an output file name (default a.out)",
      "  -v, --verbose           Verbose mode"
	};
	for (int i = 0; i < (sizeof(options_list)/sizeof(options_list[0])); i++){
		cout << options_list[i] << endl;	
	}
}

int main(int argc, char** argv){
   std::string output_filename = "";
   // Create a library of available bash/command line arguments
   std::map<std::string, bool> activated_args;
   activated_args["-cpp"] = false;
   activated_args["-g"] = false;
   activated_args["--help"] = false;
   activated_args["-o"] = false;
   activated_args["-v"] = false;
   activated_args["--verbose"] = false;

   // A vector of input files, which we will parse once we process
   // each argument passed to the program
   std::vector<std::string> input_files = std::vector<std::string>(); 
   // Process each argument passed
   for (int i = 1; i < argc; i++){
      std::string file_candidate = std::string(argv[i]);
      std::string filetype = ".prse";
      // If string is a file, check that we can open it.
      // '!' is necessary to reverse the logical FALSE return value, which means it matches.
      if (file_candidate.length() > filetype.length() && !file_candidate.compare(file_candidate.length() - filetype.length(), filetype.length(), filetype)){
         FILE* file = fopen(file_candidate.c_str(), "r");
         if (!file){
            cerr << "Could not open file \"" << file_candidate << "\"" << endl;
            return 1;
         } else {
         // Add this file to the list of files
            fclose(file);
            input_files.push_back(file_candidate);
         }
      } else {
      // Otherwise, if this isn't a file, check that it exists in our library of aviailable arguments
         std::string arg = std::string(argv[i]);
         if (activated_args.find(arg) != activated_args.end()){
            activated_args[arg] = true;
            // Get output filename
            if (arg == "-o" && i+1 < argc) {
               output_filename = std::string(argv[i+1]);
               i = i+2;
            } else if (arg == "-o" && i+1 >= argc) {
               cout << "Please specify a filename" << endl;
               return 1;
            }
         } else {
         // Finally, if this is not a valid input file nor a recognized argument,
         // output an error and continue.
            cout << "Unrecognized argument '" << arg << "'" << endl;
         }
      }
   }
   // If --help was passed as an argument, print out the help text
   // and exit.
   if (activated_args["--help"]) {
      help_text();
      return 0;
   }
	extern FILE* yyin;
   // Check that a file has been input
   if ((int)input_files.size() > 0){
      // Go through each input file and parse it
      for (int i = 0; i < (int)input_files.size(); i++){
         cout << "Processing file " << input_files[i] << endl;
         yyin = fopen(input_files[i].c_str(), "r");
         // reset the Flex buffer for the next file.
         //yyrestart(yyin);
         yyparse();
      }
      if (output_filename != ""){
         cout << "Final output binary name: " << output_filename << endl;
      }
   // Otherwise, return an error.
   } else {
      cout << "Please enter at least one input file." << endl;
   }
   return 0;
}
