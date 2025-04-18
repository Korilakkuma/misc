#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#define INT long long

pthread_mutex_t lock      = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  condition = PTHREAD_COND_INITIALIZER;

volatile bool ready = false;

char buf[256];

void *produce(void *arg);
void *consume(void *arg);

int main(int argc, char **argv) {
  pthread_t producer;
  pthread_t consumer;

  if (pthread_create(&producer, NULL, produce, (void *)"producer") != 0) {
    perror("pthread_create");
    return EXIT_FAILURE;
  }

  if (pthread_create(&consumer, NULL, consume, (void *)"consumer") != 0) {
    perror("pthread_create");
    return EXIT_FAILURE;
  }

  if (pthread_join(producer, NULL) != 0) {
    perror("pthread_join");
    return EXIT_FAILURE;
  }

  if (pthread_join(consumer, NULL) != 0) {
    perror("pthread_join");
    return EXIT_FAILURE;
  }

  if (pthread_mutex_destroy(&lock) != 0) {
    perror("pthread_mutex_destroy");
    return EXIT_FAILURE;
  }

  if (pthread_cond_destroy(&condition) != 0) {
    perror("pthread_join");
    return EXIT_FAILURE;
  }

  return 0;
}

void *produce(void *arg) {
  fputs("Type message >> ", stdout);
  fgets(buf, sizeof(buf), stdin);

  if (pthread_mutex_lock(&lock) != 0) {
    perror("pthread_mutex_lock");
    exit(EXIT_FAILURE);
  }

  ready = true;

  if (pthread_cond_signal(&condition) != 0) {
    perror("pthread_cond_signal");
    exit(EXIT_FAILURE);
  }

  if (pthread_mutex_unlock(&lock) != 0) {
    perror("pthread_mutex_lock");
    exit(EXIT_FAILURE);
  }

  return NULL;
}

void *consume(void *arg) {
  if (pthread_mutex_lock(&lock) != 0) {
    perror("pthread_mutex_lock");
    exit(EXIT_FAILURE);
  }

  while (!ready) {
    if (pthread_cond_wait(&condition, &lock) != 0) {
      perror("pthread_cond_wait");
    }
  }

  if (pthread_mutex_unlock(&lock) != 0) {
    perror("pthread_mutex_lock");
    exit(EXIT_FAILURE);
  }

  fprintf(stdout, "Received << %s", buf);

  return NULL;
}
