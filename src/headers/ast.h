#ifndef AST_H
#define AST_H

#include <stdlib.h>
#include <string.h>
#include "token.h"

typedef enum {
    NODE_LITERAL_INT,
    NODE_LITERAL_FLOAT,
    NODE_IDENTIFIER,
    NODE_GROUPING,
    NODE_BINARY_EXPR,
    NODE_VAR_DECL,
    NODE_FUNCTION_DECL,
    NODE_IF_STMT,
    NODE_WHILE_STMT,
    NODE_RETURN,
    NODE_BLOCK
} NodeType;

typedef struct ASTNode ASTNode;

typedef struct {
    ASTNode *left;
    ASTNode *right;
    Token *operator;
} ASTBinaryExpr;

typedef struct {
    int value;
} ASTLiteralInt;

typedef struct {
    float value;
} ASTLiteralFloat;

typedef struct {
    char *name;
} ASTIdentifier;

typedef struct {
    ASTNode *expression;
} ASTGrouping;

struct ASTNode {
    NodeType type;
    int line;
    union {
        ASTBinaryExpr binary;
        ASTLiteralInt literalInteger;
        ASTLiteralFloat literalFloat;
        ASTIdentifier identifier;
        ASTGrouping grouping;
    } as;
};

typedef void (*InitFn)(ASTNode *node, void *data);

ASTNode *allocASTNode(NodeType type, int line, void *data);

#endif // AST_H
