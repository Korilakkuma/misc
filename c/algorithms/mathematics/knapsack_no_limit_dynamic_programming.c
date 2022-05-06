#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUMBER_OF_ITEMS 1000
#define NOT_SELECTED -1

long long v[NUMBER_OF_ITEMS];
long long w[NUMBER_OF_ITEMS];

int main(int argc, char **argv) {
  if (argc != 3) {
    fputs("./knapsack_dynamic_programming [natural number] [natural number]", stderr);
    exit(EXIT_FAILURE);
  }

  long long N = strtoll(argv[1], NULL, 10);
  long long W = strtoll(argv[2], NULL, 10);

  srand((unsigned int)time(NULL));

  printf("Items:\n");

  for (long long i = 0; i < N; i++) {
    v[i] = (long long)(10 * ((double)rand() / RAND_MAX)) + 1;
    w[i] = (long long)(10 * ((double)rand() / RAND_MAX)) + 1;

    printf("(v, w) = (%lld, %lld)\n", v[i], w[i]);
  }

  long long c[NUMBER_OF_ITEMS];
  long long d[NUMBER_OF_ITEMS];

  for (long long i = 0; i < NUMBER_OF_ITEMS; i++) {
    c[i] = 0;
    d[i] = NOT_SELECTED;
  }

  for (long long i = 0; i < N; i++) {
    for (long long j = w[i]; j <= W; j++) {
      long long p = j - w[i];
      long long t = c[p] + v[i];

      if (t > c[j]) {
        c[j] = t;
        d[j] = i;
      }
    }
  }

  printf("Selected Items:\n");

  for (long long i = W; d[i] > NOT_SELECTED; i -= w[d[i]]) {
    printf("(v, w) = (%lld, %lld)\n", v[d[i]], w[d[i]]);
  }

  printf("Max value = %lld\n", c[W]);

  return 0;
}
