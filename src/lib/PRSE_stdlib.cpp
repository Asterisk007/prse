#include "PRSE_stdlib.h"
#include <iostream>

// Function to initialize the library map
Library::Library(){
    library["io"] = "iostream";
    library["math"] = "math";
}

Library& Library::instance(){
    static Library instance;
    return instance;
}

bool Library::lib_exists(string s){
    if (library.find(s) != library.end()) {
        return true;
    } else {
        return false;
    }
}

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