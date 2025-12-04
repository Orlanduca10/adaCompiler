#include "ast.h"
#include <stdio.h>
#include "mini_ada.tab.h" 
#include "symbol_table.h"
#include "semantics.h"
#include "tac.h"
#include "codegen.h"
#include "mips_codegen.h"

extern int yyparse(void);
extern AST *root;
extern TAC *tac_head;



int main(int argc, char **argv) {

    if (argc > 1) {
        extern FILE *yyin;
        yyin = fopen(argv[1], "r");
        if (!yyin) { perror(argv[1]); return 1; }
    }
    
    if (yyparse() != 0) {
        fprintf(stderr, "Parse error\n");
        return 1;
    }
    if (!root) {
        fprintf(stderr, "No AST produced\n");
        return 1;
    }

    print_ast(root, 0);

    check_semantics(root);
        
    print_symbol_table();


    generate_code(root);
    print_tac(tac_head); // Debugging: Print generated TAC

    const char *outname = "out.s";
    FILE *f = fopen(outname, "w");
    if (!f) { perror("fopen"); return 1; }
    generate_mips(tac_head, f);
    fclose(f);

    return 0;
}
