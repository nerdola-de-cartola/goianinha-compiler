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
    std::string line_msg;

    if (loc.begin.line != loc.end.line) {
        line_msg = " da linha "
            + std::to_string(loc.begin.line)
            + " coluna "
            + std::to_string(loc.begin.column)
            + " até a linha "
            + std::to_string(loc.end.line)
            + " coluna "
            + std::to_string(loc.end.column);
     } else {
        line_msg = " na linha "
            + std::to_string(loc.begin.line)
            + ", colunas "
            + std::to_string(loc.begin.column)
            + " - "
            + std::to_string(loc.end.column);
     }

    std::cerr
            << red 
            << errorTypeToString(type)
            << line_msg
            << ": "
            << msg
            << default
            << std::endl;

    if (stack != nullptr) {
        std::cerr 
            << "Pilha de escopos no momento do erro."
            << std::endl
            << stack->toString()
            << std::endl;
    }

    return exit(1);
}
