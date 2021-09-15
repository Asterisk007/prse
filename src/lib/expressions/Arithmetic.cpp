#include "Arithmetic.h"
#include "Expression_core.h"
#include "Valid_math_types.h"

// Decrement
Decrement::Decrement(int line, const Expression* lhs)
:Unary_operator(lhs), line(line){}

const Constant* Decrement::as_const() const {
    if (lhs != nullptr){
        auto l = lhs->as_const();
        if (!(l->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "divide", line);
        }
        string ret_s = l->value();
        ret_s += "--";
        return ret(new Constant(l->type(), ret_s));
    } else {
        return ret(new Constant(PRSE_type::NO_TYPE, ""));
    }
}

string Decrement::value() const {
    if (lhs != nullptr){
        auto l = lhs->as_const();
        if (!(l->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "divide", line);
        }
        string ret_s = l->value();
        ret_s += "--";
        return ret_s;
    } else {
        return "";
    }
}

vector<const Constant*> Decrement::as_list() const {
    vector<const Constant*> cl = vector<const Constant*>();
    if (lhs != nullptr){
        auto l = lhs->as_const();
        if (!(l->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "divide", line);
        }
        string t = l->value();
        t += "--";
        cl.push_back(ret(new Constant(l->type(), t)));
    }
    return cl;
}

// Divide
const Constant* Divide::as_const() const {
    if (lhs != nullptr && rhs != nullptr) {
        auto l = lhs->as_const();
        auto r = rhs->as_const();
        if (!(l->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "divide", line);
        }
        if (!(r->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "rhs", prse_type_to_string(l->type()), "divide", line);
        }
        string t = l->value();
        t += " / "; t += r->value();
        return ret(new Constant(l->type(), t));
    } else {
        return ret(new Constant(PRSE_type::NO_TYPE, ""));
    }
}

string Divide::value() const {
    if (lhs != nullptr && rhs != nullptr) {
        auto l = lhs->as_const();
        auto r = rhs->as_const();
        if (!(l->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "divide", line);
        }
        if (!(r->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "rhs", prse_type_to_string(l->type()), "divide", line);
        }
        string t = l->value();
        t += " / "; t += r->value();
        return t;
    } else {
        return "";
    }
}

vector<const Constant*> Divide::as_list() const {
    vector<const Constant*> cl = vector<const Constant*>();
    if (lhs != nullptr && rhs != nullptr) {
        auto l = lhs->as_const();
        auto r = rhs->as_const();
        if (!(l->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "divide", line);
        }
        if (!(r->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "rhs", prse_type_to_string(l->type()), "divide", line);
        }
        string t = l->value();
        t += " / "; t += r->value();
        vector<const Constant*> cl;
        cl.push_back(ret(new Constant(l->type(), t)));
    }
    return cl;
}

// Increment
Increment::Increment(int line, const Expression* lhs)
:Unary_operator(lhs), line(line){}

const Constant* Increment::as_const() const {
    if (lhs != nullptr){
        auto l = lhs->as_const();
        if (!(l->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "divide", line);
        }
        string ret_s = l->value();
        ret_s += "++";
        return ret(new Constant(l->type(), ret_s));
    } else {
        return ret(new Constant(PRSE_type::NO_TYPE, ""));
    }
}

string Increment::value() const {
    if (lhs != nullptr){
        auto l = lhs->as_const();
        if (!(l->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "divide", line);
        }
        string ret_s = l->value();
        ret_s += "++";
        return ret_s;
    } else {
        return "";
    }
}

vector<const Constant*> Increment::as_list() const {
    vector<const Constant*> cl = vector<const Constant*>();
    if (lhs != nullptr){
        auto l = lhs->as_const();
        if (!(l->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "divide", line);
        }
        string t = l->value();
        t += "++";
        cl.push_back(ret(new Constant(l->type(), t)));
    }
    return cl;
}

// Minus
const Constant* Minus::as_const() const {
    if (lhs != nullptr && rhs != nullptr) {
        auto l = lhs->as_const();
        auto r = rhs->as_const();
        if (!(l->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "divide", line);
        }
        if (!(r->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "rhs", prse_type_to_string(l->type()), "divide", line);
        }
        string t = l->value();
        t += " - "; t += r->value();
        return ret(new Constant(l->type(), t));
    } else {
        return ret(new Constant(PRSE_type::NO_TYPE, ""));
    }
}

