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
	mkdir -p $(OBJ_DIR)

all_versbose: $(EXECUTABLE_VERBOSE)

# A command to build and subsequently check
# that the build prsec is operating optimally
# (i.e. without memory leaks)
	#build_check: $(EXECUTABLE)
	#	valgrind ./prsec

# Rebuild the executable from scratch:
rebuild:
	make clean
	make all

# Same as above, verbose
rebuild_verbose:
	make clean
	make all_verbose

objects: $(OBJ_FILES)
	mkdir -p $(OBJ_DIR)

$(EXECUTABLE): lex.yy.cpp prse.tab.cpp $(OBJ_FILES)
	$(CPP_COMPILER) $(CPP_FLAGS) -o $@ $^

$(EXECUTABLE_VERBOSE): lex.yy.cpp prse.tab.cpp $(OBJ_FILES)
	$(CPP_COMPILER) $(CPP_FLAGS) -v -o $(EXECUTABLE) $^

prse.tab.cpp: prse.y
	bison -d prse.y
	mv prse.tab.c prse.tab.cpp

lex.yy.cpp: prse.l
	flex prse.l
	mv lex.yy.c lex.yy.cpp

$(OBJ_DIR)/%.o: $(LIB_DIR)/%.cpp $(LIB_DIR)/%.h
	$(CPP_COMPILER) $(CPP_FLAGS) -c -o $@ $<

clean:
	rm -f lex.yy.* prse.tab.* obj/*.o prsec
