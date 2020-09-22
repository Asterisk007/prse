#pragma once

#include "PRSE_type.h"
#include "Symbol.h"
#include <vector>
#include <string>

using namespace std;

class Constant {
private:
    PRSE_type type;
    string value;
    vector<string> value_list;
public:
    Constant();
    ~Constant();
    Constant(PRSE_type type, string value);
    Constant(PRSE_type type, vector<string> value_list);
    PRSE_type get_type();
    string get_value();
    vector<string> get_value_list();
};