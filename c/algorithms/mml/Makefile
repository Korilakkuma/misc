CC              = gcc
CFLAGS          = -std=c99 -O2 -Wall

all             : bin/parse
bin/parse       : main.o tree_construct.o tokenize.o
	$(CC) $^ -o $@
main.o          : cli/main.c
	$(CC) -c $(CFLAGS) $<
tree_construct.o: src/tree_construct.c
	$(CC) -c $(CFLAGS) $<
tokenize.o      : src/tokenize.c
	$(CC) -c $(CFLAGS) $<

format          :
	clang-format -style=Google -i cli/main.c src/*/*.(c|h)

clean           :
	rm -rf *.o bin/parse
