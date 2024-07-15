#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

using INT   = long long;
using Edge  = std::pair<INT, std::pair<INT, INT>>;
using Graph = std::vector<Edge>;

static std::vector<std::vector<INT>> graph_source = {
  { 0, 1, 3 },
  { 0, 3, 100 },
  { 1, 2, 50 },
  { 1, 3, 57 },
  { 1, 4, 4 },
  { 2, 3, 10 },
  { 2, 4, 5 },
  { 3, 1, 5 },
  { 4, 2, 5 },
  { 4, 3, 25 },
  { 4, 5, 8 }
};

static INT kruskal(Graph &graph, INT N);

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
  INT N = 5;
  INT M = graph_source.size();

  Graph graph(M);

  for (INT e = 0; e < M; e++) {
    std::vector<INT> ve = graph_source[e];

    graph[e] = Edge(ve[2], std::make_pair(ve[0], ve[1]));
  }

  INT min = kruskal(graph, N);

  std::cout << "minimum spanning tree is " << min << std::endl;

  return 0;
}

static INT kruskal(Graph &graph, INT N) {
  UnionFind union_find(N);

  std::sort(graph.begin(), graph.end());

  INT min = 0;

  for (INT e = 0; e < graph.size(); e++) {
    Edge edge = graph[e];

    INT w = edge.first;
    INT u = edge.second.first;
    INT v = edge.second.second;

    if (union_find.issame(u, v)) {
      continue;
    }

    min += w;

    union_find.unite(u, v);
  }

  return min;
}
