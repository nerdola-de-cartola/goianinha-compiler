#ifndef LEXICAL_ANALYZER_HPP
#define LEXICAL_ANALYZER_HPP

#include <string>
#include <fstream>
#include <sstream>
#include "token_type.hpp"
#include "FlexLexer.h"
#include "node.hpp"

class LexicalAnalyzer {
private:
    std::string content;
    std::istringstream stream;
    yyFlexLexer lexer;

public:
    LexicalAnalyzer(std::ifstream& file);
    ~LexicalAnalyzer();
    
    std::tuple<TokenType, std::string, int, int>  get_next_token();
    Node * get_ast();
};

#endif // LEXICAL_ANALYZER_HPP
