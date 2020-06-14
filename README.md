# PRSE
A programming language for C programmers, without all the hassle of C.

Design goals:
* Make a language that works a lot like C/C++, but with some additional built-in features
* Use clean, straightforward grammar
* Don't use syntactic whitespace. Who puts <i>that</i> in a programming language?
* Take up little of the programmer's time on trivial matters. If R can do vectorization, we can too.

Notes:
* Recommendation from Todd: All values must be initialized on declaration. If the user fails to do so, throw out an error (possibly a derby error?).
* I want to do pass-by-reference by default. If you want to pass by value (meaning a copy of the value rather than the variable it's associated with), use `.copy()`
