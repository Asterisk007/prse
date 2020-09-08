#pragma once

class Error {
public:
    enum Error_type {
        VARIABLE_DECLARED_BUT_NOT_SET,
        VARIABLE_NOT_DEFINED,
        UNDEFINED_FUNCTION
    };
    static void error(Error_type error_type, int line_number);
    static int num_errors();
protected:
    static int error_count;
};