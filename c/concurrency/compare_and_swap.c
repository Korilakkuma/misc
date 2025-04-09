#include <stdint.h>
#include <stdbool.h>

bool compare_and_swap(uint64_t *p, uint64_t cmp_value, uint64_t new_value) {
  if (*p != cmp_value) {
    return false;
  }

  *p = new_value;

  return true;
}
