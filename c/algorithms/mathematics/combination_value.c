#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_NUMBER 1000

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./combination_value [natural number]", stderr);
    exit(EXIT_FAILURE);
  }

  long long n = strtol(argv[1], NULL, 10);

  long long *a = (long long *)malloc(n);

  srand((unsigned int)time(NULL));

  for (long long i = 0; i < n; i++) {
    a[i] = (long long)(MAX_NUMBER * ((double)rand() / RAND_MAX));
  }

  long long c = 0;

  long long m1 = 0;
  long long m2 = 0;
  long long m3 = 0;
  long long m4 = 0;
  long long m5 = 0;

  for (long long i = 0; i < (n - 4); i++) {
    for (long long j = i + 1; j < (n - 3); j++) {
      for (long long k = j + 1; k < (n - 2); k++) {
        for (long long l = k + 1; l < (n - 1); l++) {
          for (long long m = l + 1; m < n; m++) {
            m1 = a[i];
            m2 = a[j];
            m3 = a[k];
            m4 = a[l];
            m5 = a[m];

            long long sum = m1 + m2 + m3 + m4 + m5;

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
