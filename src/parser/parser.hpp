#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <vector>
#include "../token/token.hpp"
#include "../ast/ast.hpp"

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);

    std::unique_ptr<Program> parse();

private:
    std::vector<Token> tokens;
    unsigned int currentToken;
    bool hasError;

    std::unique_ptr<Stmt> statement();
    std::unique_ptr<Stmt> printStatement();
    std::unique_ptr<Stmt> expressionStatement();
    std::unique_ptr<Stmt> blockStatement();
    std::unique_ptr<Stmt> ifStatement();
    std::unique_ptr<Stmt> loopStatement();
    std::unique_ptr<Stmt> breakStatement();
    std::unique_ptr<Stmt> continueStatement();
    std::unique_ptr<Stmt> returnStatement();
    std::unique_ptr<Stmt> varDeclaration();
    std::unique_ptr<Stmt> functionDeclaration();

    std::unique_ptr<Expression> expression();
    std::unique_ptr<Expression> assignment();
    std::unique_ptr<Expression> logicalOr();
    std::unique_ptr<Expression> logicalAnd();
    std::unique_ptr<Expression> equality();
    std::unique_ptr<Expression> comparison();
    std::unique_ptr<Expression> term();
    std::unique_ptr<Expression> factor();
    std::unique_ptr<Expression> power();
    std::unique_ptr<Expression> unary();
    std::unique_ptr<Expression> call();
    std::unique_ptr<Expression> primary();
    std::unique_ptr<Expression> arrayExpression();
    std::unique_ptr<Expression> finishCall(std::unique_ptr<Expression> callee);
    std::unique_ptr<Expression> arrayAccess(std::unique_ptr<Expression> array);

    // Yardımcı metotlar
    Token advance();
    bool check(TokenType type) const;
    bool match(TokenType type);
    bool match(const std::initializer_list<TokenType>& types);
    Token consume(TokenType type, const std::string& message);
    Token peek() const;
    Token previous() const;
    bool isAtEnd() const;

    void synchronize();
    void error(const Token& token, const std::string& message);
};


#endif //PARSER_H