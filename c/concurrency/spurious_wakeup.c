#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

pthread_mutex_t mutex    = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;

void signal_handler(int sig) {
  fprintf(stdout, "Received signal [%d]\n", sig);
}

int main(int argc, char **argv) {
  pid_t pid = getpid();

  fprintf(stdout, "process id: %d\n", pid);

  signal(SIGINT, signal_handler);

  pthread_mutex_lock(&mutex);

  if (pthread_cond_wait(&condition, &mutex) != 0) {
    perror("pthread_cond_wait");
    exit(EXIT_FAILURE);
  }

  fputs("Spurious Wakeup !\n", stdout);

  pthread_mutex_unlock(&mutex);

  return 0;
}
