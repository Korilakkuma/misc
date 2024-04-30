#include "data.hpp"

static const INT STACK_SIZE = 100;

static inline void swap(INT &a, INT &b) {
  INT t;

  t = a;
  a = b;
  b = t;
}

static void quick_sort(std::vector<INT> &list, INT low, INT high);

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "./quick_sort N" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  INT N = std::stoll(argv[1]);

  std::vector<INT> list(N);

  init_list(list);

  std::cout << "0: ";
  dump_list(list);

  std::cout << std::endl;

  quick_sort(list, 0, (list.size() - 1));

  std::cout << std::endl;
  std::cout << N << ": ";
  dump_list(list);

  std::cout << std::endl;

  return 0;
}

static void quick_sort(std::vector<INT> &list, INT low, INT high) {
  INT stack_pointer = 0;

  INT lows[STACK_SIZE];
  INT highs[STACK_SIZE];

  lows[stack_pointer]  = low;
  highs[stack_pointer] = high;

  ++stack_pointer;

  while (stack_pointer > 0) {
    --stack_pointer;

    INT l = lows[stack_pointer];
    INT h = highs[stack_pointer];

    if (l >= h) {
      continue;
    }

    INT mut_l = l;
    INT mut_h = h;

    INT n = mut_h - mut_l;

    INT pivot = (INT)(n * ((double)std::rand() / (RAND_MAX + 1.0))) + mut_l;
    // INT pivot = mut_h;

    std::cout << "pivot: " << pivot << std::endl;

    swap(list[pivot], list[mut_h]);

    pivot = mut_h;

    while (true) {
      while (list[mut_l] < list[pivot]) {
        ++mut_l;
      }

      while ((mut_l < mut_h) && (list[mut_h] > list[pivot])) {
        --mut_h;
      }

      if (mut_l >= mut_h) {
        break;
      }

      swap(list[mut_l], list[mut_h]);

      ++mut_l;
      --mut_h;
    }

    swap(list[mut_l], list[pivot]);

    dump_list(list, (std::to_string(n) + ": "));

    if ((mut_l - l) < (h - mut_l)) {
      // POP (left -> right)
      // Therefore, PUSH (right -> left)
      lows[stack_pointer]  = mut_l + 1;
      highs[stack_pointer] = h;

      ++stack_pointer;

      lows[stack_pointer]  = l;
      highs[stack_pointer] = mut_l - 1;

      ++stack_pointer;
    } else {
      // POP (right -> left)
      // Therefore, PUSH (left -> right)
      lows[stack_pointer]  = l;
      highs[stack_pointer] = mut_l - 1;

      ++stack_pointer;

      lows[stack_pointer]  = mut_l + 1;
      highs[stack_pointer] = h;

      ++stack_pointer;
    }
  }
}
