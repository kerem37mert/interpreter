#include "parser.hpp"
#include <iostream>

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

void Parser::synchronize() {
    this->advance();

    while(!this->isAtEnd()) {
        if(this->previous().type == TokenType::SEMI_COLON)
            return;

        switch(this->peek().type) {
            case TokenType::VAR:
            case TokenType::CONST:
            case TokenType::FUNCTION:
            case TokenType::IF:
            case TokenType::LOOP:
            case TokenType::RETURN:
            case TokenType::PRINT:
                return;
            default:
                break;

            this->advance();
        }
    }
}

void Parser::error(const Token& token, const std::string& message) {
    this->hasError = true;

    std::cerr << "\n\033[1;31m========== SÖZDİZİMİ HATASI ==========\033[0m" << std::endl;

    if (token.type == TokenType::TOKEN_EOF) {
        std::cerr << "\033[1;31mDosya sonu: " << message << "\033[0m" << std::endl;
    } else {
        std::string tokenValue(token.start, token.length);
        std::cerr << "\033[1;31mSözdizimi hatası: " << message << "\033[0m" << std::endl;
        std::cerr << "Token: '" << tokenValue << "'" << std::endl;
        std::cerr << "Dosya: " << token.fileName << std::endl;
        std::cerr << "Satır: " << token.line << ", Sütun: " << token.column << std::endl;
    }

    std::cerr << "\033[1;31m=====================================\033[0m" << std::endl;

    throw std::exception();
}