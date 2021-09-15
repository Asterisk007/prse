#pragma once
#include "Expression.h"

class Unary_operator : public Expression {
protected:
    const Expression* lhs;
public:
    Unary_operator(const Expression* lhs):lhs(lhs){}
    virtual ~Unary_operator(){ delete lhs; }
    virtual const Constant* as_const() const = 0;
    virtual vector<const Constant*> as_list() const = 0;
};