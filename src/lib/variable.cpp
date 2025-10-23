#include "variable.hpp"

Variable::Variable(std::string name, VariableTypes type)
    : name(name), type(type) {}

Variable::~Variable() {}

std::string Variable::get_name() {
    return this->name;
}

VariableTypes Variable::get_type() {
    return this->type;
}

std::string Variable::toString() {
    return typeToString(type) + " " + name;
}

std::string Variable::typeToString(VariableTypes type) {
    return type == INT ? "int" : "car";
}