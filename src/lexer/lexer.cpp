#include "lexer.hpp"
#include <cstring>
#include <sstream>

Lexer::Lexer(const std::string& source, const std::string& fileName)
    :source(source), fileName(fileName), startPosition(0), currentPosition(0), currentLine(1), currentColumn(0) {}

std::vector<Token> Lexer::scanner() {
    std::vector<Token> tokens;

    do
        tokens.push_back(this->scanToken());
    while(tokens.back().type != TokenType::TOKEN_EOF);

    return tokens;
}

Token Lexer::scanToken() {

    this->skipWhiteSpace();

    this->startPosition = this->currentPosition;

    if(this->isAtEnd()) 
        return this->makeToken(TokenType::TOKEN_EOF);

    char c = this->advance();

    switch(c) {
        case '+':
            if (this->isMatch('+'))
                return this->makeToken(TokenType::PLUS_PLUS);
            if(this->isMatch('='))
                return this->makeToken(TokenType::PLUS_EQUAL);
            return this->makeToken(TokenType::PLUS);
        case '-':
            if(this->isMatch('-'))
                return this->makeToken(TokenType::MINUS_MINUS);
            if(this->isMatch('='))
                return this->makeToken(TokenType::MINUS_EQUAL);
            return this->makeToken(TokenType::MINUS);
        case '*':
            if(this->isMatch('*'))
                return this->makeToken(TokenType::POWER);
            if(this->isMatch('='))
                return this->makeToken(TokenType::MULTIPLY_EQUAL);
            return this->makeToken(TokenType::MULTIPLY);
        case '/':
            if(this->isMatch('='))
                return this->makeToken(TokenType::DIVIDE_EQUAL);
            return this->makeToken(TokenType::DIVIDE);
        case '%':
            if(this->isMatch('='))
                return this->makeToken(TokenType::MODULUS_EQUAL);
            return this->makeToken(TokenType::MODULUS);
        case '(':
            return this->makeToken(TokenType::BRACKET_ROUND_LEFT);
        case ')':
            return this->makeToken(TokenType::BRACKET_ROUND_RIGHT);
        case '{':
            return this->makeToken(TokenType::BRACKET_CURLY_LEFT);
        case '}':
            return this->makeToken(TokenType::BRACKET_CURLY_RIGHT);
        case '[':
            return this->makeToken(TokenType::BRACKET_SQUARE_LEFT);
        case ']':
            return this->makeToken(TokenType::BRACKET_SQUARE_RIGHT);
        case '<':
            if(this->isMatch('='))
                return this->makeToken(TokenType::LESS_EQUAL);
            return this->makeToken(TokenType::LESS);
        case '>':
            if(this->isMatch('='))
                return this->makeToken(TokenType::GREAT_EQUAL);
            return this->makeToken(TokenType::GREAT);
        case '.':
            return this->makeToken(TokenType::DOT);
        case ',':
            return this->makeToken(TokenType::COMMA);
        case ':':
            return this->makeToken(TokenType::COLON);
        case '!':
            if(this->isMatch('='))
                return this->makeToken(TokenType::BANG_EQUAL);
            return this->makeToken(TokenType::BANG);
        case '=':
            return this->makeToken(TokenType::EQUAL);
        // && || yapılacak
        case '&':
            if(this->isMatch('&'))
                return this->makeToken(TokenType::AND);
        case '|':
            if(this->isMatch('|'))
                return this->makeToken(TokenType::OR);
        case '"':
            return this->stringLiteral();
        default:
            if(this->isAlpha(c))
                return this->identifierLiteral(c);
            if(this->isDigit(c))
                return this->numberLiteral();
            return this->errorToken("Bilinmeyen karakter");
    }
}

Token Lexer::makeToken(TokenType type) {
    return Token(type, this->fileName, &this->source[this->startPosition], this->currentPosition - this->startPosition, this->currentLine, this->currentColumn);
}

Token Lexer::errorToken(const char* errMsg) {
    return Token(TokenType::TOKEN_ERROR, this->fileName, errMsg, strlen(errMsg), this->currentLine, this->currentColumn);
}

