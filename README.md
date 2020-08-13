# Project Lemongrass, aka PRSE

A Python-esque programming language for C/C++ programmers.

Design goals:
* General function is inspired by Python, but uses syntax that mostly resembles other C-like languages.
* Make a language that works a lot like C/C++, but with some additional built-in features
* Use clean, straightforward grammar
* Don't use syntactic whitespace. Who puts <i>that</i> in a programming language?
* Take up little of the programmer's time on trivial matters. If R can do vectorization, we can too.

Notes:
* Recommendation from Todd: All values must be initialized on declaration. If the user fails to do so, throw out an error (possibly a `derby` error?). Apparently, this is called RAII.
* I want to do pass-by-reference by default. If you want to pass by value (meaning a copy of the value rather than the variable it's associated with), use `.copy()` 

# Dependencies:

## To run the PRSE compiler:
|Program|Used for|
|-|-|
|`prsec`|Compiling supplied PRSE source code (in files that end in '.prse') to C++ code|
|`clang++`|Compiling the above to a binary file|

## To build the PRSE compiler yourself:
|Program|Used for|
|-|-|
|`flex`|Creating the lexer which tokenizes a given '.prse' file|
|`bison`|Generating the `prse.tab.cpp` grammar file|
|`clang++`|Compiling C++ source files, `lex.yy.cpp` and `prse.tab.cpp` files into the prsec binary|
|`make`|Running the supplied Makefile in `/src/`|
