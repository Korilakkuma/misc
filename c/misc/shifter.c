#include <stdio.h>
#include <stdlib.h>

typedef enum {
  LEFT,
  RIGHT
} Direction;

int main(int argc, char **argv) {
  if (argc != 5) {
    fputs("./shifter [l|r] shift [8|16|32|64] int\n", stderr);
    exit(EXIT_FAILURE);
  }

  Direction d     = argv[1][0] == 'r' ? RIGHT : LEFT;
  unsigned long s = strtoul(argv[2], NULL, 10);
  unsigned long w = strtoul(argv[3], NULL, 10);
  unsigned long v = strtoul(argv[4], NULL, 10);

  unsigned long r = v;

  const char *t;

  switch (w) {
    case 8: {
      t = "uint8";

      switch (d) {
        case LEFT: {
          r = (v << s) & 0xff;
          break;
        }
        case RIGHT: {
          r = (v >> s) & 0xff;
          break;
        }
        default:
          break;
      }

      break;
    }

    case 16: {
      t = "uint16";

      switch (d) {
        case LEFT: {
          r = (v << s) & 0xffff;
          break;
        }
        case RIGHT: {
          r = (v >> s) & 0xffff;
          break;
        }
        default:
          break;
      }

      break;
    }

    case 32: {
      t = "uint32";

      switch (d) {
        case LEFT: {
          r = (v << s) & 0xffffffff;
          break;
        }
        case RIGHT: {
          r = (v >> s) & 0xffffffff;
          break;
        }
        default:
          break;
      }

      break;
    }

    case 64: {
      t = "uint64";

      switch (d) {
        case LEFT: {
          r = (v << s) & 0xffffffffffffffff;
          break;
        }
        case RIGHT: {
          r = (v >> s) & 0xffffffffffffffff;
          break;
        }
        default:
          break;
      }

      break;
    }
  }

  printf("%s_t %ld (%ld %s %ld)\n", t, r, v, (argv[1][0] == 'r' ? ">>" : "<<"), s);

  return 0;
}
