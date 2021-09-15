#pragma once
#include "Expression.h"

class Interpolated_string : public Expression {
protected:
    vector<const Expression*> list;
public:
    Interpolated_string(const Expression* expr){
        list = vector<const Expression*>(0);
        list.push_back(expr);
    }
    Interpolated_string(vector<const Expression*>& expr_list): list(expr_list){}
    virtual ~Interpolated_string(){
        for (auto a : list){
            delete a;
        }
    }
    vector<const Expression*> as_list(){ return list; }
    virtual string value() const;
};

class Function_header : public Expression {
protected:
    int line;
    PRSE_type return_type;
    string name;
    vector<shared_ptr<Symbol>> params;
public:
    Function_header(int line, string n, PRSE_type rt, vector<shared_ptr<Symbol>>& pl);
    virtual ~Function_header(){};
    virtual const Constant* as_const() const;
    virtual string value() const { return ""; }
};

class Function_definition_expr : public Expression {
protected:
    PRSE_type return_type;
    const Expression* header;
    vector<const Expression*> block;
public:
    Function_definition_expr(const Expression* header, vector<const Expression*>& code_block);
    virtual ~Function_definition_expr(){
        delete header;
        for (auto a : block){
            delete a;
        }
    }
    virtual const Constant* as_const() const;
    static PRSE_type current_return_type;
    static bool is_main;
    virtual string value() const;
};

class Function_call : public Expression {
protected:
    int line;
    string id;
    vector<const Expression*> params;
public:
    Function_call(const int line, const string& id, vector<const Expression*>& params);
    virtual ~Function_call(){
        for (auto a : params){
            delete a;
        }
    }
    virtual const Constant* as_const() const;
    virtual string value() const;
};

class Variable_definition : public Expression {
protected:
    int line;
    int size;
    const Expression* lhs;
    const Expression* rhs;
    vector<const Expression*> rhs_list;
public:
    Variable_definition(const int line, const string& id, const PRSE_type prse_type, const Expression* rhs, int size);
    Variable_definition(const int line, const string& id, const PRSE_type prse_type, vector<const Expression*>& rhs, int size);
    virtual ~Variable_definition(){ delete lhs; delete rhs; }
    virtual const Constant* as_const() const;
    virtual string value() const;
};

class Variable_assignment : public Expression {
protected:
    int line;
    const Expression* lhs;
    const Expression* rhs;
public:
    Variable_assignment(const int line, const Expression* lhs, const Expression* rhs);
    virtual ~Variable_assignment(){ delete lhs; delete rhs; }
    virtual const Constant* as_const() const;
    virtual string value() const;
};

class Array_element : public Expression {
protected:
    int line;
    const string id;
    const Expression* subscript;
public:
    Array_element(int line, const string nid, const Expression* nsub);
    virtual ~Array_element(){ delete subscript; }
    virtual const Constant* as_const() const;
    virtual string value() const;
};

class Return_statement : public Expression {
protected:
    int line;
    const Expression* rhs;
public:
    Return_statement(const int line, const Expression* rhs);
    virtual ~Return_statement(){ delete rhs; }
    virtual const Constant* as_const() const;
    static vector<pair<int, Return_statement*>> return_statements_list;
    virtual string value() const;
};

class Parentheses : public Expression {
protected:
    int line;
    const Expression* expr;
public:
    Parentheses(int line, const Expression* e);
    virtual ~Parentheses(){ delete expr; }
    virtual const Constant* as_const() const;
    virtual string value() const;
};

class Ternary_expression : public Expression {
protected:
    int line;
    const Expression* condition;
    const Expression* is_true;
    const Expression* is_false;
public:
    Ternary_expression(const int line, const Expression*, const Expression*, const Expression*);
    virtual ~Ternary_expression(){
        delete condition;
        delete is_true;
        delete is_false;
    }
    virtual const Constant* as_const() const;
    virtual string value() const;
};

class If_block : public Expression {
protected:
    int line;
    const Expression* condition;
    vector<const Expression*> code_block;
    const Expression* optional_else_or_else_if;
public:
    If_block(const int line, const Expression* cond, vector<const Expression*>& cb, const Expression* optional_else_elseif);
    virtual ~If_block(){
        delete condition;
        for (auto a : code_block) {
            delete a;
        }
        delete optional_else_or_else_if;
    }
    virtual const Constant* as_const() const;
    virtual string value() const;
};

class Else_if_block : public Expression {
protected:
    int line;
    const Expression* condition;
    vector<const Expression*> code_block;
    const Expression* optional_else_or_else_if;
public:
    Else_if_block(const int line, const Expression* cond, vector<const Expression*>& cb, const Expression* optional_else_elseif);
    virtual ~Else_if_block(){
        delete condition; delete optional_else_or_else_if;
        for (auto a : code_block) { delete a; }
    }
    virtual const Constant* as_const() const;
    virtual string value() const;
};

class Else_block : public Expression {
protected:
    int line;
    vector<const Expression*> code_block;
public:
    Else_block(const int line, vector<const Expression*>& cb);
    virtual ~Else_block(){
        for (auto a : code_block) { delete a; }
    }
    virtual const Constant* as_const() const;
    virtual string value() const;
};

class For_header : public Expression {
protected:
    int line;
    shared_ptr<Symbol> variable;
    // for (initializer; condition; assignment)
    const Expression* initializer;
    const Expression* condition;
    const Expression* assignment;
    // for (thing in other_thing)
    const Expression* range;
public:
    For_header(const int line, const shared_ptr<Symbol>& variable, const Expression* it_init, const Expression* cond, const Expression* assignment);
    For_header(const int line, string& variable, const Expression* range);
    virtual ~For_header(){
        delete initializer;
        delete condition;
        delete assignment;
        delete range;
    }
    virtual const Constant* as_const() const;
    virtual string value() const;
};

class For_loop : public Expression {
protected:
    int line;
    const Expression* header;
    const vector<const Expression*> code_block;
public:
    For_loop(const int line, const Expression* header, vector<const Expression*>& cb);
    virtual ~For_loop(){
        delete header;
        for (auto a : code_block){
            delete a;
        }
    }
    virtual const Constant* as_const() const;
    virtual string value() const;
};

class While_loop : public Expression {
protected:
    int line;
    const Expression* condition;
    const vector <const Expression*> code_block;
public:
    While_loop(const int line, const Expression* cond, vector<const Expression*>& cb);
    virtual ~While_loop(){
        delete condition;
        for (auto a : code_block){
            delete a;
        }
    }
    virtual const Constant* as_const() const;
    virtual string value() const { return ""; };
};

class Scope_block : public Expression {
protected:
    int line;
    vector<const Expression*> code_block;
public:
    Scope_block(int line, vector<const Expression*>& cb);
    virtual ~Scope_block(){
        for (auto a : code_block){
            delete a;
        }
    }
    virtual const Constant* as_const() const;
    virtual string value() const { return ""; }
};