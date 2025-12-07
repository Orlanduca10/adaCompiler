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
        return node->value;
    }

    if (node->type == NODE_BINOP) {
        char *t1 = generate_expr(node->left);
        char *t2 = generate_expr(node->right);
        char *result = new_temp_var();
        
        TACOp op;
        if (strcmp(node->value, "+") == 0) op = TAC_ADD;
        else if (strcmp(node->value, "-") == 0) op = TAC_SUB;
        else if (strcmp(node->value, "*") == 0) op = TAC_MUL;
        else if (strcmp(node->value, "/") == 0) op = TAC_DIV; // Added DIV
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
        char *rhs = generate_expr(node->right);
        append_tac(new_tac(TAC_COPY, rhs, NULL, node->value));
    }
    else if (node->type == NODE_CALL) {
         if (strcmp(node->value, "Put_Line") == 0) {
             AST *arg_node = node->left;
             char *val = generate_expr(arg_node);
             if (arg_node && arg_node->type == NODE_LITERAL && arg_node->value[0] == '"') {
                 get_string_label(arg_node->value); 
             }
             append_tac(new_tac(TAC_PRINT, val, NULL, NULL));
         }
         // --- NEW: Handle Get_Line ---
         else if (strcmp(node->value, "Get_Line") == 0) {
             // Expecting a variable as the argument (node->left)
             if (node->left && node->left->type == NODE_VAR) {
                 char *var_name = node->left->value;
                 
                 // Generate TAC: READ X
                 // We use the 'result' field to store the destination variable name
                 append_tac(new_tac(TAC_READ, NULL, NULL, var_name));
             }
         }
    }
    // --- NEW: IF-THEN-ELSE IMPLEMENTATION ---
    else if (node->type == NODE_IF) {
        char *L_false = make_label(); // Label for ELSE or END
        char *L_end = make_label();   // Label for END

        // 1. Calculate condition
        char *cond = generate_expr(node->cond);
        
        // 2. If condition is false (0), jump to L_false
        append_tac(new_tac(TAC_JFALSE, cond, NULL, L_false));

        // 3. Generate THEN block
        generate_stmt(node->then_branch);

        // 4. Jump to End (skip ELSE part)
        append_tac(new_tac(TAC_JUMP, NULL, NULL, L_end));

        // 5. Place L_false label here
        append_tac(new_tac(TAC_LABEL, NULL, NULL, L_false));

        // 6. Generate ELSE block (if it exists)
        if (node->else_branch) {
            generate_stmt(node->else_branch);
        }

        // 7. Place L_end label here
        append_tac(new_tac(TAC_LABEL, NULL, NULL, L_end));
    }
    // ----------------------------------------
    else if (node->type == NODE_WHILE) {
        char *L_start = make_label();
        char *L_end = make_label();

        append_tac(new_tac(TAC_LABEL, NULL, NULL, L_start));
        char *cond = generate_expr(node->cond);
        append_tac(new_tac(TAC_JFALSE, cond, NULL, L_end));
        generate_stmt(node->body);
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