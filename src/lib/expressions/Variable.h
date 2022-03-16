#pragma once
#include "Expression.h"

class Variable : public Expression {
protected:
    int line;
    const string symbol;
public:
    Variable(int line, const string s);
    virtual ~Variable(){};
    virtual const Constant* as_const() const;
    virtual string value() const { return ""; }
};

class Array : public Variable {
public:
    Array(int line, const string s);
    virtual ~Array(){};
    virtual const Constant* as_const() const;
    virtual string value() const { return ""; }
};