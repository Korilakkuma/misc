#include "data.hpp"

static void topological_sort(const Graph &graph, INT v);

static std::vector<std::vector<INT>> graph_source = {
  { 5 },
  { 3, 6 },
  { 5, 7 },
  { 0, 7 },
  { 1, 2, 6 },
  { },
  { 7 },
};

std::vector<INT> sorted_vertives(graph_source.size());
std::vector<bool> seen(graph_source.size(), false);

int main(int argc, char **argv) {
  INT N = graph_source.size();

  for (INT v = 0; v < N; v++) {
    std::vector<INT> vertices = graph_source[v];

    if (vertices.size() == 2) {
      graph[vertices[0]].push_back(vertices[1]);
    } else if (vertices.size() == 1) {
      graph[vertices[0]] = {};
    }
  }

  sorted_vertives.clear();

  for (INT v = 0; v < N; v++) {
    if (seen[v]) {
      continue;
    }

    topological_sort(graph, v);
  }

  std::reverse(sorted_vertives.begin(), sorted_vertives.end());

  for (INT v : sorted_vertives) {
    std::cout << v << " -> ";
  }

  std::cout << std::endl;

  return 0;
}

static void topological_sort(const Graph &graph, INT v) {
  seen[v] = true;

  for (INT next_v : graph[v]) {
    if (seen[next_v]) {
      continue;
    }

    topological_sort(graph, next_v);
  }

  sorted_vertives.push_back(v);
}
