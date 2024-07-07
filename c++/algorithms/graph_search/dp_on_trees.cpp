#include "data.hpp"

static void dynamic_programming_on_trees(const Graph &graph, INT v, INT p, INT d);

static std::vector<std::vector<INT>> graph_source = {
  {  0,  1 },
  {  1,  2 },
  {  3,  4 },
  {  5,  6 },
  {  7,  8 },
  {  9, 10 },
  { 11, 12 },
  { 13, 14 }
};

std::vector<INT> depthes;
std::vector<INT> subtree_sizes;

int main(int argc, char **argv) {
  INT M = graph_source.size();
  INT N = M + 1;

  Graph graph(N);

  for (INT v = 0; v < M; v++) {
    std::vector<INT> vertices = graph_source[v];

    graph[vertices[0]].push_back(vertices[1]);
    graph[vertices[1]].push_back(vertices[0]);
  }

  for (INT v = 0; v < M; v++) {
    std::cout << v << ": " << graph[v][0] << ", " << graph[v][1] << std::endl;
  }

  std::cout << std::endl;

  INT root = 0;

  depthes.assign(N, 0);
  subtree_sizes.assign(N, 0);

  dynamic_programming_on_trees(graph, root, -1, 0);

  for (INT v = 0; v < M; v++) {
    std::cout << v << ": depth = " << depthes[v] << ", subtree size = " << subtree_sizes[v] << std::endl;
  }

  return 0;
}

static void dynamic_programming_on_trees(const Graph &graph, INT v, INT p = -1, INT d = 0) {
  depthes[v] = d;

  for (INT next_v : graph[v]) {
    if (next_v == p) {
      continue;
    }

    dynamic_programming_on_trees(graph, next_v, v, (d + 1));
  }

  subtree_sizes[v] = 1;

  for (INT next_v : graph[v]) {
    if (next_v == p) {
      continue;
    }

    subtree_sizes[v] += subtree_sizes[next_v];
  }
}
