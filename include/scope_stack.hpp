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
    
    void add_function();
    void get_function();

    void add_variable();
    void get_variable();
};

#endif // SCOPE_STACK_HPP
