#include "parser.hpp"

#include <iostream>

Parser::Parser(const std::vector<Token>& tokens)
    : tokens(tokens), currentToken(0), hasError(false) {}

std::unique_ptr<Program> Parser::parse() {
    std::vector<std::unique_ptr<Stmt>> statements;

    while(!this->isAtEnd() && !this->hasError) {
        try {
            statements.push_back(this->statement());
        } catch(...) {
            this->synchronize();
        }
    }

    if(this->hasError)
        return std::make_unique<Program>(std::vector<std::unique_ptr<Stmt>>());

    return std::make_unique<Program>(std::move(statements));
}

std::unique_ptr<Stmt> Parser::statement() {
    if(this->match(TokenType::PRINT))
        return this->printStatement();

    if(this->match(TokenType::BRACKET_CURLY_LEFT))
        return this->blockStatement();

    if(this->match(TokenType::IF))
        return this->ifStatement();

    if(this->match(TokenType::LOOP))
        return this->loopStatement();

    if(this->match(TokenType::BREAK))
        return this->breakStatement();

    if(this->match(TokenType::CONTINUE))
        return this->continueStatement();

    if(this->match(TokenType::RETURN))
        return this->returnStatement();

    if(this->match({TokenType::VAR, TokenType::CONST}))
        return this->varDeclaration();

    if(this->match(TokenType::FUNCTION))
        return this->functionDeclaration();

    // Diğer durumlarda expression
    return expressionStatement();
}

std::unique_ptr<Stmt> Parser::printStatement() {
    std::unique_ptr<Expression> value = this->expression(); // Ekrana yazılacak değer

    this->match(TokenType::SEMI_COLON);

    return std::make_unique<PrintStmt>(std::move(value));
}

std::unique_ptr<Stmt> Parser::expressionStatement() {
    std::unique_ptr<Expression> value = this->expression();

    this->match(TokenType::SEMI_COLON);

    return std::make_unique<ExpressionStmt>(std::move(value));
}

std::unique_ptr<Stmt> Parser::blockStatement() {
    std::vector<std::unique_ptr<Stmt>> statements;

    while(!this->check(TokenType::BRACKET_CURLY_RIGHT) && !this->isAtEnd())
        statements.push_back(this->statement());

    consume(TokenType::BRACKET_CURLY_RIGHT, "Blok sonunda '}' bekleniyor.");

    return std::make_unique<BlockStmt>(std::move(statements));
}

std::unique_ptr<Stmt> Parser::ifStatement() {
    bool hasParentheses = this->match(TokenType::BRACKET_ROUND_LEFT); // koşul parantezi

    std::unique_ptr<Expression> condition = this->expression();

    if(hasParentheses)
        this->consume(TokenType::BRACKET_ROUND_RIGHT, "Koşul sonunda ')' bekleniyor.");


    std::unique_ptr<Stmt> thenBranch = this->statement();

    std::unique_ptr<Stmt> elseBranch = nullptr;
    if(this->match(TokenType::ELSE))
        elseBranch = this->statement();

    return std::make_unique<IfStmt>(std::move(condition), std::move(thenBranch), std::move(elseBranch));
}

std::unique_ptr<Stmt> Parser::loopStatement() {
    bool hasParentheses = this->match(TokenType::BRACKET_ROUND_LEFT);

    std::unique_ptr<Expression> condition = this->expression();

    if (hasParentheses)
        this->consume(TokenType::BRACKET_ROUND_RIGHT, "Döngü koşulu sonunda ')' bekleniyor.");

    std::unique_ptr<Stmt> body = this->statement();

    return std::make_unique<LoopStmt>(std::move(condition), std::move(body));
}

std::unique_ptr<Stmt> Parser::breakStatement() {
    Token keyword = this->previous();

    this->match(TokenType::SEMI_COLON);

    return std::make_unique<BreakStmt>(keyword);
}

std::unique_ptr<Stmt> Parser::continueStatement() {
    Token keyword = this->previous();

    this->match(TokenType::SEMI_COLON);

    return std::make_unique<ContinueStmt>(keyword);
}

std::unique_ptr<Stmt> Parser::returnStatement() {
    Token keyword = this->previous();
    std::unique_ptr<Expression> value = nullptr;

    if(!this->check(TokenType::SEMI_COLON))
        value = this->expression();

        this->match(TokenType::SEMI_COLON);

    return std::make_unique<ReturnStmt>(keyword, std::move(value));
}

