#include <stdint.h>
#include <stdbool.h>

bool compare_and_swap(uint64_t *p, uint64_t cmp_value, uint64_t new_value) {
  return __sync_bool_compare_and_swap(p, cmp_value, new_value);
}
