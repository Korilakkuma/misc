#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INT long long
#define MAX_NUMBER 1000

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./combination_value [natural number]", stderr);
    exit(EXIT_FAILURE);
  }

  INT n = strtoll(argv[1], NULL, 10);

  INT *a = (INT *)malloc(n * sizeof(INT));

  srand((unsigned int)time(NULL));

  for (INT i = 0; i < n; i++) {
    a[i] = (INT)(MAX_NUMBER * ((double)rand() / (RAND_MAX + 1.0)));
  }

  INT c = 0;

  INT m1 = 0;
  INT m2 = 0;
  INT m3 = 0;
  INT m4 = 0;
  INT m5 = 0;

  for (INT i = 0; i < (n - 4); i++) {
    for (INT j = i + 1; j < (n - 3); j++) {
      for (INT k = j + 1; k < (n - 2); k++) {
        for (INT l = k + 1; l < (n - 1); l++) {
          for (INT m = l + 1; m < n; m++) {
            m1 = a[i];
            m2 = a[j];
            m3 = a[k];
            m4 = a[l];
            m5 = a[m];

            INT sum = m1 + m2 + m3 + m4 + m5;

            if (sum == 1000) {
              c++;
            }
          }
        }
      }
    }
  }

  printf("Combination is %lld\n", c);

  free(a);

  return 0;
}
