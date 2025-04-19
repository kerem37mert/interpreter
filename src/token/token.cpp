#include "token.hpp"

Token::Token()
    : type(TokenType::TOKEN_EOF), fileName(""), start(nullptr), length(0), line(0), column(0) {}

Token::Token(TokenType type, const std::string& fileName, const char* start, unsigned int length, unsigned int line, unsigned int column)
    : type(type), fileName(fileName),start(start), length(length), line(line), column(column) {}
