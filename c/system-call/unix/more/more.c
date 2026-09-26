#include <stdio.h>
#include <stdlib.h>

#define LENGTH_OF_PAGE 24
#define LENGTH_OF_LINE 512

static void more(FILE *fp);
static int print(FILE *fp);

int main(int argc, char **argv) {
  FILE *fp;

  if (argc == 1) {
    more(stdin);
  } else {
    while (--argc) {
      if ((fp = fopen(*(++argv), "r")) == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
      }

      more(fp);

      if (fclose(fp) == -1) {
        perror("fclose");
        exit(EXIT_FAILURE);
      }
    }
  }

  return 0;
}

static void more(FILE *fp) {
  char lines[LENGTH_OF_LINE];

  int number_of_lines = 0;
  int read_of_lines   = 0;

  FILE *tty;

  if ((tty = fopen("/dev/tty", "r")) == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  while (fgets(lines, sizeof(lines), fp) != NULL) {
    if (number_of_lines == LENGTH_OF_PAGE) {
      read_of_lines = print(tty);

      if (read_of_lines == 0) {
        return;
      }

      number_of_lines -= read_of_lines;
    }

    if (fputs(lines, stdout) == EOF) {
      exit(EXIT_FAILURE);
    }

    ++number_of_lines;
  }
}

static int print(FILE *fp) {
  int ch;

  fputs("\0337m more? \033m", stdout);

  while ((ch = fgetc(fp)) != EOF) {
    switch (ch) {
      case 'q': {
        return 0;
      }

      case ' ': {
        return LENGTH_OF_PAGE;
      }

      case '\n': {
        return 1;
      }
    }
  }

  return 0;
}
