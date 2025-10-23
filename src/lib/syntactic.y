%language "c++"
%debug
%locations
%define parse.error detailed
%define api.value.type variant
%define api.parser.class {Parser}
%define api.location.type {yy::location}

%code requires {
    #include <string>
    #include <iostream>
    #include <variant>
    #include "lexical_analyzer.hpp"
    #include "token_type.hpp"
    #include "syntactic.hpp"
    #include "node.hpp"
    #include "variable.hpp"
    #include "location.hh"

    int yylex(void *lval, yy::location *location, LexicalAnalyzer *lexer);
    extern Node * root;
}

//%union{
//    Node *TNode;
//    char *TLexeme;
//}

%parse-param { LexicalAnalyzer* lexer }
%lex-param { LexicalAnalyzer* lexer }

%token <Node *> NOVA_LINHA TYPE_INT TYPE_CAR
%token CMD_PROGRAMA  CMD_RETORNE CMD_LEIA CMD_ESCREVA CMD_SE CMD_ENTAO CMD_SE_NAO CMD_ENQUANTO CMD_EXECUTE ABRE_PARENTESES FECHA_PARENTESES ABRE_CHAVE FECHA_CHAVE VIRGULA PONTO_VIRGULA ATRIBUICAO NEGACAO OU E IGUAL DIFERENTE MENOR_QUE MAIOR_QUE MAIOR_IGUAL_QUE MENOR_IGUAL_QUE SOMA SUBTRACAO MULIPLICACAO DIVISAO ABRE_COMENTARIO FECHA_COMENTARIO QUEBRA_COMENTARIO DELIMITA_CAR QUEBRA_CAR UNKNOWN
%token <std::string> CONST_INT CONST_CAR ID

%type <Node *> Programa DeclFuncVar DeclProg DeclVar DeclFunc ListaParametros ListaParametrosCont Bloco ListaDeclVar ListaComando Comando Expr OrExpr AndExpr EqExpr DesignExpr AddExpr MulExpr UnExpr CarExpr PrimExpr ListExpr
%type <VariableTypes> Tipo

%start Programa

%%

Programa: {root = nullptr;}
    | DeclFuncVar DeclProg {root = new Node(prog, $1, $2);}

Epsilon:

DeclFuncVar:
      Tipo ID DeclVar PONTO_VIRGULA DeclFuncVar {
        $$ = new Node(decl_func_var, 
            new Node(list_var,
                new Node(var, $2),
                $3,
                $1
            ),
            $5
        );
    }
    | Tipo ID DeclFunc DeclFuncVar {
        $$ =  new Node(decl_func_var, 
            new Node(func1,
                $3,
                $1,
                $2
            ),
            $4
        );
    }
    | Epsilon {$$ = nullptr;}

DeclProg:
    CMD_PROGRAMA Bloco {$$ = $2;}

DeclVar:
      VIRGULA ID DeclVar {$$ = new Node(var, $3, $2);}
    | Epsilon {$$ = nullptr;}

DeclFunc:
    ABRE_PARENTESES ListaParametros FECHA_PARENTESES Bloco {$$ = new Node(func2, $2, $4);}

ListaParametros:
      Epsilon {$$ = nullptr;}
    | ListaParametrosCont {$$ = $1;}

ListaParametrosCont:
    Tipo ID {$$ = new Node(list_params, new Node(param, $1, $2));}
    | Tipo ID VIRGULA ListaParametrosCont {$$ = new Node(list_params, new Node(param, $1, $2), $4);}

Bloco:
    ABRE_CHAVE ListaDeclVar ListaComando FECHA_CHAVE {$$ = new Node(block, $2, $3);}
    | ABRE_CHAVE ListaDeclVar FECHA_CHAVE {$$ = new Node(block, $2);}

ListaDeclVar:
    Epsilon {$$ = nullptr;}
    | Tipo ID DeclVar PONTO_VIRGULA ListaDeclVar {
        $$ = new Node(list_decl_var,
            new Node(list_var,
                new Node(var, $2),
                new Node(list_var, $3)
            ),
            $5,
            $1
        );
    }

