#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUMBER_OF_THREADS sysconf(_SC_NPROCESSORS_CONF)

static int func(void *arg) {
  int *p = (int *)arg;
  int n  = *p;

  printf("Thread [%d]\n", n);

  return 0;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "./pthread [1-%ld]\n", NUMBER_OF_THREADS);
    exit(EXIT_FAILURE);
  }

  int n = (int)strtol(argv[1], NULL, 10);

  if (n < 1) {
    fprintf(stderr, "./pthread [1-%ld]\n", NUMBER_OF_THREADS);
    exit(EXIT_FAILURE);
  }

  pthread_t *threads = (pthread_t *)malloc(n * sizeof(pthread_t));
  int *args = (int *)malloc(n * sizeof(int));

  for (int i = 0; i < n; i++) {
    args[i] = i;
    pthread_create(&threads[i], NULL, (void *)&func, (void *)&args[i]);
  }

  for (int i = 0; i < n; i++) {
    pthread_join(threads[i], NULL);
  }

  free(threads);
  free(args);

  return 0;
}
