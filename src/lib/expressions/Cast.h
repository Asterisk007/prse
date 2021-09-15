#pragma once
#include "Unary_operator.h"

class Cast : public Unary_operator {
private:
    int line;
    PRSE_type cast_type;
public:
    Cast(int line, const Expression* lhs, PRSE_type cast_to);
    virtual const Constant* as_const() const;
    virtual string value() const;
    virtual vector<const Constant*> as_list() const;
};