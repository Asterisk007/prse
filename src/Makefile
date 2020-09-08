# Compiler settings
EXECUTABLE = prsec
EXECUTABLE_VERBOSE = prsec_verbose
C_COMPILER += clang
C_FLAGS += -std=c18 -g
CPP_COMPILER += clang++
CPP_FLAGS += -std=c++20 -g
# Source file vars
LIB_DIR := ./lib
OBJ_DIR := ./obj
TEST_FILES := $(wildcard ../examples/*.prse)
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
		valgrind ./prsec $f; \
	done

# Rebuild the executable from scratch:
rebuild:
	make clean
	make all

# Same as above, verbose
rebuild_verbose:
	make clean
	make all_verbose

lib: $(OBJ_FILES)
	@if [ ! -d "./lib/" ]; then \
		mkdir -p $(OBJ_DIR); \
	fi

$(EXECUTABLE): prse.cpp lex.yy.cpp prse.tab.cpp $(OBJ_FILES)
	$(CPP_COMPILER) $(CPP_FLAGS) -o $@ $^

$(EXECUTABLE_VERBOSE): prse.cpp lex.yy.cpp prse.tab.cpp $(OBJ_FILES)
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

# Major updates gain a new version
# This includes new syntax and new language capabilities
increment_version: $(LIB_DIR)/Version.h

# Smaller updates gain a new subversion
# This includes optimizations to how the compiler implements
# translation from PRSE to CPP. Also includes small but significant changes
# to syntax (i.e. some new way to do an existing thing, or a revision to existing syntax)
increment_subversion: $(LIB_DIR)/Version.h


# Reserved for very minor changes. Can't think of where this might come into play.
# Still useful in case it's needed.
increment_subsubversion: $(LIB_DIR)/Version.h


# When working on the parser, sufficient additions or subtractions to the overall code
# warrant an increment in revision number. This is somewhat analogous to a build number,
# but we don't have to increment it each time we attempt to build. Best used in the event
# where we're going to make a commit and want to mark this as a change to the compiler
# without calling it a whole new thing for the language.
increment_revision: $(LIB_DIR)/Version.h
