#ifndef LEXER_HPP
#define LEXER_HPP

#include <vector>
#include <string>
#include "../token/token.hpp"

class Lexer {
    public:
        Lexer(const std::string& source, const std::string& fileName);
        std::vector<Token> scanner();

    private:
        const std::string& source;
        const std::string& fileName;
        unsigned int startPosition;
        unsigned int currentPosition;
        unsigned int currentLine;
        unsigned int currentColumn;

        Token scanToken();
        Token makeToken(TokenType type);
        Token errorToken(const char* errMsg);
        Token stringLiteral();
        Token identifierLiteral(char start);
        Token numberLiteral();

        void skipWhiteSpace();
        
        char advance();
        char peek();
        char nextPeek();
        bool isMatch(char c);

        bool isAtEnd();
        bool isAlpha(char c);
        bool isDigit(char c);
};

#endif // LEXER_HPP