#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <memory>
#include <unordered_map>
#include <vector>
#include <stack>
#include "../ast/ast.hpp"
#include "value.hpp"

class Interpreter : public ASTVisitor {
public:
    Interpreter();
    void interpret(AstNode* node);

private:
    // Çalışma zamanı ortamı
    struct Environment {
        std::unordered_map<std::string, Value> variables;
        std::unordered_map<std::string, FunctionDeclStmt*> functions;
        std::unordered_map<std::string, bool> constants; // sbt/const değişkenler için
        Environment* enclosing;

        Environment() : enclosing(nullptr) {}
        Environment(Environment* enclosing) : enclosing(enclosing) {}

        void define(const std::string& name, const Value& value, bool isConst = false);
        void defineFunction(const std::string& name, FunctionDeclStmt* function);
        Value get(const std::string& name);
        FunctionDeclStmt* getFunction(const std::string& name);
        void assign(const std::string& name, const Value& value);
    };

    Environment* currentEnvironment;
    std::stack<Environment*> environmentStack;
    Value result;

    void pushEnvironment();
    void popEnvironment();
    void enterBlock();
    void exitBlock();
    Value evaluate(Expression* expr);
    void execute(Stmt* stmt);
    bool isTruthy(const Value& value);
    bool isEqual(const Value& a, const Value& b);
    void checkNumberOperand(const Token& op, const Value& operand);
    void checkNumberOperands(const Token& op, const Value& left, const Value& right);
    void checkTypeCompatibility(const std::string& type, const Value& value);


    // AST Visitor implementasyonları
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
};

#endif // INTERPRETER_HPP