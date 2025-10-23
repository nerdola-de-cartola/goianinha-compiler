#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "lexical_analyzer.hpp"
#include "syntactic_analyzer.hpp"
#include "semantic_analyzer.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <input_file>" << std::endl;
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

    LexicalAnalyzer lexer = LexicalAnalyzer(file);
    SemanticAnalyzer sem = SemanticAnalyzer(lexer);
    sem.analyze();

    // Cleanup
    file.close();
    
    return 0;
}
