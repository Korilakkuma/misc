#include "data.hpp"

static const INT BUCKET_SIZE = 100;

class Cell {
  private:
    INT key;
    INT data;
    Cell *next;

  public:
    Cell(INT key, INT data) : key(key), data(data) {}
    ~Cell();

    INT get_key() {
      return key;
    }

    INT get_data() {
      return data;
    }

    Cell *get_next() {
      return next;
    }

    void set_next(Cell *next_cell) {
      next = next_cell;
    }
};

static INT hash(INT key);
static bool equal_keys(INT a, INT b);
static void init_table(std::vector<Cell *> &hash_table);

static Cell *find(INT key);
static bool insert_into(INT key, INT data);
static bool delete_from(INT key);

std::vector<Cell *> hash_table(BUCKET_SIZE);

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "./hash_chain N key" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  init_table(hash_table);

  return 0;
}

static INT hash(INT key) {
  return key % BUCKET_SIZE;
}

static bool equal_keys(INT a, INT b) {
  return a == b;
}

static void init_table(std::vector<Cell *> &hash_table) {
  for (auto& e : hash_table) {
    e = nullptr;
  }
}

static Cell *find(INT key) {
  INT h = hash(key);

  Cell *p = hash_table[h];

  while (p != nullptr) {
    if (equal_keys(key, p->get_key())) {
      return p;
    }

    p = p->get_next();
  }

  return nullptr;
}

static bool insert_into(INT key, INT data) {
  if (find(key) != nullptr) {
    return false;
  }

  INT h = hash(key);

  Cell *p = new Cell(key, data);

  p->set_next(hash_table[h]);

  hash_table[h] = p;

  return true;
}

static bool delete_from(INT key) {
  INT h = hash(key);

  if (hash_table[h] == nullptr) {
    return false;
  }

  if (equal_keys(key, hash_table[h]->get_key())) {
    Cell *p = hash_table[h];

    hash_table[h] = p->get_next();

    delete p;

    return true;
  }

  Cell *current = new Cell(-1, -1);

  Cell *prev = hash_table[h];

  current->set_next(prev->get_next());

  while (current != nullptr) {
    if (equal_keys(key, current->get_key())) {
      prev->set_next(current->get_next());

      delete current;

      return true;
    }

    prev = current;
    current = current->get_next();
  }

  return false;
}
