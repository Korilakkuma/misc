#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <term.h>

int main(int argc, char **argv) {
  if (argc != 3) {
    fputs("./reverse_cat pattern filename\n", stderr);
    exit(EXIT_FAILURE);
  }

  FILE *fp;

  char buf[1024];

  char *rev;
  char *sgr0;

  char *pattern  = argv[1];
  char *filename = argv[2];

  setupterm(NULL, 0, NULL);

  rev  = strdup(tigetstr("rev"));
  sgr0 = strdup(tigetstr("sgr0"));

  if ((fp = fopen(filename, "r")) == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  while (1) {
    char *p = buf;
    char *q;

    if (fgets(buf, sizeof(buf), fp) == NULL) {
      break;
    }

    while (1) {
      q = strstr(p, pattern);

      if (q == NULL) {
        fputs(p, stdout);
        break;
      }

      while (p != q) {
        fputc((*p++), stdout);
      }

      putp(rev);

      fprintf(stdout, "%s", pattern);

      putp(sgr0);

      p += strlen(pattern);
    }
  }

  free(rev);
  free(sgr0);

  if (fclose(fp) == -1) {
    perror("fclose");
    exit(EXIT_FAILURE);
  }

  return 0;
}
