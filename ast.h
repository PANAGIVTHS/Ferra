#include "token.h"

#ifndef AST_H
#define AST_H

#define allocError(ptr, func) \
    do { \
        if ((ptr) == NULL) { \
            fprintf(stderr, "Malloc - Allocation Error in %s\n", func); \
            return(NULL); \
        } \
    } while (0)


typedef enum {
    NODE_LITERAL_INT,
    NODE_LITERAL_FLOAT,
    NODE_IDENTIFIER,
    NODE_BINARY_EXPR,
    NODE_VAR_DECL,
    NODE_FUNCTION_DECL,
    NODE_IF_STMT,
    NODE_WHILE_STMT,
    NODE_RETURN,
    NODE_BLOCK
} NodeType;

typedef struct ASTNode {
    NodeType type;
    int line;
} ASTNode;

typedef struct {
    ASTNode base;
    int value;
} ASTLiteralInt;

typedef struct {
    ASTNode base;
    Token *operator;
    ASTNode *left;
    ASTNode *right;
} ASTBinaryExpr;


ASTBinaryExpr *allocBinaryExpr (ASTNode *left, Token *operator, ASTNode *right, int line);
ASTLiteralInt *allocLiteralInt(int value, int line);


#endif
