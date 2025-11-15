#ifndef MIPS_GENERATOR_HPP
#define MIPS_GENERATOR_HPP

#include <iostream>
#include "node.hpp"

std::string generate_code(Node *root);
void print_code(const std::string &code);
void run_code(const std::string &code);


#endif //MIPS_GENERATOR_HPP
