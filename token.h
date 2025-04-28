#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOKEN_EOF,
    TOKEN_INT_LITERAL,
    TOKEN_FLOAT_LITERAL,
    TOKEN_IDENTIFIER,

    // Types
    TOKEN_TYPE_INT,
    TOKEN_TYPE_BOOL,
    TOKEN_TYPE_FLOAT,

    // Keywords
    TOKEN_KEYWORD_LET,
    TOKEN_KEYWORD_FUN,
    TOKEN_KEYWORD_RETURN,
    TOKEN_KEYWORD_IF,
    TOKEN_KEYWORD_ELSE,
    TOKEN_KEYWORD_WHILE,
    TOKEN_KEYWORD_MATCH,

    // Symbols
    TOKEN_ARROW,
    TOKEN_MATCH_ARROW,
    TOKEN_EQUAL,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_PIPE,
    TOKEN_PERCENT,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_MATCH_WILDCARD,
    TOKEN_COMMA,
    TOKEN_SEMICOLON,
    TOKEN_COLON,

    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    const char *start;
    TokenType type;
    int length;
    int line;
} Token;

typedef struct parsetT {
    Token** tokens;
    int current;
    int tokenCount;
} Parser;

Token *allocToken(void);

#endif // TOKEN_H
