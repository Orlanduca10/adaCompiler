CC = gcc
CFLAGS = -g -Wall

# List of all source files
SRCS = main.c ast.c symbol_table.c semantics.c mini_ada.tab.c lex.yy.c
OBJS = $(SRCS:.c=.o)

# Main target
compiler: $(OBJS)
	$(CC) $(CFLAGS) -o compiler $(OBJS)

# Generate Bison files
mini_ada.tab.c mini_ada.tab.h: mini_ada.y
	bison -d mini_ada.y

# Generate Flex file
lex.yy.c: mini_ada.l mini_ada.tab.h
	flex mini_ada.l

# Cleanup
clean:
	rm -f compiler lex.yy.c mini_ada.tab.c mini_ada.tab.h *.o out.s