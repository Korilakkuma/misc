CC                   = clang
CFLAGS               = -Wall -O2 -std=c99

all                  : regexp
regexp               : regexp.o utils.o tokenizer.o tree_constructor.o nfa.o dfa.o
	$(CC) $^ -o $@
regexp.o             : regexp.c
	$(CC) -c $(CFLAGS) $<
utils.o              : utils.c
	$(CC) -c $(CFLAGS) $<
tokenizer.o          : tokenizer.c
	$(CC) -c $(CFLAGS) $<
tree_constructor.o   : tree_constructor.c
	$(CC) -c $(CFLAGS) $<
nfa.o                : nfa.c
	$(CC) -c $(CFLAGS) $<
dfa.o                : dfa.c
	$(CC) -c $(CFLAGS) $<
format               :
	clang-format -style=Google -i
clean                :
	rm -rf *.o regexp
