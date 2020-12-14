#include "Expression.h"
#include "Constant.h"
#include "Table_handler.h"
#include "Declared_list.h"
#include "Error.h"
#include "PRSE_stdlib.h"
#include <iostream>

PRSE_type Function_definition_expr::current_return_type = PRSE_type::NO_TYPE;
bool Function_definition_expr::is_main = false;

int valid_math_types = (PRSE_type::T_DOUBLE | PRSE_type::T_INT);

Expression::~Expression(){ delete expr_val; }

const Constant* Expression::ret(const Constant* new_constant) const {
    delete expr_val;
    return expr_val = new_constant;
}

// Variables
Variable::Variable(int line, const string symbol_name)
:line(line), symbol(symbol_name){}

const Constant* Variable::as_const() const {
    Table_handler& th = Table_handler::instance();
    //cerr << "Looking up symbol: " << symbol << endl;
    auto sym = th.lookup(symbol);
    if (sym != nullptr){
        return ret(new Constant(sym->get_type(), sym->get_name()));
    } else {
        Error::error(Error::VARIABLE_NOT_DEFINED_IN_SCOPE, symbol, "", "", line);
    }
    return ret(new Constant(PRSE_type::NO_TYPE, ""));
}

// Arrays
Array::Array(int line, const string s)
:Variable(line, s){}

const Constant* Array::as_const() const {
    Table_handler& th = Table_handler::instance();
    auto sym = th.lookup(symbol);
    if (sym != nullptr){
        if (sym->get_size() == 0) {
            Error::error(Error::VARIABLE_IS_NOT_AN_ARRAY, symbol, "", "", line);
        }
        else return ret(new Constant(sym->get_type(), sym->get_name()));
    } else {
        Error::error(Error::VARIABLE_NOT_DEFINED_IN_SCOPE, symbol, "", "", line);
    }
    return ret(new Constant(PRSE_type::NO_TYPE, ""));
}

// Variable casting
Cast::Cast(int line, const Expression* lhs, PRSE_type cast_to)
:line(line), Unary_operator(lhs), cast_type(cast_to){}

const Constant* Cast::as_const() const {
    /*
        Casting chart:
        bool ==> int, double, char, string
        int ==> string, double, bool (Values that are exactly equal to 0, such as 0, 0.0, and so on, are interpreted as false. Values higher than 0 are interpreted as true.)
        double ==> string, int, bool (ditto as above)
        char ==> string, int, double
        string ==> string, int, double
     */
    string ret_s = "";
    if (lhs != nullptr){
        switch (cast_type){
            case T_BOOL:
                ret_s = "static_cast<bool>(";
                ret_s += lhs->as_const()->value();
                ret_s += ")";
                break;
            case T_INT:
                ret_s = "static_cast<int>(";
                ret_s += lhs->as_const()->value();
                ret_s += ")";
                break;
            case T_DOUBLE:
                ret_s = "static_cast<double>(";
                ret_s += lhs->as_const()->value();
                ret_s += ")";
                break;
            case T_CHAR:
                ret_s = "static_cast<char>(";
                ret_s += lhs->as_const()->value();
                ret_s += ")";
                break;
            case T_STRING:
                ret_s = "string(";
                ret_s += lhs->as_const()->value();
                ret_s += ")";
                break;
            default:
                cerr << "Warning on line " << line << ": this operation is currently not supported. The original expression will be used." << endl;
                ret_s = lhs->as_const()->value();
                break;
        }
    }
    return ret(new Constant(cast_type, ret_s));
}

string Cast::value() const {
    return "";
}

vector<const Constant*> Cast::as_list() const {
    return vector<const Constant*>(0);
}

// Increment
Increment::Increment(int line, const Expression* lhs)
:line(line), Unary_operator(lhs){}

