#include "data.hpp"

enum HashStatus {
  EMPTY,
  DELETED
};

const INT BUCKET_SIZE = 100;

class Cell {
  private:
    INT key;
    INT data;

  public:
    Cell(INT key, INT data) : key(key), data(data) {}
    ~Cell();

    INT get_key() {
      return key;
    }

    INT get_data() {
      return data;
    }

    void set_key(INT newKey) {
      key = newKey;
    }
};

static INT hash(INT key);
static INT rehash(INT key);
static bool equal_keys(INT a, INT b);
static void init_table(std::vector<Cell *> &hash_table);

static Cell *find(INT key);
static bool insert_into(INT key, INT data);
static bool delete_from(INT key);

std::vector<Cell *> hash_table(BUCKET_SIZE);

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "./hash_open_addressing N key" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  init_table(hash_table);

  return 0;
}

static INT hash(INT key) {
  return key % BUCKET_SIZE;
}

static INT rehash(INT hashed_key) {
  return (hashed_key + 1) % BUCKET_SIZE;
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
  INT rehash_count = 0;

  INT k;

  while ((k = hash_table[h]->get_key()) != EMPTY) {
    if ((k != DELETED) && equal_keys(key, k)) {
      return hash_table[h];
    }

    if (++rehash_count > BUCKET_SIZE) {
      return nullptr;
    }

    h = rehash(h);
  }

  return nullptr;
}

static bool insert_into(INT key, INT data) {
  INT h = hash(key);
  INT rehash_count = 1;

  INT k;

  while (((k = hash_table[h]->get_key()) != EMPTY) && (k != DELETED)) {
    if (equal_keys(key, k)) {
      return false;
    }

    if (rehash_count++ > BUCKET_SIZE) {
      std::cerr << "hash table is overflow" << std::endl;
      exit(EXIT_FAILURE);
    }

    h = rehash(h);
  }

  hash_table[h] = new Cell(key, data);

  return true;
}

static bool delete_from(INT key) {
  INT h = hash(key);
  INT rehash_count = 1;

  INT k;

  while ((k = hash_table[h]->get_key()) != EMPTY) {
    if ((k != DELETED) && equal_keys(key, k)) {
      hash_table[h]->set_key(DELETED);
      return true;
    }

    if (rehash_count++ > BUCKET_SIZE) {
      return false;
    }

    h = rehash(h);
  }

  return false;
}
