
#include "headers/ast.h"
#include "headers/utils.h"

void initBinary(ASTNode *node, void *data) {
    ASTBinaryExpr *init = (ASTBinaryExpr *)data;
    node->as.binary.left = init->left;
    node->as.binary.right = init->right;
    node->as.binary.operator = init->operator;
}

void initLiteral(ASTNode *node, void *data) {
    ASTLiteralInt *init = (ASTLiteralInt *)data;
    node->as.literalInteger.value = init->value;
}

void initIdentifier(ASTNode *node, void *data) {
    ASTIdentifier *init = (ASTIdentifier *)data;
    node->as.identifier.name = init->name; // optional strdup depending on tokenizer
}

void initGrouping(ASTNode *node, void *data) {
    ASTGrouping *init = (ASTGrouping *)data;
    node->as.grouping.expression = init->expression;
}

void initFloat(ASTNode *node, void *data) {
    ASTLiteralFloat *init = (ASTLiteralFloat *)data;
    node->as.literalFloat.value = init->value;
}

void initUnary(ASTNode *node, void *data) {
    ASTUnaryExpr *init = (ASTUnaryExpr *)data;
    node->as.unary.operator = init->operator;
    node->as.unary.right = init->right;
}

static InitFn nodeInitializers[] = {
    [NODE_BINARY_EXPR] = initBinary,
    [NODE_LITERAL_INT] = initLiteral,
    [NODE_IDENTIFIER] = initIdentifier,
    [NODE_LITERAL_FLOAT] = initFloat,
    [NODE_GROUPING] = initGrouping,
    [NODE_UNARY_EXPR] = initUnary,
};

ASTNode *allocASTNode(NodeType type, int line, void *data) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    allocError(node, "allocASTNode");

    node->type = type;
    node->line = line;

    if (nodeInitializers[type]) {
        nodeInitializers[type](node, data);
    }

    return node;
}
