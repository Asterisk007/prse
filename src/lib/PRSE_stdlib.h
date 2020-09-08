#pragma once

#include <map>
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
    map<string, string> library;
public:
    Library();
    static Library& instance();
    bool lib_exists(string s);
    string get_lib(string s);
    void list_libs();
};
