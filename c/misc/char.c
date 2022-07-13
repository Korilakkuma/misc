#include<stdio.h>
#include<ctype.h>

int main(void) {
  int c;

  while ((c = fgetc(stdin)) != EOF) {
    fputc(toupper(c), stdout);
  }

  return 0;
}
