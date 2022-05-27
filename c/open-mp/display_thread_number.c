#include <stdio.h>
#include <omp.h>

int main(void) {
#pragma omp parallel
  {
#ifdef _OPENMP
    printf("Thread [%d]\n", omp_get_thread_num());
#else
    printf("No Thread\n");
#endif
  }

  return 0;
}
