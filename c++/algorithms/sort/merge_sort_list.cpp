#include "data.hpp"

struct Element {
  INT key;
  struct Element *prev;
  struct Element *next;
};

static struct Element *merge_sort_list(struct Element *list);
static struct Element *merge(struct Element *l, struct Element *r);

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "./merge_sort_list N" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  INT N = std::stoll(argv[1]);

  struct Element *head = nullptr;

  struct Element *prev = (struct Element *)malloc(1 * sizeof(struct Element));

  for (INT i = 0; i < N; i++) {
    struct Element *element = (struct Element *)malloc(1 * sizeof(struct Element));

    INT key = generate_random_value(10);

    element->key  = key;
    element->next = nullptr;

    if (i == 0) {
      head = element;
      element->prev = head;
    } else {
      prev->next = element;
      element->prev = prev;

      if (i == (N-1)) {
        head->prev = element;
      }
    }

    prev = element;
  }

  std::cout << "0: ";

  for (struct Element *e = head->next; e->next != nullptr; e = e->next) {
    std::cout << e->key << " ";
  }

  std::cout << std::endl;
  std::cout << std::endl;

  head = merge_sort_list(head);

  std::cout << std::endl;
  std::cout << N << ": ";

  for (struct Element *e = head->next; e->next != nullptr; e = e->next) {
    std::cout << e->key << " ";
  }

  std::cout << std::endl;

  return 0;
}

static struct Element *merge_sort_list(struct Element *list) {
  if ((list == nullptr) || (list->next == nullptr)) {
    return list;
  }

  struct Element *l = list;
  struct Element *r = list->next;

  if (r != nullptr) {
    r = r->next;
  }

  while (r != nullptr) {
    l = l->next;
    r = r->next;

    if (r != nullptr) {
      r = r->next;
    }
  }

  struct Element *middle = l->next;

  l->next = nullptr;

  return merge(merge_sort_list(list), merge_sort_list(middle));
}

static struct Element *merge(struct Element *l, struct Element *r) {
  struct Element head;

  struct Element *p = &head;

  while ((l != nullptr) && (r != nullptr)) {
    if (l->key <= r->key) {
      p->next = l;
      p = l;
      l = l->next;
    } else {
      p->next = r;
      p = r;
      r = r->next;
    }
  }

  if (l == nullptr) {
    p->next = r;
  }

  if (r == nullptr) {
    p->next = l;
  }

  std::cout << " : ";

  for (struct Element *e = head.next; e->next != nullptr; e = e->next) {
    std::cout << e->key << " ";
  }

  std::cout << std::endl;

  return head.next;
}
