#include "data.hpp"

static void heap_sort(std::vector<INT> &heap);
static void heapify(std::vector<INT> &heap, INT from, INT to);
// static void downheap(std::vector<INT> &heap, INT from, INT to);

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "./heap_sort N" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  INT N = std::stoll(argv[1]);

  std::vector<INT> heap(N);

  init_list(heap);
  dump_list(heap, "init: ");

  heap_sort(heap);

  dump_list(heap, "sort: ");

  return 0;
}

static void heap_sort(std::vector<INT> &heap) {
  for (INT i = ((heap.size() / 2) - 1); i >= 0; i--) {
    heapify(heap, i, heap.size());
  }

  dump_list(heap, "heap: ");

  for (INT i = (heap.size() - 1); i > 0; i--) {
    INT tmp;

    tmp     = heap[0];
    heap[0] = heap[i];
    heap[i] = tmp;

    heapify(heap, 0, i);
  }
}

static void heapify(std::vector<INT> &heap, INT from, INT to) {
  INT child_index = (from * 2) + 1;

  if (child_index >= to) {
    return;
  }

  if ((child_index + 1) < to) {
    if (heap[child_index + 1] > heap[child_index]) {
      ++child_index;
    }
  }

  if (heap[child_index] <= heap[from]) {
    return;
  }

  INT tmp;

  tmp               = heap[child_index];
  heap[child_index] = heap[from];
  heap[from]        = tmp;

  heapify(heap, child_index, to);
}

/*
static void downheap(std::vector<INT> &heap, INT from, INT to) {
  INT tail = heap[from];

  INT index = from;

  while (index < (to / 2)) {
    INT child_index = (index * 2) + 1;

    if ((child_index + 1) < to) {
      if (heap[child_index + 1] > heap[child_index]) {
        ++child_index;
      }
    }

    if (heap[child_index] <= tail) {
      break;
    }

    heap[index] = heap[child_index];
    index = child_index;
  }

  heap[index] = tail;
}
*/
