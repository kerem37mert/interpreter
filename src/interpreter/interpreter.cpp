#include "interpreter.hpp"

#include <cmath>
#include <iostream>

void Interpreter::Environment::define(const std::string& name, const Value& value) {
    this->variables[name] = value;
}

void Interpreter::Environment::defineFunction(const std::string& name, FunctionDeclStmt* function) {
    this->functions[name] = function;
}

Value Interpreter::Environment::get(const std::string& name) {
    auto it = this->variables.find(name);

    if(it == this->variables.end())
        return it->second;

    if(this->enclosing != nullptr)
        return this->enclosing->get(name);

    throw std::runtime_error("Tanımsız değişken: " + name);
}

FunctionDeclStmt* Interpreter::Environment::getFunction(const std::string& name) {
    auto it = this->functions.find(name);

    if(it != this->functions.end())
        return it->second;

    throw std::runtime_error("Tanımsız fonksiyon: " + name);
}

void Interpreter::Environment::assign(const std::string& name, const Value& value) {
    auto it = this->variables.find(name);

    if (it != variables.end()) {
        variables[name] = value;
        return;
    }

    if (enclosing != nullptr) {
        enclosing->assign(name, value);
        return;
    }

    throw std::runtime_error("Tanımsız değişken: " + name);
}

Interpreter::Interpreter() {
    this->currentEnvironment = new Environment();
    environmentStack.push(currentEnvironment);
}

void Interpreter::interpret(AstNode* node) {
    try {
        node->accept(*this);
    } catch (const std::exception& e) {
        std::cerr << "[HATA] " << e.what() << std::endl;
    }
}

void Interpreter::pushEnvironment() {
    this->currentEnvironment = new Environment(currentEnvironment);
    this->environmentStack.push(this->currentEnvironment);
}


void Interpreter::popEnvironment() {
    Environment* old = this->environmentStack.top();
    this->environmentStack.pop();
    this->currentEnvironment = this->environmentStack.top();
    delete old;
}

void Interpreter::enterBlock() {
    this->pushEnvironment();
}

void Interpreter::exitBlock() {
    this->popEnvironment();
}

Value Interpreter::evaluate(Expression* expr) {
    expr->accept(*this);
    return result;
}

void Interpreter::execute(Stmt* stmt) {
    stmt->accept(*this);
}

bool Interpreter::isTruthy(const Value& value) {
    if (value.isNil())
        return false;

    if (value.isBool())
        return value.asBool();

    return true;
}

bool Interpreter::isEqual(const Value& a, const Value& b) {
    if (a.isNil() && b.isNil())
        return true;

    if (a.isNil())
        return false;

    if (a.isBool() && b.isBool())
        return a.asBool() == b.asBool();

    if (a.isNumber() && b.isNumber())
        return a.asNumber() == b.asNumber();

    if (a.isString() && b.isString())
        return a.asString() == b.asString();

    return false;
}

void Interpreter::checkNumberOperand(const Token& op, const Value& operand) {
    if (operand.isNumber())
        return;

    throw std::runtime_error("Operand sayı olmalıdır.");
}

void Interpreter::checkNumberOperands(const Token& op, const Value& left, const Value& right) {
    if (left.isNumber() && right.isNumber())
        return;

    throw std::runtime_error("Operandlar sayı olmalıdır.");
}

void Interpreter::visitBinaryExpression(BinaryExpression* expr) {
    Value left = this->evaluate(expr->left.get());
    Value right = this->evaluate(expr->right.get());

    std::string op(expr->op.start, expr->op.length);

    if (op == "+") {
        if(left.isNumber() && right.isNumber())
            this->result = Value(left.asNumber() + right.asNumber());
        else if (left.isString() && right.isString())
            this->result = Value(left.asString() + right.asString());
        else
            throw std::runtime_error("Operandlar sayı veya metin olmalıdır.");
    } else if(op == "-") {
        this->checkNumberOperands(expr->op, left, right);
        this->result = Value(left.asNumber() - right.asNumber());
    } else if(op == "*") {
        this->checkNumberOperands(expr->op, left, right);
        this->result = Value(left.asNumber() * right.asNumber());
    } else if(op == "/") {
        this->checkNumberOperands(expr->op, left, right);
        if(right.asNumber() == 0)
            throw std::runtime_error("Sıfıra bölme hatası.");

        this->result = Value(left.asNumber() / right.asNumber());
    } else if(op == "%") {
        this->checkNumberOperands(expr->op, left, right);
        if (right.asNumber() == 0)
            throw std::runtime_error("Sıfıra bölme hatası.");

        this->result = Value(fmod(left.asNumber(), right.asNumber()));
    }
}

void Interpreter::visitUnaryExpression(UnaryExpression* expr) {
    Value right = this->evaluate(expr->operand.get());

    std::string op(expr->op.start, expr->op.length);

    if (op == "-") {
        this->checkNumberOperand(expr->op, right);
        this->result = Value(-right.asNumber());
    } else if (op == "!") {
        this->result = Value(!isTruthy(right));
    }
}