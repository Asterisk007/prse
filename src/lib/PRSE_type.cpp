#include "PRSE_type.h"

string prse_type_to_string(PRSE_type prse_type){
    switch (prse_type) {
        case NO_TYPE:
            return "no type";
        case T_VOID:
            return "void";
        case T_BOOL:
            return "bool";
        case T_INT:
            return "int";
        case T_DOUBLE:
            return "double";
        case T_CHAR:
            return "char";
        case T_STRING:
        case T_I_STRING:
            return "std::string";
    }
    return "";
}