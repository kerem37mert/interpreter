#include "astPrinter.hpp"

#include <iostream>

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
    std::cout << this->getIndent() << name;

    if (!details.empty())
        std::cout << " (" << details << ")";

    std::cout << std::endl;
}

// Visitor Metotaları Yazılacak