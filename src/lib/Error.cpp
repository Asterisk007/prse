// The Error class aims to help avoid the pitfalls of C++'s error reporting by being clear and concise.
// But since I'm no magician, I doubt I can do what rustc does and explain how to fix every error.
// (at the same time, if I have time to do it, I may as well try)

#include "Error.h"

#include <iostream>

// TODO: work out new error code names
/* Error codes:
    DERBY: variable declared but not set
    NOHAT: variable used but not declared
    TRILBY: undefined function
    CORKHAT: implicit variable type cannot be assigned to null
    VISOR: invalid type for variable
    FEDORA: 
    HATBOX: a variable set to null was operated on (+, -, *, /, etc.)
    BERET: 
    BUCKET: 
    BASEBALL_CAP:
    SNAPBACK: 
*/


int Error::error_count = 0;
bool Error::print_error_codes = true;

int Warning::warning_count = 0;
bool Warning::show_warnings = true;


void Error::error(
    const Error_type error_type,
    const string& s1,
    const string& s2,
    const string& s3,
    const int line_override
    ){
    cout << "PRSERR: Line " << line_override << ": ";
    switch (error_type){
        case VARIABLE_DECLARED_BUT_NOT_SET:
            cout << "variable '" << s1 << "' was declared, but not set";
            break;
        case VARIABLE_NOT_DEFINED_IN_SCOPE:
            cout << "variable '" << s1 << "' is not defined in this scope";
            break;
        case IMPLICIT_TYPE_CANNOT_BE_ASSIGNED_NULL:
            cout << "implicitly typed variables must be assigned a non-null value";
            break;
        case INVALID_TYPE_FOR_VARIABLE:
            cout << "data type '" << s1 << "' is not a valid variable type" << endl;
            cout << "Available types are: bool, int, double, character, string (incl. interpolated string)";
            break;
        case INVALID_EXPRESSION_TYPE_FOR_ARRAY_SIZE:
            cout << "expression type '" << s1 << "' is not a valid type for array size" << endl;
            cout << "Valid types are: int";
            break;
        case MISMATCHED_TYPE_FOR_ASSIGNMENT:
            cout << "expression type (" << s1 << ") does not match variable's type of " << s2;
            break;
        case UNDEFINED_FUNCTION:
            cout << "function '" << s1 << "(" << s2  << ")' does not match any defined function";
            break;
        case NULL_CANNOT_BE_USED_IN_OPERATION:
            cout << "null cannot be used in >, <, >=, <= comparisons";
            break;
        case INCORRECT_NUMBER_OF_PARAMETERS_FOR_MAIN:
            cout << "function 'main' may only have 0 or exactly 2 parameters: argc: int and argv: string[]";
            break;
        case INVALID_RETURN_TYPE_FOR_MAIN:
            cout << "function main() can only be of type int or void";
            break;
    }
    error_count++;
    // Print error alias code
    if (print_error_codes){
        cout << " ";
        switch (error_type){
            case VARIABLE_DECLARED_BUT_NOT_SET:
                cout << "(DERBY Error)";
                break;
            case VARIABLE_NOT_DEFINED_IN_SCOPE:
                cout << "(NOHAT Error)";
                break;
            case IMPLICIT_TYPE_CANNOT_BE_ASSIGNED_NULL:
                cout << "(CORKHAT Error)";
                break;
            case INVALID_EXPRESSION_TYPE_FOR_ARRAY_SIZE:
                cout << "(INVALIDEXPR Error)";
                break;
            case MISMATCHED_TYPE_FOR_ASSIGNMENT:
                cout << "(ASSIGNMENTTYPE Error)";
                break;
            case UNDEFINED_FUNCTION:
                cout << "(TRILBY Error)";
                break;
            case INVALID_TYPE_FOR_VARIABLE:
                cout << "(VISOR Error)";
                break;
            case NULL_CANNOT_BE_USED_IN_OPERATION:
                cout << "(HATBOX Error)";
                break;
            case INCORRECT_NUMBER_OF_PARAMETERS_FOR_MAIN:
                cout << "(BADPARAMCOUNT Error)";
                break;
            case INVALID_RETURN_TYPE_FOR_MAIN:
                cout << "(MAINTYPE Error)";
                break;
        }
    }
    cout << endl;
}

int Error::num_errors(){
    return error_count;
}

void Warning::warning(
        Warning_type warning_type,
        const string& s1,
        const string& s2,
        const string& s3
    ){
    if (show_warnings == false) { return; }

    cout << "PRSEWarn: Line " << line_count << ": ";
    switch (warning_type){
        case VARIABLE_TYPE_IMPLICIT:
            cout << " variable '" << s1 << "' has an implicit type";
            break;
        case VARIABLE_DECLARED_BUT_NOT_USED:
            cout << " variable '" << s1 << "' is declared but not used";
            break;
    }
    cout << endl;
    warning_count++;
}