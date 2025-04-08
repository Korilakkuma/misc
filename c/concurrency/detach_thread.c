#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define INT long

static void *thread_func(void *arg);

int main(int argc, char **argv) {
  pthread_attr_t attr;

  if (pthread_attr_init(&attr) != 0) {
    perror("pthread_attr_init");
    return EXIT_FAILURE;
  }

  if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != 0) {
    perror("pthread_attr_setdetachstate");
    return EXIT_FAILURE;
  }

  INT number_of_threads = sysconf(_SC_NPROCESSORS_CONF);

  pthread_t *threads = (pthread_t *)malloc((size_t)number_of_threads * sizeof(pthread_t));

  fprintf(stdout, "The number of threads (CPU cores) is %ld\n", number_of_threads);

  for (INT n = 0; n < number_of_threads; n++) {
    if (pthread_create(&threads[n], &attr, thread_func, (void *)n) != 0) {
      perror("pthread_create");
      return EXIT_FAILURE;
    }
  }

  if (pthread_attr_destroy(&attr) != 0) {
    perror("pthread_attr_destroy");
    return EXIT_FAILURE;
  }

  sleep(number_of_threads);

  fputs("All threads have been terminated\n", stdout);

  free(threads);

  return 0;
}

static void *thread_func(void *arg) {
  // pthread_detach(pthread_self());

  INT id = (INT)arg;

  for (INT n = 0; n < 3; n++) {
    fprintf(stdout, "thread id[%ld]: %ld\n", id, n);
  }

  return NULL;
}
