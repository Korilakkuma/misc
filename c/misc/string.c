#include <stdio.h>

#define MAX_SIZE 256

int main(void) {
  char s[MAX_SIZE];

  while (fgets(s, MAX_SIZE, stdin) != NULL) {
    fputs(s, stdout);
  }

  return 0;
}
