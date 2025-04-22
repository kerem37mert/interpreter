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

// Visitor Metotaları
void ASTPrinter::visitBinaryExpression(BinaryExpression* expr) {
    std::string op(expr->op.start, expr->op.length);
    this->printNode("İkili ifade", "İşlem" + op);

    this->indentLevel++;
    std::cout << this->getIndent() << "Sol:" << std::endl;
    this->indentLevel++;
    expr->left->accept(*this);
    this->indentLevel--;

    std::cout << this->getIndent() << "Sağ:" << std::endl;
    this->indentLevel++;
    expr->right->accept(*this);
    this->indentLevel--;
    this->indentLevel--;
}

void ASTPrinter::visitLogicalExpression(LogicalExpression* expr) {
    std::string op(expr->op.start(), expr->op.length);
    this->printNode("Mantıksal İfade", "İşlem" + op);

    this->indentLevel++;
    std::cout << this->getIndent() << "Sol:" << std::endl;
    this->indentLevel++;
    expr->left->accept(*this);
    this->indentLevel--;

    std::cout <<  this->getIndent() << "Sağ:" << std::endl;
    this->indentLevel++;
    expr->right->accept(*this);
    this->indentLevel--;
    this->indentLevel--;
}

void ASTPrinter::visitUnaryExpression(UnaryExpression* expr) {
    std::string op(expr->op.start, expr->op.length);
    this->printNode("Tekli İfade", "İşlem" + op);

    this->indentLevel++;
    expr->operand->accept(*this);
    this->indentLevel--;
}

void ASTPrinter::visitComparisonExpression(ComparisonExpression* expr) {
    std::string op(expr->op.start, expr->op.length);
    this->printNode("Karşılaştırma İfadesi", "İşlem" + op);

    this->indentLevel++;
    std::cout << this->getIndent() << "Sol:" << std::endl;
    this->indentLevel++;
    expr->left->accept(*this);
    this->indentLevel--;

    std::cout << this->getIndent() << "Sağ:" << std::endl;
    this->indentLevel++;
    expr->right->accept(*this);
    this->indentLevel--;
    this->indentLevel--;
}

void ASTPrinter::visitLiteralExpression(LiteralExpression* expr) {
    std::string value = expr->value;
    std::string type;

    switch(expr->token.type) {
        case TokenType::NUMBER_LITERAL:
            type = "sayı";
            break;
        case TokenType::STRING_LITERAL:
            type = "metin";
            break;
        case TokenType::TRUE:
        case TokenType::FALSE:
            type = "doğruluk";
            break;
        default:
            type = "bilinmeyen";
    }

    this->printNode("Sabit Değer", "Değer" + value + ", Tip: " + type);
}

void ASTPrinter::visitVariableExpression(VariableExpression* expr) {
    std::string name(expr->name.start, expr->name.length);
    this->printNode("Değişken", "İsim: " + name);
}

void ASTPrinter::visitAssignExpression(AssignExpression* expr) {
    std::string name(expr->name.start, expr->name.length);
    this->printNode("Atama İfadesi", "Hedef: " + name);

    this->indentLevel++;
    std::cout << this->getIndent() << "Değer:" << std::endl;
    this->indentLevel++;
    expr->value->accept(*this);
    this->indentLevel--;
    this->indentLevel--;
}
