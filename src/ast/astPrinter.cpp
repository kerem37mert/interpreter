#include "astPrinter.hpp"

void ASTPrinter::print(AstNode* node) {
    node->accept(*this);
}

std::string ASTPrinter::getIndent() const {
    std::string result;

    for(int i=0; i<indentLevel; i++)
        result += indent;;

    return result;
}

void ASTPrinter::printNode(const std::string& name, const std::string& details) {
    // yazılacak
}