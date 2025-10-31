#include "scope.hpp"
#include "error.hpp"
#include <algorithm>
#include <iostream>

Scope::Scope() {}
Scope::~Scope() {}
Result Scope::add_function(Function f) {
    auto search_func = get_function(f.get_name());

    if(search_func != nullptr) return ERROR; //ERROR

    this->functions.push_back(f);
    return OK;
}

Function* Scope::get_function(std::string name) {
    auto it = std::find_if(functions.begin(), functions.end(),
        [name](Function f) {
            return f.get_name() == name;
        });

    if (it != functions.end()) {
        return &(*it);  // pointer to element inside vector
    }

    return nullptr;      // not found
}

Result Scope::add_variable(Variable var) {
    auto search_var = get_variable(var.get_name());

    if (search_var != nullptr) return ERROR; //ERROR
    
    this->variables.push_back(var);
    return OK;
}

Variable* Scope::get_variable(std::string name) {
    auto it = std::find_if(variables.begin(), variables.end(),
        [name](Variable var) {
            return var.get_name() == name;
        });

    if (it != variables.end()) {
        return &(*it);  // pointer to element inside vector
    }

    return nullptr;      // not found
}

std::string Scope::toString() {
    std::string fs = "";
    std::string vs = "";

    for (Function& f : functions) {
        fs += f.toString() + "\n";
    }

    for (Variable& v : variables) {
        vs += v.toString() + ";\n";
    }
    

    return "FUNCTIONS:\n" + fs + "\n" + "VARIABLES:\n" + vs;
}
