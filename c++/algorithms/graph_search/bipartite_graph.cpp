#include "data.hpp"

static bool bipartite_graph(const Graph &graph, INT v, INT current_color);

static std::vector<std::vector<INT>> graph_source = {
  { 1, 3 },
  { 0, 2 },
  { 0, 4 },
  { 1, 3 }
};

std::vector<INT> colors;

int main(int argc, char **argv) {
  INT N = graph_source.size();

  Graph graph(N);

  for (INT i = 0; i < N; i++) {
    std::vector<INT> vertices = graph_source[i];

    graph[vertices[0]].push_back(vertices[1]);
    graph[vertices[1]].push_back(vertices[0]);
  }

  bool is_bipartite_graph = true;

  colors.assign(graph.size(), -1);

  for (INT v = 0; v < graph.size(); v++) {
    if (colors[v] != -1) {
      continue;
    }

    if (!bipartite_graph(graph, v, 0)) {
      is_bipartite_graph = false;
    }
  }

  if (is_bipartite_graph) {
    std::cout << "This graph is Bipartite graph" << std::endl;
  } else {
    std::cout << "This graph is not Bipartite graph" << std::endl;
  }
}

static bool bipartite_graph(const Graph &graph, INT v, INT current_color = 0) {
  colors[v] = current_color;

  for (INT next_v : graph[v]) {
    if (colors[next_v] != -1) {
      if (colors[next_v] == current_color) {
        return false;
      }

      continue;
    }

    if (!bipartite_graph(graph, next_v, (1 - current_color))) {
      return false;
    }
  }

  return true;
}
