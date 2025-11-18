#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "lexical_analyzer.hpp"
#include "syntactic_analyzer.hpp"
#include "semantic_analyzer.hpp"
#include "mips_generator.hpp"

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

    LexicalAnalyzer *lexer = new LexicalAnalyzer(file);
    SyntacticAnalyzer syn = SyntacticAnalyzer(lexer);
    syn.parse();
    SemanticAnalyzer sem = SemanticAnalyzer(*lexer, syn);
    sem.analyze();
    auto ast = lexer->get_ast();
    ast->printTree();

    auto code = generate_code(ast);

    print_code(code);
    run_code(code);

    // Cleanup
    file.close();
    delete lexer;
    
    return 0;
}
