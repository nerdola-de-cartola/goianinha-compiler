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
    std::string variable_name;
    VariableTypes variable_type;

public:
    Variable();
    ~Variable();
};

#endif // VARIABLE_HPP
