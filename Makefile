CC = gcc
CFLAGS = -c -ggdb -Wall -Wextra -std=c11 -pedantic -O3 -funroll-loops
LDFLAGS = -lm
SOURCES = $(wildcard main.c ./lexer/*.c ./parser/*.c)
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = main

$(EXECUTABLE): $(OBJECTS)
	$(CC) $^ $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(EXECUTABLE) *.o ./lexer/*.o ./parser/*.o