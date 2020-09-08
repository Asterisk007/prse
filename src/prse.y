%{
	#include <stdio.h>
    #include <fstream>
	#include <iostream>
	#include <string>
    #include <vector>
    #include <map>
    //#include <format> //TODO: format is coming in C++2020. Wait for clang to support it. https://wg21.link/P0645R10
    #include "parser.h"

	using namespace std;

	extern int yylex(void);
	extern void yyerror(const char*);
	extern int line_count;
    
    extern bool verbose;
%}

%union {
	bool                  union_bool;
	int                   union_int;
	char*                 union_char;
	double                union_double;
	std::string*          union_string;
    List*                 union_list;
    //Symbol*               union_symbol;
}

%destructor { delete $$; } <union_string>
%destructor { delete $$; } <union_char>

%define parse.error verbose

// Tokens
// Data constants
%token <union_bool>		    BOOL_CONST		"boolean constant"
%token <union_int> 		    INT_CONST		"int constant"
%token <union_double> 	    DOUBLE_CONST	"double constant"
%token <union_char> 	    CHAR_CONST		"char constant"
%token <union_string>   	STRING_CONST	"string constant"
// Variable types
%token BOOL				   	"boolean"
%token INT				   	"int"
%token DOUBLE			   	"double"
%token CHAR				   	"char"
%token STRING			   	"string"
%token VOID				   	"void"
// Key tokens
%token USE 				   	"use"
%token LET 				   	"let"
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
%token PLUS_ASSIGN 	     	"+="
%token MINUS_ASSIGN 	   	"-="
// Logic
%token LOGIC_TRUE		   	"true"
%token LOGIC_FALSE	    	"false"
%token LOGIC_EQ		    	"=="
%token LOGIC_NE		    	"!="
%token LOGIC_NOT		   	"!"
%token LOGIC_AND		   	"&&"
%token LOGIC_OR		   	    "||"
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
%token ERROR
// Variable identification
%token <union_string> ID 	"identifier"

// Production types
%type <union_list> libraries_list
%type <union_list> variable_declaration_list
%type <union_list> class_declaration_list
%type <union_string> variable_declaration
%type <union_string> variable_assignment
%type <union_string> variable_type
%type <union_string> basic_type
%type <union_string> constant
%type <union_bool> optional_array
%type <union_list> expression_list
%type <union_string> expression
%type <union_list> function_list
%type <union_list> function_definition
%type <union_string> function_header
%type <union_list> parameter_list_or_empty
%type <union_list> parameter_list
%type <union_string> parameter
%type <union_list> argument_list_or_empty
%type <union_list> argument_list
%type <union_string> argument
%type <union_string> function_call

%left LET FUNCTION CLASS
%%

program:
	import_list declaration_list function_list_or_empty
	;
	
import_list:
	import_list import_statement
	| empty
	;

import_statement:
	USE libraries_list {
        OutputBuffer& output = OutputBuffer::instance();
        Library& lib = Library::instance();
        // Iterate over a given list of libraries
        List ll = *$2;
        for (auto s : ll.list) {
            cout << s << endl;
            // Check if provided library is available
            if (lib.lib_exists(s)) {
                // If so, add it to the output buffer

                // I have to do this nonsense because format() isn't a thing in clang yet,
                // and I don't much feel like implementing it myself.
                string t = "#include <"; t += lib.get_lib(s); t += ">";
                output.add_line(t);
            } else {
                // Otherwise, ignore it
                // IMPLEMENT! error for unknown library
                cerr << "Line " << line_count << ": unknown library \"" << s << "\"" << endl;
            }
        }
        delete $2;
	}
	;

libraries_list:
	STRING_CONST COMMA libraries_list {
        List* ll = new List;
        ll->push_back(string(*$1));
        delete $1;
        List* ll2 = $3;
        ll->append_list(ll2);
        delete $3;
        $$ = ll;
	}
	| STRING_CONST {
        List* ll = new List;
        ll->push_back(string(*$1));
        delete $1;
        $$ = ll;
	}
	;
    
expression_list:
	expression expression_list {
        List* l = new List;
        l->push_back(*$1);
        l->append_list($2);
        delete $1; delete $2;
        $$ = l;
    }
	| empty {
        $$ = nullptr;
    }
	;

expression:
    variable_declaration {
        $$ = $1;
    }
    | variable_assignment {
        $$ = $1;
    }
    | function_call {
        $$ = $1;
    }
	;

declaration_list:
    variable_declaration_list declaration_list {
        OutputBuffer& output = OutputBuffer::instance();
        List* l = $1;
        for (auto a : l->list){
            output.add_line(a);
        }
        delete $1;
    }
    | class_declaration_list declaration_list {
        OutputBuffer& output = OutputBuffer::instance();
        List* l = $1;
        for (auto a : l->list){
            output.add_line(a);
        }
        delete $1;
    }
    | empty
	;

variable_declaration_list_or_empty:
    variable_declaration_list
    | empty
    ;