const Constant* Increment::as_const() const {
    if (lhs != nullptr){
        auto l = lhs->as_const();
        if (!(l->type() & valid_math_types)){
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
        if (!(l->type() & valid_math_types)){
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
        if (!(l->type() & valid_math_types)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "divide", line);
        }
        string t = l->value();
        t += "++";
        cl.push_back(ret(new Constant(l->type(), t)));
    }
    return cl;
}

// Decrement
Decrement::Decrement(int line, const Expression* lhs)
:line(line), Unary_operator(lhs){}

const Constant* Decrement::as_const() const {
    if (lhs != nullptr){
        auto l = lhs->as_const();
        if (!(l->type() & valid_math_types)){
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
        if (!(l->type() & valid_math_types)){
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
        if (!(l->type() & valid_math_types)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "divide", line);
        }
        string t = l->value();
        t += "--";
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
            Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
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
            Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
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

// Minus
const Constant* Minus::as_const() const {
    if (lhs != nullptr && rhs != nullptr) {
        auto l = lhs->as_const();
        auto r = rhs->as_const();
        if (!(l->type() & valid_math_types)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "divide", line);
        }
        if (!(r->type() & valid_math_types)){
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
        if (!(l->type() & valid_math_types)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "divide", line);
        }
        if (!(r->type() & valid_math_types)){
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
        if (!(l->type() & valid_math_types)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "divide", line);
        }
        if (!(r->type() & valid_math_types)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "rhs", prse_type_to_string(l->type()), "divide", line);
        }
        string t = l->value();
        t += " - "; t += r->value();
        vector<const Constant*> cl;
        cl.push_back(ret(new Constant(l->type(), t)));
    }
    return cl;
}

// Multiply
const Constant* Multiply::as_const() const {
    if (lhs != nullptr && rhs != nullptr) {
        auto l = lhs->as_const();
        auto r = rhs->as_const();
        if (!(l->type() & valid_math_types)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "divide", line);
        }
        if (!(r->type() & valid_math_types)){
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
        if (!(l->type() & valid_math_types)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "divide", line);
        }
        if (!(r->type() & valid_math_types)){
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
        if (!(l->type() & valid_math_types)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "divide", line);
        }
        if (!(r->type() & valid_math_types)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "rhs", prse_type_to_string(l->type()), "divide", line);
        }
        string t = l->value();
        t += " * "; t += r->value();
        vector<const Constant*> cl;
        cl.push_back(ret(new Constant(l->type(), t)));
    }
    return cl;
}

