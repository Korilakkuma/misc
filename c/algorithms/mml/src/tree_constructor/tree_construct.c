#include "tree.h"

void tree_construct(Token *tokens, Tree *trees) {
  Tree *tree  = (Tree *)malloc(sizeof(Tree));
  Tree *left  = (Tree *)malloc(sizeof(Tree));
  Tree *right = (Tree *)malloc(sizeof(Tree));

  switch (tokens->type) {
    case TEMPO:
      // Left leaf
      left->token = tokens + 1;  // Look-ahead
      left->left  = NULL;
      left->right = NULL;

      // Right partial tree
      right->left  = NULL;
      right->right = NULL;

      // Root
      tree->token = tokens;
      tree->left  = left;
      tree->right = right;
      break;
    case OCTAVE:
      // Left leaf
      left->token = tokens + 1;  // Look-ahead
      left->left  = NULL;
      left->right = NULL;

      // Right partial tree
      right->left  = NULL;
      right->right = NULL;

      // Root
      tree->token = tokens;
      tree->left  = left;
      tree->right = right;

      // Concat partial tree
      (trees - 1)->right = tree;
      break;
    case NOTE:
    case REST:
      // Left leaf
      left->token = tokens + 1;  // Look-ahead
      left->left  = NULL;
      left->right = NULL;

      // Right partial tree
      right->left  = NULL;
      right->right = NULL;

      // Root
      tree->token = tokens;
      tree->left  = left;
      tree->right = right;

      // Concat partial tree
      (trees - 1)->right = tree;
      break;
    case EOM:
      // Root and Right leaf
      tree->token = tokens;
      tree->left  = NULL;
      tree->right = NULL;

      // Concat partial tree
      (trees - 1)->right = tree;

      *trees = *tree;
      return;
    default:
      tree->token = tokens;
      tree->left  = NULL;
      tree->right = NULL;

      *trees = *tree;
      return;
  }

  *trees++ = *tree;

  tokens += 2;

  tree_construct(tokens, trees);
}

void tree_destruct(Tree *trees) {
  if (trees->token->type == TEMPO) {
    free(trees->left);
    free(trees->right);
    return;
  }

  // Increment until pointing EOM
  while (trees->token->type != EOM) {
    // Next right partial tree
    trees += 2;
  }

  while (trees->token->type != TEMPO) {
    // Previous parent
    trees -= 2;

    free(trees->left);
    free(trees->right);
  }
}

void tree_print(Tree *trees) {
  int indent = 0;

  while (trees->right != NULL) {
    for (int i = 0; i < indent; i++) {
      fputs("    ", stdout);
    }

    fprintf(stdout, "   %-2s\n", trees->token->token);

    for (int i = 0; i < indent; i++) {
      fputs("    ", stdout);
    }

    fputs("  / \\\n", stdout);

    for (int i = 0; i < indent; i++) {
      fputs("    ", stdout);
    }

    if ((trees + 1)->right == NULL) {
      fprintf(stdout, "%s", trees->left->token->token);
    } else {
      fprintf(stdout, "%s\n", trees->left->token->token);
    }

    ++trees;
    ++indent;
  }

  fputs("  EOM\n", stdout);
}
