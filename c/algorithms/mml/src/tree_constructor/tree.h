#include "../tokenizer/token.h"

typedef struct Tree {
  Token *token;
  struct Tree *left;
  struct Tree *right;
} Tree;

extern void tree_construct(Token *tokens, Tree *trees);
extern void tree_destruct(Tree *trees);
extern void tree_print(Tree *trees);
