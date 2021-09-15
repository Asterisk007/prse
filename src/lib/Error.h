#pragma once
#include <string>

extern int line_count;

using namespace std;

// This class handles various errors that PRSE encounters in a file
class Error {
public:
    enum Error_type {
        VARIABLE_DECLARED_BUT_NOT_SET,
        VARIABLE_NOT_DEFINED_IN_SCOPE,
        VARIABLE_ALREADY_DECLARED,
        VARIABLE_IS_NOT_AN_ARRAY,
        ARRAY_SUBSCRIPT_MUST_BE_AN_INTEGER_VALUE,
        IMPLICIT_TYPE_CANNOT_BE_ASSIGNED_NULL,
        INVALID_TYPE_FOR_VARIABLE,
        INVALID_EXPRESSION_TYPE_FOR_OPERATION,
        EXPRESSION_TYPES_DO_NOT_MATCH,
        INVALID_EXPRESSION_TYPE_FOR_ARRAY_SIZE,
        MISMATCHED_TYPE_FOR_ASSIGNMENT,
        UNDEFINED_FUNCTION,
        FUNCTION_WITH_RETURN_TYPE_ALREADY_DEFINED,
        NULL_CANNOT_BE_USED_IN_OPERATION,
        INCORRECT_NUMBER_OF_PARAMETERS_FOR_MAIN,
        INVALID_RETURN_TYPE_FOR_MAIN,
        RETURN_VALUE_FOR_FUNCTION_DOES_NOT_MATCH,
        CONDITION_CANNOT_BE_EMPTY,
        CONDITION_MUST_BE_BOOLEAN_EXPRESSION,
        UNEXPECTED_ELSE_ELSE_IF,
        OTHER
    };
    static void error(
        Error_type error_type,
        const string& s1 = "",
        const string& s2 = "",
        const string& s3 = "",
        const int line_override = line_count
    );
    static int num_errors();
    static bool print_error_codes;
protected:
    static int error_count;
};

// This class warns the programmer about unsafe programming practices.
class Warning {
public:
    enum Warning_type {
        VARIABLE_TYPE_IMPLICIT,
        VARIABLE_DECLARED_BUT_NOT_USED,
        VARIABLE_DECLARATION_USELESS,
        EXPRESSION_CAST_INVALID
    };
    static void warning(
        Warning_type warning_type,
        const string& s1 = "",
        const string& s2 = "",
        const string& s3 = "",
        const int line_override = line_count
    );
    static int num_warnings();
    static bool show_warnings;
protected:
    static int warning_count;
};
