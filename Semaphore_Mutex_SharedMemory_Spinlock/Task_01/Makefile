# Compiler
CC = gcc

# Lib
LIBS = -lpthread

# Build all .c file
SRCS = $(wildcard *.c)
TARGETS = $(SRCS:.c=)

# Default rule to build exe
all: $(TARGETS)

# Rule to build each exee from .c file
%: %.c
	$(CC) -o $@ $< $(LIBS)

# Clean
clean:
	rm -f $(TARGETS)
