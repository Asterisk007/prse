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
    Constant*             union_constant;
    //vector<Constant>*     union_constant_list;
    //Symbol*               union_symbol;
    Expression*           union_expression;
    vector<Expression>*   union_expression_list;
};

%destructor { delete $$; } <union_string>
%destructor { delete $$; } <union_list>
%destructor { delete $$; } <union_constant>
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
%token CXXMODE              "cxxmode"
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
%type <union_list> declaration_list
%type <union_list> function_list

%type <union_string> import_statement
%type <union_list> libraries_list
//%type <union_list> class_declaration_list
%type <union_string> declaration
%type <union_string> variable_declaration
%type <union_string> variable_assignment
%type <union_string> variable_type
%type <union_string> basic_type
%type <union_constant> constant
//%type <union_symbol> variable
//%type <union_bool> optional_array
%type <union_list> expression_list
%type <union_expression> expression
%type <union_list> function_definition
%type <union_string> function_header
%type <union_list> parameter_list_or_empty
%type <union_list> parameter_list
%type <union_string> parameter
%type <union_expression_list> argument_list_or_empty
%type <union_expression_list> argument_list
%type <union_constant> argument
%type <union_string> function_call
%type <union_list> interpolated_string
%type <union_list> i_string_segment_list
%type <union_list> i_string_middle

%token ERROR "error"

%left I_STRING_PART
%%

program:
	import_list declaration_list function_list {
        OutputBuffer& output = OutputBuffer::instance();
        if ($1 != nullptr){
            for (auto a : *$1){
                output.add_line(a);
            }
        }

        if ($2 != nullptr){
            for (auto a : *$2){
                output.add_line(a);
            }
        }
        
        if ($3 != nullptr){
            for (auto a : *$3){
                output.add_line(a);
            }
        }
        delete $1; delete $2; delete $3;
    }
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
	;

declaration_list:
    declaration_list declaration {
        vector<string>* l = new vector<string>;
        append_list(l, $1);
        if ($2 != nullptr)
            l->push_back(*$2);
        delete $1; delete $2;
        $$ = l;
    }
    | empty { $$ = nullptr; }
	;

declaration:
    variable_declaration { $$ = $1; }
    /*| object_declaration { $$ = $1; } */
    ;

variable_declaration:
	LET ID COLON variable_type ASSIGN expression SEMICOLON {
        if (*$4 != "void"){
            PRSE_type pt = prse_type_from_string(*$4);
            PRSE_type expt = $6->get_expr_type();
            if (expt == pt || expt == PRSE_type::NO_TYPE) {
                string t = string(*$4); // type
                t += " ";
                t += *$2; // var identifier
                t += " = ";
                for (auto a : $6->get_list()){
                    t += a;
                }
                t += ";";
                $$ = new string(t);
                Table_handler& th = Table_handler::instance();
                if (expt == PRSE_type::NO_TYPE){
                    th.insert(shared_ptr<Symbol>(new Symbol(*$2, pt, true)));
                } else {
                    th.insert(shared_ptr<Symbol>(new Symbol(*$2, pt, false)));
                }
            }
        } else {
            cerr << "Line " << line_count << ": Invalid type for variable" << endl;
            cerr << "Available types are: bool, int, double, character, string (incl. interpolated string)" << endl; //"
            $$ = nullptr;
        }
        delete $2; delete $4; delete $6;
    }
    | LET ID ASSIGN expression SEMICOLON {
        // TODO: Warn programmer against the use of duck typing (its not bad,
        // just not advisable. I have to do extra work to make their thing translate).
        // Duck typing is expressely prohibited if the value is not known at compile time.
        Expression ex = *$4;
        auto pt = ex.get_expr_type();
        if (pt == PRSE_type::NO_TYPE){
            cerr << "Line " << line_count << ": cannot initialize variable as 'null' without specifying a type." << endl;
        } else {
            string t = string(prse_type_to_string(pt)); // type
            t += " ";
            t += *$2; // var identifier
            t += " = ";
            for (auto a : ex.get_list()){
                t += a; // expression(s)
            }
            t += ";";
            $$ = new string(t);
            Table_handler& th = Table_handler::instance();
            th.insert(shared_ptr<Symbol>(new Symbol(*$2, pt, false)));
        }
        delete $2; delete $4;
    }
    | LET ID COLON variable_type SEMICOLON {
        delete $2; delete $4;
        $$ = nullptr;
        Error::error(Error::VARIABLE_DECLARED_BUT_NOT_SET);
    }
    ;

// TODO: ADD CLASSES
/*class_declaration_list:
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
    ;*/

