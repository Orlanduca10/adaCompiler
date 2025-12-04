#ifndef TAC_H
#define TAC_H
#include "symtab.h"

typedef enum {
    TAC_ASSIGN, TAC_BINOP, TAC_UNARY, TAC_LABEL, TAC_GOTO,
    TAC_IFZ, TAC_CALL, TAC_PARAM, TAC_RETURN, TAC_LITERAL
} TACKind;

typedef struct TAC {
    TACKind kind;
    char *a, *b, *c;
    struct TAC *next;
} TAC;

TAC *tac_new(TACKind kind, char *a, char *b, char *c);
TAC *tac_join(TAC *t1, TAC *t2);
void tac_print(TAC *t);

char *new_temp();
char *new_label();

#endif
