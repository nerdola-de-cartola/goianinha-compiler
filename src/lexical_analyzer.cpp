#include "lexical_analyzer.hpp"
#include <iostream>
#include <string>
#include <tuple>
#include <sstream>
#include "FlexLexer.h"
#include "token_type.hpp"
#include "syntactic.hpp"

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

std::tuple<TokenType, std::string, int>  LexicalAnalyzer::get_next_token() {
    TokenType token_type = (TokenType) this->lexer.yylex();
    std::string token_value = std::string(yylval);
    int token_line = this->lexer.lineno();

    if (token_type == UNKNOWN) {
        std::cout << "Erro na lina " << token_line << ". Token não identificado: " << token_value << "\n";
    }

    return {token_type, token_value, token_line};
}


void LexicalAnalyzer::analyze() {    
    while (true) {
        auto [token_type, token_value, token_line] = this->get_next_token();

        if (token_type == UNKNOWN) {
            continue;
        }

        std::cout << tokenTypeToString(token_type) << " na linha " << token_line << ": " << token_value << "\n";

        if (token_type == TOK_EOF) {
            return;
        }
    }

    return;
}

int yylex(std::string *lval, LexicalAnalyzer *lexer) {
    auto [token_type, token_value, token_line] = lexer->get_next_token();
    *lval = token_value;
    return token_type;
}


void yy::Parser::error(const std::string &msg) {
    std::cerr << "Syntax error: " << msg << std::endl;
}