#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "lexical_analyzer.hpp"

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
    lexer->analyze(); // Test lexer
    
    // Cleanup
    file.close();
    delete lexer;
    
    std::cout << "Compilation completed successfully!" << std::endl;
    return 0;
}
