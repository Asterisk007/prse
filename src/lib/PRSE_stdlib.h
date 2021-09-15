#pragma once

#include <unordered_map>
#include <string>

using namespace std;
// A map of all available standard libraries
// PRSE's standard library includes:
/* - io
   - math
   - algorithm
   - vector
   - map
   - (more C++ stdlib)
*/

class Library {
private:
    unordered_map<string, string> library;
public:
    Library();
    static Library& instance();
    unordered_map<string, bool> lib_used;
    unordered_map<string, bool> lib_required; // C++ standard libraries that need to be included.
                                              // included on a need-to-know basis, so as to avoid bloating
                                              // the file.
    bool using_library(const string& lib);
    bool lib_exists(const string& s);
    string get_lib(string s);
    void list_libs();
};
