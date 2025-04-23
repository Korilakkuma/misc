#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define INT long long

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;  // or pthread_rwlock_init

volatile INT shared_count = 0;

void *reader_thread_func(void *arg) {
  if (pthread_rwlock_rdlock(&rwlock) != 0) {
    perror("pthread_rwlock_rdlock");
    exit(EXIT_FAILURE);
  }

  INT id = (INT)arg;

  fprintf(stdout, "reader thread id[%lld]: read shared count = %lld\n", id, shared_count);

  if (pthread_rwlock_unlock(&rwlock) != 0) {
    perror("pthread_rwlock_unlock");
    exit(EXIT_FAILURE);
  }

  return NULL;
}

void *writer_thread_func(void *arg) {
  if (pthread_rwlock_wrlock(&rwlock) != 0) {
    perror("pthread_rwlock_wrlock");
    exit(EXIT_FAILURE);
  }

  INT id = (INT)arg;

  ++shared_count;

  fprintf(stdout, "writer thread id[%lld]: incremented shared count = %lld\n", id, shared_count);

  if (pthread_rwlock_unlock(&rwlock) != 0) {
    perror("pthread_rwlock_unlock");
    exit(EXIT_FAILURE);
  }

  return NULL;
}

int main(int argc, char **argv) {
  pthread_t reader_0;
  pthread_t reader_1;
  pthread_t writer;

  INT reader_0_id = 0;
  INT reader_1_id = 1;
  INT writer_id   = 3;

  if (pthread_create(&reader_0, NULL, reader_thread_func, (void *)reader_0_id) != 0) {
    perror("pthread_create");
    exit(EXIT_FAILURE);
  }

  if (pthread_create(&reader_1, NULL, reader_thread_func, (void *)reader_1_id) != 0) {
    perror("pthread_create");
    exit(EXIT_FAILURE);
  }

  if (pthread_create(&writer, NULL, writer_thread_func, (void *)writer_id) != 0) {
    perror("pthread_create");
    exit(EXIT_FAILURE);
  }

  if (pthread_join(reader_0, NULL) != 0) {
    perror("pthread_join");
    exit(EXIT_FAILURE);
  }

  if (pthread_join(reader_1, NULL) != 0) {
    perror("pthread_join");
    exit(EXIT_FAILURE);
  }

  if (pthread_join(writer, NULL) != 0) {
    perror("pthread_join");
    exit(EXIT_FAILURE);
  }

  if (pthread_rwlock_destroy(&rwlock) != 0) {
    perror("pthread_rwlock_destroy");
    exit(EXIT_FAILURE);
  }

  return 0;
}
