#include "Table_handler.h"
#include <iostream>
#include <assert.h>

using std::cout; using std::endl;

Table_handler& Table_handler::instance(){
    static Table_handler inst;
    if (inst.tables.size() == 0){
        inst.push_table();
    }
    //cerr << "Table_handler::instance(): Global symbol table has " << inst.tables.size() << " table(s)" << endl;
    //cout << "table handler has " << inst.tables.size() << " tables" << endl;
    return inst;
}

void Table_handler::push_table(){
    shared_ptr<Symbol_table> t = shared_ptr<Symbol_table>(new Symbol_table);
    tables.push_back(t);
    //cerr << "Global symbol table now has " << tables.size() << " table(s)" << endl;
}

void Table_handler::pop_table(){
    //assert(false);
    //cerr << "Trying to pop one table, current number of tables: " << tables.size() << endl;
    if ((int)tables.size() > 1){
        //cerr << "Table handler popping off one table" << endl;
        tables.pop_back();
    } else throw underflow_error("Can't pop global symbol table.");
}

bool Table_handler::insert(shared_ptr<Symbol> symbol){
    //cerr << "Table_handler::insert(): Global symbol table has " << tables.size() << " table(s)" << endl;
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