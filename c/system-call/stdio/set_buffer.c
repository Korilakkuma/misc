#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./set_buffer size (bytes)\n", stdout);
    exit(EXIT_FAILURE);
  }

  size_t size = (size_t)strtoull(argv[1], NULL, 10);

  char *buf = (char *)malloc(size * sizeof(char));

  if (buf == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  setbuf(stdin, buf);

  // fprintf(stdout, "Type > ");
  // fputs("Type > ");
  puts("Type > ");  // `puts` is line buffering output (`sputs` is not line buffering output)

  // fflush(stdout);

  scanf("%s", buf);  // Flush output buffer before line buffering input

  fprintf(stdout, "echo: %s\n", buf);

  free(buf);

  return 0;
}
