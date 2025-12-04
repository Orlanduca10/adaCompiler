#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

// Assume integer type (0) for all variables and temporaries
typedef enum {
    TYPE_INT = 0,
    TYPE_STRING = 1,
    TYPE_BOOL = 2
} DataType;

typedef struct Symbol {
    char *name;
    DataType type;
    int offset;
    struct Symbol *next;
} Symbol;

// Global variable tracking stack allocation size
extern int current_offset; 

// Prototypes for functions implemented in symbol_table.c
void add_symbol(char *name, DataType type);
Symbol* lookup_symbol(char *name);
void print_symbol_table();

#endif