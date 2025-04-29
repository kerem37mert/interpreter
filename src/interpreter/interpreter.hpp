#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP
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
    void visitArrayAssignExpression(ArrayAssignExpression* expr) override;

    //void runTimeError(const Token& token, const std::string& message);
};

#endif //INTERPRETER_HPP
