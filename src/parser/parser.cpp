#include "parser.hpp"

#include <iostream>

Parser::Parser(std::vector<Token>& tokens)
    :tokens(tokens), currentPosition(0) {}

std::vector<std::unique_ptr<Stmt>> Parser::parse() {
    std::vector<std::unique_ptr<Stmt>> head;

    while(this->isMatch(TokenType::TOKEN_EOF))
       this->advance();

    return head;
}


std::unique_ptr<Expression> Parser::parseExpression() {

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

Token& Parser::consumed(){
    return this->tokens[this->currentPosition - 1];
}

bool Parser::isMatch(TokenType type) {
    if(this->tokens[this->currentPosition].type == type) {
        this->currentPosition++;
        return true;
    }

    return false;
}

void Parser::consume(TokenType type, std::string& message) {
    if (this->tokens[currentPosition].type == type)
        this->advance();
    else
        this->error(message);
}

void Parser::error(std::string& message) {
        std::cerr << message;
}