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

    // OPERATORS
    // Logical Operators
    TOKEN_LOGICAL_OR = 0,
    TOKEN_LOGICAL_EQ = 1,
    TOKEN_LOGICAL_NE = 2,
    TOKEN_LOGICAL_LT = 3,
    TOKEN_LOGICAL_LTE = 4,
    TOKEN_LOGICAL_GT = 5,
    TOKEN_LOGICAL_GTE = 6,
    TOKEN_LOGICAL_AND = 7,

    TOKEN_SHIFT_LEFT = 8,
    TOKEN_SHIFT_RIGHT = 9,

    TOKEN_EQUAL = 10,
    TOKEN_PLUS = 11,
    TOKEN_MINUS = 12,
    TOKEN_STAR = 13,
    TOKEN_SLASH = 14,
    TOKEN_PIPE = 15,
    TOKEN_PERCENT = 16,

    // Symbols
    TOKEN_ARROW,
    TOKEN_MATCH_ARROW,
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
