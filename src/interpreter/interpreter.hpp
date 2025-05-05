#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP
#include "value.hpp"
#include "../ast/ast.hpp"

class Interpreter : public ASTVisitor {
public:
    Interpreter();
    void interpret(Program* program);

    void visitBinaryExpression(BinaryExpression* expr) override;
    void visitLogicalExpression(LogicalExpression* expr) override;
    void visitUnaryExpression(UnaryExpression* expr) override;
    void visitComparisonExpression(ComparisonExpression* expr) override;
    void visitLiteralExpression(LiteralExpression* expr) override;
    void visitVariableExpression(VariableExpression* expr) override;
    void visitAssignExpression(AssignExpression* expr) override;
    void visitCallExpression(CallExpression* expr) override;
    void visitArrayExpression(ArrayExpression* expr) override;
    void visitArrayAccessExpression(ArrayAccessExpression* expr) override;
    //void visitArrayAssignExpression(ArrayAssignExpression* expr) override;

    void visitExpressionStmt(ExpressionStmt* stmt) override;
    void visitPrintStmt(PrintStmt* stmt) override;
    void visitVarDeclStmt(VarDeclStmt* stmt) override;
    void visitBlockStmt(BlockStmt* stmt) override;
    void visitIfStmt(IfStmt* stmt) override;
    void visitLoopStmt(LoopStmt* stmt) override;
    void visitBreakStmt(BreakStmt* stmt) override;
    void visitContinueStmt(ContinueStmt* stmt) override;
    void visitFunctionDeclStmt(FunctionDeclStmt* stmt) override;
    void visitReturnStmt(ReturnStmt* stmt) override;
    void visitProgram(Program* program) override;

    //void runTimeError(const Token& token, const std::string& message);

private:
    Value result;
};

#endif //INTERPRETER_HPP