string Minus::value() const{
    if (lhs != nullptr && rhs != nullptr) {
        auto l = lhs->as_const();
        auto r = rhs->as_const();
        if (!(l->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "divide", line);
        }
        if (!(r->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "rhs", prse_type_to_string(l->type()), "divide", line);
        }
        string t = l->value();
        t += " - "; t += r->value();
        return t;
    } else {
        return "";
    }
}

vector<const Constant*> Minus::as_list() const {
    vector<const Constant*> cl = vector<const Constant*>();
    if (lhs != nullptr && rhs != nullptr) {
        auto l = lhs->as_const();
        auto r = rhs->as_const();
        if (!(l->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "divide", line);
        }
        if (!(r->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "rhs", prse_type_to_string(l->type()), "divide", line);
        }
        string t = l->value();
        t += " - "; t += r->value();
        vector<const Constant*> cl;
        cl.push_back(ret(new Constant(l->type(), t)));
    }
    return cl;
}

// Modulo
const Constant* Modulo::as_const() const {
    if (lhs != nullptr && rhs != nullptr) {
        auto l = lhs->as_const();
        auto r = rhs->as_const();
        if (!(l->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "divide", line);
        }
        if (!(r->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "rhs", prse_type_to_string(l->type()), "divide", line);
        }
        string t = l->value();
        t += " % "; t += r->value();
        return ret(new Constant(l->type(), t));
    } else {
        return ret(new Constant(PRSE_type::NO_TYPE, ""));
    }
}

string Modulo::value() const {
    return "";
}

vector<const Constant*> Modulo::as_list() const {
    vector<const Constant*> cl = vector<const Constant*>();
    return cl;
}

// Multiply
const Constant* Multiply::as_const() const {
    if (lhs != nullptr && rhs != nullptr) {
        auto l = lhs->as_const();
        auto r = rhs->as_const();
        if (!(l->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "divide", line);
        }
        if (!(r->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "rhs", prse_type_to_string(l->type()), "divide", line);
        }
        string t = l->value();
        t += " * "; t += r->value();
        return ret(new Constant(l->type(), t));
    } else {
        return ret(new Constant(PRSE_type::NO_TYPE, ""));
    }
}

string Multiply::value() const{
    if (lhs != nullptr && rhs != nullptr) {
        auto l = lhs->as_const();
        auto r = rhs->as_const();
        if (!(l->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "divide", line);
        }
        if (!(r->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "rhs", prse_type_to_string(l->type()), "divide", line);
        }
        string t = l->value();
        t += " * "; t += r->value();
        return t;
    } else {
        return "";
    }
}

vector<const Constant*> Multiply::as_list() const {
    vector<const Constant*> cl = vector<const Constant*>();
    if (lhs != nullptr && rhs != nullptr) {
        auto l = lhs->as_const();
        auto r = rhs->as_const();
        if (!(l->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "divide", line);
        }
        if (!(r->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "rhs", prse_type_to_string(l->type()), "divide", line);
        }
        string t = l->value();
        t += " * "; t += r->value();
        vector<const Constant*> cl;
        cl.push_back(ret(new Constant(l->type(), t)));
    }
    return cl;
}

// Plus
const Constant* Plus::as_const() const {
    if (lhs != nullptr && rhs != nullptr) {
        auto l = lhs->as_const();
        auto r = rhs->as_const();
        if (l->type() != r->type()){
            // TODO: might need changing
            Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()), "", line);
        }
        string t = l->value();
        t += " + "; t += r->value();
        return ret(new Constant(l->type(), t));
    } else {
        return ret(new Constant(PRSE_type::NO_TYPE, ""));
    }
}

string Plus::value() const {
    if (lhs != nullptr && rhs != nullptr) {
        auto l = lhs->as_const();
        auto r = rhs->as_const();
        if (l->type() != r->type()){
            // TODO: might need changing
            Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()), "", line);
        }
        string t = l->value();
        t += " + "; t += r->value();
        return t;
    } else {
        return "";
    }
}

vector<const Constant*> Plus::as_list() const {
    vector<const Constant*> cl = vector<const Constant*>();
    if (lhs != nullptr && rhs != nullptr) {
        auto l = lhs->as_const();
        auto r = rhs->as_const();
        if (l->type() != r->type()){
            Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
        }
        string t = l->value();
        t += " + "; t += r->value();
        cl.push_back(ret(new Constant(l->type(), t)));
    }
    return cl;
}
