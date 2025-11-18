#include <iostream>
#include "error.hpp"
#include "scope_stack.hpp"

#define red "\033[31m"
#define default "\033[0m"

std::string errorTypeToString(const ErrorType t) {
    switch (t) {
        case lexical:
            return std::string("Erro lexico");
        case syntactic:
            return std::string("Erro de sintaxe");
        case semantic:
            return std::string("Erro semântico");
    }

    return "";
}

void show_error(ErrorType type, const yy::location &loc, const std::string &msg, ScopeStack *stack) {
    std::cerr
            << red 
            << errorTypeToString(type)
            << " na linha "
            << loc.begin.line
            << ", coluna "
            << loc.begin.column
            << ": "
            << msg
            << default
            << std::endl;

    if (stack != nullptr)
        std::cout << std::endl << stack->toString() << std::endl;

    return exit(1);
}
