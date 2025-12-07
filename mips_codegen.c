#include "tac.h"
#include "symbol_table.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Registers used for temporary values
#define T0 "$t0"
#define T1 "$t1"
#define T2 "$t2"
// Register used for boolean comparison results
#define TEMP_RESULT "$t9"

// Helper function definitions
extern StringLiteral *string_list_head;
extern Symbol *lookup_symbol(char *name);

void emit_mips_header(FILE *f)
{
    // Boilerplate setup for MIPS
    fprintf(f, ".data\n");
    // Generate data for all collected string literals
    StringLiteral *curr_str = string_list_head;
    fprintf(f, "_str_newline: .asciiz \"\\n\"\n"); // Universal newline string

    while (curr_str)
    {
        // DO NOT strip quotes from string value before outputting to .data
        // In MIPS assembly, .asciiz expects quotes around the string
        // e.g., "Ola" should remain as "Ola" in the .data section
        fprintf(f, "%s: .asciiz %s\n", curr_str->label, curr_str->value);
        curr_str = curr_str->next;
    }

    fprintf(f, ".text\n");
    fprintf(f, ".globl main\n");
    fprintf(f, "main:\n");

    // Setup stack frame (adjusting the stack for local variables)
    extern int current_offset;

    // Reserve space for variables on the stack (addi $sp, $sp, -[offset])
    // The offset must be positive.
    fprintf(f, "  addi $sp, $sp, -%d\n", current_offset);
}

// Loads the value of 'name' into 'reg'.
// 'name' can be a variable (X), a literal (10), or a temp (t0).
void get_operand_mips(FILE *f, char *name, char *reg)
{
    Symbol *s = lookup_symbol(name);

    if (s)
    {
        // 1. Variable (X, Y): Load from stack offset (-offset($sp))
        fprintf(f, "  lw %s, -%d($sp)\n", reg, s->offset);
    }
    else if (name[0] >= '0' && name[0] <= '9')
    {
        // 2. Literal Number (10): Load immediate
        fprintf(f, "  li %s, %s\n", reg, name);
    }
    // 3. Temporary (t0, t1...): Since temps are variables stored on the stack
    //    for simplicity, treat them like a variable here, loading from their offset.
    //    If a temp is used as an argument in TAC, we assume it was stored to
    //    its own offset in the previous instruction.
    else if (name[0] == 't')
    {
        // This is a simplification. For full temp management, you'd track register contents.
        // Here we rely on the Symbol Table structure to also hold Temp variables (if you added them).
        // Assuming you added temps to Symbol Table:
        s = lookup_symbol(name); // Check if the temp register (t0, t1) has been stored on stack
        if (s)
        {
            fprintf(f, "  lw %s, -%d($sp)\n", reg, s->offset);
        }
        else
        {
            // If temps are NOT in the Symbol Table, we must assume the previous
            // instruction's result is in a known register (e.g., $t2) but
            // since that's unreliable, keeping them in the Symbol Table is safer.
            // For safety, assume temps are on the stack.
            fprintf(f, "  # WARNING: Temp %s not found in symbol table, treating as constant 0\n", name);
            fprintf(f, "  li %s, 0\n", reg);
        }
    }
}

// Stores the value in 'reg' into the location of 'name' (must be a variable or temp in ST)
void store_variable_mips(FILE *f, char *name, char *reg)
{
    Symbol *s = lookup_symbol(name);
    if (s)
    {
        // sw $t0, -offset($sp)
        fprintf(f, "  sw %s, -%d($sp)\n", reg, s->offset);
    }
    else
    {
        // This is a crucial error check: trying to store into something not defined.
        fprintf(f, "  # ERROR: Attempt to store into undeclared variable/temp: %s\n", name);
    }
}

