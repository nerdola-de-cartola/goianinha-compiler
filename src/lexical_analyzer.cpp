#include "lexical_analyzer.hpp"
#include <iostream>
#include <string>

LexicalAnalyzer::LexicalAnalyzer(std::ifstream& file) {
    content = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

LexicalAnalyzer::~LexicalAnalyzer() {
    // Destructor implementation
}

std::string LexicalAnalyzer::analyze() {
    return content;
}
