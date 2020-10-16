#include "PRSE_type.h"

string prse_type_to_string(PRSE_type prse_type){
    switch (prse_type) {
        case NO_TYPE:
            return "no type";
        case T_VOID:
            return "void";
        case T_BOOL:
            return "boolean";
        case T_INT:
            return "int";
        case T_DOUBLE:
            return "double";
        case T_CHAR:
            return "character";
        case T_STRING:
        case T_I_STRING:
            return "string";
    }
}

PRSE_type prse_type_from_string(const string& type){
    if (type == "bool"){
        return T_BOOL;
    } else if (type == "void"){
        return T_VOID;
    } else if (type == "int"){
        return T_INT;
    } else if (type == "double"){
        return T_DOUBLE;
    } else if (type == "char"){
        return T_CHAR;
    } else if (type == "string"){
        return T_STRING;
    } else {
        return NO_TYPE;
    }
}