variable_declaration_list:
    variable_declaration variable_declaration_list {
        List* l = new List;
        if ($1 != nullptr)
            l->push_back(*$1);
        l->append_list($2);
        delete $1; delete $2;
        $$ = l;
    }
    | variable_declaration {
        List* l = new List;
        if ($1 != nullptr)
            l->push_back(*$1);
        delete $1;
        $$ = l;
    }
    ;

variable_declaration:
	LET ID COLON variable_type ASSIGN constant SEMICOLON {        
        //     t =          TYPE                       ID          =        CONSTANT
        string *t = new string(*$4); *t += " "; *t += *$2; *t += " = "; *t += *$6; *t += ";\n";
        $$ = t;
        delete $2; delete $4; delete $6;
    }
    /*| LET ID ASSIGN constant SEMICOLON {
        delete $2; delete $4;
        $$ = nullptr;
        //IMPLEMENT duck typing
    }
    | LET ID COLON variable_type SEMICOLON {
        delete $2; delete $4;
        $$ = nullptr;
        cerr << "Derby error on line " << line_count << ". Variables must be initialized with some value." << endl;
        //IMPLEMENT! 'derby' error
    }*/
    ;

class_declaration_list:
    class_declaration_list class_declaration 
    | class_declaration {
        $$ = nullptr;
    }
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

variable_assignment:
	ID ASSIGN constant SEMICOLON {
        //     t =           ID              =        CONSTANT
        string *t = new string(*$1); *t += " = "; *t += *$3; *t += ";\n";
        $$ = t;
        delete $1; delete $3;
	}
	;

function_list_or_empty:
    function_list {
        OutputBuffer& output = OutputBuffer::instance();
        for (auto a : $1->list) {
            output.add_line(a);
        }
    }
    | empty
    ;

function_list:
	function_definition function_list {
        $2->append_list($1);
        $$ = $2;
    }
    | function_definition {
        $$ = $1;
    }
	;

// Function definitions can consist of none, one, or many arguments, and can have a declared return type (default void)
function_definition:
    function_header expression_list R_CURLY_BRACKET {
        List* l = new List;
        l->push_back(*$1);
        l->append_list($2);
        l->push_back("}\n\n");
        delete $1; delete $2;
        $$ = l;
    }

function_header:
	FUNCTION ID L_PAREN parameter_list_or_empty R_PAREN L_CURLY_BRACKET {
        string* t = new string("");
        if (*$2 == "main"){
            *t += "int";
        } else {
            *t += "void";
        }
        *t += "main (";
        if ($4 != nullptr) {
            for (auto a : $4->list) {
                *t += a;
                *t += ", ";
            }
        }
        *t += ") {\n";
        delete $2; delete $4;
        $$ = t;
	}
	| FUNCTION ID L_PAREN parameter_list_or_empty R_PAREN COLON variable_type L_CURLY_BRACKET {
        string* t = new string("");
        if (*$2 == "main"){
            *t += "int main (";
        } else {
            *t += *$7;
            *t += *$2;
        }
        if ($4 != nullptr) {
            for (auto a : $4->list) {
                *t += a;
                *t += ", ";
            }
        }
        *t += ") {\n";
        delete $2; delete $4; delete $7;
        $$ = t;
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
        delete $3; delete $8;
    }
    ;

optional_public_or_private:
    PUBLIC
    | PRIVATE
    | empty
    ;


function_call:
    ID L_PAREN argument_list_or_empty R_PAREN SEMICOLON {
        delete $1;
        if ($3 != nullptr) {
            delete $3;
        }
        $$ = new string("// TODO: ADD FUNCTION CALL BEHAVIOR (function_call in prse.y)");
    }
    ;

argument_list_or_empty:
    argument_list { $$ = nullptr; }
    | empty { $$ = nullptr; }
    ;

argument_list:
    argument COMMA argument_list {
        delete $1;
        $$ = nullptr;
    }
    | argument {
        delete $1;
        $$ = nullptr;
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

   }
   ;

parameter_list:
	parameter COMMA parameter_list {
       List* l = new List;
       l->push_back(*$1);
       l->append_list($3);
        $$ = l;
    }
	| parameter {
       List* list = new List;
       list->push_back(*$1);
       $$ = list;
    }
    ;

parameter:
	ID COLON variable_type {
        string* p = new string(*$3);
        *p += *$1;
        $$ = p;
	}
	;

constant:
    BOOL_CONST        { 
        $$ = ($1 == true) ? new string("true") : new string("false");
    }
    | INT_CONST       {
        $$ = new string(std::to_string($1));
    }
    | CHAR_CONST      { $$ = new string(to_string(*$1)); delete $1; }
    | DOUBLE_CONST 	  { $$ = new string(to_string($1)); }
    | STRING_CONST 	  { $$ = new string(*$1); delete $1; }
    | ID              { $$ = new string(*$1); delete $1; }
    ;

variable_type:
    basic_type {
        $$ = $1;
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
