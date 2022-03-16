#include "Declared_list.h"

// The point in the program where functions are defined.
vector<Function_definition*> Function_definition::definitions = vector<Function_definition*>();
// The functions of the current file, for header definition creation
vector<Function_definition*> Function_definition::definitions_in_current_file = vector<Function_definition*>();
// For functions that have been defined at some point in the program
set<string> Function_definition::defined_functions = set<string>();
// For functions that should be defined
set<string> Function_definition::declared_functions = set<string>();
// Whether main() is defined in the current file
bool Function_definition::main_in_current_file = false;

Function_declaration::Function_declaration(
    int line, 
    const string& id, 
    const vector<PRSE_type>& args
):line_num(line), id(id), arguments(args){
    declarations.push_back(this);
}

vector<Function_declaration*> Function_declaration::declarations = vector<Function_declaration*>();

Function_definition::Function_definition()
:return_type(NO_TYPE), parameter_type(vector<PRSE_type>(0)), id(""){}

Function_definition::Function_definition(int line, const string& name, PRSE_type return_type, const vector<PRSE_type>& parameter_type, const bool is_variadic)
:return_type(return_type), parameter_type(parameter_type), id(name), is_variadic(is_variadic){}

void Function_definition::cleanup(){
    for (int i = 0; i < (int)definitions.size(); i++){
        delete definitions[i];
    }
    for (int i = 0; i < (int)definitions_in_current_file.size(); i++){
        delete definitions_in_current_file[i];
    }
}

Function_definition* Function_definition::get_function_definition(const Function_declaration& fdec){
    for (auto a : definitions){
        if (a->compare_signatures(fdec)){
            return a;
        }
    }
    return nullptr;
}

/*
Static function to find function candidates
In the even that a programmer calls a function, but none are found that match,
we can call this static function to create a list of potential candidates to provide feedback to the
programmer. This is much more useful than just spitting out a "function not defined" error.
 */
vector<Function_definition*> Function_definition::find_candidates(const Function_declaration& fdec){
    vector<Function_definition*> return_list;
    for (auto a : definitions){
        if (a->get_id() == fdec.id) {
            return_list.push_back(a);
        }
    }
    return return_list;
}

bool Function_definition::compare_signatures(const Function_declaration& fdec){
    if (id != fdec.id){ return false; } // Function IDs should match exactly.

    // Function parameter counts should match exactly.
    if ((int)parameter_type.size() != (int)fdec.arguments.size()) {
        // If parameter_type size is less than arguments passed and this function is NOT
        // variadic, assume it is incorrect
        if ((int)parameter_type.size() < (int)fdec.arguments.size() && !is_variadic)
            return false;
    }
    // Finally, each parameter type must match exactly with what was called.
    for (int i = 0; i < (int)parameter_type.size(); i++){
        if (!(parameter_type[i] & fdec.arguments[i]))
            return false;
    }
    // If all those tests pass, by process of elimination, this function call matches this function's
    // signature.
    return true;
}