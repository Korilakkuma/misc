#include "data.hpp"

struct Heap {
  std::vector<INT> heap;

  Heap() {}

  INT top() {
    if (heap.empty()) {
      return -1;
    }

    return heap[0];
  }

  void upheap(INT v) {
    heap.push_back(v);

    INT i = (INT)heap.size() - 1;

    while (i > 0) {
      INT parent = (i - 1) / 2;

      if (heap[parent] >= v) {
        break;
      }

      heap[i] = heap[parent];
      i = parent;
    }

    heap[i] = v;
  }

  void downheap() {
    if (heap.empty()) {
      return;
    }

    INT v = heap.back();

    heap.pop_back();

    INT i = 0;

    while (((i * 2) + 1) < (INT)heap.size()) {
      INT child_left  = (i * 2) + 1;
      INT child_right = (i * 2) + 2;


      if ((child_right < (INT)heap.size()) && (heap[child_right] > heap[child_left])) {
        child_left = child_right;
      }

      if (heap[child_left] <= v) {
        break;
      }

      heap[i] = heap[child_left];
      i = child_left;
    }

    heap[i] = v;
  }

  void dump() {
    for (INT i = 0; i < heap.size(); i++) {
      std::cout << heap[i] << " ";
    }

    std::cout << std::endl;
  }
};

int main(int argc, char **argv) {
  Heap *heap = new Heap();

  std::string op;

  while (std::cin && (std::cin >> op)) {
    if (op[0] == 'q') {
      break;
    }

    switch (op[0]) {
      case '+': {
        INT v = generate_random_value(10);

        std::cout << "insert into heap (" << v << ")" << std::endl;

        heap->upheap(v);
        break;
      }

      case '-': {
        std::cout << "delete from heap (" << heap->top() << ")" << std::endl;

        heap->downheap();
        break;
      }

      case 't': {
        std::cout << "heap top is " << heap->top() << std::endl;
        break;
      }
    }

    heap->dump();
  }

  delete heap;

  return 0;
}
