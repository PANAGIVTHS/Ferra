#include "ast.h"
#include "token.h"

ASTLiteralInt *allocLiteralInt(int value, int line) {
    ASTLiteralInt *newLiteralInt = (ASTLiteralInt*) malloc(sizeof(ASTLiteralInt));
    allocError(newLiteralInt, "allocLiteralInt");

    newLiteralInt->base.type = NODE_LITERAL_INT;
    newLiteralInt->base.line = line;
    newLiteralInt->value = value;

    return newLiteralInt;
}

ASTBinaryExpr *allocBinaryExpr (ASTNode *left, Token *operator, ASTNode *right, int line) {
    ASTBinaryExpr *newASTBinary;

    newASTBinary = (ASTBinaryExpr*) malloc(sizeof(ASTBinaryExpr));
    allocError(newASTBinary, "ASTBinaryExpr");

    newASTBinary->base.type = NODE_BINARY_EXPR;
    newASTBinary->base.line = line;
    newASTBinary->left = left;
    newASTBinary->right = right;
    newASTBinary->operator = operator;
    
    return newASTBinary;
}
