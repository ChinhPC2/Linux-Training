# Compiler
CC = gcc

# Build all .c file
SRCS = $(wildcard *.c)
TARGETS = $(SRCS:.c=)

# Default rule to build exe
all: $(TARGETS)

# Rule to build each exe from .c file
%: %.c
	$(CC) -o $@ $<

# Clean
clean:
	rm -f $(TARGETS)