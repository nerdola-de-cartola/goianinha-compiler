#include <iostream>
#include "syntactic.hpp"

#define red "\033[31m"
#define default "\033[0m"

enum ErrorType {
    lexical,
    syntactic,
    semantc,
};

std::string errorTypeToString(const ErrorType t) {
    switch (t) {
        case lexical:
            return std::string("Erro lexico na linha ");
        case syntactic:
            return std::string("Erro de sintaxe na linha ");
        case semantc:
            return std::string("Erro semântico na linha ");
    }

    return "";
}

void show_error(ErrorType type, const yy::Parser::location_type &loc, const std::string &msg) {
    std::cerr
            << red 
            << errorTypeToString(type)
            << loc.begin.line
            << ", coluna "
            << loc.begin.column
            << ": "
            << msg
            << default
            << std::endl;

    return exit(1);
}
