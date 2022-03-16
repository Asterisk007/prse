#include "Symbol_table.h"

Symbol_table::Symbol_table(){
    symbols = unordered_map<string, shared_ptr<Symbol>>();
}

shared_ptr<Symbol> Symbol_table::lookup(const string& name){
    auto it = symbols.find(name);
    if (it == symbols.end()) { return nullptr; }
    else return it->second;
}

bool Symbol_table::add_symbol(shared_ptr<Symbol> symbol){
    if (lookup(symbol->get_name()) == nullptr){
        symbols[symbol->get_name()] = symbol;
        return true;
    } else return false;
}

void Symbol_table::erase(const string& name){
    symbols.erase(name);
}