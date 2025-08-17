#ifndef SEMANTIC_ANALYZER_HPP
#define SEMANTIC_ANALYZER_HPP

#include "lexical_analyzer.hpp"
#include <string>
#include <fstream>

class SemanticAnalyzer {
private:
    LexicalAnalyzer &lexer;

public:
    SemanticAnalyzer(LexicalAnalyzer &lexer);
    ~SemanticAnalyzer();
    
    void compile();
};

#endif // SEMANTIC_ANALYZER_HPP
