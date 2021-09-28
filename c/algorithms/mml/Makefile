CC                   = clang
CFLAGS               = -Wall -O2 -std=c99

all                  : bin/parse
bin/parse            : main.o tree_construct.o tokenize.o
	$(CC) $^ -o $@
main.o               : cli/main.c
	$(CC) -c $(CFLAGS) $<
tree_construct.o     : src/tree_constructor/tree_construct.c
	$(CC) -c $(CFLAGS) $<
tokenize.o           : src/tokenizer/tokenize.c
	$(CC) -c $(CFLAGS) $<

format               :
	clang-format -style=Google -i cli/main.c src/*/*.(c|h)

clean                :
	rm -rf *.o bin/parse
