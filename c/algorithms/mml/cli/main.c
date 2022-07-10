#include "../src/includes.h"
#include "../src/types.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("Usage:\n    $ ./bin/parse `MML string`\n", stdout);
    exit(EXIT_FAILURE);
  }

  char *in = argv[1];

  Token *tokens = (Token *)malloc(sizeof(Token) * strlen(in));

  if (tokens == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  fputs("\nTokenization\n\n", stdout);

  tokenize(in, tokens);
  tokenize_print(tokens, (unsigned long)strlen(in));

  fputs("\nTree Constructor\n\n", stdout);

  Tree *trees = (Tree *)malloc(sizeof(Tree) * strlen(in));

  if (trees == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  tree_construct(tokens, trees);
  tree_print(trees);
  tree_destruct(trees);

  free(tokens);
  free(trees);

  return 0;
}
