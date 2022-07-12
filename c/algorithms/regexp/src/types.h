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

char token_char;
token_t current_token;
char *strbuf;

nlist_t *nfa[NFA_STATE_MAX];
int nfa_entry;
int nfa_exit;

extern int nfa_nstate;

D_state_t dfa[DFA_STATE_MAX];
D_state_t *initial_dfa_state;

extern void syntax_error(const char *s);

extern void update_token(void);

extern tree_t *regexp(void);
extern void dump_tree(tree_t *p);

extern void build_nfa(tree_t *tree);
extern void dump_nfa(void);

extern void convert_nfa_to_dfa(void);
extern D_state_t *next_dfa_state(D_state_t *state, char c);
