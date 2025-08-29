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
    Function(std::string function_name, VariableTypes return_type);
    ~Function();

    std::string get_name();
    VariableTypes get_return_type();
    Variable* get_parameter(std::string name);

    void add_parameter(Variable var);

    std::string toString();
};

#endif // FUNCTION_HPP
