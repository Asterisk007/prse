%{
    #include <stdio.h>
    #include <fstream>
    #include <iostream>
    #include <sstream>
    #include <string>
    #include <vector>
    #include <map>
    #include <regex>
    #include <stdexcept>
    //#include <format> //TODO: format is coming in C++2020. Wait for clang to support it. https://wg21.link/P0645R10
    #include "parser.h"

    using namespace std;

    extern int yylex(void);
    extern void yyerror(const char*);
    extern int line_count;
%}

%union {
	bool                         union_bool;
	int                          union_int;
	double                       union_double;
	std::string*                 union_string;
    vector<string>*              union_string_list;
    PRSE_type                    union_prse_type;
    //vector<Constant>*     union_constant_list;
    const Symbol*                union_symbol;
    const Expression*            union_expression;
    vector<const Expression*>*   union_expression_list;
    vector<const Symbol*>*       union_symbol_list;
};

%destructor { delete $$; } <union_string>
%destructor { delete $$; } <union_string_list>
//%destructor { delete $$; } <union_constant_list>
//%destructor { delete $$; } <union_symbol>
%destructor { delete $$; } <union_expression>
%destructor { delete $$; } <union_expression_list>

%define parse.error verbose

// Tokens
// Data constants
%token <union_int> 	        INT_CONST		"int constant"
%token <union_double>       DOUBLE_CONST	"double constant"
%token <union_string> 	    CHAR_CONST		"char constant"
%token <union_string>   	STRING_CONST	"string constant"
%token <union_string>       I_STRING_BEGIN  "start of interpolated string"
%token <union_string>       I_STRING_PART   "part of interpolated string"
%token <union_string>       I_STRING_END    "end of interpolated string"
// Variable types
%token BOOL				   	"boolean"
%token INT				   	"int"
%token DOUBLE			   	"double"
%token CHAR				   	"char"
%token STRING			   	"string"
%token VOID				   	"void"
%token MYNULL               "null"
// Key tokens
%token USE 				   	"use"
%token LET 				   	"let"
%token FUNCTION 		   	"function"
%token RETURN 			   	"return"
%token CLASS			   	"class"
%token PUBLIC			   	"public"
%token PRIVATE			   	"private"
// Flow control
%token IF 				   	"if"
%token ELSE 			   	"else"
%token FOR				   	"for"
%token WHILE                "while"
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
%token STAR_ASSIGN 	     	"*="
%token SLASH_ASSIGN 	   	"/="
// Logic
%token <union_bool> LOGIC_TRUE		    "true"
%token <union_bool> LOGIC_FALSE	    	"false"
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
%token DOUBLE_QUOTE 	   	'"'
%token SINGLE_QUOTE 	   	"'"
%token L_PAREN			   	"("
%token R_PAREN			   	")"
%token L_SQUARE_BRACKET 	"["
%token R_SQUARE_BRACKET 	"]"
%token L_CURLY_BRACKET 		"{"
%token R_CURLY_BRACKET 		"}"
%token DOLLAR               "$"
%token IN                   "in"
// Variable identification
%token <union_string> ID 	"identifier"

// Production types
%type <union_string_list>       import_list
%type <union_string_list>       final_definition_list
%type <union_expression_list>   definition_list

%type <union_string>            import_statement
%type <union_string_list>       libraries_list
%type <union_expression>        definition
%type <union_expression>        variable_definition
%type <union_expression>        variable_assignment
%type <union_prse_type>         variable_type
%type <union_prse_type>         basic_type
%type <union_expression>        constant

