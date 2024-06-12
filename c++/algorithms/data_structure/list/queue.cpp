#include "data.hpp"

static const INT MAX_QUEUE_SIZE = 10;

static void enqueue(INT value);
static INT dequeue();
static bool is_empty();
static bool is_full();

static void dump_queue();

static INT queue[MAX_QUEUE_SIZE];
static INT rear  = 0;
static INT front = 0;

int main(int argc, char **argv) {
  std::string op;

  std::cout << "<-- rear front (empty)" << ::std::endl;
  std::cout << "(+|-|q) > ";

  while (std::cin && (std::cin >> op)) {
    if (op[0] == 'q') {
      break;
    }

    switch (op[0]) {
      case '+': {
        INT v = generate_random_value(10);

        enqueue(v);

        std::cout << "enqueue: " << v << std::endl;
        std::cout << std::endl;
        break;
      }

      case '-': {
        INT v = dequeue();

        std::cout << "dequeue: " << v << std::endl;
        std::cout << std::endl;
        break;
      }

      default: {
        break;
      }
    }

    dump_queue();

    std::cout << "(+|-|q) > ";
  }

  return 0;
}

static void enqueue(INT value) {
  if (is_full()) {
    std::cerr << "Queue is full" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  queue[rear++] = value;

  if (rear >= MAX_QUEUE_SIZE) {
    rear = 0;
  }
}

static INT dequeue() {
  if (is_empty()) {
    std::cerr << "Queue is empty" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  INT v = queue[front++];

  if (front >= MAX_QUEUE_SIZE) {
    front = 0;
  }

  return v;
}

static bool is_empty() {
  return rear == front;
}

static bool is_full() {
  return ((rear + 1) % MAX_QUEUE_SIZE) == front;
}

static void dump_queue() {
  std::cout << std::endl;

  for (INT i = 0; i < MAX_QUEUE_SIZE; i++) {
    if (is_empty()) {
      std::cout << "<-- rear front (empty)" << ::std::endl;
      break;
    }

    if (i == rear) {
      if (is_full()) {
        std::cout << queue[i] << "<-- rear (full)" << std::endl;
      } else {
        std::cout << queue[i] << "<-- rear" << std::endl;
      }
    }

    if (i == front) {
      std::cout << queue[i] << "<-- front" << std::endl;
    }

    if ((i != rear) && (i != front)) {
      std::cout << queue[i] << std::endl;
    }
  }

  std::cout << std::endl;
}
