#include "tac.h"
#include "ast.h"
#include "symtab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// --------------------- AST TO TAC ---------------------
static TAC *gen_expr(AST *n, char **result, Scope *global);
static TAC *gen_stmt(AST *n, Scope *global);

TAC *generate_tac(AST *root, Scope *global) {
    return gen_stmt(root, global);
}

// --------------------- Expression ---------------------
static TAC *gen_expr(AST *n, char **result, Scope *global) {
    if (!n) return NULL;

    if (n->type == NODE_LITERAL || n->type == NODE_VAR) {
        *result = strdup(n->value);

        if (n->type == NODE_VAR)
            scope_insert(global, n->value, TYPE_INT);  // add variable if needed

        return NULL;
    }

    if (n->type == NODE_BINOP) {
        char *l, *r;
        TAC *tl = gen_expr(n->left, &l, global);
        TAC *tr = gen_expr(n->right, &r, global);

        char *t = new_temp();
        TAC *op = tac_new(TAC_BINOP, t, l, n->value);
        op->next = tac_new(TAC_ASSIGN, NULL, r, NULL);
        *result = t;

        return tac_join(tac_join(tl, tr), op);
    }

    if (n->type == NODE_UNARYOP) {
        char *x;
        TAC *tx = gen_expr(n->left, &x, global);
        char *t = new_temp();
        *result = t;
        return tac_join(tx, tac_new(TAC_UNARY, t, n->value, x));
    }

    return NULL;
}


// --------------------- Statements ---------------------
static TAC *gen_stmt(AST *n, Scope *global) {
    if (!n) return NULL;

    switch(n->type) {
        case NODE_ASSIGN: {
            char *rhs;
            TAC *tr = gen_expr(n->right, &rhs, global);

            // insert variable into symbol table
            scope_insert(global, n->value, TYPE_INT);

            TAC *ta = tac_new(TAC_ASSIGN, n->value, rhs, NULL);
            return tac_join(tr, ta);
        }

        case NODE_BLOCK: {
            TAC *res = NULL;
            for (int i = 0; i < n->child_count; i++)
                res = tac_join(res, gen_stmt(n->children[i], global));
            return res;
        }

        case NODE_WHILE: {
            char *cond;
            TAC *tc = gen_expr(n->cond, &cond, global);

            char *Lstart = new_label();
            char *Lend = new_label();

            TAC *loop = tac_join(
                tac_new(TAC_LABEL, Lstart, NULL, NULL),
                tac_join(
                    tc,
                    tac_join(
                        tac_new(TAC_IFZ, cond, Lend, NULL),
                        tac_join(
                            gen_stmt(n->body, global),
                            tac_join(
                                tac_new(TAC_GOTO, Lstart, NULL, NULL),
                                tac_new(TAC_LABEL, Lend, NULL, NULL)
                            )
                        )
                    )
                )
            );
            return loop;
        }

        case NODE_CALL: {
            char *arg = NULL;
            if (n->left) gen_expr(n->left, &arg, global); // for Put_Line(expr)
            return tac_new(TAC_CALL, NULL, n->value, arg);
        }

        default:
            return NULL;
    }
}
