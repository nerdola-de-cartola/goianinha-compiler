#ifndef TOKEN_TYPE_HPP
#define TOKEN_TYPE_HPP
#include <string>

enum TokenType {
    TOK_EOF = 0, // Especial value that bison expects as EOF
    CONST_INT = 258, // For bison tokens nees to start at 258
    CONST_CAR,
    CMD_PROGRAMA,
    TYPE_INT,
    TYPE_CAR,
    CMD_RETORNE,
    CMD_LEIA,
    CMD_ESCREVA,
    NOVA_LINHA,
    CMD_SE,
    CMD_ENTAO,
    CMD_SE_NAO,
    CMD_ENQUANTO,
    CMD_EXECUTE,
    ABRE_PARENTESES,
    FECHA_PARENTESES,
    ABRE_CHAVE,
    FECHA_CHAVE,
    VIRGULA,
    PONTO_VIRGULA,
    ATRIBUICAO,
    NEGACAO,
    OU,
    E,
    IGUAL,
    DIFERENTE,
    MENOR_QUE,
    MAIOR_QUE,
    MAIOR_IGUAL_QUE,
    MENOR_IGUAL_QUE,
    SOMA,
    SUBTRACAO,
    MULIPLICACAO,
    DIVISAO,
    ID,
    ABRE_COMENTARIO,
    FECHA_COMENTARIO,
    QUEBRA_COMENTARIO,
    DELIMITA_CAR,
    QUEBRA_CAR,
    UNKNOWN,
};

std::string tokenTypeToString(TokenType t);

#endif