#pragma once
#include <iostream>
#include <map>

using namespace std;

class Command_args {
public:
    bool check_arg(string argument);
    bool set_arg(string argument);
    Command_args();
    static Command_args& instance();
private:
    map<string, string> args;
};