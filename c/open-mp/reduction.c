#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./reduction [natural number]\n", stderr);
    exit(EXIT_FAILURE);
  }

  long long n = strtoll(argv[1], NULL, 10);
  long long a = 0;
  long long i;

  struct timespec start_time;
  struct timespec end_time;

  clock_t start_clock;
  clock_t end_clock;

  clock_gettime(CLOCK_REALTIME, &start_time);

  start_clock = clock();

#pragma omp parallel for reduction (+:a)
  {
    for (i = 0; i < n; i++){
      a++;
    }
  }

  clock_gettime(CLOCK_REALTIME, &end_time);

  end_clock = clock();

  time_t sec = end_time.tv_sec - start_time.tv_sec;
  long nsec  = end_time.tv_nsec - start_time.tv_nsec;

  clock_t nc = end_clock - start_clock;

  double t  = (double)sec + ((double)nsec / powl(1000, 3));
  double ct = (double)nc / CLOCKS_PER_SEC;

  printf("%lld [%f sec] [clock_t %ld] [%f clock time (sec)]\n", a, t, nc, ct);

  return 0;
}
