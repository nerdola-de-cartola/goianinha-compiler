#ifndef SYNTACTIC_ANALYZER_HPP
#define SYNTACTIC_ANALYZER_HPP

#include "node.hpp"
#include "syntactic.hpp"

class SyntacticAnalyzer {

private:
    LexicalAnalyzer *lexer;
    yy::Parser parser;

public:
    SyntacticAnalyzer(LexicalAnalyzer *lexer);
    ~SyntacticAnalyzer();
    void parse();
    void printTree();
};

#endif // SYNTACTIC_ANALYZER_HPP