std::unique_ptr<Stmt> Parser::varDeclaration() {
    bool isConst = this->previous().type == TokenType::CONST;

    Token name = this->consume(TokenType::IDENTIFIER, "Değişken adı bekleniyor");

    this->consume(TokenType::COLON, "Değişken adından sonra ':' bekleniyor.");

    Token type;
    if(this->match({TokenType::TYPE_STRING, TokenType::TYPE_NUMBER, TokenType::TYPE_BOOL}))
        type = this->previous();
    else
        this->error(this->peek(), "Geçerli bir veri tipi bekleniyor (metin, sayı, doğruluk).");

    bool isArray = false;

    if(this->match(TokenType::BRACKET_SQUARE_LEFT)) {
        this->consume(TokenType::BRACKET_SQUARE_RIGHT, "Dizi tanımında ']' bekleniyor.");
        isArray = true;
    }

    std::unique_ptr<Expression> initializer = nullptr;

    if(this->match(TokenType::EQUAL)) {
        if(isArray && this->check(TokenType::BRACKET_SQUARE_LEFT))
            initializer = this->arrayExpression();
        else
            initializer = this->expression();
    }

    this->match(TokenType::SEMI_COLON);

    return std::make_unique<VarDeclStmt>(name, type, isArray, isConst, std::move(initializer));
}

std::unique_ptr<Stmt> Parser::functionDeclaration() {
    Token name = this->consume(TokenType::IDENTIFIER, "Fonksiyon adı bekleniyor");

    Token returnType;
    if(this->match(TokenType::COLON)) {
        if(this->match({TokenType::TYPE_STRING, TokenType::TYPE_NUMBER, TokenType::TYPE_BOOL}))
            returnType = this->previous();
        else
            this->error(peek(), "Geçerli bir dönüş tipi bekleniyor (metin, sayı, doğruluk).");
    }

    this->consume(TokenType::ARROW, "Fonksiyon adından sonra '=>' bekleniyor.");

    std::vector<Token> paramNames;
    std::vector<Token> paramTypes;

    if(!this->check(TokenType::BRACKET_CURLY_LEFT)) {
        do {
            paramNames.push_back(this->consume(TokenType::IDENTIFIER, "Parametre adı bekleniyor."));

            this->consume(TokenType::COLON, "Parametre adından sonra ':' bekleniyor.");

            if (this->match({TokenType::TYPE_STRING, TokenType::TYPE_NUMBER, TokenType::TYPE_BOOL}))
                paramTypes.push_back(previous());
            else
                this->error(this->peek(), "Geçerli bir parametre tipi bekleniyor (metin, sayı, doğruluk).");

            if (match(TokenType::BRACKET_SQUARE_LEFT)) {
                Token& lastType = paramTypes.back();
                std::string newTypeName = std::string(lastType.start, lastType.length) + "[]";

                this->consume(TokenType::BRACKET_SQUARE_RIGHT, "Dizi parametresinde ']' bekleniyor.");
            }

        } while(this->match(TokenType::COMMA));
    }

    this->consume(TokenType::BRACKET_CURLY_LEFT, "Fonksiyon parametrelerinden sonra '{' bekleniyor.");

    auto body = std::unique_ptr<BlockStmt>(static_cast<BlockStmt*>(this->blockStatement().release()));

    return std::make_unique<FunctionDeclStmt>(name, returnType, std::move(paramNames), std::move(paramTypes), std::move(body));
}


///// EXPRESSION METOTLARI /////
std::unique_ptr<Expression> Parser::expression() {
    return this->assignment();
}

std::unique_ptr<Expression> Parser::assignment() {
    std::unique_ptr<Expression> expression = this->logicalOr();

    if(this->match(TokenType::EQUAL)) {
        Token equals = this->previous();
        std::unique_ptr<Expression> value = this->assignment();

        if (VariableExpression* varExpr = dynamic_cast<VariableExpression*>(expression.get())) {
            Token name = varExpr->name;
            return std::make_unique<AssignExpression>(name, std::move(value));
        }

        if (auto* arrayAccessExpr = dynamic_cast<ArrayAccessExpression*>(expression.get())) {
            // Dizi elemanı atama işlemi için özel bir AST düğümü oluşturulacak
            this->error(equals, "Geçersiz atama hedefi.");
        }

        this->error(equals, "Geçersiz atama hedefi.");
    }

    return expression;
}

