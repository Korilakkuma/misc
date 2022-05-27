#include <iostream>
#include <omp.h>

int main() {
#pragma omp parallel
  {
#ifdef _OPENMP
    std::cout << "Thread [" + std::to_string(omp_get_thread_num()) + "]\n" << std::flush;
#else
    std::cout << "No Thread" << std::endl;
#endif
  }

  return 0;
}
