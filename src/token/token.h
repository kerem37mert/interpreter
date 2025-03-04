#ifndef TOKEN_H
#define TOKEN_H

typedef enum _tokenType {
    TOKEN_EOF,
    TOKEN_ERROR,

    TOKEN_STRING_LITERAL,
    TOKEN_NUMBER_LITERAL,
    
    TOKEN_PLUS,                 // + 
    TOKEN_MINUS,                // -
    TOKEN_MULTIPLY,             // *
    TOKEN_DIVIDE,               // /
    TOKEN_MODULE,               // %
    
    // Keywords
    TOKEN_FUNCTION              // fonk
    
} TokenType;


typedef struct _token {
    TokenType type;
    char* value;
    char* filename;
    unsigned int line;
    unsigned int column;
    struct _token* next;
} Token;

#endif //TOKEN_H