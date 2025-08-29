#ifndef VARIABLE_HPP
#define VARIABLE_HPP

#include <vector>
#include <string>

enum VariableTypes {
    INT,
    CAR,
};

class Variable {
private:
    std::string name;
    VariableTypes type;

public:
    Variable(std::string name, VariableTypes type);
    ~Variable();
    std::string get_name();
    VariableTypes get_type();
    std::string toString();
};

#endif // VARIABLE_HPP
