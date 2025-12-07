#include "tac.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int temp_count = 0;
int label_count = 0;
int string_count = 0;
StringLiteral *string_list_head = NULL;

// Global TAC list head and tail
TAC *tac_head = NULL;
TAC *tac_tail = NULL;

TAC *new_tac(TACOp op, char *arg1, char *arg2, char *result)
{
    TAC *instr = malloc(sizeof(TAC));
    instr->op = op;
    instr->arg1 = arg1 ? strdup(arg1) : NULL;
    instr->arg2 = arg2 ? strdup(arg2) : NULL;
    instr->result = result ? strdup(result) : NULL;
    instr->next = NULL;
    return instr;
}

char *make_temp()
{
    char buffer[20];
    sprintf(buffer, "t%d", temp_count++);
    return strdup(buffer);
}

char *make_label()
{
    char buffer[20];
    sprintf(buffer, "L%d", label_count++);
    return strdup(buffer);
}

// Function to find or create a unique label for a string literal
char *get_string_label(const char *value)
{
    StringLiteral *curr = string_list_head;
    // 1. Check if string already exists
    while (curr)
    {
        if (strcmp(curr->value, value) == 0)
        {
            return curr->label;
        }
        curr = curr->next;
    }

    // 2. String does not exist, create new entry
    StringLiteral *new_string = malloc(sizeof(StringLiteral));
    new_string->value = strdup(value);

    // Assign new label (S0, S1, S2...)
    char label_buffer[20];
    sprintf(label_buffer, "S%d", string_count++);
    new_string->label = strdup(label_buffer);
    new_string->next = NULL;

    // Add to list
    if (string_list_head == NULL)
    {
        string_list_head = new_string;
    }
    else
    {
        curr = string_list_head;
        while (curr->next)
            curr = curr->next;
        curr->next = new_string;
    }

    return new_string->label;
}

void print_tac(TAC* head) {
    TAC* curr = head;
    while (curr) {
        switch (curr->op) {
            case TAC_ADD: printf("%s = %s + %s\n", curr->result, curr->arg1, curr->arg2); break;
            case TAC_SUB: printf("%s = %s - %s\n", curr->result, curr->arg1, curr->arg2); break;
            case TAC_MUL: printf("%s = %s * %s\n", curr->result, curr->arg1, curr->arg2); break;
            case TAC_DIV: printf("%s = %s / %s\n", curr->result, curr->arg1, curr->arg2); break;
            case TAC_COPY: printf("%s = %s\n", curr->result, curr->arg1); break;
            case TAC_LABEL: printf("%s:\n", curr->result); break;
            case TAC_JUMP: printf("goto %s\n", curr->result); break;
            case TAC_JFALSE: printf("ifFalse %s goto %s\n", curr->arg1, curr->result); break;
            case TAC_GT: printf("%s = %s > %s\n", curr->result, curr->arg1, curr->arg2); break;
            case TAC_LT: printf("%s = %s < %s\n", curr->result, curr->arg1, curr->arg2); break;
            case TAC_EQ: printf("%s = %s == %s\n", curr->result, curr->arg1, curr->arg2); break;
            case TAC_NEQ: printf("%s = %s != %s\n", curr->result, curr->arg1, curr->arg2); break;
            case TAC_PRINT: printf("PRINT %s\n", curr->arg1); break;
            case TAC_READ: printf("READ %s\n", curr->result); break;
            case TAC_CALL: 
                if (curr->arg2)
                    printf("%s = call %s, %s\n", curr->result, curr->arg1, curr->arg2);
                else
                    printf("call %s\n", curr->arg1);
                break;
            default: 
                printf("Unknown TAC operation: %d\n", curr->op); 
                break;
        }
        curr = curr->next;
    }
}