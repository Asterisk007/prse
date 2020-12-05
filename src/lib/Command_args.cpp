#include "Command_args.h"
#include <algorithm>

Command_args& Command_args::instance(){
    static Command_args inst;
    return inst;
}

bool Command_args::get_arg(const string& arg){
    if (find(valid_args.begin(), valid_args.end(), arg) != valid_args.end())
        return args[arg];
    else return false;
}

bool Command_args::set_arg(const string& arg){
    if (find(valid_args.begin(), valid_args.end(), arg) != valid_args.end()){
        args[arg] = true;
        return true;
    }
    else return false;
}

bool Command_args::VERBOSE(){
    return get_arg("--verbose");
}