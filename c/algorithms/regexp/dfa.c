#include "dfa.h"

int dfa_nstate = 0;

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

D_state_t *next_dfa_state(D_state_t *state, char c) {
  for (dslist_t *p = state->next; p != NULL; p = p->next) {
    if (c == p->c) {
      return p->to;
    }
  }

  return NULL;
}

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

  printf("\n--- DFA ---\n");
  dump_dfa();
}

void dump_dfa(void) {
  for (int i = 0; i < dfa_nstate; i++) {
    printf("%2d%c: ", i, (dfa[i].accepted ? 'A' : ' '));

    for (dslist_t *l = dfa[i].next; l != NULL; l = l->next) {
      printf("%c->%ld ", l->c, (l->to - dfa));
    }

    printf("\n");
  }

  for (int i = 0; i < dfa_nstate; i++) {
    printf("state %2d%c = { ", i, (dfa[i].accepted ? 'A' : ' '));
    dump_N_state_set(dfa[i].state);
    printf("}\n");
  }
}
