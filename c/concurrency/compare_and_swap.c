#include <stdint.h>
#include <stdbool.h>

/**
 * x86-64
 *
 * ; %rsi: 2nd argument
 * ; %rdx: 3rd argument
 *
 *    cmpq %rsi, (%rdi)  ; %rsi == (%rdi)
 *    jne  LBB0_1        ; if %rsi != (%rdi) then goto LBB0_1
 *    movq %rdx, (%rdi)  ; (%rdi) 0 %rdx
 *    movl $1, %eax      ; %eax = 1
 *    retq
 *  LBB0_1:
 *    xorl %eax, %eax    ; %eax = 0;
 *    retq
 **/

bool compare_and_swap(uint64_t *p, uint64_t cmp_value, uint64_t new_value) {
  if (*p != cmp_value) {
    return false;
  }

  *p = new_value;

  return true;
}
