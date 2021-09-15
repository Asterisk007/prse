#pragma once
#include "../PRSE_type.h"
#include "../Symbol.h"
#include <memory>
#include <vector>
#include <assert.h>

using namespace std;

class Constant;
// The generic Expression class which all expressions are made of
class Expression {
public:
    Expression():expr_type(NO_TYPE), expr_val(nullptr){}
    Expression(const Expression&) = delete;
    virtual ~Expression();
    virtual PRSE_type type() const { return expr_type; }
    virtual const Constant* as_const() const = 0; // returns a constant
    virtual string value() const = 0;
protected:
    const Constant* ret(const Constant* new_constant) const;
    PRSE_type expr_type;
    mutable const Constant* expr_val;
};