#include "nfa.h"

#define DFA_STATE_MAX 100

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

D_state_t dfa[DFA_STATE_MAX];
D_state_t *initial_dfa_state;

extern int dfa_nstate;

extern D_state_t *register_D_state(N_state_set_t *s);
extern D_state_t *fetch_unvisited_D_state(void);
extern dlist_t *compute_reachable_N_state(D_state_t *dstate);
extern D_state_t *next_dfa_state(D_state_t *state, char c);
extern void add_N_state(N_state_set_t *state, int s);
extern void mark_empty_transition(N_state_set_t *state, int s);
extern void collect_empty_transition(N_state_set_t *state);
extern int equal_N_state_set(N_state_set_t *a, N_state_set_t *b);
extern void convert_nfa_to_dfa(void);
extern void dump_dfa(void);
