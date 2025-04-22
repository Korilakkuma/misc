#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#define INT long long

bool lock = false;

INT number_of_readers = 0;
INT number_of_writers = 0;

volatile INT shared_count = 0;

static inline bool test_and_set(volatile bool *p) {
  return __sync_lock_test_and_set(p, 1);
}

static inline void release_tas(volatile bool *p) {
  return __sync_lock_release(p);
}

void acquire_spinlock(volatile bool *lock);
void release_spinlock(volatile bool *lock);

void acquire_rwlock_read(INT *number_of_readers, volatile INT *number_of_writers);
void acquire_rwlock_write(INT *number_of_readers, volatile INT *number_of_writers);
void release_rwlock_read(INT *number_of_readers);
void release_rwlock_write(INT *number_of_writers);

void *reader_thread_func(void *arg) {
  INT id = (INT)arg;

  acquire_rwlock_read(&number_of_readers, &number_of_writers);

  fprintf(stdout, "reader thread id[%lld]: read shared count = %lld\n", id, shared_count);

  release_rwlock_read(&number_of_readers);

  return NULL;
}

void *writer_thread_func(void *arg) {
  INT id = (INT)arg;

  fflush(stdout);

  acquire_rwlock_write(&number_of_readers, &number_of_writers);

  ++shared_count;

  fprintf(stdout, "writer thread id[%lld]: incremented shared count = %lld\n", id, shared_count);

  release_rwlock_write(&number_of_writers);

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

  return 0;
}

void acquire_rwlock_read(INT *number_of_readers, volatile INT *number_of_writers) {
  while (true) {
    while (*number_of_writers > 0);

    __sync_fetch_and_add(number_of_readers, 1);

    if (*number_of_writers == 0) {
      break;
    }

    __sync_fetch_and_sub(number_of_readers, 1);
  }
}

void acquire_rwlock_write(INT *number_of_readers, volatile INT *number_of_writers) {
  __sync_fetch_and_add(number_of_writers, 1);

  while (*number_of_readers > 0);

  acquire_spinlock(&lock);
}

void release_rwlock_read(INT *number_of_readers) {
  __sync_fetch_and_sub(number_of_readers, 1);
}

void release_rwlock_write(INT *number_of_writers) {
  release_spinlock(&lock);

  __sync_fetch_and_sub(number_of_writers, 1);
}

void acquire_spinlock(volatile bool *lock) {
  // Spinlock (TTAS)
  while (true) {
    while (*lock);

    if (!test_and_set(lock)) {
      fputs("Get Lock and Enter Critical Section\n", stdout);
      break;
    }
  }
}

void release_spinlock(volatile bool *lock) {
  release_tas(lock);
}
