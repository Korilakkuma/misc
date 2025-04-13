#include <stdio.h>
#include <stdbool.h>

bool test_and_set(volatile bool *p);
void release_tas(volatile bool *p);

bool lock = false;

void aqcuire_spinlock(volatile bool *lock) {
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

void mutex_func(void) {
  while (true) {
    aqcuire_spinlock(&lock);

    // Critical Section

    release_spinlock(&lock);
  }
}

bool test_and_set(volatile bool *p) {
  return __sync_lock_test_and_set(p, 1);
}

void release_tas(volatile bool *p) {
  return __sync_lock_release(p);
}
