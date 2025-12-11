%{
#include "ast.h"
#include "mini_ada.tab.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
%}

%option yylineno

digit     [0-9]
id        [A-Za-z_][A-Za-z0-9_]*
string    \"[^\"]*\"

%%
"procedure"        return PROCEDURE;
"is"               return IS;
"begin"            return BEGIN_;
"end"              return END;
"Main"             return MAIN;
"if"               return IF;
"then"             return THEN;
"else"             return ELSE;
"while"            return WHILE;
"loop"             return LOOP;
"Put_Line"         return PUT_LINE;
"Get_Line"         return GET_LINE;
"and"              return AND;
"or"               return OR;
"not"              return NOT;

":="               return ASSIGN;
"<="               { yylval.str = strdup(yytext); return COMPARE; }
">="               { yylval.str = strdup(yytext); return COMPARE; }
"/="               { yylval.str = strdup(yytext); return COMPARE; }
"<"                { yylval.str = strdup(yytext); return COMPARE; }
">"                { yylval.str = strdup(yytext); return COMPARE; }
"="                { yylval.str = strdup(yytext); return COMPARE; }
"+"                { yylval.str = strdup(yytext); return OP; }
"-"                { yylval.str = strdup(yytext); return OP; }
"*"                { yylval.str = strdup(yytext); return OP; }
"/"                { yylval.str = strdup(yytext); return OP; }
"("                return LPAREN;
")"                return RPAREN;
";"                return SEMICOL;

{digit}+           { yylval.str = strdup(yytext); return NUMBER; }
{string}           { yylval.str = strdup(yytext); return STRING; }
{id}               { yylval.str = strdup(yytext); return ID; }

[ \t\r\n]+         /* Ignora whitespace */

.                  { 
                     fprintf(stderr, "Lexical error at line %d: invalid character '%s'\n", 
                             yylineno, yytext); 
                     exit(1);
                   }
%%

int yywrap() { return 1; }