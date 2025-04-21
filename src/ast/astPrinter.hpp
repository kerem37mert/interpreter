#ifndef ASTPRINTER_HPP
#define ASTPRINTER_HPP

#include "ast.hpp"
#include <string>

class ASTPrinter : public ASTVisitor {
public:
    void print(AstNode* node);

private:
    int indentLevel = 0;
    const std::string indent = "   ";
    const std::string marker = "-> ";

    std::string getIndent() const;

    void printNode(const std::string& name, const std::string& details = "");

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
};

#endif //ASTPRINTER_HPP
