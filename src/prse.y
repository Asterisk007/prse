%{
    #include <stdio.h>
    #include <fstream>
    #include <iostream>
    #include <sstream>
    #include <string>
    #include <vector>
    #include <map>
    #include <regex>
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
	double                union_double;
	std::string*          union_string;
    vector<string>*       union_list;
    //vector<Constant>*     union_constant_list;
    //Symbol*               union_symbol;
    Expression*            union_expression;
    vector<Expression*>*   union_expression_list;
};

%destructor { delete $$; } <union_string>
%destructor { delete $$; } <union_list>
//%destructor { delete $$; } <union_constant_list>
//%destructor { delete $$; } <union_symbol>
%destructor { delete $$; } <union_expression>
%destructor { delete $$; } <union_expression_list>

%define parse.error verbose

// Tokens
// Data constants
%token <union_bool>	   	    BOOL_CONST		"boolean constant"
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
%token MAIN				   	"main"
%token RETURN 			   	"return"
%token CLASS			   	"class"
%token PUBLIC			   	"public"
%token PRIVATE			   	"private"
%token <union_string> CXXMODE      "CXXMODE section"
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
%token DOUBLE_QUOTE 	   	'"'
%token SINGLE_QUOTE 	   	"'"
%token L_PAREN			   	"("
%token R_PAREN			   	")"
%token L_SQUARE_BRACKET 	"["
%token R_SQUARE_BRACKET 	"]"
%token L_CURLY_BRACKET 		"{"
%token R_CURLY_BRACKET 		"}"
%token DOLLAR               "$"
// Variable identification
%token <union_string> ID 	"identifier"

// Production types
%type <union_list> import_list
%type <union_list> definition_list

%type <union_string> import_statement
%type <union_list> libraries_list
%type <union_string> definition
%type <union_string> variable_definition
%type <union_string> variable_assignment
%type <union_string> variable_type
%type <union_string> basic_type
%type <union_expression> constant
%type <union_expression> variable
//%type <union_list> class_definition
//%type <union_symbol> variable
%type <union_list> code_block
%type <union_string> code_line
%type <union_list> scope_block
%type <union_expression> expression
%type <union_list> function_definition
%type <union_string> function_header
%type <union_expression_list> parameter_list_or_empty
%type <union_expression_list> parameter_list
%type <union_expression> parameter
%type <union_expression_list> argument_list
//%type <union_expression> argument
%type <union_expression> function_call
%type <union_string> return_statement
%type <union_expression> primary_expression
%type <union_list> if_block
%type <union_list> if_chain
%type <union_string> if_header
%type <union_string> else_if_header
%type <union_list> optional_else_or_else_if
%type <union_string> cxxblock
//%type <union_expression_list> interpolated_string
//%type <union_expression_list> i_string_segment_list
//%type <union_expression_list> i_string_middle

%token ERROR "error"

%left LOGIC_OR
%left LOGIC_AND
%left LOGIC_EQ LOGIC_NE
%left LOGIC_GREATER LOGIC_GREATER_EQUAL LOGIC_LESS LOGIC_LESS_EQUAL
%left I_STRING_PART PLUS MINUS
%%

