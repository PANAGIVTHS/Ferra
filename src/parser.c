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
#define OPERATOR_COUNT (int)(sizeof(operatorTokens) / sizeof(TokenType))

ASTNode *parseExpression();

Parser parser;

OperatorInfo operatorInfoTable[] = {
    { TOKEN_LOGICAL_OR,     1,  LEFT },  // ||
    { TOKEN_LOGICAL_EQ,     4,  LEFT },  // ==
    { TOKEN_LOGICAL_NE,     4,  LEFT },  // !=
    { TOKEN_LOGICAL_LT,     5,  LEFT },  // <
    { TOKEN_LOGICAL_LTE,    5,  LEFT },  // <=
    { TOKEN_LOGICAL_GT,     5,  LEFT },  // >
    { TOKEN_LOGICAL_GTE,    5,  LEFT },  // >=
    { TOKEN_LOGICAL_AND,    2,  LEFT },  // &&
    { TOKEN_SHIFT_LEFT,     6,  LEFT },  // <<
    { TOKEN_SHIFT_RIGHT,    6,  LEFT },  // >>
    { TOKEN_EQUAL,          9,  RIGHT },  // =
    { TOKEN_PLUS,           7,  LEFT },  // +
    { TOKEN_MINUS,          7,  LEFT },  // -
    { TOKEN_STAR,           8,  LEFT },  // *
    { TOKEN_SLASH,          8,  LEFT },  // /
    { TOKEN_PIPE,           3,  LEFT },  // |
    { TOKEN_PERCENT,        8,  LEFT },  // %
};

TokenType operatorTokens[] = {
    TOKEN_PIPE,
    TOKEN_PLUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_MINUS,
    TOKEN_EQUAL,
    TOKEN_PERCENT,
    TOKEN_LOGICAL_GT,
    TOKEN_LOGICAL_OR,
    TOKEN_LOGICAL_EQ,
    TOKEN_LOGICAL_NE,
    TOKEN_LOGICAL_LT,
    TOKEN_LOGICAL_LTE,
    TOKEN_LOGICAL_AND,
    TOKEN_LOGICAL_GTE,
    TOKEN_SHIFT_LEFT,
    TOKEN_SHIFT_RIGHT,
};

OperatorInfo getOperatorInfo(const TokenType type) {
    return operatorInfoTable[type];
}

char *strndup(const char *s, size_t n) {
    char *dup = malloc(n + 1);
    if (dup) {
        strncpy(dup, s, n);
        dup[n] = '\0';
    }
    return dup;
}

