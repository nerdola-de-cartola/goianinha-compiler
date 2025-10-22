#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "lexical_analyzer.hpp"
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
    yy::Parser parser(lexer);
    // parser.set_debug_level(1); // enable debug printing
    auto result = parser.parse();  // chama o parser, que por sua vez usa yylex()
    std::cout << "Parser result: " << result << "\n";

    Node *root = lexer->get_ast();
    root->printTree();
    
    // Cleanup
    file.close();
    delete lexer;
    
    std::cout << "Compilation completed successfully!" << std::endl;
    return 0;
}
