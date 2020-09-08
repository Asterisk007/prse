#pragma once

#include "PRSE_type.h"

class Constant {
private:
    PRSE_type type;
    union Value {
        bool *bool_pointer;
        int *int_pointer;
        char *char_pointer;
        double *double_pointer;
        std::string *string_pointer;
        // TODO: Add support for complex types (i.e. classes)

        Value(bool *val) : bool_pointer(val) {}
        Value(int *val) : int_pointer(val) {}
        Value(char *val) : char_pointer(val) {}
        Value(double *val) : double_pointer(val) {}
        Value(std::string *val) : string_pointer(val) {}
        // TODO: Add void* type here
    };
public:
    PRSE_type get_type();
    Value get_value();
};