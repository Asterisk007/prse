EXECUTABLE = prsec
C_COMPILER += clang
C_FLAGS += -std=c18
CPP_COMPILER += clang++
CPP_FLAGS += -std=c++17


all: prsec

all_versbose: prsec_verbose

prsec: prse.tab.cpp lex.yy.cpp
	$(CPP_COMPILER) $(CPP_FLAGS) -o prsec prse.tab.cpp lex.yy.cpp

prsec_verbose: prse.tab.cpp lex.yy.cpp
	$(CPP_COMPILER) $(CPP_FLAGS) -v -o prsec prse.tab.cpp lex.yy.cpp 

prse.tab.cpp: prse.y
	bison -d prse.y
	mv prse.tab.c prse.tab.cpp

lex.yy.cpp: prse.l
	flex prse.l
	mv lex.yy.c lex.yy.cpp

clean:
	rm -f lex.yy.* prse.tab.* prsec
