#include "ast.hpp"

BinaryExpression::BinaryExpression(Token& op, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
    : op(op), left(std::move(left)), right(std::move(right)) {}

UnaryExpression::UnaryExpression(Token& op, std::unique_ptr<Expression> right)
    : op(op), right(std::move(right)) {}

ExpressionLiteral::ExpressionLiteral(Token& token)
    :token(token) {}
