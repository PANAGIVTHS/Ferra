CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

SRC_DIR = src
OUT_DIR = .out

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OUT_DIR)/%.o, $(SRC))

EXEC = $(OUT_DIR)/mycompiler

all: $(EXEC)

$(EXEC): $(OBJ)
	mkdir -p $(OUT_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(OUT_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OUT_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OUT_DIR)