#ifndef MIPS_GENERATOR_HPP
#define MIPS_GENERATOR_HPP

#include <iostream>
#include <vector>
#include "node.hpp"

class MipsGenerator {
private:
public:
    std::vector<std::string> data_segments;
    MipsGenerator();
    ~MipsGenerator();
};


std::string generate_code(Node *root);
void print_code(const std::string &code);
void run_code(const std::string &code);


#endif //MIPS_GENERATOR_HPP
