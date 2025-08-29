#ifndef LEXICAL_ANALYZER_HPP
#define LEXICAL_ANALYZER_HPP

#include <string>
#include <fstream>
#include <sstream>
#include "token_type.hpp"
#include "FlexLexer.h"

class LexicalAnalyzer {
private:
    std::string content;
    std::istringstream stream;
    yyFlexLexer lexer;

public:
    LexicalAnalyzer(std::ifstream& file);
    ~LexicalAnalyzer();
    
    // Method to analyze tokens from input
    void analyze();
    std::tuple<TokenType, std::string, int>  get_next_token();
};

#endif // LEXICAL_ANALYZER_HPP
