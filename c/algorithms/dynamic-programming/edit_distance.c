#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define INT long long

static INT edit_distance(INT **dp, char *S, char *T, INT S_LENGTH, INT T_LENGTH);

static inline void init_dp(INT **dp, INT S_LENGTH, INT T_LENGTH);
static inline void update_by_min(INT *a, INT b);
static inline void dump(INT *a, INT l, INT n);

int main(int argc, char **argv) {
  if (argc != 3) {
    fputs("./edit_distance S T\n", stderr);
    exit(EXIT_FAILURE);
  }

  char *S = argv[1];
  char *T = argv[2];

  INT S_LENGTH = (INT)strlen(S);
  INT T_LENGTH = (INT)strlen(T);

  INT **dp = (INT **)malloc((S_LENGTH + 1) * sizeof(INT));

  init_dp(dp, S_LENGTH, T_LENGTH);

  printf("diff = %lld\n", edit_distance(dp, S, T, S_LENGTH, T_LENGTH));

  for (INT i = 0; i <= S_LENGTH; i++) {
    free(dp[i]);
  }

  free(dp);

  return 0;
}

static INT edit_distance(INT **dp, char *S, char *T, INT S_LENGTH, INT T_LENGTH) {
  printf("dump dynamic programming table:\n");

  for (INT i = 0; i <= S_LENGTH; i++) {
    for (INT j = 0; j <= T_LENGTH; j++) {
      if ((i > 0) && (j > 0)) {
        if (S[i - 1] == T[j - 1]) {
          update_by_min(&dp[i][j], dp[i - 1][j - 1]);
        } else {
          update_by_min(&dp[i][j], (dp[i - 1][j - 1] + 1));
        }
      }

      if (i > 0) {
        update_by_min(&dp[i][j], (dp[i - 1][j] + 1));
      }

      if (j > 0) {
        update_by_min(&dp[i][j], (dp[i][j - 1] + 1));
      }
    }

    dump(dp[i], i, (T_LENGTH + 1));
  }

  return dp[S_LENGTH][T_LENGTH];
}

static inline void init_dp(INT **dp, INT S_LENGTH, INT T_LENGTH) {
  for (INT i = 0; i <= S_LENGTH; i++) {
    dp[i] = (INT *)malloc((T_LENGTH + 1) * sizeof(INT));
  }

  for (INT i = 0; i <= S_LENGTH; i++) {
    for (INT j = 0; j <= T_LENGTH; j++) {
      dp[i][j] = UINT32_MAX;
    }
  }

  dp[0][0] = 0;
}

static inline void update_by_min(INT *a, INT b) {
  if (*a > b) {
    *a = b;
  }
}

static inline void dump(INT *a, INT l, INT n) {
  printf("%2lld: ", l);

  for (INT i = 0; i < n; i++) {
    printf(" %2lld", a[i]);
  }

  printf("\n");
}
