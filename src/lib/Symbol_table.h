#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include "Symbol.h"

using namespace std;

class Symbol_table {
private:
    unordered_map<string, shared_ptr<Symbol>> symbols;
public:
    Symbol_table();
    shared_ptr<Symbol> lookup(const string& symbol_name);

    bool add_symbol(shared_ptr<Symbol> symbol); // Add symbol to table.
    void erase(const string& name); // Remove symbol from table.
};