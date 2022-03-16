#pragma once

#include <set>
#include <utility>
#include <unordered_map>
#include <string>
#include "PRSE_type.h"
#include <vector>

using namespace std;

// Each time a function is declared in a PRSE program,
// another function declaration object is made which contains
// basic data about what arguments were passed.
// When parsing has completed, we compare each function's use to
// its (assumed) definition for any errors.
struct Function_declaration {
    int line_num; // The line this function was called on
    string id;
    vector<PRSE_type> arguments; // The arguments passed in the function call.
    Function_declaration(int line, const string& id, const vector<PRSE_type>& args);
    static vector<Function_declaration*> declarations;
};

// Defines what a function returns, and what parameters it takes
class Function_definition {
private:
    int line_num;
    PRSE_type return_type; // The return type of this function
    vector<PRSE_type> parameter_type; // The parameter type(s) this function takes.
    string id;
public:
    Function_definition();
    Function_definition(int line, const string& name, PRSE_type return_type, const vector<PRSE_type>& parameter_type, const bool is_variadic=false);
    bool compare_signatures(const Function_declaration&); // Compares the use of of a given function call with this
    PRSE_type get_type(){ return return_type; }
    int get_line_num(){ return line_num; }
    string get_id(){ return id; }
    vector<PRSE_type> get_parameters(){ return parameter_type; }
    bool is_variadic;
    static Function_definition* get_function_definition(const Function_declaration& fdec);
    static vector<Function_definition*> find_candidates(const Function_declaration& fdec);
    static set<string> defined_functions; // For functions that have been defined at some point in the program
    static set<string> declared_functions; // For functions that should be defined
    static vector<Function_definition*> definitions; // The point in the program where functions are defined.
    static vector<Function_definition*> definitions_in_current_file; // Functions defined in this file
    static bool main_in_current_file;
    static void cleanup();
};

// Class to track variable definitions

/*static Class_declaration {
    int line_num;
    string id;

};

class Class_definition {

};*/

//vector<Function_declaration> 