float strtofl(const char *str, int pos, int len) {
    float result = 0.0f;
    float frac_div = 10.0f;
    int seen_dot = 0;

    for (int i = 0; i < len; ++i) {
        char c = str[pos + i];

        if (c == '.') {
            if (seen_dot) break; // multiple dots = invalid
            seen_dot = 1;
            continue;
        }

        if (c < '0' || c > '9') break; // stop on non-digit

        if (!seen_dot) {
            result = result * 10.0f + (c - '0');
        } else {
            result += (c - '0') / frac_div;
            frac_div *= 10.0f;
        }
    }

    return result;
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

Token *allocToken() {
    parser.tokens = (Token**) realloc(parser.tokens, (parser.tokenCount + 1) * sizeof(Token*));
    allocError(parser.tokens, "allocToken");

    parser.tokens[parser.tokenCount] = malloc(sizeof(Token));
    allocError(parser.tokens[parser.tokenCount], "allocTokenInner");

    return parser.tokens[parser.tokenCount++];
}

bool match(const TokenType expectedType) {
    // EOF has been matched
    if (parser.current >= parser.tokenCount) return false;
    if (parser.tokens[parser.current] == NULL) return false;

    if (parser.tokens[parser.current]->type == expectedType) {
        advanceParser();
        return true;
    }

    return false;
}


// !Notice: This is Slower that || but more readable for now
bool matchOperator() {
    if (parser.current >= parser.tokenCount ||
        parser.tokens[parser.current]->type == TOKEN_EOF) {
        return false;
        }

    for (int i = 0; i < OPERATOR_COUNT; i++) {
        if (match(operatorTokens[i])) return true;
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

void init_parser() {
    parser.current = 0;
    parser.tokenCount = 0;
    parser.tokens = NULL;
}

ASTNode *parsePrimary() {
    Token *token = peekParser(0);

    if (parser.current >= parser.tokenCount || parser.tokens[parser.current]->type == TOKEN_EOF) {
        return NULL;
    }

    if (match(TOKEN_INT_LITERAL)) {
        ASTLiteralInt init = { .value = strtoi(token->start, 0, token->length) };
        return allocASTNode(NODE_LITERAL_INT, token->line, &init);
    }

    if (match(TOKEN_FLOAT_LITERAL)) {
        ASTLiteralFloat init = { .value = strtofl(token->start, 0, token->length) };
        return allocASTNode(NODE_LITERAL_FLOAT, token->line, &init);
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

    printf("%d", parser.tokens[parser.current-1]->type);
    fprintf(stderr, "Unexpected token in primary expression.");
    return NULL;
}

ASTNode *parseExpression() {
    ASTNode *left = parsePrimary();

    while (matchOperator()) {
        Token *operator = peekParser(-1);
        ASTNode *right = parsePrimary();

        if (!right) {
            fprintf(stderr, "Expected expression after operator '%.*s'\n",
                    operator->length, operator->start);
            return NULL;
        }

        ASTBinaryExpr init = {.left = left, .right = right, .operator = operator};
        left = allocASTNode(NODE_BINARY_EXPR, operator->line, &init);
    }

    return left;
}


void printIndent(int depth) {
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
}

void printAST(ASTNode *node, int depth);

void printASTInline(ASTNode *node, int depth) {
    if (!node) return;

    printf("{\n");

    switch (node->type) {
    case NODE_LITERAL_INT:
        printIndent(depth + 1);
        printf("\"type\": \"LiteralInt\",\n");
        printIndent(depth + 1);
        printf("\"value\": %d\n", node->as.literalInteger.value);
        break;

    case NODE_LITERAL_FLOAT:
        printIndent(depth + 1);
        printf("\"type\": \"LiteralFloat\",\n");
        printIndent(depth + 1);
        printf("\"value\": %f\n", node->as.literalFloat.value);
        break;

    case NODE_IDENTIFIER:
        printIndent(depth + 1);
        printf("\"type\": \"Identifier\",\n");
        printIndent(depth + 1);
        printf("\"name\": \"%s\"\n", node->as.identifier.name);
        break;

    case NODE_BINARY_EXPR:
        printIndent(depth + 1);
        printf("\"type\": \"BinaryExpr\",\n");
        printIndent(depth + 1);
        printf("\"operator\": \"%.*s\",\n", node->as.binary.operator->length, node->as.binary.operator->start);

        printIndent(depth + 1);
        printf("\"left\": ");
        printASTInline(node->as.binary.left, depth + 1);
        printf(",\n");

        printIndent(depth + 1);
        printf("\"right\": ");
        printASTInline(node->as.binary.right, depth + 1);
        printf("\n");
        break;

    case NODE_GROUPING:
        printIndent(depth + 1);
        printf("\"type\": \"Grouping\",\n");
        printIndent(depth + 1);
        printf("\"expression\": ");
        printASTInline(node->as.grouping.expression, depth + 1);
        printf("\n");
        break;

    default:
        printIndent(depth + 1);
        printf("\"type\": \"UnknownNode\"\n");
        break;
    }

    printIndent(depth);
    printf("}");
}

void printAST(ASTNode *node, int depth) {
    printIndent(depth);
    printASTInline(node, depth);
    if (depth != 0)
        printf(",\n");
    else
        printf("\n");
}

int main() {
    const char *source = "var | 5.432763999 % ((var22 / 4) >> 1) || (2 + 3.) * 4 - .5";

    // Initialize lexer and parser
    init_lexer(source);
    init_parser();  // this should reset parser.tokens = NULL, parser.tokenCount = 0, etc.

    // Tokenize entire input
    tokenizeAll();

    // Now parse the expression
    ASTNode *expression = parseExpression();

    if (expression) {
        printf("Expression parsed successfully!\nWith source string: %s\n", source);
        printAST(expression, 0);  // You likely have this already
    } else {
        printf("Failed to parse expression.\n");
    }

    return 0;
}