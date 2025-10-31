#ifndef ERROR_HPP
#define ERROR_HPP

#include <iostream>
#include "syntactic.hpp"

enum ErrorType {
    lexical,
    syntactic,
    semantc,
};

enum Result {
    OK,
    ERROR
};


std::string errorTypeToString(const ErrorType t);
void show_error(ErrorType type, const yy::Parser::location_type &loc, const std::string &msg);

#endif //ERROR_HPP