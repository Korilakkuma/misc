#include "data.hpp"

const INT NUMBER_OF_CHILD = 5;
const INT HALF = ((NUMBER_OF_CHILD + 1) / 2);

enum Type {
  INTERNAL = 1,
  LEAF
};

struct Node {
  Type type;

  union {
    struct {
      INT number_of_child;
      struct Node *children[NUMBER_OF_CHILD];
      INT low_keys[NUMBER_OF_CHILD];
    } internal;

    struct {
      INT key;
      INT data;
    } leaf;
  } body;
};

enum Operator {
  INSERT = '+',
  DELETE = '-'
};

enum DeleteResult {
  OK = 1,
  REMOVED,
  NEED_MERGE
};

static struct Node *search(INT key);
static struct Node *insert_into(INT key);
static bool delete_from(INT key);

static struct Node *insert_auxiliary(struct Node **parent_node, INT key, struct Node **new_node, INT *lowest_key);
static bool delete_auxiliary(struct Node *node, INT key, DeleteResult *result);
static bool merge_nodes(struct Node *node, INT index);

static struct Node *alloc_node();
static bool equal_keys(INT a, INT b);
static bool less_key(INT a, INT b);
static INT locate_subtree(Node *node, INT key);
static void print_tree(struct Node *node);
static void handle_error(std::string message);

Node *root = nullptr;

