#ifndef SEMANTIC_ANALYZER_HPP
#define SEMANTIC_ANALYZER_HPP

#include "lexical_analyzer.hpp"
#include "syntactic_analyzer.hpp"
#include <string>
#include <fstream>

class SemanticAnalyzer {
private:
    LexicalAnalyzer &lexer;
    SyntacticAnalyzer &syn;

public:
    SemanticAnalyzer(LexicalAnalyzer &lexer, SyntacticAnalyzer &syn);
    ~SemanticAnalyzer();
    
    void analyze();
};

#endif // SEMANTIC_ANALYZER_HPP
