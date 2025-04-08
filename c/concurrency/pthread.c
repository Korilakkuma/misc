#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define INT long

static void *thread_func(void *arg);

int main(int argc, char **argv) {
  INT number_of_threads = sysconf(_SC_NPROCESSORS_CONF);

  pthread_t *threads = (pthread_t *)malloc((size_t)number_of_threads * sizeof(pthread_t));

  fprintf(stdout, "The number of threads (CPU cores) is %ld\n", number_of_threads);

  for (INT n = 0; n < number_of_threads; n++) {
    if (pthread_create(&threads[n], NULL, thread_func, (void *)n) != 0) {
      perror("pthread_create");
      return EXIT_FAILURE;
    }
  }

  for (INT n = 0; n < number_of_threads; n++) {
    char *p;

    if (pthread_join(threads[n], (void **)&p) != 0) {
      perror("pthread_join");
      return EXIT_FAILURE;
    }

    fprintf(stderr, "thread id[%ld] has been %s", n, p);
  }

  fputs("All threads have been terminated\n", stdout);

  free(threads);

  return 0;
}

static void *thread_func(void *arg) {
  INT id = (INT)arg;

  for (INT n = 0; n < 3; n++) {
    fprintf(stdout, "thread id[%ld]: %ld\n", id, n);
    sleep(1);
  }

  return "terminated\n";
}
