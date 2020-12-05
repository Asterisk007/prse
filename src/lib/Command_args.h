#pragma once
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

class Command_args {
public:
    bool get_arg(const string& argument);
    bool set_arg(const string& argument);
    static Command_args& instance();
    bool VERBOSE();
private:
    map<string, bool> args;
    vector<string> valid_args;
    Command_args(){
        args = map<string, bool>();
        // Add valid arguments to the list.
        valid_args.insert(valid_args.end(),
            {
                "--cpp",
                "-g",
                "-h",
                "--help",
                "-o",
                "-v",
                "--verbose",
                "--sacrifice",
                "--sacrifice=anyways",
                "--version",
                "--nocodes",
                "-w",
            });
        for (auto a : valid_args){
            args[a] = false;
        }
    }
};