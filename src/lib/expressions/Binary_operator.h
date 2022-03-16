#pragma once
#include "Expression.h"

class Binary_operator : public Expression {
protected:
    const Expression* lhs;
    const Expression* rhs;
public:
    Binary_operator(const Expression* lhs, const Expression* rhs):lhs(lhs), rhs(rhs){}
    virtual ~Binary_operator(){ delete lhs; delete rhs; }
    virtual const Constant* as_const() const = 0;
    virtual vector<const Constant*> as_list() const = 0;
};