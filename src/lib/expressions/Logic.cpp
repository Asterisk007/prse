#include "Logic.h"
#include "Expression_core.h"
#include "Valid_math_types.h"

// Operator ==
const Constant* Logic_Eq::as_const() const {
    auto l = lhs->as_const();
    auto r = rhs->as_const();
    if (l->type() != r->type()){
        Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
    }
    string t = l->value();
    t += " == "; t += r->value();
    return ret(new Constant(PRSE_type::T_BOOL, t));
}

string Logic_Eq::value() const {
    auto l = lhs->as_const();
    auto r = rhs->as_const();
    if (l->type() != r->type()){
        Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
    }
    string t = l->value();
    t += " == "; t += r->value();
    return t;
}

vector<const Constant*> Logic_Eq::as_list() const {
    auto l = lhs->as_const();
    auto r = rhs->as_const();
    if (l->type() != r->type()){
        Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
    }
    string t = l->value();
    t += " == "; t += r->value();
    vector<const Constant*> cl;
    cl.push_back(ret(new Constant(PRSE_type::T_BOOL, t)));
    return cl;
}

//////////////////////////////////////////////////
// Operator !=
const Constant* Logic_NEq::as_const() const {
    auto l = lhs->as_const();
    auto r = rhs->as_const();
    if (l->type() != r->type()){
        Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
    }
    string t = l->value();
    t += " != "; t += r->value();
    return ret(new Constant(PRSE_type::T_BOOL, t));
}

string Logic_NEq::value() const {
    auto l = lhs->as_const();
    auto r = rhs->as_const();
    if (l->type() != r->type()){
        Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
    }
    string t = l->value();
    t += " != "; t += r->value();
    return t;
}

vector<const Constant*> Logic_NEq::as_list() const {
    auto l = lhs->as_const();
    auto r = rhs->as_const();
    if (l->type() != r->type()){
        Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
    }
    string t = l->value();
    t += " != "; t += r->value();
    vector<const Constant*> cl;
    cl.push_back(ret(new Constant(PRSE_type::T_BOOL, t)));
    return cl;
}

//////////////////////////////////////////////////
// Operator <=

const Constant* Logic_Less_Eq::as_const() const {
    auto l = lhs->as_const();
    auto r = rhs->as_const();
    if (l->type() != r->type()){
        Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
    }
    if (!(l->type() & VALID_MATH_TYPES)){
        Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "<=");
    }
    if (!(r->type() & VALID_MATH_TYPES)){
        Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "rhs", prse_type_to_string(l->type()), "<=");
    }
    string t = l->value();
    t += " <= "; t += r->value();
    return ret(new Constant(PRSE_type::T_BOOL, t));
}

string Logic_Less_Eq::value() const {
    auto l = lhs->as_const();
    auto r = rhs->as_const();
    if (l->type() != r->type()){
        Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
    }
    if (!(l->type() & VALID_MATH_TYPES)){
        Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "<=");
    }
    if (!(r->type() & VALID_MATH_TYPES)){
        Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "rhs", prse_type_to_string(l->type()), "<=");
    }
    string t = l->value();
    t += " <= "; t += r->value();
    return t;
}

vector<const Constant*> Logic_Less_Eq::as_list() const {
    auto l = lhs->as_const();
    auto r = rhs->as_const();
    if (l->type() != r->type()){
        Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
    }
    if (!(l->type() & VALID_MATH_TYPES)){
        Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "<=");
    }
    if (!(r->type() & VALID_MATH_TYPES)){
        Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "rhs", prse_type_to_string(l->type()), "<=");
    }
    string t = l->value();
    t += " <= "; t += r->value();
    vector<const Constant*> cl;
    cl.push_back(ret(new Constant(PRSE_type::T_BOOL, t)));
    return cl;
}

//////////////////////////////////////////////////
// Operator >=

