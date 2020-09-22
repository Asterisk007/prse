#pragma once
#include "PRSE_type.h"
#include <vector>

// The generic Expression class which all expressions are made of
class Expression {
public:
    Expression();
    Expression(PRSE_type& type, vector<string>& list); // Creates an expression from an existing expression list
    PRSE_type get_expr_type() const { return expr_type; }
    bool prepend_expression(PRSE_type& type, string& expr);
    bool append_expression(PRSE_type& type, string& expr);
    vector<string> get_list() const{ return list; }
private:
    PRSE_type expr_type;
    vector<string> list;
};

class Function_call : public Expression {

};