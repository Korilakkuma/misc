#include <stdint.h>
#include <stdbool.h>

/**
 * x86-64
 *
 * ; %rsi: 2nd argument
 * ; %rdx: 3rd argument
 *
 *    movq %rsi, %rax             ; %rax = %rsi
 *    xorl %ecx, %ecx             ; %ecx = 0;
 *    lock cmpxchgq %rdx, (%rdi)  ; CAS
 *    sete %cl                    ; %cl = ZF flag
 *    movl %ecx, %eax             ; %eax = %ecx
 *    retq
 **/

bool compare_and_swap(uint64_t *p, uint64_t cmp_value, uint64_t new_value) {
  return __sync_bool_compare_and_swap(p, cmp_value, new_value);
}
