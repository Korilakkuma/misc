#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#define INT long long

#define BARRIER_POINT     (INT)10
#define NUMBER_OF_THREADS (INT)10

volatile INT shared_count = 0;

pthread_mutex_t lock      = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  condition = PTHREAD_COND_INITIALIZER;

void barrier_synchronization(volatile INT *count, INT barrier_point);

void *thread_func(void *arg) {
  barrier_synchronization(&shared_count, BARRIER_POINT);

  INT id = (INT)arg;

  fprintf(stdout, "thread id[%lld]: shared count should be %lld (actual is %lld)\n", id, BARRIER_POINT, shared_count);

  if (BARRIER_POINT != shared_count) {
    fprintf(stderr, "assertion error (actual is %lld)\n", shared_count);
    exit(EXIT_FAILURE);
  }

  return NULL;
}

int main(int argc, char **argv) {
  pthread_t threads[NUMBER_OF_THREADS];

  for (INT n = 0; n < BARRIER_POINT; n++) {
    if (pthread_create(&threads[n], NULL, thread_func, (void *)n) != 0) {
      perror("pthread_create");
      exit(EXIT_FAILURE);
    }
  }

  for (INT n = 0; n < BARRIER_POINT; n++) {
    if (pthread_join(threads[n], NULL) != 0) {
      perror("pthread_join");
      exit(EXIT_FAILURE);
    }
  }

  return 0;
}

void barrier_synchronization(volatile INT *count, INT barrier_point) {
  if (pthread_mutex_lock(&lock) != 0) {
    perror("pthread_mutex_lock");
    exit(EXIT_FAILURE);
  }

  ++(*count);

  fprintf(stdout, "current shared count is be %lld\n", *count);

  if (*count == BARRIER_POINT) {
    if (pthread_cond_broadcast(&condition) != 0) {
      perror("pthread_cond_broadcast");
      exit(EXIT_FAILURE);
    }
  } else {
    do {
      if (pthread_cond_wait(&condition, &lock) != 0) {
        perror("pthread_cond_wait");
        exit(EXIT_FAILURE);
      }
    } while (*count < BARRIER_POINT);
  }

  if (pthread_mutex_unlock(&lock) != 0) {
    perror("pthread_mutex_unlock");
    exit(EXIT_FAILURE);
  }
}
