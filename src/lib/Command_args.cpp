#include "Command_args.h"
#include <stdio.h>
#include <algorithm>

Command_args& Command_args::instance(){
    static Command_args inst;
    return inst;
}

void Command_args::print_help_text(){
    // Find longest key string for fancy printing.
    int longest = 0;
    for (auto it = arg_help.begin(); it != arg_help.end(); it++){
        if (int(it->first.length()) > longest)
            longest = it->first.length();
    }
    // Print help text.
    for (auto it = arg_help.begin(); it != arg_help.end(); it++){
        printf("    %-*s%s\n", longest+5, it->first.c_str(), it->second.c_str());
    }
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
    return (get_arg("--verbose") || get_arg("-v"));
}