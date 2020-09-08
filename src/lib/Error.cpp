// The Error class aims to help avoid the pitfalls of C++'s error reporting by being clear and concise.
// But since I'm no magician, I doubt I can do what rustc does and explain how to fix every error.
// (at the same time, if I have time to do it, I may as well try)

#include "Error.h"
#include <iostream>

using namespace std;

// TODO: work out new error code names
/* Error codes:
    DERBY: variable declared but not set
    NOHAT: variable used but not declared
    TRILBY: 
    CORKHAT: 
    VISOR: 
    FEDORA: 
    HATBOX: a variable set to null was operated on (+, -, *, /, etc.)
    BERET: 
    BUCKET: 
    BASEBALL_CAP:
    SNAPBACK: 
*/

int Error::error_count = 0;

void Error::error(Error_type error_type, int line_number){
    cout << "Error on line " << line_number << ": " << endl;
    switch (error_type){
        case VARIABLE_DECLARED_BUT_NOT_SET:
            cout << "Error code DERBY: a variable was declared, but not set." << endl;
            break;
        case VARIABLE_NOT_DEFINED:
            cout << "Error code NOHAT: a variable was used, but isn't defined anywhere in the program." << endl;
            break;
        case UNDEFINED_FUNCTION:
            cout << "Error code TRILBY: a function was called, but never declared." << endl;
    }
    error_count++;
}

int Error::num_errors(){
    return error_count;
}