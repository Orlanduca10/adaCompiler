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

---
📁 Project Structure
adaCompiler/    
|- Makefile    
|- README.md    
|- ast.c    
|- ast.h    
|- ast.o    
|- codegen.c    
|- codegen.h    
|- compiler.exe    
|- lex.yy.c    
|- lex.yy.o    
|- main.c    
|- mini_ada.exe    
|- mini_ada1    
|- mini_adatab.c    
|- mini_adatab.h    
|- mini_adatab.o    
|- mini_aday    
|- mips_code    
|- mips_codegen.c    
|- mips_codegen.h    
|- outs    
|- semantics.c    
|- semantics.h    
|- symbol_table.c    
|- symbol_table.h    
|- tac.c    
|- tac.h    
|- test.ada    
|- test1.ada        
|- test2.ada    


README.md
⚙️ Build Instructions
Requirements
Make sure you have the following tools installed:

Flex (lexical analyzer generator)
Bison (parser generator)
gcc (C compiler)
Run this command (in Linux) (Ignoring the already flex and bison compiled files)

# Regenerate
bison -d mini_ada.y
flex mini_ada.l
gcc -g -Wall -o compiler main.c ast.c symbol_table.c semantics.c tac.c codegen.c mips_codegen.c mini_ada.tab.c lex.yy.c

# Test
./compiler test.ada


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
📚 **Documentation**
**Makefile** - Automates compilation, linking, and cleanup for the entire project.

**README.md** - Project documentation and instructions.

**ast.h** - Defines all AST node structures and function prototypes.

**ast.c** - Implements AST node creation, traversal, and printing utilities.

**ast.o** - Object file generated from ast.c during compilation.

**codegen.h** - Function prototypes and shared structures for code generation.

**codegen.c** - Generates intermediate code or target code from the AST.

**compiler.exe** - Compiled executable of the Mini-Ada compiler (likely Windows build).

**lex.yy.c** - Generated lexical analyzer produced by flex.

**lex.yy.o** - Object file generated from the lexer source.

**main.c** - Compiler entry point; coordinates parsing, semantic analysis, and codegen.

**mini_ada.exe** - Executable version of the Mini-Ada compiler.

**mini_ada1** - Likely an older or alternate output binary for testing.

**mini_adatab.c** - Implements lookup tables or metadata for Mini-Ada grammar.

**mini_adatab.h** - Definitions and declarations for Mini-Ada grammar tables.

**mini_adatab.o** - Object file generated from mini_adatab.c.

**mini_ada.y** - Likely parser output or binary generated from .y grammar (platform dependent).

**mips_code** - Directory or file containing MIPS assembly output.

**mips_codegen.c** - Converts internal IR or AST into MIPS assembly instructions.

**mips_codegen.h** - Header file defining MIPS codegen functions and structures.

**out.s** - Output directory or file for generated compiler results.

**semantics.c** - Implements semantic analysis (type checking, scope rules, symbol validation).

**semantics.h** - Function prototypes and data structures for the semantic analyzer.

**symbol_table.c** - Implements symbol table creation, lookup, and scope management.

**symbol_table.h** - Symbol table definitions and function prototypes.

**tac.c** - Implements TAC (Three-Address Code) intermediate representation generation.

**tac.h** - Header for TAC structures and functions.

**test.ada** - Sample Ada test program for compiler validation.

**test1.ada** - Additional test case for parsing and analysis.

**test2.ada** -Another test program for compiler functionality.

👥 Authorship
Made by Bruno Barros and Orlando Soares.

Built as part of coursework at Faculdade de Ciências da Universidade do Porto.

