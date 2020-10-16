#include "Symbol.h"

Symbol::Symbol(std::string& name, PRSE_type& type, bool is_null)
:name(name), type(type), size(0), is_null(is_null){}

Symbol::Symbol(std::string& name, PRSE_type& type, bool is_null, int& size)
:name(name), type(type), size(size), is_null(is_null){}

string Symbol::get_name() const{
    return name;
}

PRSE_type Symbol::get_type() const{
   return type;
}

int Symbol::get_size() const{
    return size;
}

Constant* Symbol::as_const() {
   return new Constant(type, name);
}