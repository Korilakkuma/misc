#include "data.hpp"

struct UnionFind {
  std::vector<INT> sizes;
  std::vector<INT> parents;

  INT root(INT n) {
    if (parents[n] == -1) {
      return n;
    }

    parents[n] = root(parents[n]);

    return parents[n];
  }

  bool issame(INT x, INT y) {
    return root(x) == root(y);
  }

  bool unite(INT x, INT y) {
    if (issame(x, y)) {
      return false;
    }

    INT x_root = root(x);
    INT y_root = root(y);

    if (sizes[x_root] < sizes[y_root]) {
      INT tmp;

      tmp    = y_root;
      y_root = x_root;
      x_root = tmp;
    }

    parents[y_root] = x_root;

    sizes[x_root] += sizes[y_root];

    return true;
  }

  INT size_at(INT n) {
    return sizes[root(n)];
  }

  UnionFind(INT n) : sizes(n, 1), parents(n, -1) {}
};

int main(int argc, char **argv) {
  UnionFind union_find(7);

  union_find.unite(1, 2);
  union_find.unite(2, 3);
  union_find.unite(5, 6);

  std::cout << "issame(1, 3): " << (union_find.issame(1, 3) ? "true" : "false") << std::endl;
  std::cout << "issame(2, 5): " << (union_find.issame(2, 5) ? "true" : "false") << std::endl;

  union_find.unite(1, 6);

  std::cout << "issame(2, 5): " << (union_find.issame(2, 5) ? "true" : "false") << std::endl;

  INT number_of_connected_components = 0;

  for (INT i = 0; i < 7; i++) {
    if (union_find.root(i) == i) {
      ++number_of_connected_components;
    }
  }

  std::cout << "The number of connected components is " << number_of_connected_components << std::endl;

  return 0;
}
