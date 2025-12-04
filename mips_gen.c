#include "tac.h"
#include "symtab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int temp_to_reg(const char *t) {
    int id = atoi(t+1);
    return id % 10; // map to $t0-$t9
}

static void load_operand(FILE *out, const char *op, const char *reg, Scope *sc) {
    if (!op) return;
    if (op[0] == 't') {
        fprintf(out, "  move %s, $t%d\n", reg, temp_to_reg(op));
    } else if (scope_lookup(sc, op)) {
        Symbol *s = scope_lookup(sc, op);
        fprintf(out, "  lw %s, -%d($fp)\n", reg, s->offset);
    } else {
        fprintf(out, "  li %s, %s\n", reg, op);
    }
}

void mips_gen(FILE *out, TAC *t, Scope *global) {
    fprintf(out, ".text\nmain:\n  move $fp, $sp\n");

    for (; t; t=t->next) {
        switch(t->kind) {
            case TAC_ASSIGN:
                load_operand(out, t->b, "$t0", global);
                fprintf(out, "  move $t%d, $t0\n", temp_to_reg(t->a));
                break;
            case TAC_BINOP:
                load_operand(out, t->b, "$t0", global);
                load_operand(out, t->c, "$t1", global);
                fprintf(out, "  add $t2, $t0, $t1\n");
                fprintf(out, "  move $t%d, $t2\n", temp_to_reg(t->a));
                break;
            case TAC_LABEL:
                fprintf(out, "%s:\n", t->a);
                break;
            case TAC_GOTO:
                fprintf(out, "  j %s\n", t->a);
                break;
            case TAC_IFZ:
                load_operand(out, t->a, "$t0", global);
                fprintf(out, "  beq $t0, $zero, %s\n", t->b);
                break;
            case TAC_CALL:
                if (!strcmp(t->b, "Put_Line")) {
                    load_operand(out, t->c, "$a0", global);
                    fprintf(out, "  li $v0, 4\n  syscall\n");
                }
                break;
            default: break;
        }
    }

    fprintf(out, "  li $v0, 10\n  syscall\n");
}
