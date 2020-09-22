#pragma once

#include "PRSE_type.h"
#include <string>

using namespace std;

class Symbol {
private:
    std::string name;
    int size;
    PRSE_type type;
    /* union Value {
        bool *bool_pointer;
        int *int_pointer;
        char *char_pointer;
        double *double_pointer;
        std::string *string_pointer;
        // TODO: Add support for complex types (i.e. classes)

        Value(bool *val) : bool_pointer(val) {}
        Value(int *val) : int_pointer(val) {}
        Value(char *val) : char_pointer(val) {}
        Value(double *val) : double_pointer(val) {}
        Value(std::string *val) : string_pointer(val) {}
        // TODO: Add void* type here
    }; */
protected:
    //Value value;
public:
    // Constructor for single variables
    Symbol(std::string& name, PRSE_type& type, bool is_null);
    // Constructor for array   
    Symbol(std::string& name, PRSE_type& type, bool is_null, int& size);


    // TODO: Find out if these are actually necessary.
    /* Symbol(std::string &n, bool *v);
    Symbol(std::string &n, int *v);
    Symbol(std::string &n, char *v);
    Symbol(std::string &n, double *v);
    Symbol(std::string &n, std::string *v);

    Symbol(std::string &n, bool *v, int c);
    Symbol(std::string &n, int *v, int c);
    Symbol(std::string &n, char *v, int c);
    Symbol(std::string &n, double *v, int c);
    Symbol(std::string &n, std::string *v, int c); */
    // TODO: function for getting symbol value (or just symbol as usable data)
    //Value* getValue();
    bool is_null;
    string get_name() const;
    // Getter for the object's Type
    PRSE_type get_type() const;
    int get_size() const;
};