std::unique_ptr<Expression> Parser::logicalOr() {
    std::unique_ptr<Expression> expression = this->logicalAnd();

    while(this->match(TokenType::OR)) {
        Token op = this->previous();
        std::unique_ptr<Expression> right = this->logicalAnd();
        expression = std::make_unique<LogicalExpression>(op, std::move(expression), std::move(right));
    }

    return expression;
}

std::unique_ptr<Expression> Parser::logicalAnd() {
    std::unique_ptr<Expression> expression = this->equality();

    while(this->match(TokenType::AND)) {
        Token op = this->previous();
        std::unique_ptr<Expression> right = this->equality();
        expression = std::make_unique<LogicalExpression>(op, std::move(expression), std::move(right));
    }

    return expression;
}

std::unique_ptr<Expression> Parser::equality() {
    std::unique_ptr<Expression> expression = this->comparison();

    while(this->match({TokenType::EQUAL_EQUAL, TokenType::BANG_EQUAL})) {
        Token op = this->previous();
        std::unique_ptr<Expression> right = this->comparison();
        expression = std::make_unique<ComparisonExpression>(op, std::move(expression), std::move(right));
    }

    return expression;
}

std::unique_ptr<Expression> Parser::comparison() {
    std::unique_ptr<Expression> expression = this->term();

    while(this->match({TokenType::LESS, TokenType::LESS_EQUAL, TokenType::GREAT, TokenType::GREAT_EQUAL})) {
        Token op = this->previous();
        std::unique_ptr<Expression> right = this->term();
        expression = std::make_unique<ComparisonExpression>(op, std::move(expression), std::move(right));
    }

    return expression;
}

std::unique_ptr<Expression> Parser::term() {
    std::unique_ptr<Expression> expression = this->factor();

    while(this->match({TokenType::PLUS, TokenType::MINUS})) {
        Token op = this->previous();
        auto right = this->factor();
        expression = std::make_unique<BinaryExpression>(op, std::move(expression), std::move(right));
    }

    return expression;
}

std::unique_ptr<Expression> Parser::factor() {
    std::unique_ptr<Expression> expression = this->power();

    while(this->match({TokenType::MULTIPLY, TokenType::DIVIDE, TokenType::MODULUS})) {
        Token op = this->previous();
        std::unique_ptr<Expression> right = this->power();
        expression = std::make_unique<BinaryExpression>(op, std::move(expression), std::move(right));
    }

    return expression;
}

std::unique_ptr<Expression> Parser::power() {
    std::unique_ptr<Expression> expression = this->unary();

    while(this->match(TokenType::POWER)) {
        Token op = this->previous();
        std::unique_ptr<Expression> right = this->unary();
        expression = std::make_unique<BinaryExpression>(op, std::move(expression), std::move(right));
    }

    return expression;
}

std::unique_ptr<Expression> Parser::unary() {
    if(this->match({TokenType::MINUS, TokenType::BANG})) {
        Token op = this->previous();
        std::unique_ptr<Expression> right = this->unary();

        return std::make_unique<UnaryExpression>(op, std::move(right));
    }

    return this->call();
}

std::unique_ptr<Expression> Parser::call() {
    std::unique_ptr<Expression> expression = this->primary();

    while(true) {
        if(this->match(TokenType::BRACKET_ROUND_LEFT))
            expression = this->finishCall(std::move(expression));
        else if (match(TokenType::BRACKET_SQUARE_LEFT))
            expression = arrayAccess(std::move(expression));
        else
            break;
    }

    return expression;
}

std::unique_ptr<Expression> Parser::finishCall(std::unique_ptr<Expression> callee) {
    std::vector<std::unique_ptr<Expression>> arguments;

    if(!this->check(TokenType::BRACKET_ROUND_RIGHT)) {
        do {
            arguments.push_back(expression());
        } while(this->match(TokenType::COMMA));
    }

    Token paren = this->consume(TokenType::BRACKET_ROUND_RIGHT, "Fonksiyon çağrısı sonunda ')' bekleniyor.");

    return std::make_unique<CallExpression>(std::move(callee), paren, std::move(arguments));
}

