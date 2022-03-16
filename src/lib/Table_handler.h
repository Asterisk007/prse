#pragma once
#include "Symbol_table.h"
#include <vector>
#include <stdexcept>

class Table_handler {
public:
    static Table_handler& instance();
    void push_table(); // Increase scope.
    void pop_table(); // Decrease scope.
    bool insert(shared_ptr<Symbol> symbol);
    bool defined_in_current_scope(const string& symbol_name);
    void erase(const string& name);
    shared_ptr<Symbol> lookup(const string& name);
private:
    vector<shared_ptr<Symbol_table>> tables;
    Table_handler(){
        push_table();
    }
};