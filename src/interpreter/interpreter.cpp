#include "interpreter.hpp"
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
