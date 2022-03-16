cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_COMPILER "/usr/bin/g++")
add_compile_options(
    -Wall
    -g
    -Wno-unused-function
)

if(CMAKE_BUILD_TYPE EQUAL Release)
# Compiler should NOT produce errors for release build
add_compile_options(-Werror)
set(BISON_ARGS "-Wall -Werror")
else()
message(WARNING "Remember to set -DCMAKE_BUILD_TYPE=Release if creating a release build.")
add_definitions(-DDEBUG)
set(CMAKE_BUILD_TYPE Debug)
set(BISON_ARGS "-Wall")
endif()

project(prsec VERSION 0.0.3)

# Bison and Flex files
find_package(BISON 3.7.1)
find_package(FLEX 2.6.4)

bison_target(prse_parser ${CMAKE_CURRENT_SOURCE_DIR}/prse.y ${CMAKE_CURRENT_SOURCE_DIR}/prse.tab.cpp
    COMPILE_FLAGS "${BISON_ARGS} -Wcounterexamples"
)
flex_target(prse_scanner ${CMAKE_CURRENT_SOURCE_DIR}/prse.l ${CMAKE_CURRENT_SOURCE_DIR}/lex.yy.cpp)

add_flex_bison_dependency(prse_scanner prse_parser)

add_executable(${PROJECT_NAME} prse.cpp ${BISON_prse_parser_OUTPUTS} ${FLEX_prse_scanner_OUTPUTS})

file(GLOB_RECURSE lib_sources ./lib/*.cpp)
#file(GLOB expr_sources ./lib/expressions/*.cpp)
add_library(lib ${lib_sources})
#add_library(expressions ${expr_sources})

target_link_libraries(prsec PUBLIC lib)

set(CMAKE_CXX_STANDARD 2a)
set(CMAKE_CXX_STANDARD_REQUIRED True)