const Constant* Logic_Greater_Eq::as_const() const {
    auto l = lhs->as_const();
    auto r = rhs->as_const();
    if (l->type() != r->type()){
        Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
    }
    if (!(l->type() & VALID_MATH_TYPES)){
        Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), ">=");
    }
    if (!(r->type() & VALID_MATH_TYPES)){
        Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "rhs", prse_type_to_string(l->type()), ">=");
    }
    string t = l->value();
    t += " >= "; t += r->value();
    return ret(new Constant(PRSE_type::T_BOOL, t));
}

string Logic_Greater_Eq::value() const {
    auto l = lhs->as_const();
    auto r = rhs->as_const();
    if (l->type() != r->type()){
        Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
    }
    if (!(l->type() & VALID_MATH_TYPES)){
        Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), ">=");
    }
    if (!(r->type() & VALID_MATH_TYPES)){
        Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "rhs", prse_type_to_string(l->type()), ">=");
    }
    string t = l->value();
    t += " >= "; t += r->value();
    return t;
}

vector<const Constant*> Logic_Greater_Eq::as_list() const {
    auto l = lhs->as_const();
    auto r = rhs->as_const();
    if (l->type() != r->type()){
        Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
    }
    if (!(l->type() & VALID_MATH_TYPES)){
        Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), ">=");
    }
    if (!(r->type() & VALID_MATH_TYPES)){
        Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "rhs", prse_type_to_string(l->type()), ">=");
    }
    string t = l->value();
    t += " >= "; t += r->value();
    vector<const Constant*> cl;
    cl.push_back(ret(new Constant(PRSE_type::T_BOOL, t)));
    return cl;
}

//////////////////////////////////////////////////
// Operator <

const Constant* Logic_Less_Than::as_const() const {
    auto l = lhs->as_const();
    auto r = rhs->as_const();
    if (l->type() != r->type()){
        Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
    }
    if (!(l->type() & VALID_MATH_TYPES)){
        Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "<");
    }
    if (!(r->type() & VALID_MATH_TYPES)){
        Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "rhs", prse_type_to_string(l->type()), "<");
    }
    string t = l->value();
    t += " < "; t += r->value();
    return ret(new Constant(PRSE_type::T_BOOL, t));
}

string Logic_Less_Than::value() const {
    auto l = lhs->as_const();
    auto r = rhs->as_const();
    if (l->type() != r->type()){
        Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
    }
    if (!(l->type() & VALID_MATH_TYPES)){
        Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "<");
    }
    if (!(r->type() & VALID_MATH_TYPES)){
        Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "rhs", prse_type_to_string(l->type()), "<");
    }
    string t = l->value();
    t += " < "; t += r->value();
    return t;
}

vector<const Constant*> Logic_Less_Than::as_list() const {
    auto l = lhs->as_const();
    auto r = rhs->as_const();
    if (l->type() != r->type()){
        Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
    }
    if (!(l->type() & VALID_MATH_TYPES)){
        Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "<");
    }
    if (!(r->type() & VALID_MATH_TYPES)){
        Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "rhs", prse_type_to_string(l->type()), "<");
    }
    string t = l->value();
    t += " < "; t += r->value();
    vector<const Constant*> cl;
    cl.push_back(ret(new Constant(PRSE_type::T_BOOL, t)));
    return cl;
}

//////////////////////////////////////////////////
// Operator >

const Constant* Logic_Greater_Than::as_const() const {
    auto l = lhs->as_const();
    auto r = rhs->as_const();
    if (l->type() != r->type()){
        Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
    }
    if (!(l->type() & VALID_MATH_TYPES)){
        Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), ">");
    }
    if (!(r->type() & VALID_MATH_TYPES)){
        Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "rhs", prse_type_to_string(l->type()), ">");
    }
    string t = l->value();
    t += " > "; t += r->value();
    return ret(new Constant(PRSE_type::T_BOOL, t));
}

string Logic_Greater_Than::value() const {
    auto l = lhs->as_const();
    auto r = rhs->as_const();
    if (l->type() != r->type()){
        Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
    }
    if (!(l->type() & VALID_MATH_TYPES)){
        Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), ">");
    }
    if (!(r->type() & VALID_MATH_TYPES)){
        Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "rhs", prse_type_to_string(l->type()), ">");
    }
    string t = l->value();
    t += " > "; t += r->value();
    return t;
}

