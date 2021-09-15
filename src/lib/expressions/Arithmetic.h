#pragma once
#include "Unary_operator.h"
#include "Binary_operator.h"

class Decrement : public Unary_operator {
private:
    int line;
public:
    Decrement(int line, const Expression* lhs);
    virtual const Constant* as_const() const;
    virtual string value() const;
    virtual vector<const Constant*> as_list() const;
};

class Divide : public Binary_operator {
private:
    int line;
public:
    Divide(int line, const Expression* lhs, const Expression* rhs):Binary_operator(lhs, rhs), line(line){}
    virtual const Constant* as_const() const;
    virtual string value() const;
    virtual vector<const Constant*> as_list() const;
};

class Increment : public Unary_operator {
private:
    int line;
public:
    Increment(int line, const Expression* lhs);
    virtual const Constant* as_const() const;
    virtual string value() const;
    virtual vector<const Constant*> as_list() const;
};

class Minus : public Binary_operator {
private:
    int line;
public:
    Minus(int line, const Expression* lhs, const Expression* rhs):Binary_operator(lhs, rhs), line(line){}
    virtual const Constant* as_const() const;
    virtual string value() const;
    virtual vector<const Constant*> as_list() const;
};

class Modulo : public Binary_operator {
private:
    int line;
public:
    Modulo(int line, const Expression* lhs, const Expression* rhs):Binary_operator(lhs, rhs), line(line){}
    virtual const Constant* as_const() const;
    virtual string value() const;
    virtual vector<const Constant*> as_list() const;
};

class Multiply : public Binary_operator {
private:
    int line;
public:
    Multiply(int line, const Expression* lhs, const Expression* rhs):Binary_operator(lhs, rhs), line(line){}
    virtual const Constant* as_const() const;
    virtual string value() const;
    virtual vector<const Constant*> as_list() const;
};

class Plus : public Binary_operator {
private:
    int line;
public:
    Plus(int line, const Expression* lhs, const Expression* rhs):Binary_operator(lhs, rhs), line(line){}
    virtual const Constant* as_const() const;
    virtual string value() const;
    virtual vector<const Constant*> as_list() const;
};