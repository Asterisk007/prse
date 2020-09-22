#include "Expression.h"

Expression::Expression()
:expr_type(PRSE_type::NO_TYPE), list(vector<string>(0)){}

Expression::Expression(PRSE_type& type, vector<string>& list)
:expr_type(type), list(list){}

bool Expression::prepend_expression(PRSE_type& type, string& expr){
    // First, if these types are the same, they are automatically compatible.
    if (type == expr_type) {
        list.insert(list.begin(), expr);
    }
    // However, if they are different, we have to check if
    // we can convert one to the other.
    else {
        switch (expr_type){
            case T_BOOL:
                break;
            case T_INT:
                break;
            case T_DOUBLE:
                break;
            case T_CHAR:
                break;
            case T_STRING:
                break;
            case T_I_STRING:
                break;
        }
    }
}

bool Expression::append_expression(PRSE_type& type, string& expr){
    // First, if these types are the same, they are automatically compatible.
    if (type == expr_type) {
        list.push_back(expr);
        return true;
    }
    // However, if they are different, we have to check if
    // we can convert one to the other.
    else {
        return false;
        /* TODO: type conversion
        switch (expr_type){
            case T_BOOL:
                if (type == T_INT){

                }
                break;
            case T_INT:
                break;
            case T_DOUBLE:
                break;
            case T_CHAR:
                break;
            case T_STRING:
                break;
            case T_I_STRING:
                break;
        }*/
    }
}