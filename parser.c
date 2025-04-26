#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "token.h"

#define advanceParser () (parser.tokens[parser.current++])
#define peekParser () (parser.tokens[parser.current])


typedef struct parsetT {
    Token** tokens;
    int current;
    int tokenCount;
} Parser;

Parser parser;

void init_parser () {
    parser.current = 0;
    parser.tokenCount = 0;
    parser.tokens = NULL;
};

Token *allocToken() {
    parser.tokens = (Token**) realloc(parser.tokens, parser.tokenCount * sizeof(Token*));
    allocError(parser.tokens, "allocToken");

    parser.tokens[parser.tokenCount] = malloc(sizeof(Token));
    allocError(parser.tokens[parser.tokenCount ], "allocTokenInner");

    return parser.tokens[parser.tokenCount++];
}


bool match (TokenType expectedType) {
    if (parser.tokens[parser.current]->type == expectedType) {
        (void) advanceParser();
        return true;
    }

    return false;
}

Token *consume (TokenType expectedType, const char * errorMsg) {
    if (parser.tokens[parser.current]->type == expectedType) {
        return advanceParser();
    }
    fprintf(stderr, errorMsg);

    return NULL;
}