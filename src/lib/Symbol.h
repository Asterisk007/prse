#pragma once

#include "PRSE_type.h"
//#include "Constant.h"
#include <string>
//#include <vector>
using namespace std;

class Constant;
class Symbol {
private:
    std::string name;
    int size;
    PRSE_type type;
public:
    // Constructor from existing symbol
    Symbol(const Symbol& s);
    
    // Constructor for single variables
    Symbol(std::string& name, PRSE_type& type);
    // Constructor for array   
    Symbol(std::string& name, PRSE_type& type, int size);

    string get_name() const;
    // Getter for the object's Type
    PRSE_type get_type() const;
    int get_size() const;

    const Constant* as_const() const;
};

/*class Function : public Symbol {
private:
    vector<Symbol> parameters;
public:
    Function(string& name, PRSE_type& type, vector<Symbol>& params):Symbol(name, type), parameters(params){}
};*/