vector<const Constant*> Logic_Greater_Than::as_list() const {
    auto l = lhs->as_const();
    auto r = rhs->as_const();
    if (l->type() != r->type()){
        Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
    }
    if (!(l->type() & VALID_MATH_TYPES)){
        Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), ">");
    }
    if (!(r->type() & VALID_MATH_TYPES)){
        Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "rhs", prse_type_to_string(l->type()), ">");
    }
    string t = l->value();
    t += " > "; t += r->value();
    vector<const Constant*> cl;
    cl.push_back(ret(new Constant(PRSE_type::T_BOOL, t)));
    return cl;
}

//////////////////////////////////////////////////
// Operator && (AND)

const Constant* Logic_And::as_const() const {
    auto l = lhs->as_const();
    auto r = rhs->as_const();
    if (l->type() != r->type()){
        Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
    }
    if (!(l->type() & PRSE_type::T_BOOL)){
        Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "&&");
    }
    if (!(r->type() & PRSE_type::T_BOOL)){
        Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "rhs", prse_type_to_string(l->type()), "&&");
    }
    string t = l->value();
    t += " && "; t += r->value();
    return ret(new Constant(PRSE_type::T_BOOL, t));
}

string Logic_And::value() const {
    auto l = lhs->as_const();
    auto r = rhs->as_const();
    if (l->type() != r->type()){
        Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
    }
    if (!(l->type() & PRSE_type::T_BOOL)){
        Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "&&");
    }
    if (!(r->type() & PRSE_type::T_BOOL)){
        Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "rhs", prse_type_to_string(l->type()), "&&");
    }
    string t = l->value();
    t += " && "; t += r->value();
    return t;
}

vector<const Constant*> Logic_And::as_list() const {
    auto l = lhs->as_const();
    auto r = rhs->as_const();
    if (l->type() != r->type()){
        Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
    }
    if (!(l->type() & PRSE_type::T_BOOL)){
        Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "&&");
    }
    if (!(r->type() & PRSE_type::T_BOOL)){
        Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "rhs", prse_type_to_string(l->type()), "&&");
    }
    string t = l->value();
    t += " && "; t += r->value();
    vector<const Constant*> cl;
    cl.push_back(ret(new Constant(PRSE_type::T_BOOL, t)));
    return cl;
}

//////////////////////////////////////////////////
// Operator || (OR)

const Constant* Logic_Or::as_const() const {
    auto l = lhs->as_const();
    auto r = rhs->as_const();
    if (l->type() != r->type()){
        Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
    }
    if (!(l->type() & PRSE_type::T_BOOL)){
        Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "||");
    }
    if (!(r->type() & PRSE_type::T_BOOL)){
        Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "rhs", prse_type_to_string(l->type()), "||");
    }
    string t = l->value();
    t += " || "; t += r->value();
    return ret(new Constant(PRSE_type::T_BOOL, t));
}

string Logic_Or::value() const{
    auto l = lhs->as_const();
    auto r = rhs->as_const();
    if (l->type() != r->type()){
        Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
    }
    if (!(l->type() & PRSE_type::T_BOOL)){
        Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "||");
    }
    if (!(r->type() & PRSE_type::T_BOOL)){
        Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "rhs", prse_type_to_string(l->type()), "||");
    }
    string t = l->value();
    t += " || "; t += r->value();
    return t;
}

vector<const Constant*> Logic_Or::as_list() const {
    auto l = lhs->as_const();
    auto r = rhs->as_const();
    if (l->type() != r->type()){
        Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
    }
    if (!(l->type() & PRSE_type::T_BOOL)){
        Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "||");
    }
    if (!(r->type() & PRSE_type::T_BOOL)){
        Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "rhs", prse_type_to_string(l->type()), "||");
    }
    string t = l->value();
    t += " || "; t += r->value();
    vector<const Constant*> cl;
    cl.push_back(ret(new Constant(PRSE_type::T_BOOL, t)));
    return cl;
}