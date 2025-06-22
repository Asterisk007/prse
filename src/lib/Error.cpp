// The Error class aims to help avoid the pitfalls of C++'s error reporting by being clear and concise.
// But since I'm no magician, I doubt I can do what rustc does and explain how to fix every error.
// (at the same time, if I have time to do it, I may as well try)

#include "Error.h"

#include <iostream>

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
    const vector<string>& details,
    const int line_override
){
    cout << "PRSERR: Line " << line_override << ": ";
    switch (error_type){
        case INVALID_USE_FILE_SPECIFIED:
            cout << "'use' statement must include '.prse' extension if it is not in the stdlib";
            break;
        case VARIABLE_DECLARED_BUT_NOT_SET:
            cout << "variable '" << details[0] << "' was declared, but not set";
            break;
        case VARIABLE_NOT_DEFINED_IN_SCOPE:
            cout << "variable '" << details[0] << "' is not defined in this scope";
            break;
        case VARIABLE_ALREADY_DECLARED:
            cout << "variable '" << details[0] << "' was already declared";
            break;
        case VARIABLE_IS_NOT_AN_ARRAY:
            cout << "variable '" << details[0] << "' is not an array, but is being used as if it was.";
            break;
        case ARRAY_SUBSCRIPT_MUST_BE_AN_INTEGER_VALUE:
            cout << "array elements must only be accessed with integers";
            break;
        case IMPLICIT_TYPE_CANNOT_BE_ASSIGNED_NULL:
            cout << "implicitly typed variables must be assigned a non-null value";
            break;
        case INVALID_TYPE_FOR_VARIABLE:
            cout << "data type '" << details[0] << "' is not a valid variable type" << endl;
            cout << "Available types are: bool, int, double, character, string (incl. interpolated string)";
            break;
        case INVALID_EXPRESSION_TYPE_FOR_OPERATION:
            cout << details[0] << " expression type " << details[1] << " is invalid for operation '" << details[2] << "'";
            break;
        case INVALID_EXPRESSION_TYPE_FOR_ARRAY_SIZE:
            cout << "expression type '" << details[0] << "' is not a valid type for array size" << endl;
            cout << "Valid types are: int";
            break;
        case MISMATCHED_TYPE_FOR_ASSIGNMENT:
            cout << "expression type (" << details[0] << ") does not match variable's type (" << details[1] << ")";
            break;
        case EXPRESSION_TYPES_DO_NOT_MATCH:
            cout << details[2] << " of type " << details[3] << " does not match " << details[0] << " of type " << details[1] << endl;
            cout << "Hint: Try casting " << details[2] << " to " << details[1] << " using (" << details[2] << ")$" << details[1];
            break;
        case UNDEFINED_FUNCTION:
            cout << "function call for '" << details[0] << "(" << details[1]  << ")' does not match any defined function";
            break;
        case FUNCTION_WITH_RETURN_TYPE_ALREADY_DEFINED:
            cout << "function '" << details[0] << "' with parameters (" << details[1] << ") is already defined";
            break;
        case NULL_CANNOT_BE_USED_IN_OPERATION:
            cout << "null cannot be used in >, <, >=, <= comparisons";
            break;
        case INCORRECT_NUMBER_OF_PARAMETERS_FOR_MAIN:
            cout << "function 'main' cannot have any parameters listed (" << details[0] << " were added)";
            break;
        case INVALID_RETURN_TYPE_FOR_MAIN:
            cout << "function main() can only be of type int or void";
            break;
        case RETURN_VALUE_FOR_FUNCTION_DOES_NOT_MATCH:
            cout << "expression in return statement has type: " << details[0] << ", which does not match the function's return type: " << details[1];
            break;
        case CONDITION_CANNOT_BE_EMPTY:
            cout << "if and else if conditions cannot be empty";
            break;
        case CONDITION_MUST_BE_BOOLEAN_EXPRESSION:
            cout << "conditions must be a boolean expression (expression is of type " << details[0] << ")";
            break;
        case UNEXPECTED_ELSE_ELSE_IF:
            cout << "unexpected else or else if. If statement should be placed before this.";
            break;
        case OTHER: // Other error that is not covered. Eg. Critical internal error
            cout << details[0];
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
            default:
                cout << "(No error code for this. Enum value is " << error_type << ")";
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

void PrseDoc::explain(
    Error::Error_type error_type,
    Warning::Warning_type warning_type
) {
    if (error_type != -1 && warning_type == -1) {
        switch (error_type) {
            case Error::INVALID_USE_FILE_SPECIFIED:
                cout << "'use' statements expect both the name of the file and the .prse extension, like so:" << endl;
                cout << "use \"foo.prse\"" << endl;
                break;
            default:
                cout << "Nothing here yet." << endl;
                break;
        }
    }
    else if (warning_type != -1 && error_type == -1) {
        switch (warning_type) {
            default:
                cout << "No documentation available for Warning_type " << warning_type << endl;
                break;
        }
    }
    else {
        cout << "How did you get here? I checked things beforehand, didn't I?" << endl;
    }
}