#include "parser.hpp"

#include <cmath>
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
        ;

    if(this->match(TokenType::BRACKET_CURLY_LEFT))
        ;

    if(this->match(TokenType::IF))
        ;

    if(this->match(TokenType::LOOP))
        ;

    if(this->match(TokenType::BREAK))
        ;

    if(this->match(TokenType::CONTINUE))
        ;

    if(this->match(TokenType::RETURN))
        ;

    if(this->match({TokenType::VAR, TokenType::CONST}))
        ;

    if(this->match(TokenType::FUNCTION))
        ;

    // Diğer durumlarda expression
}

////// EXPRESSION METOTLARI /////

std::unique_ptr<Expression> Parser::expression() {
    return this->assignment();
}

std::unique_ptr<Expression> Parser::assignment() {
}

std::unique_ptr<Expression> Parser::equality() {
    std::unique_ptr<Expression> expression = this->comparison();

    while(this->match({TokenType::EQUAL_EQUAL, TokenType::BANG_EQUAL})) {
        Token op = this->previous();
        std::unique_ptr<Expression> right = this->comparison();
        expression = std::make_unique<BinaryExpression>(op, std::move(expression), std::move(right));
    }

    return expression;
}


std::unique_ptr<Expression> Parser::comparison() {
    std::unique_ptr<Expression> expression = this->term();

    while(this->match({TokenType::LESS, TokenType::LESS_EQUAL, TokenType::GREAT, TokenType::GREAT_EQUAL})) {
        Token op = this->previous();
        std::unique_ptr<Expression> right = this->term();
        expression = std::make_unique<BinaryExpression>(op, std::move(expression), std::move(right));
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

    Token paren = consume(TokenType::BRACKET_ROUND_RIGHT, "Fonksiyon çağrısı sonunda ')' bekleniyor.");

    return std::make_unique<CallExpression>(std::move(callee), paren, std::move(arguments));
}



std::unique_ptr<Expression> Parser::arrayAccess(std::unique_ptr<Expression> array) {
    std::unique_ptr<Expression> index = this->expression();
    Token bracket = this->consume(TokenType::BRACKET_SQUARE_RIGHT, "Dizi erişiminde ']' bekleniyor.");

    return std::make_unique<ArrayAccessExpression>(std::move(array), std::move(index), bracket);
}


std::unique_ptr<Expression> Parser::arrayExpression() {
    Token bracket = consume(TokenType::BRACKET_SQUARE_LEFT, "Dizi ifadesinde '[' bekleniyor.");
    std::vector<std::unique_ptr<Expression>> elements;

    if(!this->check(TokenType::BRACKET_SQUARE_RIGHT)) {
        do {
            elements.push_back(this->expression());
        } while(this->match(TokenType::COMMA));

        consume(TokenType::BRACKET_SQUARE_RIGHT, "Dizi ifadesinde ']' bekleniyor.");

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
        consume(TokenType::BRACKET_ROUND_RIGHT, "İfade sonunda ')' bekleniyor.");
        return expression;
    }

    this->error(this->peek(), "ifade bekleniyor.");
}


////// EXPRESSION METOTLARI /////



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

    // Hata durumunda bir istisna fırlatarak mevcut işlemi durdur
    throw std::exception();
}