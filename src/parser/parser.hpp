#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <vector>
#include "../token/token.hpp"
#include "../ast/ast.hpp"

class Parser {
public:
    explicit Parser(std::vector<Token>& tokens);
    std::vector<std::unique_ptr<AstNode>> parse();

private:
    std::vector<Token>& tokens;
    unsigned int currentPosition;

    std::unique_ptr<AstNode> parseExpression();

    Token& advance();
    Token& peek();
    Token& nextPeek();

    bool isMatch(TokenType type);

    void consume(TokenType type, std::string& message);

    void error(std::string& message);
};

#endif //PARSER_H