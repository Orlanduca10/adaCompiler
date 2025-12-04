#ifndef AST_H
#define AST_H

typedef enum {
    NODE_PROGRAM, NODE_BLOCK, NODE_ASSIGN, NODE_IF, NODE_WHILE,
    NODE_BINOP, NODE_UNARYOP, NODE_LITERAL, NODE_VAR, NODE_CALL
} NodeType;

typedef struct AST {
    NodeType type;
    char *value;
    struct AST *left, *right;
    struct AST *cond, *then_branch, *else_branch, *body;
    struct AST **children;
    int child_count;
} AST;

AST *new_ast(NodeType type, const char *value);
AST *new_ast_binary(NodeType type, const char *value, AST *l, AST *r);
void print_ast(AST *node, int depth);

#endif