#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 64

void process_word(const char *name);

int myname      = 0;
int not_myname  = 0;
int junk_chars  = 0;

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./easy_data_mining name\n", stderr);
    exit(EXIT_FAILURE);
  }

  int ch;

  const char *name = argv[1];

  assert(strlen(name) <= MAX_LENGTH);

  fputs("Type > ", stdout);

  while ((ch = getchar()) != EOF) {
    if (isalpha(ch)) {
      /*
      if (ungetc(ch, stdin) == EOF) {
        perror("ungetc");
        exit(EXIT_FAILURE);
      }
      */

      process_word(name);
    } else {
      ++junk_chars;
    }
  }

  fprintf(stdout, "my name %d, not my name %d, junk chars %d\n", myname, not_myname, junk_chars);

  return 0;
}

void process_word(const char *name) {
  int p = 0;

  int ch;

  char buf[MAX_LENGTH + 2];

  while ((ch = getchar()) != EOF) {
    if (!isalpha(ch)) {
      continue;
    }

    buf[p++] = ch;

    if (p == (sizeof(buf) - 1)) {
      while ((ch = getchar()) != EOF) {
        if (!isalpha(ch)) {
          if (ungetc(ch, stdin) == EOF) {
            perror("ungetc");
            exit(EXIT_FAILURE);
          }

          break;
        }

        goto wordgot;
      }
    } else {
      if (ungetc(ch, stdin) == EOF) {
        perror("ungetc");
        exit(EXIT_FAILURE);
      }

      goto wordgot;
    }
  }

wordgot:
  buf[p] = '\0';

  if (strcmp(buf, name)) {
    ++not_myname;
  } else {
    ++myname;
  }
}
