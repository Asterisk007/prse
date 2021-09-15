#pragma once
#include "Unary_operator.h"

// A singleton expression is a standalone line of code (for instance, a function call)
// takes the expression given, and when interrogated for its Constant* value,
// it evaluates its expression and appends a semicolon.
class Singleton : public Unary_operator {
public:
    Singleton(const Expression* lhs);
    virtual const Constant* as_const() const;
    virtual vector<const Constant*> as_list() const;
    virtual string value() const;
};
