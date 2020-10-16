#pragma once
#include "PRSE_type.h"  
#include <vector>

class Constant;
// The generic Expression class which all expressions are made of
class Expression {
public:
    Expression():expr_type(NO_TYPE), expr_val(nullptr){}
    Expression(const Expression&) = delete;
    virtual ~Expression();
    virtual PRSE_type type() const { return expr_type; }
    virtual Constant* as_const() = 0; // returns a constant
    //virtual vector<Constant*> as_list() = 0; // returns a list of constants
    virtual string value() const = 0;
protected:
    PRSE_type expr_type;
    Constant* expr_val;
};

class Binary_operator : public Expression {
protected:
    Expression* lhs;
    Expression* rhs;
public:
    Binary_operator(Expression* lhs, Expression* rhs):lhs(lhs), rhs(rhs){}
    virtual ~Binary_operator(){ delete lhs; delete rhs; }
    virtual Constant* as_const() = 0;
    virtual vector<Constant*> as_list() = 0;
};

class Unary_operator : public Expression {
protected:
    Expression* lhs;
public:
    Unary_operator(Expression* lhs):lhs(lhs){}
    virtual ~Unary_operator(){ delete lhs; }
    virtual Constant* as_const() = 0;
    virtual vector<Constant*> as_list() = 0;
};

class Plus : public Binary_operator {
public:
    Plus(Expression* lhs, Expression* rhs): Binary_operator(lhs, rhs){}
    virtual Constant* as_const();
    virtual string value() const;
    virtual vector<Constant*> as_list();
};

class Minus : public Binary_operator {
public:
    Minus(Expression* lhs, Expression* rhs): Binary_operator(lhs, rhs){}
    virtual Constant* as_const();
    virtual string value() const;
    virtual vector<Constant*> as_list();
};

class Logic_Eq : public Binary_operator {
public:
    Logic_Eq(Expression* lhs, Expression* rhs): Binary_operator(lhs, rhs){}
    virtual Constant* as_const();
    virtual string value() const;
    virtual vector<Constant*> as_list();
};

class Logic_NEq : public Binary_operator {
public:
    Logic_NEq(Expression* lhs, Expression* rhs): Binary_operator(lhs, rhs){}
    virtual Constant* as_const();
    virtual string value() const;
    virtual vector<Constant*> as_list();
};

class Logic_Less_Eq : public Binary_operator {
public:
    Logic_Less_Eq(Expression* lhs, Expression* rhs): Binary_operator(lhs, rhs){}
    virtual Constant* as_const();
    virtual string value() const;
    virtual vector<Constant*> as_list();
};

class Logic_Greater_Eq : public Binary_operator {
public:
    Logic_Greater_Eq(Expression* lhs, Expression* rhs): Binary_operator(lhs, rhs){}
    virtual Constant* as_const();
    virtual string value() const;
    virtual vector<Constant*> as_list();
};

class Logic_Less_Than : public Binary_operator {
public:
    Logic_Less_Than(Expression* lhs, Expression* rhs): Binary_operator(lhs, rhs){}
    virtual Constant* as_const();
    virtual string value() const;
    virtual vector<Constant*> as_list();
};

class Logic_Greater_Than : public Binary_operator {
public:
    Logic_Greater_Than(Expression* lhs, Expression* rhs): Binary_operator(lhs, rhs){}
    virtual Constant* as_const();
    virtual string value() const;
    virtual vector<Constant*> as_list();
};

class Logic_And : public Binary_operator {
public:
    Logic_And(Expression* lhs, Expression* rhs): Binary_operator(lhs, rhs){}
    virtual Constant* as_const();
    virtual string value() const;
    virtual vector<Constant*> as_list();
};

class Logic_Or : public Binary_operator {
public:
    Logic_Or(Expression* lhs, Expression* rhs): Binary_operator(lhs, rhs){}
    virtual Constant* as_const();
    virtual string value() const;
    virtual vector<Constant*> as_list();
};

class Interpolated_string : public Expression {
protected:
    vector<Expression*> list;
public:
    Interpolated_string(Expression* expr){
        list = vector<Expression*>(0);
        list.push_back(expr);
    }
    Interpolated_string(vector<Expression*> expr_list): list(expr_list){}
    vector<Expression*> as_list(){ return list; }
};

class Function_call : public Expression {
protected:
    string id;
    vector<PRSE_type> params;
public:
    Function_call(const string& id, const vector<PRSE_type>& params){}
    virtual Constant* as_const();
};

/*class Function_call : public Expression {
private:
    vector<Expression> arguments;
public:
    Function_call():Expression(){}
    Function_call(PRSE_type type, string& name, vector<Expression>& arg_list);
    virtual string get_value() const;
};*/