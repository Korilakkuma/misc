#include "data.hpp"

static const INT MAX_STACK_SIZE = 10;

static void push(INT value);
static INT pop();
static bool is_empty();
static bool is_full();

static void dump_stack();

static INT stack[MAX_STACK_SIZE];
static INT stack_pointer = 0;

int main(int argc, char **argv) {
  std::string op;

  std::cout << "<-- stack pointer (empty)" << ::std::endl;
  std::cout << "(+|-|q) > ";

  while (std::cin && (std::cin >> op)) {
    if (op[0] == 'q') {
      break;
    }

    switch (op[0]) {
      case '+': {
        INT v = generate_random_value(10);

        push(v);

        std::cout << "push: " << v << std::endl;
        std::cout << std::endl;
        break;
      }

      case '-': {
        INT v = pop();

        std::cout << "pop: " << v << std::endl;
        std::cout << std::endl;
        break;
      }

      default: {
        break;
      }
    }

    dump_stack();

    std::cout << "(+|-|q) > ";
  }

  return 0;
}

static void push(INT value) {
  if (is_full()) {
    std::cerr << "Stack is full" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  stack[stack_pointer++] = value;
}

static INT pop() {
  if (is_empty()) {
    std::cerr << "Stack is empty" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  return stack[--stack_pointer];
}

static bool is_empty() {
  return stack_pointer == 0;
}

static bool is_full() {
  return stack_pointer == MAX_STACK_SIZE;
}

static void dump_stack() {
  if (is_empty()) {
    std::cout << "<-- stack pointer (empty)" << ::std::endl;
  } else if (is_full()) {
    std::cout << "<-- stack pointer (full)" << ::std::endl;
  } else {
    std::cout << "<-- stack pointer" << ::std::endl;
  }

  for (INT i = (stack_pointer - 1); i >= 0; i--) {
   std::cout << stack[i] << std::endl;
  }

  std::cout << std::endl;
}