%type <union_expression>        variable
%type <union_expression>        array
//%type <union_string_list> class_definition
//%type <union_symbol> variable
%type <union_expression_list>   code_block
%type <union_expression>        code_line
%type <union_expression>        scope_block
%type <union_expression>        expression
%type <union_expression>        function_definition
%type <union_expression>        function_header
%type <union_prse_type>         function_return_type_or_empty
%type <union_symbol_list>       parameter_list_or_empty
%type <union_symbol_list>       parameter_list
%type <union_symbol>            parameter
%type <union_expression_list>   argument_list
//%type <union_expression> argument
%type <union_expression>        function_call
%type <union_expression>        return_statement
%type <union_expression>        primary_expression
%type <union_expression>        flow_control_block
%type <union_expression>        for_loop
%type <union_expression>        while_loop
%type <union_expression>        for_header
%type <union_expression>        while_header
%type <union_expression>        if_block
%type <union_expression>        if_chain
%type <union_expression>        if_header
%type <union_expression>        else_if_header
%type <union_expression>        optional_else_or_else_if
//%type <union_expression_list> interpolated_string
//%type <union_expression_list> i_string_segment_list
//%type <union_expression_list> i_string_middle

%token ERROR "error"

%left LOGIC_OR
%left LOGIC_AND
%left LOGIC_EQ LOGIC_NE DOLLAR
%left LOGIC_GREATER LOGIC_GREATER_EQUAL LOGIC_LESS LOGIC_LESS_EQUAL
%left I_STRING_PART PLUS MINUS STAR SLASH STAR_ASSIGN SLASH_ASSIGN
%%

program:
	import_list final_definition_list {
        OutputBuffer& output = OutputBuffer::instance();
        // Output import list
        bool need_space = false;
        if ($1 != nullptr){
            need_space = true;
            for (auto a : *$1){
                output.add_line(a);
            }
        }
        // Also output any additional libraries needed from their use in the program
        Library& li = Library::instance();
        for (auto it = li.lib_required.begin(); it != li.lib_required.end(); it++){
            if (it->second == true){
                string t = "#include <"; t += it->first; t += ">\n";
                output.add_line(t);
                need_space = true;
            }
        }

        // Add some whitespace for nice formatting, assuming libraries were used
        if (need_space)
            output.add_line("\n");

        // Loop through all defined function headers and
        // output their C++ equivalent
        auto def = Function_definition::definitions;
        vector<string> def_strings;

        // function definition format:
        // <type> <id> (param, param)
        for (auto it = def.begin(); it != def.end(); it++){
            Function_definition* fd = *it;
            // Only make function prototypes for non-main functions
            if (fd->get_id() != "main"){
                string t = prse_type_to_string(fd->get_type());
                t += " ";
                t += fd->get_id();
                auto p = fd->get_parameters();
                t += "(";
                for (int i = 0; i < (int)p.size(); i++){
                    t += prse_type_to_string(p[i]);
                    if (i < (int)p.size()-1)
                        t += ", ";
                }
                t += ");\n";
                def_strings.push_back(t);
            }
        }
        for (int i = 0; i < (int)def_strings.size(); i++)
            output.add_line(def_strings[i]);

        // Output function/class/global variable definitions
        if ($2 != nullptr){
            for (auto a : *$2){
                output.add_line(a);
            }
        }

        delete $1; delete $2;
    }
    /*| error { yyerrorok; }*/
	;

import_list:
	import_list import_statement {
        vector<string>* l = new vector<string>(0);
        append_list(l, $1);
        l->push_back(*$2);
        delete $1; delete $2;
        $$ = l;
    }
	| empty { $$ = nullptr; }
    /*| error { yyerrorok; }*/
	;

import_statement:
	USE libraries_list {
        Library& lib = Library::instance();
        // Iterate over a given list of libraries
        vector<string>* l = $2;
        string* t = new string("");
        for (auto s : *l) {
            // Check if provided library is available
            if (lib.lib_exists(s)) {
                lib.lib_used[s] = true;
                // If so, add it to the output buffer

                // I have to do this nonsense because format() isn't a thing in clang yet,
                // and I don't much feel like implementing it myself.
                *t += "#include <"; *t += lib.get_lib(s); *t += ">\n";
            } else {
                // Otherwise, ignore it
                // IMPLEMENT! error for unknown library
                cerr << "Line " << line_count << ": unknown library \"" << s << "\"" << endl;
            }
        }
        delete $2;
        $$ = t;
	}
    /*| error { yyerrorok; }*/
	;

