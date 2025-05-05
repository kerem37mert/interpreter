#include "interpreter.hpp"
#include <iostream>

void Interpreter::interpret(Program* program) {
    try {
        program->accept(*this);
    } catch (const std::runtime_error& e) {
        std::cerr << "[Çalışma Zamanı Hatası] " << e.what() << std::endl;
    }
}