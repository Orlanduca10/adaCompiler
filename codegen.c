#include "tac.h"
#include "ast.h"
#include "symbol_table.h" // Needed for insert_symbol
#include <stdlib.h>
#include <string.h>

// Global list head and tail to append instructions easily
extern TAC *tac_head;
extern TAC *tac_tail;


// --- Symbol Management Helpers ---

// Helper function to create a temporary and insert it into the Symbol Table
char* new_temp_var() {
    char *temp_name = make_temp();
    // CRITICAL FIX: Insert the new temporary variable into the Symbol Table
    add_symbol(temp_name, TYPE_INT); 
    return temp_name;
}

void append_tac(TAC *instr) {
    if (!tac_head) {
        tac_head = instr;
        tac_tail = instr;
    } else {
        tac_tail->next = instr;
        tac_tail = instr;
    }
}

// Returns the name of the variable (or temp) storing the result
char* generate_expr(AST *node) {
    if (!node) return NULL;

    if (node->type == NODE_LITERAL || node->type == NODE_VAR) {
        // It's already a value (e.g., "10" or "X"), just return the name
        return node->value;
    }

    if (node->type == NODE_BINOP) {
        char *t1 = generate_expr(node->left);
        char *t2 = generate_expr(node->right);
        char *result = new_temp_var(); // *** USING FIXED HELPER ***
        
        TACOp op;
        if (strcmp(node->value, "+") == 0) op = TAC_ADD;
        else if (strcmp(node->value, "-") == 0) op = TAC_SUB;
        else if (strcmp(node->value, "*") == 0) op = TAC_MUL;
        else if (strcmp(node->value, ">") == 0) op = TAC_GT;
        else if (strcmp(node->value, "<") == 0) op = TAC_LT;
        else if (strcmp(node->value, "=") == 0) op = TAC_EQ;
        else if (strcmp(node->value, "/=") == 0) op = TAC_NEQ;
        else op = TAC_ADD; 

        append_tac(new_tac(op, t1, t2, result));
        return result;
    }
    return NULL;
}

void generate_stmt(AST *node) {
    if (!node) return;

    if (node->type == NODE_BLOCK) {
        if (node->children) {
            for (int i = 0; i < node->child_count; i++) {
                generate_stmt(node->children[i]);
            }
        }
    } 
    else if (node->type == NODE_ASSIGN) {
        // X := expr
        char *rhs = generate_expr(node->right); // Evaluate right side
        append_tac(new_tac(TAC_COPY, rhs, NULL, node->value));
    }
    else if (node->type == NODE_CALL) {
         if (strcmp(node->value, "Put_Line") == 0) {
             AST *arg_node = node->left;
             char *val = generate_expr(arg_node);
             
             // CRITICAL FIX: Ensure string literals are registered for MIPS data section
             if (arg_node && arg_node->type == NODE_LITERAL && arg_node->value[0] == '"') {
                 // Calling this function registers the string and assigns it an S-label
                 get_string_label(arg_node->value); 
             }
             
             append_tac(new_tac(TAC_PRINT, val, NULL, NULL));
         }
    }
    else if (node->type == NODE_WHILE) {
        char *L_start = make_label();
        char *L_end = make_label();

        append_tac(new_tac(TAC_LABEL, NULL, NULL, L_start));
        
        // Condition
        char *cond = generate_expr(node->cond);
        append_tac(new_tac(TAC_JFALSE, cond, NULL, L_end));

        // Body
        generate_stmt(node->body);

        // Jump back
        append_tac(new_tac(TAC_JUMP, NULL, NULL, L_start));
        append_tac(new_tac(TAC_LABEL, NULL, NULL, L_end));
    }
}

void generate_code(AST *root) {
    // Assuming root is PROGRAM
    if (root->body) {
         generate_stmt(root->body); 
    }
}