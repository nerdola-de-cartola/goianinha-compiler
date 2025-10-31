#include "scope_stack.hpp"
#include "scope.hpp"
#include <iostream>
ScopeStack::ScopeStack() {}
ScopeStack::~ScopeStack() {}

void ScopeStack::push() {
    this->scopes.push_back(Scope());
}

void ScopeStack::pop() {
    this->scopes.pop_back();
}

void ScopeStack::add_function(Function f) {
    if (this->scopes.empty()) {
        std::cout << "ERROR NO SCOPE TO ADD FUNCTION\n";
        exit(1);
    }

    this->scopes.back().add_function(f);
}

Function* ScopeStack::get_function(std::string name) {
    Function *f = nullptr;

    if (this->scopes.empty()) return f;

    // Reverse iteration
    for (long int i = this->scopes.size()-1; i >= 0; i--) {
        // std::cout<<i<<"\n";
        f = this->scopes[i].get_function(name);
        if (f != nullptr) return f;
    }

    return f;
}

void ScopeStack::add_variable(Variable var) {
    if (this->scopes.empty()) {
        std::cout << "ERROR NO SCOPE TO ADD VAR\n";
        exit(1);
    }
    this->scopes.back().add_variable(var);
}

Variable* ScopeStack::get_variable(std::string name) {
    Variable *var = nullptr;

    if (this->scopes.empty()) return var;

    // Reverse iteration
    for (long int i = this->scopes.size()-1; i >= 0; i--) {
        // std::cout<<i<<"\n";
        var = this->scopes[i].get_variable(name);
        if (var != nullptr) return var;
    }

    return var;
}

std::string ScopeStack::toString() {
    std::string sep = "----------------------\n";
    std::string str = "";

    for (Scope& s : scopes) {
        str += sep + s.toString();
    }

    return "All scopes from bottom to top\n" + str + sep;
}
