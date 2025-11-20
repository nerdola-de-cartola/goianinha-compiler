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

int last_line = 1;
int last_length = 0;
int token_col = 1; 
int token_line = 1; 

std::tuple<TokenType, std::string, int, int>  LexicalAnalyzer::get_next_token() {
    TokenType token_type = (TokenType) this->lexer.yylex();
    std::string token_value = std::string(yylval);

    last_line = token_line;
    token_line = this->lexer.lineno();

    if (token_line == last_line) {
        token_col += last_length; 
        //std::cout << "lasjdh" << std::endl;
    } else {
        token_col = 1;
    }

    last_length = token_value.length();

    //std::cout << "(" << token_line << ", " << token_col << ")" << std::endl;

    if(token_type == WHITES) {
        //std::cout << "white space " << token_value.length() << std::endl;
        return get_next_token();
    }

    return {token_type, token_value, token_line, token_col};
}


int yylex(void *lval, yy::location *location, LexicalAnalyzer *lexer) {
    auto [token_type, token_value, token_line, token_col] = lexer->get_next_token();

    if (location != nullptr) {
        location->begin.line = token_line;
        location->begin.column = token_col;
        location->end = location->begin;
    }

    //std::cout << location->begin.column << std::endl;
    
    if(token_type == UNKNOWN) {
        show_error(lexical, *location, std::string("Token não identificado: " + token_value), nullptr);
        return 1; 
    }

    if(token_type == QUEBRA_COMENTARIO) {
        show_error(lexical, *location, std::string("Comentário não termina"), nullptr);
        return 1; 

    }

    if(token_type == QUEBRA_CAR) {
        show_error(lexical, *location, std::string("Cadeia de caracteres não termina"), nullptr);
        return 1; 

    }

    yy::Parser::value_type *vl = (yy::Parser::value_type *) lval;
    
    if (token_type == CONST_INT || token_type == CONST_CAR || token_type == CONST_STR || token_type == ID) {
        vl->emplace<std::string>(token_value); // Provides token_value to Bison
    } else {
        vl->emplace<yy::location>(*location);
    }

    return token_type;
}

void yy::Parser::error(const location_type& loc, const std::string& msg) {
    show_error(syntactic, loc, msg, nullptr);
}

