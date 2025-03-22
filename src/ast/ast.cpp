#include "ast.hpp"

//Expression Implementasyonları
BinaryExpression::BinaryExpression(const Token& op, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
    : op(op), left(std::move(left)), right(std::move(right)) {}

void BinaryExpression::accept(ASTVisitor& visitor) {
    visitor.visitBinaryExpression(this);
}

LogicalExpression::LogicalExpression(const Token& op, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
    : op(op), left(std::move(left)), right(std::move(right)) {}

void LogicalExpression::accept(ASTVisitor& visitor) {
    visitor.visitLogicalExpression(this);
}

UnaryExpression::UnaryExpression(const Token& op, std::unique_ptr<Expression> operand)
    : op(op), operand(std::move(operand)) {}

void UnaryExpression::accept(ASTVisitor& visitor) {
    visitor.visitUnaryExpression(this);
}

ComparisonExpression::ComparisonExpression(const Token& op, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
    : op(op), left(std::move(left)), right(std::move(right)) {}

void ComparisonExpression::accept(ASTVisitor& visitor) {
    visitor.visitComparisonExpression(this);
}

LiteralExpression::LiteralExpression(const Token& token)
    : token(token) {}

void LiteralExpression::accept(ASTVisitor& visitor) {
    visitor.visitLiteralExpression(this);
}

VariableExpression::VariableExpression(const Token& name)
    : name(name) {}

void VariableExpression::accept(ASTVisitor& visitor) {
    visitor.visitVariableExpression(this);
}

AssignExpression::AssignExpression(const Token& name, std::unique_ptr<Expression> value)
    : name(name), value(std::move(value)) {}

void AssignExpression::accept(ASTVisitor& visitor) {
    visitor.visitAssignExpression(this);
}

CallExpression::CallExpression(std::unique_ptr<Expression> callee, const Token& paren, std::vector<std::unique_ptr<Expression>> arguments)
    : callee(std::move(callee)), arguments(std::move(arguments)), line(paren.line), column(paren.column) {}

void CallExpression::accept(ASTVisitor& visitor) {
    visitor.visitCallExpression(this);
}

ArrayExpression::ArrayExpression(std::vector<std::unique_ptr<Expression>> elements, const Token& bracket)
    : elements(std::move(elements)), line(bracket.line), column(bracket.column) {}

void ArrayExpression::accept(ASTVisitor& visitor) {
    visitor.visitArrayExpression(this);
}

ArrayAccessExpression::ArrayAccessExpression(std::unique_ptr<Expression> array,
    std::unique_ptr<Expression> index,
    const Token& bracket)
    : array(std::move(array)), index(std::move(index)), line(bracket.line), column(bracket.column) {}

void ArrayAccessExpression::accept(ASTVisitor& visitor) {
    visitor.visitArrayAccessExpression(this);
}

// Statements implementasyonları
ExpressionStmt::ExpressionStmt(std::unique_ptr<Expression> expression)
    : expression(std::move(expression)) {}

void ExpressionStmt::accept(ASTVisitor& visitor) {
    visitor.visitExpressionStmt(this);
}

PrintStmt::PrintStmt(std::unique_ptr<Expression> expression)
    : expression(std::move(expression)) {}

void PrintStmt::accept(ASTVisitor& visitor) {
    visitor.visitPrintStmt(this);
}

VarDeclStmt::VarDeclStmt(const Token& name, const Token& type, bool isArray, bool isConst,
    std::unique_ptr<Expression> initializer)
    : name(name), type(type), isArray(isArray), isConst(isConst), initializer(std::move(initializer)) {}

void VarDeclStmt::accept(ASTVisitor& visitor) {
    visitor.visitVarDeclStmt(this);
}

BlockStmt::BlockStmt(std::vector<std::unique_ptr<Stmt>> statements)
    : statements(std::move(statements)) {}

void BlockStmt::accept(ASTVisitor& visitor) {
    visitor.visitBlockStmt(this);
}

IfStmt::IfStmt(std::unique_ptr<Expression> condition,
    std::unique_ptr<Stmt> thenBranch,
    std::unique_ptr<Stmt> elseBranch)
    : condition(std::move(condition)), thenBranch(std::move(thenBranch)), elseBranch(std::move(elseBranch)) {}

void IfStmt::accept(ASTVisitor& visitor) {
    visitor.visitIfStmt(this);
}

LoopStmt::LoopStmt(std::unique_ptr<Expression> condition, std::unique_ptr<Stmt> body)
    : condition(std::move(condition)), body(std::move(body)) {}

void LoopStmt::accept(ASTVisitor& visitor) {
    visitor.visitLoopStmt(this);
}

BreakStmt::BreakStmt(const Token& keyword)
    : keyword(keyword) {}

void BreakStmt::accept(ASTVisitor& visitor) {
    visitor.visitBreakStmt(this);
}

ContinueStmt::ContinueStmt(const Token& keyword)
    : keyword(keyword) {}

void ContinueStmt::accept(ASTVisitor& visitor) {
    visitor.visitContinueStmt(this);
}

FunctionDeclStmt::FunctionDeclStmt(const Token& name, const Token& returnType,
    std::vector<Token> paramNames,
    std::vector<Token> paramTypes,
    std::unique_ptr<BlockStmt> body)
    : name(name), returnType(returnType), paramNames(std::move(paramNames)),
      paramTypes(std::move(paramTypes)), body(std::move(body)) {}

void FunctionDeclStmt::accept(ASTVisitor& visitor) {
    visitor.visitFunctionDeclStmt(this);
}

ReturnStmt::ReturnStmt(const Token& keyword, std::unique_ptr<Expression> value)
    : keyword(keyword), value(std::move(value)) {}

void ReturnStmt::accept(ASTVisitor& visitor) {
    visitor.visitReturnStmt(this);
}

Program::Program(std::vector<std::unique_ptr<Stmt>> statements)
    : statements(std::move(statements)) {}

void Program::accept(ASTVisitor& visitor) {
    visitor.visitProgram(this);
}