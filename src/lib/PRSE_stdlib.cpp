#include "PRSE_stdlib.h"
#include <iostream>

// Function to initialize the library map
Library::Library(){
    library["io"] = "iostream";
    library["math"] = "math";
    library["algorithm"] = "algorithm";
    for (auto it = library.begin(); it != library.end(); it++)
        lib_used[it->first] = false;
    lib_required["vector"] = false;
}

Library& Library::instance(){
    static Library instance;
    return instance;
}

bool Library::using_library(const string& lib){
    return lib_used[lib];
}

bool Library::lib_exists(const string& s){
    if (library.find(s) != library.end()) {
        return true;
    } else {
        return false;
    }
}

/* void Library::need_library(const string& s){
    
} */

string Library::get_lib(string s){
    if (lib_exists(s) == true){
        return library[s];
    } else return "";
}

void Library::list_libs(){
    for (auto it = library.begin(); it != library.end(); it++){
        cout << it->first << ": " << it->second << endl;
    }
}