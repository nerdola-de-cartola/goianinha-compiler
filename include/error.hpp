#include <iostream>
#include "syntactic.hpp"

enum ErrorType {
    lexical,
    syntactic,
    semantc,
};

std::string errorTypeToString(const ErrorType t);
void show_error(ErrorType type, const yy::Parser::location_type &loc, const std::string &msg);