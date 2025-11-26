#ifndef VARIABLE_HPP
#define VARIABLE_HPP

#include <vector>
#include <string>

enum VariableTypes {
    TNULL = 0,
    INT = 152,
    CAR,
    STR
};

class Variable {
private:
    std::string name;
    VariableTypes type;
    
public:
    int pos;

    Variable(std::string name, VariableTypes type);
    Variable(std::string name, VariableTypes type, int pos);
    ~Variable();
    std::string get_name();
    VariableTypes get_type();
    std::string toString();

static std::string typeToString(VariableTypes type);
};


#endif // VARIABLE_HPP
