#include "token.hpp"

Token::Token(TokenType type, const std::string& fileName, const char* start, unsigned int length, unsigned int line, unsigned int column)
    : type(type), fileName(fileName),start(start), length(length), line(line), column(column) {}
