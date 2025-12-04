#ifndef MIPS_CODEGEN_H
#define MIPS_CODEGEN_H

#include "tac.h"
#include <stdio.h>

/* Registers used for MIPS code generation */
#define T0 "$t0"
#define T1 "$t1"
#define T2 "$t2"
#define FP "$fp"

/**
 * Emits MIPS assembly header and prologue.
 * Sets up data segment, text segment, and stack frame.
 * 
 * @param f Output file stream for assembly code
 */
void emit_mips_header(FILE *f);

/**
 * Generates MIPS code to load an operand into a register.
 * Handles variables (from stack), literals, and temporaries.
 * 
 * @param f Output file stream
 * @param name Name of operand (variable, temp, or literal)
 * @param reg Destination register
 */
void get_operand_mips(FILE *f, char *name, char *reg);

/**
 * Generates MIPS code to store a register value to a variable.
 * 
 * @param f Output file stream
 * @param name Variable name to store to
 * @param reg Source register containing value
 */
void store_variable_mips(FILE *f, char *name, char *reg);

/**
 * Main MIPS code generation function.
 * Converts TAC instructions to MIPS assembly.
 * 
 * @param head Head of TAC instruction list
 * @param f Output file stream for assembly code
 */
void generate_mips(TAC *head, FILE *f);

#endif /* MIPS_CODEGEN_H */