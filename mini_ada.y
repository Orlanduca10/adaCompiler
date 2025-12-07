%{
#include "ast.h" 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Root of the AST
AST *root;
// Prototype for the lexer (from Flex)
int yylex(void);
// Parser error callback
void yyerror(const char *s) { fprintf(stderr, "Error: %s\n", s); }
%}

%union {
    char *str;   // semantic type for string tokens (IDs, NUMBER text, STRING text, ops)
    AST *node;   // semantic type for AST nodes
}

%token <str> ID NUMBER STRING
%token <str> OP COMPARE
%token PROCEDURE IS BEGIN_ END MAIN
%token IF THEN ELSE WHILE LOOP
%token PUT_LINE GET_LINE
%token AND OR NOT
%token ASSIGN LPAREN RPAREN SEMICOL

// Nonterminals produce AST nodes
%type <node> program block statement statement_list expr term factor

%%

/* Top-level program rule: procedure main */
program
    : PROCEDURE MAIN IS BEGIN_ block END MAIN SEMICOL
      {
          // create program root and attach the parsed block
          root = new_ast(NODE_PROGRAM, "Main");
          root->body = $5;
      }
    ;

/* A block is represented by a statement_list */
block
    : statement_list
      {
          // wrap the statement_list into a NODE_BLOCK (steal its children)
          $$ = new_ast(NODE_BLOCK, NULL);
          $$->children = $1->children;
          $$->child_count = $1->child_count;
          free($1); // free the temporary list node
      }
    ;

/* Build up a list of statements (flat block) */
statement_list
    : statement
      {
          // first statement -> create block container with one child
          $$ = new_ast(NODE_BLOCK, NULL);
          $$->children = malloc(sizeof(AST*));
          $$->children[0] = $1;
          $$->child_count = 1;
      }
    | statement_list statement
      {
          // append statement to existing list's children array
          $$ = $1;
          $$->child_count++;
          $$->children = realloc($$->children, sizeof(AST*) * $$->child_count);
          $$->children[$$->child_count - 1] = $2;
      }
    ;

/* Different kinds of statements */
statement
    : ID ASSIGN expr SEMICOL
      {
          // assignment: ID := expr;
          $$ = new_ast(NODE_ASSIGN, $1);
          $$->right = $3;
      }
    | IF expr THEN block ELSE block END IF SEMICOL
      {
          // if-then-else statement
          $$ = new_ast(NODE_IF, NULL);
          $$->cond = $2;
          $$->then_branch = $4;
          $$->else_branch = $6;
      }
    | WHILE expr LOOP block END LOOP SEMICOL
      {
          // while loop
          $$ = new_ast(NODE_WHILE, NULL);
          $$->cond = $2;
          $$->body = $4;
      }
    | PUT_LINE LPAREN expr RPAREN SEMICOL
      {
          // procedure call: Put_Line(expr);
          $$ = new_ast(NODE_CALL, "Put_Line");
          $$->left = $3;
      }
    | GET_LINE LPAREN ID RPAREN SEMICOL
      {
          // procedure call: Get_Line(ID);
          $$ = new_ast(NODE_CALL, "Get_Line");
          // Store the variable name in a new NODE_VAR node attached to 'left'
          $$->left = new_ast(NODE_VAR, $3); 
      }
    ;

/* Expressions and operators */
expr
    : term
      { $$ = $1; } // single term
    | expr OP term
      { $$ = new_ast_binary(NODE_BINOP, $2, $1, $3); } // binary operator
    | expr COMPARE term
      { $$ = new_ast_binary(NODE_BINOP, $2, $1, $3); } // comparison
    | expr AND term
      { $$ = new_ast_binary(NODE_BINOP, "and", $1, $3); } // logical and
    | expr OR term
      { $$ = new_ast_binary(NODE_BINOP, "or", $1, $3); } // logical or
    ;

term
    : factor
      { $$ = $1; } // term -> factor (no precedence implemented)
    ;

/* Factors: literals, variables, parentheses, NOT */
factor
    : NUMBER
      { $$ = new_ast(NODE_LITERAL, $1); }  // numeric literal
    | STRING
      { $$ = new_ast(NODE_LITERAL, $1); }  // string literal
    | ID
      { $$ = new_ast(NODE_VAR, $1); }      // variable reference
    | LPAREN expr RPAREN
      { $$ = $2; }                         // parenthesized expression
    | NOT factor
      {
          // unary not
          $$ = new_ast(NODE_UNARYOP, "not");
          $$->left = $2;
      }
    ;

%%

/* Main entry: parse, then print AST if successful 
int main(void) {
    if (yyparse() == 0) {
        printf("Parsed successfully!\n");
        print_ast(root, 0);
    }
    return 0;
}
*/