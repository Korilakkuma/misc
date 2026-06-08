#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fputs("./execute_program_as_process program ...\n", stderr);
    exit(EXIT_FAILURE);
  }


  char *const *envp = NULL;

  char *program = argv[1];

  fprintf(stdout, "Execute %s.\n", program);
  fflush(NULL);

  execve(program, &argv[1], envp);

  perror("execve");

  fputs("Not print if `execve` is success", stdout);

  return 0;
}
