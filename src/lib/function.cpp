#include "function.hpp"
#include <algorithm>

Function::Function(std::string function_name, VariableTypes return_type)
    : name(function_name), return_type(return_type) {}

Function::~Function() {}

std::string Function::get_name() {
    return this->name;
}

void Function::add_parameter(Variable var) {
    this->parameters.push_back(var);
}

VariableTypes Function::get_return_type() {
    return this->return_type;
}

Variable* Function::get_parameter(std::string name) {
    auto it = std::find_if(parameters.begin(), parameters.end(),
    [name](Variable var) {
        return var.get_name() == name;
    });

    if (it != parameters.end()) {
        return &(*it);  // pointer to element inside vector
    }

    return nullptr;      // not found
}

std::string Function::toString() {
    std::string t = return_type == INT ? "int" : "car";
    std::string params = "";

    int count = 0;
    for (Variable& p : parameters) {
        params += p.toString();

        bool isLast = (++count == (int) parameters.size());
        if (!isLast) {
            params += ", ";
        }
    }
    
    return t + " " + name + "(" + params + ");";
}