libraries_list:
	STRING_CONST COMMA libraries_list {
        vector<string>* ll = new vector<string>(0);
        ll->push_back(*$1);
        delete $1;
        vector<string>* ll2 = $3;
        append_list(ll, ll2);
        delete $3;
        $$ = ll;
	}
	| STRING_CONST {
        vector<string>* ll = new vector<string>(0);
        ll->push_back(*$1);
        delete $1;
        $$ = ll;
	}
    /*| error { yyerrorok; }*/
	;

final_definition_list:
    definition_list {
        // Now that we have a list of defined functions/classes,
        // we want to process them accordingly. All functions and classes
        // are now defined and have been added to memory, so if any
        // functions are called, and a definition cannot be found, we know
        // for a *fact* that such a function does not exist in this scope
        // (unless we find it in another file, but that's on the programmer
        // to include the file)
        if ($1 != nullptr){
            vector<string>* output = new vector<string>;
            vector<const Expression*> el = *$1;
            for (int i = 0; i < (int)el.size(); i++){
                if (el[i] != nullptr){
                    const Constant* el_i_as_const = el[i]->as_const();
                    output->push_back(el_i_as_const->value());
                    delete el[i];
                }
            }
            delete $1;
            $$ = output;
        } else {
            $$ = nullptr;
        }
    }
    ;

definition_list:
    definition_list definition {
        vector<const Expression*>* l = new vector<const Expression*>;
        append_list(l, $1);
        if ($2 != nullptr)
            l->push_back($2);
        delete $1;
        $$ = l;
    }
    | definition_list function_definition {
        vector<const Expression*>* l = new vector<const Expression*>;
        append_list(l, $1);
        if ($2 != nullptr)
            l->push_back($2);
        delete $1;
        $$ = l;
    }
    | definition_list flow_control_block {
        vector<const Expression*>* l = new vector<const Expression*>;
        append_list(l, $1);
        if ($2 != nullptr)
            l->push_back($2);
        delete $1;
        $$ = l;
    }
    /* | definition_list class_definition {
        vector<string>* l = new vector<string>;
        append_list(l, $1);
        append_list(l, $2);
        delete $1; delete $2;
        $$ = l;
    } */
    | empty { $$ = nullptr; }
    /*| error { yyerrorok; }*/
	;

definition:
    variable_definition SEMICOLON {
        $$ = $1;
    }
    | variable_assignment SEMICOLON {
        $$ = new Singleton($1);
    };
    /*| object_definition { $$ = $1; } */
    /*| error { yyerrorok; }*/
    ;

variable_definition:
	LET ID COLON variable_type ASSIGN expression {
        // First check that this variable has not already been defined elsewhere in the program
        if ($4 != PRSE_type::T_VOID){
            $$ = new Variable_definition(line_count, *$2, $4, $6, 0);
        } else {
            Error::error(Error::INVALID_TYPE_FOR_VARIABLE, prse_type_to_string($4));
            $$ = nullptr;
        }
        delete $2;
    }
    | LET ID COLON variable_type L_SQUARE_BRACKET R_SQUARE_BRACKET ASSIGN L_CURLY_BRACKET argument_list R_CURLY_BRACKET {
        // Array with specified type        
        if ($4 != PRSE_type::T_VOID){
            Library& li = Library::instance();
            li.lib_required["vector"] = true;
            int size = 1;
            $$ = new Variable_definition(line_count, *$2, $4, *$9, size);
        } else {
            Error::error(Error::INVALID_TYPE_FOR_VARIABLE, prse_type_to_string($4));
            $$ = nullptr;
        }
        delete $2; delete $9;
    }
    | LET ID COLON variable_type {
        $$ = nullptr;
        Error::error(Error::VARIABLE_DECLARED_BUT_NOT_SET, *$2);
        delete $2;
    }
    /*| error { yyerrorok; }*/
    ;

flow_control_block:
    if_block {
        $$ = $1;
    }
    | for_loop {
        $$ = $1;
    }
    | while_loop {
        $$ = $1;
    }
    ;

if_block:
    if_chain { $$ = $1; }
    /*| error { yyerrorok; }*/
    ;

