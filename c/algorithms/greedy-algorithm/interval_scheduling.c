#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define INT long long
#define MAX_TIME 10LL

typedef struct {
  INT start;
  INT end;
  bool selected;
} Interval;

static int comp(const void *a, const void *b);
static inline void dump(Interval *interval, INT N);

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./interval_scheduling N\n", stderr);
    exit(EXIT_FAILURE);
  }

  INT N = strtoll(argv[1], NULL, 10);

  Interval *intervals = (Interval *)malloc(N * sizeof(Interval));

  srand((unsigned int)time(NULL));

  for (INT i = 0; i < N; i++) {
    INT s = (INT)(MAX_TIME * ((double)rand() / (RAND_MAX + 1.0)));
    INT d = (INT)(MAX_TIME * ((double)rand() / (RAND_MAX + 1.0))) + 1LL;

    intervals[i].start    = s;
    intervals[i].end      = s + d;
    intervals[i].selected = false;
  }

  qsort((void *)intervals, (size_t)N, sizeof(Interval), comp);

  INT max = 0;
  INT end = 0;

  for (INT i = 0; i < N; i++) {
    if (end > intervals[i].start) {
      continue;
    }

    max++;

    intervals[i].selected = true;
    end = intervals[i].end;
  }

  printf("The maximum number of tasks = %lld\n", max);

  dump(intervals, N);

  free(intervals);

  return 0;
}

static int comp(const void *a, const void *b) {
  return ((Interval *)a)->end - ((Interval *)b)->end;
}

static inline void dump(Interval *interval, INT N) {
  for (INT i = 0; i < N; i++) {
    if (interval[i].selected) {
      printf("*%2lld [%2lld, %2lld ]\n",i, interval[i].start, interval[i].end);
    } else {
      printf(" %2lld [%2lld, %2lld ]\n",i, interval[i].start, interval[i].end);
    }
  }
}
