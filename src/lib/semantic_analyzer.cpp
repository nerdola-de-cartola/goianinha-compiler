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
    // lexer.analyze();

    while (true) {
        auto [token_type, token_value, token_line] = lexer.get_next_token();

        if (token_type == UNKNOWN) {
            continue;
        }

        if (token_type == TOK_EOF) {
            return;
        }
    }
}
