#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define INT long long

#define BARRIER_POINT     (INT)10
#define NUMBER_OF_THREADS (INT)10

volatile INT shared_count = 0;

void barrier_synchronization(volatile INT *count, INT barrier_point);

void *thread_func(void *arg) {
  barrier_synchronization(&shared_count, BARRIER_POINT);

  INT id = (INT)arg;

  fprintf(stdout, "thread id[%lld]: barrier synchronization\n", id);

  return NULL;
}

int main(int argc, char **argv) {
  pthread_t threads[BARRIER_POINT];

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
  __sync_fetch_and_add(count, 1);

  while (*count < barrier_point);
}
