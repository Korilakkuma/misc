#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <pthread.h>

#define INT               long long
#define EMPTY_ID          (INT)-1
#define NUMBER_OF_THREADS 10

struct Reentrantlock {
  bool lock;
  INT id;
  INT count;
};

struct Reentrantlock lock;

static inline bool test_and_set(volatile bool *p) {
  return __sync_lock_test_and_set(p, 1);
}

static inline void release_tas(volatile bool *p) {
  return __sync_lock_release(p);
}

void acquire_spinlock(volatile bool *lock);
void release_spinlock(volatile bool *lock);

void acquire_reentrantlock(struct Reentrantlock *lock, INT id);
void release_reentrantlock(struct Reentrantlock *lock);

void reentrant_lock_thread(INT id, INT n) {
  if (n == 0) {
    return;
  }

  acquire_reentrantlock(&lock, id);

  fprintf(stdout, "Reentrant lock (Recursive lock) thread id[%lld]: count = %lld\n", id, n);
  reentrant_lock_thread(id, (n - 1));

  release_reentrantlock(&lock);
}

void *thread_func(void *arg) {
  INT id = (INT)arg;

  assert(id > 0);

  for (INT n = 0; n < 5; n++) {
    reentrant_lock_thread(id, 10);
  }

  return NULL;
}

int main(int argc, char **argv) {
  pthread_t threads[NUMBER_OF_THREADS];

  for (INT n = 0; n < NUMBER_OF_THREADS; n++) {
    if (pthread_create(&threads[n], NULL, thread_func, (void *)(n + 1)) != 0) {
      perror("pthread_create");
      exit(EXIT_FAILURE);
    }
  }

  for (INT n = 0; n < NUMBER_OF_THREADS; n++) {
    if (pthread_join(threads[n], NULL) != 0) {
      perror("pthread_join");
      exit(EXIT_FAILURE);
    }
  }

  return 0;
}

void acquire_reentrantlock(struct Reentrantlock *lock, INT id) {
  if (lock->lock && (lock->id == id)) {
    ++lock->count;
  } else {
    acquire_spinlock(&lock->lock);

    lock->id = id;

    ++lock->count;
  }
}

void release_reentrantlock(struct Reentrantlock *lock) {
  --lock->count;

  if (lock->count == 0) {
    lock->id = EMPTY_ID;

    release_spinlock(&lock->lock);
  }
};

void acquire_spinlock(volatile bool *lock) {
  // Spinlock (TTAS)
  while (true) {
    while (*lock);

    if (!test_and_set(lock)) {
      // fputs("Get Lock and Enter Critical Section\n", stdout);
      break;
    }
  }
}

void release_spinlock(volatile bool *lock) {
  release_tas(lock);
}
