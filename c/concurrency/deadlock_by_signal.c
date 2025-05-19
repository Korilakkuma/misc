#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void signal_handler(int sig) {
  fputs("Deadlock occurred !\n", stderr);
  pthread_mutex_lock(&mutex);
  fputs("Cannot be executed !\n", stderr);
  pthread_mutex_unlock(&mutex);
  exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
  signal(SIGINT, signal_handler);

  pthread_mutex_lock(&mutex);

  sleep(5);
  // Deadlock occur if received signal

  pthread_mutex_unlock(&mutex);

  return 0;
}
