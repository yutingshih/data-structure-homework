SRC = main.c
OBJ = $(patsubst %.c, %.o, $(SRC))
BIN = a.out

CC = gcc
CFLAGS = -std=c11 -Wall -O0 -g
LDFLAGS = -lm

.PHONY: all clean

all: $(BIN)

clean:
	-@$(RM) -rfv $(BIN) $(OBJ)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

