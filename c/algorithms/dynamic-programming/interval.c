#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define INT long long

#define MIN_COST  1
#define MAX_COST 10

static INT interval(INT *dp, INT **c, INT N);

static inline void init_dp(INT *dp, INT N);
static inline void update_by_min(INT *a, INT b);

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./interval N\n", stderr);
    exit(EXIT_FAILURE);
  }

  INT N = strtoll(argv[1], NULL, 10);

  INT *dp = (INT *)malloc((N + 1) * sizeof(INT));

  init_dp(dp, N);

  INT **c = (INT **)malloc((N + 1) * sizeof(INT));

  for (INT i = 0; i <= N; i++) {
    c[i] = (INT *)malloc((N + 1) * sizeof(INT));
  }

  srand((unsigned int)time(NULL));

  printf("cost table:\n");

  for (INT i = 0; i <= N; i++) {
    printf("%2lld :", i);

    for (INT j = 0; j <= N; j++) {
      c[i][j] = (INT)(((rand() / (RAND_MAX + 1.0)) * MAX_COST) + MIN_COST);

      printf(" %2lld", c[i][j]);
    }

    printf("\n");
  }

  printf("min cost = %lld\n", interval(dp, c, N));

  for (INT i = 0; i <= N; i++) {
    free(c[i]);
  }

  free(dp);
  free(c);

  return 0;
}

static INT interval(INT *dp, INT **c, INT N) {
  printf("dump dynamic programming table:\n");

  for (INT i = 0; i <= N; i++) {
    for (INT j = 0; j < i; j++) {
      update_by_min(&dp[i], (dp[j] + c[i][j]));

      printf("(%2lld,%2lld) = %2lld\n", i, j, dp[i]);
    }
  }

  return dp[N];
}

static inline void init_dp(INT *dp, INT N) {
  for (INT i = 0; i <= N; i++) {
    dp[i] = UINT32_MAX;
  }

  dp[0] = 0;
}

static inline void update_by_min(INT *a, INT b) {
  if (*a > b) {
    *a = b;
  }
}
