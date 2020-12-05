#pragma once

#include "PRSE_type.h"
#include "Expression.h"

using namespace std;

class Constant : public Expression {
public:
    Constant(const Constant* c);
    Constant(PRSE_type type, const string& value);
    virtual ~Constant();
    virtual const Constant* as_const() const { return this; }
    //virtual vector<Constant*> as_list();
    virtual string value() const { return const_value; }
    virtual PRSE_type type() const final;
private:
    const PRSE_type const_type;
    string const_value;
};