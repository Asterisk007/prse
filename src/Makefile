# Compiler settings
EXECUTABLE = prsec
EXECUTABLE_VERBOSE = prsec_verbose
C_COMPILER += clang
C_FLAGS += -std=c18 -g
BISON = bison
BISON_FLAGS = --color=auto -Wcex
FLEX = flex
FLEX_FLAGS = 
CPP_COMPILER += clang++
CPP_FLAGS += -std=c++20 -g
# CPP_FLAGS += -target x86_64-pc-windows-gnu
# Source file vars
LIB_DIR := ./lib
OBJ_DIR := ./obj
TEST_FILES := ./examples/*.prse
LIB_FILES := $(wildcard $(LIB_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(LIB_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(LIB_FILES))

all: $(EXECUTABLE)

all_versbose: $(EXECUTABLE_VERBOSE)

# A command to build and subsequently check
# that the build prsec is operating optimally
# (i.e. without memory leaks)
build_and_check: $(EXECUTABLE)
	make check

check: $(TEST_FILES)
	for f in $(TEST_FILES); do \
		valgrind ./prsec --verbose --check $$f; \
	done

objects:
	make $(OBJ_FILES)

# Rebuild the executable from scratch:
rebuild:
	make clean
	make all

# Same as above, verbose
rebuild_verbose:
	make clean
	make all_verbose

lib: $(OBJ_FILES)

$(EXECUTABLE): prse.cpp lex.yy.cpp prse.tab.cpp $(OBJ_FILES)
	@echo Building compiler...
	@$(CPP_COMPILER) $(CPP_FLAGS) -o $@ $^

$(EXECUTABLE_VERBOSE): prse.cpp lex.yy.cpp prse.tab.cpp $(OBJ_FILES)
	@echo "Building compiler..."
	$(CPP_COMPILER) $(CPP_FLAGS) -v -o $(EXECUTABLE) $^

grammar: prse.tab.cpp lex.yy.cpp

prse.tab.cpp: prse.y
	$(BISON) --defines=prse.tab.h $(BISON_FLAGS) -o prse.tab.cpp prse.y

lex.yy.cpp: prse.l
	$(FLEX) -o lex.yy.cpp $(FLEX_FLAGS) prse.l

$(OBJ_DIR)/%.o: $(LIB_DIR)/%.cpp $(LIB_DIR)/%.h | $(OBJ_DIR)
	-$(CPP_COMPILER) $(CPP_FLAGS) -c -o $@ $<

$(OBJ_DIR):
	@if [ ! -d "./obj/" ]; then \
		mkdir -p $(OBJ_DIR); \
	fi

clean:
	rm -f lex.yy.* prse.tab.* obj/*.o $(EXECUTABLE)
