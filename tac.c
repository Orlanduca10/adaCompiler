#include "tac.h"
#include "ast.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int temp_id = 0;
static int label_id = 0;

char *new_temp() {
    char buf[32];
    sprintf(buf, "t%d", temp_id++);
    return strdup(buf);
}

char *new_label() {
    char buf[32];
    sprintf(buf, "L%d", label_id++);
    return strdup(buf);
}

TAC *tac_new(TACKind k, char *a, char *b, char *c){
    TAC *t = malloc(sizeof(TAC));
    t->kind = k;
    t->a = a ? strdup(a) : NULL;
    t->b = b ? strdup(b) : NULL;
    t->c = c ? strdup(c) : NULL;
    t->next = NULL;
    return t;
}

TAC *tac_join(TAC *a, TAC *b) {
    if (!a) return b;
    TAC *t = a;
    while (t->next) t = t->next;
    t->next = b;
    return a;
}

void tac_print(TAC *t) {
    while (t) {
        switch (t->kind) {
            case TAC_ASSIGN:
                if (t->a && t->b)
                    printf("%s = %s\n", t->a, t->b);
                /* if a is NULL this assign may be part of a BINOP layout (handled below) */
                break;

            case TAC_BINOP:
                /* Generator emits BINOP followed by an ASSIGN holding the right operand.
                   Print using both nodes if available, and skip the following assign. */
                if (t->a && t->b && t->c) {
                    const char *right = NULL;
                    if (t->next && t->next->kind == TAC_ASSIGN && t->next->b)
                        right = t->next->b;
                    if (right)
                        printf("%s = %s %s %s\n", t->a, t->b, t->c, right);
                    else
                        printf("%s = %s %s ?\n", t->a, t->b, t->c);
                    /* skip the following assign node if it was used */
                    if (t->next && t->next->kind == TAC_ASSIGN)
                        t = t->next;
                }
                break;

            case TAC_LITERAL:
                /* nothing to print for standalone literal nodes */
                break;

            case TAC_UNARY:
                if (t->a && t->b && t->c)
                    printf("%s = %s %s\n", t->a, t->b, t->c);
                break;

            case TAC_LABEL:
                if (t->a) printf("%s:\n", t->a);
                break;

            case TAC_GOTO:
                if (t->a) printf("goto %s\n", t->a);
                break;

            case TAC_IFZ:
                if (t->a && t->b) printf("ifz %s goto %s\n", t->a, t->b);
                break;

            case TAC_CALL:
                /* support both forms: dest = call name(arg)  OR  call name(arg) */
                if (t->a && t->b)
                    printf("%s = call %s(%s)\n", t->a, t->b, t->c ? t->c : "");
                else if (t->b)
                    printf("call %s(%s)\n", t->b, t->c ? t->c : "");
                break;

            case TAC_PARAM:
                if (t->a) printf("param %s\n", t->a);
                break;

            case TAC_RETURN:
                if (t->a) printf("return %s\n", t->a);
                break;
        }
        t = t->next;
    }
}
