#pragma once

#include "PRSE_type.h"
#include "Constant.h"
#include <string>

using namespace std;

class Symbol {
private:
    std::string name;
    int size;
    PRSE_type type;
public:
    // Constructor for single variables
    Symbol(std::string& name, PRSE_type& type, bool is_null);
    // Constructor for array   
    Symbol(std::string& name, PRSE_type& type, bool is_null, int& size);

    bool is_null;
    string get_name() const;
    // Getter for the object's Type
    PRSE_type get_type() const;
    int get_size() const;

    Constant* as_const();
};

class Function : public Symbol {
private:
    vector<Symbol> parameters;
public:
    Function(string& name, PRSE_type& type, vector<Symbol>& params):Symbol(name, type, false), parameters(params){}
};