#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <stdbool.h>

#define KEYWORD_COUNT (sizeof(keywords) / sizeof(Keyword))

#define peek() (source[lexer.currentOffset])
#define advance() (source[lexer.currentOffset++])

typedef enum {
    LEFT,
    RIGHT
} AssocType;

typedef struct keyword {
    const char* word;
    TokenType type;
} Keyword;

typedef struct {
    int startOffset;
    int currentOffset;
    int line;
} LexerInfo;

typedef struct {
    TokenType type;
    int precedence;
    bool canUnary;
    AssocType assoc;
} OperatorInfo;

extern const char *source;
extern LexerInfo lexer;

void init_lexer(const char* initSource);
int is_end(void);
void skip_whitespace(void);
Token* tokenizer(TokenType type);
Token* identifierOrKeyword(void);
Token* number(void);
Token* lexToken(void);
void tokenizeAll(void);

#ifdef TEST_LEXER
const char* tokenTypeToString(TokenType type);
#endif

#endif // LEXER_H
