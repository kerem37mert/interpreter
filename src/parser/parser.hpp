#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "../token/token.hpp"

class Parser {
    public:
        explicit Parser(std::vector<Token>& tokens);

    private:
        std::vector<Token>& tokens;
        unsigned int currentPosition;

        Token& advance();
        Token& peek();
        Token& nextPeek();

        bool isMatch(TokenType type);
};



#endif //PARSER_H