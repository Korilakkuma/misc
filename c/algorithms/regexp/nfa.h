#include "utils.h"
#include "tree_constructor.h"

#define EMPTY -1
#define NFA_STATE_MAX 128
#define NFA_VECTOR_SIZE (NFA_STATE_MAX / 8)
#define check_N_state(state, s) ((state)->vec[(s) / 8] & (1 << ((s) % 8)))

typedef struct nlist {
  char c;
  int to;
  struct nlist *next;
} nlist_t;

typedef struct {
  unsigned char vec[NFA_VECTOR_SIZE];
} N_state_set_t;

nlist_t *nfa[NFA_STATE_MAX];
int nfa_entry;
int nfa_exit;

extern int nfa_nstate;

extern int generate_node(void);
extern void add_transition(int from, int to, char c);
extern void generate_nfa(tree_t *tree, int entry, int way_out);
extern void build_nfa(tree_t *tree);
extern void dump_N_state_set(N_state_set_t *p);
extern void dump_nfa(void);
