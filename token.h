#define allocError(ptr, func) \
    do { \
        if ((ptr) == NULL) { \
            fprintf(stderr, "Malloc - Allocation Error in %s\n", func); \
            return(NULL); \
        } \
    } while (0)

#ifndef TOKEN_H
#define TOKEN_H


typedef enum {
    TOKEN_EOF,
    TOKEN_INT_LITERAL,
    TOKEN_FLOAT_LITERAL,
    TOKEN_IDENTIFIER,

    // types
    TOKEN_TYPE_INT,
    TOKEN_TYPE_BOOL,
    TOKEN_TYPE_FLOAT,

    TOKEN_KEYWORD_LET,
    TOKEN_KEYWORD_FUN,
    TOKEN_KEYWORD_RETURN,
    TOKEN_KEYWORD_IF,
    TOKEN_KEYWORD_ELSE,
    TOKEN_KEYWORD_WHILE,
    TOKEN_KEYWORD_MATCH,

    TOKEN_ARROW,         // ->
    TOKEN_MATCH_ARROW,   // =>
    TOKEN_EQUAL,         // =
    TOKEN_PLUS,          // +
    TOKEN_MINUS,         // -
    TOKEN_STAR,          // *
    TOKEN_SLASH,         // /
    TOKEN_PIPE,          // |

    TOKEN_LPAREN,        // (
    TOKEN_RPAREN,        // )
    TOKEN_LBRACE,        // {
    TOKEN_RBRACE,        // }
    TOKEN_MATCH_WILDCARD,// _

    TOKEN_COMMA,         // ,
    TOKEN_SEMICOLON,     // ;
    TOKEN_COLON,         // :

    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    const char *start;
    TokenType type;
    int length;
    int line;
} Token;

#endif