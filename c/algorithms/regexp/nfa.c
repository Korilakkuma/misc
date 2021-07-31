#include "nfa.h"

int nfa_nstate = 0;

int generate_node(void) {
  if (nfa_nstate >= NFA_STATE_MAX) {
    fputs("Too many NFA state.\n", stderr);
    exit(EXIT_FAILURE);
  }

  return nfa_nstate++;
}

void add_transition(int from, int to, char c) {
  nlist_t *p;

  p = (nlist_t *)alloc_memory(sizeof(nlist_t));

  p->c    = c;
  p->to   = to;
  p->next = nfa[from];

  nfa[from] = p;
}

void generate_nfa(tree_t *tree, int entry, int way_out) {
  int a1, a2;

  switch (tree->op) {
    case op_char:
      add_transition(entry, way_out, tree->u.c);
      break;
    case op_empty:
      add_transition(entry, way_out, EMPTY);
      break;
    case op_union:
      generate_nfa(tree->u.x._left, entry, way_out);
      generate_nfa(tree->u.x._right, entry, way_out);
      break;
    case op_closure:
      a1 = generate_node();
      a2 = generate_node();

      add_transition(entry, a1, EMPTY);
      generate_nfa(tree->u.x._left, a1, a2);
      add_transition(a2, a1, EMPTY);
      add_transition(a1, way_out, EMPTY);
      break;
    case op_concat:
      a1 = generate_node();

      generate_nfa(tree->u.x._left, entry, a1);
      generate_nfa(tree->u.x._right, a1, way_out);
      break;
    default:
      fputs("This cannot happen in <generate_nfa>\n", stderr);
      exit(EXIT_FAILURE);
  }
}

void build_nfa(tree_t *tree) {
  nfa_entry = generate_node();

  nfa_exit = generate_node();

  generate_nfa(tree, nfa_entry, nfa_exit);
}

void dump_N_state_set(N_state_set_t *p) {
  for (int i = 0; i < nfa_nstate; i++) {
    if (check_N_state(p, i)) {
      printf("%d ", i);
    }
  }
}

void dump_nfa(void) {
  nlist_t *p;

  for (int i = 0; i < NFA_STATE_MAX; i++) {
    if (nfa[i] != NULL) {
      printf("state %3d: ", i);

      for (p = nfa[i]; p != NULL; p = p->next) {
        printf("(%c . %d)", (p->c == EMPTY ? '?' : p->c), p->to);
      }

      printf("\n");
    }
  }
}
