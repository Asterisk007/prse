#pragma once
#include "Binary_operator.h"

enum Bulk_assign_type {
    BULK_ASSIGN,
    BULK_INCREMENT,
    BULK_DECREMENT,
    BULK_PLUS_ASSIGN,
    BULK_MINUS_ASSIGN,
    BULK_MULTIPLY_ASSIGN,
    BULK_DIVIDE_ASSIGN
};

class Bulk_assign : public Binary_operator {
protected:
    int line;
    Bulk_assign_type op_type;
public:
    Bulk_assign(int line, Bulk_assign_type type, const Expression* lhs, const Expression* rhs):
        Binary_operator(lhs, rhs), line(line), op_type(type){}
    virtual const Constant* as_const() const;
    virtual string value() const { return ""; }
    virtual vector<const Constant*> as_list() const;
};