if_chain:
    if_header L_CURLY_BRACKET code_block R_CURLY_BRACKET optional_else_or_else_if {
        $$ = new If_block(line_count, $1, *$3, $5);
        delete $3;
    }
    | if_header code_line optional_else_or_else_if {
        vector<const Expression*> line = vector<const Expression*>();
        line.push_back($2);
        $$ = new If_block(line_count, $1, line, $3);
    }
    /*| else_if_header scope_block optional_else_or_else_if {
        // Doing this to explicitly tell the programmer that
        // they must first use an if block/statement
        // before starting an else or else if.
        Error::error(Error::UNEXPECTED_ELSE_ELSE_IF);
        $$ = nullptr;
    }*/
    /*| error { yyerrorok; }*/
    ;

optional_else_or_else_if:
    else_if_header L_CURLY_BRACKET code_block R_CURLY_BRACKET optional_else_or_else_if {
        $$ = new Else_if_block(line_count, $1, *$3, $5);
        delete $3;
    }
    | else_if_header code_line optional_else_or_else_if {
        vector<const Expression*> line = vector<const Expression*>();
        line.push_back($2);
        $$ = new Else_if_block(line_count, $1, line, $3);
    }
    | ELSE L_CURLY_BRACKET code_block R_CURLY_BRACKET {
        $$ = new Else_block(line_count, *$3);
    }
    | ELSE code_line {
        vector<const Expression*> line = vector<const Expression*>();
        line.push_back($2);
        $$ = new Else_block(line_count, line);
    }
    | empty { $$ = nullptr; }
    /*| error { yyerrorok; }*/
    ;

if_header:
    IF L_PAREN expression R_PAREN {
        $$ = $3;
    }
    /*| error { yyerrorok; }*/
    ;

else_if_header:
    ELSE IF L_PAREN expression R_PAREN {
        $$ = $4;
    }
    /*| error { yyerrorok; }*/
    ;

for_loop:
    for_header L_CURLY_BRACKET code_block R_CURLY_BRACKET {
        $$ = new For_loop(line_count, $1, *$3);
        delete $3;
    }
    ;

for_header:
    FOR L_PAREN ID COLON basic_type ASSIGN expression SEMICOLON expression SEMICOLON variable_assignment R_PAREN {
        if ($7 == nullptr || $9 == nullptr || $11 == nullptr){
            $$ = nullptr;
            delete $3;
            delete $7;
            delete $9;
            delete $11;
            break;
        }

        $$ = new For_header(line_count, make_shared<Symbol>(*$3, $5), $7, $9, $11);
        delete $3;
    }
    | FOR L_PAREN ID IN array R_PAREN {
        if ($5 == nullptr){
            // Array was not declared. Error will be thrown. Return nothing.
            $$ = nullptr;
            delete $3;
            break;
        }
        $$ = new For_header(line_count, *$3, $5);
        delete $3;
    }
    ;

while_loop:
    while_header L_CURLY_BRACKET code_block R_CURLY_BRACKET {
        $$ = new While_loop(line_count, $1, *$3);
    }
    ;

while_header:
    WHILE L_PAREN expression R_PAREN {
        $$ = $3;
    }
    ;

/*class_definition:
    CLASS ID L_CURLY_BRACKET public_private_block_list R_CURLY_BRACKET {
        delete $2;
    }
    ;

public_private_block_list:
    public_private_block public_private_block_list
    | empty {  }
    ;

public_private_block:
    PUBLIC L_CURLY_BRACKET variable_definition_list_or_empty {

    }
    | PRIVATE L_CURLY_BRACKET variable_definition_list_or_empty {

    }
    ;*/

scope_block:
    L_CURLY_BRACKET code_block R_CURLY_BRACKET {
        $$ = new Scope_block(line_count, *$2);
        delete $2;
    }
    /*| error { yyerrorok; }*/
    ;

