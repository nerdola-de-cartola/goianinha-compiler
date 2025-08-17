#ifndef LEXICAL_ANALYZER_HPP
#define LEXICAL_ANALYZER_HPP

#include <string>
#include <fstream>

class LexicalAnalyzer {
private:
    std::string content;

public:
    LexicalAnalyzer(std::ifstream& file);
    ~LexicalAnalyzer();
    
    // Method to analyze tokens from input
    std::string analyze();
};

#endif // LEXICAL_ANALYZER_HPP
