#include "Constant.h"

Constant::Constant()
:type(PRSE_type::NO_TYPE), value(""), value_list(vector<string>(0)){}

Constant::Constant(PRSE_type type, string value)
:type(type), value(value), value_list(vector<string>(0)){}

Constant::Constant(PRSE_type type, vector<string> value_list)
:type(type), value(""), value_list(value_list){}

Constant::~Constant(){}

PRSE_type Constant::get_type(){
    return type;
}

string Constant::get_value(){
    return value;
}

vector<string> Constant::get_value_list(){
    return value_list;
}