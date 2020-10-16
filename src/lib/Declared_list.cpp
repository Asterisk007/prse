#include "Declared_list.h"

vector<Function_definition*> Function_definition::definitions = vector<Function_definition*>();
set<string> Function_definition::defined_functions = set<string>();
set<string> Function_definition::declared_functions = set<string>();

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

Function_definition::Function_definition(int line, const string& name, PRSE_type return_type, const vector<PRSE_type>& parameter_type)
:id(name), return_type(return_type), parameter_type(parameter_type){}

void Function_definition::cleanup(){
    for (int i = 0; i < (int)definitions.size(); i++){
        delete definitions[i];
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

bool Function_definition::compare_signatures(const Function_declaration& fdec){
    if (id != fdec.id){ return false; } // Function IDs should match exactly.
    if ((int)parameter_type.size() != (int)fdec.arguments.size()) { return false; } // Function parameter counts should match exactly.
    // Finally, each parameter type must match exactly with what was called.
    for (int i = 0; i < (int)parameter_type.size(); i++){
        if (parameter_type[i] != fdec.arguments[i])
            return false;
    }
    // If all those tests pass, by process of elimination, this function call matches this function's
    // signature.
    return true;
}