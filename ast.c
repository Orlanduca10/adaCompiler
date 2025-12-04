#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

AST *new_ast(NodeType type, const char *value) {
    AST *node = malloc(sizeof(AST));
    node->type = type;
    node->value = value ? strdup(value) : NULL;
    node->left = node->right = NULL;
    node->cond = node->then_branch = node->else_branch = node->body = NULL;
    node->children = NULL;
    node->child_count = 0;
    return node;
}

AST *new_ast_binary(NodeType type, const char *value, AST *l, AST *r) {
    AST *node = new_ast(type, value);
    node->left = l;
    node->right = r;
    return node;
}

void print_ast(AST *node, int depth) {
    if (!node) return;
    
    for (int i = 0; i < depth; i++) printf("  ");
    
    switch (node->type) {
        case NODE_PROGRAM: printf("PROGRAM"); break;
        case NODE_BLOCK: printf("BLOCK"); break;
        case NODE_ASSIGN: printf("ASSIGN"); break;
        case NODE_IF: printf("IF"); break;
        case NODE_WHILE: printf("WHILE"); break;
        case NODE_BINOP: printf("BINOP"); break;
        case NODE_UNARYOP: printf("UNARYOP"); break;
        case NODE_LITERAL: printf("LITERAL"); break;
        case NODE_VAR: printf("VAR"); break;
        case NODE_CALL: printf("CALL"); break;
    }
    
    if (node->value) printf(" [%s]", node->value);
    printf("\n");
    
    print_ast(node->left, depth + 1);
    print_ast(node->right, depth + 1);
    print_ast(node->cond, depth + 1);
    print_ast(node->then_branch, depth + 1);
    print_ast(node->else_branch, depth + 1);
    print_ast(node->body, depth + 1);
    
    if (node->children) {
        for (int i = 0; i < node->child_count; i++) {
            print_ast(node->children[i], depth + 1);
        }
    }
}