variable_assignment:
    variable ASSIGN expression {
        if ($1 == nullptr){
            delete $3;
            $$ = nullptr;
            break;
        }
        $$ = new Variable_assignment(line_count, $1, $3);
    }
    | variable INCREMENT {
        if ($1 == nullptr){
            $$ = nullptr;
            break;
        }
        $$ = new Increment(line_count, $1);
    }
    | variable DECREMENT {
        if ($1 == nullptr){
            $$ = nullptr;
            break;
        }
        $$ = new Decrement(line_count, $1);
    }
    | variable PLUS_ASSIGN expression {
        if ($1 == nullptr){
            $$ = nullptr;
            break;
        }
        $$ = new Plus_assign(line_count, $1, $3);
    }
    | variable MINUS_ASSIGN expression {
        if ($1 == nullptr){
            $$ = nullptr;
            break;
        }
        $$ = new Minus_assign(line_count, $1, $3);
    }
    | variable STAR_ASSIGN expression {
        if ($1 == nullptr){
            $$ = nullptr;
            break;
        }
        $$ = new Multiply_assign(line_count, $1, $3);
    }
    | variable SLASH_ASSIGN expression {
        if ($1 == nullptr){
            $$ = nullptr;
            break;
        }
        $$ = new Divide_assign(line_count, $1, $3);
    }
    /*| error { yyerrorok; }*/
    ;

// Function definitions can consist of zero or many parameters, and can have a declared return type (default void)
function_definition:
    function_header L_CURLY_BRACKET code_block R_CURLY_BRACKET {
        $$ = new Function_definition_expr($1, *$3);
        delete $3;
    }
    /*| error { yyerrorok; }*/
    ;

function_header:
	FUNCTION ID L_PAREN parameter_list_or_empty R_PAREN function_return_type_or_empty {
        if (Function_definition::defined_functions.find(*$2) == Function_definition::defined_functions.end())
            Function_definition::defined_functions.insert(*$2);

        // Valid return types for main function are explicitly T_VOID and T_INT
        if (*$2 == "main"){
            if ( !($6 & (PRSE_type::T_INT | PRSE_type::T_VOID)) ){
                Error::error(Error::INVALID_RETURN_TYPE_FOR_MAIN);
            }
        }
        vector<PRSE_type> params;
        vector<shared_ptr<Symbol>> syms;
        if ($4 != nullptr) {
            // Main function is the only function that MUST have 0 parameters defined.
            // The plan is to get command args in a similar fashion to how Golang does it,
            // ergo: programmer has to call a function of some kind which puts all command
            // args into an array (PRSE array, C++ std::vector) for the user to then
            // iterate through.
            if (*$2 == "main"){
                if ((int)$4->size() > 0){
                    Error::error(Error::INCORRECT_NUMBER_OF_PARAMETERS_FOR_MAIN, to_string((int)$4->size()));
                }
            } else {
                for (int i = 0; i < (int)$4->size(); i++) {
                    auto a = (*$4)[i];
                    syms.push_back(make_shared<Symbol>(*a));
                    params.push_back(a->get_type());
                    delete a;
                }
            }
        }

        Function_declaration fdec = Function_declaration(line_count, *$2, params);
        // If this function has already been defined, check that this definition's signature (its parameters + id)
        // do not match that of any known function.

        // A function with the same identifier and return type but a different number of parameters
        // than a previously defined function is valid C++. Thus, it is also valid PRSE code.
        if (Function_definition::get_function_definition(fdec) != nullptr){
            string h = "";
            for (int i = 0; i < (int)params.size(); i++){
                h += prse_type_to_string(params[i]);
                if (i < (int)params.size()-1)
                    h += ", ";
            }
            Error::error(Error::FUNCTION_WITH_RETURN_TYPE_ALREADY_DEFINED, *$2, h);
        } else {
            //cerr << "Function " << *$2 << " has return type " << prse_type_to_string($6) << endl;
            Function_definition::definitions.push_back(new Function_definition(line_count, *$2, $6, params));
            $$ = new Function_header(line_count, *$2, $6, syms);
        }
        delete $2; delete $4;
    }
    /*| error { yyerrorok; }*/
    ;

function_return_type_or_empty:
    COLON basic_type { $$ = $2; }
    | empty { $$ = PRSE_type::T_VOID; }

