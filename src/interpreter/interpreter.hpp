#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP
#include "../ast/ast.hpp"

class Interpreter : public ASTVisitor {
public:
    Interpreter();
    void interpret(Program* program);
    void runTimeError(const Token& token, const std::string& message);
};

#endif //INTERPRETER_HPP