Token Lexer::stringLiteral() {
    while(!this->isAtEnd() && this->peek() != '"') {
       this->advance();
    }

    // Tamamlanmayan string
    if(this->isAtEnd())
        return this->errorToken("Sonlandırılmayan metinsel ifade");

    this->advance();
    return this->makeToken(TokenType::STRING_LITERAL);
}

Token Lexer::identifierLiteral(char start) {
    std::stringstream lexeme;
    lexeme << start;

    while(this->isAlpha(this->peek()) || this->isDigit(this->peek()))
        lexeme << this->advance();

    std::string lexemeStr = lexeme.str();

    if(lexemeStr == "metin")
        return this->makeToken(TokenType::TYPE_STRING);
    if(lexemeStr == "sayi")
        return this->makeToken(TokenType::TYPE_NUMBER);
    if(lexemeStr == "doğruluk")
        return this->makeToken(TokenType::TYPE_BOOL);
    if(lexemeStr == "doğru")
        return this->makeToken(TokenType::TRUE);
    if(lexemeStr == "yanlış")
        return this->makeToken(TokenType::FALSE);
    if(lexemeStr == "eğer")
        return this->makeToken(TokenType::IF);
    if(lexemeStr == "yoksa")
        return this->makeToken(TokenType::ELSE);
    if(lexemeStr == "döngü")
        return this->makeToken(TokenType::LOOP);
    if(lexemeStr == "deg")
        return this->makeToken(TokenType::VAR);
    if(lexemeStr == "sbt")
        return this->makeToken(TokenType::CONST);
    if(lexemeStr == "fonk")
        return this->makeToken(TokenType::FUNCTION);
    if(lexemeStr == "dön")
        return this->makeToken(TokenType::RETURN);
    if(lexemeStr == "eşle")
        return this->makeToken(TokenType::SWITCH);
    if(lexemeStr == "kır")
        return this->makeToken(TokenType::BREAK);
    if(lexemeStr == "devam")
        return this->makeToken(TokenType::CONTINUE);
    if(lexemeStr == "yaz")
        return this->makeToken(TokenType::PRINT);

    return this->makeToken(TokenType::IDENTIFIER);
}

Token Lexer::numberLiteral() {
    while(!this->isAtEnd() && this->isDigit(this->peek()))
        this->advance();

    if(this->isMatch('.'))
        while(!this->isAtEnd() && this->isDigit(this->peek()))
            this->advance();

    return makeToken(TokenType::NUMBER_LITERAL);
}

void Lexer::skipWhiteSpace() {
    while(true) {
        char c = this->peek();

        switch(c) {
            case ' ':
            case '\t':
            case '\r':
                this->advance();
                break;
            case '\n':
                this->currentLine++;
                this->currentColumn = 0;
                this->advance();
                break;
            
            // Comment Lines
            case '/':
                if(this->nextPeek() == '/') {
                    while(this->peek() != '\n' && !this->isAtEnd())
                        this->advance();
                    this->skipWhiteSpace();
                } 
                else if(this->nextPeek() == '*') {
                    while(!(this->peek() == '*' && this->nextPeek() == '/') && !this->isAtEnd()) {
                        char x = this->advance();
                        if(x == '\n')
                            this->currentLine++;
                    }
                    this->currentPosition += 2;
                    this->skipWhiteSpace();
                }
            default:
                return;
        }
    }
}

char Lexer::advance() {
    this->currentColumn++;
    return this->source[this->currentPosition++];
}

char Lexer::peek() {
    return this->source[this->currentPosition];
}

char Lexer::nextPeek() {
    if(this->isAtEnd())
        return '\0';
    return this->source[this->currentPosition+1];
}

bool Lexer::isMatch(char c) {
    if(this->isAtEnd() || this->peek() != c)
        return false;
    this->advance();
    return true;
}

bool Lexer::isAtEnd() {
    return this->currentPosition == this->source.length();
}

bool Lexer::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Lexer::isDigit(char c) {
    return c >= '0' && c <= '9';
}