code_block:
    code_block variable_definition SEMICOLON {
        vector<const Expression*>* l = new vector<const Expression*>;
        append_list(l, $1);
        l->push_back($2);
        delete $1;
        $$ = l;
    }
    | code_block variable_assignment SEMICOLON {
        vector<const Expression*>* l = new vector<const Expression*>;
        append_list(l, $1);
        l->push_back($2);
        delete $1;
        $$ = l;
    }
    | code_block expression SEMICOLON {
        vector<const Expression*>* l = new vector<const Expression*>;
        append_list(l, $1);
        l->push_back(new Singleton($2));
        delete $1;
        $$ = l;
    }
    | code_block flow_control_block {
        vector<const Expression*>* l = new vector<const Expression*>;
        append_list(l, $1);
        l->push_back($2);
        delete $1;
        $$ = l;
    }
    /*| code_block function_call SEMICOLON {
        vector<const Expression*>* l = new vector<const Expression*>;
        append_list(l, $1);
        l->push_back(new Singleton($2));
        delete $1;
        $$ = l;
    }*/
    | code_block return_statement SEMICOLON {
        vector<const Expression*>* l = new vector<const Expression*>;
        append_list(l, $1);
        l->push_back($2);
        delete $1;
        $$ = l;
    }
    | code_block scope_block {
        vector<const Expression*>* l = new vector<const Expression*>;
        append_list(l, $1);
        l->push_back($2);
        delete $1;
        $$ = l;
    }
	| empty {
        $$ = nullptr;
    }
    /*| error { yyerrorok; }*/
	;

code_line:
    variable_definition SEMICOLON {
        // Because code lines are single-line statements
        // found just after the header of an if-statement, we must warn the user
        // that this variable will immediately go out-of-scope as soon as they
        // declare it
        Warning::warning(Warning::VARIABLE_DECLARATION_USELESS);
        $$ = $1;
    }
    | variable_assignment SEMICOLON {
        $$ = $1;
    }
    | function_call SEMICOLON {
        $$ = new Singleton($1);
    }
    | return_statement SEMICOLON {
        $$ = $1;
    }
    /*| error { yyerrorok; }*/
    ;

expression:
    primary_expression { $$ = $1; }
    | function_call { $$ = $1; }
    | expression DOLLAR basic_type {
        $$ = new Cast(line_count, $1, $3);
    }
    | expression PLUS expression {
        $$ = new Plus($1, $3);
    }
    | expression MINUS expression {
        $$ = new Minus($1, $3);
    }
    | expression STAR expression {
        $$ = new Multiply($1, $3);
    }
    | expression SLASH expression {
        $$ = new Divide($1, $3);
    }
    | expression LOGIC_EQ expression {
        // Check that PRSE types can be compared
        $$ = new Logic_Eq($1, $3);
    }
    | expression LOGIC_NE expression {
        $$ = new Logic_NEq($1, $3);
    }
    | expression LOGIC_LESS expression {
        $$ = new Logic_Less_Than($1, $3);
    }
    | expression LOGIC_LESS_EQUAL expression {
        $$ = new Logic_Less_Eq($1, $3);
    }
    | expression LOGIC_GREATER expression {
        $$ = new Logic_Greater_Than($1, $3);
    }
    | expression LOGIC_GREATER_EQUAL expression {
        $$ = new Logic_Greater_Eq($1, $3);
    }
    | expression LOGIC_AND expression {
        $$ = new Logic_And($1, $3);
    }
    | expression LOGIC_OR expression {
        $$ = new Logic_Or($1, $3);
    }
    | empty { $$ = nullptr; }
    /*| error { yyerrorok; }*/
	;

/*class_function_list_or_empty:
    class_function_list_or_empty class_function
    | empty
    ;

class_function:
    optional_public_or_private FUNCTION ID L_PAREN parameter_list R_PAREN L_CURLY_BRACKET code_block R_CURLY_BRACKET {
        delete $3;
    }
    | optional_public_or_private FUNCTION ID L_PAREN parameter_list R_PAREN COLON variable_type L_CURLY_BRACKET code_block R_CURLY_BRACKET {
        delete $3; delete $8;
    }
    ;

optional_public_or_private:
    PUBLIC
    | PRIVATE
    | empty
    ;*/