int main(int argc, char **argv) {
  if (argc != 1) {
    std::cerr << "./b_tree" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  char op[100];

  print_tree(root);
  std::cout << " (+|-)?key > ";

  while (std::cin && std::cin >> op) {
    if (op[0] == 'q') {
      break;
    }

    INT k = std::abs(std::stoll(op));

    switch (op[0]) {
      case Operator::INSERT: {
        if (insert_into(k) != nullptr) {
          std::cout << k << " inserted" << std::endl;
        } else {
          std::cout << k << " not inserted" << std::endl;
        }

        break;
      }

      case Operator::DELETE: {
        if (delete_from(k)) {
          std::cout << k << " deleted" << std::endl;
        } else {
          std::cout << k << " not deleted" << std::endl;
        }

        break;
      }

      default: {
        if (search(k) != nullptr) {
          std::cout << k << " found" << std::endl;
        } else {
          std::cout << k << " not found" << std::endl;
        }

        break;
      }
    }

    print_tree(root);
    std::cout << " (+|-)?key > ";
  }

  return 0;
}

static struct Node *search(INT key) {
  if (root == nullptr) {
    return nullptr;
  }

  struct Node *p = root;

  while (p->type == Type::INTERNAL) {
    INT i = locate_subtree(p, key);

    p = p->body.internal.children[i];
  }

  if (equal_keys(key, p->body.leaf.key)) {
    return p;
  }

  return nullptr;
}

static struct Node *insert_into(INT key) {
  if (root == nullptr) {
    root = alloc_node();

    root->type = Type::LEAF;
    root->body.leaf.key = key;

    return root;
  }

  struct Node *new_node;
  INT lowest_key;

  struct Node *inserted_node = insert_auxiliary(&root, key, &new_node, &lowest_key);

  if (new_node != nullptr) {
    struct Node *n = alloc_node();

    n->type = Type::INTERNAL;

    n->body.internal.number_of_child = 2;
    n->body.internal.children[0]     = root;
    n->body.internal.children[1]     = new_node;
    n->body.internal.low_keys[1]     = lowest_key;

    root = n;
  }

  return inserted_node;
}

static struct Node *insert_auxiliary(struct Node **parent_node, INT key, struct Node **new_node, INT *lowest_key) {
  struct Node *node = *parent_node;

  *new_node = nullptr;

  if (node->type == Type::LEAF) {
    if (equal_keys(key, node->body.leaf.key)) {
      return nullptr;
    }

    struct Node *n = alloc_node();

    n->type = Type::LEAF;
    n->body.leaf.key = key;

    if (less_key(key, node->body.leaf.key)) {
      *parent_node = n;

      *lowest_key = node->body.leaf.key;
      *new_node   = node;
    } else {
      *lowest_key = key;
      *new_node   = n;
    }

    return n;
  }

  INT position = locate_subtree(node, key);

  struct Node *internal_new_node;
  INT internal_lowest_key;

  struct Node *inserted_node = insert_auxiliary(&(node->body.internal.children[position]), key, &internal_new_node, &internal_lowest_key);

  if (internal_new_node == nullptr) {
    return inserted_node;
  }

  if (node->body.internal.number_of_child < NUMBER_OF_CHILD) {
    for (INT i = (node->body.internal.number_of_child - 1); i > position; i--) {
      node->body.internal.children[i + 1] = node->body.internal.children[i];
      node->body.internal.low_keys[i + 1] = node->body.internal.low_keys[i];
    }

    node->body.internal.children[position + 1] = internal_new_node;
    node->body.internal.low_keys[position + 1] = internal_lowest_key;
    ++node->body.internal.number_of_child;

    return inserted_node;
  }

  struct Node *n = alloc_node();

  n->type = Type::INTERNAL;

  if (position < (HALF - 1)) {
    for (INT i = (HALF - 1), j = 0; i < NUMBER_OF_CHILD; i++, j++) {
      n->body.internal.children[j] = node->body.internal.children[i];
      n->body.internal.low_keys[j] = node->body.internal.low_keys[i];
    }

    for (INT i = (HALF - 2); i > position; i--) {
      node->body.internal.children[i + 1] = node->body.internal.children[i];
      node->body.internal.low_keys[i + 1] = node->body.internal.low_keys[i];
    }

    node->body.internal.children[position + 1] = internal_new_node;
    node->body.internal.low_keys[position + 1] = internal_lowest_key;
  } else {
    INT j = NUMBER_OF_CHILD - HALF;

    for (INT i = (NUMBER_OF_CHILD - 1); i >= HALF; i--) {
      if (position == i) {
        n->body.internal.children[j] = internal_new_node;
        n->body.internal.low_keys[j] = internal_lowest_key;

        --j;
      }

      n->body.internal.children[j] = node->body.internal.children[i];
      n->body.internal.low_keys[j] = node->body.internal.low_keys[i];

      --j;
    }

    if (position < HALF) {
      n->body.internal.children[0] = internal_new_node;
      n->body.internal.low_keys[0] = internal_lowest_key;
    }

    node->body.internal.number_of_child = HALF;
    n->body.internal.number_of_child    = (NUMBER_OF_CHILD + 1) - HALF;

    *new_node   = n;
    *lowest_key = n->body.internal.low_keys[0];
  }

  return inserted_node;
}

static bool delete_from(INT key) {
  if (root == nullptr) {
    return false;
  }

  DeleteResult result;

  bool deleted_result = delete_auxiliary(root, key, &result);

  if (result == DeleteResult::REMOVED) {
    root = nullptr;
  } else if ((result == DeleteResult::NEED_MERGE) && (root->body.internal.number_of_child == 1)) {
    struct Node *p = root;

    root = root->body.internal.children[0];

    free(p);
  }

  return deleted_result;
}

static bool delete_auxiliary(struct Node *node, INT key, DeleteResult *result) {
  *result = DeleteResult::OK;

  if (node->type == Type::LEAF) {
    if (equal_keys(key, node->body.leaf.key)) {
      *result = DeleteResult::REMOVED;

      free(node);

      return true;
    }

    return false;
  }

  INT position = locate_subtree(node, key);

  DeleteResult internal_result;

  bool deleted = delete_auxiliary(node->body.internal.children[position], key, &internal_result);

  if (internal_result == DeleteResult::OK) {
    return deleted;
  }

  if (internal_result == DeleteResult::NEED_MERGE) {
    INT subtree_position = (position == 0) ? 0 : (position - 1);

    bool merged = merge_nodes(node, subtree_position);

    if (merged) {
      position = subtree_position + 1;
    }

    if (merged || (internal_result == DeleteResult::REMOVED)) {
      for (INT i = position; i < (node->body.internal.number_of_child - 1); i++) {
        node->body.internal.children[i] = node->body.internal.children[i + 1];
        node->body.internal.low_keys[i] = node->body.internal.low_keys[i + 1];
      }

      if (--node->body.internal.number_of_child < HALF) {
        *result = DeleteResult::NEED_MERGE;
      }
    }
  }

  return deleted;
}

static bool merge_nodes(struct Node *node, INT index) {
  struct Node *current = node->body.internal.children[index];
  struct Node *next    = node->body.internal.children[index + 1];

  next->body.internal.low_keys[0] = node->body.internal.low_keys[index + 1];

  INT current_number_of_child = current->body.internal.number_of_child;
  INT next_number_of_child    = next->body.internal.number_of_child;

  if ((current_number_of_child + next_number_of_child) <= NUMBER_OF_CHILD) {
    for (INT i = 0; i < next_number_of_child; i++) {
      current->body.internal.children[i + current_number_of_child] = next->body.internal.children[i];
      current->body.internal.low_keys[i + current_number_of_child] = next->body.internal.low_keys[i];
    }

    current->body.internal.number_of_child += next_number_of_child;

    free(next);

    return true;
  }

  INT n = (current_number_of_child + next_number_of_child) / 2;

  if (current_number_of_child > n) {
    INT move = current_number_of_child - n;

    for (INT i = (next_number_of_child - 1); i >= 0; i--) {
      next->body.internal.children[move + i] = next->body.internal.children[i];
      next->body.internal.low_keys[move + i] = next->body.internal.low_keys[i];
    }

    for (INT i = 0; i < move; i++) {
      next->body.internal.children[i] = current->body.internal.children[i + n];
      next->body.internal.low_keys[i] = current->body.internal.low_keys[i + n];
    }
  } else {
    INT move = n - current_number_of_child;

    for (INT i = 0; i < move; i++) {
      current->body.internal.children[current_number_of_child + i] = next->body.internal.children[i];
      current->body.internal.low_keys[current_number_of_child + i] = next->body.internal.low_keys[i];
    }

    for (INT i = 0; i < (next_number_of_child - move); i++) {
      next->body.internal.children[i] = next->body.internal.children[move + i];
      next->body.internal.low_keys[i] = next->body.internal.low_keys[move + i];
    }
  }

  current->body.internal.number_of_child = n;
  next->body.internal.number_of_child    = current_number_of_child + next_number_of_child - n;

  node->body.internal.low_keys[index + 1] = next->body.internal.low_keys[0];

  return false;
}

static struct Node *alloc_node() {
  struct Node *node;

  if ((node = (struct Node *)malloc(1 * sizeof(struct Node))) == nullptr) {
    handle_error("memory leak");
  }

  return node;
}

static bool equal_keys(INT a, INT b) {
  return a == b;
}

static bool less_key(INT a, INT b) {
  return a < b;
}

static INT locate_subtree(Node *node, INT key) {
  for (INT i = (node->body.internal.number_of_child - 1); i > 0; i--) {
    if (node->body.internal.low_keys[i] <= key) {
      return i;
    }
  }

  return 0;
}

static void print_tree(struct Node *node) {
  if (node == nullptr) {
    return;
  }

  if (node->type == Type::LEAF) {
    fprintf(stdout, "%04lx Leaf value = %lld\n", (uintptr_t)node, node->body.leaf.key);
  } else {
    fprintf(stdout, "%04lx %02lld [%04lx] %lld[%04lx] %lld[%04lx] %lld[%04lx] %lld[%04lx]\n",
        (uintptr_t)node, node->body.internal.number_of_child, (uintptr_t)node->body.internal.children[0],
        node->body.internal.low_keys[1], (uintptr_t)node->body.internal.children[1],
        node->body.internal.low_keys[2], (uintptr_t)node->body.internal.children[2],
        node->body.internal.low_keys[3], (uintptr_t)node->body.internal.children[3],
        node->body.internal.low_keys[4], (uintptr_t)node->body.internal.children[4]);

    for (INT i = 0; i < node->body.internal.number_of_child; i++) {
      print_tree(node->body.internal.children[i]);
    }
  }
}

static void handle_error(std::string message) {
  std::cerr << message << std::endl;
  std::exit(EXIT_FAILURE);
}
