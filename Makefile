SRC = main.c
OBJ = $(patsubst %.c, %.o, $(SRC))
BIN = a.out

TEST_SCRIPT = ../test.sh

CC = gcc
CFLAGS = -std=c11 -Wall -O0 -g
LDFLAGS = -lm

.PHONY: all clean test help

all: $(BIN)	## compile the source file (main.c)

test: $(BIN) $(TEST_SCRIPT)	## run the test script
	$(TEST_SCRIPT)

clean:	## remove object and executable fils
	-@$(RM) -rfv $(BIN) $(OBJ)

help:	## show this help message
	@egrep -h '\s##\s' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "%-10s %s\n", $$1, $$2}'

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
