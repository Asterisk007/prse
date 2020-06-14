EXECUTABLE = prsec
C_COMPILER += clang
C_FLAGS += -std=c18 -g
CPP_COMPILER += clang++
CPP_FLAGS += -std=c++17 -g


all: prsec

all_versbose: prsec_verbose

prsec: prse.tab.cpp lex.yy.cpp Arglist.o
	$(CPP_COMPILER) $(CPP_FLAGS) -o prsec prse.tab.cpp lex.yy.cpp Arglist.o

prsec_verbose: prse.tab.cpp lex.yy.cpp
	$(CPP_COMPILER) $(CPP_FLAGS) -v -o prsec prse.tab.cpp lex.yy.cpp 

prse.tab.cpp: prse.y
	bison -d prse.y
	mv prse.tab.c prse.tab.cpp

lex.yy.cpp: prse.l
	flex prse.l
	mv lex.yy.c lex.yy.cpp

Arg_list.o: Arg_list.cpp Arg_list.h
	$(CPP_COMPILER) $(CPP_FLAGS) -c Arg_list.cpp

clean:
	rm -f lex.yy.* prse.tab.* prsec
