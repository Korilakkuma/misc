#include "data.hpp"

class Node {
  private:
    INT data;
    Node *left;
    Node *right;

  public:
    Node(INT data, Node *left, Node *right) : data(data), left(left), right(right) {}
    ~Node();

    INT get_data() {
      return data;
    }

    Node *get_left() {
      return left;
    }

    Node *get_right() {
      return right;
    }

    void set_left(Node *new_left) {
      left = new_left;
    }

    void set_right(Node *new_right) {
      right = new_right;
    }
};

static bool equal_keys(INT a, INT b);

static Node *search(INT key);
static Node *insert_into(INT key);
static bool delete_from(INT key);
static Node *find_min_node_from_right_partial_tree(Node **node);

Node *root = nullptr;

int main(int argc, char **argv) {
  if (argc != 1) {
    std::cerr << "./binary_search_tree" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  return 0;
}

static bool equal_keys(INT a, INT b) {
  return a == b;
}

static Node *search(INT key) {
  Node *p;

  p = root;

  while (p != nullptr) {
    if (equal_keys(key, p->get_data())) {
      return p;
    }

    if (key < p->get_data()) {
      p = p->get_left();
    } else {
      p = p->get_right();
    }
  }

  return nullptr;
}

static Node *insert_into(INT key) {
  Node **p = &root;

  while (p != nullptr) {
    if (equal_keys(key, (*p)->get_data())) {
      return nullptr;
    }

    if (key < (*p)->get_data()) {
      *p = ((*p)->get_left());
    } else {
      *p = ((*p)->get_right());
    }
  }

  Node *node = new Node(key, nullptr, nullptr);

  *p = node;

  return node;
}

static bool delete_from(INT key) {
  Node **p = &root;

  while (p != nullptr) {
    if (equal_keys(key, (*p)->get_data())) {
      if (((*p)->get_left() == nullptr) && ((*p)->get_right() == nullptr)) {
        delete *p;
      } else if (((*p)->get_left() == nullptr)) {
        *p = (*p)->get_right();
      } else if (((*p)->get_right() == nullptr)) {
        *p = (*p)->get_left();
      } else {
        Node *r = (*p)->get_right();

        *p = find_min_node_from_right_partial_tree(&r);

        (*p)->set_left(r->get_left());
        (*p)->set_right(r->get_right());

        delete r;
      }

      return true;
    }

    if (key < (*p)->get_data()) {
      *p = ((*p)->get_left());
    } else {
      *p = ((*p)->get_right());
    }
  }

  return false;
}

static Node *find_min_node_from_right_partial_tree(Node **node) {
  while ((*node)->get_left() != nullptr) {
    *node = (*node)->get_left();
  }

  Node *min = *node;

  *node = (*node)->get_right();

  return min;
}
