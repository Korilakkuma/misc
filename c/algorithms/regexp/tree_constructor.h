#include "utils.h"
#include "tokenizer.h"

typedef enum {
  op_char,
  op_concat,
  op_union,
  op_closure,
  op_empty
} op_t;

typedef struct tree {
  op_t op;

  union {
    char c;

    struct {
      struct tree *_left;
      struct tree *_right;
    } x;
  } u;
} tree_t;

extern tree_t *make_tree_node(op_t op, tree_t *left, tree_t *right);
extern tree_t *make_atom(char c);
extern tree_t *regexp(void);
extern tree_t *term(void);
extern tree_t *factor(void);
extern tree_t *primary(void);
extern void dump_tree(tree_t *p);
