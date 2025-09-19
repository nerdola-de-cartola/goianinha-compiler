%option c++
%option noyywrap

%{
#include <string>
#include "token_type.hpp"

// To store the semantic value
std::string yylval;
int yylineno = 1;
%}

DIGIT           [0-9]
LETTER          [a-zA-Z]
ID              ({LETTER}|"_"{LETTER})({LETTER}|"_"|{DIGIT})*
WHITESPACE      [\t\r' ']

%x comentario
%x car_const

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
<comentario>"\n"                    { BEGIN(INITIAL); yylineno++; yylval = YYText(); return QUEBRA_COMENTARIO; }   // comnetário quebrado
<comentario>.                       { /* IGONORE COMENTS */ }


\"                                  { BEGIN(car_const); yylval = YYText(); return DELIMITA_CAR; }
<car_const>\"                       { BEGIN(INITIAL); yylval = YYText(); return DELIMITA_CAR; }
<car_const>"\n"                     { BEGIN(INITIAL); yylineno++; yylval = YYText(); return QUEBRA_CAR; }   // const car quebrado
<car_const>[^'"\n]+                 { yylval = YYText(); return CONST_CAR; }

{ID}                                { yylval = YYText(); return ID; }

{DIGIT}+                            { yylval = YYText(); return CONST_INT; }
{WHITESPACE}                        { /* ignore white spaces */ }
"\n"                                { yylineno++; }   // conta linhas
<<EOF>>                             { yylval = YYText(); return TOK_EOF; }
.                                   { yylval = YYText(); return UNKNOWN; }
%%
