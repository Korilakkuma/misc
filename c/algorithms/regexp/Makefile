CC                = gcc
CFLAGS            = -std=c99 -O2 -Wall

all               : bin/regexp
bin/regexp        : main.o utils.o tokenizer.o tree_constructor.o nfa.o dfa.o
	$(CC) $^ -o $@
main.o            : cli/main.c
	$(CC) -c $(CFLAGS) $<
utils.o           : src/utils.c
	$(CC) -c $(CFLAGS) $<
tokenizer.o       : src/tokenizer.c
	$(CC) -c $(CFLAGS) $<
tree_constructor.o: src/tree_constructor.c
	$(CC) -c $(CFLAGS) $<
nfa.o             : src/nfa.c
	$(CC) -c $(CFLAGS) $<
dfa.o             : src/dfa.c
	$(CC) -c $(CFLAGS) $<
format            :
	clang-format -style=Google -i
clean             :
	rm -rf *.o regexp
