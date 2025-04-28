CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

SRC = lexer.c parser.c ast.c
OBJ = $(SRC:.c=.o)

EXEC = mycompiler

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o $(EXEC)
