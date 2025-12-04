#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Symbol *symbol_table = NULL;
int current_offset = 0; // Tracks stack position for MIPS

void add_symbol(char *name, DataType type) {
    // Check if exists first
    if (lookup_symbol(name) != NULL) return;

    Symbol *s = malloc(sizeof(Symbol));
    s->name = strdup(name);
    s->type = type;
    s->offset = current_offset;
    s->next = symbol_table;
    
    // Update global state
    symbol_table = s;
    current_offset += 4; // Increment by 4 bytes (size of int/pointer)
}

Symbol* lookup_symbol(char *name) {
    Symbol *current = symbol_table;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void print_symbol_table() {
    printf("\n--- Symbol Table ---\n");
    Symbol *current = symbol_table;
    while (current != NULL) {
        // Convert type enum to string
        char *type_str;
        switch(current->type) {
            case TYPE_INT:    type_str = "INT"; break;
            case TYPE_STRING: type_str = "STRING"; break;
            case TYPE_BOOL:   type_str = "BOOL"; break;
            default:          type_str = "UNKNOWN"; break;
        }
        
        printf("Name: %-10s | Type: %-6s | Offset: %d\n", 
               current->name, type_str, current->offset);
        current = current->next;
    }
    printf("--------------------\n");
}