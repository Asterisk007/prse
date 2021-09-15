#include "Expression.h"
#include "Constant.h"

Expression::~Expression(){ delete expr_val; }

const Constant* Expression::ret(const Constant* new_constant) const {
    delete expr_val;
    return expr_val = new_constant;
}