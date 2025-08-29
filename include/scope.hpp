#ifndef SCOPE_HPP
#define SCOPE_HPP

#include <vector>

class Scope {
private:
    std::vector<int> variables;
    std::vector<int> functions;

public:
    Scope();
    ~Scope();
    
    void add_function();
    void get_function();
    
    void add_variable();
    void get_variable();
};

#endif // SCOPE_HPP