program:
	import_list definition_list {
        OutputBuffer& output = OutputBuffer::instance();
        // Output import list
        if ($1 != nullptr){
            for (auto a : *$1){
                output.add_line(a);
            }
        }
        // Also output any additional libraries needed from their use in the program
        Library& li = Library::instance();
        for (auto it = li.lib_required.begin(); it != li.lib_required.end(); it++){
            if (it->second == true){
                string t = "#include <"; t += it->first; t += ">";
                output.add_line(t);
            }
        }



        // Loop through all defined function headers and
        // output their C++ equivalent
        auto def = Function_definition::definitions;
        vector<string> def_strings;

        // function definition format:
        // <type> <id> (param, param)
        for (auto it = def.begin(); it != def.end(); it++){
            Function_definition* fd = *it;
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
            t += ");";
            def_strings.push_back(t);
        }
        for (int i = 0; i < (int)def_strings.size(); i++)
            output.add_line(def_strings[i]);

        // Output function/class/global variable definitions
        if ($2 != nullptr){
            for (auto a : *$2){
                output.add_line(a);
            }
        }

        // Find if any declared functions/classes were not defined
        /* auto func_decs = Function_declaration::declarations;
        for (auto a : func_decs){
            Function_declaration* fdec_p = a;
            if (Function_definition::get_function_definition(fdec_p) == nullptr){
                string t = "";
                for (int i = 0; i < (int)a->arguments.size(); i++) {
                    t += prse_type_to_string(a->arguments[i]);
                    if (i < (int)a->arguments.size()-1)
                        t += ", ";
                }
                Error::error(Error::UNDEFINED_FUNCTION, a->id, t, "", a->line_num);
            }
        } */

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

definition_list:
    definition_list definition {
        vector<string>* l = new vector<string>;
        append_list(l, $1);
        if ($2 != nullptr)
            l->push_back(*$2);
        delete $1; delete $2;
        $$ = l;
    }
    | definition_list function_definition {
        vector<string>* l = new vector<string>;
        append_list(l, $1);
        append_list(l, $2);
        delete $1; delete $2;
        $$ = l;
    }
    | definition_list cxxblock {
        vector<string>* l = new vector<string>;
        append_list(l, $1);
        l->push_back(*$2);
        delete $1; delete $2;
        $$ = l;
    }
    | definition_list if_block {
        vector<string>* l = new vector<string>;
        append_list(l, $1);
        append_list(l, $2);
        delete $1; delete $2;
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
    variable_definition SEMICOLON { *$1 += ";\n"; $$ = $1; }
    | function_header SEMICOLON { *$1 += ";\n"; $$ = $1; }
    /*| error { yyerrorok; }*/
    /*| object_definition { $$ = $1; } */
    ;

variable_definition:
	LET ID COLON variable_type ASSIGN expression {
        if (*$4 != "void"){
            PRSE_type pt = prse_type_from_string(*$4);
            if ($6 == nullptr){
                Error::error(Error::VARIABLE_DECLARED_BUT_NOT_SET, *$2);
                $$ = new string("");
            } else {
                PRSE_type expt = $6->type();
                if (expt == pt || expt == PRSE_type::NO_TYPE || expt == PRSE_type::T_VOID) {
                    string t = string(*$4); // type
                    t += " ";
                    t += *$2; // var identifier
                    t += " = ";
                    t += $6->as_const()->value();
                    $$ = new string(t);
                    Table_handler& th = Table_handler::instance();
                    if (expt == PRSE_type::NO_TYPE){
                        th.insert(shared_ptr<Symbol>(new Symbol(*$2, pt, true)));
                    } else {
                        th.insert(shared_ptr<Symbol>(new Symbol(*$2, pt, false)));
                    }
                } else {
                    Error::error(Error::MISMATCHED_TYPE_FOR_ASSIGNMENT, prse_type_to_string(expt), *$4);
                    $$ = new string("");
                }
            }
        } else {
            Error::error(Error::INVALID_TYPE_FOR_VARIABLE, *$4);
            $$ = new string("");
        }
        delete $2; delete $4; delete $6;
    }
    | LET ID COLON variable_type L_SQUARE_BRACKET R_SQUARE_BRACKET ASSIGN L_CURLY_BRACKET argument_list R_CURLY_BRACKET {
        // Array with specified type
        if (*$4 != "void"){
            Library& li = Library::instance();
            li.lib_required["vector"] = true;

            PRSE_type pt = prse_type_from_string(*$4);
            string t = string("std::vector<"); //"
            t += string(*$4); // type
            t += "> ";
            t += *$2; // var identifier
            t += "{ ";
            for (int i = 0; i < (int)$9->size(); i++){
                Expression* a = (*$9)[i];
                if (a->as_const()->type() != pt){
                    Error::error(Error::MISMATCHED_TYPE_FOR_ASSIGNMENT, prse_type_to_string(a->as_const()->type()), *$4);
                }
                t += a->as_const()->value();
                if (i < (int)$9->size()-1)
                    t += ", ";
            }
            t += " }";
            $$ = new string(t);
            Table_handler& th = Table_handler::instance();
            if ((int)$9->size() == 0){
                th.insert(shared_ptr<Symbol>(new Symbol(*$2, pt, true)));
            } else {
                th.insert(shared_ptr<Symbol>(new Symbol(*$2, pt, false)));
            }
        } else {
            Error::error(Error::INVALID_TYPE_FOR_VARIABLE, *$4);
            $$ = new string("");
        }
        delete $2; delete $4; delete $9;
    }
    | LET ID ASSIGN expression {
        // Duck typing is expressely prohibited if the value is not known at compile time.
        Constant* ex = $4->as_const();
        auto pt = ex->type();
        if (pt == PRSE_type::NO_TYPE){
            Error::error(Error::IMPLICIT_TYPE_CANNOT_BE_ASSIGNED_NULL);
            $$ = new string("");
            cerr << "Line " << line_count << ": cannot initialize variable as 'null' without specifying a type." << endl;
        } else {
            Warning::warning(Warning::VARIABLE_TYPE_IMPLICIT, *$2);
            string t = "auto"; // type
            t += " ";
            t += *$2; // var identifier
            t += " = ";
            t += ex->value();
            $$ = new string(t);
            Table_handler& th = Table_handler::instance();
            th.insert(shared_ptr<Symbol>(new Symbol(*$2, pt, false)));
        }
        delete $2; delete $4;
    }
    | LET ID L_SQUARE_BRACKET R_SQUARE_BRACKET ASSIGN L_CURLY_BRACKET argument_list R_CURLY_BRACKET {
        // Array with unspecified type
        if ((int)$7->size() > 0){
            Library& li = Library::instance();
            li.lib_required["vector"] = true;

            string t = string("std::vector<"); //"
            PRSE_type pt;
            t += prse_type_to_string((*$7)[0]->as_const()->type()); // type
            t += "> ";
            t += *$2; // var identifier
            t += "{ ";
            // The type of the first expression in this list determines the type that this array takes on.
            pt = (*$7)[0]->as_const()->type();
            for (int i = 0; i < (int)$7->size(); i++){
                Expression* a = (*$7)[i];
                if (a->as_const()->type() != pt){
                    Error::error(Error::MISMATCHED_TYPE_FOR_ASSIGNMENT, prse_type_to_string(a->as_const()->type()), prse_type_to_string(pt));
                }
                t += a->as_const()->value();
                if (i < (int)$7->size()-1)
                    t += ", ";
            }
            t += " }";
            $$ = new string(t);
            Table_handler& th = Table_handler::instance();
            th.insert(shared_ptr<Symbol>(new Symbol(*$2, pt, false)));
            delete $2; delete $7;
            break;
        } else {
            Error::error(Error::IMPLICIT_TYPE_CANNOT_BE_ASSIGNED_NULL);
            $$ = new string("");
            delete $2; delete $7;
            break;
        }
    }
    | LET ID COLON variable_type {
        $$ = new string("");
        Error::error(Error::VARIABLE_DECLARED_BUT_NOT_SET, *$2);
        delete $2; delete $4;
    }
    /*| error { yyerrorok; }*/
    ;

if_block:
    if_chain { $$ = $1; }
    /*| error { yyerrorok; }*/
    ;

if_chain:
    if_header scope_block optional_else_or_else_if {
        vector<string>* l = new vector<string>;
        *$1 += "{";
        l->push_back(*$1);
        append_list(l, $2);
        l->push_back("}");
        append_list(l, $3);
        $$ = l;
        delete $1; delete $2; delete $3;
    }
    | if_header code_line optional_else_or_else_if {
        vector<string>* l = new vector<string>;
        *$1 += " ";
        l->push_back(*$1);
        l->push_back(*$2);
        append_list(l, $3);
        $$ = l;
        delete $1; delete $2; delete $3;
    }
    /*| error { yyerrorok; }*/
    ;

optional_else_or_else_if:
    else_if_header scope_block optional_else_or_else_if {
        vector<string>* l = new vector<string>;
        *$1 += "{";
        l->push_back(*$1);
        append_list(l, $2);
        l->push_back("}");
        append_list(l, $3);
        delete $1; delete $2; delete $3;
        $$ = l;
    }
    | else_if_header code_line optional_else_or_else_if {
        vector<string>* l = new vector<string>;
        l->push_back(*$1);
        l->push_back(*$2);
        append_list(l, $3);
        delete $1; delete $2; delete $3;
        $$ = l;
    }
    | ELSE scope_block {
        vector<string>* l = new vector<string>;
        l->push_back("else {");
        append_list(l, $2);
        l->push_back("}");
        delete $2;
        $$ = l;
    }
    | ELSE code_line {
        vector<string>* l = new vector<string>;
        l->push_back("else {");
        l->push_back(*$2);
        delete $2;
        $$ = l;
    }
    | empty { $$ = nullptr; }
    /*| error { yyerrorok; }*/
    ;

if_header:
    IF L_PAREN expression R_PAREN {
        if ($3 == nullptr){
            cerr << "Line " << line_count << ": if statement condition cannot be empty" << endl;
            $$ = new string("");
            break;
        }
        if ($3->as_const()->type() != PRSE_type::T_BOOL){
            cerr << "Line " << line_count << ": if statement condition must be a boolean expression" << endl;
            $$ = new string("");
            break;
        }
        string *t = new string("if (");
        *t += $3->as_const()->value();
        *t += ")";
        $$ = t;
        delete $3;
    }
    /*| error { yyerrorok; }*/
    ;

else_if_header:
    ELSE IF L_PAREN expression R_PAREN {
        if ($4->as_const()->type() != T_BOOL){
            cerr << "Line " << line_count << ": else if statement condition must be a boolean expression" << endl;
            $$ = new string("");
            break;
        }
        string* t = new string("else if (");
        *t += $4->as_const()->value();
        *t += ")";
        $$ = t;
    }
    /*| error { yyerrorok; }*/
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
        $$ = $2;
    }
    /*| error { yyerrorok; }*/
    ;

variable_assignment:
    ID ASSIGN expression {
        if (Table_handler::instance().defined_in_current_scope(*$1) == false){
            Error::error(Error::VARIABLE_NOT_DEFINED_IN_SCOPE, *$1);
            delete $1;
            $$ = new string("");
        }
        Constant* c = $3->as_const();
        //     t =           ID              =             CONSTANT
        string *t = new string(*$1); *t += " = "; *t += c->value();
        $$ = t;
        delete $1; delete $3;
    }
    /*| error { yyerrorok; }*/
    ;

// Function definitions can consist of zero or many parameters, and can have a declared return type (default void)
function_definition:
    function_header scope_block {
        vector<string>* l = new vector<string>;
        string t = *$1;
        delete $1;
        t += "{";
        l->push_back(t);
        append_list(l, $2);
        delete $2;
        l->push_back("}\n");
        /* try {
            Table_handler::instance().pop_table();
        } catch (exception& e){
            cerr << e.what() << endl;
        } */
        $$ = l;
    }
    /*| error { yyerrorok; }*/
    ;

function_header:
	FUNCTION ID L_PAREN parameter_list_or_empty R_PAREN {
        cout << "Function header for " << *$2 << endl;
        Table_handler& th = Table_handler::instance();
        //th.push_table();

        if (Function_definition::defined_functions.find(*$2) == Function_definition::defined_functions.end())
            Function_definition::defined_functions.insert(*$2);

        string* t = new string("");
        if (*$2 == "main"){
            *t += "int ";
        } else {
            *t += "void ";
        }
        *t += *$2; *t += "(";
        vector<PRSE_type> params;
        if ($4 != nullptr) {
            // Main function can have 2 parameters: argc - an int, and argv - a string array
            if (*$2 == "main"){
                // Even though we can make t anything we want, I still insist that the program
                // be grammatically correct.
                if ((int)$4->size() > 2){
                    Error::error(Error::INCORRECT_NUMBER_OF_PARAMETERS_FOR_MAIN);
                } else if ((int)$4->size() == 1){
                    Error::error(Error::INCORRECT_NUMBER_OF_PARAMETERS_FOR_MAIN);
                }
                *t += "int argc, char* argv[]";
            } else {
                for (int i = 0; i < (int)$4->size(); i++) {
                    Expression* a = (*$4)[i];
                    Constant* c = a->as_const();
                    params.push_back(c->type());
                    *t += prse_type_to_string(c->type());
                    *t += " "; *t += c->value();

                    auto d = c->value();
                    auto e = c->type();
                    th.insert(shared_ptr<Symbol>(new Symbol(d, e, false)));

                    if (i != (int)(*$4).size()-1)
                        *t += ", ";
                    delete (*$4)[i];
                }
            }
        }
        Function_definition::definitions.push_back(new Function_definition(line_count, *$2, PRSE_type::T_VOID, params));
        *t += ")";
        delete $2; delete $4;
        $$ = t;
	}
	| FUNCTION ID L_PAREN parameter_list_or_empty R_PAREN COLON variable_type {
        Table_handler& th = Table_handler::instance();
        //th.push_table();

        if (Function_definition::defined_functions.find(*$2) == Function_definition::defined_functions.end())
            Function_definition::defined_functions.insert(*$2);

        string* t = new string("");
        if (*$2 == "main"){
            if (*$7 == "int" || *$7 == "void"){
                *t += "int main(";
            } else {
                cerr << "Line " << line_count << ": function main() can only be of type int or void" << endl;
            }
        } else {
            *t += *$7;
            *t += " ";
            *t += *$2;
            *t += "(";
        }
        vector<PRSE_type> params;
        if ($4 != nullptr) {
            // Main function can have 2 parameters - argc, an in, and argv, a string array
            if (*$2 == "main"){
                // Even though we can make t anything we want, I still insist that the program
                // be grammatically correct.
                if ((int)$4->size() > 2){
                    Error::error(Error::INCORRECT_NUMBER_OF_PARAMETERS_FOR_MAIN);
                } else if ((int)$4->size() == 1){
                    Error::error(Error::INCORRECT_NUMBER_OF_PARAMETERS_FOR_MAIN);
                }
                *t += "int argc, char* argv[]";
            } else {
                for (int i = 0; i < (int)$4->size(); i++) {
                    Expression* a = (*$4)[i];
                    Constant* c = a->as_const();
                    params.push_back(c->type());
                    *t += prse_type_to_string(c->type());
                    *t += " "; *t += c->value();

                    auto d = c->value();
                    auto e = c->type();
                    th.insert(shared_ptr<Symbol>(new Symbol(d, e, false)));

                    if (i != (int)(*$4).size()-1)
                        *t += ", ";
                    delete (*$4)[i];
                }
            }
        }
        Function_definition::definitions[line_count] = new Function_definition(line_count, *$2, prse_type_from_string(*$7), params);
        *t += ")";
        delete $2; delete $4; delete $7;
        $$ = t;
    }
    /*| error { yyerrorok; }*/
    ;

code_block:
    code_block variable_definition SEMICOLON {
        vector<string>* l = new vector<string>;
        *$2 += ";";
        append_list(l, $1);
        l->push_back(*$2);
        delete $1; delete $2;
        $$ = l;
    }
    | code_block variable_assignment SEMICOLON {
        vector<string>* l = new vector<string>;
        *$2 += ";";
        append_list(l, $1);
        l->push_back(*$2);
        delete $1; delete $2;
        $$ = l;
    }
    | code_block cxxblock {
        vector<string>* l = new vector<string>;
        append_list(l, $1);
        l->push_back(*$2);
        delete $1; delete $2;
        $$ = l;
    }
    | code_block if_block {
        vector<string>* l = new vector<string>;
        append_list(l, $1);
        append_list(l, $2);
        delete $1; delete $2;
        $$ = l;
    }
    | code_block function_call SEMICOLON {
        vector<string>* l = new vector<string>;
        string t = $2->as_const()->value();
        cout << "Function call: " << t << endl;
        t += ";";
        append_list(l, $1);
        l->push_back(t);
        delete $1; delete $2;
        $$ = l;
    }
    | code_block return_statement SEMICOLON {
        vector<string>* l = new vector<string>;
        append_list(l, $1);
        *$2 += ";";
        l->push_back(*$2);
        delete $1; delete $2;
        $$ = l;
    }
    | code_block scope_block {
        vector<string>* l = new vector<string>;
        append_list(l, $1);
        //Table_handler::instance().push_table();
        append_list(l, $2);
        /* try {
            Table_handler::instance().pop_table();
        } catch (exception& e){
            cerr << e.what() << endl;
        } */
        delete $1; delete $2;
        $$ = l;
    }
	| empty {
        $$ = nullptr;
    }
    /*| error { yyerrorok; }*/
	;

code_line:
    variable_definition SEMICOLON {
        *$1 += ";";
        $$ = $1;
    }
    | variable_assignment SEMICOLON {
        *$1 += ";";
        $$ = $1;
    }
    | function_call SEMICOLON {
        string* t = new string($1->as_const()->value());
        *t += ";";
        $$ = t;
    }
    | return_statement SEMICOLON {
        *$1 += ";";
        $$ = $1;
    }
    /*| error { yyerrorok; }*/
    ;

expression:
    primary_expression { $$ = $1; }
    | function_call { $$ = $1; }
    | expression PLUS expression {
        $$ = new Plus($1, $3);
    }
    | expression MINUS expression {
        $$ = new Minus($1, $3);
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
        string t = string("");
        // Print functions:
        /*
            valid syntax:
            print(<string constant>); --> cout << <string constant>;
            print($"string with {vars}"); --> cout << "string with " << vars;

            println works same as above, but appends an endl to the end.

            $<string constant> is an interpolated string
        */
        vector<PRSE_type> args;
        if (*$1 == "print" || *$1 == "println") {
            //if ($3 != nullptr) {
                if ($3->size() == 1){
                    if ((*$3)[0]->type() == PRSE_type::T_STRING){
                        t += "std::cout << ";
                        string arg = (*$3)[0]->as_const()->value();
                        t += "\""; t += arg; t += "\"";
                        if (*$1 == "println") {
                            t += " << std::endl;";
                        }
                    }
                    /*else if ((*$3)[0]->get_expr_type() == PRSE_type::T_I_STRING){
                        // Interpolated string
                        t += "std::cout << ";
                        vector<string> l = (*$3)[0]->get_list();
                        for (int i = 0; i < (int)l.size(); i++){
                            t += l[i];
                            if (i != (int)l.size()-1)
                                t += " << ";
                        }
                        if (*$1 == "println") {
                            t += " << std::endl;";
                        } else {
                            t += ";";
                        }
                    }*/
                    else {
                        cerr << "Line " << line_count << ": incorrect use of function 'print()'.\n";
                        cerr << "For correct usage, see: \nhttps://asterisk007.gitlab.io/prse/docs/print.html\n"; //"
                    }
                } else {
                    cerr << "Line " << line_count << ": incorrect use of function 'print()'.\n";
                    cerr << "For correct usage, see: \nhttps://asterisk007.gitlab.io/prse/docs/print.html\n";
                }
            //}
        }
        else {
            // First, add this function to the list of declared functions.
            // when finished with parsing, we will throw out errors if some function is not defined.
            if (Function_definition::declared_functions.find(*$1) == Function_definition::declared_functions.end())
                Function_definition::declared_functions.insert(*$1);
            //Function_declaration()
            // Append function ID
            t += *$1;
            t += "(";
            // Go through each argument, and put it into the function call.
            // TODO: compare argument types with function definition, and throw
            // out errors if types do not match.
            vector<Expression*>* l = $3;
            for (int i = 0; i < (int)l->size(); i++){
                t += (*l)[i]->as_const()->value();
                args.push_back((*l)[i]->as_const()->type());
                if (i != (int)l->size()-1)
                    t += ", ";
                delete (*l)[i];
            }
            // TODO: throw out error if function is defined with parameters but no arguments are given.
            t += ")";
        }
        //Function_declaration::declarations.push_back(new Function_declaration(line_count, *$1, args));
        //auto fd = Function_definition::get_function_definition(*$1, args);
        PRSE_type pt = PRSE_type::T_VOID;
        //if (fd != nullptr){
        //    pt = fd->get_type();
        //}
        Constant* c = new Constant(pt, t);
        $$ = c;
        delete $1; delete $3;
    }
    /*| error { yyerrorok; }*/
    ;

return_statement:
    RETURN expression {
        string* t = new string("return ");
        *t += $2->as_const()->value();
        $$ = t;
    }
    /*| error { yyerrorok; }*/
    ;

argument_list:
    expression COMMA argument_list {
        // Expression vector pointer for $$'s value.
        vector<Expression*>* l = new vector<Expression*>();
        if ($1 != nullptr)
            l->push_back($1);
        append_list(l, $3);
        $$ = l;
        //delete $1;
        delete $3;
    }
    | expression {
        vector<Expression*>* l = new vector<Expression*>();
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
        vector<Expression*>* l = new vector<Expression*>();
        l->push_back($1);
        append_list(l, $3);
        delete $3;
        $$ = l;
    }
    | parameter {
        vector<Expression*>* l = new vector<Expression*>();
        l->push_back($1);
        $$ = l;
    }
    /*| error { yyerrorok; }*/
    ;

parameter:
	ID COLON variable_type {
        cout << "Parameter: " << *$1 << ": " << *$3 << endl;
        $$ = new Constant(prse_type_from_string(*$3), *$1);
        delete $1;
        delete $3;
	}
    /*| error { yyerrorok; }*/
	;

primary_expression:
    L_PAREN expression R_PAREN { $$ = $2; /*cout << "lprp exp" << endl;*/ }
    | variable { $$ = ($1 != nullptr ? $1 : nullptr); /*cout << "Expression: " << $1->value() << endl;*/ }
    | constant { $$ = $1; /*cout << "Expression: " << $1->value() << endl;*/ }
    /*| error { yyerrorok; }*/
    ;

variable:
    ID {
        Table_handler& th = Table_handler::instance();
        cout << "Checking if variable " << *$1 << " on line " << line_count << " is defined in this scope" << endl;
        if (th.defined_in_current_scope(*$1)){
            cout << "It is!" << endl;
            $$ = th.lookup(*$1)->as_const();
            delete $1;
            break;
        } else {
            cout << "It is not :(" << endl;
            Error::error(Error::VARIABLE_NOT_DEFINED_IN_SCOPE, *$1);
            $$ = nullptr;
            delete $1;
        }
    }
    /*| error { yyerrorok; }*/
    ;

constant:
    BOOL_CONST            { $$ = ($1 == true) ? new Constant(PRSE_type::T_BOOL, "true") : new Constant(PRSE_type::T_BOOL, "false"); }
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
    BOOL { $$ = new std::string("bool"); }
    | INT { $$ = new std::string("int"); }
    | CHAR { $$ = new std::string("char"); }
    | DOUBLE { $$ = new std::string("double"); }
    | STRING { $$ = new std::string("string"); }
    | VOID { $$ = new std::string("void"); }
    /*| error { yyerrorok; }*/
    ;

cxxblock:
    CXXMODE {
        $$ = $1;
    }
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