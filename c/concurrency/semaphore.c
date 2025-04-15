#include <stdbool.h>

#define INT long long

#define NUMBER_OF_SEMAPHORE 5

INT shared_count = 0;

void aqcuire_semaphore(volatile INT *count) {
  while (true) {
    while (*count >= NUMBER_OF_SEMAPHORE);

    __sync_fetch_and_add(count, 1);

    if (*count <= NUMBER_OF_SEMAPHORE) {
      break;
    }

    __sync_fetch_and_sub(count, 1);
  }
}

void release_semaphore(volatile INT *count) {
  __sync_fetch_and_sub(count, 1);
};

void semaphore() {
  while (true) {
    aqcuire_semaphore(&shared_count);

    // Critical Section

    release_semaphore(&shared_count);
  }
}
