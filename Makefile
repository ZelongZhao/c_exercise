CFLAGS=-Wall -g

CC=gcc

SRC=$(wildcard *.c)

TARGETS=$(patsubst %.c,%,$(SRC))

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(TARGETS)

.PHONY: all clean