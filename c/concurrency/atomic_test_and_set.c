#include <stdbool.h>

/**
 * x86-64
 *
 *    movb $1, %al      ; %al = 1
 *    xchb %al, (%rdi)  ; TAS
 *    andb $1, %al      ; %al = %al & 1
 *    retq
 **/

bool test_and_set(volatile bool *p) {
  return __sync_lock_test_and_set(p, 1);
}

void release_tas(volatile bool *p) {
  return __sync_lock_release(p);
}
