#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INT long long

#define MAX_WEIGHT 10
#define MAX_VALUE  100

static INT knapsack_problem(INT **dp, INT *w, INT *v, INT N, INT W);

static inline void init_dp(INT **dp, INT N, INT W);
static inline void update_by_max(INT *a, INT b);
static inline void dump(INT *a, INT n, char *l);

int main(int argc, char **argv) {
  if (argc != 3) {
    fputs("./knapsack_problem N W\n", stderr);
    exit(EXIT_FAILURE);
  }

  INT N = strtoll(argv[1], NULL, 10);
  INT W = strtoll(argv[2], NULL, 10);

  INT **dp = (INT **)malloc((N + 1) * sizeof(INT));

  init_dp(dp, N, W);

  INT *w = (INT *)malloc(N * sizeof(INT));
  INT *v = (INT *)malloc(N * sizeof(INT));

  srand((unsigned int)time(NULL));

  for (INT i = 0; i < N; i++) {
    w[i] = (INT)((rand() / (RAND_MAX + 1.0) * MAX_WEIGHT) + 1);
    v[i] = (INT)((rand() / (RAND_MAX + 1.0) * MAX_VALUE));
  }

  dump(w, N, "weight:");
  dump(v, N, "value :");

  printf("max value = %lld\n", knapsack_problem(dp, w, v, N, W));

  free(w);
  free(v);

  for (INT i = 0; i <= N; i++) {
    free(dp[i]);
  }

  free(dp);

  return 0;
}

static INT knapsack_problem(INT **dp, INT *w, INT *v, INT N, INT W) {
  printf("dump dynamic programming table:\n");

  for (INT i = 0; i < N; i++) {
    printf("%lld:", i);

    for (INT j = 0; j <= W; j++) {
      if ((j - w[i]) >= 0) {
        update_by_max(&dp[i + 1][j], (dp[i][j - w[i]] + v[i]));
      }

      update_by_max(&dp[i + 1][j], dp[i][j]);

      printf(" %3lld", dp[i][j]);
    }

    printf("\n");
  }

  printf("%lld:", N);

  dump(dp[N], (W + 1), "");

  return dp[N][W];
}

static inline void init_dp(INT **dp, INT N, INT W) {
  for (INT i = 0; i <= N; i++) {
    dp[i] = (INT *)malloc((W + 1) * sizeof(INT));
  }

  for (INT i = 0; i < N; i++) {
    for (INT j = 0; j <= W; j++) {
      dp[i][j] = 0;
    }
  }
}

static inline void update_by_max(INT *a, INT b) {
  if (*a < b) {
    *a = b;
  }
}

static inline void dump(INT *a, INT n, char *l) {
  printf("%s", l);

  for (INT i = 0; i < n; i++) {
    printf(" %3lld", a[i]);
  }

  printf("\n");
}
