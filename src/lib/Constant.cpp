#include "Constant.h"

Constant::Constant(PRSE_type type, const string& value):const_type(type), const_value(value){}

Constant::~Constant(){}

PRSE_type Constant::type() const { return const_type; }