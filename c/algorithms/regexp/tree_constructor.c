#include "tree_constructor.h"

tree_t *make_tree_node(op_t op, tree_t *left, tree_t *right) {
  tree_t *p;

  p = (tree_t *)alloc_memory(sizeof(tree_t));

  p->op         = op;
  p->u.x._left  = left;
  p->u.x._right = right;

  return p;
}

tree_t *make_atom(char c) {
  tree_t *p;

  p = (tree_t *)alloc_memory(sizeof(tree_t));

  p->op  = op_char;
  p->u.c = c;

  return p;
}

tree_t *regexp(void) {
  tree_t *x;

  x = term();

  while (current_token == tk_union) {
    update_token();
    x = make_tree_node(op_union, x, term());
  }

  return x;
}

tree_t *term(void) {
  tree_t *x;

  if (current_token == tk_union || current_token == tk_rpar || current_token == tk_end) {
    x = make_tree_node(op_empty, NULL, NULL);
  } else {
    x = factor();

    while (current_token != tk_union && current_token != tk_rpar && current_token != tk_end) {
      x = make_tree_node(op_concat, x, factor());
    }
  }

  return x;
}

tree_t *factor(void) {
  tree_t *x;

  x = primary();

  if (current_token == tk_star) {
    x = make_tree_node(op_closure, x, NULL);
    update_token();
  } else if (current_token == tk_plus) {
    x = make_tree_node(op_concat, x, make_tree_node(op_closure, x, NULL));
    update_token();
  }

  return x;
}

tree_t *primary(void) {
  tree_t *x;

  if (current_token == tk_char) {
    x = make_atom(token_char);
    update_token();
  } else if (current_token == tk_lpar) {
    update_token();

    x = regexp();

    if (current_token != tk_rpar) {
      syntax_error("Close paren is expected.");
      exit(EXIT_FAILURE);
    }

    update_token();
  } else {
    syntax_error("Normal character or open paren is expected.");
    exit(EXIT_FAILURE);
  }

  return x;
}

void dump_tree(tree_t *p) {
  switch (p->op) {
    case op_char:
      printf("\"%c\"", p->u.c);
      break;
    case op_concat:
      printf("(concat ");
      dump_tree(p->u.x._left);
      printf(" ");
      dump_tree(p->u.x._right);
      printf(")");
      break;
    case op_union:
      printf("(or ");
      dump_tree(p->u.x._left);
      printf(" ");
      dump_tree(p->u.x._right);
      printf(")");
      break;
    case op_closure:
      printf("(closure ");
      dump_tree(p->u.x._left);
      printf(")");
      break;
    case op_empty:
      printf("EMPTY");
      break;
    default:
      fputs("This cannot happen in <dump_tree>\n", stderr);
      exit(EXIT_FAILURE);
  }
}
