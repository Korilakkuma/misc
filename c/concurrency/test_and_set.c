#include <stdbool.h>

bool test_and_set(bool *p) {
  if (*p) {
    return true;
  } else {
    *p = true;
    return false;
  }
}
