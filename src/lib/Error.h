#pragma once

// This class handles various errors that PRSE encounters in a file
class Error {
public:
    enum Error_type {
        VARIABLE_DECLARED_BUT_NOT_SET,
        VARIABLE_NOT_DEFINED,
        UNDEFINED_FUNCTION
    };
    static void error(Error_type error_type);
    static int num_errors();
protected:
    static int error_count;
};

// This class warns the programmer about unsafe programming practices.
class Warning {
public:
    enum Warning_type {
        VARIABLE_TYPE_IMPLICIT,
        VARIABLE_DECLARED_BUT_NOT_USED
    };
    static void warning(Warning_type warning_type);
    static int num_warnings();
protected:
    static int warning_count;
};