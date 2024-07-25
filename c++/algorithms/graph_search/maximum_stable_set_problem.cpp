#include "data.hpp"

static void maximum_stable_set_problem(const Graph &graph, INT v, INT p);

static std::vector<std::vector<INT>> graph_source = {
  { 0, 1, 1 },
  { 0, 2, 2 },
  { 1, 3, 3 },
  { 1, 4, 4 }
};

std::vector<INT> weights;
std::vector<INT> dp1;
std::vector<INT> dp2;

int main(int argc, char **argv) {
  INT N = 5;
  INT M = N - 1;

  weights.resize(N);

  for (INT i = 0; i < M; i++) {
    weights[i] = graph_source[i][2];
  }

  Graph graph(N);

  for (INT e = 0; e < M; e++) {
    std::vector<INT> vertices = graph_source[e];

    graph[vertices[0]].push_back(vertices[1]);
    graph[vertices[1]].push_back(vertices[0]);
  }

  INT root = 0;

  dp1.assign(N, root);
  dp2.assign(N, root);

  maximum_stable_set_problem(graph, root, -1);

  std::cout << "maximum stable set is " << std::max(dp1[root], dp2[root]) << std::endl;

  return 0;
}

static void maximum_stable_set_problem(const Graph &graph, INT v, INT p) {
  for (INT child : graph[v]) {
    if (child == p) {
      continue;
    }

    maximum_stable_set_problem(graph, child, v);
  }

  dp1[v] = 0;
  dp2[v] = weights[v];

  for (INT child : graph[v]) {
    if (child == p) {
      continue;
    }

    dp1[v] += std::max(dp1[child], dp2[child]);
    dp2[v] += dp1[child];
  }
}
