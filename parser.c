#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ast.h"

#define advanceParser () (parser.tokens[parser.current++])
#define peekParser () (parser.tokens[parser.current])



Parser parser;

int strtoi(const char *str, int pos, int len) {
    int result = 0;

    for (int i = 0; i < len; ++i) {
        char c = str[pos + i];
        if (c < '0' || c > '9') break; // Being Safe :)
        result = result * 10 + (c - '0');
    }

    return result;
}

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

ASTNode *parsePrimary() {
    ASTNode *newNode = NULL;
    Token *currentToken = parser.tokens[parser.current];

    switch (currentToken->type) {
        case TOKEN_INT_LITERAL: {
            // Integer Logic: Convert the literal to an integer and create a node
            int nodeValue = strtoi(currentToken->start, 0, currentToken->length);
            newNode = (ASTNode *) allocLiteralInt(nodeValue, currentToken->line);
            consume(TOKEN_INT_LITERAL, "Expected integer literal");
            break;
        }
        case TOKEN_LPAREN: {
            // Handle parentheses: Parse the expression inside the parentheses
            consume(TOKEN_LPAREN, "Expected '('");
            newNode = parseExpression();  // Recursively parse the expression inside parentheses
            consume(TOKEN_RPAREN, "Expected ')'");
            break;
        }
        default: {
            // Handle unexpected token type
            fprintf(stderr, "Unexpected token: %d\n", currentToken->type);
            return NULL;
        }
    }

    return newNode;
}

ASTNode *parseExpression() {
    ASTNode *left = parsePrimary();  // Start by parsing the primary expression

    while (match(TOKEN_PLUS) || match(TOKEN_MINUS) || match(TOKEN_STAR) || match(TOKEN_SLASH)) {
        // Parse the right-hand side
        Token *operator = parser.tokens[parser.current - 1];
        ASTNode *right = parsePrimary();

        // Combine the left and right expressions into a binary expression
        left = allocBinaryExpr(left, operator, right, operator->line);
    }

    // This will return either just the default parse or a binary expression (that might contain more of those)
    return left;
}
