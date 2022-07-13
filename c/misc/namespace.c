struct main {
  int main;
};

int main(int argc, char **argv) {
  struct main main;

  main.main = 0;

  if (main.main == 0) {
    goto main;
  }

main:
  return 0;
};
