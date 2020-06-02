all: prse.tab.o lex.yy.o 
	clang++ -std=c++17 -o prse_c prse.tab.o lex.yy.o 

all_verbose: prse.tab.o lex.yy.o
	clang++ -v -std=c++17 -o prse prse.tab.o lex.yy.o 

prse.tab.o:
	cp prse.y prse.ypp
	bison -d prse.ypp
	rm prse.ypp
	clang++ prse.tab.cpp -c -o prse.tab.o

lex.yy.o:
	flex++ prse.l
	clang++ lex.yy.cc -c -o lex.yy.o

clean:
	rm -f lex.yy.cc prse.tab.cpp prse.tab.hpp prse.tab.o prse_c
