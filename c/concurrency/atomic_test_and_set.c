#include <stdbool.h>

bool test_and_set(bool *p) {
  return __sync_lock_test_and_set(p, 1);
}