Tipo:
     TYPE_INT {$$ = INT;}
    | TYPE_CAR {$$ = CAR;}

ListaComando:
    Comando {$$ = $1;}
    | Comando ListaComando {$$ = new Node(command, $1, $2);}

Comando:
    PONTO_VIRGULA {$$ = nullptr;}
    | Expr {$$ = $1;}
    | CMD_RETORNE Expr PONTO_VIRGULA {$$ = new Node(return_cmd, $2);}
    | CMD_LEIA ID PONTO_VIRGULA {$$ = new Node(read, $2);}
    | CMD_ESCREVA Expr PONTO_VIRGULA {$$ = new Node(write, $2);}
    | NOVA_LINHA PONTO_VIRGULA {$$ = new Node(new_line);}
    | CMD_SE ABRE_PARENTESES Expr FECHA_PARENTESES CMD_ENTAO Comando {$$ = new Node(if_cond, $3, $6);}
    | CMD_SE ABRE_PARENTESES Expr FECHA_PARENTESES CMD_ENTAO Comando CMD_SE_NAO Comando {$$ = new Node(if_cond, $3, new Node(if_blocks, $6, $8));}
    | CMD_ENQUANTO ABRE_PARENTESES Expr FECHA_PARENTESES CMD_EXECUTE Comando {$$ = new Node(loop, $3, $6);}
    | Bloco {$$ = $1;}

Expr:
    OrExpr {$$ = $1;}
    | ID ATRIBUICAO Expr {$$ = new Node(assign_op, $3, $1);}

OrExpr:
    OrExpr OU AndExpr {$$ = new Node(or_op, $1, $3);}
    | AndExpr {$$ = $1;}

AndExpr:
    AndExpr E EqExpr {$$ = new Node(and_op, $1, $3);}
    | EqExpr {$$ = $1;}

EqExpr:
    EqExpr IGUAL DesignExpr {$$ = new Node(eq_op, $1, $3);}
    | EqExpr DIFERENTE DesignExpr {$$ = new Node(dif_op, $1, $3);}
    | DesignExpr {$$ = $1;}

DesignExpr:
    DesignExpr MENOR_QUE AddExpr {$$ = new Node(less_op, $1, $3);}
    | DesignExpr MAIOR_QUE AddExpr {$$ = new Node(greater_op, $1, $3);}
    | DesignExpr MAIOR_IGUAL_QUE AddExpr {$$ = new Node(ge_op, $1, $3);}
    | DesignExpr MENOR_IGUAL_QUE AddExpr {$$ = new Node(le_op, $1, $3);}
    | AddExpr {$$ = $1;}

AddExpr:
    AddExpr SOMA MulExpr {$$ = new Node(add_op, $1, $3);}
    | AddExpr SUBTRACAO MulExpr {$$ = new Node(sub_op, $1, $3);}
    | MulExpr {$$ = $1;}

MulExpr:
    MulExpr MULIPLICACAO UnExpr {$$ = new Node(mul_op, $1, $3);}
    | MulExpr DIVISAO UnExpr {$$ = new Node(div_op, $1, $3);}
    | UnExpr {$$ = $1;}

UnExpr:
    SUBTRACAO PrimExpr {$$ = new Node(negative_op, $2);}
    | NEGACAO PrimExpr {$$ = new Node(not_op, $2);}
    | PrimExpr {$$ = $1;}

CarExpr:
    DELIMITA_CAR CONST_CAR DELIMITA_CAR {$$ = new Node(character, $2);}

PrimExpr:
    ID ABRE_PARENTESES ListExpr FECHA_PARENTESES {$$ = new Node(exp, $3, $1);} //TODO VERIFY 
    | ID ABRE_PARENTESES FECHA_PARENTESES {$$ = new Node(exp, $1);}
    | ID {$$ = new Node(exp, $1);}
    | CarExpr {$$ = $1;}
    | CONST_INT {$$ = new Node(number, std::stoi($1));}
    | ABRE_PARENTESES Expr FECHA_PARENTESES {$$ = $2;}

ListExpr:
    Expr {$$ = $1;}
    | ListExpr VIRGULA Expr {$$ = new Node(list_exp, $1, $3);}