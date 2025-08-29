#ifndef SCOPE_HPP
#define SCOPE_HPP

#include <vector>
#include "variable.hpp"
#include "function.hpp"

class Scope {
private:
    std::vector<Variable> variables;
    std::vector<Function> functions;

public:
    Scope();
    ~Scope();
    
    void add_function(Function f);
    Function* get_function(std::string name);

    void add_variable(Variable var);
    Variable* get_variable(std::string name);

    std::string toString();
};

#endif // SCOPE_HPP
