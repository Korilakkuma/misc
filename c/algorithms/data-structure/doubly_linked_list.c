#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define INT long long
#define MAX_VALUE 10LL

typedef enum {
  INSERT,
  UPDATE,
  DELETE
} CRUD;

struct Node {
  struct Node *prev;
  struct Node *next;
  INT key;
};

static void init_list(struct Node *head, struct Node *list, INT N);
static void free_list(struct Node *head, struct Node *list);

static void insert(struct Node *list, struct Node *target, INT key);
static void update(struct Node *list, INT key, INT new_key);
static void delete(struct Node *list, INT key);

static inline void dump(struct Node *list, char *label);

int main(int argc, char **argv) {
  if (argc < 4) {
    fputs("./doubly_linked_list N key 0|1|2 [new_key]\n", stderr);
    exit(EXIT_FAILURE);
  }

  INT N   = strtoll(argv[1], NULL, 10);
  INT key = strtoll(argv[2], NULL, 10);
  CRUD op = (CRUD)strtoll(argv[3], NULL, 10);

  struct Node *list = (struct Node *)malloc(sizeof(struct Node));
  struct Node *head = (struct Node *)malloc(sizeof(struct Node));

  init_list(head, list, N);

  dump(head->next, "Before Doubly Linked List:");

  switch (op) {
    case INSERT: {
      if (argc != 5) {
        fputs("Require inserted key\n", stderr);
        exit(EXIT_FAILURE);
      }

      struct Node *target = (struct Node *)malloc(sizeof(struct Node));

      target->prev = NULL;
      target->next = NULL;
      target->key  = strtoll(argv[4], NULL, 10);

      insert(head->next, target, key);
      break;
    }

    case UPDATE: {
      if (argc != 5) {
        fputs("Require updated key\n", stderr);
        exit(EXIT_FAILURE);
      }

      update(head->next, key, strtoll(argv[4], NULL, 10));
      break;
    }

    case DELETE: {
      delete(head->next, key);
      break;
    }

    default: {
      fputs("./doubly_linked_list N key 0|1|2 [new_key]\n", stderr);
      exit(EXIT_FAILURE);
    }
  }

  dump(head->next, "After Doubly Linked List :");

  free_list(head, head->next);

  return 0;
}

static void init_list(struct Node *head, struct Node *list, INT N) {
  head->next = list;

  list->prev = NULL;
  list->next = NULL;
  list->key  = (INT)((MAX_VALUE) * ((double)rand() / (RAND_MAX + 1.0)));

  srand((unsigned int)time(NULL));

  for (INT i = 1; i < N; i++) {
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));

    node->prev = list;
    node->key  = (INT)((MAX_VALUE) * ((double)rand() / (RAND_MAX + 1.0)));

    list->next = node;

    list = node;
  }

  list->next = NULL;
}

static void free_list(struct Node *head, struct Node *list) {
  while (list->next != NULL) {
    list = list->next;
    free(list->prev);
  }

  free(list);
  free(head);
}

static void insert(struct Node *list, struct Node *target, INT key) {
  while (list->next != NULL) {
    if (list->key == key) {
      struct Node *next = list->next;

      target->next = next;
      target->prev = list;

      next->prev = target;
      list->next = target;
      break;
    }

    list = list->next;
  }
}

static void update(struct Node *list, INT key, INT new_key) {
  while (list->next != NULL) {
    if (list->key == key) {
      list->key = new_key;
      break;
    }

    list = list->next;
  }
}

static void delete(struct Node *list, INT key) {
  while (list->next != NULL) {
    if (list->key == key) {
      struct Node *prev = list->prev;
      struct Node *next = list->next;

      prev->next = next;
      next->prev = prev;

      free(list);
      break;
    }

    list = list->next;
  }
}

static inline void dump(struct Node *list, char *label) {
  printf("%s\n", label);

  while (list->next != NULL) {
    if (list->prev == NULL) {
      printf("          NULL <- %lld (%p) -> %p\n", list->key, list, list->next);
    } else {
      printf("%p <- %lld (%p) -> %p\n", list->prev, list->key, list, list->next);
    }

    list = list->next;
  }

  printf("%p <- %lld (%p) -> NULL\n", list->prev, list->key, &list);
}
