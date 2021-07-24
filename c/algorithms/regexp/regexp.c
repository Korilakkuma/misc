#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 1
#define EMPTY -1
#define NFA_STATE_MAX 128
#define NFA_VECTOR_SIZE (NFA_STATE_MAX / 8)
#define check_N_state(state, s) ((state)->vec[(s) / 8] & (1 << ((s) % 8)))
#define DFA_STATE_MAX 100

typedef enum {
  tk_char,
  tk_union,
  tk_lpar,
  tk_rpar,
  tk_star,
  tk_plus,
  tk_end
} token_t;

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

typedef struct nlist {
  char c;
  int to;
  struct nlist *next;
} nlist_t;

typedef struct {
  unsigned char vec[NFA_VECTOR_SIZE];
} N_state_set_t;

typedef struct dlist {
  char c;
  N_state_set_t to;
  struct dlist *next;
} dlist_t;

typedef struct dslist {
  char c;
  struct D_state_t *to;
  struct dslist *next;
} dslist_t;

typedef struct D_state_t {
  N_state_set_t *state;
  int visited;
  int accepted;
  dslist_t *next;
} D_state_t;

void *alloc_memory(size_t size);
void syntax_error(char *s);
token_t get_token();
void initialize(char *str);
tree_t *make_tree_node(op_t op, tree_t *left, tree_t *right);
tree_t *make_atom(char c);
tree_t *regexp(void);
tree_t *term(void);
tree_t *factor(void);
tree_t *primary(void);
void dump_tree(tree_t *p);
tree_t *parse_regexp(char *str);
int generate_node(void);
void add_transition(int from, int to, char c);
void generate_nfa(tree_t *tree, int entry, int way_out);
void build_nfa(tree_t *tree);
void dump_nfa(void);
void dump_N_state_set(N_state_set_t *p);
void dump_dfa(void);
void add_N_state(N_state_set_t *state, int s);
void mark_empty_transition(N_state_set_t *state, int s);
void collect_empty_transition(N_state_set_t *state);
int equal_N_state_set(N_state_set_t *a, N_state_set_t *b);
D_state_t *register_D_state(N_state_set_t *s);
D_state_t *fetch_unvisited_D_state(void);
dlist_t *compute_reachable_N_state(D_state_t *dstate);
void convert_nfa_to_dfa(void);
D_state_t *next_dfa_state(D_state_t *state, char c);
void do_match(char *string, char **from, char **to);

token_t current_token;

char token_char;

char *strbuf;

nlist_t *nfa[NFA_STATE_MAX];

int nfa_entry;

int nfa_exit;

int nfa_nstate = 0;

D_state_t dfa[DFA_STATE_MAX];

D_state_t *initial_dfa_state;

int dfa_nstate = 0;

int main(int argc, char **argv) {
  tree_t *tree;
  char buf[1024];
  char *p, *from, *to;

  if (argc != 2) {
    fputs("Usage: regexp regular-expression\n", stderr);
    exit(EXIT_FAILURE);
  }

  tree = parse_regexp(argv[1]);

  build_nfa(tree);

#if DEBUG
  dump_nfa();
#endif

  convert_nfa_to_dfa();

  while (fgets(buf, (sizeof(buf) - 1), stdin) != NULL) {
    buf[sizeof(buf) - 1] = '\0';

    if ((p = strchr(buf, '\n')) != NULL) {
      *p = '\0';
    }

    do_match(buf, &from, &to);

    if (from != NULL) {
      printf("%s\n", buf);

      for (p = buf; p < from; p++) {
        printf(" ");

        for(; p < to; p++) {
          printf("-");
        }

        printf("\n");
      }
    }
  }

  return 0;
}

void *alloc_memory(size_t size) {
  void *p;

  if ((p = malloc(size)) == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  memset(p, 0, size);

  return p;
}

void syntax_error(char *s) {
  fprintf(stderr, "Syntax error: %s\n", s);
  exit(EXIT_FAILURE);
}

token_t get_token() {
  char c;

  if (*strbuf == '\0') {
    current_token = tk_end;
  } else {
    c = *strbuf++;

    switch (c) {
      case '|': current_token = tk_union; break;
      case '(': current_token = tk_lpar;  break;
      case ')': current_token = tk_rpar;  break;
      case '*': current_token = tk_star;  break;
      case '+': current_token = tk_plus;  break;
      default:
        current_token = tk_char;
        token_char = c;
        break;
    }
  }

  return current_token;
}

void initialize(char *str) {
  strbuf = str;

  get_token();
}

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
    get_token();
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
    get_token();
  } else if (current_token == tk_plus) {
    x = make_tree_node(op_concat, x, make_tree_node(op_closure, x, NULL));
    get_token();
  }

  return x;
}

tree_t *primary(void) {
  tree_t *x;

  if (current_token == tk_char) {
    x = make_atom(token_char);
    get_token();
  } else if (current_token == tk_lpar) {
    get_token();

    x = regexp();

    if (current_token != tk_rpar) {
      syntax_error("Close paren is expected.");
      exit(EXIT_FAILURE);
    }

    get_token();
  } else {
    syntax_error("Normal character or open paren is expected.");
    exit(EXIT_FAILURE);
  }

  return x;
}

#if DEBUG == 1
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
#endif

tree_t *parse_regexp(char *str) {
  tree_t *t;

  initialize(str);

  t = regexp();

  if (current_token != tk_end) {
    syntax_error("Extra character at end of pattern");
  }

#if DEBUG == 1
  dump_tree(t);
  printf("\n");
#endif

  return t;
}

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

