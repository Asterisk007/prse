#include "Expression.h"
#include "Constant.h"
#include "Declared_list.h"

//Function_call::Function_call(PRSE_type type, string& name, vector<Expression>& arg_list)
//:Expression(type, name), arguments(arg_list){}

Expression::~Expression(){ delete expr_val; }

// Plus
Constant* Plus::as_const(){
    Constant* l = lhs->as_const();
    Constant* r = rhs->as_const();
    string t = l->value();
    t += " + "; t += r->value();
    return new Constant(l->type(), t);
}

string Plus::value() const{
    Constant* l = lhs->as_const();
    Constant* r = rhs->as_const();
    string t = l->value();
    t += " + "; t += r->value();
    return t;
}

vector<Constant*> Plus::as_list(){
    Constant* l = lhs->as_const();
    Constant* r = rhs->as_const();
    string t = l->value();
    t += " + "; t += r->value();
    vector<Constant*> cl;
    cl.push_back(new Constant(l->type(), t));
    return cl;
}

// Minus
Constant* Minus::as_const(){
    Constant* l = lhs->as_const();
    Constant* r = rhs->as_const();
    string t = l->value();
    t += " - "; t += r->value();
    return new Constant(l->type(), t);
}

string Minus::value() const{
    Constant* l = lhs->as_const();
    Constant* r = rhs->as_const();
    string t = l->value();
    t += " - "; t += r->value();
    return t;
}

vector<Constant*> Minus::as_list(){
    Constant* l = lhs->as_const();
    Constant* r = rhs->as_const();
    string t = l->value();
    t += " - "; t += r->value();
    vector<Constant*> cl;
    cl.push_back(new Constant(l->type(), t));
    return cl;
}

// Equality
//===================================================
// Operator ==
Constant* Logic_Eq::as_const(){
    Constant* l = lhs->as_const();
    Constant* r = rhs->as_const();
    string t = l->value();
    t += " == "; t += r->value();
    return new Constant(PRSE_type::T_BOOL, t);
}

string Logic_Eq::value() const{
    Constant* l = lhs->as_const();
    Constant* r = rhs->as_const();
    string t = l->value();
    t += " == "; t += r->value();
    return t;
}

vector<Constant*> Logic_Eq::as_list(){
    Constant* l = lhs->as_const();
    Constant* r = rhs->as_const();
    string t = l->value();
    t += " == "; t += r->value();
    vector<Constant*> cl;
    cl.push_back(new Constant(PRSE_type::T_BOOL, t));
    return cl;
}

//////////////////////////////////////////////////
// Operator ==
Constant* Logic_NEq::as_const(){
    Constant* l = lhs->as_const();
    Constant* r = rhs->as_const();
    string t = l->value();
    t += " != "; t += r->value();
    return new Constant(PRSE_type::T_BOOL, t);
}

string Logic_NEq::value() const{
    Constant* l = lhs->as_const();
    Constant* r = rhs->as_const();
    string t = l->value();
    t += " != "; t += r->value();
    return t;
}

vector<Constant*> Logic_NEq::as_list(){
    Constant* l = lhs->as_const();
    Constant* r = rhs->as_const();
    string t = l->value();
    t += " != "; t += r->value();
    vector<Constant*> cl;
    cl.push_back(new Constant(PRSE_type::T_BOOL, t));
    return cl;
}

//////////////////////////////////////////////////
// Operator <=

Constant* Logic_Less_Eq::as_const(){
    Constant* l = lhs->as_const();
    Constant* r = rhs->as_const();
    string t = l->value();
    t += " <= "; t += r->value();
    return new Constant(PRSE_type::T_BOOL, t);
}

string Logic_Less_Eq::value() const{
    Constant* l = lhs->as_const();
    Constant* r = rhs->as_const();
    string t = l->value();
    t += " <= "; t += r->value();
    return t;
}

vector<Constant*> Logic_Less_Eq::as_list(){
    Constant* l = lhs->as_const();
    Constant* r = rhs->as_const();
    string t = l->value();
    t += " <= "; t += r->value();
    vector<Constant*> cl;
    cl.push_back(new Constant(PRSE_type::T_BOOL, t));
    return cl;
}

//////////////////////////////////////////////////
// Operator >=

