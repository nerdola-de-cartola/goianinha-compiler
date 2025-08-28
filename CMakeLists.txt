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

# Find Flex
find_package(FLEX REQUIRED)

# Include directories
include_directories(include)

# Run Flex on lex.l -> generates lex.yy.cc
FLEX_TARGET(Lexer lex.l ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.cc)

# Collect all source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create the main executable
add_executable(compiler ${SOURCES} ${FLEX_Lexer_OUTPUTS})

# Link directories and libraries (if needed)
# target_link_libraries(compiler ${CMAKE_THREAD_LIBS_INIT})

# Install targets (optional)
# install(TARGETS compiler DESTINATION bin)

# Testing (optional)
# enable_testing()
# if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests")
#     add_subdirectory(tests)
# endif()