#if DEBUG == 1
void dump_nfa(void) {
  nlist_t *p;

  for (int i = 0; i < NFA_STATE_MAX; i++) {
    if (nfa[i] != NULL) {
      printf("state %3d: ", i);

      for (p = nfa[i]; p != NULL; p = p->next) {
        printf("(%c . %d)", (p->c == EMPTY ? '?' : p->c), p->to);
      }
    }
  }
}
#endif

#if DEBUG == 1
void dump_N_state_set(N_state_set_t *p) {
  for (int i = 0; i < nfa_nstate; i++) {
    if (check_N_state(p, i)) {
      printf("%d ", i);
    }
  }
}

void dump_dfa(void) {
  for (int i = 0; i < dfa_nstate; i++) {
    printf("%2d%c: ", i, (dfa[i].accepted ? 'A' : ' '));

    for (dslist_t *l = dfa[i].next; l != NULL; l = l->next) {
      printf("%c->%ld ", l->c, (l->to - dfa));
    }

    printf("\n");
  }
}
#endif

void add_N_state(N_state_set_t *state, int s) {
  state->vec[s / 8] |= (1 << (s % 8));
}

void mark_empty_transition(N_state_set_t *state, int s) {
  add_N_state(state, s);

  for (nlist_t *p = nfa[s]; p != NULL; p = p->next) {
    if ((p->c == EMPTY) && !check_N_state(state, p->to)) {
      mark_empty_transition(state, p->to);
    }
  }
}

void collect_empty_transition(N_state_set_t *state) {
  for (int i = 0; i < nfa_nstate; i++) {
    if (check_N_state(state, i)) {
      mark_empty_transition(state, i);
    }
  }
}

int equal_N_state_set(N_state_set_t *a, N_state_set_t *b) {
  for (int i = 0; i < NFA_VECTOR_SIZE; i++) {
    if (a->vec[i] != b->vec[i]) {
      return 0;
    }
  }

  return 1;
}

D_state_t *register_D_state(N_state_set_t *s) {
  for (int i = 0; i < dfa_nstate; i++) {
    if (equal_N_state_set(dfa[i].state, s)) {
      return &dfa[i];
    }
  }

  if (dfa_nstate >= DFA_STATE_MAX) {
    fprintf(stderr, "Too many DFA state.\n");
    exit(EXIT_FAILURE);
  }

  dfa[dfa_nstate].state    = s;
  dfa[dfa_nstate].visited  = 0;
  dfa[dfa_nstate].accepted = check_N_state(s, nfa_exit) ? 1 : 0;
  dfa[dfa_nstate].next     = NULL;

  return &dfa[dfa_nstate++];
}

D_state_t *fetch_unvisited_D_state(void) {
  for (int i = 0; i < dfa_nstate; i++) {
    if (dfa[i].visited == 0) {
      return &dfa[i];
    }
  }

  return NULL;
}

dlist_t *compute_reachable_N_state(D_state_t *dstate) {
  nlist_t *p;
  dlist_t *result, *a, *b;

  N_state_set_t *state = dstate->state;

  result = NULL;

  for (int i = 0; i < nfa_nstate; i++) {
    if (check_N_state(state, i)) {
      for (p = nfa[i]; p != NULL; p = p->next) {
        if (p->c != EMPTY) {
          for (a = result; a != NULL; a = a->next) {
            if (a->c == p->c) {
              add_N_state(&a->to, p->to);
              goto added;
            }
          }

          b    = alloc_memory(sizeof(dlist_t));
          b->c = p->c;

          add_N_state(&b->to, p->to);

          b->next = result;
          result  = b;
added:
          ;
        }
      }
    }
  }

  return result;
}

void convert_nfa_to_dfa(void) {
  N_state_set_t *initial_state;
  D_state_t *t;
  dlist_t *x;
  dslist_t *p;

  initial_state = alloc_memory(sizeof(N_state_set_t));

  add_N_state(initial_state, nfa_entry);
  collect_empty_transition(initial_state);

  initial_dfa_state = register_D_state(initial_state);

  while ((t = fetch_unvisited_D_state()) != NULL) {
    t->visited = 1;

    for (x = compute_reachable_N_state(t); x != NULL; x = x->next) {
      collect_empty_transition(&x->to);

      p    = alloc_memory(sizeof(dslist_t));
      p->c = x->c;

      p->to   = register_D_state(&x->to);
      p->next = t->next;
      t->next = p;
    }
  }

#if DEBUG == 1
  printf("\n--- DFA ---\n");
  dump_dfa();
#endif
}

D_state_t *next_dfa_state(D_state_t *state, char c) {
  for (dslist_t *p = state->next; p != NULL; p = p->next) {
    if (c == p->c) {
      return p->to;
    }
  }

  return NULL;
}

void do_match(char *string, char **from, char **to) {
  char *p, *max_match;
  D_state_t *state;

  for (char *start = string; *start != '\0'; start++) {
    state     = initial_dfa_state;
    max_match = NULL;
    p         = start;

    while (state != NULL) {
      if (state->accepted) {
        max_match = p;
      }

      if (*p == '\0') {
        break;
      }

      state = next_dfa_state(state, *p++);
    }

    if ((max_match != NULL) && (max_match != start)) {
      *from = start;
      *to   = max_match;
      return;
    }
  }

  *from = NULL;
}
