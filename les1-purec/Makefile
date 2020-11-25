CC=gcc
LD=gcc
CFLAGS=-g -Wall -Werror -pedantic-errors -std=c17

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

fibonacci: src/fibonacci.o
	$(LD) -o fibonacci src/fibonacci.o

.PHONY : clean
clean:
	rm -f src/*.o fibonacci