#pragma once
#include <string>

using namespace std;

enum PRSE_type {
    NO_TYPE = 0,
    T_VOID,
    T_BOOL,
    T_INT,
    T_DOUBLE,
    T_CHAR,
    T_STRING, // regular string
    T_I_STRING // interpolated string, starts with the '$' character
};

string prse_type_to_string(PRSE_type prse_type);
PRSE_type prse_type_from_string(const string& type);