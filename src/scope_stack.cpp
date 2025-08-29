#include "scope_stack.hpp"
#include "scope.hpp"

ScopeStack::ScopeStack() {}
ScopeStack::~ScopeStack() {}

void ScopeStack::push() {
    this->scopes.push_back(Scope());
}

void ScopeStack::pop() {
    this->scopes.pop_back();
}

void ScopeStack::add_function(Function f) {
    this->scopes.back().add_function(f);
}

Function* ScopeStack::get_function(std::string name) {
    return this->scopes.back().get_function(name);
}

void ScopeStack::add_variable(Variable var) {
    this->scopes.back().add_variable(var);
}

Variable* ScopeStack::get_variable(std::string name) {
    return this->scopes.back().get_variable(name);
}

std::string ScopeStack::toString() {
    std::string sep = "----------------------\n";
    std::string str = "";

    for (Scope& s : scopes) {
        str += sep + s.toString();
    }

    return str + sep;
}
