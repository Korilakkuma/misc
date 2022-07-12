#include "../src/includes.h"
#include "../src/types.h"

static void initialize(char *str);
static tree_t *parse_regexp(char *str);
static void do_match(char *string, char **from, char **to);

int main(int argc, char **argv) {
  tree_t *tree;

  char buf[1024];
  char *p;
  char *from;
  char *to;

  if (argc != 2) {
    fputs("Usage: ./bin/regexp regular-expression\n", stderr);
    exit(EXIT_FAILURE);
  }

  tree = parse_regexp(argv[1]);

  build_nfa(tree);

  printf("\n--- NFA ---\n");
  dump_nfa();

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

static void initialize(char *str) {
  strbuf = str;

  update_token();
}

static tree_t *parse_regexp(char *str) {
  tree_t *t;

  initialize(str);

  t = regexp();

  if (current_token != tk_end) {
    syntax_error("Extra character at end of pattern");
  }

  dump_tree(t);
  printf("\n");

  return t;
}

static void do_match(char *string, char **from, char **to) {
  D_state_t *state;

  char *p;
  char *max_match;

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
