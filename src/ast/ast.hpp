#ifndef AST_H
#define AST_H

#include <memory>
#include "../token/token.hpp"

class AstNode {
public:
    virtual ~AstNode() = default;
};

class Expression : public AstNode {

};

class Stmt : public AstNode {

};

class BinaryExpression : public Expression {
public:
    Token& op;
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;

    BinaryExpression(Token& op, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right);

};

class UnaryExpression : public Expression {
public:
    Token& op;
    std::unique_ptr<Expression> right;

    UnaryExpression(Token& op, std::unique_ptr<Expression> right);
};


#endif //AST_H