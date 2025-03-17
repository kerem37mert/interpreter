#include "parser.hpp"

#include <iostream>

Parser::Parser(std::vector<Token>& tokens)
    :tokens(tokens), currentPosition(0) {}

std::vector<std::unique_ptr<AstNode>> Parser::parse() {
    std::vector<std::unique_ptr<AstNode>> head;

    while(this->isMatch(TokenType::TOKEN_EOF))
       this->advance();

    return head;
}


// Shunting Yard Algoritması Kullanılacak
// Expresision yoksa nullptr return et
std::unique_ptr<AstNode> Parser::parseExpression() {

    // Yardımcı Vektörler
    std::vector<Token> stack;
    std::vector<Token> queue;

    while(this->peek().type != TokenType::TOKEN_EOF) {
        switch(this->peek().type) {
            case TokenType::IDENTIFIER:
                // yazılacak
                break;
            case TokenType::NUMBER_LITERAL:
            case TokenType::STRING_LITERAL:
            case TokenType::TRUE:
            case TokenType::FALSE:
                // yazılacak
                break;
            default:
                return nullptr;
        }

        return nullptr;
    }
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

void Parser::consume(TokenType type, std::string& message) {
    if (this->tokens[currentPosition].type == type)
        this->advance();
    else
        this->error(message);
}

void Parser::error(std::string& message) {
        std::cerr << message;
}