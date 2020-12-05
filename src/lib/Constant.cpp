#include "Constant.h"

Constant::Constant(const Constant* c)
:const_type(c->type()), const_value(c->value()){}

Constant::Constant(PRSE_type type, const string& value):const_type(type), const_value(value){}

Constant::~Constant(){}

PRSE_type Constant::type() const { return const_type; }