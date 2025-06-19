#pragma once
#include "Unary_operator.h"
#include "Binary_operator.h"

class Xor : public Binary_operator {
    private:
        int line;
    public:
        Xor(int line, const Expression* lhs, const Expression* rhs):Binary_operator(lhs, rhs), line(line){}
        virtual const Constant* as_const() const;
        virtual string value() const;
        virtual vector<const Constant*> as_list() const;
};