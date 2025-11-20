#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "lexical_analyzer.hpp"

#define red "\033[31m"
#define default "\033[0m"

std::string posToString(int token_line, int token_col) {
    return "(" + std::to_string(token_line) + ", " + std::to_string(token_col) + ")";
}

bool isError(TokenType token_type, std::string token_value, int token_line, int token_col) {
    if (token_type == UNKNOWN) {
        std::cout << red << "Erro na posição " << posToString(token_line, token_col) << ". Token não identificado: " << token_value << default << "\n";
        return true;
    }

    if (token_type == QUEBRA_COMENTARIO) {
        std::cout << red << "Erro na posição " << posToString(token_line, token_col) << ". Comentário não termina" << default << "\n";
        return true;

    }

    if (token_type == QUEBRA_CAR) {
        std::cout << red << "Erro na posição " << posToString(token_line, token_col) << ". Cadeia de caracteres não termina" << default << "\n";
        return true;
    }

    return false;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <input_file>" << std::endl;
        std::cout << "A simple compiler framework" << std::endl;
        return 1;
    }
    
    std::string input_file = argv[1];
    std::cout << "Processing file: " << input_file << std::endl;
    
    // Read input file
    std::ifstream file(input_file);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << input_file << std::endl;
        return 1;
    }

    LexicalAnalyzer* lexer = new LexicalAnalyzer(file);

    while (true) {
        auto [token_type, token_value, token_line, token_col] = lexer->get_next_token();

        if (isError(token_type, token_value, token_line, token_col)) {
            continue;
        }

        std::cout << tokenTypeToString(token_type) << " na posição " << posToString(token_line, token_col) <<  ": " << token_value << "\n";

        if (token_type == TOK_EOF) {
            break;
        }
    }
    
    // Cleanup
    file.close();
    delete lexer;
    
    return 0;
}
