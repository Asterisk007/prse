# Project Lemongrass, aka PRSE

A programming language for C programmers, without all the hassle of C.

Design goals:
* Make a language that works a lot like C/C++, but with some additional built-in features
* Use clean, straightforward grammar
* Don't use syntactic whitespace. Who puts <i>that</i> in a programming language?
* Take up little of the programmer's time on trivial matters. If R can do vectorization, we can too.

Notes:
* Recommendation from Todd: All values must be initialized on declaration. If the user fails to do so, throw out an error (possibly a `derby` error?).
* I want to do pass-by-reference by default. If you want to pass by value (meaning a copy of the value rather than the variable it's associated with), use `.copy()` 

# Dependencies:

## To run the PRSE compiler:
|Program|Used for|
|-|-|
|`prsec`|Compiling supplied PRSE source code (in files that end in '.prse') to C++ code|
|`clang++`|Compiling the above to a binary file|

## To build the PRSE compiler yourself:
### Dependencies
|Program|Used for|
|-|-|
|`flex`|Creating the lexer which tokenizes a given '.prse' file|
|`bison`|Generating the `prse.tab.cpp` grammar file|
|`clang++`|Compiling C++ source files, `lex.yy.cpp` and `prse.tab.cpp` files into the prsec binary|
|`make`|Running the supplied Makefile in `/src/`|

Installing dependencies:

Ubuntu
```
apt install flex bison clang make
```

Arch
```
pacman -S flex bison clang make
```

### Building and Installing

First, clone this repository:
```
git clone https://gitlab.com/Asterisk007/prse.git
```
or, if you prefer to use SSH:
```
git clone git@gitlab.com:Asterisk007/prse.git
```

Then, run the following commands:
```
cd prse/src/
make
sudo make install
```

This will install the PRSE compiler to /usr/bin.
