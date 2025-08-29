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
    
    void add_function();
    void get_function();

    void add_variable();
    void get_variable();
};

#endif // SCOPE_HPP
