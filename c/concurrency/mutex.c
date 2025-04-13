#include <stdio.h>
#include <stdbool.h>

bool test_and_set(volatile bool *p);
void release_tas(volatile bool *p);

bool lock = false;

void mutex_func(void) {
retry:
  if (!test_and_set(&lock)) {
    fputs("Get Lock and Enter Critical Section\n", stdout);
  } else {
    goto retry;
  }

  release_tas(&lock);
}

bool test_and_set(volatile bool *p) {
  return __sync_lock_test_and_set(p, 1);
}

void release_tas(volatile bool *p) {
  return __sync_lock_release(p);
}
