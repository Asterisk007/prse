#include "Symbol.h"

Symbol::Symbol(std::string& name, PRSE_type& type, bool is_null)
:name(name), type(type), size(0), is_null(is_null){}

Symbol::Symbol(std::string& name, PRSE_type& type, bool is_null, int& size)
:name(name), type(type), size(size), is_null(is_null){}

/* Symbol::Symbol(std::string& n, bool* v):
   name(n), count(0), type(BOOL), value(v){}
Symbol::Symbol(std::string& n, bool* v, int c):
   name(n), count(0), type(BOOL), value(v){}

Symbol::Symbol(std::string& n, int* v):
   name(n), count(0), type(INT), value(v){}
Symbol::Symbol(std::string& n, int* v, int c):
   name(n), count(0), type(INT), value(v){}
   
Symbol::Symbol(std::string& n, char* v):
   name(n), count(0), type(CHAR), value(v){}
Symbol::Symbol(std::string& n, char* v, int c):
   name(n), count(0), type(CHAR), value(v){}

Symbol::Symbol(std::string& n, double* v):
   name(n), count(0), type(DOUBLE), value(v){}
Symbol::Symbol(std::string& n, double* v, int c):
   name(n), count(0), type(DOUBLE), value(v){}

Symbol::Symbol(std::string& n, std::string* v):
   name(n), count(0), type(STRING), value(v){}
Symbol::Symbol(std::string& n, std::string* v, int c):
   name(n), count(0), type(STRING), value(v){} */

string Symbol::get_name() const{
    return name;
}

PRSE_type Symbol::get_type() const{
   return type;
}

int Symbol::get_size() const{
    return size;
}