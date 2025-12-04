#include "symtab.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static unsigned hash(const char *s) {
    unsigned h = 2166136261u;
    while (*s) {
        h ^= (unsigned char)*s++;
        h *= 16777619u;
    }
    return h;
}

Scope *scope_new(Scope *parent, int size) {
    Scope *sc = malloc(sizeof(Scope));
    sc->size = size;
    sc->parent = parent;
    sc->table = calloc(size, sizeof(Symbol *));
    return sc;
}

bool scope_insert(Scope *s, const char *name, TypeKind type) {
    unsigned h = hash(name) % s->size;
    for (Symbol *cur = s->table[h]; cur; cur = cur->next)
        if (!strcmp(cur->name, name)) return false;  // already exists

    Symbol *sym = malloc(sizeof(Symbol));
    sym->name = strdup(name);
    sym->type = type;
    sym->offset = -1;
    sym->next = s->table[h];
    s->table[h] = sym;
    return true;
}

Symbol *scope_lookup(Scope *s, const char *name) {
    while (s) {
        unsigned h = hash(name) % s->size;
        for (Symbol *cur = s->table[h]; cur; cur = cur->next)
            if (!strcmp(cur->name, name)) return cur;
        s = s->parent;
    }
    return NULL;
}

void scope_free(Scope *s) {
    if (!s) return;
    for (int i = 0; i < s->size; i++) {
        Symbol *cur = s->table[i];
        while (cur) {
            Symbol *next = cur->next;
            free(cur->name);
            free(cur);
            cur = next;
        }
    }
    free(s->table);
    free(s);
}

void print_scope(Scope *s) {
    printf("---- Symbol Table ----\n");
    for (int i = 0; i < s->size; i++) {
        for (Symbol *cur = s->table[i]; cur; cur = cur->next) {
            printf("  %s : type=%d offset=%d\n",
                   cur->name, cur->type, cur->offset);
        }
    }
}
