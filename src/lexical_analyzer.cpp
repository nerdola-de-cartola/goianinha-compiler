#include "lexical_analyzer.hpp"
#include <iostream>
#include <string>
#include <tuple>
#include <sstream>
#include "FlexLexer.h"
#include "token_type.hpp"

// extern yylval (declared in lex.l)
extern std::string yylval;

LexicalAnalyzer::LexicalAnalyzer(std::ifstream& file)
    : stream(), lexer(&stream)
{
    content = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    // Cria um stream a partir da string
    stream.str(content);
}

LexicalAnalyzer::~LexicalAnalyzer() {
    // Destructor implementation
}

std::tuple<TokenType, std::string>  LexicalAnalyzer::get_next_token() {
    TokenType token_type;
    std::string token_value;

    token_type = (TokenType) this->lexer.yylex();
    token_value = std::string(yylval);


    return {token_type, token_value};
}


std::string LexicalAnalyzer::analyze() {
    while (true) {
        auto [token_type, token_value] = this->get_next_token();

        if (token_type == UNKNOWN) {
            std::cout << "Erro na lina " << this->lexer.lineno() << ". Token não identificado: " << token_value << "\n";
            continue;
        }

        std::string text = tokenTypeToString(token_type);

        std::cout << text << " na linha " << lexer.lineno() << ": " << token_value << "\n";

        if (token_type == TOK_EOF) {
            return "";
        }
    }

    return "";
}
