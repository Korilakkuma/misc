#include <stdio.h>
#include <stdlib.h>

static long long permutation(long long n, long long r);
static long long combination(long long n, long long r);

int main(int argc, char **argv) {
  if (argc != 3) {
    fputs("./combination [natural number] [natural number]", stderr);
    exit(EXIT_FAILURE);
  }

  long long n = strtol(argv[1], NULL, 10);
  long long r = strtol(argv[2], NULL, 10);

  long long c = combination(((n > r) ? n : r), ((r < n) ? r : n));

  if (n > r) {
    printf("%lld_C_%lld = %lld\n", n, r, c);
  } else {
    printf("%lld_C_%lld = %lld\n", r, n, c);
  }

  return 0;
}

static long long permutation(long long n, long long r) {
  long long p = 1;

  for (long long i = n, nr = (n - r); i > nr; i--) {
    p *= i;
  }

  return p;
}

static long long combination(long long n, long long r) {
  return permutation(n, r) / permutation(r, r);
}
