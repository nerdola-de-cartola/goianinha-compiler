cmake_minimum_required(VERSION 3.10)
project(Compiler VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

include_directories(include)

# --- Bison ---
BISON_TARGET(Syntactic src/lib/syntactic.y
             ${CMAKE_CURRENT_BINARY_DIR}/bison/syntactic.cpp
             DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/bison/syntactic.hpp
             COMPILE_FLAGS --defines=${CMAKE_CURRENT_BINARY_DIR}/bison/syntactic.hpp)

# --- Flex ---
FLEX_TARGET(Lexer src/lib/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.cc)
ADD_FLEX_BISON_DEPENDENCY(Lexer Syntactic)

# --- Library (compiled only once) ---
file(GLOB_RECURSE LIB_SOURCES "src/lib/*.cpp")

add_library(compiler_lib STATIC
    ${LIB_SOURCES}
    ${BISON_Syntactic_OUTPUTS}
    ${FLEX_Lexer_OUTPUTS}
)

target_include_directories(compiler_lib
    PUBLIC
        include
        ${CMAKE_CURRENT_BINARY_DIR}/bison
)

# --- Executables ---
set(TESTS
    test_lexical_analyzer
    test_scope_stack
    test_ast
    test_syntactic_analyzer
    test_semantic_analyzer
)

add_executable(compiler src/main.cpp)
target_link_libraries(compiler PRIVATE compiler_lib)

foreach(test_name IN LISTS TESTS)
    add_executable(${test_name} src/${test_name}.cpp)
    target_link_libraries(${test_name} PRIVATE compiler_lib)
endforeach()
