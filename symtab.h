#ifndef SYMTAB_H
#define SYMTAB_H
#include <stdbool.h>

typedef enum { TYPE_INT, TYPE_STR } TypeKind;

typedef struct Symbol {
    char *name;
    TypeKind type;
    int offset;
    struct Symbol *next;
} Symbol;

typedef struct Scope {
    struct Scope *parent;
    int size;
    Symbol **table;
} Scope;

Scope *scope_new(Scope *parent, int size);
bool scope_insert(Scope *s, const char *name, TypeKind type);
Symbol *scope_lookup(Scope *s, const char *name);
void scope_free(Scope *s);
void print_scope(Scope *s);

#endif
