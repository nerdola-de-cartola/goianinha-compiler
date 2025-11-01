    #ifndef FUNCTION_HPP
    #define FUNCTION_HPP

    #include <vector>
    #include <string>
    #include "variable.hpp"
    #include "error.hpp"

    class Function {
    private:
        std::string name;
        std::vector<Variable> parameters;
        VariableTypes return_type;

    public:
        Function(std::string function_name, VariableTypes return_type);
        ~Function();

        std::string get_name();
        VariableTypes get_return_type();
        Variable* get_parameter(std::string name);

        // Iterator access
        auto begin() { return parameters.begin(); }
        auto end()   { return parameters.end(); }

        // Const versions for read-only iteration
        auto begin() const { return parameters.begin(); }
        auto end()   const { return parameters.end(); }

        std::vector<Variable> &getParameters() { return parameters; };

        Result add_parameter(Variable var);

        std::string toString();
    };

    #endif // FUNCTION_HPP