variable_assignment:
    ID ASSIGN constant SEMICOLON {
        //     t =           ID              =             CONSTANT
        string *t = new string(*$1); *t += " = "; *t += $3->get_value(); *t += ";\n";
        $$ = t;
        delete $1; delete $3;
    }
    ;

function_list:
    function_list function_definition {
        vector<string>* l = new vector<string>;
        append_list(l, $1);
        append_list(l, $2);
        delete $1; delete $2;
        $$ = l;
    }
    | empty { $$ = nullptr; }
    ;

// Function definitions can consist of none, one, or many arguments, and can have a declared return type (default void)
function_definition:
    function_header L_CURLY_BRACKET expression_list R_CURLY_BRACKET {
        vector<string>* l = new vector<string>;
        *$1 += "{";
        l->push_back(*$1);
        append_list(l, $3);
        l->push_back("}\n");
        delete $1; delete $3;
        $$ = l;
    }
    ;

function_header:
	FUNCTION ID L_PAREN parameter_list_or_empty R_PAREN {
        string* t = new string("");
        if (*$2 == "main"){
            *t += "int ";
        } else {
            *t += "void ";
        }
        *t += "main (";
        if ($4 != nullptr) {
            for (auto a : *$4) {
                *t += a;
                *t += ", ";
            }
        }
        *t += ")";
        delete $2; delete $4;
        $$ = t;
	}
	| FUNCTION ID L_PAREN parameter_list_or_empty R_PAREN COLON variable_type L_CURLY_BRACKET {
        string* t = new string("");
        if (*$2 == "main"){
            if (*$7 == "int" || *$7 == "void"){
                *t += "int main (";
            } else {
                cerr << "Line " << line_count << ": function main() can only be of type int or void" << endl;
            }
        } else {
            *t += *$7;
            *t += *$2;
        }
        if ($4 != nullptr) {
            for (auto a : *$4) {
                *t += a;
                *t += ", ";
            }
        }
        *t += ") {\n";
        delete $2; delete $4; delete $7;
        $$ = t;
    }
    ;

expression_list:
    variable_declaration expression_list{
        vector<string>* l = new vector<string>;
        l->push_back(*$1);
        append_list(l, $2);
        delete $1; delete $2;
        $$ = l;
    }
    | variable_assignment expression_list {
        vector<string>* l = new vector<string>;
        l->push_back(*$1);
        append_list(l, $2);
        delete $1; delete $2;
        $$ = l;
    }
    | function_call expression_list {
        vector<string>* l = new vector<string>;
        l->push_back(*$1);
        append_list(l, $2);
        delete $1; delete $2;
        $$ = l;
    }
	| empty {
        $$ = nullptr;
    }
	;

expression:
    ID {
        Table_handler& th = Table_handler::instance();
        if (th.defined_in_current_scope(*$1)){
            shared_ptr<Symbol> sym = th.lookup(*$1);
            vector<string> expr_list = vector<string>(0);
            expr_list.push_back(sym->get_name());
            auto pt = sym->get_type();
            $$ = new Expression(pt, expr_list);
            delete $1;
            break;
        } else {
            cerr << "Line " << line_count << ": variable '" << *$1 << "' does not exist in this scope." << endl;
            delete $1;
        }
    }
    | constant {
        vector<string> el = vector<string>(0);
        //cout << "Constant value is " << $1->get_value() << endl;
        el.push_back($1->get_value());
        auto et = $1->get_type();
        $$ = new Expression(et, el);
        delete $1;
    }
    // IMPLEMENT
    /*| function_call {
        $$ = $1;
    }*/
    /*| expression PLUS expression {

    }*/
    | empty { $$ = nullptr; }
	;

/*class_function_list_or_empty:
    class_function_list_or_empty class_function
    | empty
    ;

class_function:
    optional_public_or_private FUNCTION ID L_PAREN parameter_list R_PAREN L_CURLY_BRACKET expression_list R_CURLY_BRACKET {
        delete $3;
    }
    | optional_public_or_private FUNCTION ID L_PAREN parameter_list R_PAREN COLON variable_type L_CURLY_BRACKET expression_list R_CURLY_BRACKET {
        delete $3; delete $8;
    }
    ;

optional_public_or_private:
    PUBLIC
    | PRIVATE
    | empty
    ;*/


