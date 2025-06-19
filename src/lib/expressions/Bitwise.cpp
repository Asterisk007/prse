#include "Bitwise.h"
#include "Expression_core.h"
#include "Valid_math_types.h"

const Constant* Xor::as_const() const {
    if (lhs != nullptr && rhs != nullptr){
        auto l = lhs->as_const();
        auto r = rhs->as_const();
        if(!(l->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "Bitwise Xor", line);
        }
        if (!(r->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "rhs", prse_type_to_string(l->type()), "Bitwise Xor", line);
        }
        string emit  = l->value() + " ^ " + r->value();
        return ret(new Constant(l->type(), emit));
    }
        else{
            return ret(new Constant(PRSE_type::NO_TYPE,""));
        }
}

string Xor::value() const {
    if (lhs != nullptr && rhs != nullptr){
        auto l = lhs->as_const();
        auto r = rhs->as_const();
        if(!(l->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "Bitwise Xor", line);
        }
        if (!(r->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "rhs", prse_type_to_string(l->type()), "Bitwise Xor", line);
        }
        string emit  = l->value() + " ^ " + r->value();
        return emit;
    }
        else{
            return "";
        }
}
vector<const Constant*>Xor::as_list() const {
    vector<const Constant*> cl = vector<const Constant*>();
    if (lhs != nullptr && rhs != nullptr){
        auto l = lhs->as_const();
        auto r = rhs->as_const();
        if(!(l->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "Bitwise Xor", line);
        }
        if (!(r->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "rhs", prse_type_to_string(l->type()), "Bitwise Xor", line);
        }
        string emit  = l->value() + " ^ " + r->value();
        cl.push_back(ret(new Constant(l->type(), emit)));
    }
    return cl;
}