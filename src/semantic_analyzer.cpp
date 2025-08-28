#include "semantic_analyzer.hpp"
#include <iostream>

SemanticAnalyzer::SemanticAnalyzer(LexicalAnalyzer &lexer) : lexer(lexer) {
    // Constructor implementation
}

SemanticAnalyzer::~SemanticAnalyzer() {
    // Destructor implementation
}

void SemanticAnalyzer::compile() {
    std::cout << "Analyzing input\n";
    lexer.analyze();
}
