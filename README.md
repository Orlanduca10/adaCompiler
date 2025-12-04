# 🧠 Mini Ada Compiler

This project implements a **Mini Ada compiler frontend**, capable of parsing a simplified subset of the **Ada programming language** and generating an **Abstract Syntax Tree (AST)** representation of the program.

The project uses **Flex** for lexical analysis and **Bison** for parsing, written in **C**.

---

## 🚀 Features

- Lexical and syntax analysis for a small subset of Ada  
- AST (Abstract Syntax Tree) construction  
- Pretty-printing of the AST  
- Basic support for:
  - `procedure Main` program structure  
  - Variable assignment (`x := 5;`)  
  - `if ... then ... else ... end if;` statements  
  - `while ... loop ... end loop;` loops  
  - `Put_Line(...)` and `Get_Line;` statements  
  - Arithmetic and comparison operators (`+`, `-`, `*`, `/`, `=`, `<`, `>`, etc.)  
  - Logical operators (`and`, `or`, `not`)  
  - Literals (numbers, strings) and variable references  

---

## 🧩 Project Structure

mini_ada/
│
├── ast.h # AST (Abstract Syntax Tree) structure and prototypes
├── ast.c # Implementation of AST creation and printing
│
├── mini_ada.l # Lexical analyzer (Flex)
├── mini_ada.y # Parser (Bison)
|
└── README.md # This file

## ⚙️ Build Instructions

### Requirements
Make sure you have the following tools installed:

- **Flex** (lexical analyzer generator)
- **Bison** (parser generator)
- **gcc** (C compiler)

Run this command (in Linux):

flex mini_ada.l
bison -d mini_ada.y
gcc mini_ada.tab.c lex.yy.c ast.c -o mini_ada