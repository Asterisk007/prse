#include "Variable.h"
#include "Expression_core.h"

// Variables
Variable::Variable(int line, const string symbol_name)
:line(line), symbol(symbol_name){}

const Constant* Variable::as_const() const {
    Table_handler& th = Table_handler::instance();
    //cerr << "Looking up symbol: " << symbol << endl;
    auto sym = th.lookup(symbol);
    if (sym != nullptr){
        return ret(new Constant(sym->get_type(), sym->get_name()));
    } else {
        Error::error(Error::VARIABLE_NOT_DEFINED_IN_SCOPE, symbol, "", "", line);
    }
    return ret(new Constant(PRSE_type::NO_TYPE, ""));
}

// Arrays
Array::Array(int line, const string s)
:Variable(line, s){}

const Constant* Array::as_const() const {
    Table_handler& th = Table_handler::instance();
    auto sym = th.lookup(symbol);
    if (sym != nullptr){
        if (sym->get_size() == 0) {
            Error::error(Error::VARIABLE_IS_NOT_AN_ARRAY, symbol, "", "", line);
        }
        else return ret(new Constant(sym->get_type(), sym->get_name()));
    } else {
        Error::error(Error::VARIABLE_NOT_DEFINED_IN_SCOPE, symbol, "", "", line);
    }
    return ret(new Constant(PRSE_type::NO_TYPE, ""));
}