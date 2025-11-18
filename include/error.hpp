#ifndef ERROR_HPP
#define ERROR_HPP

#include <iostream>
#include "location.hh"

enum ErrorType {
    lexical,
    syntactic,
    semantic,
};

enum Result {
    OK,
    ERROR
};

class ScopeStack;

std::string errorTypeToString(const ErrorType t);
void show_error(ErrorType type, const yy::location &loc, const std::string &msg, ScopeStack *stack = nullptr);

#endif //ERROR_HPP