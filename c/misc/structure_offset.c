#include <stdio.h>
#include <stddef.h>

struct Int {
  short s;
  int i;
  long l;
  long long ll;
};

int main(void) {
  fprintf(stdout, "Alignment is %zu (s) %zu (i) %zu (l) %zu (ll)\n", offsetof(struct Int, s), offsetof(struct Int, i), offsetof(struct Int, l), offsetof(struct Int, ll));

  return 0;
}
