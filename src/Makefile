all: prse_c

all_versbose: prse_c_verbose

prse_c: prse.tab.cpp lex.yy.cpp
	clang++ -std=c++17 -o prse_c prse.tab.cpp lex.yy.cpp

prse_c_verbose: prse.tab.cpp lex.yy.cpp
	clang++ -v -std=c++17 -o prse prse.tab.cpp lex.yy.cpp 

prse.tab.cpp: prse.y
	bison -d prse.y
	mv prse.tab.c prse.tab.cpp

lex.yy.cpp: prse.l
	flex prse.l
	mv lex.yy.c lex.yy.cpp

clean:
	rm -f lex.yy.* prse.tab.* prse_c
