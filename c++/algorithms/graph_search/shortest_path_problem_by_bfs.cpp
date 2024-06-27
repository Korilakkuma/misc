#include "data.hpp"

static std::vector<INT> shortest_path_problem_by_bfs(const Graph &graph);

static std::vector<std::vector<INT>> graph_source = {
  { 1, 2 },
  { 0, 4 },
  { 0, 5 },
  { 1, 8 },
  { 0, 8 },
  { 1, 8 },
  { 2, 8 },
  { 5, 7 },
  { 3, 4 },
};

int main(int argc, char **argv) {
  INT N = graph_source.size();

  Graph graph(N);

  for (INT i = 0; i < N; i++) {
    std::vector<INT> vertices = graph_source[i];

    graph[vertices[0]].push_back(vertices[1]);
    graph[vertices[1]].push_back(vertices[0]);
  }

  std::vector<INT> dists = shortest_path_problem_by_bfs(graph);

  for (INT v = 0; v < dists.size(); v++) {
    std::cout << "vertex[" << v << "]: "<< dists[v] << std::endl;
  }

  return 0;
}

static std::vector<INT> shortest_path_problem_by_bfs(const Graph &graph) {
  INT N = graph.size();

  std::vector<INT> dists(N, -1);
  std::queue<INT> queue;

  dists[0] = 0;
  queue.push(0);

  while (!queue.empty()) {
    INT v = queue.front();

    queue.pop();

    for (INT next : graph[v]) {
      if (dists[next] != -1) {
        continue;
      }

      dists[next] = dists[v] + 1;
      queue.push(next);
    }
  }

  return dists;
}
