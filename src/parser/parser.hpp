#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <vector>
#include "../token/token.hpp"
#include "../ast/ast.hpp"

class Parser {
public:
    explicit Parser(std::vector<Token>& tokens);
    std::vector<std::unique_ptr<Stmt>> parse();

private:
    std::vector<Token>& tokens;
    unsigned int currentPosition;

    std::unique_ptr<Expression> parseExpression();
    std::unique_ptr<Expression> asignment();
    std::unique_ptr<Expression> logicOR();
    std::unique_ptr<Expression> logicAND();
    std::unique_ptr<Expression> equality();
    std::unique_ptr<Expression> comparison();
    std::unique_ptr<Expression> addition();
    std::unique_ptr<Expression> multiplication();
    std::unique_ptr<Expression> call();
    std::unique_ptr<Expression> primary();

    Token& advance();
    Token& peek();
    Token& nextPeek();
    Token& consumed();

    bool isMatch(TokenType type);

    void consume(TokenType type, std::string& message);

    void error(std::string& message);
};

#endif //PARSER_H