#pragma once
#include "Binary_operator.h"

class Logic_Eq : public Binary_operator {
public:
    Logic_Eq(const Expression* lhs, const Expression* rhs): Binary_operator(lhs, rhs){}
    virtual const Constant* as_const() const;
    virtual string value() const;
    virtual vector<const Constant*> as_list() const;
};

class Logic_NEq : public Binary_operator {
public:
    Logic_NEq(const Expression* lhs, const Expression* rhs): Binary_operator(lhs, rhs){}
    virtual const Constant* as_const() const;
    virtual string value() const;
    virtual vector<const Constant*> as_list() const;
};

class Logic_Less_Eq : public Binary_operator {
public:
    Logic_Less_Eq(const Expression* lhs, const Expression* rhs): Binary_operator(lhs, rhs){}
    virtual const Constant* as_const() const;
    virtual string value() const;
    virtual vector<const Constant*> as_list() const;
};

class Logic_Greater_Eq : public Binary_operator {
public:
    Logic_Greater_Eq(const Expression* lhs, const Expression* rhs): Binary_operator(lhs, rhs){}
    virtual const Constant* as_const() const;
    virtual string value() const;
    virtual vector<const Constant*> as_list() const;
};

class Logic_Less_Than : public Binary_operator {
public:
    Logic_Less_Than(const Expression* lhs, const Expression* rhs): Binary_operator(lhs, rhs){}
    virtual const Constant* as_const() const;
    virtual string value() const;
    virtual vector<const Constant*> as_list() const;
};

class Logic_Greater_Than : public Binary_operator {
public:
    Logic_Greater_Than(const Expression* lhs, const Expression* rhs): Binary_operator(lhs, rhs){}
    virtual const Constant* as_const() const;
    virtual string value() const;
    virtual vector<const Constant*> as_list() const;
};

class Logic_And : public Binary_operator {
public:
    Logic_And(const Expression* lhs, const Expression* rhs): Binary_operator(lhs, rhs){}
    virtual const Constant* as_const() const;
    virtual string value() const;
    virtual vector<const Constant*> as_list() const;
};

class Logic_Or : public Binary_operator {
public:
    Logic_Or(const Expression* lhs, const Expression* rhs): Binary_operator(lhs, rhs){}
    virtual const Constant* as_const() const;
    virtual string value() const;
    virtual vector<const Constant*> as_list() const;
};