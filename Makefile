SRC = main.c
OBJ = $(patsubst %.c, %.o, $(SRC))
BIN = a.out

TEST_SCRIPT = ../test.sh

CC = gcc
CFLAGS = -std=c11 -Wall -O0 -g
LDFLAGS = -lm

.PHONY: all clean test

all: $(BIN)

test: $(BIN) $(TEST_SCRIPT)
	$(TEST_SCRIPT)

clean:
	-@$(RM) -rfv $(BIN) $(OBJ)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

