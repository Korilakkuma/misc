#include <stdio.h>

int main(void) {
  fprintf(stdout, "short alignment is %zu\n", __alignof__(short));
  fprintf(stdout, "int alignment is %zu\n", __alignof__(int));
  fprintf(stdout, "long alignment is %zu\n", __alignof__(long));
  fprintf(stdout, "long long alignment is %zu\n", __alignof__(long));

  return 0;
}
