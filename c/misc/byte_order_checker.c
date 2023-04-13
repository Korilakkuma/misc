#include <stdio.h>

int main(int argc, char **argv) {
  long i = 0x88442211;

  unsigned char *c = (unsigned char *)&i;

  printf("%x:%x:%x:%x\n", c[0], c[1], c[2], c[3]);

  if (c[0] == 0x11) {
    fputs("little endian\n", stdout);
  } else {
    fputs("big endian\n", stdout);
  }

  fputc('\n', stdout);

  fwrite(&i, sizeof(i), (size_t)1, stdout);

  return 0;
}
