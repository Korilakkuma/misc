#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

struct termios stored_term;
struct termios term_cursor;

void restore_term(void);
void ls(void);
void pwd(void);
void up(void);
void down(void);

int main(void) {
  tcgetattr(STDIN_FILENO, &stored_term);

  term_cursor = stored_term;

  if (atexit(restore_term) == -1) {
    perror("atexit");
    exit(EXIT_FAILURE);
  }

  term_cursor.c_lflag    &= ~(ICANON | ECHO);
  term_cursor.c_cc[VMIN]  = 1;
  term_cursor.c_cc[VTIME] = 0;

  tcsetattr(STDIN_FILENO, TCSANOW, &term_cursor);

  setbuf(stdin, NULL);

  int ch;

  while (1) {
    ch = getchar();

    switch (ch) {
      case 'l': {
        ls();
        break;
      }

      case 'u': {
        up();
        break;
      }

      case 'd': {
        down();
        break;
      }

      case '?': {
        pwd();
        break;
      }

      case 'q': {
        exit(EXIT_SUCCESS);
        break;
      }

      default: {
        fprintf(stderr, "Unknown command '%c'\n", ch);
        break;
      }
    }
  }

  return 0;
}

void restore_term(void) {
  tcsetattr(STDIN_FILENO, TCSANOW, &stored_term);
}

void ls(void) {
  DIR *dp;
  struct dirent *entry;

  if  ((dp = opendir(".")) == NULL) {
    perror("opendir");
    return;
  }

  errno = 0;

  while ((entry = readdir(dp)) != NULL) {
    fprintf(stdout, "%s ", entry->d_name);
  }

  if (errno != 0) {
    perror("readdir");
    return;
  }

  putchar('\n');

  if (closedir(dp) == -1) {
    perror("closedir");
    return;
  }
}

void pwd(void) {
  char pathname[PATH_MAX + 1];

  if (getcwd(pathname, sizeof(pathname)) == NULL) {
    perror("getcwd");
    return;
  }

  fprintf(stdout, "%s\n", pathname);
}

void up(void) {
  if (chdir("../") == -1) {
    perror("chdir");
    return;
  }
}

void down(void) {
  char dirname[PATH_MAX + 1];
  char *p;

  tcsetattr(STDIN_FILENO, TCSANOW, &stored_term);

  fflush(NULL);
  fputs("Type Directory > ", stdout);

  fgets(dirname, sizeof(dirname), stdin);

  if ((p = strchr(dirname, '\n')) != NULL) {
    *p = '\0';
  }

  if (chdir(dirname) == -1) {
    perror("chdir");
    return;
  }

  tcsetattr(STDIN_FILENO, TCSANOW, &term_cursor);
}
