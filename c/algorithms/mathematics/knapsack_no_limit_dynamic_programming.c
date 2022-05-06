#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INT long long
#define NUMBER_OF_ITEMS 1000
#define NOT_SELECTED -1

INT v[NUMBER_OF_ITEMS];
INT w[NUMBER_OF_ITEMS];

int main(int argc, char **argv) {
  if (argc != 3) {
    fputs("./knapsack_dynamic_programming [natural number] [natural number]", stderr);
    exit(EXIT_FAILURE);
  }

  INT N = strtoll(argv[1], NULL, 10);
  INT W = strtoll(argv[2], NULL, 10);

  srand((unsigned int)time(NULL));

  printf("Items:\n");

  for (INT i = 0; i < N; i++) {
    v[i] = (INT)(10 * ((double)rand() / (RAND_MAX + 1.0))) + 1;
    w[i] = (INT)(10 * ((double)rand() / (RAND_MAX + 1.0))) + 1;

    printf("(v, w) = (%lld, %lld)\n", v[i], w[i]);
  }

  INT c[NUMBER_OF_ITEMS];
  INT d[NUMBER_OF_ITEMS];

  for (INT i = 0; i < NUMBER_OF_ITEMS; i++) {
    c[i] = 0;
    d[i] = NOT_SELECTED;
  }

  for (INT i = 0; i < N; i++) {
    for (INT j = w[i]; j <= W; j++) {
      INT p = j - w[i];
      INT t = c[p] + v[i];

      if (t > c[j]) {
        c[j] = t;
        d[j] = i;
      }
    }
  }

  printf("Selected Items:\n");

  for (INT i = W; d[i] > NOT_SELECTED; i -= w[d[i]]) {
    printf("(v, w) = (%lld, %lld)\n", v[d[i]], w[d[i]]);
  }

  printf("Max value = %lld\n", c[W]);

  return 0;
}
