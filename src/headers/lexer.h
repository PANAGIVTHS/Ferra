#ifndef LEXER_H
#define LEXER_H

#include "token.h"

#define KEYWORD_COUNT (sizeof(keywords) / sizeof(Keyword))

#define peek() (source[lexer.currentOffset])
#define advance() (source[lexer.currentOffset++])

typedef struct keyword {
    const char* word;
    TokenType type;
} Keyword;

typedef struct {
    int startOffset;
    int currentOffset;
    int line;
} LexerInfo;

extern const char *source;
extern LexerInfo lexer;

void init_lexer(const char* initSource);
int is_end(void);
void skip_whitespace(void);
Token* tokenizer(TokenType type);
Token* identifierOrKeyword(void);
Token* number(void);
Token* lexToken(void);

#ifdef TEST_LEXER
const char* tokenTypeToString(TokenType type);
#endif

#endif // LEXER_H
