#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./set_buffering_type Unbuffered|Line-buffered|Fully-buffered\n", stderr);
    exit(EXIT_FAILURE);
  }

  char *mode = argv[1];

  char *buf = (char *)malloc(BUFSIZ * sizeof(char));

  if (buf == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  fprintf(stdout, "buffering type is %c\n", mode[0]);

  switch (mode[0]) {
    case 'U': {
      setvbuf(stdin, buf, _IONBF, BUFSIZ);
      break;
    }

    case 'L': {
      setvbuf(stdin, buf, _IOLBF, BUFSIZ);
      break;
    }

    case 'F': {
      setvbuf(stdin, buf, _IOFBF, BUFSIZ);
      break;
    }

    default: {
      fputs("./set_buffering_type Unbuffered|Line-buffered|Fully-buffered\n", stderr);
      exit(EXIT_FAILURE);
    }
  }

  puts("Type > ");  // `puts` is line buffering output (`sputs` is not line buffering output)

  // fflush(stdout);

  scanf("%s", buf);  // Flush output buffer before line buffering input

  fprintf(stdout, "echo: %s\n", buf);

  free(buf);

  return 0;
}
