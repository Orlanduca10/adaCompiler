🧠 Mini Ada Compiler
This project implements a Mini Ada compiler frontend, capable of parsing a simplified subset of the Ada programming language and generating an Abstract Syntax Tree (AST) representation of the program.

The project uses Flex for lexical analysis and Bison for parsing, written in C.

C Flex Bison

🚀 Features
Lexical and syntax analysis for a small subset of Ada
AST (Abstract Syntax Tree) construction
Pretty-printing of the AST
Basic support for:
procedure Main program structure
Variable assignment (x := 5;)
if ... then ... else ... end if; statements
while ... loop ... end loop; loops
Put_Line(...) and Get_Line; statements
Arithmetic and comparison operators (+, -, *, /, =, <, >, etc.)
Logical operators (and, or, not)
Literals (numbers, strings) and variable references
📁 Project Structure
mini_ada/
├── mini_ada.l # Lexical analyzer (Flex)
|
├── mini_ada.y # Parser (Bison)
|
├── ast.h # AST (Abstract Syntax Tree) structure and prototypes
|
└── ast.c # Implementation of AST creation and printing




README.md
⚙️ Build Instructions
Requirements
Make sure you have the following tools installed:

Flex (lexical analyzer generator)
Bison (parser generator)
gcc (C compiler)
Run this command (in Linux):

flex mini_ada.l bison -d mini_ada.y

gcc mini_ada.tab.c lex.yy.c ast.c -o mini_ada

./mini_ada < test.ada

For test.ada that looks like this:procedure Main is

begin
    X := 10;
    while X > 0 loop
        Put_Line("Countdown");
        X := X - 1;
        Y:=  X - 3;
        while X > 0 loop
            Put_Line("Countdown");
            X := 10;
        end loop;
    end loop;
end Main;
The programs prints this AST

PROGRAM [Main]
  BLOCK
    ASSIGN [X]
      LITERAL [10]
    WHILE
      BINOP [>]
        VAR [X]
        LITERAL [0]
      BLOCK
        CALL [Put_Line]
          LITERAL ["Countdown"]
        ASSIGN [X]
          BINOP [-]
            VAR [X]
            LITERAL [1]
        ASSIGN [Y]
          BINOP [-]
            VAR [X]
            LITERAL [3]
        WHILE
          BINOP [>]
            VAR [X]
            LITERAL [0]
          BLOCK
            CALL [Put_Line]
              LITERAL ["Countdown"]
            ASSIGN [X]
              LITERAL [10]
📚 Documentation
ast.h - Contains all AST node definitions and function prototypes

ast.c - Implements AST node creation and pretty-printing functions

mini_ada.l - Lexical rules for token recognition

mini_ada.y - Grammar rules and AST construction

👥 Authorship
Made by Bruno Barros and Orlando Soares.

Built as part of coursework at Faculdade de Ciências da Universidade do Porto.
