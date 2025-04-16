#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define INT long long

#define NUMBER_OF_THREADS 10
#define NUMBER_OF_LOOPS   10

INT shared_count = 0;

const char *semaphore_name = "/tmp_semaphore";

void *thread_func(void *arg) {
  sem_t *s = sem_open(semaphore_name, 0);

  if (s == SEM_FAILED) {
    perror("sem_open");
    exit(EXIT_FAILURE);
  }

  INT id = (INT)arg;

  for (INT n = 0; n < NUMBER_OF_LOOPS; n++) {
    if (sem_wait(s) == -1) {
      perror("sem_wait");
      exit(EXIT_FAILURE);
    }

    __sync_fetch_and_add(&shared_count, 1);

    fprintf(stdout, "thread id[%lld]: shared count = %lld\n", id, shared_count);

    usleep(10 * 1000);  // 10 msec

    __sync_fetch_and_sub(&shared_count, 1);

    if (sem_post(s) == -1) {
      perror("sem_post");
      exit(EXIT_FAILURE);
    }
  }

  if (sem_close(s) == -1) {
    perror("sem_close");
  }

  return NULL;
}

int main(int argc, char **argv) {
  sem_t *s = sem_open(semaphore_name, O_CREAT, 0066, 3);

  if (s == SEM_FAILED) {
    perror("sem_open");
    return EXIT_FAILURE;
  }

  INT number_of_threads = sysconf(_SC_NPROCESSORS_CONF);

  pthread_t *threads = (pthread_t *)malloc((size_t)number_of_threads * sizeof(pthread_t));

  fprintf(stdout, "The number of threads (CPU cores) is %lld\n", number_of_threads);

  for (INT n = 0; n < number_of_threads; n++) {
    if (pthread_create(&threads[n], NULL, thread_func, (void *)n) != 0) {
      perror("pthread_create");
      goto UNLINK;
    }
  }

  for (INT n = 0; n < number_of_threads; n++) {
    if (pthread_join(threads[n], NULL) != 0) {
      perror("pthread_join");
      goto UNLINK;
    }
  }

  if (sem_close(s) == -1) {
    perror("sem_close");
    goto UNLINK;
  }

UNLINK:
  if (sem_unlink(semaphore_name) == -1) {
    perror("sem_unlink");
    return EXIT_FAILURE;
  }

  return 0;
}
