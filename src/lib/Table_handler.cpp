#include "Table_handler.h"
#include <iostream>

using std::cout; using std::endl;

Table_handler& Table_handler::instance(){
    static Table_handler inst;
    if (inst.tables.size() == 0){
        inst.push_table();
    }
    cout << "table handler has " << inst.tables.size() << " tables" << endl;
    return inst;
}

void Table_handler::push_table(){
    shared_ptr<Symbol_table> t = shared_ptr<Symbol_table>(new Symbol_table);
    tables.push_back(t);
}

void Table_handler::pop_table(){
    if ((int)tables.size() > 1){
        tables.pop_back();
    } else throw underflow_error("Can't pop global symbol table.");
}

bool Table_handler::insert(shared_ptr<Symbol> symbol){
    return tables.back()->add_symbol(symbol);
}

shared_ptr<Symbol> Table_handler::lookup(const string& name){
    shared_ptr<Symbol> ret;
    for (int i = (int)tables.size()-1; i >= 0; i--){
        if ((ret = tables[i]->lookup(name)) != nullptr) { return ret; }
    }
    return nullptr;
}

bool Table_handler::defined_in_current_scope(const string& name){
    if (tables.back()->lookup(name) != nullptr) { return true; }
    return false;
}

void Table_handler::erase(const string& name){
    if (lookup(name) != nullptr){
        for (int i = (int)tables.size()-1; i >= 0; i--){
            if (tables[i]->lookup(name) != nullptr) { tables[i]->erase(name); }
        }
    }
}