function_call:
    ID L_PAREN argument_list R_PAREN {
        // Function calls are processed at the final_definition_list production,
        // since we will then have a definitive list of all defined functions in this program
        $$ = new Function_call(line_count, *$1, *$3);
        delete $1; delete $3;
    }
    /*| error { yyerrorok; }*/
    ;

return_statement:
    RETURN expression {
        $$ = new Return_statement(line_count, $2);
    }
    /*| error { yyerrorok; }*/
    ;

argument_list:
    expression COMMA argument_list {
        // Expression vector pointer for $$'s value.
        vector<const Expression*>* l = new vector<const Expression*>;
        if ($1 != nullptr)
            l->push_back($1);
        append_list(l, $3);
        $$ = l;
        //delete $1;
        delete $3;
    }
    | expression {
        vector<const Expression*>* l = new vector<const Expression*>;
        if ($1 != nullptr)
            l->push_back($1);
        $$ = l;
        // delete $1;
    }
    /*| error { yyerrorok; }*/
    ;

parameter_list_or_empty:
    parameter_list { $$ = $1; }
    | empty { $$ = nullptr; }
    /*| error { yyerrorok; }*/
    ;

parameter_list:
    parameter COMMA parameter_list {
        vector<const Symbol*>* l = new vector<const Symbol*>;
        l->push_back($1);
        append_list(l, $3);
        delete $3;
        $$ = l;
    }
    | parameter {
        vector<const Symbol*>* l = new vector<const Symbol*>;
        l->push_back($1);
        $$ = l;
    }
    /*| error { yyerrorok; }*/
    ;

parameter:
	ID COLON variable_type {
        $$ = new Symbol(*$1, $3);
        delete $1;
	}
    | ID COLON variable_type L_SQUARE_BRACKET R_SQUARE_BRACKET {
        int size = 1;
        $$ = new Symbol(*$1, $3, size);
    }
    /*| error { yyerrorok; }*/
	;

primary_expression:
    L_PAREN expression R_PAREN { $$ = new Parentheses(line_count, $2); /*cout << "lprp exp" << endl;*/ }
    | variable { 
        $$ = ($1 != nullptr ? $1 : nullptr); /*cout << "Expression: " << $1->value() << endl;*/ 
    }
    | constant { $$ = $1; /*cout << "Expression: " << $1->value() << endl;*/ }
    /*| error { yyerrorok; }*/
    ;

array:
    ID {
        $$ = new Array(line_count, *$1);
        delete $1;
    }
    ;

variable:
    ID {
        $$ = new Variable(line_count, *$1);
        delete $1;
    }
    | ID L_SQUARE_BRACKET expression R_SQUARE_BRACKET {
        $$ = new Array_element(line_count, *$1, $3);
        delete $1;
    }
    /*| error { yyerrorok; }*/
    ;

constant:
    LOGIC_TRUE            { $$ = new Constant(PRSE_type::T_BOOL, "true"); }
    | LOGIC_FALSE         { $$ = new Constant(PRSE_type::T_BOOL, "false"); }
    | INT_CONST           { $$ = new Constant(PRSE_type::T_INT, to_string($1)); }
    | CHAR_CONST          { $$ = new Constant(PRSE_type::T_CHAR, *$1); delete $1; }
    | DOUBLE_CONST 	      { $$ = new Constant(PRSE_type::T_DOUBLE, to_string($1)); }
    | STRING_CONST 	      {
        string t = *$1;
        t.insert(0, 1, '"'); t.insert(t.size(), 1, '"');
        $$ = new Constant(PRSE_type::T_STRING, t);
        delete $1;
    }
    | MYNULL              { $$ = new Constant(PRSE_type::NO_TYPE, "null"); } /* null is acceptable for variable initialization, nothing else. */
    /*| error { yyerrorok; }*/
    ;

