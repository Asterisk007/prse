#include "Symbol.h"
#include "expressions/Constant.h"

Symbol::Symbol(const Symbol& s)
:name(s.get_name()), type(s.get_type()), size(s.get_size()){}

Symbol::Symbol(std::string& name, PRSE_type& type)
:name(name), type(type), size(0){}

Symbol::Symbol(std::string& name, PRSE_type& type, int size)
:name(name), type(type), size(size){}

string Symbol::get_name() const{
    return name;
}

PRSE_type Symbol::get_type() const{
   return type;
}

int Symbol::get_size() const{
    return size;
}

const Constant* Symbol::as_const() const {
   return new Constant(type, name);
}