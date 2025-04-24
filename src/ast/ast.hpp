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
public:
    Token name;
    std::unique_ptr<Expression> value;

    AssignExpression(const Token& name, std::unique_ptr<Expression> value);
    void accept(ASTVisitor& visitor) override;
};

class CallExpression : public Expression {
public:
    std::unique_ptr<Expression> callee;
    unsigned int line;
    unsigned int column;
    std::vector<std::unique_ptr<Expression>> arguments;

    CallExpression(std::unique_ptr<Expression> callee, const Token& paren,
        std::vector<std::unique_ptr<Expression>> arguments);
    void accept(ASTVisitor& visitor) override;
};

class ArrayExpression : public Expression {
public:
    std::vector<std::unique_ptr<Expression>> elements;
    unsigned int line;
    unsigned int column;

    ArrayExpression(std::vector<std::unique_ptr<Expression>> elements, const Token& bracket);
    void accept(ASTVisitor& visitor) override;
};

class ArrayAccessExpression : public Expression {
public:
    std::unique_ptr<Expression> array;
    std::unique_ptr<Expression> index;
    unsigned int line;
    unsigned int column;

    ArrayAccessExpression(std::unique_ptr<Expression> array, std::unique_ptr<Expression> index,
        const Token& bracket);
    void accept(ASTVisitor& visitor) override;
};

class ArrayAsignExpression : public Expression {
public:
    std::unique_ptr<Expression> array;
    std::unique_ptr<Expression> index;
    std::unique_ptr<Expression> value;

    ArrayAsignExpression(std::unique_ptr<Expression> array, std::unique_ptr<Expression> index,
        std::unique_ptr<Expression> value);
    void accept(ASTVisitor& visitor) override;
};

class ExpressionStmt : public Stmt {
public:
    std::unique_ptr<Expression> expression;

    explicit ExpressionStmt(std::unique_ptr<Expression> expression);
    void accept(ASTVisitor& visitor) override;
};

class PrintStmt : public Stmt {
public:
    std::unique_ptr<Expression> expression;

    explicit PrintStmt(std::unique_ptr<Expression> expression);
    void accept(ASTVisitor& visitor) override;
};

class VarDeclStmt : public Stmt {
public:
    Token name;
    Token type;
    bool isArray;
    bool isConst;

    std::unique_ptr<Expression> initializer;

    VarDeclStmt(const Token& name, const Token& type, bool isArray, bool isConst,
        std::unique_ptr<Expression> initializer);
    void accept(ASTVisitor& visitor) override;
};

class BlockStmt : public Stmt {
public:
    std::vector<std::unique_ptr<Stmt>> statements;

    explicit BlockStmt(std::vector<std::unique_ptr<Stmt>> statements);
    void accept(ASTVisitor& visitor) override;
};

class IfStmt : public Stmt {
public:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Stmt> thenBranch;
    std::unique_ptr<Stmt> elseBranch;

    IfStmt(std::unique_ptr<Expression> condition,
        std::unique_ptr<Stmt> thenBranch,
        std::unique_ptr<Stmt> elseBlock);
    void accept(ASTVisitor& visitor) override;
};

class LoopStmt : public Stmt {
public:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Stmt> body;

    LoopStmt(std::unique_ptr<Expression> condition, std::unique_ptr<Stmt> body);
    void accept(ASTVisitor& visitor) override;
};

class BreakStmt : public Stmt {
public:
    Token keyword;

    explicit BreakStmt(const Token& keyword);
    void accept(ASTVisitor& visitor) override;
};

class ContinueStmt : public Stmt {
public:
    Token keyword;

    explicit ContinueStmt(const Token& keyword);
    void accept(ASTVisitor& visitor) override;
};

class FunctionDeclStmt : public Stmt {
public:
    Token name;
    Token returnType;
    std::vector<Token> paramNames;
    std::vector<Token> paramTypes;
    std::unique_ptr<BlockStmt> body;

    FunctionDeclStmt(const Token& name, const Token& returnType,
        std::vector<Token> paramNames,
        std::vector<Token> paramTypes,
        std::unique_ptr<BlockStmt> body);
    void accept(ASTVisitor& visitor) override;
};

class ReturnStmt : public Stmt {
public:
    Token keyword;
    std::unique_ptr<Expression> value;

    ReturnStmt(const Token& keyword, std::unique_ptr<Expression> value);
    void accept(ASTVisitor& visitor) override;
};

class Program : public AstNode {
public:
    std::vector<std::unique_ptr<Stmt>> statements;

    explicit Program(std::vector<std::unique_ptr<Stmt>> statements);
    void accept(ASTVisitor& visitor) override;
};

class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;

    virtual void visitBinaryExpression(BinaryExpression* expr) = 0;
    virtual void visitLogicalExpression(LogicalExpression* expr) = 0;
    virtual void visitUnaryExpression(UnaryExpression* expr) = 0;
    virtual void visitComparisonExpression(ComparisonExpression* expr) = 0;
    virtual void visitLiteralExpression(LiteralExpression* expr) = 0;
    virtual void visitVariableExpression(VariableExpression* expr) = 0;
    virtual void visitAssignExpression(AssignExpression* expr) = 0;
    virtual void visitCallExpression(CallExpression* expr) = 0;
    virtual void visitArrayExpression(ArrayExpression* expr) = 0;
    virtual void visitArrayAccessExpression(ArrayAccessExpression* expr) = 0;

    virtual void visitExpressionStmt(ExpressionStmt* stmt) = 0;
    virtual void visitPrintStmt(PrintStmt* stmt) = 0;
    virtual void visitVarDeclStmt(VarDeclStmt* stmt) = 0;
    virtual void visitBlockStmt(BlockStmt* stmt) = 0;
    virtual void visitIfStmt(IfStmt* stmt) = 0;
    virtual void visitLoopStmt(LoopStmt* stmt) = 0;
    virtual void visitBreakStmt(BreakStmt* stmt) = 0;
    virtual void visitContinueStmt(ContinueStmt* stmt) = 0;
    virtual void visitFunctionDeclStmt(FunctionDeclStmt* stmt) = 0;
    virtual void visitReturnStmt(ReturnStmt* stmt) = 0;
    virtual void visitProgram(Program* program) = 0;
};
#endif //AST_H