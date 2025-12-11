%{
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

AST *root;
int yylex(void);
void yyerror(const char *s) { fprintf(stderr, "Error: %s\n", s); }
%}

%union {
    char *str;
    AST *node;
}

%token <str> ID NUMBER STRING
%token <str> OP COMPARE
%token PROCEDURE IS BEGIN_ END MAIN
%token IF THEN ELSE WHILE LOOP
%token PUT_LINE GET_LINE
%token AND OR NOT
%token ASSIGN LPAREN RPAREN SEMICOL

// Precedência (do mais baixo ao mais alto)
%left OR
%left AND
%right NOT
%left COMPARE
%left '+' '-'
%left '*' '/'

%type <node> program block statement statement_list 
%type <node> expr logical_expr comparison_expr arith_expr term factor

%%

program
    : PROCEDURE MAIN IS BEGIN_ block END MAIN SEMICOL
      {
          root = new_ast(NODE_PROGRAM, "Main");
          root->body = $5;
      }
    ;

block
    : statement_list
      {
          $$ = new_ast(NODE_BLOCK, NULL);
          $$->children = $1->children;
          $$->child_count = $1->child_count;
          free($1);
      }
    ;

statement_list
    : statement
      {
          $$ = new_ast(NODE_BLOCK, NULL);
          $$->children = malloc(sizeof(AST*));
          $$->children[0] = $1;
          $$->child_count = 1;
      }
    | statement_list statement
      {
          $$ = $1;
          $$->child_count++;
          $$->children = realloc($$->children, sizeof(AST*) * $$->child_count);
          $$->children[$$->child_count - 1] = $2;
      }
    ;

statement
    : ID ASSIGN expr SEMICOL
      {
          $$ = new_ast(NODE_ASSIGN, $1);
          $$->right = $3;
      }
    | IF expr THEN block ELSE block END IF SEMICOL
      {
          $$ = new_ast(NODE_IF, NULL);
          $$->cond = $2;
          $$->then_branch = $4;
          $$->else_branch = $6;
      }
    | WHILE expr LOOP block END LOOP SEMICOL
      {
          $$ = new_ast(NODE_WHILE, NULL);
          $$->cond = $2;
          $$->body = $4;
      }
    | PUT_LINE LPAREN expr RPAREN SEMICOL
      {
          $$ = new_ast(NODE_CALL, "Put_Line");
          $$->left = $3;
      }
    | GET_LINE LPAREN ID RPAREN SEMICOL
      {
          $$ = new_ast(NODE_CALL, "Get_Line");
          $$->left = new_ast(NODE_VAR, $3);
      }
    ;

// Hierarquia de expressões
expr
    : logical_expr
      { $$ = $1; }
    ;

logical_expr
    : comparison_expr
      { $$ = $1; }
    | logical_expr AND comparison_expr
      { $$ = new_ast_binary(NODE_BINOP, "and", $1, $3); }
    | logical_expr OR comparison_expr
      { $$ = new_ast_binary(NODE_BINOP, "or", $1, $3); }
    ;

comparison_expr
    : arith_expr
      { $$ = $1; }
    | arith_expr COMPARE arith_expr
      { $$ = new_ast_binary(NODE_BINOP, $2, $1, $3); }
    ;

arith_expr
    : term
      { $$ = $1; }
    | arith_expr OP term
      { $$ = new_ast_binary(NODE_BINOP, $2, $1, $3); }
    ;

term
    : factor
      { $$ = $1; }
    ;

factor
    : NUMBER
      { $$ = new_ast(NODE_LITERAL, $1); }
    | STRING
      { $$ = new_ast(NODE_LITERAL, $1); }
    | ID
      { $$ = new_ast(NODE_VAR, $1); }
    | LPAREN expr RPAREN
      { $$ = $2; }
    | NOT factor
      {
          $$ = new_ast(NODE_UNARYOP, "not");
          $$->left = $2;
      }
    ;

%%