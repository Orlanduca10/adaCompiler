#include "ast.h"
#include "symbol_table.h"
#include <stdio.h>

void check_semantics(AST *node) {
    if (!node) return;

    switch (node->type) {
        case NODE_ASSIGN:
            // logic: LHS is the variable name. 
            // In implicit declaration languages, if we see an assign 
            // to a variable that doesn't exist, we create it.
            if (node->value) {
                // Determine type based on the RHS (simplified for now)
                // If RHS is a string literal, type is STRING, else INT
                DataType inferred_type = TYPE_INT; 
                
                // Very basic inference:
                if (node->right && node->right->type == NODE_LITERAL) {
                    // Check if the literal starts with quote
                    if (node->right->value[0] == '"') 
                        inferred_type = TYPE_STRING;
                }

                add_symbol(node->value, inferred_type);
            }
            break;

        case NODE_VAR:
            // logic: If we use a variable (in an expression), 
            // it MUST exist in the symbol table already.
            if (lookup_symbol(node->value) == NULL) {
                fprintf(stderr, "Semantic Error: Variable '%s' used before assignment.\n", node->value);
                // exit(1); // Optional: stop compilation
            }
            break;
            
        default:
            break;
    }

    // Recursively check children
    check_semantics(node->left);
    check_semantics(node->right);
    check_semantics(node->cond);
    check_semantics(node->then_branch);
    check_semantics(node->else_branch);
    check_semantics(node->body);

    if (node->children) {
        for (int i = 0; i < node->child_count; i++) {
            check_semantics(node->children[i]);
        }
    }
}