std::unique_ptr<Expression> Parser::arrayAccess(std::unique_ptr<Expression> array) {
    std::unique_ptr<Expression> index = this->expression();
    Token bracket = this->consume(TokenType::BRACKET_SQUARE_RIGHT, "Dizi erişiminde ']' bekleniyor.");

    return std::make_unique<ArrayAccessExpression>(std::move(array), std::move(index), bracket);
}

std::unique_ptr<Expression> Parser::arrayExpression() {
    Token bracket = this->consume(TokenType::BRACKET_SQUARE_LEFT, "Dizi ifadesinde '[' bekleniyor.");
    std::vector<std::unique_ptr<Expression>> elements;

    if(!this->check(TokenType::BRACKET_SQUARE_RIGHT)) {
        do {
            elements.push_back(this->expression());
        } while(this->match(TokenType::COMMA));

        this->consume(TokenType::BRACKET_SQUARE_RIGHT, "Dizi ifadesinde ']' bekleniyor.");

        return std::make_unique<ArrayExpression>(std::move(elements), bracket);
    }
}

std::unique_ptr<Expression> Parser::primary() {
    if(this->match(TokenType::STRING_LITERAL))
        return std::make_unique<LiteralExpression>(this->previous());

    if(this->match(TokenType::NUMBER_LITERAL))
        return std::make_unique<LiteralExpression>(this->previous());

    if(this->match({TokenType::TRUE, TokenType::FALSE}))
        return std::make_unique<LiteralExpression>(this->previous());

    if(this->match(TokenType::BRACKET_SQUARE_LEFT))
        return this->arrayExpression();

    if(this->match(TokenType::IDENTIFIER))
        return std::make_unique<VariableExpression>(this->previous());

    if (this->match(TokenType::BRACKET_ROUND_LEFT)) {
        std::unique_ptr<Expression> expression = this->expression();
        this->consume(TokenType::BRACKET_ROUND_RIGHT, "İfade sonunda ')' bekleniyor.");
        return expression;
    }

    this->error(this->peek(), "ifade bekleniyor.");
}

///// EXPRESSION METOTLARI /////



Token Parser::advance() {
    if(!this->isAtEnd())
        this->currentToken++;
    return this->previous();
}

bool Parser::check(TokenType type) const {
    if(this->isAtEnd())
        return false;
    return this->peek().type == type;
}

bool Parser::match(TokenType type) {
    if(this->check(type)) {
        this->advance();
        return true;
    }
    return false;
}

bool Parser::match(const std::initializer_list<TokenType>& types) {
    for(auto type : types) {
        if(this->check(type)) {
            this->advance();
            return true;
        }
    }
    return false;
}

Token Parser::consume(TokenType type, const std::string& message) {
    if(this->check(type))
        return this->advance();

    this->error(this->peek(), message);
}

Token Parser::peek() const {
    return this->tokens[this->currentToken];
}

Token Parser::previous() const {
    return this->tokens[this->currentToken - 1];
}

bool Parser::isAtEnd() const {
    return this->peek().type == TokenType::TOKEN_EOF;
}

void Parser::synchronize() {
    this->advance();

    while(!this->isAtEnd()) {
        if(this->previous().type == TokenType::SEMI_COLON)
            return;

        switch(this->peek().type) {
        case TokenType::VAR:
            case TokenType::CONST:
            case TokenType::FUNCTION:
            case TokenType::IF:
            case TokenType::LOOP:
            case TokenType::RETURN:
            case TokenType::PRINT:
                return;
            default:
                break;
        }
        this->advance();
    }
}

void Parser::error(const Token& token, const std::string& message) {
    hasError = true;

    std::cerr << "\n\033[1;31m========== SÖZDİZİMİ HATASI ==========\033[0m" << std::endl;

    if (token.type == TokenType::TOKEN_EOF) {
        std::cerr << "\033[1;31mDosya sonu: " << message << "\033[0m" << std::endl;
    } else {
        std::string tokenValue(token.start, token.length);
        std::cerr << "\033[1;31mSözdizimi hatası: " << message << "\033[0m" << std::endl;
        std::cerr << "Token: '" << tokenValue << "'" << std::endl;
        std::cerr << "Dosya: " << token.fileName << std::endl;
        std::cerr << "Satır: " << token.line << ", Sütun: " << token.column << std::endl;
    }

    std::cerr << "\033[1;31m=====================================\033[0m" << std::endl;

    throw std::exception();
}