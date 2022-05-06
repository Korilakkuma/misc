#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUMBER_OF_ITEMS 100
#define NOT_SELECTED -1

long long v[NUMBER_OF_ITEMS];
long long w[NUMBER_OF_ITEMS];
long long dp[NUMBER_OF_ITEMS][100000];  // array(item => array(weight => value))

static inline int max(int a, int b);

int main(int argc, char **argv) {
  if (argc != 3) {
    fputs("./knapsack_dynamic_programming [natural number] [natural number]", stderr);
    exit(EXIT_FAILURE);
  }

  long long N = strtoll(argv[1], NULL, 10);
  long long W = strtoll(argv[2], NULL, 10);

  srand((unsigned int)time(NULL));

  v[0] = 0;
  w[0] = 0;

  for (long long i = 1; i <= N; i++) {
    v[i] = (long long)(100 * ((double)rand() / RAND_MAX)) + 1;
    w[i] = (long long)(100 * ((double)rand() / RAND_MAX)) + 1;

    printf("(v, w) = (%lld, %lld)\n", v[i], w[i]);
  }

  dp[0][0] = 0;

  for (long long i = 1; i <= W; i++) {
    dp[0][i] = NOT_SELECTED;
  }

  for (long long i = 1; i <= N; i++) {
    for (long long j = 0; j <= W; j++) {
      if (j < w[i]) {
        dp[i][j] = dp[i - 1][j];
      } else {
        dp[i][j] = max(dp[i - 1][j], dp[i - 1][j - w[i]] + v[i]);
      }
    }
  }

  long long max_v = 0;

  for (long long i = 0; i <= W; i++) {
    max_v = max(max_v, dp[N][i]);
  }

  printf("Max value is %lld\n", max_v);

  return 0;
}

static inline int max(int a, int b) {
  return a >= b ? a : b;
}
