#ifndef TAC_H
#define TAC_H

// ... (Existing TACOp enum and TAC struct definitions) ...

typedef enum
{
    TAC_ADD,
    TAC_SUB,
    TAC_MUL,
    TAC_DIV,
    TAC_EQ,
    TAC_LT,
    TAC_GT,     // Comparisons
    TAC_COPY,   // x := y
    TAC_LABEL,  // Label for jumps (L1:)
    TAC_JUMP,   // Unconditional jump (goto L1)
    TAC_JFALSE, // Jump if false (if t0 == 0 goto L1)
    TAC_CALL,   // Function call
    TAC_PRINT,  // Special for Put_Line
    TAC_READ,   // Special for Get_Line
    TAC_NEQ,     // Not equal comparison
    TAC_AND,
    TAC_OR,
    TAC_NOT, // And, Or, Not expressions
} TACOp;

typedef struct TAC
{
    TACOp op;
    char *arg1;
    char *arg2;
    char *result;
    struct TAC *next;
} TAC;

// New structure for tracking string literals
typedef struct StringLiteral
{
    char *value;
    char *label;
    struct StringLiteral *next;
} StringLiteral;

// Global head for string list
extern StringLiteral *string_list_head;

// Functions
TAC *new_tac(TACOp op, char *arg1, char *arg2, char *result);
void print_tac(TAC *head);
char *make_temp();
char *make_label();

// New function to handle string literals
char *get_string_label(const char *value);

#endif