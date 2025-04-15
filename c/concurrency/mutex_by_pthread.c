#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define INT long long

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;  // or `pthread_mutex_init`

void *thread_func(void *arg) {
  if (pthread_mutex_lock(&lock) != 0) {
    perror("pthread_mutex_lock");
    exit(EXIT_FAILURE);
  }

  INT id = (INT)arg;

  fprintf(stdout, "thread id[%lld]: critical section\n", id);

  if (pthread_mutex_unlock(&lock) != 0) {
    perror("pthread_mutex_unlock");
    exit(EXIT_FAILURE);
  }

  return NULL;
}

int main(int argc, char **argv) {
  INT number_of_threads = sysconf(_SC_NPROCESSORS_CONF);

  pthread_t *threads = (pthread_t *)malloc((size_t)number_of_threads * sizeof(pthread_t));

  fprintf(stdout, "The number of threads (CPU cores) is %lld\n", number_of_threads);

  for (INT n = 0; n < number_of_threads; n++) {
    if (pthread_create(&threads[n], NULL, thread_func, (void *)n) != 0) {
      perror("pthread_create");
      return EXIT_FAILURE;
    }
  }

  for (INT n = 0; n < number_of_threads; n++) {
    if (pthread_join(threads[n], NULL) != 0) {
      perror("pthread_join");
      return EXIT_FAILURE;
    }
  }

  if (pthread_mutex_destroy(&lock) != 0) {
    perror("pthread_mutex_destroy");
    return EXIT_FAILURE;
  }

  return 0;
}
