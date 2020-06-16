# Compiler settings
EXECUTABLE = prsec
EXECUTABLE_VERBOSE = prsec_verbose
C_COMPILER += clang
C_FLAGS += -std=c18 -g
CPP_COMPILER += clang++
CPP_FLAGS += -std=c++17 -g
# Source file vars
LIB_DIR := ./lib
OBJ_DIR := ./obj
LIB_FILES := $(wildcard $(LIB_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(LIB_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(LIB_FILES))

all: $(EXECUTABLE)

all_versbose: $(EXECUTABLE_VERBOSE)

$(EXECUTABLE): prse.tab.cpp lex.yy.cpp $(OBJ_FILES)
	$(CPP_COMPILER) $(CPP_FLAGS) -o $@ $^

$(EXECUTABLE_VERBOSE): prse.tab.cpp lex.yy.cpp $(OBJ_FILES)
	$(CPP_COMPILER) $(CPP_FLAGS) -v -o $(EXECUTABLE) $^

prse.tab.cpp: prse.y
	bison -d prse.y
	mv prse.tab.c prse.tab.cpp

lex.yy.cpp: prse.l
	flex prse.l
	mv lex.yy.c lex.yy.cpp

$(OBJ_DIR)/%.o: $(LIB_DIR)/%.cpp $(LIB_DIR)/%.h
	mkdir -p $(OBJ_DIR)
	$(CPP_COMPILER) $(CPP_FLAGS) -c -o $@ $<

clean:
	rm -f lex.yy.* prse.tab.* obj/*.o prsec
