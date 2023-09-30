# Compiler to use
CC = gcc

# Flags to pass to the compiler
CFLAGS = -Wall -Wextra

# Source files
SOURCES = $(wildcard src/*.c)

# Output binary
OUT = program

all: $(OUT)

$(OUT): $(SOURCES)
	$(CC) $(CFLAGS) -o $(OUT) $(SOURCES)

clean:
	rm -f $(OUT)
