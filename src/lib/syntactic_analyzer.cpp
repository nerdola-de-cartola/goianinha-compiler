#include "syntactic_analyzer.hpp"
#include "syntactic.hpp"

SyntacticAnalyzer::SyntacticAnalyzer(LexicalAnalyzer *lexer)
    : lexer(lexer), parser(yy::Parser(lexer)) {}

SyntacticAnalyzer::~SyntacticAnalyzer() {}

void SyntacticAnalyzer::parse() {
    root = lexer->get_ast();
    if (root != nullptr) delete root;

    parser.parse();
}

void SyntacticAnalyzer::printTree() {
    lexer->get_ast()->printTree();
}