void generate_mips(TAC *head, FILE *f)
{
    emit_mips_header(f);
    TAC *curr = head;

    while (curr)
    {
        // REMOVE the extra fprintf - only print once in the switch
        // fprintf(f, "  # TAC: ");  // Move this inside each case

        switch (curr->op)
        {
        case TAC_COPY:
            // TAC: X = 10 or X = t0
            fprintf(f, "  # %s = %s\n", curr->result, curr->arg1);
            get_operand_mips(f, curr->arg1, T0);      // Load RHS (10 or t0) into $t0
            store_variable_mips(f, curr->result, T0); // Store $t0 into LHS (X)
            break;

        case TAC_ADD:
        case TAC_SUB:
            // TAC: t2 = X + 10 or t2 = X - 1
            fprintf(f, "  # %s = %s %c %s\n", curr->result, curr->arg1,
                    (curr->op == TAC_ADD ? '+' : '-'), curr->arg2);

            get_operand_mips(f, curr->arg1, T0); // Get Arg1 into $t0
            get_operand_mips(f, curr->arg2, T1); // Get Arg2 into $t1

            // Calculate result - use $t2 (or TEMP_RESULT if defined)
            if (curr->op == TAC_ADD)
            {
                fprintf(f, "  add $t2, %s, %s\n", T0, T1);
            }
            else
            {
                fprintf(f, "  sub $t2, %s, %s\n", T0, T1);
            }

            // Store result (now in $t2) into TAC result (e.g., t2)
            store_variable_mips(f, curr->result, "$t2");
            break;

        case TAC_LABEL:
            // TAC: L0:
            fprintf(f, "%s:\n", curr->result); // ONLY ONCE - remove duplicate
            break;

        case TAC_JUMP:
            // TAC: goto L0
            fprintf(f, "  # goto %s\n", curr->result);
            fprintf(f, "  j %s\n", curr->result); // ONLY ONCE - remove first fprintf
            break;

        case TAC_GT:
            // TAC: t0 = X > 0 (Comparison result is stored in TAC result)
            fprintf(f, "  # %s = %s > %s\n", curr->result, curr->arg1, curr->arg2);

            get_operand_mips(f, curr->arg1, T0); // Get Arg1 into $t0
            get_operand_mips(f, curr->arg2, T1); // Get Arg2 into $t1

            // MIPS instruction for X > Y: slt $t2, $t1, $t0 ($t2 = (Y < X) which is (X > Y))
            fprintf(f, "  slt $t2, %s, %s\n", T1, T0); // $t2 = (Arg2 < Arg1) which is (Arg1 > Arg2)

            // Store the boolean result (now in $t2) into the TAC result temp (e.g., t0)
            store_variable_mips(f, curr->result, "$t2");
            break;

        case TAC_LT:
            // TAC: t0 = X < 10
            fprintf(f, "  # %s = %s < %s\n", curr->result, curr->arg1, curr->arg2);

            get_operand_mips(f, curr->arg1, T0); // Get Arg1 into $t0
            get_operand_mips(f, curr->arg2, T1); // Get Arg2 into $t1

            // MIPS instruction for X < Y: slt $t2, $t0, $t1 ($t2 = (X < Y))
            fprintf(f, "  slt $t2, %s, %s\n", T0, T1);

            // Store the boolean result (now in $t2) into the TAC result temp
            store_variable_mips(f, curr->result, "$t2");
            break;

        case TAC_JFALSE:
            // TAC: ifFalse t0 goto L1
            fprintf(f, "  # ifFalse %s goto %s\n", curr->arg1, curr->result);

            // Load the boolean result (0 or 1) of the condition into $t0
            get_operand_mips(f, curr->arg1, T0);

            // Branch if equal to zero (i.e., if condition is False)
            fprintf(f, "  beqz %s, %s\n", T0, curr->result);
            break;

        case TAC_PRINT:
            // TAC: PRINT "Ola" or PRINT X
            fprintf(f, "  # PRINT %s\n", curr->arg1);

            // If the argument starts with '"', it's a string literal
            if (curr->arg1[0] == '"')
            {
                // 1. Get the string label (S0, S1, etc.)
                char *label = get_string_label(curr->arg1);

                // 2. MIPS code to print string (syscall 4)
                fprintf(f, "  li $v0, 4\n");
                fprintf(f, "  la $a0, %s\n", label); // Load address of string label
                fprintf(f, "  syscall\n");
            }
            else
            {
                // MIPS code to print integer (syscall 1)
                get_operand_mips(f, curr->arg1, "$a0"); // Load value into argument register $a0
                fprintf(f, "  li $v0, 1\n");            // syscall 1 for integer print
                fprintf(f, "  syscall\n");
            }

            // Print Newline (for Put_Line semantics)
            fprintf(f, "  li $v0, 4\n");
            fprintf(f, "  la $a0, _str_newline\n"); // Load address of newline string
            fprintf(f, "  syscall\n");
            break;
        case TAC_EQ:
            // TAC: t0 = X == Y
            fprintf(f, "  # %s = %s == %s\n", curr->result, curr->arg1, curr->arg2);
            get_operand_mips(f, curr->arg1, T0);
            get_operand_mips(f, curr->arg2, T1);
            // MIPS 'seq' (Set Equal) sets dest to 1 if equal, 0 otherwise
            fprintf(f, "  seq $t2, %s, %s\n", T0, T1);
            store_variable_mips(f, curr->result, "$t2");
            break;
        case TAC_READ:
            // TAC: READ X
            fprintf(f, "  # READ %s\n", curr->result);

            // MIPS Syscall 5: Read Integer
            // The system waits for input, reads an int, and puts it in $v0
            fprintf(f, "  li $v0, 5\n");
            fprintf(f, "  syscall\n");

            // Store the read value (from $v0) into the variable (X)
            store_variable_mips(f, curr->result, "$v0");
            break;

        default:
            fprintf(f, "  # Unknown TAC operation: %d\n", curr->op);
            break;
        }
        curr = curr->next;
    }

    // MIPS Footer (Restore stack and exit)
    extern int current_offset;
    fprintf(f, "  # Restore stack and exit program\n");
    fprintf(f, "  addi $sp, $sp, %d\n", current_offset); // Deallocate stack space
    fprintf(f, "  li $v0, 10\n");                        // syscall 10 for exit
    fprintf(f, "  syscall\n");
}
