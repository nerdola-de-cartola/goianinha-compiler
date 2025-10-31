#include "lexical_analyzer.hpp"
#include <iostream>
#include <string>
#include <tuple>
#include <sstream>
#include "FlexLexer.h"
#include "token_type.hpp"
#include "syntactic.hpp"
#include "error.hpp"

// extern yylval (declared in lex.l)
extern std::string yylval;
Node *root = nullptr;

LexicalAnalyzer::LexicalAnalyzer(std::ifstream& file)
    : stream(), lexer(&stream)
{
    content = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    // Cria um stream a partir da string
    stream.str(content);
}

LexicalAnalyzer::~LexicalAnalyzer() {
    if (root != nullptr) delete root;
    root = nullptr;
}

Node * LexicalAnalyzer::get_ast() {
    return root;
}

std::tuple<TokenType, std::string, int>  LexicalAnalyzer::get_next_token() {
    TokenType token_type = (TokenType) this->lexer.yylex();
    std::string token_value = std::string(yylval);
    int token_line = this->lexer.lineno();

    return {token_type, token_value, token_line};
}

int yylex(void *lval, yy::location *location, LexicalAnalyzer *lexer) {
    auto [token_type, token_value, token_line] = lexer->get_next_token();
    location->begin.line   = token_line;
    location->begin.column = token_value.length();
    
    if(token_type == UNKNOWN) {
        show_error(lexical, *location, std::string("Token não identificado: " + token_value));
        return 1; 
    }

    if(token_type == QUEBRA_COMENTARIO) {
        show_error(lexical, *location, std::string("Comentário não termina"));
        return 1; 

    }

    if(token_type == QUEBRA_CAR) {
        show_error(lexical, *location, std::string("Cadeia de caracteres não termina"));
        return 1; 

    }
    
    if (token_type == CONST_INT || token_type == CONST_CAR || token_type == ID) {
        yy::Parser::value_type *vl = (yy::Parser::value_type *) lval;
        vl->emplace<std::string>(token_value); // Provides token_value to Bison
    }

    return token_type;
}

void yy::Parser::error(const location_type& loc, const std::string& msg) {
    show_error(syntactic, loc, msg);
}

