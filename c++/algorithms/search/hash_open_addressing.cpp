#include "data.hpp"

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
    std::cerr << "./hash_open_addressing N key" << std::endl;
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
  return nullptr;
}

static bool insert_into(INT key, INT data) {
  return true;
}

static bool delete_from(INT key) {
  return true;
}