// Divide
const Constant* Divide::as_const() const {
    if (lhs != nullptr && rhs != nullptr) {
        auto l = lhs->as_const();
        auto r = rhs->as_const();
        if (!(l->type() & valid_math_types)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "divide", line);
        }
        if (!(r->type() & valid_math_types)){
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
        if (!(l->type() & valid_math_types)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "divide", line);
        }
        if (!(r->type() & valid_math_types)){
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
        if (!(l->type() & valid_math_types)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "divide", line);
        }
        if (!(r->type() & valid_math_types)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "rhs", prse_type_to_string(l->type()), "divide", line);
        }
        string t = l->value();
        t += " / "; t += r->value();
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
        if (!(l->type() & valid_math_types)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "divide", line);
        }
        if (!(r->type() & valid_math_types)){
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

// Plus asssign
const Constant* Plus_assign::as_const() const {
    if (lhs != nullptr && rhs != nullptr) {
        auto l = lhs->as_const();
        auto r = rhs->as_const();
        if (l->type() != r->type()){
            Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
        }
        string t = l->value();
        t += " += "; t += r->value(); t += ";\n";
        return ret(new Constant(l->type(), t));
    } else {
        return ret(new Constant(PRSE_type::NO_TYPE, ""));
    }
}

string Plus_assign::value() const {
    if (lhs != nullptr && rhs != nullptr) {
        auto l = lhs->as_const();
        auto r = rhs->as_const();
        if (l->type() != r->type()){
            Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(l->type()), prse_type_to_string(r->type()));
        }
        string t = l->value();
        t += " += "; t += r->value(); t += ";\n";
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


// Equality
//===================================================
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
// Operator ==
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
    string t = l->value();
    t += " || "; t += r->value();
    vector<const Constant*> cl;
    cl.push_back(ret(new Constant(PRSE_type::T_BOOL, t)));
    return cl;
}

/////////////////////////////////////////////////
// Program expressions

// Function header
Function_header::Function_header(int line, string n, PRSE_type rt, vector<shared_ptr<Symbol>>& pl)
:line(line), name(n), return_type(rt), params(pl){}

const Constant* Function_header::as_const() const {
    Table_handler& th = Table_handler::instance();
    //cerr << "Pushing table" << endl;
    th.push_table();

    // The function header's return type
    string ret_s; 
    if (name == "main"){
        ret_s = "int";
        Function_definition_expr::is_main = true;
    } else {
        Function_definition_expr::is_main = false;
        ret_s = prse_type_to_string(return_type);
    }

    Function_definition_expr::current_return_type = return_type;

    // SPACE function name LEFT_PAREN
    ret_s += " "; ret_s += name; ret_s += "(";
    // Each function parameter
    for (int i = 0; i < (int)params.size(); i++){
        // For each parameter, first add it to the symbol table,
        // then add it to the translation string
        //cerr << "Inserting symbol " << params[i]->get_name() << " with type " << prse_type_to_string(params[i]->get_type()) << endl;
        th.insert(params[i]);
        ret_s += prse_type_to_string(params[i]->get_type()); ret_s += " "; ret_s += params[i]->get_name();
        // Add a separating comma to the string if this is not the last parameter in the list
        if (i < (int)params.size()-1)
            ret_s += ", ";
    }
    // RIGHT_PAREN
    ret_s += ")";
    // Return completed header
    return ret(new Constant(return_type, ret_s));
}

// Function definitions
Function_definition_expr::Function_definition_expr(const Expression* header, vector <const Expression*>& code_block)
:header(header), block(code_block){}

const Constant* Function_definition_expr::as_const() const {
    if (header == nullptr){
        cerr << "Error in Expression.cpp::Function_definition_expr::as_const: function header is null" << endl;
        return ret(new Constant(PRSE_type::NO_TYPE, ""));
    }
    
    auto head_const = header->as_const();
    //cerr << "Processing function: " << head_const->value() << endl;
    
    string ret_s = head_const->value();
    ret_s += "{\n";
    current_return_type = head_const->type();

    if (head_const->value().substr(4, 4) == "main")
    {is_main = true; }
    else { is_main = false; }

    for (auto a : block){
        if (a != nullptr){
            ret_s += a->as_const()->value();
        }
    }
    if (is_main){
        ret_s += "return 0;\n";
    }
    ret_s += "}\n\n";

    try {
        Table_handler::instance().pop_table();
    } catch (...){
        cerr << "Error in Expression.cpp::Function_definition_expr::as_const: tried but failed to pop a scope table." << endl;
    }

    return ret(new Constant(return_type, ret_s));
}

string Function_definition_expr::value() const {
    return "";
}

// Function calls
Function_call::Function_call(const int line, const string& id, vector<const Expression*>& params)
:line(line), id(id), params(params){}

const Constant* Function_call::as_const() const {
    // Check whether this is a standard library function
    if (id == "print" || id == "println") {
        if (Library::instance().using_library("io")){
            string ret_s = "std::cout << ";
            for (int i = 0; i < (int)params.size(); i++){
                ret_s += params[i]->as_const()->value();
                if (i < (int)params.size()-1)
                    ret_s += " << ";
            }
            if (id == "println")
                ret_s += " << std::endl";
            return ret(new Constant(PRSE_type::T_VOID, ret_s));
        } else {
            // Toss out an error for not adding a `use "io"`
            Error::error(Error::UNDEFINED_FUNCTION, id, "", "", line);
            return ret(new Constant(PRSE_type::NO_TYPE, ""));
        }
    } else if (id == "get_input"){
        if (Library::instance().using_library("io")){
            if ((int)params.size() == 0){
                Error::error(Error::UNDEFINED_FUNCTION, id, "", "", line);
                return ret(new Constant(PRSE_type::NO_TYPE, ""));
            } else {
                Table_handler& th = Table_handler::instance();
                bool all_clear = true;
                for (auto a : params){
                    if (th.lookup(a->as_const()->value()) == nullptr){
                        Error::error(Error::VARIABLE_NOT_DEFINED_IN_SCOPE, id, "", "", line);
                        all_clear = false;
                    }
                }
                if (all_clear){
                    string ret_s = "std::cin >> ";
                    for (int i = 0; i < (int)params.size(); i++){
                        ret_s += params[i]->as_const()->value();
                        if (i < (int)params.size()-1)
                            ret_s += " >> ";
                    }
                    return ret(new Constant(PRSE_type::T_VOID, ret_s));
                }
            }
        } else {
            // Toss out an error for not adding a `use "io"`
            Error::error(Error::UNDEFINED_FUNCTION, id, "", "", line);
            return ret(new Constant(PRSE_type::NO_TYPE, ""));
        }
    }

    // Check that this function has been defined
    // This will be evaluated at the end of parsing the program, so it should be
    // able to find the function definition
    vector<PRSE_type> ptl;
    for (auto a : params) ptl.push_back(a->as_const()->type());
    Function_definition* fdef = Function_definition::get_function_definition(Function_declaration(line, id, ptl));
    if (fdef == nullptr){
        string t = "";
        for (int i = 0; i < (int)ptl.size(); i++){
            t += prse_type_to_string(ptl[i]);
            if (i < (int)ptl.size()-1)
                t += ", ";
        }
        Error::error(Error::UNDEFINED_FUNCTION, id, t, "", line);
        vector<Function_definition*> candidates = Function_definition::find_candidates(Function_declaration(line, id, ptl));
        if (candidates.size() > 0){
            for (int i = 0; i < (int)candidates.size(); i++){
                cout << "Note - line " << candidates[i]->get_line_num() << ": function '" << candidates[i]->get_id()
                << "' requires " << candidates[i]->get_parameters().size() << " parameters, but " << params.size() << " were provided." << endl;
            }
        }
        return ret(new Constant(PRSE_type::NO_TYPE, ""));
    } else {
        string t = id;
        t += "(";
        for (int i = 0; i < (int)params.size(); i++){
            t += params[i]->as_const()->value();
            if (i < (int)params.size()-1)
                t += ", ";
        }
        t += ")";
        PRSE_type ret_pt = fdef->get_type();
        return ret(new Constant(ret_pt, t));
    }
}

string Function_call::value() const {
    return "";
}

// Variable defintions
Variable_definition::Variable_definition(const int line, const string& id, const PRSE_type prse_type, const Expression* rhs, int size)
:line(line), lhs(new Constant(prse_type, id)), rhs(rhs), rhs_list(vector<const Expression*>()), size(size){}

Variable_definition::Variable_definition(const int line, const string& id, const PRSE_type prse_type, vector <const Expression*>& rhs, int size)
:line(line), lhs(new Constant(prse_type, id)), rhs(nullptr), rhs_list(rhs), size(size){}

const Constant* Variable_definition::as_const() const {
    Table_handler& th = Table_handler::instance();
    string ret_s = "";
    PRSE_type ret_t = PRSE_type::NO_TYPE;
    if (th.lookup(lhs->as_const()->value()) != nullptr){
        Error::error(Error::VARIABLE_ALREADY_DECLARED, lhs->as_const()->value(), "", "", line);
        return ret(new Constant(ret_t, ret_s));
    }
    // Check that the right hand size expression is not a nullptr,
    // otherwise return an empty value.
    if ( (size > 0 && rhs_list.size() <= 0)
    ||   (size <= 0 && rhs == nullptr)){
        Error::error(Error::VARIABLE_DECLARED_BUT_NOT_SET, lhs->as_const()->value());
        return ret(new Constant(ret_t, ret_s));
    }
    // Variable type and expression types must match.
    PRSE_type lhs_t = lhs->as_const()->type();
    // If variable has defined size > 0, it is an array. We must first process the list of arguments
    // given
    if (size > 0){
        // Type is dictated by the first item in the list assigned to the array
        PRSE_type rhs_t = rhs_list[0]->as_const()->type();
        if (lhs_t != rhs_t){
            Error::error(Error::MISMATCHED_TYPE_FOR_ASSIGNMENT, prse_type_to_string(rhs_t), prse_type_to_string(lhs_t), "", line);
            ret_t = PRSE_type::NO_TYPE;
        } else {
            ret_s = "std::vector<";
            ret_s += prse_type_to_string(lhs_t);
            ret_s += "> ";
            ret_s += lhs->value();
            ret_s += " { ";
            if (rhs_list.size() > 0){
                for (int i = 0; i < (int)rhs_list.size(); i++){
                    auto cur_t = rhs_list[i]->as_const()->type();
                    if (cur_t != rhs_t){
                        Error::error(Error::MISMATCHED_TYPE_FOR_ASSIGNMENT, prse_type_to_string(rhs_t), prse_type_to_string(cur_t), "", line);
                    }
                    ret_s += rhs_list[i]->as_const()->value();
                    if (i < (int)rhs_list.size()-1)
                        ret_s += ", ";
                }
            } else {
                ret_s += rhs->as_const()->type();
            }
            ret_s += " };\n";

        }
        string var_name = lhs->as_const()->value();
        PRSE_type var_type = lhs->as_const()->type();
        int size = 1;
        th.insert(make_shared<Symbol>(var_name, var_type, size));
    } else {
        PRSE_type rhs_t = rhs->as_const()->type();
        if (lhs_t != rhs_t){
            Error::error(Error::MISMATCHED_TYPE_FOR_ASSIGNMENT, prse_type_to_string(rhs_t), prse_type_to_string(lhs_t), "", line);
            ret_t = PRSE_type::NO_TYPE;
        } else {
            ret_s = prse_type_to_string(lhs_t);
            ret_s += " ";
            ret_s += lhs->value();
            ret_s += " = ";
            ret_s += rhs->as_const()->value();
            ret_s += ";\n";
        }
        string var_name = lhs->as_const()->value();
        PRSE_type var_type = lhs->as_const()->type();
        th.insert(make_shared<Symbol>(var_name, var_type));
    }
    return ret(new Constant(ret_t, ret_s));
}

string Variable_definition::value() const {
    return "";
}


Variable_assignment::Variable_assignment(const int line, const Expression* lhs, const Expression* rhs)
:line(line), lhs(lhs), rhs(rhs){}

const Constant* Variable_assignment::as_const() const {
    // Variable type and expression types must match.
    PRSE_type lhs_t = lhs->as_const()->type();
    PRSE_type rhs_t = rhs->as_const()->type();
    PRSE_type ret_t;
    string ret_s = "";
    if (lhs_t != rhs_t){
        if ( lhs_t == PRSE_type::T_DOUBLE && !(rhs_t & (PRSE_type::T_INT | PRSE_type::T_DOUBLE)) ){
            Error::error(Error::MISMATCHED_TYPE_FOR_ASSIGNMENT, prse_type_to_string(rhs_t), prse_type_to_string(lhs_t), "", line);
            ret_t = PRSE_type::NO_TYPE;
        }
    } else {
        ret_s += lhs->as_const()->value();
        ret_s += " = ";
        ret_s += rhs->as_const()->value();
        ret_s += ";\n";
    }
    return ret(new Constant(ret_t, ret_s));
}

string Variable_assignment::value() const {
    return "";
}

Array_element::Array_element(int line, const string nid, const Expression* nsub)
:line(line), id(nid), subscript(nsub){}

const Constant* Array_element::as_const() const {
    Table_handler& th = Table_handler::instance();
    if (th.lookup(id) == nullptr){
        Error::error(Error::VARIABLE_NOT_DEFINED_IN_SCOPE, id, "", "", line);
        return ret(new Constant(PRSE_type::NO_TYPE, ""));
    } else if (th.lookup(id)->get_size() <= 0){
        Error::error(Error::VARIABLE_IS_NOT_AN_ARRAY, id, "", "", line);
        return ret(new Constant(PRSE_type::NO_TYPE, ""));
    }
    auto sym = th.lookup(id);

    PRSE_type ret_t = sym->get_type();
    string ret_s = "";
    if (subscript->as_const()->type() != PRSE_type::T_INT){
        Error::error(Error::ARRAY_SUBSCRIPT_MUST_BE_AN_INTEGER_VALUE, "", "", "", line);
    }
    ret_s += sym->get_name();
    ret_s += "[";
    ret_s += subscript->as_const()->value();
    ret_s += "]";
    return ret(new Constant(ret_t, ret_s));
}

string Array_element::value() const{
    return "";
}

Parentheses::Parentheses(int line, const Expression* e)
:line(line), expr(e){}

const Constant* Parentheses::as_const() const {
    PRSE_type ret_t = expr->as_const()->type();
    string ret_s = "(";
    ret_s += expr->as_const()->value();
    ret_s += ")";
    return ret(new Constant(ret_t, ret_s));
}

string Parentheses::value() const{
    return "";
}

Ternary_expression::Ternary_expression(const int line, const Expression* cond, const Expression*expr_1, const Expression* expr_2)
:line(line), condition(cond), is_true(expr_1), is_false(expr_2){}

const Constant* Ternary_expression::as_const() const {
    PRSE_type ret_t = PRSE_type::NO_TYPE;
    string ret_s = "";
    if (condition != nullptr && is_true != nullptr && is_false != nullptr){
        if (condition->as_const()->type() != PRSE_type::T_BOOL){
            Error::error(Error::CONDITION_MUST_BE_BOOLEAN_EXPRESSION, prse_type_to_string(condition->as_const()->type()), "", "", line);
            return ret(new Constant(PRSE_type::NO_TYPE, ""));
        }
        if (is_true->as_const()->type() != is_false->as_const()->type()){
            Error::error(Error::EXPRESSION_TYPES_DO_NOT_MATCH, prse_type_to_string(is_true->as_const()->type()), prse_type_to_string(is_false->as_const()->type()), "", line);
            cout << "==> Expressions in ternary assignments must match each other's types." << endl;
            return ret(new Constant(PRSE_type::NO_TYPE, ""));
        } else {
            ret_t = is_true->as_const()->type();
            ret_s += "(";
            ret_s += condition->as_const()->value();
            ret_s += " ? ";
            ret_s += is_true->as_const()->value();
            ret_s += " : ";
            ret_s += is_false->as_const()->value();
            ret_s += ")";
        }
    }
    return ret(new Constant(ret_t, ret_s));
}

string Ternary_expression::value() const {
    return "";
}

vector<pair<int, Return_statement*>> Return_statement::return_statements_list = vector<pair<int, Return_statement*>>();

// Return statements
Return_statement::Return_statement(const int line, const Expression* rhs)
:line(line), rhs(rhs){
    return_statements_list.push_back(make_pair(line, this));
}

const Constant* Return_statement::as_const() const {
    // If this return statement has no right-hand side expression,
    // by default it is a void return.
    PRSE_type rhs_t = PRSE_type::T_VOID;
    if (rhs != nullptr)
        rhs_t = rhs->as_const()->type();
    if (rhs_t != Function_definition_expr::current_return_type){
        Error::error(Error::RETURN_VALUE_FOR_FUNCTION_DOES_NOT_MATCH,
        prse_type_to_string(rhs_t), prse_type_to_string(Function_definition_expr::current_return_type), "", line);
    }
    string ret_s = "return";
    if (rhs != nullptr){
        ret_s += " "; ret_s += rhs->as_const()->value();
    } else if (Function_definition_expr::is_main) {
        // If the current context is main (which can be void), and the user adds a return statement,
        // then the above error checks prevent the user from writing a `return 0` expression
        // for a void main. Thus, we must ensure compilability by doing this:
        ret_s += " 0";
    }
    ret_s += ";\n";
    return ret(new Constant(rhs_t, ret_s));
}

string Return_statement::value() const {
    return "";
}

// If block

If_block::If_block(const int line, const Expression* cond, vector<const Expression*>& cb, const Expression* optional_else_elseif)
:line(line), condition(cond), code_block(cb), optional_else_or_else_if(optional_else_elseif){}

const Constant* If_block::as_const() const {
    string ret_s = "";
    PRSE_type ret_t = PRSE_type::NO_TYPE;
    if (condition != nullptr){
        auto cond_const = condition->as_const();
        auto cond_type = cond_const->type();
        if (cond_type != PRSE_type::T_BOOL){
            Error::error(Error::CONDITION_MUST_BE_BOOLEAN_EXPRESSION, prse_type_to_string(cond_type), "", "", line);
        } else {
            ret_s += "if (";
            ret_s += cond_const->value();
            ret_s += ") {\n";

            Table_handler& th = Table_handler::instance();
            th.push_table();

            for (auto a : code_block){
                if (a != nullptr){
                    ret_s += a->as_const()->value();
                }
            }

            ret_s += "}";

            try {
                th.pop_table();
            } catch (...) {
                cerr << "Error in Expression.cpp::If_block::as_const: tried but failed to pop a scope table." << endl;
            }

            if (optional_else_or_else_if != nullptr){
                ret_s += " ";
                ret_s += optional_else_or_else_if->as_const()->value();
            } else ret_s += "\n";
        }
    } else {
        Error::error(Error::CONDITION_CANNOT_BE_EMPTY, "", "", "", line);
    }
    return ret(new Constant(ret_t, ret_s));
}

string If_block::value() const {
    return "";
}

// Else if block

Else_if_block::Else_if_block(const int line, const Expression* cond, vector<const Expression*>& cb, const Expression* optional_else_elseif)
:line(line), condition(cond), code_block(cb), optional_else_or_else_if(optional_else_elseif){}

const Constant* Else_if_block::as_const() const {
    string ret_s = "";
    PRSE_type ret_t = PRSE_type::NO_TYPE;
    if (condition != nullptr){
        auto cond_const = condition->as_const();
        auto cond_type = cond_const->type();
        if (cond_type != PRSE_type::T_BOOL){
            Error::error(Error::CONDITION_MUST_BE_BOOLEAN_EXPRESSION, prse_type_to_string(cond_type), "", "", line);
        } else {
            Table_handler& th = Table_handler::instance();
            th.push_table();

            ret_s += "else if (";
            ret_s += cond_const->value();
            ret_s += ") {\n";
            for (auto a : code_block){
                if (a != nullptr){
                    ret_s += a->as_const()->value();
                }
            }
            ret_s += "}";

            try {
                th.pop_table();
            } catch (...) {
                cerr << "Error in Expression.cpp::If_block::as_const: tried but failed to pop a scope table." << endl;
            }

            if (optional_else_or_else_if != nullptr){
                ret_s += " ";
                ret_s += optional_else_or_else_if->as_const()->value();
            } else ret_s += "\n";
        }
    } else {
        Error::error(Error::CONDITION_CANNOT_BE_EMPTY, "", "", "", line);
    }
    return ret(new Constant(ret_t, ret_s));
}

string Else_if_block::value() const {
    return "";
}

// Else block

Else_block::Else_block(const int line, vector<const Expression*>& cb)
:line(line), code_block(cb){}

const Constant* Else_block::as_const() const {
    string ret_s = "";
    PRSE_type ret_t = PRSE_type::NO_TYPE;

    Table_handler& th = Table_handler::instance();
    th.push_table();

    ret_s += "else {\n";
    for (auto a : code_block){
        if (a != nullptr){
            ret_s += a->as_const()->value();
        }
    }
    ret_s += "}\n";

    try {
        th.pop_table();
    } catch (...) {
        cerr << "Error in Expression.cpp::If_block::as_const: tried but failed to pop a scope table." << endl;
    }

    return ret(new Constant(ret_t, ret_s));
}

string Else_block::value() const {
    return "";
}

// For header: ()
For_header::For_header(const int line, const shared_ptr<Symbol>& var, const Expression* it_init, const Expression* cond, const Expression* assignment)
:line(line), variable(var), initializer(it_init), condition(cond), assignment(assignment), range(nullptr){}

For_header::For_header(const int line, string& var, const Expression* range)
:line(line), initializer(nullptr), condition(nullptr), assignment(nullptr){
    if (range != nullptr){
        PRSE_type var_type = range->as_const()->type();
        variable = make_shared<Symbol>(var, var_type);
    }
    else {
        cerr << "Error in Expression.cpp::For_header::For_header: range is a nullptr" << endl;
    }
}

const Constant* For_header::as_const() const{
    PRSE_type ret_t = PRSE_type::NO_TYPE;
    string ret_s = "";
    if (initializer != nullptr && condition != nullptr && assignment != nullptr) {
        Table_handler& th = Table_handler::instance();
        th.push_table();
        th.insert(variable);
        ret_s += "for (";
        ret_s += prse_type_to_string(variable->get_type());
        ret_s += " ";
        ret_s += variable->get_name();
        ret_s += " = ";
        ret_s += initializer->as_const()->value();
        ret_s += "; ";
        ret_s += condition->as_const()->value();
        ret_s += "; ";
        ret_s += assignment->as_const()->value();
        ret_s += ")";
    } else {
        Table_handler& th = Table_handler::instance();
        th.push_table();
        th.insert(variable);
        ret_s += "for (auto ";
        ret_s += variable->as_const()->value();
        ret_s += " : ";
        ret_s += range->as_const()->value();
        ret_s += ")";
    }
    return ret(new Constant(ret_t, ret_s));
}

string For_header::value() const{
    return "";
}

// For loop
For_loop::For_loop(const int line, const Expression* header, vector<const Expression*>& cb)
:line(line), header(header), code_block(cb){}

const Constant* For_loop::as_const() const{
    PRSE_type ret_t = PRSE_type::NO_TYPE;
    string ret_s = "";
    Table_handler& th = Table_handler::instance();
    if (header != nullptr)
        ret_s += header->as_const()->value();
    ret_s += "{\n";
    for (auto a : code_block){
        if (a != nullptr){
            ret_s += a->as_const()->value();
            ret_s += ";\n";
        }
    }
    ret_s += "}\n";
    try {
        th.pop_table();
    } catch (...) {
        cerr << "Error in Expression.cpp::For_loop::as_const: tried but failed to pop a scope table." << endl;
    }
    return ret(new Constant(ret_t, ret_s));
}

string For_loop::value() const{
    return "";
}

// While loop
While_loop::While_loop(const int line, const Expression* cond, vector<const Expression*>& cb)
:line(line), condition(cond), code_block(cb){}

const Constant* While_loop::as_const() const{
    string ret_s = "while(";
    PRSE_type ret_t = PRSE_type::NO_TYPE;
    if (condition != nullptr){
        if (condition->as_const()->type() != PRSE_type::T_BOOL){
            Error::error(Error::CONDITION_MUST_BE_BOOLEAN_EXPRESSION, prse_type_to_string(condition->as_const()->type()), "", "", line);
            return ret(new Constant(PRSE_type::NO_TYPE, ""));
        }
        else ret_s += condition->as_const()->value();
    }
    ret_s += "){\n";

    Table_handler& th = Table_handler::instance();
    th.push_table();

    for (auto a : code_block){
        if (a != nullptr){
            ret_s += a->as_const()->value();
            ret_s += ";\n";
        }
    }
    ret_s += "}\n";
    try {
        th.pop_table();
    } catch (...) {
        cerr << "Error in Expression.cpp::While_loop::as_const: tried but failed to pop a scope table." << endl;
    }
    return ret(new Constant(ret_t, ret_s));
}

// Scope block
Scope_block::Scope_block(int line, vector<const Expression*>& cb)
:line(line), code_block(cb){}

const Constant* Scope_block::as_const() const {
    Table_handler& th = Table_handler::instance();
    th.push_table();

    string ret_s = "";
    for (auto a : code_block){
        if (a != nullptr)
            ret_s += a->as_const()->value();
    }

    try {
        th.pop_table();
    } catch (...){
        cerr << "Error in Expression.cpp::Scope_block::as_const: tried but failed to pop a scope table." << endl;
    }
    return ret(new Constant(PRSE_type::NO_TYPE, ret_s));
}

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
