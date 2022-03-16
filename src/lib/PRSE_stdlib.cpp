#include "PRSE_stdlib.h"
#include "Declared_list.h"
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

void Library::add_lib(const string& lib){
    // Add functions for each respective standard library

    // IO: File I/O
    if (lib == "io") {
        #ifdef DEBUG
            printf("TODO: Add file I/O functions");
        #endif
    }
    // Math: trig, sqrt, etc.
    else if (lib == "math") {
        #ifdef DEBUG
            printf("TODO: Add math functions");
        #endif
    }
    // Shouldn't encounter an error if we check beforehand that
    // the library exists in our standard library, but it doesn't
    // hurt to throw out an error.
    else {
        fprintf(stderr, "Error:Library.cpp::add_lib: No such library '%s'", lib.c_str());
    }
}