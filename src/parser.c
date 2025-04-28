#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "headers/ast.h"
#include "headers/lexer.h"
#include "headers/utils.h"

#define TEST_PARSER

#define advanceParser() (parser.tokens[parser.current++])
#define peekParser(offset) (parser.tokens[parser.current + offset])

Parser parser;

ASTNode *parseExpression();

char *strndup(const char *s, size_t n) {
    char *dup = malloc(n + 1);
    if (dup) {
        strncpy(dup, s, n);
        dup[n] = '\0';
    }
    return dup;
}

int strtoi(const char *str, int pos, int len) {
    int result = 0;

    for (int i = 0; i < len; ++i) {
        char c = str[pos + i];
        if (c < '0' || c > '9') break;
        result = result * 10 + (c - '0');
    }

    return result;
}

void init_parser() {
    parser.current = 0;
    parser.tokenCount = 0;
    parser.tokens = NULL;
}

Token *allocToken() {
    parser.tokens = (Token**) realloc(parser.tokens, (parser.tokenCount + 1) * sizeof(Token*));
    allocError(parser.tokens, "allocToken");

    parser.tokens[parser.tokenCount] = malloc(sizeof(Token));
    allocError(parser.tokens[parser.tokenCount], "allocTokenInner");

    return parser.tokens[parser.tokenCount++];
}

bool match(TokenType expectedType) {
    if (parser.tokens[parser.current]->type == expectedType) {
        (void) advanceParser();
        return true;
    }

    return false;
}

Token *consume(TokenType expectedType, const char *errorMsg) {
    if (parser.tokens[parser.current]->type == expectedType) {
        return advanceParser();
    }
    fprintf(stderr, "%s", errorMsg);

    return NULL;
}

ASTNode *parsePrimary() {
    Token *token = peekParser(0);

    if (match(TOKEN_INT_LITERAL)) {
        ASTLiteralInt init = { .value = strtoi(token->start, 0, token->length) };
        return allocASTNode(NODE_LITERAL_INT, token->line, &init);
    }

    if (match(TOKEN_IDENTIFIER)) {
        ASTIdentifier init = { .name = strndup(token->start, token->length) };
        return allocASTNode(NODE_IDENTIFIER, token->line, &init);
    }

    if (match(TOKEN_LPAREN)) {
        ASTNode *expr = parseExpression();
        consume(TOKEN_RPAREN, "Expect ')' after expression.");
        ASTGrouping init = { .expression = expr };
        return allocASTNode(NODE_GROUPING, token->line, &init);
    }

    fprintf(stderr, "Unexpected token in primary expression.");
    return NULL;
}

ASTNode *parseExpression() {
    ASTNode *left = parsePrimary();

    while (match(TOKEN_PLUS) || match(TOKEN_MINUS)) {
        Token *operator = peekParser(-1);
        ASTNode *right = parsePrimary();

        // Alocate the binary expression node
        ASTBinaryExpr init = {.left = left, .right = right, .operator = operator};
        left = allocASTNode(NODE_BINARY_EXPR, operator->line, &init);
    }

    return left;
}


void printAST(ASTNode *node) {
    if (!node) return;

    printf("{\n");

    switch (node->type) {
        case NODE_LITERAL_INT:
            printf("  \"type\": \"LiteralInt\",\n");
            printf("  \"value\": %d\n", node->as.literal.value);
            break;

        case NODE_BINARY_EXPR:
            printf("  \"type\": \"BinaryExpr\",\n");
            printf("  \"operator\": \"%s\",\n", strndup(node->as.binary.operator->start, 1));
            printf("  \"left\": ");
            printAST(node->as.binary.left);
            printf(",\n");
            printf("  \"right\": ");
            printAST(node->as.binary.right);
            break;

        default:
            printf("  \"type\": \"UnknownNode\"\n");
            break;
    }

    printf("}\n");
}

int main() {
    const char *source = "3 + 5";

    // Initialize lexer and parser
    init_lexer(source);
    init_parser();  // this should reset parser.tokens = NULL, parser.tokenCount = 0, etc.

    // Tokenize entire input
    while (1) {
        Token *token = lexToken();
        if (token->type == TOKEN_EOF) break;
    }

    // Now parse the expression
    ASTNode *expression = parseExpression();

    if (expression) {
        printf("Expression parsed successfully!\n");
        printAST(expression);  // You likely have this already
    } else {
        printf("Failed to parse expression.\n");
    }

    return 0;
}