/*interpolated_string:
    DOLLAR i_string_segment_list {
        $$ = $2;
    }
    ;

i_string_segment_list:
    I_STRING_BEGIN i_string_middle I_STRING_END { // $"string {format segment} string {or multiple format segments} string"
        vector<string>* l = new vector<string>(0);
        // This is a special case where the strings *have* to have
        // quotes at the beginning and end.
        string t = *$1;
        if (t.size() > 0){
            t.insert(0, 1, '"'); t.insert(t.size(), 1, '"');
            l->push_back(t);
        }

        append_list(l, $2);

        string p = *$3;
        if (p.size() > 0){
            p.insert(0, 1, '"'); p.insert(p.size(), 1, '"');
            l->push_back(p);
        }
        delete $1; delete $2; delete $3;
        $$ = l;
    }
    | STRING_CONST { // $"string of characters"
        string t = *$1;
        delete $1;
        t.insert(0, 1, '"'); t.insert(t.size(), 1, '"');
        vector<string>* l = new vector<string>(0);
        l->push_back(t);
        $$ = l;
    }
    ;

i_string_middle:
    expression I_STRING_PART i_string_middle { // expression | } string part { | <any three of what i_string_middle can take on>
        vector<string>* l = new vector<string>(0);
        if ($1 != nullptr) {
            vector<string>* temp = new vector<string>($1->get_list());
            append_list(l, temp);
            delete temp;
        } else {
            // Warn the programmer against empty string interpolation sections.
            // Can be disabled, just need to decide how.
            cerr << "Line " << line_count << ": warning - interpolated section is empty. Consider removing it." << endl;
        }
        delete $1;
        string t = *$2;
        if (t.size() > 0) {
            t.insert(0, 1, '"'); t.insert(t.size(), 1, '"');
            l->push_back(t);
        }
        delete $2;
        append_list(l, $3);
        delete $3;
        $$ = l;
    }
    | expression {
        vector<string>* l = new vector<string>(0);
        if ($1 != nullptr) {
            vector<string>* temp = new vector<string>($1->get_list());
            append_list(l, temp);
            delete temp;
        } else {
            // Warn the programmer against empty string interpolation sections.
            // Can be disabled, just need to decide how.
            cerr << "Line " << line_count << ": warning - interpolated section is empty. Consider removing it." << endl;
        }
        delete $1;
        $$ = l;
    }
    ;*/

variable_type:
    basic_type {
        $$ = $1;
    }
    /*| error { yyerrorok; }*/
    ;

basic_type:
    BOOL        { $$ = PRSE_type::T_BOOL;   }
    | INT       { $$ = PRSE_type::T_INT;    }
    | CHAR      { $$ = PRSE_type::T_CHAR;   }
    | DOUBLE    { $$ = PRSE_type::T_DOUBLE; }
    | STRING    {
        Library::instance().lib_required["string"] = true;
        $$ = PRSE_type::T_STRING;
    }
    | VOID      { $$ = PRSE_type::T_VOID;   }
    /*| error { yyerrorok; }*/
    ;

empty:
	;

%%

/*static int yyreport_syntax_error(const yypcontext_t* ctx){
    int res = 0;
    YY_LOCATION_PRINT (stderr, *yypcontext_location (ctx));
    fprintf (stderr, ": syntax error");
    // Report the tokens expected at this point.
    {
        enum { TOKENMAX = 5 };
        yysymbol_kind_t expected[TOKENMAX];
        int n = yypcontext_expected_tokens (ctx, expected, TOKENMAX);
        if (n < 0)
        // Forward errors to yyparse.
        res = n;
        else
        for (int i = 0; i < n; ++i)
            fprintf (stderr, "%s %s",
                    i == 0 ? ": expected" : " or", yysymbol_name (expected[i]));
    }
    // Report the unexpected token.
    {
        yysymbol_kind_t lookahead = yypcontext_token (ctx);
        if (lookahead != YYSYMBOL_YYEMPTY)
        fprintf (stderr, " before %s", yysymbol_name (lookahead));
    }
    fprintf (stderr, "\n");
    return res;
}*/
