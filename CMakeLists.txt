cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Compiler VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler-specific options
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(FLEX REQUIRED) # Find Flex
find_package(BISON REQUIRED) # Find Bison

# Include directories
include_directories(include)

# Gerar parser
BISON_TARGET(Syntactic src/lib/syntactic.y ${CMAKE_CURRENT_BINARY_DIR}/bison/syntactic.cpp
             DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/bison/syntactic.hpp
             COMPILE_FLAGS --defines=${CMAKE_CURRENT_BINARY_DIR}/bison/syntactic.hpp)

# Run Flex on lex.l -> generates lex.yy.cc
FLEX_TARGET(Lexer src/lib/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.cc)

ADD_FLEX_BISON_DEPENDENCY(Lexer Syntactic)

# Collect all lib files
file(GLOB_RECURSE LIBS "src/lib/*.cpp")

# Create the main executable
add_executable(compiler src/main.cpp ${LIBS} ${BISON_Syntactic_OUTPUTS} ${FLEX_Lexer_OUTPUTS})
target_include_directories(compiler PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/bison/)
add_executable(test_lexical_analyzer src/test_lexical_analyzer.cpp ${LIBS} ${BISON_Syntactic_OUTPUTS} ${FLEX_Lexer_OUTPUTS})
target_include_directories(test_lexical_analyzer PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/bison/)
add_executable(test_stack_scope src/test_scope_stack.cpp ${LIBS} ${BISON_Syntactic_OUTPUTS} ${FLEX_Lexer_OUTPUTS})
target_include_directories(test_stack_scope PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/bison/)

# Link directories and libraries (if needed)
# target_link_libraries(compiler ${CMAKE_THREAD_LIBS_INIT})

# Install targets (optional)
# install(TARGETS compiler DESTINATION bin)

# Testing (optional)
# enable_testing()
# if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests")
#     add_subdirectory(tests)
# endif()