function_call:
    ID L_PAREN argument_list_or_empty R_PAREN SEMICOLON {
        string* t = new string("");
        // Print functions:
        /*
            valid syntax:
            print(<string constant>); --> cout << <string constant>;
            print($"string with {vars}"); --> cout << "string with " << vars;

            println works same as above, but appends an endl to the end.

            $<string constant> is an interpolated string
        */
        if (*$1 == "print" || *$1 == "println") {
            if ($3 != nullptr) {
                if ($3->size() == 1){
                    if ((*$3)[0].get_expr_type() == PRSE_type::T_STRING){
                        *t += "std::cout << ";
                        string arg = (*$3)[0].get_list()[0];
                        *t += "\""; *t += arg; *t += "\"";
                        if (*$1 == "println") {
                            *t += " << std::endl;";
                        }
                    }
                    else if ((*$3)[0].get_expr_type() == PRSE_type::T_I_STRING){
                        // Interpolated string
                        *t += "std::cout << ";
                        vector<string> l = (*$3)[0].get_list();
                        for (int i = 0; i < (int)l.size(); i++){
                            *t += l[i];
                            if (i != (int)l.size()-1)
                                *t += " << ";
                        }
                        if (*$1 == "println") {
                            *t += " << std::endl;";
                        } else {
                            *t += ";";
                        }
                    }
                    else {
                        cerr << "Line " << line_count << ": incorrect use of function 'print()'.\n";
                        cerr << "For correct usage, see: \nhttps://asterisk007.gitlab.io/prse/docs/print.html\n"; //"
                    }
                } else {
                    cerr << "Line " << line_count << ": incorrect use of function 'print()'.\n";
                    cerr << "For correct usage, see: \nhttps://asterisk007.gitlab.io/prse/docs/print.html\n";
                }
            }
        }
        else {
            cerr << "Function not defined: " << *$1 << endl;
        }
        $$ = t;
        delete $1; delete $3;
    }
    ;

argument_list_or_empty:
    argument_list { $$ = $1; }
    | empty { $$ = nullptr; }
    ;

argument_list:
    argument COMMA argument_list {
        vector<Expression>* l = new vector<Expression>(0);
        PRSE_type et = $1->get_type();
        vector<string>* ev = new vector<string>(0);
        vector<string>* vl;
        if ($1->get_value_list().size() > 0) {
            vl = new vector<string>($1->get_value_list());
            append_list(ev, vl);
            delete vl;
        } else ev->push_back($1->get_value());
        l->push_back(Expression(et, *ev));
        delete ev;
        append_list(l, $3);
        $$ = l;
        delete $1; delete $3;
    }
    | argument {
        vector<Expression>* l = new vector<Expression>(0);
        PRSE_type et = $1->get_type();
        vector<string>* ev = new vector<string>(0);
        vector<string>* vl;
        if ($1->get_value_list().size() > 0) {
            vl = new vector<string>($1->get_value_list());
            append_list(ev, vl);
            delete vl;
        } else ev->push_back($1->get_value());
        l->push_back(Expression(et, *ev));
        delete ev;
        $$ = l;
        delete $1;
    }
    ;

argument:
    constant {
        $$ = $1;
    }
    | ID {
        // Have to now look up variable in table.
        // If it exists, return a shared pointer to it
        // If not, return a nullptr.
        Table_handler& th = Table_handler::instance();
        if (th.defined_in_current_scope(*$1)){
            auto s = th.lookup(*$1);
            $$ = new Constant(s->get_type(), s->get_name());
        } else {
            cerr << "Line " << line_count << ": variable '" << *$1 << "' is not defined in this scope." << endl;
        }
        delete $1;
    }
    ;

parameter_list_or_empty:
    parameter_list { $$ = $1; }
    | empty { $$ = nullptr; }
    ;

parameter_list:
    parameter COMMA parameter_list {
        vector<string>* l = new vector<string>;
        *$1 += ", ";
        l->push_back(*$1);
        append_list(l, $3);
        delete $1; delete $3;
        $$ = l;
    }
    | parameter {
        vector<string>* list = new vector<string>;
        list->push_back(*$1);
        delete $1;
        $$ = list;
    }
    ;

parameter:
	ID COLON variable_type {
        string* p = new string(*$3);
        delete $3;
        *p += *$1;
        delete $1;
        $$ = p;
	}
	;

/*symbol:
    ID { $$ = $1; }*/

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
    /*| ID                  { $$ = new Constant(PRSE_type::, *$1); delete $1; }*/
    | interpolated_string { $$ = new Constant(PRSE_type::T_I_STRING, *$1); delete $1; }
    | MYNULL              { $$ = new Constant(PRSE_type::NO_TYPE, "null"); } /* null is acceptable for variable initialization, nothing else. */
    ;

interpolated_string:
    DOLLAR i_string_segment_list { $$ = $2; }
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

/*optional_array:
    L_SQUARE_BRACKET R_SQUARE_BRACKET {
        $$ = true;
    }
    | empty {
        $$ = false;
    }
    ;*/

// TODO: Add support for raw C++ code blocks
/*cxxblock:
    L_SQUARE_BRACKET CXXMODE R_SQUARE_BRACKET L_CURLY_BRACKET CXXCODE R_CURLY_BRACKET {

    }
*/

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