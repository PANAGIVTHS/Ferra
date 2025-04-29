#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
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

    TOKEN_EOF = 17,
    TOKEN_INT_LITERAL = 18,
    TOKEN_FLOAT_LITERAL = 19,
    TOKEN_IDENTIFIER = 20,

    // Types
    TOKEN_TYPE_INT = 21,
    TOKEN_TYPE_BOOL = 22,
    TOKEN_TYPE_FLOAT = 23,

    // Keywords
    TOKEN_KEYWORD_LET = 24,
    TOKEN_KEYWORD_FUN = 25,
    TOKEN_KEYWORD_RETURN = 26,
    TOKEN_KEYWORD_IF = 27,
    TOKEN_KEYWORD_ELSE = 28,
    TOKEN_KEYWORD_WHILE = 29,
    TOKEN_KEYWORD_MATCH = 30,

    // Symbols
    TOKEN_ARROW = 31,
    TOKEN_MATCH_ARROW = 32,
    TOKEN_LPAREN = 33,
    TOKEN_RPAREN = 34,
    TOKEN_LBRACE = 35,
    TOKEN_RBRACE = 36,
    TOKEN_MATCH_WILDCARD = 37,
    TOKEN_COMMA = 38,
    TOKEN_SEMICOLON = 39,
    TOKEN_COLON = 40,

    // Artificial Types
    TOKEN_UNKNOWN = 41,
    TOKEN_NOT_OP = 42
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
