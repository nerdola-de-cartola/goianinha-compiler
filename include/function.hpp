#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <vector>
#include <string>
#include "variable.hpp"

class Function {
private:
    std::string name;
    std::vector<Variable> parameters;
    VariableTypes return_type;

public:
    Function();
    ~Function();
};

#endif // FUNCTION_HPP
