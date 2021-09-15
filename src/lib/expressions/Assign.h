#pragma once
#include "Binary_operator.h"

class Plus_assign : public Binary_operator {
protected:
    int line;
public:
    Plus_assign(int line, const Expression* lhs, const Expression* rhs):Binary_operator(lhs, rhs), line(line){}
    virtual const Constant* as_const() const;
    virtual string value() const;
    virtual vector<const Constant*> as_list() const;
};

class Minus_assign : public Binary_operator {
protected:
    int line;
public:
    Minus_assign(int line, const Expression* lhs, const Expression* rhs):Binary_operator(lhs, rhs), line(line){}
    virtual const Constant* as_const() const;
    virtual string value() const;
    virtual vector<const Constant*> as_list() const;
};

class Divide_assign : public Binary_operator {
protected:
    int line;
public:
    Divide_assign(int line, const Expression* lhs, const Expression* rhs):Binary_operator(lhs, rhs), line(line){}
    virtual const Constant* as_const() const;
    virtual string value() const;
    virtual vector<const Constant*> as_list() const;
};

class Multiply_assign : public Binary_operator {
protected:
    int line;
public:
    Multiply_assign(int line, const Expression* lhs, const Expression* rhs):Binary_operator(lhs, rhs), line(line){}
    virtual const Constant* as_const() const;
    virtual string value() const;
    virtual vector<const Constant*> as_list() const;
};