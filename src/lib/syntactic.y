%language "c++"
%debug
%locations
%define parse.error detailed
%define api.value.type {std::string}
%define api.parser.class {Parser}
%define api.location.type {yy::location}

%code requires {

#include <string>
#include <iostream>
#include "lexical_analyzer.hpp"
#include "token_type.hpp"
#include "syntactic.hpp"
#include "location.hh"

int yylex(std::string *lval, yy::location *location, LexicalAnalyzer *lexer);

}

%parse-param { LexicalAnalyzer* lexer }
%lex-param { LexicalAnalyzer* lexer }

%token CONST_INT CONST_CAR CMD_PROGRAMA TYPE_INT TYPE_CAR CMD_RETORNE CMD_LEIA CMD_ESCREVA NOVA_LINHA CMD_SE CMD_ENTAO CMD_SE_NAO CMD_ENQUANTO CMD_EXECUTE ABRE_PARENTESES FECHA_PARENTESES ABRE_CHAVE FECHA_CHAVE VIRGULA PONTO_VIRGULA ATRIBUICAO NEGACAO OU E IGUAL DIFERENTE MENOR_QUE MAIOR_QUE MAIOR_IGUAL_QUE MENOR_IGUAL_QUE SOMA SUBTRACAO MULIPLICACAO DIVISAO ID ABRE_COMENTARIO FECHA_COMENTARIO QUEBRA_COMENTARIO DELIMITA_CAR QUEBRA_CAR UNKNOWN

%start Programa

%%

Programa:
    | DeclFuncVar DeclProg

Epsilon:

DeclFuncVar:
      Tipo ID DeclVar PONTO_VIRGULA DeclFuncVar
    | Tipo ID DeclFunc DeclFuncVar
    | Epsilon

DeclProg:
    CMD_PROGRAMA Bloco

DeclProg:
      VIRGULA ID DeclVar

DeclVar:
      VIRGULA ID DeclVar
    | Epsilon

DeclFunc:
    ABRE_PARENTESES ListaParametros FECHA_PARENTESES Bloco

ListaParametros:
      Epsilon
    | ListaParametrosCont

ListaParametrosCont:
    Tipo ID
    | Tipo ID VIRGULA ListaParametrosCont

Bloco:
    ABRE_CHAVE ListaDeclVar ListaComando FECHA_CHAVE
    | ABRE_CHAVE ListaDeclVar FECHA_CHAVE

ListaDeclVar:
    Epsilon
    | Tipo ID DeclVar PONTO_VIRGULA ListaDeclVar

Tipo:
     TYPE_INT
    | TYPE_CAR

ListaComando:
    Comando
    | Comando ListaComando

Comando:
    PONTO_VIRGULA
    | Expr
    | CMD_RETORNE Expr PONTO_VIRGULA
    | CMD_LEIA LValueExpr PONTO_VIRGULA
    | CMD_ESCREVA Expr PONTO_VIRGULA
    | NOVA_LINHA PONTO_VIRGULA
    | CMD_SE ABRE_PARENTESES Expr FECHA_PARENTESES CMD_ENTAO Comando
    | CMD_SE ABRE_PARENTESES Expr FECHA_PARENTESES CMD_ENTAO Comando CMD_SE_NAO Comando
    | CMD_ENQUANTO ABRE_PARENTESES Expr FECHA_PARENTESES CMD_EXECUTE Comando
    | Bloco

Expr:
    OrExpr
    | ID ATRIBUICAO Expr

OrExpr:
    OrExpr OU AndExpr
    | AndExpr

AndExpr:
    AndExpr E EqExpr
    | EqExpr

EqExpr:
    EqExpr IGUAL DesignExpr
    | EqExpr DIFERENTE DesignExpr
    | DesignExpr

DesignExpr:
    DesignExpr MENOR_QUE AddExpr
    | DesignExpr MAIOR_QUE AddExpr
    | DesignExpr MAIOR_IGUAL_QUE AddExpr
    | DesignExpr MENOR_IGUAL_QUE AddExpr
    | AddExpr

AddExpr:
    AddExpr SOMA MulExpr
    | AddExpr SUBTRACAO MulExpr
    | MulExpr

MulExpr:
    MulExpr MULIPLICACAO UnExpr
    | MulExpr DIVISAO UnExpr
    | UnExpr

UnExpr:
    SUBTRACAO PrimExpr
    | NEGACAO PrimExpr
    | PrimExpr

LValueExpr:
    ID

CarExpr:
    DELIMITA_CAR CONST_CAR DELIMITA_CAR

PrimExpr:
    ID ABRE_PARENTESES ListExpr FECHA_PARENTESES
    | ID ABRE_PARENTESES FECHA_PARENTESES
    | ID
    | CarExpr
    | CONST_INT
    | ABRE_PARENTESES Expr FECHA_PARENTESES {  }

ListExpr:
    Expr
    | ListExpr VIRGULA Expr