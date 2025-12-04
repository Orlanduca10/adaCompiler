#include "ast.h"
#include <stdio.h>
#include "mini_ada.tab.h" 
#include "symtab.h"
#include "tac.h"

extern int yyparse(void);
extern AST *root;

// forward generators
TAC *generate_tac(AST *root, Scope *global);
void mips_gen(FILE *out, TAC *tac, Scope *global);


// Traverse AST and insert all variables into the symbol table
void collect_vars(AST *node, Scope *sc) {
    if (!node) return;

    switch(node->type) {
        case NODE_VAR:
            scope_insert(sc, node->value, TYPE_INT);  // assume all ints
            break;
        case NODE_ASSIGN:
            scope_insert(sc, node->value, TYPE_INT);
            collect_vars(node->right, sc);
            break;
        case NODE_BINOP:
        case NODE_UNARYOP:
            collect_vars(node->left, sc);
            collect_vars(node->right, sc);
            break;
        case NODE_IF:
            collect_vars(node->cond, sc);
            collect_vars(node->then_branch, sc);
            collect_vars(node->else_branch, sc);
            break;
        case NODE_WHILE:
            collect_vars(node->cond, sc);
            collect_vars(node->body, sc);
            break;
        case NODE_BLOCK:
            for (int i = 0; i < node->child_count; i++)
                collect_vars(node->children[i], sc);
            break;
        case NODE_CALL:
            collect_vars(node->left, sc);  // for Put_Line(expr)
            break;
        default:
            break;
    }
}

int main(int argc, char **argv) {
    if (yyparse() != 0) {
        fprintf(stderr, "Parse error\n");
        return 1;
    }
    if (!root) {
        fprintf(stderr, "No AST produced\n");
        return 1;
    }

    print_ast(root, 0);

    // create global scope
    Scope *global = scope_new(NULL, 120);

    // generate TAC
    TAC *ir = generate_tac(root, global);

    // print TAC to stdout
    printf("=== TAC ===\n");
    tac_print(ir);

    // emit MIPS
    const char *outname = "out.s";
    FILE *f = fopen(outname, "w");
    if (!f) { perror("fopen"); return 1; }
    mips_gen(f, ir, global);
    fclose(f);
    printf("MIPS written to %s\n", outname);

    print_scope(global);
    scope_free(global);
    return 0;
}
