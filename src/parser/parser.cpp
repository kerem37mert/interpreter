#include "parser.hpp"

Parser::Parser(std::vector<Token>& tokens)
    :tokens(tokens), currentPosition(0) {}

void Parser::parse() {
    // return type değiştirilecek fonkun içi doldurulacak
}

Token& Parser::advance() {
    return this->tokens[this->currentPosition++];
}

Token& Parser::peek() {
    return this->tokens[this->currentPosition];
}

Token& Parser::nextPeek() {
    return this->tokens[this->currentPosition + 1];
}

bool Parser::isMatch(TokenType type) {
    if(this->tokens[this->currentPosition].type == type) {
        this->currentPosition++;
        return true;
    }

    return false;
}