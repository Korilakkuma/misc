#include "../src/tree_constructor/tree.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("Usage:\n    $ ./tokenize `MML string`\n", stdout);
    exit(EXIT_FAILURE);
  }

  char *in = argv[1];

  Token *tokens = (Token *)malloc(sizeof(Token) * strlen(in));

  if (tokens == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  fputs("\nTokenization\n\n", stdout);

  tokenize_print(in, tokens);

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
