#ifndef SCOPE_STACK_HPP
#define SCOPE_STACK_HPP

#include <vector>
#include "scope.hpp"

class ScopeStack {
private:
    std::vector<Scope> scopes;

public:
    ScopeStack();
    ~ScopeStack();

    void push();
    void pop();
    
    void add_function(Function f);
    Function* get_function(std::string name);

    void add_variable(Variable var);
    Variable* get_variable(std::string name);

    std::string toString();
};

#endif // SCOPE_STACK_HPP
