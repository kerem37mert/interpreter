#include "parser.hpp"

Parser::Parser(const std::vector<Token>& tokens)
    : tokens(tokens), currentToken(0), hasError(false) {}


Token Parser::advance(){
    if(!this->isAtEnd())
        this->currentToken++;

    return this->previous();
}

bool Parser::check(TokenType type) const {
    if(this->isAtEnd())
        return false;

    return this->peek().type == type;
}

bool Parser::match(TokenType type) {
    if(this->check(type)) {
        this->advance();
        return true;
    }

    return false;
}

bool Parser::match(const std::initializer_list<TokenType>& types) {
    for(auto type : types) {
        if(this->check(type)) {
            this->advance();
            return true;
        }
    }

    return false;
}

Token Parser::consume(TokenType type, const std::string& message) {
    if(this->check(type))
        return this->advance();

    this->error(this->peek(), message);
}

Token Parser::peek() const {
    return this->tokens[this->currentToken];
}

Token Parser::previous() const {
    return this->tokens[this->currentToken - 1];
}

bool Parser::isAtEnd() const {
    return this->peek().type == TokenType::TOKEN_EOF;
}
