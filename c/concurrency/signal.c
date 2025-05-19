#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#define INT long long

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sigset_t signal_set;

void *signal_thread_func(void *arg) {
  pthread_detach(pthread_self());

  int sig;

  while (true) {
    if (sigwait(&signal_set, &sig) != 0) {
      perror("sigwait");
      exit(EXIT_FAILURE);
    }

    fprintf(stdout, "Received signal [%d]\n", sig);

    pthread_mutex_lock(&mutex);

    fputs("Critical section (Signal thread)\n", stdout);

    pthread_mutex_unlock(&mutex);
  }

  return NULL;
}

void *worker_thread_func(void *arg) {
  for (INT n = 0; n < 10; n++) {
    pthread_mutex_lock(&mutex);

    fputs("Critical section (Worker thread)\n", stdout);

    sleep(1);

    pthread_mutex_unlock(&mutex);

    sleep(1);
  }

  return NULL;
}

int main(int argc, char **argv) {
  pid_t pid = getpid();

  fprintf(stdout, "process id: %d\n", pid);

  sigemptyset(&signal_set);
  sigaddset(&signal_set, SIGINT);

  if (pthread_sigmask(SIG_BLOCK, &signal_set, NULL) != 0) {
    perror("pthread_sigmask");
    exit(EXIT_FAILURE);
  }

  pthread_t thread;
  pthread_t worker;

  if (pthread_create(&thread, NULL, signal_thread_func, (void *)1) != 0) {
    perror("pthread_create");
    exit(EXIT_FAILURE);
  }

  if (pthread_create(&worker, NULL, worker_thread_func, (void *)2) != 0) {
    perror("pthread_create");
    exit(EXIT_FAILURE);
  }

  if (pthread_join(worker, NULL) != 0) {
    perror("pthread_join");
    exit(EXIT_FAILURE);
  }

  return 0;
}
