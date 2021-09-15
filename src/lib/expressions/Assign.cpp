#include "Assign.h"
#include "Expression_core.h"
#include "Valid_math_types.h"

// Plus asssign
const Constant* Plus_assign::as_const() const {
    if (lhs != nullptr && rhs != nullptr) {
        auto l = lhs->as_const();
        auto r = rhs->as_const();
        bool is_array = (Table_handler::instance().lookup(l->value())->get_size() > 0);
        if (l->type() != r->type()){
            Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
        }
        if (!(l->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "plus assign", line);
        }
        if (!(r->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "rhs", prse_type_to_string(l->type()), "plus assign", line);
        }

        string t = "";
        if (!is_array) {
            t = l->value();
            t += " += "; t += r->value(); t += ";\n";
        } else {
            string it_name = l->value() + "_iterator";
            t = "for (int " + it_name + " = 0; i < " + it_name + ".size(); " + it_name + "++){ ";
            t += l->value() + "[" + it_name + "] += " + r->value() + ";";
            t += "}";
        }
        return ret(new Constant(l->type(), t));
    } else {
        return ret(new Constant(PRSE_type::NO_TYPE, ""));
    }
}

string Plus_assign::value() const {
    if (lhs != nullptr && rhs != nullptr) {
        auto l = lhs->as_const();
        auto r = rhs->as_const();
        bool is_array = (Table_handler::instance().lookup(l->value())->get_size() > 0);
        if (l->type() != r->type()){
            Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
        }
        if (!(l->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "plus assign", line);
        }
        if (!(r->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "rhs", prse_type_to_string(l->type()), "plus assign", line);
        }

        string t = "";
        if (!is_array) {
            t = l->value();
            t += " += "; t += r->value(); t += ";\n";
        } else {
            string it_name = l->value() + "_iterator";
            t = "for (int " + it_name + " = 0; i < " + it_name + ".size(); " + it_name + "++){ ";
            t += l->value() + "+=" + r->value();
            t += "}";
        }
        return t;
    } else {
        return "";
    }
}

vector<const Constant*> Plus_assign::as_list() const {
    vector<const Constant*> cl = vector<const Constant*>();
    if (lhs != nullptr && rhs != nullptr) {
        auto l = lhs->as_const();
        auto r = rhs->as_const();
        if ( l->type() == (PRSE_type::T_CHAR | PRSE_type::T_VOID) ){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "+=", line);
        }
        if ( r->type() == (PRSE_type::T_CHAR | PRSE_type::T_VOID) ){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "rhs", prse_type_to_string(r->type()), "+=", line);
        }
        if (l->type() != r->type()){
            Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
        }
        string t = l->value();
        t += " += "; t += r->value(); t += ";\n";
        vector<const Constant*> cl;
        cl.push_back(ret(new Constant(l->type(), t)));
    }
    return cl;
}

// Minus asssign
const Constant* Minus_assign::as_const() const {
    if (lhs != nullptr && rhs != nullptr) {
        auto l = lhs->as_const();
        auto r = rhs->as_const();
        if (l->type() != r->type()){
            Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
        }
        string t = l->value();
        t += " -= "; t += r->value(); t += ";\n";
        return ret(new Constant(l->type(), t));
    } else {
        return ret(new Constant(PRSE_type::NO_TYPE, ""));
    }
}

string Minus_assign::value() const {
    if (lhs != nullptr && rhs != nullptr) {
        auto l = lhs->as_const();
        auto r = rhs->as_const();
        if (l->type() != r->type()){
            Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
        }
        string t = l->value();
        t += " -= "; t += r->value(); t += ";\n";
        return t;
    } else {
        return "";
    }
}

vector<const Constant*> Minus_assign::as_list() const {
    vector<const Constant*> cl = vector<const Constant*>();
    if (lhs != nullptr && rhs != nullptr) {
        auto l = lhs->as_const();
        auto r = rhs->as_const();
        if (l->type() != r->type()){
            Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
        }
        string t = l->value();
        t += " -= "; t += r->value(); t += ";\n";
        vector<const Constant*> cl;
        cl.push_back(ret(new Constant(l->type(), t)));
    }
    return cl;
}

// Multiply asssign
const Constant* Multiply_assign::as_const() const {
    if (lhs != nullptr && rhs != nullptr) {
        auto l = lhs->as_const();
        auto r = rhs->as_const();
        if (l->type() != r->type()){
            Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
        }
        string t = l->value();
        t += " *= "; t += r->value(); t += ";\n";
        return ret(new Constant(l->type(), t));
    } else {
        return ret(new Constant(PRSE_type::NO_TYPE, ""));
    }
}

string Multiply_assign::value() const {
    if (lhs != nullptr && rhs != nullptr) {
        auto l = lhs->as_const();
        auto r = rhs->as_const();
        if (l->type() != r->type()){
            Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
        }
        string t = l->value();
        t += " *= "; t += r->value(); t += ";\n";
        return t;
    } else {
        return "";
    }
}

vector<const Constant*> Multiply_assign::as_list() const {
    vector<const Constant*> cl = vector<const Constant*>();
    if (lhs != nullptr && rhs != nullptr) {
        auto l = lhs->as_const();
        auto r = rhs->as_const();
        if (l->type() != r->type()){
            Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
        }
        string t = l->value();
        t += " *= "; t += r->value(); t += ";\n";
        vector<const Constant*> cl;
        cl.push_back(ret(new Constant(l->type(), t)));
    }
    return cl;
}

// Divide assign
const Constant* Divide_assign::as_const() const {
    if (lhs != nullptr && rhs != nullptr) {
        auto l = lhs->as_const();
        auto r = rhs->as_const();
        if (l->type() != r->type()){
            Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
        }
        string t = l->value();
        t += " /= "; t += r->value(); t += ";\n";
        return ret(new Constant(l->type(), t));
    } else {
        return ret(new Constant(PRSE_type::NO_TYPE, ""));
    }
}

string Divide_assign::value() const{
    if (lhs != nullptr && rhs != nullptr) {
        auto l = lhs->as_const();
        auto r = rhs->as_const();
        if (l->type() != r->type()){
            Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
        }
        string t = l->value();
        t += " /= "; t += r->value(); t += ";\n";
        return t;
    } else {
        return "";
    }
}

vector<const Constant*> Divide_assign::as_list() const {
    vector<const Constant*> cl = vector<const Constant*>();
    if (lhs != nullptr && rhs != nullptr) {
        auto l = lhs->as_const();
        auto r = rhs->as_const();
        if (l->type() != r->type()){
            Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
        }
        string t = l->value();
        t += " /= "; t += r->value(); t += ";\n";
        vector<const Constant*> cl;
        cl.push_back(ret(new Constant(l->type(), t)));
    }
    return cl;
}