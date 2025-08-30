#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "lexical_analyzer.hpp"
#include "semantic_analyzer.hpp"
#include "scope_stack.hpp"
#include "syntactic.hpp"

int main(int argc, char* argv[]) {
    std::cout << "Compiler v1.0.0" << std::endl;
    
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <input_file>" << std::endl;
        std::cout << "A simple compiler framework" << std::endl;
        return 1;
    }
    
    std::string input_file = argv[1];
    std::cout << "Processing file: " << input_file << std::endl;
    
    // Read input file
    std::ifstream file(input_file);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << input_file << std::endl;
        return 1;
    }

    LexicalAnalyzer* lexer = new LexicalAnalyzer(file);
    SemanticAnalyzer* semantic_analyzer = new SemanticAnalyzer(*lexer);
    ScopeStack* scope_stack = new ScopeStack();

    // Test Test syntactic analyzer
    yy::Parser parser(lexer);
    //parser.set_debug_level(1); // enable debug printing
    auto result = parser.parse();  // chama o parser, que por sua vez usa yylex()
    std::cout << "Parser result: " << result << "\n";
    exit(0);

    lexer->analyze(); // Test lexer
    semantic_analyzer->compile(); // TODO: Test semantic analyzer

    // Test scope stack
    Function f1 = Function("funcao_basica", CAR);
    Function f2 = Function("funcao_exemplo", CAR);
    f2.add_parameter(Variable("x", INT));
    Function f3 = Function("funcao_com_varios_parametros", INT);
    f3.add_parameter(Variable("x", INT));
    f3.add_parameter(Variable("y", CAR));
    f3.add_parameter(Variable("z", INT));
    f3.add_parameter(Variable("w", CAR));

    scope_stack->push();
    scope_stack->add_variable(Variable("a", INT));
    scope_stack->add_variable(Variable("_b12", CAR));
    scope_stack->add_function(f1);
    scope_stack->add_function(f2);
    scope_stack->add_function(f3);

    scope_stack->push();
    scope_stack->add_variable(Variable("a", INT));
    scope_stack->add_variable(Variable("_b12", CAR));
    scope_stack->add_function(f1);
    scope_stack->add_function(f2);
    scope_stack->add_function(f3);

    std::cout << "\n" << scope_stack->toString() << "\n";
    
    // Cleanup
    file.close();
    delete lexer;
    delete semantic_analyzer;
    
    std::cout << "Compilation completed successfully!" << std::endl;
    return 0;
}
