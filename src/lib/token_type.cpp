#include "token_type.hpp"
#include <string>

std::string tokenTypeToString(TokenType t) {
    switch (t) {
        case CONST_INT:          return "CONST_INT";
        case CONST_CAR:          return "CONST_CAR";
        case CONST_STR:          return "CONST_STR";
        case TOK_EOF:            return "TOK_EOF";
        case CMD_PROGRAMA:       return "CMD_PROGRAMA";
        case TYPE_INT:           return "TYPE_INT";
        case TYPE_CAR:           return "TYPE_CAR";
        case CMD_RETORNE:        return "CMD_RETORNE";
        case CMD_LEIA:           return "CMD_LEIA";
        case CMD_ESCREVA:        return "CMD_ESCREVA";
        case NOVA_LINHA:         return "NOVA_LINHA";
        case CMD_SE:             return "CMD_SE";
        case CMD_ENTAO:          return "CMD_ENTAO";
        case CMD_SE_NAO:         return "CMD_SE_NAO";
        case CMD_ENQUANTO:       return "CMD_ENQUANTO";
        case CMD_EXECUTE:        return "CMD_EXECUTE";
        case UNKNOWN:            return "UNKNOWN";
        case ABRE_PARENTESES:    return "ABRE_PARENTESES";
        case FECHA_PARENTESES:   return "FECHA_PARENTESES";
        case ABRE_CHAVE:         return "ABRE_CHAVE";
        case FECHA_CHAVE:        return "FECHA_CHAVE";
        case VIRGULA:            return "VIRGULA";
        case PONTO_VIRGULA:      return "PONTO_VIRGULA";
        case ATRIBUICAO:         return "ATRIBUICAO";
        case NEGACAO:            return "NEGACAO";
        case OU:                 return "OU";
        case E:                  return "E";
        case IGUAL:              return "IGUAL";
        case DIFERENTE:          return "DIFERENTE";
        case MENOR_QUE:          return "MENOR_QUE";
        case MAIOR_QUE:          return "MAIOR_QUE";
        case MAIOR_IGUAL_QUE:    return "MAIOR_IGUAL_QUE";
        case MENOR_IGUAL_QUE:    return "MENOR_IGUAL_QUE";
        case SOMA:               return "SOMA";
        case SUBTRACAO:          return "SUBTRACAO";
        case MULIPLICACAO:       return "MULIPLICACAO";
        case DIVISAO:            return "DIVISAO";
        case ID:                 return "ID";
        case ABRE_COMENTARIO:    return "ABRE_COMENTARIO";
        case FECHA_COMENTARIO:   return "FECHA_COMENTARIO";
        case QUEBRA_COMENTARIO:  return "QUEBRA_COMENTARIO";
        case ASPAS_SIMPLES:      return "ASPAS_SIMPLES";
        case ASPAS_DUPLAS:       return "ASPAS_DUPLAS";
        case QUEBRA_CAR:         return "QUEBRA_CAR";
    }

    return "";
}
