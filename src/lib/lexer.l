%option c++
%option noyywrap
%option yylineno

%{
#include <string>
#include "token_type.hpp"

std::string yylval; // To store the semantic value
%}

DIGIT           [0-9]
NUMBER          ("+"|"-"|""){DIGIT}+
LETTER          [a-zA-Z]
ID              ({LETTER}|"_"{LETTER})({LETTER}|"_"|{DIGIT})*
WHITESPACE      [\t\r" "]

%x comentario
%x car_const
%x str_const

%%
"programa"                          { yylval = YYText(); return CMD_PROGRAMA; }
"int"                               { yylval = YYText(); return TYPE_INT; }    
"car"                               { yylval = YYText(); return TYPE_CAR; }    
"retorne"                           { yylval = YYText(); return CMD_RETORNE; }
"leia"                              { yylval = YYText(); return CMD_LEIA; }
"escreva"                           { yylval = YYText(); return CMD_ESCREVA; }
"novalinha"                         { yylval = YYText(); return NOVA_LINHA; }
"se"                                { yylval = YYText(); return CMD_SE; }
"entao"                             { yylval = YYText(); return CMD_ENTAO; }
"senao"                             { yylval = YYText(); return CMD_SE_NAO; }
"enquanto"                          { yylval = YYText(); return CMD_ENQUANTO; }
"execute"                           { yylval = YYText(); return CMD_EXECUTE; }

"("                                 { yylval = YYText(); return ABRE_PARENTESES; }
")"                                 { yylval = YYText(); return FECHA_PARENTESES; }
"{"                                 { yylval = YYText(); return ABRE_CHAVE; }
"}"                                 { yylval = YYText(); return FECHA_CHAVE; }
","                                 { yylval = YYText(); return VIRGULA; }
";"                                 { yylval = YYText(); return PONTO_VIRGULA; }

"="                                 { yylval = YYText(); return ATRIBUICAO; }

"!"                                 { yylval = YYText(); return NEGACAO; }
"ou"                                { yylval = YYText(); return OU; }
"e"                                 { yylval = YYText(); return E; }

"=="                                { yylval = YYText(); return IGUAL; }
"!="                                { yylval = YYText(); return DIFERENTE; }
"<"                                 { yylval = YYText(); return MENOR_QUE; }
">"                                 { yylval = YYText(); return MAIOR_QUE; }
">="                                { yylval = YYText(); return MAIOR_IGUAL_QUE; }
"<="                                { yylval = YYText(); return MENOR_IGUAL_QUE; }

"+"                                 { yylval = YYText(); return SOMA; }
"-"                                 { yylval = YYText(); return SUBTRACAO; }
"*"                                 { yylval = YYText(); return MULIPLICACAO; }
"/"                                 { yylval = YYText(); return DIVISAO; }

"/*"                                { BEGIN(comentario); yylval = YYText(); }
<comentario>"*/"                    { BEGIN(INITIAL); yylval = YYText(); }
<comentario>"\n"                    { BEGIN(INITIAL); yylval = YYText(); return QUEBRA_COMENTARIO; }   // comnetário quebrado
<comentario>.                       { /* IGONORE COMENTS */ }


\'                                  { BEGIN(car_const); yylval = YYText(); return ASPAS_SIMPLES; }
<car_const>\'                       { BEGIN(INITIAL); yylval = YYText(); return ASPAS_SIMPLES; }
<car_const>"\n"                     { BEGIN(INITIAL); ; yylval = YYText(); return QUEBRA_CAR; }   // const car quebrado
<car_const>[^''\n]                  { yylval = YYText(); return CONST_CAR; }

\"                                  { BEGIN(str_const); yylval = YYText(); return ASPAS_DUPLAS; }
<str_const>\"                       { BEGIN(INITIAL); yylval = YYText(); return ASPAS_DUPLAS; }
<str_const>"\n"                     { BEGIN(INITIAL); yylval = YYText(); return QUEBRA_CAR; }   // const car quebrado
<str_const>[^'"\n]+                 { yylval = YYText(); return CONST_STR; }

{ID}                                { yylval = YYText(); return ID; }

{NUMBER}                            { yylval = YYText(); return CONST_INT; }
{WHITESPACE}                        { yylval = YYText(); return WHITES; }
"\n"                                { }   // conta linhas
<<EOF>>                             { yylval = YYText(); return TOK_EOF; }
.                                   { yylval = YYText(); return UNKNOWN; }
%%