Constant* Logic_Greater_Eq::as_const(){
    Constant* l = lhs->as_const();
    Constant* r = rhs->as_const();
    string t = l->value();
    t += " >= "; t += r->value();
    return new Constant(PRSE_type::T_BOOL, t);
}

string Logic_Greater_Eq::value() const{
    Constant* l = lhs->as_const();
    Constant* r = rhs->as_const();
    string t = l->value();
    t += " >= "; t += r->value();
    return t;
}

vector<Constant*> Logic_Greater_Eq::as_list(){
    Constant* l = lhs->as_const();
    Constant* r = rhs->as_const();
    string t = l->value();
    t += " >= "; t += r->value();
    vector<Constant*> cl;
    cl.push_back(new Constant(PRSE_type::T_BOOL, t));
    return cl;
}

//////////////////////////////////////////////////
// Operator <

Constant* Logic_Less_Than::as_const(){
    Constant* l = lhs->as_const();
    Constant* r = rhs->as_const();
    string t = l->value();
    t += " < "; t += r->value();
    return new Constant(PRSE_type::T_BOOL, t);
}

string Logic_Less_Than::value() const{
    Constant* l = lhs->as_const();
    Constant* r = rhs->as_const();
    string t = l->value();
    t += " < "; t += r->value();
    return t;
}

vector<Constant*> Logic_Less_Than::as_list(){
    Constant* l = lhs->as_const();
    Constant* r = rhs->as_const();
    string t = l->value();
    t += " < "; t += r->value();
    vector<Constant*> cl;
    cl.push_back(new Constant(PRSE_type::T_BOOL, t));
    return cl;
}

//////////////////////////////////////////////////
// Operator >

Constant* Logic_Greater_Than::as_const(){
    Constant* l = lhs->as_const();
    Constant* r = rhs->as_const();
    string t = l->value();
    t += " > "; t += r->value();
    return new Constant(PRSE_type::T_BOOL, t);
}

string Logic_Greater_Than::value() const{
    Constant* l = lhs->as_const();
    Constant* r = rhs->as_const();
    string t = l->value();
    t += " > "; t += r->value();
    return t;
}

vector<Constant*> Logic_Greater_Than::as_list(){
    Constant* l = lhs->as_const();
    Constant* r = rhs->as_const();
    string t = l->value();
    t += " > "; t += r->value();
    vector<Constant*> cl;
    cl.push_back(new Constant(PRSE_type::T_BOOL, t));
    return cl;
}

//////////////////////////////////////////////////
// Operator && (AND)

Constant* Logic_And::as_const(){
    Constant* l = lhs->as_const();
    Constant* r = rhs->as_const();
    string t = l->value();
    t += " && "; t += r->value();
    return new Constant(PRSE_type::T_BOOL, t);
}

string Logic_And::value() const{
    Constant* l = lhs->as_const();
    Constant* r = rhs->as_const();
    string t = l->value();
    t += " && "; t += r->value();
    return t;
}

vector<Constant*> Logic_And::as_list(){
    Constant* l = lhs->as_const();
    Constant* r = rhs->as_const();
    string t = l->value();
    t += " && "; t += r->value();
    vector<Constant*> cl;
    cl.push_back(new Constant(PRSE_type::T_BOOL, t));
    return cl;
}

//////////////////////////////////////////////////
// Operator || (OR)

Constant* Logic_Or::as_const(){
    Constant* l = lhs->as_const();
    Constant* r = rhs->as_const();
    string t = l->value();
    t += " || "; t += r->value();
    return new Constant(PRSE_type::T_BOOL, t);
}

string Logic_Or::value() const{
    Constant* l = lhs->as_const();
    Constant* r = rhs->as_const();
    string t = l->value();
    t += " || "; t += r->value();
    return t;
}

vector<Constant*> Logic_Or::as_list(){
    Constant* l = lhs->as_const();
    Constant* r = rhs->as_const();
    string t = l->value();
    t += " || "; t += r->value();
    vector<Constant*> cl;
    cl.push_back(new Constant(PRSE_type::T_BOOL, t));
    return cl;
}

/////////////////////////////////////////////////
// Misc

Function_call::Function_call(const string& id, const vector<PRSE_type>& params)
:id(id), params(params){}

// Function calls
Constant* Function_call::as_const(){
    // Check that this function has been defined
    // This will be evaluated at the end of parsing the program, so it should be
    // able to find the function definition
    Function_declaration fd = Function_declaration();
    Function_definition::get_function_definition()
}