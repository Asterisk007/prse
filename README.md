# PRSE
## A Python-esque programming language for C/C++ programmers.
### Documentation: [click here](https://asterisk007.gitlab.io/prse/)

Design goals:
* Functionality inspired by:
   - Python: for loops, dictionaries, `range()`, easy conversion between data types
   - C/C++: general look
   - R: vectorization
   - More to explore...
* Use clean, straightforward grammar
* Don't use syntactic whitespace.

Notes:
* Recommendation from Todd: All values must be initialized on declaration. If the user fails to do so, throw out an error (`error core derby: variable declared with no value set.`)
    - Apparently, this is called RAII.
* I want to do pass-by-reference by default. If you want to pass by value (meaning a copy of the value rather than the variable it's associated with), use `.copy()` 

Extras to be implemented later:
* CLA option: --sacrifice
    - Sacrifice a file to the PRSE compiler
    - If the code compiles, file is spared
    - If any errors are found, the file is destroyed
    - Hardcore mode: --sacrifice=anyways sacrifices a file regardless of errors, so that you are left with only a binary or nothing at all.

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
