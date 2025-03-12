#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

enum class TokenType {
    TOKEN_EOF,
    TOKEN_ERROR,

    STRING_LITERAL,
    NUMBER_LITERAL,

    IDENTIFIER,
    
    // one char
    PLUS,                         // + 
    MINUS,                        // -
    MULTIPLY,                     // *
    DIVIDE,                       // /
    MODULUS,                      // %
    EQUAL,                        // =
    BRACKET_ROUND_LEFT,           // (
    BRACKET_ROUND_RIGHT,          // )
    BRACKET_SQUARE_LEFT,          // [
    BRACKET_SQUARE_RIGHT,         // ]
    BRACKET_CURLY_LEFT,           // {
    BRACKET_CURLY_RIGHT,          // }
    LESS,                         // <
    GREAT,                        // >
    DOT,                          // .
    COMMA,                        // ,
    COLON,                        // :
    BANG,                         // !

    // two chars
    EQUAL_EQUAL,                  // ==
    LESS_EQUAL,                   // <=
    GREAT_EQUAL,                  // >=
    BANG_EQUAL,                   // !=
    PLUS_EQUAL,                   // +=
    MINUS_EQUAL,                  // -=
    MULTIPLY_EQUAL,               // *=
    DIVIDE_EQUAL,                 // /=
    MODULUS_EQUAL,                // %=
    PLUS_PLUS,                    // ++
    MINUS_MINUS,                  // --
    POWER,                        // **
    COLON_COLON,                  // ::
    AND,                          // &&
    OR,                           // ||
    ARROW,                        // =>


    // Keywords
    TYPE_STRING,                  // metin
    TYPE_NUMBER,                  // sayı
    TYPE_BOOL,                    // doğruluk

    TRUE,                         // doğru
    FALSE,                        // yanlış

    IF,                           // eğer
    ELSE,                         // yoksa
    LOOP,                         // döngü
    VAR,                          // deg
    CONST,                        // sbt
    FUNCTION,                     // fonk
    RETURN,                       // dön
    SWITCH,                       // eşle
    BREAK,                        // kır
    CONTINUE,                     // devam
    PRINT                         // yaz
};

class Token {
public:
    const TokenType type;
    const std::string fileName;
    const char* start;
    const unsigned int length;
    const unsigned int line;
    const unsigned int column;

    Token(TokenType type, const std::string& fileName, const char* start, unsigned int length, unsigned int line, unsigned int column);
};

#endif // TOKEN_HPP