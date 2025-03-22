#include "parser.hpp"

Parser::Parser(const std::vector<Token> tokens)
    : tokens(tokens), currentToken(0), hasError(false) {}
