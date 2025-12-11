#ifndef CODEGEN_H
#define CODEGEN_H

#include "tac.h"
#include "ast.h"

/* Global TAC list pointers */
extern TAC *tac_head;
extern TAC *tac_tail;

/**
 * Creates a new temporary variable and inserts it into the symbol table.
 * 
 * @return Name of the newly created temporary variable
 */
char* new_temp_var(void);

/**
 * Appends a TAC instruction to the global instruction list.
 * 
 * @param instr The TAC instruction to append
 */
void append_tac(TAC *instr);

/**
 * Generates TAC instructions for an expression AST node.
 * Returns the name of the temporary variable holding the result.
 * 
 * @param node The AST expression node
 * @return Name of temporary variable or value containing the result
 */
char* generate_expr(AST *node);

/**
 * Generates TAC instructions for a statement AST node.
 * 
 * @param node The AST statement node
 */
void generate_stmt(AST *node);

/**
 * Main code generation entry point.
 * Generates TAC instructions for the entire program.
 * 
 * @param root The root AST node (PROGRAM)
 */
void generate_code(AST *root);

#endif /* CODEGEN_H */