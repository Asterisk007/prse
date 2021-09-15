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
    FEDORA: variable is not an array, but is being accessed as if it was
    HATBOX: a variable set to null was operated on (+, -, *, /, etc.)
    BERET: function already defined with specified parameters
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
        case VARIABLE_ALREADY_DECLARED:
            cout << "variable '" << s1 << "' was already declared";
            break;
        case VARIABLE_IS_NOT_AN_ARRAY:
            cout << "variable '" << s1 << "' is not an array, but is being used as if it was.";
            break;
        case ARRAY_SUBSCRIPT_MUST_BE_AN_INTEGER_VALUE:
            cout << "array elements must only be accessed with integers";
            break;
        case IMPLICIT_TYPE_CANNOT_BE_ASSIGNED_NULL:
            cout << "implicitly typed variables must be assigned a non-null value";
            break;
        case INVALID_TYPE_FOR_VARIABLE:
            cout << "data type '" << s1 << "' is not a valid variable type" << endl;
            cout << "Available types are: bool, int, double, character, string (incl. interpolated string)";
            break;
        case INVALID_EXPRESSION_TYPE_FOR_OPERATION:
            cout << s1 << " expression type " << s2 << " is invalid for operation '" << s3 << "'";
            break;
        case INVALID_EXPRESSION_TYPE_FOR_ARRAY_SIZE:
            cout << "expression type '" << s1 << "' is not a valid type for array size" << endl;
            cout << "Valid types are: int";
            break;
        case MISMATCHED_TYPE_FOR_ASSIGNMENT:
            cout << "expression type (" << s1 << ") does not match variable's type (" << s2 << ")";
            break;
        case EXPRESSION_TYPES_DO_NOT_MATCH:
            cout << "expression type " << s1 << " does not match expression type " << s2;
            break;
        case UNDEFINED_FUNCTION:
            cout << "function call for '" << s1 << "(" << s2  << ")' does not match any defined function";
            break;
        case FUNCTION_WITH_RETURN_TYPE_ALREADY_DEFINED:
            cout << "function '" << s1 << "' with parameters (" << s2 << ") is already defined";
            break;
        case NULL_CANNOT_BE_USED_IN_OPERATION:
            cout << "null cannot be used in >, <, >=, <= comparisons";
            break;
        case INCORRECT_NUMBER_OF_PARAMETERS_FOR_MAIN:
            cout << "function 'main' cannot have any parameters listed (" << s1 << " were added)";
            break;
        case INVALID_RETURN_TYPE_FOR_MAIN:
            cout << "function main() can only be of type int or void";
            break;
        case RETURN_VALUE_FOR_FUNCTION_DOES_NOT_MATCH:
            cout << "expression in return statement has type: " << s1 << ", which does not match the function's return type: " << s2;
            break;
        case CONDITION_CANNOT_BE_EMPTY:
            cout << "if and else if conditions cannot be empty";
            break;
        case CONDITION_MUST_BE_BOOLEAN_EXPRESSION:
            cout << "conditions must be a boolean expression (expression is of type " << s1 << ")";
            break;
        case UNEXPECTED_ELSE_ELSE_IF:
            cout << "unexpected else or else if. If statement should be placed before this.";
            break;
        case OTHER: // Other error that is not covered. Eg. Critical internal error
            cout << s1;
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
            case VARIABLE_ALREADY_DECLARED:
                cout << "(EXISTINGVAR Error)";
                break;
            case VARIABLE_IS_NOT_AN_ARRAY:
                cout << "(VARNOTARRAY Error)";
                break;
            case ARRAY_SUBSCRIPT_MUST_BE_AN_INTEGER_VALUE:
                cout << "(ARREXPRNOTVALID Error)";
                break;
            case IMPLICIT_TYPE_CANNOT_BE_ASSIGNED_NULL:
                cout << "(CORKHAT Error)";
                break;
            case INVALID_EXPRESSION_TYPE_FOR_ARRAY_SIZE:
                cout << "(INVALIDEXPR Error)";
                break;
            case INVALID_EXPRESSION_TYPE_FOR_OPERATION:
                cout << "(EXPRINVALIDFOROP Error)";
                break;
            case EXPRESSION_TYPES_DO_NOT_MATCH:
                cout << "(EXPRNOMATCH Error)";
                break;
            case MISMATCHED_TYPE_FOR_ASSIGNMENT:
                cout << "(ASSIGNMENTTYPE Error)";
                break;
            case UNDEFINED_FUNCTION:
                cout << "(TRILBY Error)";
                break;
            case FUNCTION_WITH_RETURN_TYPE_ALREADY_DEFINED:
                cout << "(BERET Error)";
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
            case RETURN_VALUE_FOR_FUNCTION_DOES_NOT_MATCH:
                cout << "(RETURNTYPE Error)";
                break;
            case CONDITION_CANNOT_BE_EMPTY:
                cout << "(EMPTYCOND Error)";
                break;
            case CONDITION_MUST_BE_BOOLEAN_EXPRESSION:
                cout << "(CONDTYPE Error)";
                break;
            case UNEXPECTED_ELSE_ELSE_IF:
                cout << "(NOIFFOUND Error)";
                break;
            case OTHER:
                cout << "(SOMETHINGELSE Error)";
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
        const string& s3,
        const int line_override
    ){
    if (show_warnings == false) { return; }

    cout << "PRSEWARN: Line " << line_override << ": ";
    switch (warning_type){
        case VARIABLE_TYPE_IMPLICIT:
            cout << "variable '" << s1 << "' has an implicit type";
            break;
        case VARIABLE_DECLARED_BUT_NOT_USED:
            cout << "variable '" << s1 << "' is declared but not used";
            break;
        case VARIABLE_DECLARATION_USELESS:
            cout << "variable declared here will go out of scope immediately. Consider surrounding it with curly braces ( { and } characters )";
        case EXPRESSION_CAST_INVALID:
            break;
    }
    cout << endl;
    warning_count++;
}
