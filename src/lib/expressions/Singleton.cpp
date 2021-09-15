#include "Singleton.h"
#include "Expression_core.h"

// Singleton class
Singleton::Singleton(const Expression* lhs)
:Unary_operator(lhs){}

const Constant* Singleton::as_const() const {
    if (lhs != nullptr){
        auto lhs_const = lhs->as_const();
        return ret(new Constant(lhs_const->type(), string(lhs_const->value() + ";\n")));
    } else {
        return ret(new Constant(PRSE_type::NO_TYPE, ""));
    }
}

vector<const Constant*> Singleton::as_list() const {
    vector<const Constant*> return_list = vector<const Constant*>();
    if (lhs != nullptr){
        auto lhs_const = lhs->as_const();
        return_list.push_back(new Constant(lhs_const->type(), string(lhs_const->value() + ";\n")));
    } else {
        return_list.push_back(new Constant(PRSE_type::NO_TYPE, ";\n"));
    }
    return return_list;
}

string Singleton::value() const {
    return "";
}