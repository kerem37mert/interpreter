#ifndef AST_H
#define AST_H

#include <memory>
#include <vector>

#include "../token/token.hpp"

class ASTVisitor;

class AstNode {
public:
    virtual ~AstNode() = default;
    virtual void accept(ASTVisitor& visitor) = 0;
};

class Expression : public AstNode {};
class Stmt : public AstNode {};

// AST KÖK DÜĞÜM

class BinaryExpression : public Expression {
public:
    Token op;
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;

    BinaryExpression(const Token& op, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right);
    void accept(ASTVisitor& visitor) override;
};

class LogicalExpression : public Expression {
public:
    Token op;
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;

    LogicalExpression(const Token& op, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right);
    void accept(ASTVisitor& visitor) override;
};

class UnaryExpression : public Expression {
public:
    Token op;
    std::unique_ptr<Expression> operand;

    UnaryExpression(const Token& op, std::unique_ptr<Expression> operand);
    void accept(ASTVisitor& visitor) override;
};


class ComparisonExpression : public Expression {
public:
    Token op;
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;

    ComparisonExpression(const Token& op, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right);
    void accept(ASTVisitor& visitor) override;
};

class LiteralExpression : public Expression {
public:
    Token token;
    const std::string value;

    explicit LiteralExpression(const Token& token);
    void accept(ASTVisitor& visitor) override;
};

class VariableExpression : public Expression {
public:
    Token name;

    explicit VariableExpression(const Token& name);
    void accept(ASTVisitor& visitor) override;
};

class AssignExpression : public Expression {
    Token name;
    std::unique_ptr<Expression> value;

    AssignExpression(const Token& name, std::unique_ptr<Expression> value);
    void accept(ASTVisitor& visitor) override;
};

class CallExpression : public Expression {
public:
    std::unique_ptr<Expression> callee;
    int line;
    int column;
    std::vector<std::unique_ptr<Expression>> arguments;

    CallExpression(std::unique_ptr<Expression> callee, const Token& paren,
                   std::vector<std::unique_ptr<Expression>> arguments);
    void accept(ASTVisitor& visitor) override;
};

class ArrayExpression : public Expression {
public:
    std::vector<std::unique_ptr<Expression>> elements;
    int line;
    int column;

    ArrayExpression(std::vector<std::unique_ptr<Expression>> elements, const Token& bracket);
    void accept(ASTVisitor& visitor) override;
};

class ArrayAccessExpression : public Expression {
public:
    std::unique_ptr<Expression> array;
    std::unique_ptr<Expression> index;
    int line;
    int column;

    ArrayAccessExpression(std::unique_ptr<Expression> array,
                         std::unique_ptr<Expression> index,
                         const Token& bracket);
    void accept(ASTVisitor& visitor) override;
};

class ExpressionStmt : public Stmt
{
    
};
#endif //AST_H